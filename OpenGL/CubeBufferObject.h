
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


#ifndef _CUBE_BUFFER_OBJECT_OPENGL_
#define _CUBE_BUFFER_OBJECT_OPENGL_

#ifdef WIN32
#define NOMINMAX		//for the physx sdk
#include <windows.h>
#endif

#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "FrameBufferObject.h"

#include "Frustum.h"


typedef enum{
	CBO_FRUSTUM_POSITIVE_X_ARB =0,
	CBO_FRUSTUM_NEGATIVE_X_ARB =1,
	CBO_FRUSTUM_POSITIVE_Y_ARB =2,
	CBO_FRUSTUM_NEGATIVE_Y_ARB =3,
	CBO_FRUSTUM_POSITIVE_Z_ARB =4,
	CBO_FRUSTUM_NEGATIVE_Z_ARB =5
} CBO_Frustum;


/**
 *	The CubeBufferObject class allowing to use render to a cube map
 */
class CubeBufferObject
{
private:

	/**
	 *	default constructeur not allowed
	 */
	CubeBufferObject();
	/**
	 *	copy constructeur not allowed
	 */
	CubeBufferObject(CubeBufferObject&);
	/**
	 * copy assignement operator not allowed
	 */
	CubeBufferObject& operator=(const CubeBufferObject&);

	/**
	 * the width in pixel of each cube face
	 */
	unsigned int fboWidth;
	/**
	 * the height in pixel of each cube face
	 */
	unsigned int fboHeight;

	GLuint cubeMapID;

	Frustum frustums[6];

	Vector3 centerPosition;

	unsigned int virtualCubeMapFaceBounds[6][4];
	unsigned int virtualfaceWidth;
	unsigned int virtualfaceHeight;
	unsigned int virtualfaceNoBorderWidth;
	unsigned int virtualfaceNoBorderHeight;

	float nearClipPlane, farClipPlane;

	FrameBufferObject* virtualDepthCubeMap;

public:

	/**
	 *	The only available constructor
	 */
	CubeBufferObject(unsigned int width, unsigned int height, bool hasRGB, GLuint colorBufferFormat=GL_RGB8);

	/**
	 *	The destructor
	 */
	~CubeBufferObject();


	/**
	 *
	 */
	void setCenterPosition(Vector3& centerPos);
	/**
	 *
	 */
	void setClipPlanes(float nearClipPlane, float farClipPlane);
	/**
	 *
	 */
	void updateFrustums();

	/**
	 *
	 */
	Frustum& getFrustum(CBO_Frustum cboFrustum);


	/**
	 */
	void saveAndSetProjectionMatrix() const;
	/**
	 * @see saveAndSetViewPort
	 */
	void restoreProjectionMatrix() const;

	/**
	 */
	void saveAndSetViewPort(CBO_Frustum cboFrustum) const;
	/**
	 */
	void saveAndSetWholeViewPort() const;
	/**
	 * @see saveAndSetViewPort
	 */
	void restoreViewPort() const;



	void enableRenderToColorAndDepth() const;

	void disableRenderToColorDepth() const;



	void bindIndirectionCubeMap();

	void bindVirtualDepthMap();

	void bindVirtualColorMap();


	void clearAllBuffers(bool colorBuffer, bool depthBuffer);

	unsigned int getWholeWidth();
	unsigned int getWholeHeight();

};



#endif

