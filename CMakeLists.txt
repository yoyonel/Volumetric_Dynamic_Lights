cmake_minimum_required(VERSION 2.8)

#SET( CMAKE_BUILD_TYPE "DEBUG" )
#SET( CMAKE_BUILD_TYPE "RELEASE")

############ MACROS ################

PROJECT( Volumetric_Dynamic_Lights )

set(CTEST_BUILD_FLAGS "-i -j 12")

############ SOURCES ################

SET(VOLUMETRIC_DYNAMIC_LIGHTS_SRC
    main.cpp
)

SET(VOLUMETRIC_DYNAMIC_LIGHTS_SHADERS
    Resources/Shaders/bilatDirBlur2.fp
    Resources/Shaders/bilatDirBlur.fp
    Resources/Shaders/composite.fp
    Resources/Shaders/depthDownSample.fp
    Resources/Shaders/downsample.fp
    Resources/Shaders/Shader.fp
    Resources/Shaders/temporalBlend.fp
    Resources/Shaders/temporalUpsampling.fp
    Resources/Shaders/VolumetricPointLight.fp
    Resources/Shaders/bilatDirBlur2.vp
    Resources/Shaders/bilatDirBlur.vp
    Resources/Shaders/composite.vp
    Resources/Shaders/depthDownSample.vp
    Resources/Shaders/downsample.vp
    Resources/Shaders/Shader.vp
    Resources/Shaders/temporalBlend.vp
    Resources/Shaders/temporalUpsampling.vp
    Resources/Shaders/VolumetricPointLight.vp
)

#######################################################
#### Finding Packages 
#######################################################

SET( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH} )
#
SET( CMAKE_PREFIX_PATH /usr/local/lib64 )

###########################
## Add OpenGL dependencies#
###########################
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

###########################
## Add GLU dependencies#
###########################
find_package(GLU REQUIRED)
include_directories(${GLU_INCLUDE_PATH})
#MESSAGE(GLU_LIBRARY: ${GLU_LIBRARY})
#MESSAGE(GLU_INCLUDE_DIRS: ${GLU_INCLUDE_PATH})

###########################
## Add GLUT dependencies#
###########################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})

###########################
## Add GLEW dependencies#
###########################
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

#######################################################
#### Adding subdirectories (internals libraries)
#######################################################
## OpenGL engine
ADD_SUBDIRECTORY(OpenGL)

#######################################################
#### Fixing somes bugs with MSVC
#######################################################
## MOG: fix problème de compilation sous MSVC :/
## http://comments.gmane.org/gmane.comp.graphics.openscenegraph.user/70551
## http://forum.osgearth.org/SVN-Build-errors-using-VS10-Win32-td6847842.html
if(MSVC)
  if(${MSVC_VERSION} STREQUAL 1600)
    message("msvc2010 build fix /FORCE:MULTIPLE")
    set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /FORCE:MULTIPLE" )
  endif()
endif()

## Activate C++11 (gcc) support
# IF(CMAKE_COMPILER_IS_GNUCXX)
# 	MESSAGE( "Activate C++11 compiler" )
# 	ADD_DEFINITIONS(-std=c++11) 
# ENDIF(CMAKE_COMPILER_IS_GNUCXX)



#######################################################
#### Building the executable program
#######################################################
add_executable(
    Volumetric_Dynamic_Lights
    ##
    ${VOLUMETRIC_DYNAMIC_LIGHTS_SRC}
    ${VOLUMETRIC_DYNAMIC_LIGHTS_SHADERS}
    )

target_link_libraries( 
    Volumetric_Dynamic_Lights
    ##
    #${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${GLU_LIBRARY}
    ${GLEW_LIBRARIES}
    ##
    OpenGL
    ##
)

