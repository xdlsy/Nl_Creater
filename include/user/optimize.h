#pragma once
#include "mynetlist.h"
void remove_const_input(MyNetlist* global_nl);

void remove_same_input_mux(MyNetlist* global_nl);

void merge_same_function_net(MyNetlist* global_nl);