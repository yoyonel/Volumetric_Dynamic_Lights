cmake_minimum_required(VERSION 2.8)

############ MACROS ################

project( MathLib CXX )

SET(MATHLIB_SRC
	MathLib.cpp
	MathLibFunc.cpp
	Matrix4.cpp
	Plane.cpp
	Quaternion.cpp
	Sphere.cpp
	Vector2.cpp
	Vector3.cpp
	Vector4.cpp
)

SET(MATHLIB_H
	MathLibFunc.h
	MathLib.h
	Matrix4.h
	Plane.h
	Quaternion.h
	Sphere.h
	Vector2.h
	Vector3.h
	Vector4.h
)

#set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../CMakeModules ${CMAKE_MODULE_PATH} )

#find_package(OpenSceneGraph REQUIRED)
#FIND_PACKAGE(OSG REQUIRED)
#include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

#find_package(Shapelib REQUIRED)
#include_directories(${SHAPELIB_INCLUDE_DIR})

add_library(
	MathLib
	${MATHLIB_SRC} 
	${MATHLIB_H}
)
	
#target_link_libraries( 
#	osgSEG 
#	${OPENSCENEGRAPH_LIBRARIES} 
#	${SHAPELIB_LIBRARY} 
#)

target_include_directories (MathLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})