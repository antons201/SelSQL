//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_VARIABLENODE_H
#define SELSQL_VARIABLENODE_H

#include <string>
#include <utility>
#include "../../Utils/Headers/CommonUtils.h"
#include "BaseNode.h"
#include "ConstraintNode.h"

class VariableNode : public BaseNode {
   public:
    VariableNode(std::string _name, Type _type) : name(std::move(_name)), type(_type) {}

    VariableNode(std::string _name, Type _type, std::vector<ConstraintNode *> _childs)
                                                                                                        : name(std::move(_name)),
                                                                                                          type(_type),
                                                                                                          constraints(std::move(_childs)) {
    }

    std::vector<ConstraintNode *> getConstraints() { return constraints; }

    void accept(TreeVisitor *v) override { addVariable(v->visitTemplate(this)); }

    void setSize(int _size) { size = _size; }

    int getSize() { return size; }

    std::string getVarName() { return name; }
    Type getVarType() { return type; }

    std::pair<std::string, Variable> getVariable() { return variable; }

    void addVariable(std::pair<std::string, Variable> var) { variable = std::move(var); }

   private:
    std::string name;
    Type type;
    std::vector<ConstraintNode *> constraints;
    std::pair<std::string, Variable> variable;
    int size = 0;
};

#endif  // SELSQL_VARIABLENODE_H
