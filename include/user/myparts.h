#pragma once
#include "inst.h"
#include "mypin.h"

class MyPart{

public:
    virtual ~MyPart();

};
class MyAnd : public MyPart
{
  public:
    MyAnd(Inst* ad){
      _and = ad;
      i0_ = new MyPin(ad->GetI0Pin());
      i1_ = new MyPin(ad->GetI1Pin());  
      o_ = new MyPin(ad->GetOutPin());  
      i0_->SetPart(this);
      i1_->SetPart(this);
      o_->SetPart(this);
    }
    MyPin* GetI0Pin(){return i0_;}
    MyPin* GetI1Pin(){return i1_;}
    MyPin* GetOutPin(){return o_;}

  private:
    Inst* _and;
    MyPin* i0_;
    MyPin* i1_;
    MyPin* o_;

};

class MyNot : public MyPart
{
  public:
    MyNot(Inst* nt) {
        _not = nt;
        i_ = new MyPin(nt->GetI0Pin());
        o_ = new MyPin(nt->GetOutPin());
        i_->SetPart(this);
        o_->SetPart(this);
    }
    ~MyNot(){delete _not;};
    MyPin* GetI0Pin() {return i_;};
    MyPin* GetOutPin() {return o_;};
    // void WriteVerilog(std::ofstream& ofs){_not->WriteVerilog(ofs);}

  private:
    Inst* _not;
    MyPin*         i_;
    MyPin*         o_;
};

class MyMux : public MyPart{
public:
    MyMux(Inst* mux) : mux(mux){
        i0_ = new MyPin(mux->GetI0Pin());
        i1_ = new MyPin(mux->GetI1Pin());
        sel_ = new MyPin(mux->GetSelPin());
        o_ = new MyPin(mux->GetOutPin());
        i0_->SetPart(this);
        i1_->SetPart(this);
        sel_->SetPart(this);
        o_->SetPart(this);
    }

    MyPin*         GetI0Pin() { return i0_; }
    MyPin*         GetI1Pin() { return i1_; }
    MyPin*         GetSelPin() { return sel_; }
    MyPin*         GetOutPin() { return o_; }
    void WriteVerilog(std::ofstream& ofs) {return mux->WriteVerilog(ofs);}
    
private:
    Inst* mux;

    MyPin*         i0_;
    MyPin*         i1_;
    MyPin*         sel_;
    MyPin*         o_;
};