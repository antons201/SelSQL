#include "Headers/FileManager.h"
#include <memory>

#include "../Utils/Structures/Data/DataBlock.h"

void FileManager::WriteTableMetaData(const std::shared_ptr<Table>& table) {
    if (files_.find(table->name) == files_.end() or !files_[table->name].isOpen()) {
        std::cerr << __func__ << "\t File isn't opened" << std::endl;
        throw FileNotOpened();
    }

    auto meta_file = files_[table->name].meta_file;
    meta_file->seekp(0, std::ios::beg);
    buffer_data buffer = GetTableBuffer(table.get());
    meta_file->write(buffer.first, buffer.second);
    meta_file->flush();
    delete[] buffer.first;
}

void FileManager::ReadTableMetaData(const std::string& table_name) {
    if (files_.find(table_name) == files_.end() or !files_[table_name].isOpen()) {
        std::cerr << __func__ << "\t File isn't opened" << std::endl;
        throw FileNotOpened();
    }
    auto meta_file = files_[table_name].meta_file;
    int size = GetFileSize(meta_file);
    char buffer[size];
    meta_file->read(buffer, size);
    table_data[table_name] = ReadTableFromBuffer(buffer);
}
int FileManager::OpenFile(const std::string& table_name) {
    const std::string& directory = table_name;
    std::string file_name = table_name;
    if (!fs::exists(directory)) {
        return 1;
    }
    auto dbFile = DB_FILE(new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::META_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary),
                          new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::DATA_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary));
    if (!dbFile.isOpen()) {
        dbFile.close();
        return 1;
    }
    files_[table_name] = dbFile;
    ReadTableMetaData(table_name);
    return 0;
}
int FileManager::CreateFile(const std::shared_ptr<Table>& table) {
    std::string table_name = table->name;
    this->CloseAllFiles();
    if (!fs::create_directory(table->name)) {
        return 1;
    }

    auto dbFile = DB_FILE(new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::META_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc),
                          new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::DATA_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc));
    files_[table_name] = dbFile;
    WriteTableMetaData(table);
    CloseAllFiles();
    return 0;
}
std::shared_ptr<Table> FileManager::GetTable(const std::string& table_name) { return table_data[table_name]; }

int FileManager::DeleteFile(const std::string& table_name) {
    this->CloseAllFiles();
    return !fs::remove_all(table_name);
}

int FileManager::UpdateBlock(const std::shared_ptr<Table>& table, std::shared_ptr<DataBlock> data, int block_id) {
    if (files_.find(table->name) == files_.end() or !files_[table->name].isOpen()) {
        std::cerr << __func__ << "\t File isn't opened" << std::endl;
        throw FileNotOpened();
    }

    if (data != nullptr) {
        this->WriteDataBlockToTemp(std::string(table->name), data, block_id);
    }

    return 0;
}

std::shared_ptr<DataBlock> FileManager::ReadDataBlock(const std::string& table_name, int block_id) {
    if (files_.find(table_name) == files_.end() or !files_[table_name].isOpen()) {
        std::cerr << __func__ << "\t File isn't opened" << std::endl;
        throw FileNotOpened();
    }

    int readed_data = 0;
    int offset = 0;
    auto data_file = files_[table_name].data_file;
    auto table = table_data[table_name];

    if (!GetFileSize(data_file)) {
        return nullptr;
    }
    if (GetFileSize(data_file) <= 4 + GetDataBlockSize(table->record_size) * block_id) {
        return nullptr;
    }

    data_file->seekg(0, std::ios::beg);
    int v = ReadIntFromFile(data_file);
    offset += 4;

    table->record_amount = v;
    if (table->record_amount == 0) {
        return nullptr;
    }

    data_file->seekg(offset + GetDataBlockSize(table->record_size) * block_id, std::ios::beg);
    char data_buffer[GetDataBlockSize(table->record_size)];
    data_file->read(data_buffer, GetDataBlockSize(table->record_size));
    return ReadDataBlockFromBuffer(data_buffer, table->record_size);
}
void FileManager::WriteDataBlockToTemp(const std::string& table_name, std::shared_ptr<DataBlock> data, int block_id) {
    if (files_.find(table_name) == files_.end() or !files_[table_name].isOpen()) {
        std::cerr << __func__ << "\t File isn't opened" << std::endl;
        throw FileNotOpened();
    }

    auto data_file = temp;
    int offset = 4;
    data_file->seekp(std::ios::beg);
    WriteIntToFile(data_file, table_data[table_name]->record_amount);

    data_file->seekp(offset + GetDataBlockSize(data.get()) * block_id, std::ios::beg);
    if (data->record_amount != 0) {
        buffer_data buffer = GetDataBlockBuffer(data.get());
        data_file->write(buffer.first, buffer.second);
        delete[] buffer.first;
    }


    data_file->flush();
}

void FileManager::CloseAllFiles() {
    for (auto& file : files_) {
        file.second.close();
    }
    files_.clear();
}

int FileManager::UpdateFile(const std::string& table_name) {
    auto flag = new std::ofstream("flag.flag");
    if (!flag->is_open()) {
        delete flag;
        flag = new std::ofstream("flag.flag", std::ios::trunc);
    }
    if (table_name == "") {
        auto res = std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::DATA_FILE_TYPE,
                                std::ios::binary | std::ios::in);
        if (res.is_open()) {
            RestoreFromTemp(temp, &res, flag);
        }
    } else {
        *flag << table_name;
        auto res = files_[table_name].data_file;
        RestoreFromTemp(temp, res, flag);
    }

    delete flag;
}

FileManager::FileManager() {
    temp = new std::fstream("TEMP", std::ios::binary | std::ios::out | std::ios::trunc | std::ios::in);
    UpdateFile("");
}
FileManager::~FileManager() { delete temp; }
