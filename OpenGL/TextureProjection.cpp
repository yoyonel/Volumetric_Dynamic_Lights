/*
*	TextureProjection.cpp
*
*	Programmé par Hillaire Sébastien
*
*
*	Se référé au fichier en-tête .h pour plus de détail
*
*/

#include "TextureProjection.h"

#include <iostream>


TextureProjection::TextureProjection()
{
	//pas d'initialisation des tableau de matrice et plan
}

TextureProjection::~TextureProjection()
{
}

void TextureProjection::setProperties(float* position, float* rotation, float fov, float aspect)
{
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	static float scaleBiasMatrix[16] = { 0.5f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.5f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.5f, 0.0f,
									0.5f, 0.5f, 0.5f, 1.0f };

	//	Construit la matrice de projection.
	//Nous allons utilisé OpenGL pour les calculs.
	glLoadMatrixf(scaleBiasMatrix);
	gluPerspective(fov,aspect,1.0f,2.0f);
	glRotatef(rotation[0],1,0,0);
	glRotatef(rotation[1],0,1,0);
	glRotatef(rotation[2],0,0,1);
	glTranslatef(position[0],position[1],position[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);

	//Le plan de clipping qui sert à éliminé la projection arrière indésirable.
	clipPlane[0] = (double)matrix[3];
	clipPlane[1] = (double)matrix[7];
	clipPlane[2] = (double)matrix[11];
	clipPlane[3] = (double)matrix[15];

	glPopMatrix();
}

/*void TextureProjection::setProperties(NxMat34* pos, float fov)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	static float scaleBiasMatrix[16] = { 0.5f, 0.0f, 0.0f, 0.0f,
										0.0f, 0.5f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.5f, 0.0f,
										0.5f, 0.5f, 0.5f, 1.0f };

	//	Construit la matrice de projection.
	//Nous allons utilisé OpenGL pour les calculs.
	glLoadMatrixf(scaleBiasMatrix);
	gluPerspective(fov,1.0f,1.0f,2.0f);

	NxQuat quaternion(pos->M);
	glRotatef(-RAD2DEG*quaternion.getAngle(),quaternion.x,quaternion.y,quaternion.z);
	glTranslatef(-pos->t.x,-pos->t.y,-pos->t.z);

	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);

	//Le plan de clipping qui sert à éliminé la projection arrière indésirable.
	clipPlane[0] = (double)matrix[3];
	clipPlane[1] = (double)matrix[7];
	clipPlane[2] = (double)matrix[11];
	clipPlane[3] = (double)matrix[15];

	glPopMatrix();
}*/

void TextureProjection::setProperties(float* position, float* rotation, float fov, float aspect,float* scaleBiasMatrix)
{
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	//	Construit la matrice de projection.
	//Nous allons utilisé OpenGL pour les calculs.
	glLoadMatrixf(scaleBiasMatrix);
	gluPerspective(fov,aspect,1.0f,2.0f);
	glRotatef(rotation[0],1,0,0);
	glRotatef(rotation[1],0,1,0);
	glRotatef(rotation[2],0,0,1);
	glTranslatef(position[0],position[1],position[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);

	//Le plan de clipping qui sert à éliminé la projection arrière indésirable.
	clipPlane[0] = (double)matrix[3];
	clipPlane[1] = (double)matrix[7];
	clipPlane[2] = (double)matrix[11];
	clipPlane[3] = (double)matrix[15];

	glPopMatrix();
}

void TextureProjection::bindProjectionMatrix()
{
/*		static float planeS[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
		static float planeT[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
		static float planeR[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
		static float planeQ[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
		glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
		glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
		glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);

		glTexGenfv(GL_S,GL_EYE_PLANE,planeS);
		glTexGenfv(GL_T,GL_EYE_PLANE,planeT);
		glTexGenfv(GL_R,GL_EYE_PLANE,planeR);
		glTexGenfv(GL_Q,GL_EYE_PLANE,planeQ);

		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable(GL_TEXTURE_GEN_Q);*/

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadMatrixf(matrix);//on charge notre texture de projection dans la matrice de texture
		glMatrixMode(GL_MODELVIEW);
}

void TextureProjection::unbindProjectionMatrix()
{
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

/*		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_GEN_Q);*/
}