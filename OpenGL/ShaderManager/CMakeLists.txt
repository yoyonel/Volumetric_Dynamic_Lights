cmake_minimum_required(VERSION 2.8)

############ MACROS ################

project( ShaderManager CXX )

SET(SHADERMANAGER_SRC
	GPUProgram.cpp
	GPUProgramManager.cpp
	GPUShader.cpp
)

SET(SHADERMANAGER_H
	GPUProgram.h
	GPUProgramManager.h
	GPUShader.h
)

add_library(
	ShaderManager
	${SHADERMANAGER_SRC} 
	${SHADERMANAGER_H}
)

target_include_directories (ShaderManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})