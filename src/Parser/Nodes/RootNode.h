//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_ROOTNODE_H
#define SELSQL_ROOTNODE_H

#include <map>
#include <utility>
#include "BaseNode.h"
class RootNode : public BaseNode {
   public:
    explicit RootNode(std::map<NodeType, BaseNode*> _children)
                                                                                                        : BaseNode(NodeType::ROOT),
                                                                                                          children(std::move(_children)) {
    }

    ~RootNode() override {
        for (auto& ch : children) {
            delete ch.second;
        }
    }

    std::map<NodeType, BaseNode*> getChildren() { return children; }

    NodeType getNodeType() override { return BaseNode::getNodeType(); }

   private:
    std::map<NodeType, BaseNode*> children;
};

#endif  // SELSQL_ROOTNODE_H