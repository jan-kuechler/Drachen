#include "pch.h"
#include "Enemy.h"

float norm(const Vector2f& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

void Enemy::Update(float elapsed)
{
	AnimSprite::Update(elapsed);

	if (!path.empty()) {
		Vector2f tgt = path.top();
		Vector2f dist = tgt - GetPosition();

		if (norm(dist) < 5)
			path.pop();
	}
	if (!path.empty()) {
		Vector2f tgt = path.top();
		Vector2f dir = tgt - GetPosition();

		float r = norm(dir);
		dir /= r;

		Move(dir * speed * elapsed);
	}
}

void Enemy::SetTarget(size_t x, size_t y)
{
	FindPath(x, y);
}

struct Node
{
	size_t x, y;
	float g, f;

	Node *parent;

	explicit Node(Vector2i pos)
	: x(pos.x), y(pos.y), g(.0f), f(.0f), parent(0)
	{ }

	Node(size_t x, size_t y)
	: x(x), y(y), g(.0f), f(.0f), parent(0)
	{ }

	bool operator == (const Node& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator != (const Node& rhs) const
	{
		return !(*this == rhs);
	}

	// heuristic distance from node to goal
	float CalcH(const Node& goal)
	{
		return 1.0f * (std::abs(static_cast<int>(x - goal.x)) + std::abs(static_cast<int>(y - goal.y)));
	}
};

struct NodePtrComp
{
	bool operator()(const Node* lhs, const Node *rhs) const
	{
		return lhs->f > rhs->f;
	}
};

struct NodeEqPos
{
	const Node& ref;

	explicit NodeEqPos(const Node& r)
	: ref(r)
	{ }

	bool operator()(const Node* nptr) const
	{
		return (*nptr) == ref;
	}
};

static bool ValidNeighbor(int x, int y, const Map& map)
{
	return (x >= 0) && (y >= 0)
		&& (static_cast<size_t>(x) < map.GetWidth()) && (static_cast<size_t>(y) < map.GetHeight()) 
		&& map.GetGrid()[x][y];
}

static void FillNeighbors(std::vector<Vector2i>& c, const Map& map, int x, int y)
{
	if (ValidNeighbor(x-1, y, map))
		c.push_back(Vector2i(x-1, y));

	if (ValidNeighbor(x+1, y, map))
		c.push_back(Vector2i(x+1, y));

	if (ValidNeighbor(x, y-1, map))
		c.push_back(Vector2i(x, y-1));

	if (ValidNeighbor(x, y+1, map))
		c.push_back(Vector2i(x, y+1));
}

void Enemy::FindPath(size_t tgtX, size_t tgtY)
{
	// clear the path stack
	while (!path.empty())
		path.pop();

	NodePtrComp comp;
	std::vector<Node*> allNodes; // remember all nodes to free them at the end
	std::vector<Node*> open, closed;

	Node *goal = new Node(tgtX, tgtY);
	allNodes.push_back(goal);

	Node *start = new Node(map->PositionToBlock(GetPosition()));
	allNodes.push_back(start);

	// open is a heap, sorted by (cost-to-reach + expected-distance-to-goal)
	open.push_back(start);
	boost::push_heap(open, comp);

	while (!open.empty() && *open.front() != *goal) {
		Node *cur = open.front();

		// remove the current node from the open heap
		boost::pop_heap(open, comp);
		open.pop_back();

		// and find all neighbors
		std::vector<Vector2i> neighbors;
		FillNeighbors(neighbors, *map, cur->x, cur->y);

		for (std::vector<Vector2i>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
			// neighbor node
			Node *suc = new Node(it->x, it->y);
			allNodes.push_back(suc);

			float newg = cur->g + 1.f; // the cost to reach the neighbor is the cost to reach the current node plus one step

			// find the neighbor in the open or the closed list
			std::vector<Node*>::iterator oit = boost::find_if(open, NodeEqPos(*suc));
			std::vector<Node*>::iterator cit = boost::find_if(closed, NodeEqPos(*suc));

			if (oit != open.end()) {
				if ((*oit)->g <= newg)
					continue; // there's a better path
			}
			if (cit != closed.end()) {
				if ((*cit)->g <= newg)
					continue;
			}

			// best step so far
			suc->parent = cur;
			suc->g = newg;
			suc->f = suc->g + suc->CalcH(*goal);

			// remove the node from open/closed
			if (cit != closed.end()) {
				closed.erase(cit);
			}
			if (oit != open.end()) {
				open.erase(oit);
				boost::make_heap(open, comp);
			}

			open.push_back(suc);
			boost::push_heap(open, comp);
		}

		closed.push_back(cur);
	}

	if (open.empty())
		return;

	// reconstruct the path from the goal node, following the parent pointers
	Node *n = open.front();
	std::ofstream out("path.txt");
	while (n) {
		out << n->x << "\t" << n->y << "\n";
		path.push(map->BlockToPosition(Vector2i(n->x, n->y)));
		n = n->parent;
	}
	out.close();

	for (size_t i=0; i < allNodes.size(); ++i) {
		delete allNodes[i];
	}
}
