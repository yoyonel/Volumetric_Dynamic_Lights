
#include "Texture.h"



Texture::Texture()
{
	this->id = 0;

	this->name = "TextureNotLoaded";
	this->path = ".";

	this->target = 0;
	this->internalFormat = 0;

	this->textureWrapS = this->textureWrapT = this->textureWrapR = GL_REPEAT;

	this->textureMinFilter = this->textureMagFilter = GL_LINEAR;
	this->textureAnisotropy = 1;

	this->quality = 0;
	this->heigth = this->width = this->heigth = 0;

	this->lockedAttrib = this->lockedInMem = this->loaded = this->loadedInVideoMem = false;
}
Texture::Texture(Texture&)
{
}
Texture::~Texture()
{
	this->unload(true);
}


std::string Texture::getName() const
{
	return this->name;
}

unsigned int Texture::getWidth() const
{
	return this->width;
}

unsigned int Texture::getHeigth() const
{
	return this->heigth;
}

unsigned int Texture::getDepth() const
{
	return this->depth;
}

//
void Texture::loadInVideoMem(){}
void Texture::reload(){}
void Texture::unload(bool force){}