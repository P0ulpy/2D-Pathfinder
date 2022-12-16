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
		AddEdge(FindNode(lhs), FindNode(rhs));
	}

	void AddEdge(const NodeSharedPtr<T>& pLhs, const NodeSharedPtr<T>& pRhs)
	{
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

	void RemoveAllEdges(const NodeSharedPtr<T> pNode)
	{
		for (const auto& neighbor : pNode->GetNeighbors())
		{
			neighbor->RemoveNeighbor(pNode);
		}

		pNode->ClearAllNeighbors();
	}

	void RemoveAllEdges(const TNode<T>& node)
	{
		RemoveAllEdges(FindNode(node));
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

class GraphMap2D
{
public:
	GraphMap2D(int width, int height) : _width(width), _height(height)
	{
		for (int l = 0; l < _height; ++l)
		{
			for (int c = 0; c < _width; ++c)
			{
				_graph.AddNodes(Tile2D(c, l));
			}
		}

		UpdateAllTilesNeighbors(true, true);
	}

	void SetRandomWallsForMap(unsigned int chanceForNoWall = 7)
	{
		std::srand(std::time(nullptr));

		std::ranges::for_each(_graph, [&](const NodeSharedPtr<Tile2D>& nodeTile2D)
			{
				if(!static_cast<bool>(std::rand() % chanceForNoWall))
				{
					AddWallAt(nodeTile2D->GetContent());
				}
			});

		UpdateAllTilesNeighbors();
	}

	void AddWallAt(const Tile2D& tile2D)
	{
		const auto& nodeTile2D = _graph.FindNode(tile2D);

		Tile2D newContent = Tile2D(nodeTile2D->GetContent());
		newContent.SetIsTraversable(false);
		nodeTile2D->SetContent(newContent);

		_graph.RemoveAllEdges(nodeTile2D);
	}
	
	void RemoveWallAt(const Tile2D& tile2D)
	{
		const auto nodeTile2D = _graph.FindNode(tile2D);

		if(IsTileAWall(nodeTile2D))
		{
			Tile2D newContent = Tile2D(nodeTile2D->GetContent());
			newContent.SetIsTraversable(true);
			nodeTile2D->SetContent(newContent);

			UpdateDirectNeighborsForTile(nodeTile2D, true);
		}
	}

	void UpdateDirectNeighborsForTile(const NodeSharedPtr<Tile2D>& nodeTile2D, bool ignoreIfTileIsAWall = false, bool ignoreIfNeighborsAreWalls = false)
	{
		if (!ignoreIfTileIsAWall || IsTileAWall(nodeTile2D))
			return; // Not traversable -> no neighbors

		const auto x = nodeTile2D->GetContent()._pos.x;
		const auto y = nodeTile2D->GetContent()._pos.y;

		if (x - 1 >= 0) // Left neighbor
		{
			if(const auto neighborNode = _graph.FindNode(Tile2D(x - 1, y)); ignoreIfNeighborsAreWalls || !IsTileAWall(neighborNode))
				_graph.AddEdge(nodeTile2D, neighborNode);
		}

		if (y - 1 >= 0) // Top neighbor
		{
			if (const auto neighborNode = _graph.FindNode(Tile2D(x, y - 1)); ignoreIfNeighborsAreWalls || !IsTileAWall(neighborNode))
				_graph.AddEdge(nodeTile2D, neighborNode);
		}

		if (x + 1 < _width) // Right neighbor
		{
			if (const auto neighborNode = _graph.FindNode(Tile2D(x + 1, y)); ignoreIfNeighborsAreWalls || !IsTileAWall(neighborNode))
				_graph.AddEdge(nodeTile2D, neighborNode);
		}

		if (y + 1 < _height) // Bottom neighbor
		{
			if (const auto neighborNode = _graph.FindNode(Tile2D(x, y + 1)); ignoreIfNeighborsAreWalls || !IsTileAWall(neighborNode))
				_graph.AddEdge(nodeTile2D, neighborNode);
		}
	}

	void UpdateAllTilesNeighbors(bool ignoreIfTileIsAWall = false, bool ignoreIfNeighborsAreWalls = false)
	{
		for (int l = 0; l < _height; ++l)
		{
			for (int c = 0; c < _width; ++c)
			{
				UpdateDirectNeighborsForTile(_graph.FindNode(Tile2D(c, l)), ignoreIfTileIsAWall, ignoreIfNeighborsAreWalls);
			}
		}
	}

	void AddPortal(const Tile2D& tile2DFirst, const Tile2D& tile2DSecond)
	{
		RemoveWallAt(tile2DFirst);
		RemoveWallAt(tile2DSecond);

		_graph.AddEdge(tile2DFirst, tile2DSecond);
	}

	void RemovePortal(const Tile2D& tile2DFirst, const Tile2D& tile2DSecond)
	{
		_graph.RemoveEdge(tile2DFirst, tile2DSecond);
	}

	void DisplayGraphInConsole()
	{
		for (int i = 0; i < _width + 2; ++i)
		{
			std::cout << "X ";
		}

		std::cout << std::endl;

		for (int l = 0; l < _height; ++l)
		{
			std::cout << "X ";

			for (int c = 0; c < _width; ++c)
			{
				std::cout << (IsTileAWall(_graph.FindNode(Tile2D(c, l))) ? "X " : "  ");
			}

			std::cout << "X " << std::endl;
		}

		for (int i = 0; i < _width + 2; ++i)
		{
			std::cout << "X ";
		}

		std::cout << std::endl;

	}

	bool IsTileAWall(const NodeSharedPtr<Tile2D>& nodeTile2D)
	{
		//return !nodeTile2D->HasAnyNeighbors();
		return !nodeTile2D->GetContent()._isTraversable;
	}

	TGraph<Tile2D>& GetGraph() { return _graph; }

private:
	TGraph<Tile2D> _graph;

	int _width{ 0 }, _height{ 0 };
};
