//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_SELECTVISITOR_H
#define SELSQL_SELECTVISITOR_H
#include <utility>

#include "../Nodes/ActionNodes/SelectNode.h"
#include "../Nodes/ColumnNode.h"
#include "../Nodes/ColumnsAndExprNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/AddNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/ArithmeticNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/DivNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/MultNode.h"
#include "../Nodes/ExpressionsNodes/ArithmeticNodes/SubNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/EqualsNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/LessEqNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/LessNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/MoreEqNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/MoreNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/NoEqualsNode.h"
#include "../Nodes/ExpressionsNodes/ExprNode.h"
#include "../Nodes/ExpressionsNodes/IndentExprNode.h"
#include "../Nodes/ExpressionsNodes/IndentNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/AndLogicNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/NotLogicNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/OrLogicNode.h"
#include "../Nodes/ExpressionsNodes/ValueExprNode.h"
#include "../Nodes/JoinNodes/FullJoinNode.h"
#include "../Nodes/JoinNodes/JoinNode.h"
#include "../Nodes/JoinNodes/LeftJoinNode.h"
#include "../Nodes/JoinNodes/RightJoinNode.h"
#include "../Nodes/JoinNodes/SourceJoinNode.h"
#include "../Nodes/SystemTimeAllNode.h"
#include "../Nodes/SystemTimeNode.h"
#include "../Nodes/TableNode.h"
#include "TreeVisitor.h"
typedef std::vector<RecordsFull> JoinRecord;
class SelectVisitor : public TreeVisitor {
   public:
    int64_t tr_id = 0;
    int flag_system_time = 0;
    explicit SelectVisitor(std::shared_ptr<MainEngine> _engine) : TreeVisitor(std::move(_engine)){};

    void visit(SelectNode* node) override {
        tr_id = node->getId();
        allrecords.clear();
        columns.clear();
        node->getChild()->accept(this);
        auto systemTime = node->getSystemTimeNode();
        if (systemTime != nullptr) {
            systemTime->accept(this);
        }
        source = node->getSource();
        std::cout << node->getId() << std::endl;
    }

    void visit(ColumnsAndExprNode* node) override {
        for (auto& col : node->getColumns()) {
            col->accept(this);
        }
        expr = node->getExpr();
    }

    void visit(TableNode* node) override {
        node->getChild()->accept(this);
        tableName = std::move(curValue);
    }

    void visit(SystemTimeNode* node) override {
        startTime = node->getPeriodA();
        finishTIme = node->getPeriodB();
        flag_system_time = 1;
    }

    void visit(SystemTimeAllNode* node) override {
        auto date = "21-01-1999 8:30:21";
        std::tm tm{};
        strptime(date, "%d-%m-%Y %H:%M:%S", &tm);
        startTime = 0;
        //        auto s_time = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        //        std::memcpy(&startTime, &s_time, sizeof(s_time));
        finishTIme = INT64_MAX;
    }

    void visit(IdentNode* node) override { curValue = node->getBaseValue(); }

    void visit(ColumnNode* node) override {
        if (node->getAlias() == nullptr) {
            columns.emplace_back(std::make_pair("", node->getColumn()->getBaseValue()));
        } else {
            columns.emplace_back(std::make_pair(node->getAlias()->getBaseValue(), node->getColumn()->getBaseValue()));
        }
    }

    void visit(SourceJoinNode* node) override {
        node->getSource()->accept(this);

        if (!curValue.empty()) {
            auto _tableName = std::move(curValue);
            std::string alias = _tableName;
            if (node->getAlias() != nullptr) {
                node->getAlias()->accept(this);
                alias = std::move(curValue);
            }

            auto cursor = getEngine()->GetCursor(_tableName, tr_id);
            if (cursor.first->name.empty()) {
                message = Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
                return;
            } else {
                std::vector<std::pair<std::string, std::string>> columnValues;

                for (auto& col : columns) {
                    if (col.first == alias) {
                        columnValues.emplace_back(std::make_pair(col.second, ""));
                    }
                }
                message = ActionsUtils::checkFieldsExist(cursor.first, columnValues);
                if (message.getErrorCode()) {
                    return;
                }
                allrecords.emplace_back(addRecord(alias, cursor.second));
            }
            std::cerr << _tableName << std::endl;
        }
    }

    std::vector<RecordsFull> addRecord(const std::string& aliasName, std::shared_ptr<Cursor> cursor) {
        std::vector<RecordsFull> records;
        do {
            auto _record = cursor->Fetch();
            if (_record.first.empty()) {
                continue;
            }
            RecordsFull _newRecord;
            for (auto& col : _record.first) {
                _newRecord.emplace_back(std::make_pair(std::make_pair(aliasName, col.first), col.second));
            }
            records.emplace_back(_newRecord);
        } while (!cursor->NextRecord());
        return records;
    }

    void startExecuteJoin(BaseJoinNode* node) {
        node->getFirstSource()->accept(this);
        if (message.getErrorCode()) {
            return;
        }
        node->getSecondSource()->accept(this);
        if (message.getErrorCode()) {
            return;
        }
        secondRecords = allrecords.back();
        allrecords.pop_back();
        firstRecords = allrecords.back();
        allrecords.pop_back();
    }

    void endExecuteJoin() {
        allrecords.emplace_back(records);
        records.clear();
        firstRecords.clear();
        secondRecords.clear();
    }

    int sideJoin(const RecordsFull& firstRec, const RecordsFull& secondRec, const RecordsFull& firstJoinRec,
                 BaseJoinNode* node) {
        auto flag = 0;
        auto joinRecords = firstRec;
        expressionVisitor->setSecondValues(secondRec);
        node->getExpr()->accept(expressionVisitor);
        if (expressionVisitor->getMessage().getErrorCode()) {
            message = expressionVisitor->getMessage();
            return -1;
        }
        if (expressionVisitor->getResult()) {
            joinRecords.insert(joinRecords.end(), firstJoinRec.begin(), firstJoinRec.end());
            records.emplace_back(joinRecords);
            flag = 1;
        }
        return flag;
    }

    void visit(LeftJoinNode* node) override {
        startExecuteJoin(node);
        for (auto& first : firstRecords) {
            expressionVisitor->setFirstValues(first);
            auto flag = 0;
            for (auto& second : secondRecords) {
                auto temp = sideJoin(first, second, second, node);
                if (temp == -1) {
                    return;
                }
                if (temp) {
                    flag = temp;
                }
            }
            if (!flag) {
                auto joinRecords = first;
                for (auto& rec : secondRecords[0]) {
                    auto tempRec = rec;
                    tempRec.second = "null";
                    joinRecords.emplace_back(tempRec);
                }
                records.emplace_back(joinRecords);
            }
        }

        endExecuteJoin();
    }

    void visit(RightJoinNode* node) override {
        startExecuteJoin(node);
        // records.clear();
        for (auto& first : secondRecords) {
            expressionVisitor->setFirstValues(first);
            auto flag = 0;
            for (auto& second : firstRecords) {
                auto temp = sideJoin(second, second, first, node);
                if (temp == -1) {
                    return;
                }
                if (temp) {
                    flag = temp;
                }
            }
            if (!flag) {
                auto joinRecords = firstRecords[0];
                for (auto& joinRec : joinRecords) {
                    joinRec.second = "null";
                }
                for (auto& rec : first) {
                    joinRecords.emplace_back(rec);
                }
                records.emplace_back(joinRecords);
            }
        }
        endExecuteJoin();
    }

    void visit(FullJoinNode* node) override {
        startExecuteJoin(node);
        // records.clear();
        for (auto& first : firstRecords) {
            expressionVisitor->setFirstValues(first);
            auto flag = 0;
            for (auto& second : secondRecords) {
                auto temp = sideJoin(first, second, second, node);
                if (temp == -1) {
                    return;
                }
                if (temp) {
                    flag = temp;
                }
            }
            if (!flag) {
                auto joinRecords = first;
                for (auto& rec : secondRecords[0]) {
                    auto tempRec = rec;
                    tempRec.second = "null";
                    joinRecords.emplace_back(tempRec);
                }
                records.emplace_back(joinRecords);
            }
        }

        for (auto& first : secondRecords) {
            expressionVisitor->setFirstValues(first);
            auto flag = 0;
            for (auto& second : firstRecords) {
                auto temp = sideJoin(second, second, first, node);
                if (temp == -1) {
                    return;
                }
                if (temp) {
                    flag = temp;
                }
            }
            if (!flag) {
                auto joinRecords = firstRecords[0];
                for (auto& joinRec : joinRecords) {
                    joinRec.second = "null";
                }
                for (auto& rec : first) {
                    joinRecords.emplace_back(rec);
                }
                records.emplace_back(joinRecords);
            }
        }

        for (auto i = 0; i < records.size(); i++) {
            for (auto j = 0; j < records.size(); ++j) {
                if (i == j) {
                    continue;
                }
                if (records[i] == records[j]) {
                    records.erase(records.begin() + j);
                }
            }
        }
        endExecuteJoin();
    }

    void visit(JoinNode* node) override {
        startExecuteJoin(node);
        node->getExpr()->accept(this);
        if (countEq == 2) {
            hashJoin(node);
            countEq = 0;
            curExpr.clear();
        } else {
            nestedLoopsJoin(node);
        }
        endExecuteJoin();
    }

    void visit(ExprNode* node) override { node->getChild()->accept(this); }

    void visit(IndentExprNode* node) override {
        countEq++;
        curExpr.emplace_back(node->getAliasName(), node->getName());
    }

    void visit(EqualsNode* node) override {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    static bool compareForHash(RecordColumn& val) {
        auto curName = std::find_if(curExpr.begin(), curExpr.end(),
                                    [val](const std::pair<std::string, std::string>& value) {
                                        return value.second == val.first.second;
                                    });
        if (curName == curExpr.end()) {
            return false;
        }

        for (int i = 0; i < curExpr.size(); i++) {
            auto exp = curExpr[i];
            if (exp.first == val.first.first || exp.first.empty()) {
                if (val.first.second == exp.second) {
                    id = i;
                    return true;
                }
            }
        }

        return false;
    }

    void addRec(JoinRecord small, JoinRecord large, std::unordered_map<std::string, RecordsFull> vals) {
        for (auto& rec : small) {
            auto ident = std::find_if(rec.begin(), rec.end(), compareForHash);
            auto record = vals.find(ident->second);
            if (record != vals.end()) {
                if (record->second == rec) {
                    for (auto& newrecord : large) {
                        auto joinRecords = rec;
                        joinRecords.insert(joinRecords.end(), newrecord.begin(), newrecord.end());
                        records.emplace_back(joinRecords);
                    }
                } else {
                    auto joinRecords = rec;
                    joinRecords.insert(joinRecords.end(), record->second.begin(), record->second.end());
                    records.emplace_back(joinRecords);
                }
            }
        }
        if (id >= 0) {
            curExpr.erase(curExpr.begin() + id);
        }
    }

    void hashJoin(JoinNode* node) {
        std::unordered_map<std::string, RecordsFull> vals;

        auto large = firstRecords;
        auto small = secondRecords;
        if (firstRecords.size() < secondRecords.size()) {
            large = secondRecords;
            small = firstRecords;
        }

        if (!small.empty() && !large.empty()) {
            auto ident = std::find_if(small[0].begin(), small[0].end(), compareForHash);
            if (ident == small[0].end()) {
                ident = std::find_if(large[0].begin(), large[0].end(), compareForHash);
                if (ident == large[0].end()) {
                    message = Message(ErrorConstants::ERR_NO_SUCH_FIELD);
                    return;
                } else {
                    for (auto& rec : large) {
                        ident = std::find_if(rec.begin(), rec.end(), compareForHash);
                        vals.emplace(std::make_pair(ident->second, rec));
                    }
                    if (id >= 0) {
                        curExpr.erase(curExpr.begin() + id);
                    }
                }
            } else {
                for (auto& rec : small) {
                    ident = std::find_if(rec.begin(), rec.end(), compareForHash);
                    vals.emplace(std::make_pair(ident->second, rec));
                }
                if (id >= 0) {
                    curExpr.erase(curExpr.begin() + id);
                }
            }

            ident = std::find_if(large[0].begin(), large[0].end(), compareForHash);
            if (ident == large[0].end()) {
                ident = std::find_if(small[0].begin(), small[0].end(), compareForHash);
                if (ident == small[0].end()) {
                    //                    this->getEngine()->Commit(tr_id);
                    message = Message(ErrorConstants::ERR_NO_SUCH_FIELD);
                    return;
                } else {
                    addRec(small, large, vals);
                }
            } else {
                addRec(large, small, vals);
            }
        }
    }

    void nestedLoopsJoin(JoinNode* node) {
        records.clear();
        for (auto& first : firstRecords) {
            expressionVisitor->setFirstValues(first);
            for (auto& second : secondRecords) {
                auto joinRecords = first;
                expressionVisitor->setSecondValues(second);
                node->getExpr()->accept(expressionVisitor);
                if (expressionVisitor->getMessage().getErrorCode()) {
                    message = expressionVisitor->getMessage();
                    return;
                }
                if (expressionVisitor->getResult()) {
                    joinRecords.insert(joinRecords.end(), second.begin(), second.end());
                    records.emplace_back(joinRecords);
                }
            }
        }
    }

    std::vector<std::pair<std::string, std::string>> getColumns() { return columns; }

    BaseNode* getSource() { return source; }

    BaseExprNode* getExpr() { return expr; }

    void setRecords(JoinRecord _records) { allrecords.emplace_back(_records); }

    std::vector<RecordsFull> getRecords() { return allrecords[0]; }

    void setExpressionVisitor(ExpressionVisitor* visitor) { expressionVisitor = visitor; }

    std::string getTableName() { return tableName; }

    int64_t getStartTime() { return startTime; }

    int64_t getFinishTime() { return finishTIme; }

   private:
    std::string curValue;
    std::string tableName;
    JoinRecord firstRecords;
    JoinRecord secondRecords;

    JoinRecord records;
    std::vector<JoinRecord> allrecords;

    std::vector<std::pair<std::string, std::string>> columns;
    BaseExprNode* expr = nullptr;
    BaseNode* source = nullptr;
    ExpressionVisitor* expressionVisitor = nullptr;
    int countEq = 0;
    inline static int id = -1;
    inline static std::vector<std::pair<std::string, std::string>> curExpr;
    int64_t startTime = -1;
    int64_t finishTIme = -1;
};

#endif  // SELSQL_SELECTVISITOR_H
