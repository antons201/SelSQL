//
// Created by sapiest on 24.12.2019.
//

#include "Headers/IndexCreateAction.h"
#include "../../Parser/Headers/IndexCreateVisitor.h"

Message IndexCreateAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<IndexCreateVisitor *>(getTreeVisitor().get());
    auto tableName = v->getTableName();
    auto colName = v->getColName();
    auto cursor = v->getEngine()->GetCursor(tableName, root->getId());
    auto table = cursor.first;

    table->setIndexField(colName);
    v->getEngine()->UpdateTableMeta(table);

    auto data_manager = cursor.second->GetDataManager();
    //    auto it = std::find_if(table->getFields().begin(), table->getFields().end(),
    //                           [colName](const std::pair<std::string, Variable>& field) { return field.first ==
    //                           colName; });
    std::pair<std::string, Variable> it;
    bool hasCol = false;
    for (auto &field : table->getFields()) {
        if (field.first == colName) {
            it = field;
            hasCol = true;
            break;
        }
    }
    if (!hasCol) {
        // TODO нет такой колонки
        return Message();
    }

    data_manager->CreateIndex(tableName, it.second.type);

    do {
        auto _record = cursor.second->Fetch();
        if (_record.first.empty()) {
            continue;
        }
        auto indexCol = std::find_if(_record.first.begin(), _record.first.end(),
                                     [colName](const std::pair<std::string, std::string> &val) {
                                         return val.first == colName;
                                     });
        if (indexCol != _record.first.end()) {
            auto val = indexCol->second;
            data_manager->InsertIndex(tableName, val, cursor.second->GetCurrentPos());
        }
    } while (!cursor.second->NextRecord());

    return Message();
}
