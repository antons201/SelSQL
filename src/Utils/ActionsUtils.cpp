//
// Created by sapiest on 02.10.2019.
//

#include "Headers/ActionsUtils.h"
#include <queue>
#include <utility>

std::string ActionsUtils::makeRequestCreateFromTable(const std::shared_ptr<Table>& table) {
    const char space = ' ';
    const char semicolon = ';';
    const char comma = ',';
    std::string str = "CREATE TABLE ";
    str += table->name + '(';
    for (int i = 0; i < table->getFields().size(); ++i) {
        int countConstraint = 0;
        auto field = table->getFields()[i];
        str += field.first + space;
        str += ParserUtils::typeToString(field.second.type);
        if (field.second.type == TYPE_CHAR) {
            str += '(' + std::to_string(field.second.size) + ')';
        }
        if (!field.second.getConstraints().empty()) {
            str += space;
        }
        for (auto& constraint : field.second.getConstraints()) {
            countConstraint++;
            str += ParserUtils::constraintToString(constraint);
            if (countConstraint != field.second.getConstraints().size()) {
                str += space;
            }
        }
        if (i != table->getFields().size() - 1 && table->getFields().size() > 1) {
            str += comma;
            str += space;
        }
    }
    str += ')';
    str += semicolon;

    return str;
}

int ActionsUtils::checkSameForUpdate(const Record& oldRec, const Record& newRec, const std::shared_ptr<Table>& table) {
    for (int i = 0; i < oldRec.size(); ++i) {
        auto tableCol = table->getFields()[i];
        if (tableCol.first == oldRec[i].first) {
            for (int j = 0; j < newRec.size(); ++j) {
                if (oldRec[i].first == newRec[j].first) {
                    for (auto& constr : tableCol.second.getConstraints()) {
                        if (constr == Constraint::NOT_NULL) {
                            continue;
                        }
                        auto curVal = newRec[j].second;
                        std::transform(curVal.begin(), curVal.end(), curVal.begin(),
                                       [](unsigned char c) { return std::tolower(c); });
                        if (oldRec[i].second == curVal) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

Message ActionsUtils::checkConstraint(const Record& updateColumns, const std::shared_ptr<Table>& table,
                                      const std::vector<ActionsUtils::Record>& records, bool isUpdate) {
    Message error;
    std::string colName;
    int countSameVal = 0;
    if (records.empty()) {
        int i = 0;
        for (auto& colValue : updateColumns) {
            auto value = colValue.second;
            if (colValue.first == "*") {
                colName = table->getFields()[i++].first;
            } else {
                colName = colValue.first;
            }
            for (auto& field : table->getFields()) {
                if (field.first != colName) {
                    continue;
                }

                for (auto& constraint : field.second.getConstraints()) {
                    if (constraint == Constraint::NOT_NULL || constraint == Constraint::PRIMARY_KEY) {
                        error = checkNotNull(value);
                        if (error.getErrorCode()) {
                            return error;
                        }
                    }
                }
            }
        }
    }

    for (auto& record : records) {
        int i = 0;
        countSameVal += checkSameForUpdate(record, updateColumns, table);
        for (auto& colValue : updateColumns) {
            auto value = colValue.second;
            if (colValue.first == "*") {
                colName = table->getFields()[i++].first;
            } else {
                colName = colValue.first;
            }
            // не сработает в случае, если update t set id = 1 where id = 1; гдк t = create table t(id INT UNIQUE, age
            // INT); ,
            //могу быть записи (1, 0), а обновить на (1, 2) update t set id = 1, age = 2 where id = 1;

            for (auto& field : table->getFields()) {
                if (field.first != colName) {
                    continue;
                }

                for (auto& constraint : field.second.getConstraints()) {
                    for (auto& elem : record) {
                        if (elem.first != colName) {
                            continue;
                        }
                        auto curVal = elem.second;
                        std::transform(curVal.begin(), curVal.end(), curVal.begin(),
                                       [](unsigned char c) { return std::tolower(c); });
                        error = constraintsCheckers[constraint](value, curVal);
                        if ((error.getErrorCode() == ErrorConstants::ERR_UNIQUE) && isUpdate) {
                            if (countSameVal > 1) {
                                return error;
                            }
                            error = Message();
                        } else if (error.getErrorCode()) {
                            std::cout << records.size() << std::endl;
                            return error;
                        }
                    }
                }
            }
        }
    }
    return error;
}

Message ActionsUtils::checkNotNull(std::string newVal, const std::string& oldVal) {
    std::string temp = std::move(newVal);
    std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c) { return std::tolower(c); });
    if (temp == "null") {
        return Message(ErrorConstants::ERR_NOT_NULL);
    }
    return Message();
}

Message ActionsUtils::checkUnique(std::string newVal, const std::string& oldVal) {
    std::string val;
    if (newVal[0] == '\'') {
        val = newVal.substr(1, newVal.length() - 1);
    } else {
        val = newVal;
    }
    if (val == oldVal) {
        return Message(ErrorConstants::ERR_UNIQUE);
    }
    return Message();
}

Message ActionsUtils::checkPrimaryKey(const std::string& newVal, const std::string& oldVal) {
    Message err = checkNotNull(newVal, oldVal);
    if (err.getErrorCode()) {
        return err;
    }
    err = checkUnique(newVal, oldVal);
    if (err.getErrorCode()) {
        return err;
    }
    return Message();
}

Message ActionsUtils::checkFieldsExist(const std::shared_ptr<Table>& table, const Record& updateColumns) {
    std::vector<int> existCols;
    for (auto& col : updateColumns) {
        //проверить для insert и update
        if (col.first == "*") {
            existCols.emplace_back(1);
            continue;
        }
        for (auto& tableCol : table->getFields()) {
            if (col.first == tableCol.first) {
                existCols.emplace_back(1);
            }
        }
    }
    if (existCols.size() != updateColumns.size()) {
        return Message(ErrorConstants::ERR_NO_SUCH_FIELD);
    }
    return Message();
}

std::string ActionsUtils::checkSelectColumns(std::vector<Record> values, const std::vector<std::string>& selectCols) {
    if (values.empty()) {
        return "";
    }
    std::vector<std::vector<std::pair<std::string, std::string>>> printVal;
    std::map<std::string, int> colsIndex;
    int strSize = values[0].size();
    int count = 0;
    for (auto& val : values[0]) {
        colsIndex.insert(std::make_pair(val.first, count));
        count++;
    }
    count = 0;
    for (auto& str : values) {
        std::vector<std::pair<std::string, std::string>> colVals;
        for (auto& col : selectCols) {
            if (col != "*") {
                colVals.push_back(str[colsIndex[col]]);
            } else {
                for (auto& val : values[count]) {
                    colVals.push_back(val);
                }
            }
        }
        printVal.push_back(colVals);
        count++;
    }
    return ActionsUtils::getSelectMessage(printVal);
}

std::string ActionsUtils::getSelectMessage(std::vector<Record> values) {
    std::string str;
    std::vector<int> len;
    std::stringstream stringstream;
    int n = values.size();
    int strSize = values[0].size();
    len.reserve(strSize);
    for (int i = 0; i < strSize; i++) {
        len.push_back(values[0][i].first.length());
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < strSize; j++) {
            int maxLen = len[j];
            values[i][j].second.length() > len[j] ? len[j] = values[i][j].second.length() : len[j] = len[j];
        }
    }
    for (int i = 0; i < strSize; i++) {
        stringstream << values[0][i].first;
        int lenStr = values[0][i].first.length();
        for (int j = lenStr; j < len[i]; j++) {
            stringstream << "\40";
        }
        stringstream << "|";
    }
    stringstream << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < strSize; j++) {
            stringstream << values[i][j].second;
            int lenStr = values[i][j].second.length();
            for (int k = lenStr; k < len[j]; k++) {
                stringstream << "\40";
            }
            stringstream << "|";
        }
        stringstream << std::endl;
    }
    return stringstream.str();
}

std::string ActionsUtils::getTableInfo(const std::shared_ptr<Table>& table, int includeCols) {
    std::stringstream stringstream;
    stringstream << "Table " << table->name << ":" << std::endl;
    if (includeCols) {
        for (const auto& field : table->getFields()) {
            stringstream << field.first << "|";
        }
        stringstream << std::endl;
    }

    return stringstream.str();
}
int ActionsUtils::isNumbers(const std::string& a, const std::string& b) {
    if (std::isdigit(*a.c_str()) && std::isdigit(*b.c_str()))
        return 1;
    return 0;
}
int ActionsUtils::isChars(const std::string& a, const std::string& b) {
    if (std::isalpha(*a.c_str()) && std::isalpha(*b.c_str()))
        return 1;
    return 0;
}
int ActionsUtils::compareEquals(const std::string& a, const std::string& b) {
    if (!isNumbers(a, b)) {
        // auto c = a.substr(1, a.length() - 2);
        // if (isChars(c, b)) {
        return a == b;
        //}
    }
    return std::stod(a) == std::stod(b);
}
int ActionsUtils::compareMore(const std::string& a, const std::string& b) {
    if (!isNumbers(a, b)) {
        // auto c = a.substr(1, a.length() - 2);
        // if (isChars(c, b)) {
        return a > b;
        //}
    }
    return std::stod(a) > std::stod(b);
}
int ActionsUtils::compareMoreEq(const std::string& a, const std::string& b) {
    if (!isNumbers(a, b)) {
        // auto c = a.substr(1, a.length() - 2);
        // if (isChars(c, b)) {
        return a >= b;
        //}
    }
    return std::stod(a) >= std::stod(b);
}
int ActionsUtils::compareLess(const std::string& a, const std::string& b) {
    if (!isNumbers(a, b)) {
        // auto c = a.substr(1, a.length() - 2);
        // if (isChars(c, b)) {
        return a < b;
        //}
    }
    return std::stod(a) < std::stod(b);
}
int ActionsUtils::compareLessEq(const std::string& a, const std::string& b) {
    if (!isNumbers(a, b)) {
        // auto c = a.substr(1, a.length() - 2);
        // if (isChars(c, b)) {
        return a <= b;
        //}
    }
    return std::stod(a) <= std::stod(b);
}
std::vector<ActionsUtils::Record>
ActionsUtils::getAllRecords(const std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>>& cursor) {
    std::vector<Record> records;

    cursor.second->Reset();
    do {
        auto record = cursor.second->Fetch();
        if (!record.empty()) {
            records.emplace_back(record);
        }

    } while (!cursor.second->Next());
    cursor.second->Reset();
    return records;
}
