#pragma once

#include <memory>
#include <list>
#include <ostream>
#include <cassert>  
#include <algorithm>

template<typename NodeType>
class TNode
{
public:
	using NodeSharedPtr = std::shared_ptr<TNode<NodeType>>;

	TNode() : TNode(NodeType()) { }

	TNode(NodeType content)
		: _content(content) { }

	TNode(TNode& node) : _content(node._content)
	{
		for (auto& neighbor : node._neighbors)
			_neighbors.push_back(neighbor);
	}

	TNode& operator=(TNode&& other) noexcept // move assignment
	{
		return *this;
	}

	TNode& operator=( TNode& node)
	{
		return *this;
	}

	~TNode() = default;

	bool operator==( TNode& node)
	{
		return (_content == node._content && _neighbors.size() == node._neighbors.size());
	}

	bool operator!=( TNode& node)
	{
		return !(*this == node);
	}

	template<typename...Args>
	void AddNeighbors(NodeSharedPtr pNode, Args ... args)
	{
		if (IsAlreadyNeighbor(*pNode))
			return;

		assert(!IsAlreadyNeighbor(*pNode) && "Neighbor already added");

		_neighbors.push_back(std::move(pNode));
		if constexpr (sizeof...(args) > 0)
			AddNeighbors(args...);
	}

	void RemoveNeighbor(NodeSharedPtr neighborToRemove)
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

	bool HasSameContent( TNode& node) 
	{
		return _content == node._content;
	}

	void SetContent(NodeType content)
	{
		_content = content;
	}

	NodeType GetContent() const
	{
		return _content;
	}

	 std::list<NodeSharedPtr>& GetNeighbors() 
	{
		return _neighbors;
	}

	bool HasAnyNeighbors() const
	{
		return !_neighbors.empty();
	}

	void SetIsVisitedByParent(const NodeSharedPtr& parent)
	{
		if (_parent == nullptr)
		{
			_parent = parent;
		}
	}

	bool IsVisitedByParent() 
	{
		return _parent != nullptr;
	}

	NodeSharedPtr GetParent() 
	{
		return _parent;
	}

	void ResetParent()
	{
		_parent.reset();
	}

private:

	bool IsAlreadyNeighbor( TNode& nodeNeighborToFind) 
	{
		 auto it = std::find_if(_neighbors.begin(), _neighbors.end(), [&nodeNeighborToFind]( auto pNeighbor)
		 {
			return nodeNeighborToFind._content == pNeighbor->_content;
		 });

		return it != _neighbors.end();
	}

	NodeType _content;
	std::list<NodeSharedPtr> _neighbors;

	NodeSharedPtr _parent{ nullptr };
};