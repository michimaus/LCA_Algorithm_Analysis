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

// Tabelul RMQ construit in functie de nivelul nodurilor din reprezentarea euler
void makeRmq(int **rmq, const int &sizeOfRmq, std::vector<int> &levelEvidence, int *lg) {
	int lgIdx;

	for (int i = 2; i <= sizeOfRmq; ++i) {
		lg[i] = lg[i >> 1] + 1;
	}
	for (int i = 1; i <= sizeOfRmq; ++i) {
		rmq[0][i] = i;
	}

	for (int i = 1; (1 << i) < sizeOfRmq; ++i) {
		for (int j = 1; j <= sizeOfRmq - (1 << i); ++j) {
			lgIdx = 1 << (i - 1);
			rmq[i][j] = rmq[i - 1][j];
			if (levelEvidence[rmq[i][j]] > levelEvidence[rmq[i - 1][j + lgIdx]]) {
				rmq[i][j] = rmq[i - 1][j + lgIdx];
			}
		}
	}
}

// Interogare RMQ pe forma euler din intervalul
// Determinat de aparitiile celor doua noduri in reprezenare
int querryLca(int &node1, int &node2, int *firstOccourance, int *lg,
	std::vector<int> &eulerRep, std::vector<int> &levelEvidence, int **rmq) {
	int sol;
	int oc1 = firstOccourance[node1];
	int oc2 = firstOccourance[node2];
	if (oc1 > oc2)
		mySwap(oc1, oc2);

	int dif = oc2 - oc1 + 1;
	int lgIdx = lg[dif];
	int shift = dif - (1 << lgIdx);

	if (levelEvidence[rmq[lgIdx][oc1]] > levelEvidence[rmq[lgIdx][oc1 + shift]])
		sol = rmq[lgIdx][oc1 + shift];
	else
		sol = rmq[lgIdx][oc1];

	return eulerRep[sol];
}

// Functia ce construieste algoritmul pentru un arbore si multiple interogari
std::vector<int> lca(std::vector<std::vector<int>> &graph,
	std::vector< std::pair<int, int> > &queries) {

	std::vector<int> eulerRep, levelEvidence, ans;

	eulerRep.push_back(0);
	levelEvidence.push_back(0);

	int *firstOccourance;
	int node = 1, level = 0, **rmq, *lg;

	firstOccourance = new int [graph.size()];

	recursiveDfs(node, level, eulerRep, levelEvidence, firstOccourance, graph, level);
	rmq = new int* [21];
	for (int i = 0; i < 21; ++i) {
		rmq[i] = new int [levelEvidence.size() + 2] {0};
	}

	lg = new int [eulerRep.size() + 1] {0};

	makeRmq(rmq, eulerRep.size(), levelEvidence, lg);

	for (unsigned int i = 0; i < queries.size(); ++i) {
		ans.push_back(querryLca(queries[i].first, queries[i].second,
			firstOccourance, lg, eulerRep, levelEvidence, rmq));
	}

	delete [] firstOccourance;
	for (int i = 0; i < 21; ++i) {
		delete [] rmq[i];
	}
	delete [] lg;
	delete [] rmq;

	return ans;

}
