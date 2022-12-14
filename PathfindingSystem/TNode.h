#pragma once

#include <memory>
#include <list>
#include <algorithm>
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

	TNode(const T& content/* = T()*/) // Can't make the difference between default contructor and copy constructor with T() as default parameter
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


struct Tile2D
{
	Tile2D() = default;
	Tile2D(int x, int y, bool isTraversable = true) : _pos(x, y), _isTraversable(isTraversable) {}
	Tile2D(const sf::Vector2i point, bool isWall = true) : _pos(point.x, point.y), _isTraversable(isWall) {}

	sf::Vector2i _pos{ 0, 0 };
	bool _isTraversable{ false }; // Could be replaced by an int _weight

	/* distance from the starting node to a given node */
	double g{ 0 };
	/* distance from a given node to the final node */
	double h{ 0 };

	void SetG(const double value) { g = value; }
	void SetH(const double value) { h = value; }

	bool operator==(const Tile2D& tile) const
	{
		return _pos == tile._pos;
	}

	friend std::ostream& operator<<(std::ostream& os, const Tile2D& tile) // Why "friend" here and not on == operator ?
	{
		os << tile._pos.x << "_" << tile._pos.y;
		return os;
	}

	/* G + H, sum of the two distances */
	double GetF() const { return g + h; };
};