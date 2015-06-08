
#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "ImageBMP.h"
#include "ImageTGA.h"
#include "Texture.h"
#include <stdio.h>

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
 *	An implementation of the Texture interface for 2D textures.
 *
 * @see Texture
 *
 * @author Hillaire Sébastien
 */
class Texture2D : public Texture
{
private:

	friend class TextureManager;

	/**
	 *	Do we generate alpha channel
	 */
	bool generateAlpha;
	/**
	 *	red component for pixel with modified alpha
	 */
	GLubyte alphaColor_r;
	/**
	 *	green component for pixel with modified alpha
	 */
	GLubyte alphaColor_g;
	/**
	 *	blue component for pixel with modified alpha
	 */
	GLubyte alphaColor_b;
	/**
	 *	the alpha value for modified pixels
	 */
	GLubyte alphaValue;
	/**
	 *	the default alpha value
	 */
	GLubyte noAlphaValue;

	
	/**
	 *	do we use another texture for the alpha channel
	 */
	bool useAlphaTexture;
	/**
	 *	The path of the texture used to write the alpha channel (we use only red channel)
	 */
	std::string alphaTexturePath;

	/**
	 * Protected constructor
	 */
	Texture2D();
	/**
	 * Protected copy constructor
	 */
	Texture2D(Texture2D&);
	/**
	 * Protected destructor
	 */
	virtual ~Texture2D();


	/**
	 *	Create a 2D texture from image(s). You can load power and non power
	 * of two texture.
	 *
	 *	REM : When mipmiap are enabled, texture are transform to a power of
	 * two texture.
	 *
	 * @pre we have not assigned yet name and path
	 * @pre name is not empty
	 * @pre name and path are valid pointers
	 * @pre path point to an existing file
	 *
	 * @param name : texture name
	 * @param path : disk path to texture
	 *
	 * @return true if the generation works well
	 */
	bool genTexture(const char* name, const char* path);
	/**
	 *	@see Texture
	 */
	void loadInVideoMem();
	/**
	 *	@see Texture
	 */
	void reload();
	/**
	 *	@see Texture
	 */
	void unload(bool force = false);

protected:


public:
};



#endif
