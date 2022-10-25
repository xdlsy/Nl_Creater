#include "common.h"
#include "mynetlist.h"
#include "parser.h"
#include "construct.h"
#include "optimize.h"

static Nl* core_nl = nullptr;
static MyNetlist* global_nl = nullptr;
static Parser*  global_parser = nullptr;

static void Initialize(const std::string& sourcefile, const std::string& csvfile) {

  core_nl = new Nl();
  global_nl = new MyNetlist(*core_nl);
  global_parser = new Parser();
  global_parser->ReadVerilogFile(sourcefile);

  global_parser->ReadLibCsv(csvfile);
  DEVICE_INFO device_info = global_parser->GetDeviceInfo();
  for(auto& i : device_info){
  	std::cout<<"name:"<<i.first<<" "<<i.second[0]<<" "<<i.second[1]<<std::endl;
  }
  global_nl->SetDeviceInfo(device_info);
};
 
static void Clear() {
  delete global_nl;
  delete global_parser;
};

int main(int argc, char** argv) {

  // Step 1, get file name from command line.
  std::string filename(argv[1]);
  std::string libcsv(argv[2]);

  // Step 2, Interpret input file.
  Initialize(filename, libcsv);

  // Step 3, Construct an answer.
  construct_mux_from_matrix(global_nl, global_parser);

  // Step 4, Optimize
  remove_const_input(global_nl);
  
  // Step 5, Create netlist in official format.

  // Step 6, Convert netlist to verilog and output.
  std::cout<<"begin write verilog"<<std::endl;
  std::string out_file("../case/simple_case/out.v");
  global_nl->WriteVerilog(out_file);

  Clear();
  return 0;
} // end of main

