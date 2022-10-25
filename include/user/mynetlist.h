#pragma once
#include "netlist.h"
#include "mypin.h"
#include "myparts.h"
#include <set>
#include <unordered_set>

class MyNetlist
{
    
public:
    MyNetlist(Nl &netlist) : netlist(netlist) {
        n_Net = 0;
    }
    ~MyNetlist(){
    }
// Inherited interface
    MyMux *CreateMux(int index);
    MyAnd *CreateAnd(int index);
    MyNot *CreateNot();
    Net *Connect(MyPin *from, MyPin *to);
    MyPin *CreateModelInPin(const unsigned &index, bool add_reverse);
    MyPin *CreateModelOutPin(const unsigned &index);
    MyPin *CreateModelSelPin(const unsigned &index);
    void WriteVerilog(const std::string &filename);

public:
  std::string GetWireDecl();
  void SetModelDecl(const std::string& ms);
  void SetDeviceInfo(const DEVICE_INFO& device_info);
  void AddInPin(const unsigned& index, MyPin* din);
// New methods
public:
    std::map<int, MyMux*>& GetId2Mux() {return id2Mux;}
    std::map<MyMux*, int>& GetId2Not() {return mux2Id;}
    std::map<int, MyPin*>& GetId2Sel() {return id2Sel;}
    std::map<MyPin*, int>& GetSel2Id() {return sel2Id;}
    std::map<int, MyPin* >& GetId2OutPin() {return id2OutPin;}
    std::map<int, MyPin* >& GetId2InPin() {return id2InPin;}
    
    void Disconnect(MyPin *from, MyPin *to);
private:
    std::map<int, MyAnd *> id2and;
    std::map<MyAnd *, int> and2id;
    std::map<int, MyNot*> id2not;
    std::map<MyNot*, int> not2id;
 

    std::map<int, MyMux*> id2Mux;
    std::map<MyMux*, int> mux2Id;
    std::map<int, MyPin *> id2Sel, id2InPin, id2OutPin;
    std::map<MyPin *, int> sel2Id, inPin2Id, outPin2Id;
    
    std::map<int, Net*> id2Net;
    std::map<Net*, int> net2Id;

    Nl &netlist;
    int n_Net;

public:
    // for optimization
    std::set<MyPart*> const_mux; //mux connect with const input.

    std::unordered_set<MyPart*> top_mux;
};