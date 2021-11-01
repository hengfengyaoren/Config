#工程名称
PROJECT(INSTANCE_SERVER)
#cmake版本限制
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
#指定编译器
SET(CMAKE_CXX_COMPILER "g++")

SET(PROJDIR ${CMAKE_CURRENT_SOURCE_DIR}/)

SET(EXECUTABLE_OUTPUT_PATH  "${PROJECT_SOURCE_DIR}")

SET(EXEC "Main")

#设置-std=c++11
SET(CXX_FLAGS 
	-std=c++14 
	-O2 
	-Wall
   	-Werror	
	-Wno-unused-but-set-variable 
	-Wno-deprecated-declarations
	-Wno-unused-function
	-fPIC
	)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

ADD_EXECUTABLE(${EXEC} ${DIR_SRCS})

