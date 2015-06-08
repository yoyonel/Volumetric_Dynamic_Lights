/*
*	TextureProjection.cpp
*
*	Programmé par Hillaire Sébastien
*
*
*	Se référé au fichier en-tête .h pour plus de détail
*
*/

#include "TextureProjectionOrtho.h"

#include <iostream>


TextureProjectionOrtho::TextureProjectionOrtho()
{
	//pas d'initialisation des tableau de matrice et plan
}

TextureProjectionOrtho::~TextureProjectionOrtho()
{
}

void TextureProjectionOrtho::setProperties(float* position, float* rotation, float sizeX, float sizeY)
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
	//gluPerspective(fov,aspect,1.0f,2.0f);
	glOrtho(-sizeX/2.0f,sizeX/2.0f, -sizeY/2.0f,sizeY/2.0f, -10.0f,10.0f);
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

void TextureProjectionOrtho::setProperties(float* position, float* rotation, float sizeX, float sizeY, float* scaleBiasMatrix)
{
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	//	Construit la matrice de projection.
	//Nous allons utilisé OpenGL pour les calculs.
	glLoadMatrixf(scaleBiasMatrix);
	//gluPerspective(fov,aspect,1.0f,2.0f);
	glOrtho(-sizeX/2.0f,sizeX/2.0f, -sizeY/2.0f,sizeY/2.0f, 0.0f,2.0f);
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

void TextureProjectionOrtho::bindProjectionMatrix()
{
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadMatrixf(matrix);
		glMatrixMode(GL_MODELVIEW);
}

void TextureProjectionOrtho::unbindProjectionMatrix()
{
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
}