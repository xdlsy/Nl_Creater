#pragma once

#include "common.h"

class Pin
{
protected:
  enum PIN_VAL {
    CONST0 = 0,
    CONST1 = 1,
    NET_VAL,
    UNKNOWN
  };

public:
  Pin(const bool& is_drive);
  virtual ~Pin();

  void Tie0(); // pin connect const 0
  void Tie1(); // pin connect const 1

  // Internal API , Do not use
  virtual bool IsMpin();
  virtual std::string Name();
  bool IsDangling();
  bool IsConst0();
  bool IsConst1();
  void SetNet(Net* net);
  Net* GetNet();

private:
  bool          is_drive_;
  PIN_VAL       type_;
  Net*          net_;
};

class MPin : public Pin 
{
public:
  MPin(const bool& is_drive, const std::string& name);
  ~MPin();

  bool        IsMpin();
  std::string Name();

private:
  std::string  name_;
};

class InstPin : public Pin 
{
public:
  InstPin(const bool& is_drive);
  ~InstPin();
};
