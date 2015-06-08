
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#ifdef __linux__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <glew/glew.h>
	#include <glut/glut.h>
#endif

#include "stdlib.h"
#include <string>
#include <map>

#include "Texture.h"
#include "Texture2D.h"
#include "TextureCubeMapARB.h"
#include "Texture3DEXT.h"

/**
 *	The texture manager allow to manage texture in video memory in a simple way.
 * we can specify textures to load with their attributes, file containing texels,
 * name(unique ID), and video memory attribute (do the manager can unload this texture?)
 *
 *	The manager manage 2D texture with transparency, 3D texture and cubemap.
 *
 * Implement a Singleton pattern.
 *
 * @author Hillaire Sébastien
 */
class TextureManager
{
private:

	/**
	 *	unique instance. (pattern singleton)
	 */
	static TextureManager* textureManager;

	/**
	 *	Constructor
	 */
	TextureManager();
	/**
	 *	Destructor
	 */
	~TextureManager();
	/**
	 *	Copy constructeur
	 */
	TextureManager(TextureManager&);

	/**
	 *	contains all texture map by their key (their name)
	 */
	std::map<std::string, Texture*> texMap;
	/**
	 *	Default texture returned when texture cannot be found
	 */
	Texture* defTex2D;

protected:

public:

	/**
	 *	Allow to get the unique instance of the TextureManager
	 *
	 * @return the unique instance of the TextureManager
	 */
	static TextureManager& getInstance();

	/**
	 *	Destroy the unique instance of the TextureManager
	 */
	static void destroy();

	///////////////////////////////////
	//	General texture manipulation //
	///////////////////////////////////

	/**
	 *	Reload all textures (to take into account their new parameters
	 * if they can be changed).
	 */
	void reloadAllTextures();
	/**
	 *	Allow to unload textures from the video memory but not the locked textures. You can use this
	 * when quitting the app but it's preffered to use destroy().
	 *
	 * @param force : force locked textures unload.
	 */
	void unloadAllTextures(bool force = false);

	/**
	 *	Permet de récupérer une texture OpenGL dans le mananger de texture suivant son nom.
	 *	Cette recherche est rapide car on utilise un map mais c'est quand même un comparaison
	 * de chaine de caractère donc attention!
	 *
	 *
	 *
	 * @param name : texture name.
	 *
	 * @return The found texture. If the texture is not found, default texture is returned.
	 */
	Texture& getTexture(const std::string& name) const;
	
	/**
	 *	Delete a texture
	 * WARNING : all references to the texture become invalid!
	 * !!! THIS METHOD SHOULD NOT BE USED!!!
	 *
	 * @param name : texture name.
	 */
	void removeTexture(const std::string& name);
	/**
	 *	Delete all texture of the memory manager (after a unload)
	 *
	 * !!! THIS METHOD SHOULD NOT BE USED : instead, use destroy() or unloadAllTextures!!!
	 */
	void removeAllTexture();

	////////////////
	// Texture 2D //
	////////////////

	/**
	 *	Add a 2D texture
	 *
	 * REM :
	 *	This method load power and non power of two texture. You have to verify if the
	 * graphic card support non power of two textures. When mipmap are enabled, all textures are
	 * passed to a power of two texture.
	 *
	 * @param name          : texture name
	 * @param path          : file path( 24bits : tga or bmp)
	 * @param generateAlpha : true alpha channel must be generate
	 * @param magFilter     : magnification filter
	 * @param minFilter     : minification filter
	 * @param sWrap         : repetition type on S coordinate
	 * @param tWrap         : repetition type on T coordinate
	 * @param alphaColor_r  : red component for pixel with modified alpha
	 * @param alphaColor_g  : green component for pixel with modified alpha
	 * @param alphaColor_b  : blue component for pixel with modified alpha
	 * @param alphaValue    : the alpha value for modified pixels
	 * @param noAlphaValue  : the default alpha value
	 * @param quality       : texture quality
	 * @param anisotropy    : anisotropy level
	 * @param lockedAttrib  : true if attributes are locked
	 * @param lockedInMem   : true if texture is locked in video memory
	 *
	 * @return true if all works well (else false). If the name already exist, texture is not*
	 * registred.
	 */
	bool addTexture2D(const std::string& name, const std::string& path, bool generateAlpha=false, 
		GLint magFilter=GL_LINEAR, GLint minFilter=GL_LINEAR_MIPMAP_LINEAR, GLint sWrap=GL_REPEAT, GLint tWrap=GL_REPEAT,
		GLubyte alphaColor_r=0, GLubyte alphaColor_g=0,GLubyte alphaColor_b=0,
		GLubyte alphaValue=0, GLubyte noAlphaValue=255,
		unsigned int quality=0, GLint anisotropy=1, bool lockedAttrib=false, bool lockedInMem=false);

	/**
	 *	Add a 2D texture with alpha channel generate from another image. If both image are not the
	 * same size, alpha values are 0.5.
	 *
	 * REM :
	 *	This method load power and non power of two texture. You have to verify if the
	 * graphic card support non power of two textures. When mipmap are enabled, all textures are
	 * passed to a power of two texture.
	 *
	 * @param name          : texture name
	 * @param path          : color image file path( 24bits : tga or bmp)
	 * @param pathAlpha     : alpha image file path( 24bits : tga or bmp) (the red component give directly the alpha value)
	 * @param magFilter     : magnification filter
	 * @param minFilter     : minification filter
	 * @param sWrap         : repetition type on S coordinate
	 * @param tWrap         : repetition type on T coordinate
	 * @param quality       : texture quality
	 * @param anisotropy    : anisotropy level
	 * @param lockedAttrib  : true if attributes are locked
	 * @param lockedInMem   : true if texture is locked in video memory
	 *
	 * @return true if all works well (else false). If the name already exist, texture is not*
	 * registred.
	 */
	bool addTexture2DAlphaTexture(const std::string& name, const std::string& path, const std::string& pathAlpha, 
		GLint magFilter=GL_LINEAR, GLint minFilter=GL_LINEAR_MIPMAP_LINEAR, GLint sWrap=GL_REPEAT, GLint tWrap=GL_REPEAT,
		unsigned int quality=0, GLint anisotropy=1, bool lockedAttrib=false, bool lockedInMem=false);

	/////////////////
	//   CubeMap   //
	/////////////////


	/**
	 *	Add a texture cubemap with 6 differents textures possible on each face.
	 *
	 * REM :
	 *	This method load power and non power of two texture. You have to verify if the
	 * graphic card support non power of two textures. When mipmap are enabled, all textures are
	 * passed to a power of two texture.
	 *
	 * @param name          : texture name
	 * @param xn			: neg x image
	 * @param xp			: pos x image
	 * @param yn			: neg y image
	 * @param yp			: pos y image
	 * @param zn			: neg z image
	 * @param zp			: pos z image
	 * @param magFilter     : magnification filter
	 * @param minFilter     : minification filter
	 * @param sWrap         : repetition type on S coordinate
	 * @param tWrap         : repetition type on T coordinate
	 * @param quality       : texture quality
	 * @param anisotropy    : anisotropy level
	 * @param lockedAttrib  : true if attributes are locked
	 * @param lockedInMem   : true if texture is locked in video memory
	 *
	 * @return true if all works well (else false). If the name already exist, texture is not
	 * registred.
	 */
	bool addTextureCubeMapARB(const std::string& name, const std::string& xn, const std::string& xp, 
		const std::string& yn, const std::string& yp, const std::string& zn, const std::string& zp, 
		GLint magFilter=GL_LINEAR, GLint minFilter=GL_LINEAR_MIPMAP_LINEAR, GLint sWrap=GL_CLAMP_TO_EDGE, GLint tWrap=GL_CLAMP_TO_EDGE, 
		unsigned int quality=0, GLint anisotropy=1, bool lockedAttrib=false, bool lockedInMem=false);


	/**
	 *	Add a normalization cubemap texture.
	 *
	 * @param name          : texture name
	 * @param size			: resolution of each texture on each face
	 * @param magFilter     : magnification filter
	 * @param minFilter     : minification filter
	 * @param sWrap         : repetition type on S coordinate
	 * @param tWrap         : repetition type on T coordinate
	 * @param anisotropy    : anisotropy level
	 * @param lockedAttrib  : true if attributes are locked
	 * @param lockedInMem   : true if texture is locked in video memory
	 *
	 * @return true if all works well (else false). If the name already exist, texture is not
	 * registred.
	 */
	bool addNormalisationCubeMapARB(const std::string& name, unsigned int size=256, 
		GLint magFilter=GL_LINEAR, GLint minFilter=GL_LINEAR_MIPMAP_LINEAR, GLint sWrap=GL_CLAMP_TO_EDGE, GLint tWrap=GL_CLAMP_TO_EDGE, 
		GLint anisotropy=1, bool lockedAttrib=false, bool lockedInMem=false);

	/////////////////
	//	Texture 3D //
	/////////////////

	/**
	 *	Add a spheremap 3D texture
	 *
	 * @param name          : texture name
	 * @param size			: resolution of the 3D texture
	 * @param magFilter     : magnification filter
	 * @param minFilter     : minification filter
	 * @param sWrap         : repetition type on S coordinate
	 * @param tWrap         : repetition type on T coordinate
	 * @param rWrap         : repetition type on R coordinate
	 * @param anisotropy    : anisotropy level
	 * @param lockedAttrib  : true if attributes are locked
	 * @param lockedInMem   : true if texture is locked in video memory
	 *
	 * @return true if all works well (else false). If the name already exist or texture3D extension doesn't exist, texture is not
	 * registred.
	 */
	bool addTexture3DEXT_sphereMap(const std::string& name, unsigned int size=128, 
		GLint magFilter=GL_LINEAR, GLint minFilter=GL_LINEAR_MIPMAP_LINEAR, 
		GLint sWrap=GL_CLAMP, GLint tWrap=GL_CLAMP,  GLint rWrap=GL_CLAMP, 
		GLint anisotropy=1, bool lockedAttrib=false, bool lockedInMem=false);

	/**
	 *	Permet d'ajouter une texture 3D EXT dont le contenu est donnée par un tableau en mémoire
	 *
	 * @param name          : texture name
	 * @param pArbitraryTexture3DTexels	: the fonction to call to get the texture texels
	 * @param sSize			: resolution of the 3D texture on S
	 * @param tSize			: resolution of the 3D texture on T
	 * @param rSize			: resolution of the 3D texture on R
	 * @param magFilter     : magnification filter
	 * @param minFilter     : minification filter
	 * @param sWrap         : repetition type on S coordinate
	 * @param tWrap         : repetition type on T coordinate
	 * @param rWrap         : repetition type on R coordinate
	 * @param anisotropy    : anisotropy level
	 * @param lockedAttrib  : true if attributes are locked
	 * @param lockedInMem   : true if texture is locked in video memory
	 *
	 * @return true if all works well (else false). If the name already exist or texture3D extension doesn't exist, texture is not
	 * registred.
	 */
	bool addTexture3DEXT_arbitraryRGB(const std::string& name, GLfloat* (*pArbitraryTexture3DTexels)(),
		unsigned int sSize=128, unsigned int tSize=128, unsigned int rSize=128, 
		GLint magFilter=GL_LINEAR, GLint minFilter=GL_LINEAR_MIPMAP_LINEAR, 
		GLint sWrap=GL_CLAMP, GLint tWrap=GL_CLAMP,  GLint rWrap=GL_CLAMP, 
		GLint anisotropy=1, bool lockedAttrib=false, bool lockedInMem=false);

};


#endif
