//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_TABLE_H
#define SELSQL_TABLE_H

#include <map>
#include <string>
#include <unordered_map>
#include "../../Headers/CommonUtils.h"
#include "../../Headers/Constants.h"
#include "Variable.h"

class Table {
   public:
    typedef std::vector<std::pair<std::string, Variable>> FieldsMap;

    size_t record_amount = 0;
    size_t record_size = 0;
    Table() = default;

    Table(std::string _name, FieldsMap _fields) : name(_name), fields(_fields) {}

    void addField(const std::string &field_name, Type type) {
        fields.emplace_back(std::make_pair(field_name, Variable(type)));
        last_var_name = field_name;
    }

    void addField(const std::string &field_name, const Variable &var) {
        fields.emplace_back(std::make_pair(field_name, var));
        last_var_name = field_name;
    }

    int addConstraint(Constraint constraint) { return fields[fields.size() - 1].second.addConstraint(constraint); }

    FieldsMap getFields() { return fields; }

    FieldsMap getFields() const { return fields; }

    void calcRecordSize() {
        record_size = 0;
        for (auto &field : fields) {
            record_size += Constants::TYPE_SIZE[field.second.type] + 1;
        }
    }
    void setFields(std::vector<std::pair<std::string, Variable>> &fields_) { this->fields = fields_; }

    void clear() {
        name.erase();
        fields.clear();
    }

    std::string name;
    std::vector<std::pair<std::string, Variable>> fields;

   private:
    std::string last_var_name;
};

#endif  // SELSQL_TABLE_H
