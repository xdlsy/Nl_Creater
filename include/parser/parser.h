#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include "common.h"

/*
 * @class  Parser
 * @brief  verilog parser
 * @note   analyze/elaborate specific verilog file and provide corresponding addr/dataMat,
 *         Refer to the manual for detailed document formats
 */
class Parser
{
public:
  Parser() = default;
  ~Parser() = default;
  bool     ReadVerilog(const std::string& filename);
  bool     ReadLibCsv(const std::string& filename);
  MATRIX   GetDataMat();
  MATRIX   GetAddrMat();
  inline RANGE GetDinRange() { return _din_port_range; }
  inline RANGE GetDoutRange() { return _dout_port_range; }
  inline RANGE GetSelRange() { return _sel_port_range; }

  // Internal API, Do not use
  inline std::string GetModelDecl() { return _model_decl; };
  inline DEVICE_INFO GetDeviceInfo() { return _device_info; };

private:
  bool     Analyze(const std::string& filename); // Parse the verilog statement to obtain the corresponding data structure
  bool     Elaborate(); // Transform the CASE statement to get the corresponding matrix
  void     Clear();
  void     AddPortRange(const std::string& port_str);
  void     AddAddrMatrix(const std::string& condi_it);
  void     AddDataMatrix(const std::string& condi_it);

private:
  std::string     _model_decl;
  std::string     _port_decl;
  std::string     _statement_str;
  RANGE           _din_port_range;
  RANGE           _dout_port_range;
  RANGE           _sel_port_range;
  MATRIX          _data_matrix;
  MATRIX          _addr_matrix;
  DEVICE_INFO     _device_info; // device name -> <area , delay> map

public:
// 为了效率，改为返回引用
  MATRIX&   DataMat(){return _data_matrix;};
  MATRIX&   AddrMat(){return _addr_matrix;};
// 提供的解析函数太慢了，改用siyuan写的

    void ReadVerilogFile(const std::string& filename) {
        std::ifstream in(filename);
        std::string s;
        //获取sel、din、dout大小
        int sel_n, din_n, dout_n;
        auto GetPinSize = [&](const std::string& pin, int& size) {
            size = 0;
            while (s.find(pin) == std::string::npos)
                std::getline(in, s);
            int begin = s.find('[') + 1;
            while (s[begin] != ':')
                size = size * 10 + (s[begin++] - 48);
            ++size;
        };
        GetPinSize("sel", sel_n);
        _sel_port_range.push_back(sel_n);
        _sel_port_range.push_back(0);
        GetPinSize("din", din_n);
        _din_port_range.push_back(din_n);
        _din_port_range.push_back(0);
        GetPinSize("dout", dout_n);
        _dout_port_range.push_back(dout_n);
        _dout_port_range.push_back(0);

        //获取sel矩阵和dout矩阵
        while (s.find("'") == std::string::npos)
            std::getline(in, s);
        std::string::size_type sel_pos = s.find('b') + 1;
        std::string::size_type dout_pos = s.find('{') + 1;
        while (s.find("default") == std::string::npos) {
            int start_pos = sel_pos;
            std::vector<int> sel_row(sel_n);
            for (int i = 0; i < sel_n; ++i, ++start_pos) {
                if (s[start_pos] == '?')
                    sel_row[i] = -1;
                else
                    sel_row[i] = s[start_pos] - 48;
            }
            _addr_matrix.push_back(sel_row);

            start_pos = dout_pos;
            std::vector<int> dout_row(dout_n);
            for (int i = 0; i < dout_n; ++i) {
                if (s[start_pos] == '1') {
                    start_pos += 3;
                    dout_row[i] = s[start_pos] - 48;
                } else {
                    int neg = 0;
                    if (s[start_pos] == '~')
                        neg = 1;
                    start_pos += (4 + neg);
                    int num = 0;
                    while (s[start_pos] != ']')
                        num = num * 10 + s[start_pos++] - 48;
                    num = 2 * num + 2 + neg;
                    dout_row[i] = num;
                }
                start_pos += 3;
            }
            _data_matrix.push_back(dout_row);
            std::getline(in, s);
        }
        while (s.find("default") == std::string::npos)
            std::getline(in, s);
        dout_pos = s.find('{') + 1;
        std::vector<int> dout_row(dout_n);
        for (int i = 0; i < dout_n; ++i) {
            if (s[dout_pos] == '1') {
                dout_pos += 3;
                dout_row[i] = s[dout_pos] - 48;
            } else {
                int neg = 0;
                if (s[dout_pos] == '1' == '~')
                    neg = 1;
                dout_pos += (3 + neg);
                int num = 0;
                while (s[dout_pos] != ']')
                    num = num * 10 + s[dout_pos++];
                num = 2 * num + 2 + neg;
                dout_row[i] = num;
            }
            dout_pos += 3;
        }
        _data_matrix.push_back(dout_row);
    }

};