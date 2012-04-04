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

#define ENABLE_DIAG 0

struct Node
{
	size_t x, y;

	float g, h, f;

	Node *parent;

	Node(Vector2i pos)
	: x(pos.x), y(pos.y), g(.0f), h(.0f), f(.0f), parent(0)
	{ }

	Node(size_t x, size_t y)
	: x(x), y(y), g(.0f), h(.0f), f(.0f), parent(0)
	{ }

	bool operator == (const Node& rhs)
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator != (const Node& rhs)
	{
		return !(*this == rhs);
	}

	float CalcH(const Node& goal)
	{
#if ENABLE_DIAG
		return 1.0f * std::max(std::abs(static_cast<int>(x - goal.x)), std::abs(static_cast<int>(y - goal.y)));
#else
		return 1.0f * (std::abs(static_cast<int>(x - goal.x)) + std::abs(static_cast<int>(y - goal.y)));
#endif
	}
};

struct NodePtrComp
{
	bool operator()(const Node* lhs, const Node *rhs) const
	{
		return lhs->f > rhs->f;
	}
};

void Enemy::FindPath(size_t tgtX, size_t tgtY)
{
	while (!path.empty())
		path.pop();

	std::vector<Node*> allNodes;
	std::vector<Node*> open, closed;

	Node *goal = new Node(tgtX, tgtY);
	allNodes.push_back(goal);

	Node *start = new Node(map->PositionToBlock(GetPosition()));
	allNodes.push_back(start);

	open.push_back(start);
	std::push_heap(open.begin(), open.end(), NodePtrComp());

	while (!open.empty() && *open.front() != *goal) {
		Node *cur = open.front();
		std::pop_heap(open.begin(), open.end(), NodePtrComp());
		open.pop_back();

#if ENABLE_DIAG
		for (int x = cur->x-1; x <= (int)cur->x+1; ++x) {
			for (int y = cur->y-1; y <= (int)cur->y+1; ++y) {
#else
		for (int i=0; i < 4; ++i) {
			int x, y;
			switch (i) {
			case 0:
				x = cur->x-1; y = cur->y;
				break;
			case 1:
				x = cur->x+1; y = cur->y;
				break;
			case 2: 
				x = cur->x; y = cur->y-1;
				break;
			case 3:
				x = cur->x; y = cur->y+1;
				break;
			}
			{

#endif
				if (x < 0 || y < 0 || (size_t)x >= map->GetWidth() || (size_t)y >= map->GetHeight() || (x == cur->x && y == cur->y) || !map->GetGrid()[x][y])
					continue;

				Node *suc = new Node(x, y);
				allNodes.push_back(suc);

				float newg = cur->g + 1.f;

				std::vector<Node*>::iterator oit, cit;
				for (oit = open.begin(); oit != open.end(); ++oit) {
					if (**oit == *suc)
						break;
				}
				for (cit = closed.begin(); cit != closed.end(); ++cit) {
					if (**cit == *suc)
						break;
				}

				if (oit != open.end()) {
					if ((*oit)->g <= newg)
						continue;
				}
				if (cit != closed.end()) {
					if ((*cit)->g <= newg)
						continue;
				}

				suc->parent = cur;
				suc->g = newg;
				suc->h = suc->CalcH(*goal);
				suc->f = suc->g + suc->h;

				if (cit != closed.end()) {
					closed.erase(cit);
				}
				if (oit != open.end()) {
					open.erase(oit);
					std::make_heap(open.begin(), open.end(), NodePtrComp());
				}

				open.push_back(suc);
				std::push_heap(open.begin(), open.end(), NodePtrComp());
			}
		}

		closed.push_back(cur);
	}

	if (open.empty())
		return;

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
