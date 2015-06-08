cmake_minimum_required(VERSION 2.8)

############ MACROS ################

project( OpenGL CXX )

SET(OPENGL_SRC
	Camera3D.cpp
	CubeBufferObject.cpp
	FrameBufferObject.cpp
	Frustum.cpp
	GpuTimerQuery.cpp
	OGLShapes.cpp
	OGLStateMachine.cpp
	OrientPos.cpp
	TextureProjection.cpp
	TextureProjectionOrtho.cpp	
)

SET(OPENGL_H
	Camera3D.h
	CubeBufferObject.h
	FrameBufferObject.h
	Frustum.h
	GpuTimerQuery.h
	OGLShapes.h
	OGLStateMachine.h
	OrientPos.h
	TextureProjection.h
	TextureProjectionOrtho.h
)

###########################
## Add GLU dependencies#
###########################
find_package(GLU REQUIRED)
include_directories(${GLU_INCLUDE_PATH})

#######################################################
#### Adding subdirectories (internals libraries)
#######################################################
## OpenGL engine
ADD_SUBDIRECTORY(MathLib)
ADD_SUBDIRECTORY(ShaderManager)
ADD_SUBDIRECTORY(TextureManager)

add_library(
	OpenGL
	${OPENGL_SRC} 
	${OPENGL_H}
)

target_link_libraries(
	OpenGL
	#
	${GLU_LIBRARY}
	#
	MathLib	TextureManager ShaderManager
	)

target_include_directories (OpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})