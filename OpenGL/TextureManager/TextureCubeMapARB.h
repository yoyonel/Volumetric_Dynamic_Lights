
#ifndef TEXTURE_CUBEMAP_ARB_H
#define TEXTURE_CUBEMAP_ARB_H

#include "ImageBMP.h"
#include "ImageTGA.h"
#include "Texture.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

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
 *	An implementation of the Texture interface for cubemap textures.
 *
 * @see Texture
 *
 * @author Hillaire Sébastien
 */
class TextureCubeMapARB : public Texture
{
private:

	friend class TextureManager;

	/**
	 *	neg x image
	 */
	std::string face_xn;
	/**
	 *	pos x image
	 */
	std::string face_xp;
	/**
	 *	neg y image
	 */
	std::string face_yn;
	/**
	 *	pos y image
	 */
	std::string face_yp;
	/**
	 *	neg z image
	 */
	std::string face_zn;
	/**
	 *	pos z image
	 */
	std::string face_zp;
	/**
	 *	true if it's a normalisation cubemap
	 */
	bool isNormalizedCubeMap;

	/**
	 * Protected constructor
	 */
	TextureCubeMapARB();
	/**
	 * Protected copy constructor
	 */
	TextureCubeMapARB(TextureCubeMapARB&);
	/**
	 * Protected destructor
	 */
	virtual ~TextureCubeMapARB();


	/**
	 *
	 */
	void loadCubeFaceInVideoMem(GLenum face, const std::string path);

	/**
	 *	Create a normalization cubemap texture. 
	 *
	 * @pre we have not assigned yet name
	 * @pre name is not empty
	 *
	 * @param name : texture name
	 * @param path : size of the texture in texel on s, t and r axis
	 *
	 * @return true if the generation works well
	 */
	bool genNormalizedCubeMap(const char* name, unsigned int size=256);

	/**
	 *	Create a cubemap texture from images. 
	 *
	 * @pre we have not assigned yet name
	 * @pre name is not empty
	 * @pre name, xn, xp, ..., zp are valid pointer
	 * @pre xn, xp, ..., zp are existing files
	 *
	 * @param name : texture name
	 * @param xn : neg x image
	 * @param xp : pos x image
	 * @param yn : neg y image
	 * @param yp : pos y image
	 * @param zn : neg z image
	 * @param zp : pos z image
	 *
	 * @return true if the generation works well
	 */
	bool genCubeMap(const char* name, 
		const char* xn, const char* xp, const char* yn, const char* yp, const char* zn, const char* zp);

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
