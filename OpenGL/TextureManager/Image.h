
#ifndef IMAGE_H
#define IMAGE_H

#ifdef __linux__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <glew/glew.h>
	#include <glut/glut.h>
#endif

#include "stdlib.h"


/**
 *	RGB or RGBA image loading class interface.
 *	You can transform image to grey level or normalmap(from a heightmap).
	Image type and format are given and match OpenGL specification for texture
 * object creation.
 *
 * @author Hillaire Sébastien
 */
class Image
{
protected:

	/**
	* pointer to image data in system memory
	*/
	unsigned char *datas;

	/**
	 *	Texture format (GL_RGB, GL_RGBA, GL_R3_G3_B2...)
	 */
	GLenum format;
	/**
	 *	data type (GL_UNDIGNED_BYTE, GL_FLOAT...)
	 */
	GLenum type;

	/**
	 *	Modifier
	 *
	 * @param bpp : the number of bit per pixel
	 */
	virtual void setBPP(int bpp) = 0;

public:

	/**
	 *	accessor
	 *
	 * @return image's format matching OpenGL specification
	 */
	GLenum getFormat(){ return this->format;}
	/**
	 *	accessor
	 *
	 * @return image's type matching OpenGL specification
	 */
	GLenum getType(){ return this->type;}
	
	/**
	 *	accessor
	 *
	* @return the pointer to image data in system memory
	*/
	virtual unsigned char* getDatasPointer()=0;
	
	/**
	 *	accessor
	 *
	 * @return the number of bit per pixel
	*/
	virtual unsigned short getBPP()=0;
	
	/**
	 *	accessor
	 *
	 * @return height of the image
	*/
	virtual unsigned int getHeight()=0;
	
	/**
	 *	accessor
	 *
	 * @return width of the image
	*/
	virtual unsigned int getWidth()=0;
	
	/**
	*	Load image from file
	*
	* @param filepath : the path of the image
	*
	* @return true if the image is loaded. false if the image have not been found or have a bad extension
	*/
	virtual bool load(const char *filepath)=0;
	
	/**
	* constructor
	*/
	Image();
	
	/**
	* destructor
	*/
	virtual ~Image();

	/**
	 */
/*	static Image* loadImage(char* path)
	{
		bool fileFounded = true;
		Image *img = NULL;

		char *pc = path + strlen(path)-3;

		if(strcmp(pc,"bmp")==0)
		{
			img = new ImageBMP();
		}
		else if(strcmp(pc,"tga")==0)
		{
			img = new ImageTGA();
		}
		else
			return NULL;

		fileFounded = img->load(path);
		if(fileFounded)
			return img;

		delete img;
		return NULL;
	}*/

	/**
	 * transform image datas in grey level (24 bits)
	 *
	 * This mlethod use the weight 1/3 for each component of color
	 * TODO : use true weight of color
	 */
	void toHeightMap();

	/**
	 * transform image datas in normalmap. The currnt image must be a heightmap.
	 *
	 * @param factor : rugosity factor of the normal map. The more the value is near zero,
	 * the more the normal map will represent a simple plane.
	 * @param storeHeightAsAlpha : allow to store heightmap values in the alpha channel.
	 */
	void toNormalMap(int factor = 4, bool storeHeightAsAlpha = false);

};


#endif