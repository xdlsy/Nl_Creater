#pragma once

#include "fstream"

#include "common.h"
#include "pin.h"

class Inst {
  public:
    Inst() { };
    virtual ~Inst(){ };

    inline void SetAreaDelay(const float& area, const float& delay) { area_ = area; delay_ = delay; }

    inline float GetArea() { return area_; }
    inline float GetDelay() { return delay_; }
    virtual inline Pin* GetI0Pin() { return nullptr; }
    virtual inline Pin* GetI1Pin() { return nullptr; }
    virtual inline Pin* GetSelPin() { return nullptr; }
    virtual inline Pin* GetOutPin() { return nullptr; }
    virtual void WriteVerilog(std::ofstream& ofs) { };

  protected:
    float         area_ = 0.0;
    float         delay_ = 0.0;
};

class Mux : public Inst {
  public:
    Mux();
    ~Mux();

    inline Pin*         GetI0Pin() { return i0_; }
    inline Pin*         GetI1Pin() { return i1_; }
    inline Pin*         GetSelPin() { return sel_; }
    inline Pin*         GetOutPin() { return o_; }
    
    // Internal API, Do not use
    void WriteVerilog(std::ofstream& ofs);

  private:
    Pin*         i0_;
    Pin*         i1_;
    Pin*         sel_;
    Pin*         o_;
};

class And : public Inst
{
  public:
    And();
    ~And();

    inline Pin*         GetI0Pin() { return i0_; }
    inline Pin*         GetI1Pin() { return i1_; }
    inline Pin*         GetOutPin() { return o_; }

    // Internal API, Do not use
    void WriteVerilog(std::ofstream& ofs);

  private:
    Pin*         i0_;
    Pin*         i1_;
    Pin*         o_;
};

class Not : public Inst
{
  public:
    Not();
    ~Not();

    inline Pin*         GetI0Pin() { return i_; }
    inline Pin*         GetOutPin() { return o_; }

    // Internal API, Do not use
    void WriteVerilog(std::ofstream& ofs);

  private:
    Pin*         i_;
    Pin*         o_;
};
