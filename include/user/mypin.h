#pragma once
#include "pin.h"
#include <set>

class MyPart;
class MyPin
{
public:
    MyPin(Pin *pin) : pin(pin)
    {
    }
    // Public interface
    void Tie0(); // pin connect const 0
    void Tie1(); // pin connect const 1
    virtual bool IsMpin();
    virtual std::string Name();
    bool IsDangling();
    bool IsConst0();
    bool IsConst1();
    void SetNet(Net *net);
    Net *GetNet();
    // Interface end

    // Added function
    MyPin* GetInPin();
    std::set<MyPin*>& GetOutPins(){}
    void SetName(std::string n);
    Pin* GetPin();
    MyPart* GetPart(){return part;}
    void SetPart(MyPart* P){part = P;}
private:
    std::string name;
    Pin *pin;
    MyPart* part;
};