#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <queue>

// database forward declarations
class Model;
class Inst;
class Pin;
class Net;
// parser forward declarations
class Parser;

typedef std::vector<Inst*> INST_VEC;
typedef std::vector<Net*> NET_VEC;
typedef std::vector<Pin*> PIN_VEC;
typedef std::map<std::string, Pin*> STR_PIN_MAP;

typedef std::vector< std::vector<int> > MATRIX;
typedef std::vector< int > RANGE;
typedef std::map< std::string, std::vector<float> > DEVICE_INFO;

