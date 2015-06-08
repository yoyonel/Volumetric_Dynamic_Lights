
#include "TextureManager.h"


#define DEFAULT_TEXTURE_NAME		"default"
#define DEFAULT_TEXTURE_PATH		"DEFAULT_PATH"
#define DEFAULT_TEXTURE_SIZE		16

/**
 *	Default texture
 */
float defaultTexture[] = {
	1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
	1.0f,0.8f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.8f,1.0f,
	1.0f,0.7f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.3f,0.7f,1.0f,
	1.0f,0.7f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.3f,0.7f,1.0f,
	1.0f,0.8f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.7f,0.8f,1.0f,
	1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f
};

TextureManager* TextureManager::textureManager = NULL;


TextureManager::TextureManager()
{
	this->texMap.clear();

	//we get 3D texture function
//	Texture3DEXT::glTexImage3DEXT = (PFNGLTEXIMAGE3DEXTPROC) wglGetProcAddress("glTexImage3DEXT");
}

TextureManager::~TextureManager()
{
	this->texMap.clear();
}

TextureManager::TextureManager(TextureManager&)
{
}

TextureManager& TextureManager::getInstance()
{
	if(textureManager==NULL)
	{
		//unique instance creation
		textureManager = new TextureManager();
		//we add default texture
		textureManager->defTex2D = new Texture2D();
		textureManager->defTex2D->name = DEFAULT_TEXTURE_NAME;
		textureManager->defTex2D->path = DEFAULT_TEXTURE_PATH;
		textureManager->defTex2D->enableTexture();
		glGenTextures(1,&textureManager->defTex2D->id);
		textureManager->defTex2D->bindTexture();
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,textureManager->defTex2D->textureWrapS);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,textureManager->defTex2D->textureWrapT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,textureManager->defTex2D->textureMagFilter);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,textureManager->defTex2D->textureMinFilter);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,textureManager->defTex2D->textureAnisotropy);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,DEFAULT_TEXTURE_SIZE,DEFAULT_TEXTURE_SIZE,0,GL_LUMINANCE,GL_FLOAT,defaultTexture);
		textureManager->defTex2D->internalFormat = GL_RGB;
		textureManager->defTex2D->width = DEFAULT_TEXTURE_SIZE;
		textureManager->defTex2D->heigth = DEFAULT_TEXTURE_SIZE;
		textureManager->defTex2D->depth = 0;
		textureManager->defTex2D->disableTexture();
		//we don't add the texture in the map. She's always use by the textureManager
	}
	return *textureManager;
}

void TextureManager::destroy()
{
	if(textureManager!=NULL)
	{
		//we delete all texture
		textureManager->removeAllTexture();
		//we delete default texture
		textureManager->defTex2D->unload(true);
		delete textureManager->defTex2D;
		//we deelte unique instance
		delete textureManager;
	}
	textureManager = NULL;
}


void TextureManager::reloadAllTextures()
{
	std::map<std::string, Texture*>::iterator it;

	for(it=this->texMap.begin(); it!=this->texMap.end(); it++)
	{
		it->second->reload();
	}
}

void TextureManager::unloadAllTextures(bool force)
{
	std::map<std::string, Texture*>::iterator it;

	for(it=this->texMap.begin(); it!=this->texMap.end(); it++)
	{
		it->second->unload(force);
	}
}

Texture& TextureManager::getTexture(const std::string& name) const
{
	std::map<std::string, Texture*>::const_iterator it = this->texMap.find(name);

	if(it!=this->texMap.end())
		return *(it->second);		//we have find the texture
	//the texture have not been found
	return *defTex2D;
}

void TextureManager::removeTexture(const std::string& name)
{
	std::map<std::string, Texture*>::iterator it;

	//seraching for the texture
	it = this->texMap.find(name);

	if(it!=this->texMap.end())
	{
		//we have find the texture : we delete it
		this->texMap.erase(it);
	}
}

void TextureManager::removeAllTexture()
{
	std::map<std::string, Texture*>::iterator it;

	//we force unload of all texture in video memory
	this->unloadAllTextures(true);

	//we delete all instace of texture in system memory
	for(it=this->texMap.begin(); it!=this->texMap.end(); it++)
	{
		delete it->second;
	}

	this->texMap.clear();
}

bool TextureManager::addTexture2D(const std::string& name, const std::string& path, bool generateAlpha, 
		GLint magFilter, GLint minFilter, GLint sWrap, GLint tWrap,
		GLubyte alphaColor_r, GLubyte alphaColor_g,GLubyte alphaColor_b,
		GLubyte alphaValue, GLubyte noAlphaValue,
		unsigned int quality, GLint anisotropy, bool lockedAttrib, bool lockedInMem)
{
	//we check that the name don't already exist
	if(this->texMap.find(name)!=this->texMap.end())
		return false;

	Texture2D* tex = new Texture2D();
	//source image path
	bool exist = tex->genTexture(name.c_str(), path.c_str());

	if(exist)
	{
		tex->lockedAttrib = lockedAttrib;
		tex->lockedInMem = lockedInMem;

		tex->textureMagFilter = magFilter;
		tex->textureMinFilter = minFilter;
		tex->textureWrapS = sWrap;
		tex->textureWrapT = tWrap;
		tex->textureAnisotropy = anisotropy;
		tex->quality = quality;

		//alpha generation
		tex->generateAlpha = generateAlpha;
		tex->alphaColor_r = alphaColor_r;
		tex->alphaColor_g = alphaColor_g;
		tex->alphaColor_b = alphaColor_b;
		tex->alphaValue = alphaValue;
		tex->noAlphaValue = noAlphaValue;

		//alpha generation from texture
		tex->useAlphaTexture = false;
		tex->alphaTexturePath = "";

		texMap[tex->name] = tex;
	}
	else
		delete tex;

	return exist;
}



bool TextureManager::addTexture2DAlphaTexture(const std::string& name, const std::string& path, const std::string& pathAlpha, 
	GLint magFilter, GLint minFilter, GLint sWrap, GLint tWrap,
	unsigned int quality, GLint anisotropy, bool lockedAttrib, bool lockedInMem)
{
	//we check that the name don't already exist
	if(this->texMap.find(name)!=this->texMap.end())
		return false;

	Texture2D* tex = new Texture2D();
	//source image path
	bool exist = tex->genTexture(name.c_str(), path.c_str());

	if(exist)
	{
		tex->lockedAttrib = lockedAttrib;
		tex->lockedInMem = lockedInMem;

		tex->textureMagFilter = magFilter;
		tex->textureMinFilter = minFilter;
		tex->textureWrapS = sWrap;
		tex->textureWrapT = tWrap;
		tex->textureAnisotropy = anisotropy;
		tex->quality = quality;

		//alpha generation
		tex->generateAlpha = false;
		tex->alphaColor_r = 0;
		tex->alphaColor_g = 0;
		tex->alphaColor_b = 0;
		tex->alphaValue = 0;
		tex->noAlphaValue = 0;

		//alpha generation from texture
		tex->useAlphaTexture = true;
		tex->alphaTexturePath = pathAlpha;

		texMap[tex->name] = tex;
	}
	else
		delete tex;

	return exist;
}



bool TextureManager::addTextureCubeMapARB(const std::string& name, const std::string& xn,  const std::string& xp, 
		const std::string& yn,  const std::string& yp, const std::string& zn,  const std::string& zp, 
		GLint magFilter, GLint minFilter, GLint sWrap, GLint tWrap, 
		unsigned int quality, GLint anisotropy, bool lockedAttrib, bool lockedInMem)
{
	//we check that the name don't already exist
	if(this->texMap.find(name)!=this->texMap.end())
		return false;

	TextureCubeMapARB* tex = new TextureCubeMapARB();
	//source image path
	bool exist = tex->genCubeMap(name.c_str(), xn.c_str(), xp.c_str(), yn.c_str(), yp.c_str(), zn.c_str(), zp.c_str());

	if(exist)
	{
		tex->lockedAttrib = lockedAttrib;
		tex->lockedInMem = lockedInMem;

		tex->textureMagFilter = magFilter;
		tex->textureMinFilter = minFilter;
		tex->textureWrapS = sWrap;
		tex->textureWrapT = tWrap;
		tex->textureAnisotropy = anisotropy;
		tex->quality = quality;

		texMap[tex->name] = tex;
	}
	else
		delete tex;

	return exist;
}

bool TextureManager::addNormalisationCubeMapARB(const std::string& name, unsigned int size, 
		GLint magFilter, GLint minFilter, GLint sWrap, GLint tWrap, 
		GLint anisotropy, bool lockedAttrib, bool lockedInMem)
{
	//we check that the name don't already exist
	if(this->texMap.find(name)!=this->texMap.end())
		return false;

	TextureCubeMapARB* tex = new TextureCubeMapARB();
	//source image path
	bool exist = tex->genNormalizedCubeMap(name.c_str(), size);

	if(exist)
	{
		tex->lockedAttrib = lockedAttrib;
		tex->lockedInMem = lockedInMem;

		tex->textureMagFilter = magFilter;
		tex->textureMinFilter = minFilter;
		tex->textureWrapS = sWrap;
		tex->textureWrapT = tWrap;
		tex->textureAnisotropy = anisotropy;
		tex->quality = 0;

		texMap[tex->name] = tex;
	}
	else
		delete tex;

	return exist;
}


/*
bool TextureManager::addTexture3DEXT_sphereMap(const std::string& name, unsigned int size, 
		GLint magFilter, GLint minFilter, 
		GLint sWrap, GLint tWrap,  GLint rWrap, 
		GLint anisotropy, bool lockedAttrib, bool lockedInMem)
{
	//we check that the name don't already exist
	if(this->texMap.find(name)!=this->texMap.end())
		return false;

	Texture3DEXT* tex = new Texture3DEXT();
	//source image path
	bool exist = tex->genAttenuatedSphereMap(name.c_str(), size);

	if(exist)
	{
		tex->lockedAttrib = lockedAttrib;
		tex->lockedInMem = lockedInMem;

		tex->textureMagFilter = magFilter;
		tex->textureMinFilter = minFilter;
		tex->textureWrapS = sWrap;
		tex->textureWrapT = tWrap;
		tex->textureWrapR = rWrap;
		tex->textureAnisotropy = anisotropy;
		tex->quality = 0;

		texMap[tex->name] = tex;
	}
	else
		delete tex;

	return exist;
}



bool TextureManager::addTexture3DEXT_arbitraryRGB(const std::string& name, GLfloat* (*pArbitraryTexture3DTexels)(),
		unsigned int sSize, unsigned int tSize, unsigned int rSize, 
		GLint magFilter, GLint minFilter, 
		GLint sWrap, GLint tWrap,  GLint rWrap, 
		GLint anisotropy, bool lockedAttrib, bool lockedInMem)
{
	//we check that the name don't already exist
	if(this->texMap.find(name)!=this->texMap.end())
		return false;

	Texture3DEXT* tex = new Texture3DEXT();
	//source image path
	bool exist = tex->genArbitraryMap(name.c_str(),sSize,tSize,rSize,pArbitraryTexture3DTexels);

	if(exist)
	{
		tex->lockedAttrib = lockedAttrib;
		tex->lockedInMem = lockedInMem;

		tex->textureMagFilter = magFilter;
		tex->textureMinFilter = minFilter;
		tex->textureWrapS = sWrap;
		tex->textureWrapT = tWrap;
		tex->textureWrapR = rWrap;
		tex->textureAnisotropy = anisotropy;
		tex->quality = 0;

		texMap[tex->name] = tex;
	}
	else
		delete tex;

	return exist;
} 
 */


