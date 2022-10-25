#pragma onceint
#include <cmath>
#include <algorithm>
#include <set>
#include "common.h"
#include "mynetlist.h"
#include "parser.h"

void construct_baseline_sub_circuit(int outpin_idx, MyNetlist* global_nl, Parser* global_parser);
void construct_mux_from_matrix(MyNetlist* global_nl, Parser* global_parser);
void remove_redundancy(std::set<int>& set1, std::set<int>& set2);
bool check_redundancy(std::set<int>& set1, std::set<int>& set2);
template<typename T, typename N> int argmax(N& values, T& index);
void build_mux_tree(std::map<int, std::vector<std::vector<int>>>& dval_to_svals, MyNetlist* net, MyPin* out, int _default);
int find_best_sel(std::set<int>& dins_to_zero, std::set<int>& dins_to_one, std::set<int>& remaining_sels, std::set<int>& dins, std::map<int, std::vector<std::vector<int>>>& dval_to_svals);
void init_net(MyNetlist* net, Parser* parser);