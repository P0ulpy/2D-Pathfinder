#pragma once

#include <memory>
#include <list>
#include <algorithm>
#include <ostream>
#include <vector>

struct Vec2i
{
	int x, y;

	inline Vec2i(int x, int y) : x(x), y(y) {}

	inline bool operator==(const Vec2i& other) const
	{
		return (other.x == x && other.y == y);
	}
};

// Le node
template<typename T>
class TNode
{
public:
	using NodeSharedPtr = std::shared_ptr<TNode<T>>;
	using NodeWeakPtr = std::weak_ptr<TNode<T>>;


	inline TNode() : TNode(T()) { }

	inline TNode(const T& content)
		: _content(content) { }

	inline TNode(const TNode& node) : _content(node._content)
	{
		for (auto& neighbor : node._neighbors)
			_neighbors.push_back(neighbor);
	}

	inline TNode& operator=(TNode&& other) noexcept // move assignment
	{
		return *this;
	}

	inline TNode& operator=(const TNode& node)
	{
		return *this;
	}

	~TNode() = default;

	inline bool operator==(const TNode& node)
	{
		return (_content == node._content && _neighbors.size() == node._neighbors.size());
	}

	inline bool operator!=(const TNode& node)
	{
		return !(*this == node);
	}


	template<typename...Args>
	void AddNeighbors(NodeSharedPtr pNode, Args... args);

	void RemoveNeighbor(NodeSharedPtr neighborToRemove);

	void ClearAllNeighbors();

	bool HasSameContent(const TNode& node) const;

	void SetContent(T content);

	T GetContent() const;

	const std::list<NodeSharedPtr>& GetNeighbors() const;

	void SetIsVisitedByParent(const std::shared_ptr<TNode<T>>& parent);

	bool IsVisitedByParent() const;
	NodeSharedPtr GetParent() const;
	void ResetParent();

private:

	bool IsAlreadyNeighbor(const TNode& nodeNeighborToFind) const;

	T _content;
	std::list<NodeSharedPtr> _neighbors;

	NodeSharedPtr _parent{ nullptr };
};


struct Tile2D
{
	Tile2D() = default;
	Tile2D(int x, int y, bool isTraversable = true) : _pos(x, y), _isTraversable(isTraversable) {}
	Tile2D(const Vec2i point, bool isWall = true) : _pos(point.x, point.y), _isTraversable(isWall) {}

	Vec2i _pos{ 0, 0 };
	bool _isTraversable{ false }; // Could be replaced by an int _weight

	/* distance from the starting node to a given node */
	double g{ 0 };
	/* distance from a given node to the final node */
	double h{ 0 };

	inline void SetG(const double value) { g = value; }
	inline void SetH(const double value) { h = value; }

	inline bool operator==(const Tile2D& tile) const
	{
		return _pos == tile._pos;
	}

	inline friend std::ostream& operator<<(std::ostream& os, Tile2D& tile) // Why "friend" here and not on == operator ?
	{
		os << tile._pos.x << "_" << tile._pos.y;
		return os;
	}

	/* G + H, sum of the two distances */
	inline double GetF() const { return g + h; };
};