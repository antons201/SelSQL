
#ifndef SELSQL_CURSOR_H
#define SELSQL_CURSOR_H
#include <functional>
#include <memory>
#include <utility>
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/Record.h"
#include "DataManager.h"
#include "FileManager.h"
#include "TransactManager.h"

class Cursor {
    std::shared_ptr<Table> table_;

    std::shared_ptr<DataManager> data_manager_;
    std::shared_ptr<TransactManager> transact_manager_;
    std::shared_ptr<DataBlock> data_block_;
    std::vector<std::pair<std::string, std::string>> values_;
    std::shared_ptr<std::fstream> data_file_;

    int block_id_ = 0;
    int pos_in_block_ = 0;
    int64_t current_tr_id_ = 0;
    int max_pos = 0;
    int last_inserted = 0;
    static void SaveFieldData(std::string val, Type type, char *dist, int start_pos);
    static void GetFieldData(std::string *dist, Type type, char *src, int start_pos);

    int EmplaceBack(Record *record);
    int NextDataBlock();

   public:
    Cursor(const std::shared_ptr<Table> &table, const std::shared_ptr<DataManager> &data_manager,
           const std::shared_ptr<TransactManager> &transact_manager, std::shared_ptr<std::fstream> data_file,
           int64_t tr_id);

    Cursor();

    int Insert(const std::vector<std::string> &cols, const std::vector<std::string> &new_data);

    int NextRecord();

    int Delete();

    int Update(std::vector<std::string> cols, std::vector<std::string> new_data);

    int Reset();

    ~Cursor();

    int GetLastInsertedPos();
    int GetCurrentPos();
    int SetPos(int pos);

    std::shared_ptr<DataManager> GetDataManager();
    //    std::vector<std::pair<std::string, std::string>> Fetch(int64_t time_s = 0, int64_t time_e = 0);
    std::pair<std::vector<std::pair<std::string, std::string>>, std::pair<int64_t, int64_t>> Fetch(int64_t time_s = 0,
                                                                                                   int64_t time_e = 0);
    static void MakeCommited(const std::shared_ptr<DataBlock> &block, int64_t tr_id,
                             const std::vector<std::pair<int, int>> &need_commit, int record_size);
};
#endif  // SELSQL_CURSOR_H
