

#ifndef TEXTURE_PROJECTION_ORTHO
#define TEXTURE_PROJECTION_ORTHO

#define NOMINMAX

#ifdef WIN32
#include "windows.h"
#endif

#ifdef __linux__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <glew/glew.h>
	#include <glut/glut.h>
#endif


#include "MathLib/MathLib.h"



class TextureProjectionOrtho
{
private:

protected:

public:

	/**
	* La matrice de ModelView servant à génerer les coordonnée de texture.
	*/
	float matrix[16];
	/**
	* Le plan de clipping permettant d'éviter de rendre la projection arrière.
	*/
	double clipPlane[4];

	/**
	* Le constructeur
	*/
	TextureProjectionOrtho();

	/**
	* Le destructeur
	*/
	virtual ~TextureProjectionOrtho();

	/**
	* Permet de régler les propriétés de la projections.
	*
	* @param position un pointeur sur 3 float indiquant la position de l'origine de la projection
	* @param rotation un pointeur sur 3 float indiquant les rotations autour des axes X, Y et Z. 
	*/
	void setProperties(float* position, float* rotation, float sizeX, float sizeY);

	/**
	* Permet de régler les propriétés de la projections.
	*
	* @param position un pointeur sur 3 float indiquant la position de l'origine de la projection
	* @param rotation un pointeur sur 3 float indiquant les rotations autour des axes X, Y et Z.
	* @param fov l'angle d'ouverture de la projection.(example : l'angle d'ouverture d'un spot)
	* @param aspect :  
	* @param ScaleBiasMatrix la matrice de scale et bias de la projection(*float[16]). Peut être utilisé pour changer la projection d'une texture utilisé seulement dans une partie après un rendu dans texture par exemple
	*/
	void setProperties(float* position, float* rotation, float sizeX, float sizeY, float* scaleBiasMatrix);

	/**
	* Permet d'activer la géneration de coordonnée de texture pour l'unité de texture active.
	* A la sortie de cette fonction, le mode de matrice est GL_TEXTURE.
	*/
	void bindProjectionMatrix();

	/**
	* Permet de désactiver la géneration de coordonnée de texture pour l'unité de texture active.
	* A la sortie de cette fonction, le mode de matrice est GL_MODELVIEW.
	*/
	void unbindProjectionMatrix();

	/**
	* Permet d'activer le plan de clipping empechant lma projection arrière.
	*
	* @param planeID l'identifiant OpenGL du plan de clipping(de la forme GL_CLIP_PLANEi)
	*/
	inline void bindBackProjClippingPlane(int planeID)
	{
		glEnable(planeID);
		glClipPlane(planeID,clipPlane);
	}

	/**
	* Permet de désactiver le plan de clipping empechant la projection arrière.
	*
	* @param planeID l'identifiant OpenGL du plan de clipping(de la forme GL_CLIP_PLANEi)
	*/
	inline void unbindBackProjClippingPlane(int planeID)
	{
		glDisable(planeID);
	}

	/**
	*	Permet de récupérer le plan de clipping empechant la projection arrière.
	*	L'utilisateur doit donc ce débrouiller seul pour l'activé : il peut ainsi choisir le
	* numéro OpenGL d'activation du plan (GL_CLIP_PLANEi).
	*
	* @return l'équation du plan de clipping.
	*/
	inline double* getBackProjClippingPlane()
	{
		return &clipPlane[0];
	}
};


#endif