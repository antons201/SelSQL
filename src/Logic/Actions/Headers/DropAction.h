//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_DROPACTION_H
#define SELSQL_DROPACTION_H

#include "BaseAction.h"
class DropAction : public BaseAction {
   public:
    explicit DropAction(std::shared_ptr<TreeVisitor> _visitor) : BaseAction(std::move(_visitor)) {}

    Message execute(std::shared_ptr<BaseActionNode>) override;
};

#endif  // SELSQL_DROPACTION_H
