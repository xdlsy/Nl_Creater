// Netlist class: consists of all APIs to manipulate netlist elements

#pragma once

#include "common.h"
#include "inst.h"
#include "net.h"
#include "pin.h"

class Nl
{
public:
  static std::string DEVICE_MUX_NAME;
  static std::string DEVICE_AND_NAME;
  static std::string DEVICE_NOT_NAME;

public:
  Nl() {}
  ~Nl() {}
  // available APIs for construct netlists
  Inst* CreateMux();
  Inst* CreateAnd();
  Inst* CreateNot();
  Net*  Connect(Pin* from, Pin* to);
  Pin*  CreateModelInPin(const unsigned& index);
  Pin*  CreateModelOutPin(const unsigned& index);
  Pin*  CreateModelSelPin(const unsigned& index);
  void  WriteVerilog(const std::string& filename);

  // Internal API , Do not use
  std::string GetWireDecl();
  inline void SetModelDecl(const std::string& ms) { module_decl_ = ms; };
  inline void SetDeviceInfo(const DEVICE_INFO& device_info) { device_info_ = device_info; }

private:
  float GetInstArea(const std::string& device_name);
  float GetInstDelay(const std::string& device_name);
  
private:
  INST_VEC      insts_;
  NET_VEC       nets_;
  STR_PIN_MAP   mpins_;
  std::string   module_decl_; 
  DEVICE_INFO   device_info_;
};
