
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#ifdef __linux__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <glew/glew.h>
	#include <glut/glut.h>
#endif

/**
 *	pre declaration for friend relation
 */
class TextureManager;


/**
 *	OpenGL texture class interface. This class is controled by the texture manager.
 * An implementation of this class exist for each OpenGL texture target type.
 *
 * @author Hillaire Sébastien
 */
class Texture
{
private:

protected:

	friend class TextureManager;

	/**
	 *	Texture name
	 */
	std::string name;
	/**
	 *	Texture path on disk
	 */
	std::string path;

	/**
	 *	texture's OpenGL id.
	 */
	GLuint id;
	/**
	 *	OpenGL type
	 */
	GLenum target;
	/**
	 *	OpenGL format
	 */
	GLint internalFormat;

	/**
	 *	S repetition type
	 */
	GLint textureWrapS;
	/**
	 *	T repetition type
	 */
	GLint textureWrapT;
	/**
	 *	R repetition type
	 */
	GLint textureWrapR;

	/**
	 *	minification mode
	 */
	GLint textureMinFilter;
	/**
	 *	magnification mode
	 */
	GLint textureMagFilter;
	/**
	 *	anisotropy level
	 */
	GLint textureAnisotropy;
	
	/**
	 *	Texture quality. 0 mean original quality, 1 mean a resolution divided by two,
	 * 2 mean  a resolution divided by 4, etc.
	 */
	unsigned int quality;
	/**
	 *	Texture's width in video memory(pour les texture 1D, 2D et 3D et les cube-map)
	 */
	unsigned int width;
	/**
	 *	Texture's height in video memory(pour les texture 1D, 2D et 3D et les cube-map)
	 */
	unsigned int heigth;
	/**
	 *	Texture's depth in video memory(pour les texture 1D, 2D et 3D et les cube-map)
	 */
	unsigned int depth;

	//////////////////////////////////
	// Texture attributs management //
	//////////////////////////////////

	/**
	 *	Allow to lock texture attributes. Usefull to forbid quality
	 * modification for font texture (who may become incomprehensible)
	 */
	bool lockedAttrib;
	/**
	 *	Ensure that the texture will always be in video memory even after an unload command.
	 * The only way to unload the texture isd to destroy the texture manager.
	 */
	bool lockedInMem;
	/**
	 *	true if the name and the image file path have been affected and are valid.
	 */
	bool loaded;
	/**
	 *	true if the texture is in video memory
	 */
	bool loadedInVideoMem;

	/////////////////////////////
	// TextureManager commands //
	/////////////////////////////

	/**
	 *	Load texure in video memory : the OpenGL texture object is created here.
	 *	This method is time consuming because it read data on disk and send it to graphic card.
	 *
	 * @pre texture name and image file path have been affected and are valid
	 * @pre the texture is not yet loaded
	 */
	virtual void loadInVideoMem();
	/**
	 *	Reload the texture in ideo memory with the new paramters
	 *
	 * @pre A texture object have already been loaded!
	 * @pre We have the right to modify the texture in video memory. (@see lockedAttrib)
	 */
	virtual void reload();
	/**
	 *	Destroy OpenGL texture object.
	 *
	 * @pre texture name and image file path have been affected and are valid
	 * @pre the texture object is loaded!
	 * @pre We have the right to modify the texture in video memory. (@see lockedAttrib)
	 * @pre the texture object is not locked in video memory
	 */
	virtual void unload(bool force = false);

	/**
	 * Constructor (protected)
	 */
	Texture();
	/**
	 * Copy constructor (protected)
	 */
	Texture(Texture&);
	/**
	 * Destructor (protected)
	 */
	virtual ~Texture();

public:

	/**
	 *	getter
	 *	To get a valid id, you must touch the texture via a bind command before.
	 *
	 * @return OpenGL Texture id
	 */
	inline GLuint getId() const { /*if(!this->loadedInVideoMem)this->loadInVideoMem();*/  return this->id; }

	/**
	 *	getter
	 *
	 * @return texture name
	 */
	std::string getName() const;

	/**
	 *	getter
	 *
	 * @return texture width
	 */
	unsigned int getWidth() const;
	/**
	 *	getter
	 *
	 * @return texture height
	 */
	unsigned int getHeigth() const;
	/**
	 *	getter
	 *
	 * @return texture depth
	 */
	unsigned int getDepth() const;

	/**
	 *	= glEnable(target)
	 */
	inline void enableTexture() const { glEnable(this->target); };
	/**
	 *	= glEnable(target)
	 *
	 * @param target : texture target
	 */
	inline static void enableTexture(GLenum target) { glEnable(target); };
	/**
	 *	=glBindTexture(target,texid)
	 *	If the texture is not loaded in video memory, we load it.
	 */
	inline void bindTexture() { if(!this->loadedInVideoMem)this->loadInVideoMem();  glBindTexture(this->target,this->id); };
	/**
	 *	= glDisable(target)
	 */
	inline void disableTexture() const { glDisable(this->target); };
	/**
	 *	= glDisable(target)
	 *
	 * @param target : texture target
	 */
	inline static void disableTexture(GLenum target) { glDisable(target); }
	/**
	 *	= glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, pname)
	 *
	 * @param pname : texel combine method (GL_REPLACE, GL_MODULATE....)
	 */
	inline static void textureEnvMode(GLint pname) { glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, pname); }
};


#endif
