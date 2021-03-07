#include "algo.h"

// Functie de swap
void mySwap (int &a, int &b) {
	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}

// Pracurgerea DFS prin care se obtine forma euler, nivelul fiecarui nod din forma euler
// cat si prima aparitie in sir
void recursiveDfs (int &node, int &level, std::vector<int> &eulerRep,
	std::vector<int> &levelEvidence, int *firstOccourance,
	std::vector<std::vector<int>> &graph, int &fromNode) {

	eulerRep.push_back(node);
	levelEvidence.push_back(level);
	firstOccourance[node] = eulerRep.size() - 1;

	for (unsigned int i = 0; i < graph[node].size(); ++i) {
		if (graph[node][i] == fromNode)
			continue;

		int nextLevel = level + 1;
		recursiveDfs(graph[node][i], nextLevel, eulerRep, levelEvidence,
			firstOccourance, graph, node);

		eulerRep.push_back(node);
		levelEvidence.push_back(level);
	}
}

// Arbore de intervale construit in functie de nivelul nodurilor din reprezentarea euler
void makeSegmentTree(int &node, int &left, int &right,
	int *segmentTree, std::vector<int> &levelEvidence) {

	if (left == right) {
		segmentTree[node] = left;

	} else {
		int mijl = ((left + right) >> 1);
		int nodeLeft = (node << 1);
		int nodeRight = ((node << 1) + 1);

		makeSegmentTree(nodeLeft, left, mijl, segmentTree, levelEvidence);
		++mijl;
		makeSegmentTree(nodeRight, mijl, right, segmentTree, levelEvidence);

		if (levelEvidence[segmentTree[nodeLeft]] <=
			levelEvidence[segmentTree[nodeRight]]) {
			segmentTree[node] = segmentTree[nodeLeft];
		} else {
			segmentTree[node] = segmentTree[nodeRight];
		}
	}
}

// Interogare arbore de intervale pe forma euler din intervalul
// Determinat de aparitiile celor doua noduri in reprezenare
void querryLca(int &node, int &left, int &right,
	int *segmentTree, std::vector<int> &levelEvidence, std::vector<int> &eulerRep,
	int &sol, int &solH, int &nodeLeft, int &nodeRight) {

	if (nodeLeft <= left && right <= nodeRight) {
		if (levelEvidence[segmentTree[node]] < solH) {
			sol = eulerRep[segmentTree[node]];
			solH = levelEvidence[segmentTree[node]];
		}
	} else {
		int mijl = ((left + right) >> 1);
		int nodeL = (node << 1);
		int nodeR = ((node << 1) + 1);

		if (nodeLeft <= mijl)
			querryLca(nodeL, left, mijl, segmentTree, levelEvidence, eulerRep,
				sol, solH, nodeLeft, nodeRight);
		++mijl;
		if (mijl <= nodeRight)
			querryLca(nodeR, mijl, right, segmentTree, levelEvidence, eulerRep,
				sol, solH, nodeLeft, nodeRight);
	}
}

// Functia ce construieste algoritmul pentru un arbore si multiple interogari
std::vector<int> lca(std::vector<std::vector<int>> &graph,
	std::vector< std::pair<int, int> > &queries) {

	std::vector<int> eulerRep, levelEvidence, ans;
	eulerRep.push_back(0);
	levelEvidence.push_back(0);
	int *firstOccourance;
	int node = 1, level = 0;
	int left = 1;

	firstOccourance = new int [graph.size() + 2];

	recursiveDfs(node, level, eulerRep, levelEvidence, firstOccourance, graph, level);

	int *segmentTree;
	segmentTree = new int [levelEvidence.size() * 4] {0};

	int right = levelEvidence.size() - 1;

	makeSegmentTree(node, left, right, segmentTree, levelEvidence);

	for (unsigned int i = 0; i < queries.size(); ++i) {
		int nodeL = firstOccourance[queries[i].first];
		int nodeR = firstOccourance[queries[i].second];
		if (nodeL > nodeR)
			mySwap(nodeL, nodeR);
		int sol = 2e9;
		int solH = 2e9;
		
		querryLca(node, left, right, segmentTree, levelEvidence, eulerRep,
			sol, solH, nodeL, nodeR);
		ans.push_back(sol);
	}

	delete [] firstOccourance;
	delete [] segmentTree;
	return ans;

}
