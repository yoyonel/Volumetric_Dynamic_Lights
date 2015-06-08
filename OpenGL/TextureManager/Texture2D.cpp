
#include "Texture2D.h"

#ifdef __linux__
	#include <cstring>
#endif

Texture2D::Texture2D() : Texture()
{
	this->target = GL_TEXTURE_2D;
	
	//for alpha channel generation
	this->generateAlpha = false;
	this->alphaColor_r = this->alphaColor_g = this->alphaColor_b = 0;
	this->alphaValue = 0;
	this->noAlphaValue = 255;
}

Texture2D::Texture2D(Texture2D&)
{
}

Texture2D::~Texture2D()
{
	//we delete texture from the video memory
	this->lockedAttrib = false;
	this->lockedInMem = false;
	this->unload();
}

bool Texture2D::genTexture(const char* name, const char* path)
{
	//texture already loaded, we can only modified its attributes
	if(this->loaded) return false;

	//no empty namme
	if(strlen(name)==0) return false;

	//file(s) exist
	FILE* f = fopen(path,"r");
	if(f==NULL)
	{
		return false;
	}
	fclose(f);

	this->name = name;
	this->path = path;
	this->loaded = true;
	return true;
}

void Texture2D::loadInVideoMem()
{
	unsigned int finalWidth;
	unsigned int finalHeight;
	unsigned int finalWidthAlpha;
	unsigned int finalHeightAlpha;
	GLubyte* scaledImage = NULL;
	GLubyte* scaledImageAlpha = NULL;
	//GLubyte* alphaImage = NULL;
	GLubyte* finalImage = NULL;
	Image* img = NULL;
	Image* imgAlpha = NULL;

	//
	if(!this->loaded || this->loadedInVideoMem) return;

	const char *pc = path.c_str() + strlen(path.c_str())-3;
	if(strcmp(pc,"bmp")==0)
		img = new ImageBMP();
	else if(strcmp(pc,"tga")==0)
		img = new ImageTGA();

	//alpha texture loading if needed
	if(this->useAlphaTexture)
	{
		const char *pc = alphaTexturePath.c_str() + strlen(alphaTexturePath.c_str())-3;
		if(strcmp(pc,"bmp")==0)
			imgAlpha = new ImageBMP();
		else if(strcmp(pc,"tga")==0)
			imgAlpha = new ImageTGA();
	}

	if(img->load(path.c_str()))
	{
		//good alpha texture specification?
		if(this->useAlphaTexture)
		{
			if(!imgAlpha->load(this->alphaTexturePath.c_str()) || imgAlpha->getWidth()!=img->getWidth() || imgAlpha->getHeight()!=img->getHeight() )
			{
				//image not compatible
				delete imgAlpha;
				this->useAlphaTexture = false;
				this->generateAlpha = true;
				this->alphaColor_r= this->alphaColor_g = this->alphaColor_b = 0;
				this->alphaValue = this->noAlphaValue = 127;
			}
		}

		//
		//we scale image for wished quality
		//
		finalWidth = img->getWidth();
		finalHeight = img->getHeight();
		finalWidthAlpha = img->getWidth();
		finalHeightAlpha = img->getHeight();
		if(quality>0)
		{
			for(int i=0;i<quality;i++)
			{
				finalWidth /=2;
				finalHeight /=2;
				finalWidthAlpha /=2;
				finalHeightAlpha /=2;
			}
			if(finalWidth<1 ) finalWidth  = 1;
			if(finalHeight<1) finalHeight = 1;
			if(finalWidthAlpha<1 ) finalWidthAlpha  = 1;
			if(finalHeightAlpha<1) finalHeightAlpha = 1;

			scaledImage = new GLubyte[(finalWidth+1)*(finalHeight+1)*3];
			gluScaleImage(GL_RGB,img->getWidth(),img->getHeight(),img->getType(),img->getDatasPointer(),
				finalWidth,finalHeight,GL_UNSIGNED_BYTE,scaledImage);

			if(this->useAlphaTexture)
				scaledImageAlpha = new GLubyte[(finalWidthAlpha+1)*(finalHeightAlpha+1)*3];
				gluScaleImage(GL_RGB,imgAlpha->getWidth(),imgAlpha->getHeight(),imgAlpha->getType(),imgAlpha->getDatasPointer(),
					finalWidthAlpha,finalHeightAlpha,GL_UNSIGNED_BYTE,scaledImageAlpha);
		}
		else
		{
			scaledImage = img->getDatasPointer();
			if(this->useAlphaTexture)
				scaledImageAlpha = imgAlpha->getDatasPointer();
		}

		//
		//compute alpha channel if needed
		//
		if(useAlphaTexture)
		{
			////////////////////////////////////////////////////////   TODO
			finalImage = new GLubyte[finalWidth*finalHeight*4];
			for(int i=0;i<finalWidth*finalHeight;i++)
			{
				finalImage[i*4+3] = scaledImageAlpha[i*3];
				//les couleurs
				finalImage[i*4]   = scaledImage[i*3];
				finalImage[i*4+1] = scaledImage[i*3+1];
				finalImage[i*4+2] = scaledImage[i*3+2];
			}
		}
		else
		{
			if(generateAlpha)
			{
				finalImage = new GLubyte[finalWidth*finalHeight*4];
				for(int i=0;i<finalWidth*finalHeight;i++)
				{
					if( scaledImage[i*3]==this->alphaColor_r && scaledImage[i*3+1]==this->alphaColor_g && scaledImage[i*3+2]==this->alphaColor_b )
						finalImage[i*4+3] = this->alphaValue;
					else
						finalImage[i*4+3] = this->noAlphaValue;
					finalImage[i*4]   = scaledImage[i*3];
					finalImage[i*4+1] = scaledImage[i*3+1];
					finalImage[i*4+2] = scaledImage[i*3+2];
				}
			}
			else
				finalImage = scaledImage;
		}


		this->enableTexture();

		//
		glGenTextures(1,&this->id);
		//
		glBindTexture(this->target, this->id);

		//
		glTexParameteri(this->target,GL_TEXTURE_WRAP_S,this->textureWrapS);
		glTexParameteri(this->target,GL_TEXTURE_WRAP_T,this->textureWrapT);
		//
		glTexParameteri(this->target,GL_TEXTURE_MAG_FILTER,this->textureMagFilter);
		glTexParameteri(this->target,GL_TEXTURE_MIN_FILTER,this->textureMinFilter);

		//the anisotropy level is controlled by the TextureManager
		glTexParameteri(this->target,GL_TEXTURE_MAX_ANISOTROPY_EXT,this->textureAnisotropy);

		this->width = finalWidth;
		this->heigth = finalHeight;
		this->depth = 0;
		
		if(		this->textureMinFilter==GL_NEAREST_MIPMAP_NEAREST || this->textureMinFilter==GL_NEAREST_MIPMAP_LINEAR
			||	this->textureMinFilter==GL_LINEAR_MIPMAP_NEAREST  || this->textureMinFilter==GL_LINEAR_MIPMAP_LINEAR  )
		{//we use mipmap
			if(this->generateAlpha || this->useAlphaTexture)
			{//we have an alpha channel
				gluBuild2DMipmaps(this->target,GL_RGBA8,finalWidth,finalHeight,GL_RGBA,GL_UNSIGNED_BYTE,finalImage);
				this->internalFormat = GL_RGBA8;
			}
			else
			{//no alpha channel
				gluBuild2DMipmaps(this->target,GL_RGB8,finalWidth,finalHeight,GL_RGB,GL_UNSIGNED_BYTE,finalImage);
				this->internalFormat = GL_RGB8;
			}
		}
		else
		{//we don't use mipmap
			if(this->generateAlpha || this->useAlphaTexture)
			{//we have an alpha channel
				glTexImage2D(this->target,0,GL_RGBA8,finalWidth,finalHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,finalImage);
				this->internalFormat = GL_RGBA8;
			}
			else
			{//no alpha channel
				glTexImage2D(this->target,0,GL_RGB,finalWidth,finalHeight,0,GL_RGB,GL_UNSIGNED_BYTE,finalImage);
				this->internalFormat = GL_RGB8;
			}
		}

		this->loadedInVideoMem = true;
		this->disableTexture();

		if(quality>0)
		{
			delete [] scaledImage;
			if(this->useAlphaTexture)
				delete [] scaledImageAlpha;
		}
		if(generateAlpha || useAlphaTexture)
			delete [] finalImage;
	}

	delete img;
	if(this->useAlphaTexture)
		delete imgAlpha;
}

void Texture2D::reload()
{
	//if the texture have not been loaded, we don't do anything
	if(!this->loadedInVideoMem) return;

	//can we modify the texture attributes?
	if(this->lockedAttrib) return;

	//we destroy the OpenGL object...
	this->enableTexture();
	glDeleteTextures(1,&this->id);
	this->disableTexture();
	this->loadedInVideoMem = false;
	//...then we reload it!
	this->loadInVideoMem();
}

void Texture2D::unload(bool force)
{
	//	We unload the texture from video memory only if it's in the video memory, if we can
	//modify its attributs and it's not lock in video memory. OR if the unload is forced. 
	if( (this->loadedInVideoMem && !this->lockedAttrib && !this->lockedInMem) || force)
	{
		//on détruit l'objet de texture
		this->enableTexture();
		glDeleteTextures(1,&this->id);
		this->disableTexture();
		this->loadedInVideoMem = false;
	}
}