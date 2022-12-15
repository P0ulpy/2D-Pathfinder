#pragma once

#include <cassert>

#include "TNode.h"

template<typename T>
using NodeSharedPtr = std::shared_ptr<TNode<T>>;

template<typename T>
using NodeWeakPtr = std::weak_ptr<TNode<T>>;

template<typename T>
using NodePtr = TNode<T>*;

template<typename T>
class TGraph : public std::list<NodeSharedPtr<T>>
{
public:
	TGraph() = default;

	TGraph(std::initializer_list<TNode<T>> init)
	{
		for (auto element : init)
			AddNodes(element);
	}

	~TGraph() = default;

	template<typename... Args>
	void AddNodes(TNode<T> node, Args... args)
	{
		this->push_back(std::make_shared<TNode<T>>(node));
		if constexpr (sizeof...(args) > 0)
			AddNodes(args...);
	}

	void AddEdge(const TNode<T>& lhs, const TNode<T>& rhs)
	{
		auto pLhs = FindNode(lhs);
		auto pRhs = FindNode(rhs);
		pLhs->AddNeighbors(pRhs);
		pRhs->AddNeighbors(pLhs);
	}

	void RemoveEdge(const TNode<T>& lhs, const TNode<T>& rhs)
	{
		auto pLhs = FindNode(lhs);
		auto pRhs = FindNode(rhs);
		pLhs->RemoveNeighbor(pRhs);
		pRhs->RemoveNeighbor(pLhs);
	}

	void RemoveAllEdges(const TNode<T>& node)
	{
		auto pNode = FindNode(node);

		for (const auto& neighbor : pNode->GetNeighbors())
		{
			neighbor->RemoveNeighbor(pNode);
		}

		pNode->ClearAllNeighbors();
	}

	void ResetParentsForAllNodes()
	{
		std::for_each(this->begin(), this->end(), [](const auto& node)
		{
			node->ResetParent();
		});
	}

	NodeSharedPtr<T> FindNode(const T& nodeContent)
	{
		// Find in the list if the node is contained
		const auto it = std::find_if(this->begin(), this->end(), [&nodeContent](const auto& pNode)
		{
			return pNode->GetContent() == nodeContent;
		});

		assert(it != this->end() && "The node is not present in the graph.");
		return (*it);
	}

	NodeSharedPtr<T> FindNode(const TNode<T>& node)
	{
		return FindNode(node.GetContent());
	}

	template <typename Callable>
	void VisitParentsFrom(NodeSharedPtr<T> currNode, Callable& Functor)
	{
		Functor.Visit(currNode);

		const auto& parentNode = currNode->GetParent();
		if (parentNode != nullptr && currNode != parentNode)
		{
			VisitParentsFrom(parentNode, Functor);
		}
	}
};
