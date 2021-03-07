#include "algo.h"

// Functie de swap
void mySwap (int &a, int &b) {
	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}

// Functia care gaseste radacina unei multimi pornind de la un nod dat
int findParent(int &node, int *parent) {
	if (node != parent[node])
		parent[node] = findParent(parent[node], parent);
	return parent[node];
}

// Functia care face ca doua noduri sa fie parte din aceeasi multime
void makeUnion (int &node1, int &node2, int *parent, int *rank) {
	int root1 = findParent(node1, parent);
	int root2 = findParent(node2, parent);

	if (root1 != root2) {
		if (rank[root1] < rank[root2])
			mySwap(root1, root2);

		parent[root2] = root1;
		if (rank[root1] == rank[root2])
			rank[root1]++;
	}
}

// Parcurgerea DFS a arborelui si construirea padurilor de multimi disjuncte
void recursiveDfs(int &node, std::vector<std::vector<int>> &graph,
	std::vector< std::vector< std::pair<int, int> > >& queries, bool *visited,
	int *ancestor, int *parent, int *rank, std::vector<int> &answer) {

	parent[node] = node;
	ancestor[node] = node;
	rank[node] = 1;
	visited[node] = 1;

	for (unsigned int i = 0; i < graph[node].size(); ++i) {
		if (!visited[graph[node][i]]) {

			recursiveDfs(graph[node][i], graph, queries, visited,
				ancestor, parent, rank, answer);

			makeUnion(node, graph[node][i], parent, rank);
			ancestor[findParent(node, parent)] = node;
		}
	}

	// Interogarile ce se desfasoara concomitent cu parcurgerea
	for (unsigned int i = 0; i < queries[node].size(); ++i) {
		if (visited[queries[node][i].first] == 1)
			answer[queries[node][i].second] =
		ancestor[findParent(queries[node][i].first, parent)];
	}
}


// Functia ce construieste algoritmul pentru un arbore si multiple interogari
std::vector<int> lca(std::vector<std::vector<int>> &graph,
	std::vector< std::pair<int, int> > &queries) {

	bool *visited;
	int node = 1, *ancestor, *rank, *parent;
	std::vector<int> answer(queries.size(), 0);
	
	std::vector< std::vector< std::pair<int, int> > > eachNodeQuery(graph.size());

	// Corespondenta intre perechea de noduri pentru interogare si numarul unei interogari
	for (unsigned int i = 0; i < queries.size(); ++i) {
		eachNodeQuery[queries[i].first].push_back({queries[i].second, i});
		eachNodeQuery[queries[i].second].push_back({queries[i].first, i});

	}

	visited = new bool[graph.size()] {0};
	ancestor = new int[graph.size()] {0};
	parent = new int[graph.size()] {0};
	rank = new int[graph.size()] {1};

	recursiveDfs(node, graph, eachNodeQuery, visited, ancestor, parent, rank, answer);

	delete [] visited;
	delete [] ancestor;
	delete [] rank;
	delete [] parent;

    return answer;
}
