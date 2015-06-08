
#include "TextureCubeMapARB.h"


/** Given a cube map face index, cube map size, and integer 2D face position,
 * return the cooresponding normalized vector.
 *
 *	COME FROM NVIDIA DEMO ON SHADOW VOLUME!!!
 *
 * @param face		: face number
 * @param cubesize	: face resolution
 * @param x			: s coordinate in texel
 * @param y			: t coordinate in texel
 * @param vector	: result vector
 */
void getCubeVector(int face, int cubesize, int x, int y, float* vector)
{
  float s, t, sc, tc, mag;

  s = ((float)x + 0.5f) / (float)cubesize;
  t = ((float)y + 0.5f) / (float)cubesize;
  sc = s*2.0f - 1.0f;
  tc = t*2.0f - 1.0f;

  switch (face) {
  case 0:
    vector[0] = 1.0;
    vector[1] = -tc;
    vector[2] = -sc;
    break;
  case 1:
    vector[0] = -1.0;
    vector[1] = -tc;
    vector[2] = sc;
    break;
  case 2:
    vector[0] = sc;
    vector[1] = 1.0;
    vector[2] = tc;
    break;
  case 3:
    vector[0] = sc;
    vector[1] = -1.0;
    vector[2] = -tc;
    break;
  case 4:
    vector[0] = sc;
    vector[1] = -tc;
    vector[2] = 1.0;
    break;
  case 5:
    vector[0] = -sc;
    vector[1] = -tc;
    vector[2] = -1.0;
    break;
  }

  mag = 1.0f/sqrtf(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
  vector[0] *= mag;
  vector[1] *= mag;
  vector[2] *= mag;
}

TextureCubeMapARB::TextureCubeMapARB() : Texture()
{
	this->target = GL_TEXTURE_CUBE_MAP_ARB;
	this->isNormalizedCubeMap = false;
}

TextureCubeMapARB::TextureCubeMapARB(TextureCubeMapARB&)
{
}

TextureCubeMapARB::~TextureCubeMapARB()
{
	//we delete texture from the video memory
	this->lockedAttrib = false;
	this->lockedInMem = false;
	this->unload();
}

bool TextureCubeMapARB::genCubeMap(const char* name, 
		const char* xn, const char* xp, const char* yn, const char* yp, const char* zn, const char* zp)
{
	//texture already loaded, we can only modified its attributes
	if(this->loaded || name==NULL || xn==NULL || xp==NULL || yn==NULL || yp==NULL || zn==NULL || zp==NULL) return false;

	if(strlen(name)==0) return false;

	face_xn = xn;
	face_xp = xp;
	face_yn = yn;
	face_yp = yp;
	face_zn = zn;
	face_zp = zp;

	//do all files exist?
	FILE* f = fopen(xn,"r");
	if(f==NULL)
	{
		return false;
	}
	fclose(f);
	f = fopen(xp,"r");
	if(f==NULL)
	{
		return false;
	}
	fclose(f);
	f = fopen(yn,"r");
	if(f==NULL)
	{
		return false;
	}
	fclose(f);
	f = fopen(yp,"r");
	if(f==NULL)
	{
		return false;
	}
	fclose(f);
	f = fopen(zn,"r");
	if(f==NULL)
	{
		return false;
	}
	fclose(f);
	f = fopen(zp,"r");
	if(f==NULL)
	{
		return false;
	}
	fclose(f);

	this->name = name;
	this->path = path;
	this->loaded = true;
	this->isNormalizedCubeMap = false;
	return true;
}

void TextureCubeMapARB::loadInVideoMem()
{
	//already loaded or assigned
	if(!this->loaded || this->loadedInVideoMem) return;
	
	this->enableTexture();

	glGenTextures(1,&this->id);
	glBindTexture(this->target, this->id);

	//
	glTexParameteri(this->target,GL_TEXTURE_WRAP_S,this->textureWrapS);
	glTexParameteri(this->target,GL_TEXTURE_WRAP_T,this->textureWrapT);
	//
	glTexParameteri(this->target,GL_TEXTURE_MAG_FILTER,this->textureMagFilter);
	glTexParameteri(this->target,GL_TEXTURE_MIN_FILTER,this->textureMinFilter);

		//the anisotropy level is controlled by the TextureManager
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_MAX_ANISOTROPY_EXT,this->textureAnisotropy);

	if(this->isNormalizedCubeMap)
	{
		float vector[3];
		int i, x, y;
		GLubyte *pixels = new GLubyte[this->width*this->width*3];
		
		for (i = 0; i < 6; i++)
		{
			for (y = 0; y < this->width; y++)
			{
				for (x = 0; x < this->width; x++)
				{
					getCubeVector(i, this->width, x, y, vector);
					//negative values to have the vector from surface to light
					pixels[3*(y*this->width+x) + 0] = 128 - 127*vector[0];
					pixels[3*(y*this->width+x) + 1] = 128 - 127*vector[1];
					pixels[3*(y*this->width+x) + 2] = 128 - 127*vector[2];
				}
			}
			if(		this->textureMinFilter==GL_NEAREST_MIPMAP_NEAREST || this->textureMinFilter==GL_NEAREST_MIPMAP_LINEAR
				||	this->textureMinFilter==GL_LINEAR_MIPMAP_NEAREST  || this->textureMinFilter==GL_LINEAR_MIPMAP_LINEAR  )
			{	//we use mipmap
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+i,GL_RGB8,this->width,this->width,GL_RGB,GL_UNSIGNED_BYTE,pixels);
				this->internalFormat = GL_RGB8;
			}
			else
			{	//we don't use mipmap
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+i, 0, GL_RGB8, this->width, this->width, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
				this->internalFormat = GL_RGB8;
			}
		}

		delete [] pixels;
	}
	else
	{
		this->loadCubeFaceInVideoMem(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, this->face_xn);
		this->loadCubeFaceInVideoMem(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, this->face_xp);
		this->loadCubeFaceInVideoMem(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, this->face_yn);
		this->loadCubeFaceInVideoMem(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, this->face_yp);
		this->loadCubeFaceInVideoMem(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, this->face_zn);
		this->loadCubeFaceInVideoMem(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, this->face_zp);
	}

	this->loadedInVideoMem = true;
	this->disableTexture();
}

void TextureCubeMapARB::reload()
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

void TextureCubeMapARB::unload(bool force)
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

void TextureCubeMapARB::loadCubeFaceInVideoMem(GLenum face, const std::string path)
{
	unsigned int finalWidth;
	unsigned int finalHeight;
	GLubyte* scaledImage = NULL;
	Image* img;

	const char *pc = path.c_str() + strlen(path.c_str())-3;
	if(strcmp(pc,"bmp")==0)
		img = new ImageBMP();
	else if(strcmp(pc,"tga")==0)
		img = new ImageTGA();

	if(img->load(path.c_str()))
	{
		finalWidth = img->getWidth();
		finalHeight = img->getHeight();

		//
		//image scaling
		//
		if(quality>0)
		{
			for(int i=0;i<quality;i++)
			{
				finalWidth /=2;
				finalHeight /=2;
			}
			if(finalWidth<1 ) finalWidth  = 1;
			if(finalHeight<1) finalHeight = 1;

			scaledImage = new GLubyte[(finalWidth+1)*(finalHeight+1)*3];
			gluScaleImage(GL_RGB,img->getWidth(),img->getHeight(),img->getType(),img->getDatasPointer(),
				finalWidth,finalHeight,GL_UNSIGNED_BYTE,scaledImage);
		}
		else
			scaledImage = img->getDatasPointer();

		//
		//we send image in the right cubemap face
		//
		if(		this->textureMinFilter==GL_NEAREST_MIPMAP_NEAREST || this->textureMinFilter==GL_NEAREST_MIPMAP_LINEAR
			||	this->textureMinFilter==GL_LINEAR_MIPMAP_NEAREST  || this->textureMinFilter==GL_LINEAR_MIPMAP_LINEAR  )
		{//we use mipmap
			gluBuild2DMipmaps(face,GL_RGB8,finalWidth,finalHeight,GL_RGB,GL_UNSIGNED_BYTE,scaledImage);
			this->internalFormat = GL_RGB8;
		}
		else
		{//we don't use mipmap
			glTexImage2D(face,0,GL_RGB,finalWidth,finalHeight,0,GL_RGB,GL_UNSIGNED_BYTE,scaledImage);
			this->internalFormat = GL_RGB8;
		}


		this->width = finalWidth;
		this->heigth = finalHeight;
		if(quality>0)
			delete [] scaledImage;
	}

	delete img;
}

bool TextureCubeMapARB::genNormalizedCubeMap(const char* name, unsigned int size)
{
	//texture already loaded
	if(this->loaded ) return false;

	//empty name
	if(strlen(name)==0) return false;

	this->width = this->heigth = size;
	this->depth = 0;

	this->name = name;
	this->loaded = true;
	this->isNormalizedCubeMap = true;
	return true;
}