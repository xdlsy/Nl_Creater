#include "mypin.h"
#include "pin.h"


    // Public interface
    void MyPin::Tie0(){
        pin->Tie0();
    }
    void MyPin::Tie1(){
        pin->Tie1();
    }
    bool MyPin::IsMpin(){
        return pin->IsMpin();
    }
    std::string MyPin::Name(){
        return name;
    }
    bool MyPin::IsDangling(){
        return pin->IsDangling();
    }
    bool MyPin::IsConst0(){
        return IsConst0();
    }
    bool MyPin::IsConst1(){
        return IsConst1();
    }
    void MyPin::SetNet(Net *net){
        pin->SetNet(net);
    }
    Net *MyPin::GetNet(){
        return pin->GetNet();
    }
    // Interface end

    // Added function
    void MyPin::SetName(std::string n){
        name = n;
    }
    Pin* MyPin::GetPin(){
        return pin;
    }
