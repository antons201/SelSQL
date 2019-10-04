//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_PARSERLOGIC_H
#define SELSQL_PARSERLOGIC_H

#include <string>
using std::string;

#include "../../Engine/Headers/MainEngine.h"
#include "../../Utils/Headers/ParserUtils.h"

class ParserLogic {
    MainEngine engineApi;
    ParserUtils parserUtils;
    Response response;
    std::map<std::string, int> checkName;

   public:
    void addColumn(char* name, char* type);
    void addConstraint(char* name);
    Response finish();
    void addTableName(char* name);
    void addActionName(char* name);
};

#endif  // SELSQL_PARSERLOGIC_H