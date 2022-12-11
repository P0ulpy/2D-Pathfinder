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

	TNode(const T& content/* = T()*/) // Can't make the difference between default contructor and copyuconstructor with T() as default parameter
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

		// assert(!IsAlreadyNeighbor(*pNode) && "Neighbour already added");

		_neighbors.push_back(std::move(pNode));
		if constexpr (sizeof...(args) > 0)
			AddNeighbors(args...);
	}

	void RemoveNeighbor(NodeSharedPtr<T> neighborToRemove)
	{
		_neighbors.remove_if([&neighborToRemove, this](const auto currNeighbor)
		{
			return neighborToRemove->_content == currNeighbor->_content && neighborToRemove->IsAlreadyNeighbor(*this);
		});
	}

	void ClearAllNeighbors()
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

	void SetIsVisitedByParent(const std::shared_ptr<TNode<T>>& parent)
	{
		if (_parent == nullptr)
		{
			_parent = parent;
		}
	}

	bool IsVisitedByParent() const { return _parent != nullptr; }
	NodeSharedPtr<T> GetParent() const { return _parent; }
	void ResetParent() { _parent.reset(); }

private:

	bool IsAlreadyNeighbor(const TNode& nodeNeighborToFind) const
	{
		const auto it = std::find_if(_neighbors.begin(), _neighbors.end(), [&nodeNeighborToFind](const auto pNeighbor)
		{
			return nodeNeighborToFind._content == pNeighbor->_content;
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
using NodeWeakPtr = std::weak_ptr<TNode<T>>;

template<typename T>
using NodePtr = TNode<T>*;

template<typename T>
class TGraph : public std::vector<NodeSharedPtr<T>>
{
public:
	static constexpr int MAP_WIDTH = 12;
	static constexpr int MAP_HEIGHT = 7;

	TGraph() = default;

	TGraph(std::initializer_list<TNode<T>> init)
	{
		for (auto element : init)
			AddNodes(element);
	}

	// You can initialize a graph with array
	TGraph(const T(&arr)[MAP_HEIGHT][MAP_WIDTH])
	{
		for (size_t l = 0; l < MAP_HEIGHT; l++)
		{
			for (size_t c = 0; c < MAP_WIDTH; c++) {
				AddNodes(arr[l][c]);
			}
		}
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

	NodeSharedPtr<T> FindNode(const T& nodeContent)
	{
		// Find in the vector if the node is contained
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
				if (!neighbor->IsVisitedByParent())
				{
					neighbor->SetIsVisitedByParent(currentNode); // Put currentNode as parent ref of neighbor
					queueNodesToVisit.push(neighbor);
				}
			}

			TraversalGraphRecursifBreathFirst(goalNode, queueNodesToVisit, Functor);
		}
	}
};