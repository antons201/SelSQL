
#ifndef SELSQL_CURSOR_H
#define SELSQL_CURSOR_H
#include <memory>
#include <utility>
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/Record.h"
#include "FileManager.h"
#include "TransactManager.h"
class Cursor {
    std::shared_ptr<Table> table_;
    std::shared_ptr<FileManager> file_manager_;
    std::shared_ptr<TransactManager> transact_manager_;
    std::shared_ptr<DataBlock> data_block_;
    std::vector<std::pair<std::string, std::string>> values_;
    std::shared_ptr<std::fstream> data_file_;

    int changed = 0;
    int read_block_id = 0;
    int write_block_id = 0;
    int current_pos = 0;
    int readed_data = 0;
    int current_session_deleted_ = 0;

    static void SaveFieldData(std::string val, Type type, char* dist, int start_pos);
    static void GetFieldData(std::string* dist, Type type, char* src, int start_pos);
    void Allocate();
    int EmplaceBack(char* record_buf, long tr_s, long tr_e);

   public:
    Cursor(const std::shared_ptr<Table>& table, const std::shared_ptr<FileManager>& file_manager,
           const std::shared_ptr<TransactManager>& transact_manager, std::shared_ptr<std::fstream> data_file);
    Cursor();
    int Insert(const std::vector<std::string>& cols, const std::vector<std::string>& new_data, long transact_sp = 0);
    int UpdateDataBlock();
    int NextRecord();
    int NextDataBlock();
    int Delete(long transact_sp = 0);
    int Update(std::vector<std::string> cols, std::vector<std::string> new_data, long transact_sp = 0);
    int Reset();

    void Commit();
    std::vector<std::pair<std::string, std::string>> Fetch(long tr_p);
    ~Cursor();
};
#endif  // SELSQL_CURSOR_H
