#ifndef ALGO_H
#define ALGO_H

#include <vector>

std::vector<int> lca(std::vector<std::vector<int>> &graph,
	std::vector< std::pair<int, int> > &queries);

#endif