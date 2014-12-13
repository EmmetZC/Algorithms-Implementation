#include <vector>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <list>

using namespace std;

#define INFI 100000

class Node{
private:
	int index;
	unordered_set<int> prevs;
	int dist;
public:
	Node(int _index, int _prev, int _dist) :index(_index), dist(_dist)
	{
		prevs.insert(_prev);
	}

	Node(int _index, unordered_set<int> _prevs, int _dist) :index(_index), prevs(_prevs), dist(_dist){}

	bool Update(int newP, int newD)
	{
		if (newD > dist){
			return false;
		}
		else if (newD == dist)
		{
			prevs.insert(newP);
			return true;
		}
		else
		{
			dist = newD;
			prevs.clear();
			prevs.insert(newP);
			return true;
		}
	}

	unordered_set<int> Prevs()
	{
		return prevs;
	}

	int Index()
	{
		return index;
	}

	int Dist()
	{
		return dist;
	}
};

int addDist(int a, int b)
{
	if (a >= INFI || b >= INFI)
	{
		return INFI;
	}
	else
	{
		return a + b;
	}
}

void sortByDist(list<Node> &q)
{
	q.sort([](Node a, Node b){
			return (a.Dist() < b.Dist());
		}
	);
}

void sortByIndex(vector<Node> &q)
{
	sort(q.begin(), q.end(),
		[](Node a, Node b){
		return (a.Index() < b.Index());
	}
	);
}

list<Node> initWaiting(int start, int end, vector<vector<int>> &dists)
{
	list<Node> Waiting;
	auto N = dists.size();
	for (decltype(N) i = 0; i < N; ++i)
	{
		if (dists[start][i] >= INFI)
		{
			Node n = Node(i, -1, INFI);
			Waiting.push_back(n);
		}
		else
		{
			Node n = Node(i, 0, dists[start][i]);
			Waiting.push_back(n);
		}
	}
	sortByDist(Waiting);
	return Waiting;
}

vector<vector<int>> getShortestPaths(int start, int end, vector<Node> &Visited)
{
	vector<vector<int>> paths, restPaths;
	vector<int> path = { end };
	if (Visited[end].Dist() >= INFI)
	{
		return paths;
	}
	else if (start == end)
	{
		paths.push_back(path);
		return paths;
	}
	int last = end;
	while (Visited[last].Prevs().size() == 1)
	{
		last = *(Visited[last].Prevs().begin());
		path.push_back(last);
		if (last == start)
		{
			paths.push_back(path);
			return paths;
		}
	}
	for (auto n : Visited[last].Prevs())
	{
		restPaths = getShortestPaths(start, n, Visited);
		for (auto p : restPaths)
		{
			p.insert(p.begin(), path.begin(), path.end());
			paths.push_back(p);
		}
	}
	return paths;
}

vector<vector<int>> Dijkstra(int start, int end, vector<vector<int>> &dists)
{
	vector<vector<int>> paths;
	auto N = dists.size();
	list<Node> Waiting;
	vector<Node> Visited;
	//Init DijQueue
	Waiting = initWaiting(start, end, dists);
	while (!Waiting.empty())
	{
		Node c = Waiting.front();	//current
		Waiting.pop_front();
		for (auto &n : Waiting)
		{
			int dist = dists[c.Index()][n.Index()];
			if (dist < INFI)
			{
				n.Update(c.Index(), addDist(c.Dist(),dist));
			}
		}
		sortByDist(Waiting);
		Visited.push_back(c);
	}
	sortByIndex(Visited);
	vector<vector<int>> rPaths = getShortestPaths(start, end, Visited);
	for (auto rP : rPaths)
	{
		vector<int> p;
		for (auto rit = rP.rbegin(); rit < rP.rend(); rit++)
		{
			p.push_back(*rit);
		}
		paths.push_back(p);
	}
	return paths;
}

void main()
{
	vector<vector<int>> dists = {
		{ 0, 1, 3, INFI + 1 },
		{ 1, 0, 1, 2 },
		{ 3, 1, 0, 1 },
		{ INFI + 1, 2, 1, 0 }
	};
	vector<vector<int>> paths = Dijkstra(0, 3, dists);
	for (auto p : paths)
	{
		for (auto n : p)
		{
			cout << n << " ";
		}
	}
}