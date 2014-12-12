#include <vector>
#include <iostream>
#include <unordered_set>

using namespace std;

#define INFI 100000

class Next_Dist{
private:
	unordered_set<int> nexts;
	int dist;
public:
	Next_Dist(int _next, int _dist) :dist(_dist)
	{
		nexts.insert(_next);
	}

	Next_Dist(unordered_set<int> _nexts, int _dist) :nexts(_nexts), dist(_dist){}

	bool Update(int newN, int newD)
	{
		if (newD > dist){
			return false;
		}
		else if (newD == dist)
		{
			nexts.insert(newN);
			return true;
		}
		else
		{
			dist = newD;
			nexts.clear();
			nexts.insert(newN);
			return true;
		}
	}

	bool Update(unordered_set<int> newNs, int newD)
	{
		if (newD > dist){
			return false;
		}
		else if (newD == dist)
		{
			for (auto n : newNs)
			{
				nexts.insert(n);
			}
			return true;
		}
		else
		{
			dist = newD;
			nexts = newNs;
			return true;
		}
	}

	unordered_set<int> getNexts()
	{
		return nexts;
	}

	int getDist()
	{
		return dist;
	}

	static int addDist(Next_Dist a, Next_Dist b)
	{
		if (a.getDist() >= INFI || b.getDist() >= INFI)
		{
			return INFI;
		}
		else
		{
			return a.getDist() + b.getDist();
		}
	}
};
typedef Next_Dist Nd;

class FloydWarshall
{
private:
	vector<vector<Nd>> T;
	void initShortPathTable(vector<vector<int>> &dists)
	{
		auto N = dists.size();
		for (decltype(N) i = 0; i < N; ++i)
		{
			vector<Nd> subT;
			for (decltype(N) j = 0; j < N; ++j)
			{
				if (dists[i][j] >= INFI){
					subT.push_back(Nd(-1, INFI));
				}
				else
				{
					subT.push_back(Nd(j, dists[i][j]));
				}
			}
			T.push_back(subT);
		}
	}

	void doAlgorithm()
	{
		auto N = T.size();
		for (decltype(N) k = 0; k < N; ++k)
		{
			for (decltype(N) i = 0; i < N; ++i)
			{
				for (decltype(N) j = 0; j < N; ++j)
				{
					if (i == k || k == j) continue;
					T[i][j].Update(T[i][k].getNexts(), Nd::addDist(T[i][k], T[k][j]));
				}
			}
		}
	}

	void getShortPathTable(vector<vector<int>> &dists)
	{
		initShortPathTable(dists);
		doAlgorithm();
	}

	vector<vector<int>> getShortestPaths(int start, int end)
	{
		vector<vector<int>> paths, restPaths;
		vector<int> path;
		path.push_back(start);
		if (T[start][end].getDist() >= INFI) {
			return paths;
		}
		else if (start == end)
		{
			paths.push_back(path);
			return paths;
		}

		int restStart = start;
		while (T[restStart][end].getNexts().size() == 1)
		{
			restStart = *(T[restStart][end].getNexts().begin());
			path.push_back(restStart);
			if (restStart == end)
			{
				paths.push_back(path);
				return paths;
			}
		}
		for (auto n : T[restStart][end].getNexts())
		{
			restPaths = getShortestPaths(n, end);
			for (auto p : restPaths)
			{
				p.insert(p.begin(), path.begin(), path.end());
				paths.push_back(p);
			}
		}
		return paths;
	}

public:
	FloydWarshall(vector<vector<int>> &dists)
	{
		getShortPathTable(dists);
	}

	int getDistance(int start, int end)
	{
		return T[start][end].getDist();
	}

	vector<vector<int>> getPaths(int start, int end)
	{
		return getShortestPaths(start, end);
	}
};

void main()
{
	vector<vector<int>> dists = {
		{ 0, 1, 3, INFI + 1 },
		{ 1, 0, 1, 2 },
		{ 3, 1, 0, 1 },
		{ INFI + 1, 2, 1, 0 }
	};
	FloydWarshall FW = FloydWarshall(dists);
	int dist = FW.getDistance(3, 1);
	vector<vector<int>> paths = FW.getPaths(3, 1);
	cout << "A --> C: Distance = " << dist << endl;
	cout << "Paths:" << endl;
	for (decltype(paths.size())i = 0; i < paths.size(); ++i)
	{
		cout << "\t" << i + 1 << ") ";
		for (auto j : paths[i])
		{
			cout << j << " ";
		}
		cout << endl;
	}
}