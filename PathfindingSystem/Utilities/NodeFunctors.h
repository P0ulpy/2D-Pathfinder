#pragma once

struct FunctorEmpty
{
    template <typename T>
    void Visit(NodeSharedPtr<T> pSharedNode) { }
};

struct FunctorLoggerNodes
{
    template <typename T>
    void Visit(NodeSharedPtr<T> pSharedNode)
    {
    	std::cout << pSharedNode->GetContent() << " ---> ";
    }
};

template <typename T>
struct FunctorVisitedList
{
    FunctorVisitedList(const int& preallocListVisitedSize = 0)
    {
        _listVisited.resize(preallocListVisitedSize);
    }

    std::list<NodeSharedPtr<T>> _listVisited;

    void Visit(NodeSharedPtr<T> pSharedNode)
    {
        _listVisited.push_front(pSharedNode);
    }
};

template<typename T>
struct FunctorCustomLoggerNodes
{
    FunctorCustomLoggerNodes(const std::function<void(NodeSharedPtr<T>)>& callable) : _callable(callable) {}

    std::function<void(NodeSharedPtr<T>)> _callable;

    void Visit(NodeSharedPtr<T> pSharedNode)
    {
        _callable(pSharedNode);
    }
};