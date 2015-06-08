/*
*	TextureProjection.h
*
*	Programm� par Hillaire S�bastien
*
*
*	- utilit� :
*		Permet de r�gler la g�neration de coordonn�e de texture pour la projection de texture
*	dans un espace 3D. Un system de bind est mis en place pour l'utilisation simplifier.
*		La classe offre un plan de clipping pour emp�cher de rendre la projection arri�re.
*
*	- version :
*		1.0
*
*	- update :
*		06/11/2004 : premi�re version
*
*	- NOTE :
*		Merci � Lukas Heise (axodoss@hotmail.com) pour avoir mis en ligne son exemple de mise
*	en oeuvre de la projection de texture : cela m'a permis de bien comprendre l'impl�mentation.
*
*/

#ifndef TEXTURE_PROJECTION
#define TEXTURE_PROJECTION

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


#include "MathLib.h"


/**
*	Classe permettant d'utilis� la projection de texture pour des spots de lumi�re par exemple.
* tr�s utile aussi pour le shadow-mapping.
*/
class TextureProjection
{
private:

protected:

public:

	/**
	* La matrice de ModelView servant � g�nerer les coordonn�e de texture.
	*/
	float matrix[16];
	/**
	* Le plan de clipping permettant d'�viter de rendre la projection arri�re.
	*/
	double clipPlane[4];

	/**
	* Le constructeur
	*/
	TextureProjection();

	/**
	* Le destructeur
	*/
	virtual ~TextureProjection();

	/**
	* Permet de r�gler les propri�t�s de la projections.
	*
	* @param position un pointeur sur 3 float indiquant la position de l'origine de la projection
	* @param rotation un pointeur sur 3 float indiquant les rotations autour des axes X, Y et Z.
	* @param fov l'angle d'ouverture de la projection.(example : l'angle d'ouverture d'un spot)
	* @param aspect :  
	*/
	void setProperties(float* position, float* rotation, float fov, float aspect);

	/**
	* Permet de r�gler les propri�t�s de la projections.
	*
	* @param position un pointeur une matrice de Novodex pour le calcul du frustum
	* @param fov l'angle d'ouverture de la projection.(example : l'angle d'ouverture d'un spot)
	*/
	//void setProperties(NxMat34* pos, float fov);

	/**
	* Permet de r�gler les propri�t�s de la projections.
	*
	* @param position un pointeur sur 3 float indiquant la position de l'origine de la projection
	* @param rotation un pointeur sur 3 float indiquant les rotations autour des axes X, Y et Z.
	* @param fov l'angle d'ouverture de la projection.(example : l'angle d'ouverture d'un spot)
	* @param aspect :  
	* @param ScaleBiasMatrix la matrice de scale et bias de la projection(*float[16]). Peut �tre utilis� pour changer la projection d'une texture utilis� seulement dans une partie apr�s un rendu dans texture par exemple
	*/
	void setProperties(float* position, float* rotation, float fov, float aspect, float* scaleBiasMatrix);

	/**
	* Permet d'activer la g�neration de coordonn�e de texture pour l'unit� de texture active.
	* A la sortie de cette fonction, le mode de matrice est GL_TEXTURE.
	*/
	void bindProjectionMatrix();

	/**
	* Permet de d�sactiver la g�neration de coordonn�e de texture pour l'unit� de texture active.
	* A la sortie de cette fonction, le mode de matrice est GL_MODELVIEW.
	*/
	void unbindProjectionMatrix();

	/**
	* Permet d'activer le plan de clipping empechant lma projection arri�re.
	*
	* @param planeID l'identifiant OpenGL du plan de clipping(de la forme GL_CLIP_PLANEi)
	*/
	inline void bindBackProjClippingPlane(int planeID)
	{
		glEnable(planeID);
		glClipPlane(planeID,clipPlane);
	}

	/**
	* Permet de d�sactiver le plan de clipping empechant la projection arri�re.
	*
	* @param planeID l'identifiant OpenGL du plan de clipping(de la forme GL_CLIP_PLANEi)
	*/
	inline void unbindBackProjClippingPlane(int planeID)
	{
		glDisable(planeID);
	}

	/**
	*	Permet de r�cup�rer le plan de clipping empechant la projection arri�re.
	*	L'utilisateur doit donc ce d�brouiller seul pour l'activ� : il peut ainsi choisir le
	* num�ro OpenGL d'activation du plan (GL_CLIP_PLANEi).
	*
	* @return l'�quation du plan de clipping.
	*/
	inline double* getBackProjClippingPlane()
	{
		return &clipPlane[0];
	}
};


#endif