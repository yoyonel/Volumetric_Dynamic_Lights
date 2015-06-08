
#include "ImageBMP.h"

#ifdef __linux__
	#include <cstdio>
#endif

ImageBMP::ImageBMP() : Image()
{
	datas=NULL;
}

ImageBMP::~ImageBMP()
{
	delete [] datas;
}

bool ImageBMP::load(const char *filepath)
{
	bool ret=false;
	FILE *f=NULL;

	f=fopen(filepath,"rb");

	if(f!=NULL)
	{
		//read header
		fread(&bmph.bfType1,1,sizeof(BMP_BYTE),f);
		fread(&bmph.bfType2,1,sizeof(BMP_BYTE),f);
		fread(&bmph.bfSize,1,sizeof(BMP_DWORD),f);
		fread(&bmph.bfReserved1,1,sizeof(BMP_WORD),f);
		fread(&bmph.bfReserved2,1,sizeof(BMP_WORD),f);
		fread(&bmph.bfOffBits,1,sizeof(BMP_DWORD),f);
		if (bmph.bfOffBits==54)//on ne lit que se type d'en-tête
		{
			fread(&bmph.biSize,1,sizeof(BMP_DWORD),f);
			fread(&bmph.biWidth,1,sizeof(BMP_DWORD),f);
			fread(&bmph.biHeight,1,sizeof(BMP_DWORD),f);
			fread(&bmph.biPlanes,1,sizeof(BMP_WORD),f);
			fread(&bmph.biBitCount,1,sizeof(BMP_WORD),f);
			fread(&bmph.biCompression,1,sizeof(BMP_DWORD),f);
			fread(&bmph.biSizeImage,1,sizeof(BMP_DWORD),f);
			fread(&bmph.biXPelsPerMeter,1,sizeof(BMP_DWORD),f);
			fread(&bmph.biYPelsPerMeter,1,sizeof(BMP_DWORD),f);
			fread(&bmph.biClrUsed,1,sizeof(BMP_DWORD),f);
			fread(&bmph.biClrImportant,1,sizeof(BMP_DWORD),f);

			if(bmph.biBitCount==24)
			{
				ret=true;	//the file must exist and image at 8bit per channel (RGB)

				datas = new unsigned char[(unsigned int)bmph.biWidth*(unsigned int)bmph.biHeight*3];

				//load image's datas in BGR order
				fread(&datas[0], sizeof(unsigned char), (unsigned int)bmph.biWidth*(unsigned int)bmph.biHeight*3, f);

				//swap component to have RGB order
				unsigned char tmp;
				for(unsigned int i=0;i<(unsigned int)bmph.biWidth*(unsigned int)bmph.biHeight*3;i+=3)
				{
					tmp=datas[i];
					datas[i]=datas[i+2];
					datas[i+2]=tmp;
				}
			}
		}
		fclose(f);
	}

	this->format = GL_RGB;
	this->type = GL_UNSIGNED_BYTE;
	
	return ret;
}

unsigned int ImageBMP::getWidth()
{
	return (unsigned int)bmph.biWidth;
}

unsigned int ImageBMP::getHeight()
{
	return (unsigned int)bmph.biHeight;
}

unsigned short ImageBMP::getBPP()
{
	return (unsigned int)bmph.biBitCount;
}

unsigned char* ImageBMP::getDatasPointer()
{
	return &datas[0];
}

void ImageBMP::setBPP(int newBpp)
{
	bmph.biBitCount = newBpp;
}

bool ImageBMP::captureAndSaveToDisk(char* filePath, int x, int y, int width, int height)
{
	FILE* f = fopen(filePath,"wb");
	if(f!=NULL)
	{
		BMPHeader bmph;
		GLubyte* pixels = new GLubyte[width*height*3];
		bmph.bfType1 = 'B';
		bmph.bfType2 = 'M';
		bmph.bfSize = width*height*3 + 54;;
		bmph.bfReserved1 = 0;
		bmph.bfReserved2 = 0;
		bmph.bfOffBits = 54;
		bmph.biSize = 40;
		bmph.biWidth = width;
		bmph.biHeight = height;
		bmph.biPlanes = 1;
		bmph.biBitCount = 24;
		bmph.biCompression = 0;
		bmph.biSizeImage = width*height*3;
		bmph.biXPelsPerMeter = 1;
		bmph.biYPelsPerMeter = 1;
		bmph.biClrUsed = 0;
		bmph.biClrImportant = 0;

		//read pixel in current selected buffer for reading
		glReadPixels( x, y, width, height, GL_BGR, GL_UNSIGNED_BYTE, &pixels[0] );

		fwrite(&bmph.bfType1,1,sizeof(BMP_BYTE),f);
		fwrite(&bmph.bfType2,1,sizeof(BMP_BYTE),f);
		fwrite(&bmph.bfSize,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.bfReserved1,1,sizeof(BMP_WORD),f);
		fwrite(&bmph.bfReserved2,1,sizeof(BMP_WORD),f);
		fwrite(&bmph.bfOffBits,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biSize,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biWidth,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biHeight,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biPlanes,1,sizeof(BMP_WORD),f);
		fwrite(&bmph.biBitCount,1,sizeof(BMP_WORD),f);
		fwrite(&bmph.biCompression,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biSizeImage,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biXPelsPerMeter,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biYPelsPerMeter,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biClrUsed,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biClrImportant,1,sizeof(BMP_DWORD),f);

		fwrite(&pixels[0], bmph.biWidth*bmph.biHeight*3 , sizeof(GLubyte), f);

		//cklose file
		fclose(f);
		delete [] pixels;
		return true;	//OK
	}
	return false;
}



bool ImageBMP::TextureToDisk(const char* filePath, int target, GLuint texId)
{
	int width,height;

	FILE* f = fopen(filePath,"wb");
	if(f!=NULL)
	{
		glEnable(target);
		glBindTexture(target,texId);

		glGetTexLevelParameteriv(target,0,GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(target,0,GL_TEXTURE_HEIGHT, &height);

		BMPHeader bmph;
		GLubyte* pixels = new GLubyte[width*height*3*2];
		bmph.bfType1 = 'B';
		bmph.bfType2 = 'M';
		bmph.bfSize = width*height*3 + 54;;
		bmph.bfReserved1 = 0;
		bmph.bfReserved2 = 0;
		bmph.bfOffBits = 54;
		bmph.biSize = 40;
		bmph.biWidth = width;
		bmph.biHeight = height;
		bmph.biPlanes = 1;
		bmph.biBitCount = 24;
		bmph.biCompression = 0;
		bmph.biSizeImage = width*height*3;
		bmph.biXPelsPerMeter = 1;
		bmph.biYPelsPerMeter = 1;
		bmph.biClrUsed = 0;
		bmph.biClrImportant = 0;

		//read texture image
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glGetTexImage(target, 0, GL_BGR, GL_UNSIGNED_BYTE, &pixels[0]);

		glDisable(target);

		fwrite(&bmph.bfType1,1,sizeof(BMP_BYTE),f);
		fwrite(&bmph.bfType2,1,sizeof(BMP_BYTE),f);
		fwrite(&bmph.bfSize,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.bfReserved1,1,sizeof(BMP_WORD),f);
		fwrite(&bmph.bfReserved2,1,sizeof(BMP_WORD),f);
		fwrite(&bmph.bfOffBits,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biSize,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biWidth,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biHeight,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biPlanes,1,sizeof(BMP_WORD),f);
		fwrite(&bmph.biBitCount,1,sizeof(BMP_WORD),f);
		fwrite(&bmph.biCompression,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biSizeImage,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biXPelsPerMeter,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biYPelsPerMeter,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biClrUsed,1,sizeof(BMP_DWORD),f);
		fwrite(&bmph.biClrImportant,1,sizeof(BMP_DWORD),f);

		fwrite(&pixels[0], bmph.biWidth*bmph.biHeight*3 , sizeof(GLubyte), f);

		//cklose file
		fclose(f);
		delete [] pixels;
		return true;	//OK
	}
	return false;
}

