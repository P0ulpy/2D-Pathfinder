#pragma once

#include "../TGraph.h"

struct LoggerNodes
{
    template <typename T>
    void Visit(NodeSharedPtr<T> pSharedNode)
    {
        NodeWeakPtr<T> pWeakNode(pSharedNode);

        if(auto node = pWeakNode.lock())
        {
            std::cout << node->GetContent() << " ---> ";
        }
        else
        {
            std::cout << "null ---> ";
        }

    }
};