cmake_minimum_required(VERSION 3.22)
project(Nl_Creater)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/database)
include_directories(include/parser)
include_directories(include/util)
include_directories(include/user)

link_directories(lib)

add_executable(Nl_Creater
        include/database/inst.h
        include/database/net.h
        include/database/netlist.h
        include/database/pin.h
        include/user/construct.h
        include/user/function.h
        include/user/mynetlist.h
        include/user/myparts.h
        include/user/mypin.h
        include/user/optimize.h
        include/util/common.h
        include/parser/parser.h
        include/util/common.h
        src/main/main.cpp
        src/main/parser_verilog_file.cpp
        src/user/construct.cpp
        src/user/mynetlist.cpp
        src/user/mypin.cpp
        src/user/optimize.cpp)

target_link_libraries(Nl_Creater libcreater.a)
