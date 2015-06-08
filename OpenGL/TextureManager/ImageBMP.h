
#ifndef IMAGEBMP_H
#define IMAGEBMP_H


#include <iostream>

#ifdef __linux__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <glew/glew.h>
	#include <glut/glut.h>
#endif

#include "Image.h"


#define BMP_DWORD			unsigned long
#define BMP_BYTE			unsigned char
#define BMP_WORD			unsigned short

/**
* Header structure of a BMP file
*/
struct BMPHeader
{
	/** type
	*/
	BMP_BYTE		bfType1;
	/** type
	*/
	BMP_BYTE		bfType2;
	/** size
	*/
	BMP_DWORD		bfSize;
	/** reserved
	*/
	BMP_WORD		bfReserved1;
	/** reserved
	*/
	BMP_WORD		bfReserved2;
	/** data offset
	*/
	BMP_DWORD		bfOffBits;
	/** size
	*/
	BMP_DWORD		biSize;
	/** width
	*/
	BMP_DWORD		biWidth;
	/** height
	*/
	BMP_DWORD		biHeight;
	/** planes
	*/
	BMP_WORD		biPlanes;
	/** bit count
	*/
	BMP_WORD		biBitCount;
	/** compressed format
	*/
	BMP_DWORD		biCompression;
	/** image size
	*/
	BMP_DWORD		biSizeImage;
	/** ratio
	*/
	BMP_DWORD		biXPelsPerMeter;
	/** ratio
	*/
	BMP_DWORD		biYPelsPerMeter;
	/** Clr used?
	*/
	BMP_DWORD		biClrUsed;
	/** Clr important?
	*/
	BMP_DWORD		biClrImportant;
};


/**
 * Implementation of Image for .bmp files
 *
 * @author Hillaire Sébastien
 */
class ImageBMP : public Image
{
	/**
	* bmp header
	*/
	BMPHeader bmph;

protected:
	/**
	 * @see Image
	 */
	void setBPP(int newBpp);

public:
	
	/**
	 * @see Image
	*/
	unsigned char* getDatasPointer();
	
	/**
	 * @see Image
	*/
	unsigned short getBPP();
	
	/**
	 * @see Image
	*/
	unsigned int getHeight();
	
	/**
	 * @see Image
	*/
	unsigned int getWidth();
	
	/**
	 * @see Image
	*/
	bool load(const char *filepath);
	
	/**
	* constructor
	*/
	ImageBMP();
	
	/**
	* destructor
	*/
	~ImageBMP();

	/**
	 *	Capture an image from an OpenL render buffer and save it to disk
	 *
	 * @pre you should specify the read buffer via the glReadBuffer(...) command
	 *
	 * @param filePath : the path of the saved bmp image
	 * @param x : the x coordinate of the top left corner of the part of the buffer saved on disk
	 * @param y : the y coordinate of the top left corner of the part of the buffer saved on disk
	 * @param width : the width of the image
	 * @param height : the height of the image
	 *
	 * @return true if image have been saved successfuly
	 */
	static bool captureAndSaveToDisk(char* filePath, int x, int y, int width, int height);

	/**
	 *	Save an OpenGL texture to the disk. (good for texture and frame buffer object)
	 * Works only for 1D and 2D textures.
	 *
	 * @param filePath : the path of the saved bmp image
	 * @param target : the target type of the texture
	 * @param texId : the id of the texture
	 *
	 * @return true if image have been saved successfuly
	 */
	static bool TextureToDisk(const char* filePath, int target, GLuint texId);


};


#endif
