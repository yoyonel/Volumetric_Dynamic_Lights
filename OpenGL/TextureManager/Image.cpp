
#include "Image.h"


//for the sqrt function
#include <math.h>


Image::Image()
{
	datas=NULL;
}

Image::~Image()
{
}

void Image::toHeightMap()
{
	int chanel = getBPP()/8;
	int size = getHeight() * getWidth();
	unsigned char * p = datas;
	for (int i = 0; i < size; i++)
	{
		unsigned char * texel = p;
		int luminance = 0;;
		if (chanel <=3)
		{
			for (int j = 0; j < chanel; j++)
			{
				luminance +=texel[j];
			}
			luminance /= chanel;
			//luminance *= luminance;
			for (int t = 0; t < chanel; t++)
			{
				*p = luminance;
				p++;
			}
		}
		else
		{
			for (int j = 0; j < 3; j++)
			{
				luminance +=texel[j];
			}
			luminance /= chanel;
			//luminance *= luminance;
			int t;
			for (t = 0; t < 3; t++)
			{
				*p = luminance;
				p++;
			}
			for (t = 3; t < chanel; t++)
			{
				p++;
			}
		}
	}

}

void Image::toNormalMap(int factor, bool storeHeightAsAlpha)
{
	float V1[3]; // Y axis vector
	float V2[3]; // X axis vector
	float normal[3];
	int channels = getBPP()/8;
	float len;
	unsigned char * pixel[3];	//0=base pixel, 1=up pixel, 2=right pixel

	//choose channel number
	int newChannels;
	if(storeHeightAsAlpha)
	{
		if (channels == 3)
		{
			newChannels = 4;
		}
		else
		{
			newChannels = channels;
		}
	}
	else
	{
		newChannels = channels;
	}
	unsigned char * newBuffer = new unsigned char[getHeight()*getWidth()*newChannels];	//the new image buffer in system memory

	for (int i = 0; i < getWidth(); i++)
	{
		for (int j = 0; j < getHeight(); j++)
		{
			int pos1 = (i*getHeight() + j)*channels;
			int newPos1 = (i*getHeight() + j)*newChannels;
			int pos2 = (((i+1)%getWidth())*getHeight() + j)*channels;
			int pos3 = (i*getHeight() + ((j+1)%getHeight()))*channels;

			pixel[0] = &datas[pos1];
			pixel[1] = &datas[pos3];
			pixel[2] = &datas[pos2];

			V1[0] = 0;
			V1[1] = 1;
			V1[2] = (((int)*pixel[1] - (int)*pixel[0])/255.f) * factor;	// [0..255]
			V2[0] = 1;
			V2[1] = 0;
			V2[2] = (((int)*pixel[2] - (int)*pixel[0])/255.f) * factor;	// [0..255]

			// normal vector
			normal[0] = V2[1] * V1[2] - V2[2] * V1[1];
			normal[1] = V2[2] * V1[0] - V2[0] * V1[2];
			normal[2] = V2[0] * V1[1] - V2[1] * V1[0];
			// normalisation of the normal vector
			len = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
			normal[0]/=len;
			normal[1]/=len;
			normal[2]/=len;
			newBuffer[newPos1] = normal[1]*127 + 128;
			newBuffer[newPos1+1] = normal[0]*127 + 128;
			newBuffer[newPos1+2] = normal[2]*127 + 128;
			// we copy alpha if needed
			if (newChannels == 4)
			{
				if (!storeHeightAsAlpha)
				{
					// we save alpha of preceding image
					newBuffer[newPos1+3] = datas[pos1+3];
				}
				else
				{
					// we save height in the alpha channel of the new buffer
					newBuffer[newPos1+3] = datas[pos1];
				}
			}
			setBPP(newChannels * 8);
		}
	}
	// buffer swapping
	delete [] datas;
	datas = newBuffer;
}
