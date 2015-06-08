
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

#include "ImageTGA.h"

#ifdef __linux__
	#include <cstdio>
#endif


#define TGA_RGB		 2		// RGB
#define TGA_A		 3		// Alpha channel
#define TGA_RLE		10		//RLE compressed TGA


ImageTGA::ImageTGA() : Image()
{
	datas=NULL;
}

ImageTGA::~ImageTGA()
{
	delete [] datas;
}

bool ImageTGA::load(const char *filepath)
{
	unsigned short awidth = 0, aheight = 0;
	unsigned char length = 0;			// pixel offset from file begin
	unsigned char imageType = 0;		// image type
	unsigned char bits = 0;				// bpp
	FILE *pFile = NULL;					
	int channels = 0;					// channel number (3 = RGA : 4 = RGBA)
	int stride = 0;						// ligne length
	int i = 0;							
		
	if((pFile = fopen(filepath, "rb")) == NULL) 
	{
		return false;
	}
		
	fread(&length, sizeof(unsigned char), 1, pFile);
	
	// skip on byte...
	fseek(pFile,1,SEEK_CUR); 

	fread(&imageType, sizeof(unsigned char), 1, pFile);
	
	// skip info we don't need
	fseek(pFile, 9, SEEK_CUR); 

	fread(&awidth,  sizeof(unsigned short), 1, pFile);
	fread(&aheight, sizeof(unsigned short), 1, pFile);
	fread(&bits,   sizeof(unsigned char), 1, pFile);
	
	// save attribute
	width = awidth;
	height = aheight;
	bpp = bits;

	// move to pixel datas
	fseek(pFile, length + 1, SEEK_CUR); 

	if(imageType != TGA_RLE)
	{
		if(bits == 24 || bits == 32)
		{
			// channel number
			channels = bits / 8;
			// lenght of a line in byte
			stride = channels * width;

			delete[] datas;
			datas = new unsigned char[stride * height];

			// load all pixel line by line
			for(int y = 0; y < height; y++)
			{
				unsigned char *line = &(datas[stride * y]);

				fread(line, stride, 1, pFile);
			
				// swap R and B channel to have RGB format
				for(i = 0; i < stride; i += channels)
				{
					int temp     = line[i];
					line[i]      = line[i + 2];
					line[i + 2]  = temp;
				}
			}
		}
		// 16bits image?
		else if(bits == 16)
		{
			unsigned short pixels = 0;
			int r=0, g=0, b=0;

			// we force 3 channel
			channels = 3;
			stride = channels * width;
			delete[] datas;
			datas = new unsigned char[stride * height];

			// we load pixel by pixel
			for(int i = 0; i < width*height; i++)
			{
				// lecture du pixel courant
				fread(&pixels, sizeof(unsigned short), 1, pFile);
				
				// 16 bit to 24 bits via masking
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;
				
				// swap and store color in RGB order
				datas[i * 3 + 0] = r;
				datas[i * 3 + 1] = g;
				datas[i * 3 + 2] = b;
			}
		}	
		// unknown format
		else
		{
			//log_printf("impossible de lire le fichier TGA %s : format de pixel inconnu\n", LOG_DEVELOPER|LOG_WARNING, filepath);
			return false;
		}
	}
	else// RLE compressed TGA
	{
		unsigned char rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		delete[] datas;
		datas = new unsigned char[stride * height];
		//temp tab
		unsigned char *colors = new unsigned char [channels];

		// loading
		while(i < width*height)
		{
			// color count
			fread(&rleID, sizeof(unsigned char), 1, pFile);
			
			//not compressed data?
			if(rleID < 128)
			{
				// read next color
				rleID++;

				// read all uncompressed datas
				while(rleID)
				{
					fread(colors, sizeof(unsigned char) * channels, 1, pFile);

					datas[colorsRead + 0] = colors[2];
					datas[colorsRead + 1] = colors[1];
					datas[colorsRead + 2] = colors[0];

					// 32 bit per pixel? add alpha channel
					if(bits == 32)
						datas[colorsRead + 3] = colors[3];

					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			// compressed data
			else
			{
				// substract 127 to know number of color
				rleID -= 127;

				fread(colors, sizeof(unsigned char) * channels, 1, pFile);

				// read same pixel rleID times
				while(rleID)
				{
					datas[colorsRead + 0] = colors[2];
					datas[colorsRead + 1] = colors[1];
					datas[colorsRead + 2] = colors[0];

					// 32 bit per pixel? add alpha channel
					if(bits == 32)
						datas[colorsRead + 3] = colors[3];

					i++;
					rleID--;
					colorsRead += channels;
				}
				
			}
				
		}

		delete[] colors;
	}

	// close file
	fclose(pFile);

	this->format = GL_RGB;
	this->type = GL_UNSIGNED_BYTE;

	return true;
}

unsigned int ImageTGA::getWidth()
{
	return (unsigned int)width;
}

unsigned int ImageTGA::getHeight()
{
	return (unsigned int)height;
}

unsigned short ImageTGA::getBPP()
{
	return (unsigned int)bpp;
}

unsigned char* ImageTGA::getDatasPointer()
{
	return &datas[0];
}

void ImageTGA::setBPP(int newBpp)
{
	bpp = newBpp;
}

