#include "TNode.h"

template<typename T>
template<typename ...Args>
void TNode<T>::AddNeighbors(TNode<T>::NodeSharedPtr pNode, Args ...args)
{
	if (IsAlreadyNeighbor(*pNode))
		return;

	assert(!IsAlreadyNeighbor(*pNode) && "Neighbor already added");

	_neighbors.push_back(std::move(pNode));
	if constexpr (sizeof...(args) > 0)
		AddNeighbors(args...);
}

template<typename T>
void TNode<T>::RemoveNeighbor(TNode<T>::NodeSharedPtr neighborToRemove)
{
	_neighbors.remove_if([&neighborToRemove, this](const auto currNeighbor)
	{
		return neighborToRemove->_content == currNeighbor->_content && neighborToRemove->IsAlreadyNeighbor(*this);
	});
}

template<typename T>
void TNode<T>::ClearAllNeighbors()
{
	_neighbors.clear();
}

template<typename T>
bool TNode<T>::HasSameContent(const TNode& node) const
{
	return _content == node._content;
}

template<typename T>
void TNode<T>::SetContent(T content)
{
	_content = content;
}

template<typename T>
T TNode<T>::GetContent() const
{
	return _content;
}

template<typename T>
const std::list<typename TNode<T>::NodeSharedPtr >& TNode<T>::GetNeighbors() const
{
	return _neighbors;
}

template<typename T>
void TNode<T>::SetIsVisitedByParent(const TNode<T>::NodeSharedPtr& parent)
{
	if (_parent == nullptr)
	{
		_parent = parent;
	}
}

template<typename T>
bool TNode<T>::IsVisitedByParent() const { return _parent != nullptr; }

template<typename T>
TNode<T>::NodeSharedPtr TNode<T>::GetParent() const { return _parent; }

template<typename T>
void TNode<T>::ResetParent() { _parent.reset(); }

template<typename T>
bool TNode<T>::IsAlreadyNeighbor(const TNode& nodeNeighborToFind) const
{
	const auto it = std::find_if(_neighbors.begin(), _neighbors.end(), [&nodeNeighborToFind](const auto pNeighbor)
	{
		return nodeNeighborToFind._content == pNeighbor->_content;
	});

	return it != _neighbors.end();
}