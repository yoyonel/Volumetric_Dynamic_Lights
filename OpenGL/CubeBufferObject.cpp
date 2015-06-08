
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

#include "CubeBufferObject.h"

#include "ImageBMP.h"

//////////////////////////////////////////////////////////////////////////////////



CubeBufferObject::CubeBufferObject(unsigned int fWidth, unsigned int fHeight, bool hasRGB, GLuint colorBufferFormat)
{
	this->virtualfaceNoBorderWidth = fWidth;
	this->virtualfaceNoBorderHeight = fHeight;
	this->fboWidth = this->virtualfaceNoBorderWidth*3+6;
	this->fboHeight= this->virtualfaceNoBorderHeight*2+4;
	this->virtualfaceWidth = this->fboWidth/3;
	this->virtualfaceHeight = this->fboHeight/2;

	unsigned int iFormat = colorBufferFormat;
	unsigned int glLinear = GL_LINEAR;
	unsigned int glRepeat = GL_CLAMP;

	//creation of the frame buffer object containing the virtual depth cubemap
	this->virtualDepthCubeMap = new FrameBufferObject(this->fboWidth, this->fboHeight,hasRGB?1:0,&iFormat,&glRepeat,&glRepeat,&glLinear,&glLinear,FBO_DepthBufferType_TEXTURE);

	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_X_ARB][0]= 0;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_X_ARB][1]= this->virtualfaceWidth ;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_X_ARB][2]= this->virtualfaceHeight;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_X_ARB][3]= this->virtualfaceHeight*2;
	
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_Y_ARB][0]= this->virtualfaceWidth;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_Y_ARB][1]= this->virtualfaceWidth*2;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_Y_ARB][2]= this->virtualfaceHeight;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_Y_ARB][3]= this->virtualfaceHeight*2;
	
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_Z_ARB][0]= this->virtualfaceWidth*2;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_Z_ARB][1]= this->virtualfaceWidth*3;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_Z_ARB][2]= this->virtualfaceHeight;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_POSITIVE_Z_ARB][3]= this->virtualfaceHeight*2;

	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_X_ARB][0]= 0;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_X_ARB][1]= this->virtualfaceWidth;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_X_ARB][2]= 0;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_X_ARB][3]= this->virtualfaceHeight;
	
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_Y_ARB][0]= this->virtualfaceWidth;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_Y_ARB][1]= this->virtualfaceWidth*2;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_Y_ARB][2]= 0;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_Y_ARB][3]= this->virtualfaceHeight;
	
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_Z_ARB][0]= this->virtualfaceWidth*2;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_Z_ARB][1]= this->virtualfaceWidth*3;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_Z_ARB][2]= 0;
	this->virtualCubeMapFaceBounds[CBO_FRUSTUM_NEGATIVE_Z_ARB][3]= this->virtualfaceHeight;

	
	
	// Generation of the indirection cubemap
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glGenTextures(1,&this->cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, this->cubeMapID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	const unsigned int cmfacesize = 256;
	float resS, resT;
	static float face[2*cmfacesize*cmfacesize];
	for (int i = 0; i < 6; i++)
	{
		for (int y = 0; y < cmfacesize; y++)
		{
			for (int x = 0; x < cmfacesize; x++)
			{
				float ratioS = (float)x/(float)cmfacesize;
				float ratioT = (float)y/(float)cmfacesize;

				switch (i) 
				{
				case CBO_FRUSTUM_POSITIVE_X_ARB:
					resS = (float)(1+ ratioS*this->virtualfaceNoBorderWidth) / (float)(this->fboWidth);
					resT = (float)(3+ this->virtualfaceNoBorderHeight+ ratioT*this->virtualfaceNoBorderHeight) / (float)(this->fboHeight);
					break;
				case CBO_FRUSTUM_POSITIVE_Y_ARB:
					resS = (float)(3+ this->virtualfaceNoBorderWidth+ ratioS*this->virtualfaceNoBorderWidth) / (float)(this->fboWidth);
					resT = (float)(3+ this->virtualfaceNoBorderHeight+ ratioT*this->virtualfaceNoBorderHeight) / (float)(this->fboHeight);
					break;
				case CBO_FRUSTUM_POSITIVE_Z_ARB:
					resS = (float)(5+ this->virtualfaceNoBorderWidth*2+ ratioS*this->virtualfaceNoBorderWidth) / (float)(this->fboWidth);
					resT = (float)(3+ this->virtualfaceNoBorderHeight+ ratioT*this->virtualfaceNoBorderHeight) / (float)(this->fboHeight);
					break;
				case CBO_FRUSTUM_NEGATIVE_X_ARB:
					resS = (float)(1+ ratioS*this->virtualfaceNoBorderWidth) / (float)(this->fboWidth);
					resT = (float)(1+ ratioT*this->virtualfaceNoBorderHeight) / (float)(this->fboHeight);
					break;
				case CBO_FRUSTUM_NEGATIVE_Y_ARB:
					resS = (float)(3+ this->virtualfaceNoBorderWidth+ ratioS*this->virtualfaceNoBorderWidth) / (float)(this->fboWidth);
					resT = (float)(1+ ratioT*this->virtualfaceNoBorderHeight) / (float)(this->fboHeight);
					break;
				case CBO_FRUSTUM_NEGATIVE_Z_ARB:
					resS = (float)(5+ this->virtualfaceNoBorderWidth*2+ ratioS*this->virtualfaceNoBorderWidth) / (float)(this->fboWidth);
					resT = (float)(1+ ratioT*this->virtualfaceNoBorderHeight) / (float)(this->fboHeight);
					break;
				default:
					resS=0.0;
					resT=0.0;
					break;
				}

				face[2*(y*cmfacesize+x) + 0] = resS;
				face[2*(y*cmfacesize+x) + 1] = resT;
			}
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+i, 0, GL_LUMINANCE_ALPHA16F_ARB, cmfacesize, cmfacesize, 0, GL_LUMINANCE_ALPHA, GL_FLOAT, face);
	}
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	// Now we initialize frustums
	this->centerPosition.set(0.0,0.0,0.0);
	this->nearClipPlane = 1.0f;
	this->farClipPlane = 10.0f;
	this->updateFrustums();
}

CubeBufferObject::~CubeBufferObject()
{
	glDeleteTextures(1,&this->cubeMapID);

	delete this->virtualDepthCubeMap;
}



//////////////////////////////////////////////////////////////////////////////////



void CubeBufferObject::setCenterPosition(Vector3& centerPos)
{
	this->centerPosition = centerPos;
}

void CubeBufferObject::setClipPlanes(float nearClipPlane, float farClipPlane)
{
	this->nearClipPlane = nearClipPlane;
	this->farClipPlane = farClipPlane;
}



void CubeBufferObject::updateFrustums()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(90,1.0,this->nearClipPlane,this->farClipPlane);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB
	glPushMatrix();
	gluLookAt(this->centerPosition.x, this->centerPosition.y, this->centerPosition.z,	//eye position
		 this->centerPosition.x+1.0, this->centerPosition.y+0.0, this->centerPosition.z+0.0,			//look at position
		 0.0,-1.0, 0.0);		//up direction (t coordinate)
	this->frustums[CBO_FRUSTUM_POSITIVE_X_ARB].update();
	glPopMatrix();

	//GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB
	glPushMatrix();
	gluLookAt(this->centerPosition.x, this->centerPosition.y, this->centerPosition.z,	//eye position
		this->centerPosition.x-1.0, this->centerPosition.y+0.0, this->centerPosition.z+0.0,			//look at position
		 0.0,-1.0, 0.0);		//up direction (t coordinate)
	this->frustums[CBO_FRUSTUM_NEGATIVE_X_ARB].update();
	glPopMatrix();

	//GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB
	glPushMatrix();
	gluLookAt(this->centerPosition.x, this->centerPosition.y, this->centerPosition.z,	//eye position
		 this->centerPosition.x+0.0, this->centerPosition.y+1.0, this->centerPosition.z+0.0,			//look at position
		 0.0, 0.0, 1.0);		//up direction (t coordinate)
	this->frustums[CBO_FRUSTUM_POSITIVE_Y_ARB].update();
	glPopMatrix();

	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB
	glPushMatrix();
	gluLookAt(this->centerPosition.x, this->centerPosition.y, this->centerPosition.z,	//eye position
		 this->centerPosition.x+0.0, this->centerPosition.y-1.0, this->centerPosition.z+0.0,			//look at position
		 0.0, 0.0,-1.0);		//up direction (t coordinate)
	this->frustums[CBO_FRUSTUM_NEGATIVE_Y_ARB].update();
	glPopMatrix();

	//GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB
	glPushMatrix();
	gluLookAt(this->centerPosition.x, this->centerPosition.y, this->centerPosition.z,	//eye position
		 this->centerPosition.x+0.0, this->centerPosition.y+0.0, this->centerPosition.z+1.0,			//look at position
		 0.0,-1.0, 0.0);		//up direction (t coordinate)
	this->frustums[CBO_FRUSTUM_POSITIVE_Z_ARB].update();
	glPopMatrix();

	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB
	glPushMatrix();
	gluLookAt(this->centerPosition.x, this->centerPosition.y, this->centerPosition.z,	//eye position
		 this->centerPosition.x+0.0, this->centerPosition.y+0.0, this->centerPosition.z-1.0,			//look at position
		 0.0,-1.0, 0.0);		//up direction (t coordinate)
	this->frustums[CBO_FRUSTUM_NEGATIVE_Z_ARB].update();
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

Frustum& CubeBufferObject::getFrustum(CBO_Frustum cboFrustum)
{
	return this->frustums[cboFrustum];
}

void CubeBufferObject::saveAndSetProjectionMatrix() const
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//gluPerspective(90,1.0,this->nearClipPlane,this->farClipPlane);

	//we compensate the view angle due to the one pixel border
	float virtualDepthCubeMapViewAngle = 2.0*atanf((float)this->virtualfaceWidth/(float)this->virtualfaceNoBorderWidth)*RAD2DEG;
	gluPerspective( virtualDepthCubeMapViewAngle ,1.0,this->nearClipPlane,this->farClipPlane);

	glMatrixMode(GL_MODELVIEW);

}

void CubeBufferObject::restoreProjectionMatrix() const
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void CubeBufferObject::saveAndSetViewPort(CBO_Frustum cboFrustum) const
{
	glPushAttrib(GL_VIEWPORT_BIT);

	const unsigned int* bb = &this->virtualCubeMapFaceBounds[cboFrustum][0];
	glViewport(bb[0], bb[2], bb[1]-bb[0], bb[3]-bb[2]);
}


void CubeBufferObject::saveAndSetWholeViewPort() const
{
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,this->virtualDepthCubeMap->getWidth(),this->virtualDepthCubeMap->getHeight());
}



void CubeBufferObject::restoreViewPort() const
{
	glPopAttrib();
}


void CubeBufferObject::enableRenderToColorAndDepth() const
{
	this->virtualDepthCubeMap->enableRenderToColorAndDepth(0);
}

void CubeBufferObject::disableRenderToColorDepth() const
{
	this->virtualDepthCubeMap->disableRenderToColorDepth();
}


void CubeBufferObject::bindIndirectionCubeMap()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB,this->cubeMapID);
}

void CubeBufferObject::bindVirtualDepthMap()
{	
	this->virtualDepthCubeMap->bindDepthTexture();
}

void CubeBufferObject::bindVirtualColorMap()
{	
	this->virtualDepthCubeMap->bindColorTexture(0);
}

void CubeBufferObject::clearAllBuffers(bool colorBuffer, bool depthBuffer)
{
	int clearFlag = 0;
	if(colorBuffer) clearFlag|=GL_COLOR_BUFFER_BIT;
	if(depthBuffer) clearFlag|=GL_DEPTH_BUFFER_BIT;

	this->virtualDepthCubeMap->enableRenderToColorAndDepth(0);
	this->saveAndSetWholeViewPort();
	glClear(clearFlag);
	this->restoreViewPort();
	this->virtualDepthCubeMap->disableRenderToColorDepth();
}


unsigned int CubeBufferObject::getWholeWidth()
{
	return this->virtualDepthCubeMap->getWidth();
}

unsigned int CubeBufferObject::getWholeHeight()
{
	return this->virtualDepthCubeMap->getHeight();
}


