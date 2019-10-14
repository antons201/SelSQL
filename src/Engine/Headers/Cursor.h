//
// Created by quiks on 07.10.2019.
//

#ifndef SELSQL_CURSOR_H
#define SELSQL_CURSOR_H
#include <memory>
#include "../../Utils/Headers/Constants.h"
#include "FileManager.h"
class Cursor {
    std::shared_ptr<FileManager> fileManager;
    unsigned char* data;

    std::vector<std::pair<std::string, std::string>> vals;

    void SaveFieldData(std::string val, Type type, unsigned char* dist, int start_pos);
    void GetFieldData(std::string* dist, Type type, unsigned char* src, int start_pos);

   public:
    int pos = 0;
    int readed_data = 0;
    std::shared_ptr<Table> table;
    Cursor(std::shared_ptr<Table> t, std::shared_ptr<FileManager> fm) {
        table = t;
        fileManager = fm;
        data = new unsigned char[Constants::DATA_PAGE_SIZE];
        data = fileManager->GetData(table->name);
        for (const auto& i : table->fields) {
            vals.emplace_back(std::make_pair(i.first, ""));
        }
    };
    Cursor() = default;
    int Insert(std::vector<std::string> cols, std::vector<std::string> new_data);
    int Commit();
    int Next();
    int Delete();
    std::vector<std::pair<std::string, std::string>> Fetch();
};
#endif  // SELSQL_CURSOR_H