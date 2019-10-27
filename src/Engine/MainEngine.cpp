//
// Created by quiks on 01.10.2019.
//

#include "Headers/MainEngine.h"

Message MainEngine::CreateTable(const std::shared_ptr<Table>& table) {
    Message result;
    int error = file_manager_->CreateFile(table);
    file_manager_->CloseAllFiles();
    if (error) {
        result = Message(ErrorConstants::ERR_TABLE_EXISTS);
    }
    return result;
}

std::shared_ptr<Table> MainEngine::ShowCreateTable(const std::string& tableName) {
    std::shared_ptr<Table> table(new Table());
    if (file_manager_->OpenFile(tableName)) {
        return table;
    }
    table = file_manager_->GetTable(tableName);
    file_manager_->CloseAllFiles();
    return table;
}

Message MainEngine::DropTable(const std::string& tableName) {
    Message result;
    int error = file_manager_->DeleteTable(tableName);
    if (error) {
        result = Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }
    file_manager_->CloseAllFiles();
    return result;
}

MainEngine::MainEngine() { file_manager_ = std::make_shared<FileManager>(); }

std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> MainEngine::GetCursor(std::string tableName) {
    file_manager_->CloseAllFiles();
    std::shared_ptr<Table> table(new Table());
    std::shared_ptr<Cursor> cursor1(new Cursor());

    if (file_manager_->OpenFile(tableName)) {
        return std::make_pair(table, cursor1);
    };
    table = std::make_shared<Table>(*file_manager_->GetTable(tableName));
    cursor1 = std::make_shared<Cursor>(table, file_manager_);
    return std::make_pair(table, cursor1);
}
