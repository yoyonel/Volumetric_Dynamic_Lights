
/**********************************************************************\
* AUTHOR : HILLAIRE Sébastien
*
* MAIL   : hillaire_sebastien@yahoo.fr
* SITE   : sebastien.hillaire.free.fr
*
*	You are free to totally or partially use this file/code.
* If you do, please credit me in your software or demo and leave this
* note.
*	Share your work and your ideas as much as possible!
\*********************************************************************/

#ifndef IMAGETGA_H
#define IMAGETGA_H


#include <iostream>
#include "Image.h"


/**
 * Implementation of Image for .tga files
 *
 * @author Hillaire Sébastien
 */
class ImageTGA : public Image
{
protected:
	
	/**
	 * number of bit per pixel of the image
	 */
	int bpp;
	/**
	 * image's width
	 */
	int width;
	/**
	 * image's height
	 */
	int height;

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
	ImageTGA();
	
	/**
	* destructor
	*/
	~ImageTGA();
	
};



#endif