#pragma once

#include <memory>
#include <iostream>

#include <list>
#include <algorithm>
#include <cassert>
#include <queue>
#include <unordered_map>
#include <vector>

// Le node
template<typename T>
class TNode
{
	template<typename P>
	using NodeSharedPtr = std::shared_ptr<TNode<P>>;

public:
	TNode() : TNode(T())
	{ }

	TNode(const T& content/* = T()*/)
		: _content(content)
	{

	}

	TNode(const TNode& node) : _content(node._content)
	{
		for (auto& neighbor : node._neighbors)
			_neighbors.push_back(neighbor);
	}

	TNode& operator=(TNode&& other) noexcept // move assignment
	{
		return *this;
	}

	TNode& operator=(const TNode& node)
	{
		return *this;
	}

	~TNode() = default;

	bool operator==(const TNode& node)
	{
		return (_content == node._content && _neighbors.size() == node._neighbors.size());
	}

	bool operator!=(const TNode& node)
	{
		return !(*this == node);
	}


	template<typename...Args>
	void AddNeighbors(NodeSharedPtr<T> pNode, Args... args)
	{
		if (IsAlreadyNeighbor(*pNode))
			return;

		assert(!IsAlreadyNeighbor(*pNode) && "Neighbour already added");

		_neighbors.push_back(std::move(pNode));
		if constexpr (sizeof...(args) > 0)
			AddNeighbors(args...);
	}

	void RemoveAllNeighbors()
	{
		_neighbors.clear();
	}

	bool HasSameContent(const TNode& node) const
	{
		return _content == node._content;
	}

	void SetContent(T content)
	{
		_content = content;
	}

	T GetContent() const
	{
		return _content;
	}

	const std::list<NodeSharedPtr<T>>& GetNeighbors() const
	{
		return _neighbors;
	}

	void SetIsVisitedBy(const std::shared_ptr<TNode<T>>& parent)
	{
		if (_parent == nullptr)
		{
			_parent = parent;
		}
	}

	bool IsVisited() const { return _parent != nullptr; }

	NodeSharedPtr<T> GetParent() const { return _parent; }

private:

	bool IsAlreadyNeighbor(const TNode& node) const
	{
		const auto it = std::find_if(_neighbors.begin(), _neighbors.end(), [&node](const auto pNeighbor)
			{
				return node._content == pNeighbor->_content;
			});

		return it != _neighbors.end();
	}

	T _content;
	std::list<NodeSharedPtr<T>> _neighbors;

	NodeSharedPtr<T> _parent{ nullptr };
};

// Le graph 

template<typename T>
using NodeSharedPtr = std::shared_ptr<TNode<T>>;

template<typename T>
using WeakSharedPtr = std::weak_ptr<TNode<T>>;

template<typename T>
using NodePtr = TNode<T>*;

template<typename T>
class TGraph : public std::vector<NodeSharedPtr<T>>
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

	NodeSharedPtr<T> FindNode(const TNode<T>& node)
	{
		const auto it = std::find_if(this->begin(), this->end(), [&node](const auto& pNode)
			{
				return pNode->HasSameContent(node);
			});

		assert(it != this->end() && "The node is not present in the graph.");
		return (*it);
	}

	NodeSharedPtr<T> FindNode(const T& nodeContent)
	{
		const auto it = std::find_if(this->begin(), this->end(), [](const auto& pNode)
			{
				return pNode->GetContent() == nodeContent;
			});

		assert(it != this->end() && "The node is not present in the graph.");
		return (*it);
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

	template <typename Callable>
	void TraversalGraphRecursifBreathFirst(NodeSharedPtr<T> goalNode, std::queue<NodeSharedPtr<T>>& queueNodesToVisit, Callable& Functor)
	{
		if (!queueNodesToVisit.empty())
		{
			auto currentNode = queueNodesToVisit.front(); // Copy of pointer
			//Functor.Visit(currentNode);

			if (currentNode == goalNode)
				return; // Omg on a trouvé

			queueNodesToVisit.pop();

			for (const auto& neighbor : currentNode->GetNeighbors())
			{
				if (!neighbor->IsVisited())
				{
					neighbor->SetIsVisitedBy(currentNode); // Put currentNode as parent ref of neighbor
					queueNodesToVisit.push(neighbor);
				}
			}

			TraversalGraphRecursifBreathFirst(goalNode, queueNodesToVisit, Functor);
		}
	}
};