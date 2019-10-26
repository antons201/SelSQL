#include "Headers/Cursor.h"
#include <cstring>

void convert(unsigned char *dist, std::string val, Type type) {
    unsigned char null_flag = 'n';
    switch (type) {
        case TYPE_INT: {
            if (!val.empty() and val != "null") {
                int v = std::stoi(val);
                std::memcpy(&dist[1], &v, Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
        case TYPE_FLOAT: {
            if (!val.empty() and val != "null") {
                double fl = std::stod(val);
                std::memcpy(&dist[1], &fl, Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
        case TYPE_CHAR: {
            if (!val.empty()) {
                val.reserve(Constants::TYPE_SIZE[type]);
                std::memcpy(&dist[1], val.c_str(), Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
    }
    dist[0] = null_flag;
}

void Cursor::SaveFieldData(std::string val, Type type, unsigned char *dist, int start_pos) {
    unsigned char null_flag = 'n';
    unsigned char temp_field[Constants::TYPE_SIZE[type] + 1];
    auto temp_val = new unsigned char[Constants::TYPE_SIZE[type]];
    convert(temp_field, val, type);
    std::memcpy(&dist[start_pos], temp_field, Constants::TYPE_SIZE[type] + 1);
}

int Cursor::Insert(std::vector<std::string> cols, std::vector<std::string> new_data) {
    size_t pos_in_block = 0;
    std::shared_ptr<DataBlock> block;
    int no_place = 1;
    for (const auto &i : dataBlocks_) {
        if (i->deleted) {
            block = i;
            pos_in_block = i->deleted_pos_[--i->deleted] * table->record_size;
            no_place = 0;
            break;
        } else if (i->last_record_pos <= Constants::DATA_SIZE / table->record_size - 1) {
            block = i;
            pos_in_block = i->last_record_pos++ * table->record_size;
            no_place = 0;
            break;
        }
    }
    if (no_place) {
        pos_in_block = 0;
        block = std::make_shared<DataBlock>();
        block->record_size = table->record_size;
        auto new_del_pos = new char[Constants::DATA_SIZE];
        block->setDeletedPos(new_del_pos);
        dataBlocks_.emplace_back(block);
    }

    int count = 0;
    for (auto &i : vals) {
        if (cols.empty()) {
            if (count < new_data.size()) {
                i.second = new_data[count++];
                continue;
            }
        }
        for (size_t j = 0; j < cols.size(); ++j) {
            if (i.first == cols[j]) {
                i.second = new_data[j];
                break;
            }
        }
    }
    auto fields = table->getFields();
    unsigned char record[table->record_size];
    size_t next_pos = 0;
    for (size_t i = 0; i < vals.size(); ++i) {
        Type type = fields[i].second.type;
        SaveFieldData(vals[i].second, type, record, next_pos);
        next_pos += Constants::TYPE_SIZE[type] + 1;
    }

    table->record_amount++;
    block->record_amount++;
    std::memcpy(&block->data_[pos_in_block], record, table->record_size);

    return 0;
}

int Cursor::Commit() {
    fileManager->UpdateFile(table, dataBlocks_);
    return 0;
}

std::vector<std::pair<std::string, std::string>> Cursor::Fetch() {
    auto block = dataBlocks_[current_block];
    unsigned char record[table->record_size];
    std::vector<std::pair<std::string, std::string>> values;
    std::memcpy(record, &block->data_[current_pos_in_block * table->record_size], table->record_size);
    int field_pos = 0;
    for (int i = 0; i < table->fields.size(); ++i) {
        unsigned char field[Constants::TYPE_SIZE[table->fields[i].second.type] + 1];
        Type type = table->fields[i].second.type;
        std::string value = "";
        std::memcpy(field, &block->data_[field_pos + current_pos_in_block * table->record_size],
                    Constants::TYPE_SIZE[table->fields[i].second.type] + 1);
        if (field[0] == '0' or field[0] == '\000') {
            return std::vector<std::pair<std::string, std::string>>();
        }
        if (field[0] == 'n') {
            GetFieldData(&value, type, field, 0);
        }

        values.emplace_back(std::make_pair(table->fields[i].first, value));
        field_pos += Constants::TYPE_SIZE[table->fields[i].second.type] + 1;
    }
    readed_data++;
    return values;
}

void Cursor::GetFieldData(std::string *dist, Type type, unsigned char *src, int start_pos) {
    switch (type) {
        case TYPE_INT: {
            int v;
            std::memcpy(&v, &src[start_pos + 1], Constants::TYPE_SIZE[type]);
            *dist = std::to_string(v);
            break;
        }

        case TYPE_FLOAT: {
            double v;
            std::memcpy(&v, &src[start_pos + 1], Constants::TYPE_SIZE[type]);
            *dist = std::to_string(v);
            break;
        }
        case TYPE_CHAR: {
            char dst[Constants::TYPE_SIZE[type]];
            std::memcpy(dst, &src[start_pos + 1], Constants::TYPE_SIZE[type]);
            *dist = std::string(dst);
            break;
        }
    }
}

int Cursor::Next() {
    if (dataBlocks_[current_block]->record_amount > readed_data) {
        current_pos_in_block++;
        return 0;
    } else {
        if (dataBlocks_.size() - 1 == current_block) {
            return 1;
        }
        current_block++;
        readed_data = 0;
        current_pos_in_block = 0;
        return 0;
    }
    //        if (readed_data + 1 > table->record_amount) {
    //            return 1;
    //        } else {
    //            current_pos_in_block++;
    //            return 0;
    //        }
}

int Cursor::Delete() {
    auto block = dataBlocks_[current_block];
    std::memset(&block->data_[current_pos_in_block * table->record_size], '0', table->record_size);
    block->deleted_pos_[block->deleted++] = current_pos_in_block;
    block->last_record_pos++;
    //    dataBlocks_[current_block]->deleted_pos_[dataBlocks_[current_block]] = current_pos_in_block;
    //    table->last_record_pos++;
    return 0;
}

int Cursor::Update(std::vector<std::string> cols, std::vector<std::string> new_data) {
    auto block = dataBlocks_[current_block];
    unsigned char record[table->record_size];
    std::memcpy(record, &block->data_[current_pos_in_block * table->record_size], table->record_size);
    auto fields = table->getFields();
    int next_pos = 0;
    for (size_t i = 0; i < vals.size(); ++i) {
        Type type = fields[i].second.type;

        for (size_t j = 0; j < cols.size(); j++) {
            if (vals[i].first == cols[j]) {
                SaveFieldData(new_data[j], type, record, next_pos);
            }
        }
        next_pos += Constants::TYPE_SIZE[type] + 1;
    }

    std::memcpy(&block->data_[current_pos_in_block * table->record_size], record, table->record_size);

    return 0;
}

int Cursor::StartPos() {
    current_pos_in_block = 0;
    readed_data = 0;
    current_block = 0;
    return 0;
}

Cursor::~Cursor() {
    if (!table->name.empty())
        fileManager->CloseAllFiles();
}

Cursor::Cursor() { table = std::make_shared<Table>(); }
