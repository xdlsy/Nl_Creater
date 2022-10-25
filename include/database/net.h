// A net has only 1 source pin and >= 1 sink pins

#pragma once

#include "common.h"

class Net
{
public:
  Net(std::string name, Pin* inpin, Pin* outpin) : 
    name_(name), inpin_(inpin) {
      inpin->SetNet(this);
      outpins_.push_back(outpin);
    }
  ~Net() = default;

  // Internal API, Do net use
  inline void AddOutpin(Pin* opin) { outpins_.push_back(opin); }
  inline std::string Name() { return name_; }
  inline Pin* Inpin() { return inpin_; }
  inline PIN_VEC& Outpins() { return outpins_; }

private:
  std::string name_;
  Pin*        inpin_;
  PIN_VEC     outpins_;
};
