#include "mynetlist.h"

using namespace std;
MyMux *MyNetlist::CreateMux(int sel)
{
    Inst *mux = netlist.CreateMux();
    MyMux* mymux = new MyMux(mux);
    
    int index = mux2Id.size();
    std::string name_base = std::string("mux_")+std::to_string(index);
    mymux->GetI0Pin()->SetName(name_base + "_I0");
    mymux->GetI1Pin()->SetName(name_base + "_I1");
    mymux->GetOutPin()->SetName(name_base + "_Out");
    mymux->GetSelPin()->SetName(name_base + "_Sel");

    id2Mux[index] = mymux;
    mux2Id[mymux] = index;
    return mymux;
}

MyAnd *MyNetlist::CreateAnd(int index)
{
    Inst *ad = netlist.CreateAnd();
    MyAnd * myand = new MyAnd(ad);
    id2and[index] = myand;
    and2id[myand] = index;
    return myand;
}

MyNot *MyNetlist::CreateNot()
{
    int idx = not2id.size();
    MyNot *not_ = new MyNot(netlist.CreateNot());
    id2not[idx] = not_;
    not2id[not_] = idx;
    return not_;
}

Net *MyNetlist::Connect(MyPin *from, MyPin *to)
{
    Net *net = netlist.Connect(from->GetPin(), to->GetPin());
    id2Net[n_Net] = net;
    net2Id[net] = n_Net;
    n_Net++;
    return net;
}
MyPin *MyNetlist::CreateModelInPin(const unsigned &index, bool add_reverse)
{
    // din
    Pin *pin = netlist.CreateModelInPin(index);
    MyPin* mypin = new MyPin(pin);
    mypin->SetName("in_" + std::to_string(index));
    id2InPin[index*2+2] = mypin;
    inPin2Id[mypin] = index*2+2;
    
    // ~din
    if(add_reverse){
        MyNot* not_ = CreateNot();
        std::cout<<"create not:"<<index*2+3<<" "<<not_<<std::endl;
        Connect(mypin, not_->GetI0Pin());
        // Connect(not_->GetOutPin(), CreateModelOutPin(99999));
        id2InPin[index*2+3] = not_->GetOutPin();
        inPin2Id[not_->GetOutPin()] = index*2+3;
    }
    return mypin;
}

void MyNetlist::AddInPin(const unsigned& index, MyPin* din){
    id2InPin[index] = din;
    inPin2Id[din] = index;
}

MyPin *MyNetlist::CreateModelOutPin(const unsigned &index)
{
    Pin *pin = netlist.CreateModelOutPin(index);
    MyPin* mypin = new MyPin(pin);
    mypin->SetName("out_" + std::to_string(index));
    id2OutPin[index] = mypin;
    outPin2Id[mypin] = index;
    return mypin;
}

MyPin *MyNetlist::CreateModelSelPin(const unsigned &index)
{
    Pin *pin = netlist.CreateModelSelPin(index);
    MyPin* mypin = new MyPin(pin);
    mypin->SetName("sel_" + std::to_string(index));
    id2Sel[index] = mypin;
    sel2Id[mypin] = index;
    return mypin;
}

void MyNetlist::WriteVerilog(const string &filename)
{
    netlist.WriteVerilog(filename);
}

std::string MyNetlist::GetWireDecl(){
    return netlist.GetWireDecl();
}

void MyNetlist::SetModelDecl(const string& ms){
    netlist.SetModelDecl(ms);
}

void MyNetlist::SetDeviceInfo(const DEVICE_INFO& device_info){
    netlist.SetDeviceInfo(device_info);
}

