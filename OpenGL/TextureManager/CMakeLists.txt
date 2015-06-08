cmake_minimum_required(VERSION 2.8)

############ MACROS ################

project( TextureManager CXX )

SET(TEXTUREMANAGER_SRC
	ImageBMP.cpp
	Image.cpp
	ImageTGA.cpp
	Texture2D.cpp
	Texture3DEXT.cpp
	Texture.cpp
	TextureCubeMapARB.cpp
	TextureManager.cpp
)

SET(TEXTUREMANAGER_H
	ImageBMP.h
	Image.h
	ImageTGA.h
	Texture2D.h
	Texture3DEXT.h
	TextureCubeMapARB.h
	Texture.h
	TextureManager.h
)

add_library(
	TextureManager
	${TEXTUREMANAGER_SRC} 
	${TEXTUREMANAGER_H}
)

target_include_directories (
	TextureManager 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
