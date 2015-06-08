/*
*	Frustum.h
*
*	Programm� par Hillaire S�bastien
*
*
*	- utilit� :
*		Gere un volume de vision. Permet de savoir si des volumes sont dedans pour leur affichage.
*		Permet, de plus, de g�nerer des volum de clipping
*
*	- version :
*		1.0
*
*	- update :
*		28/08/2004 : - ajout de l'inversion de 4 normal pour corriger activation plan de clipping avec OpenGL
*				==> si des probl�mes de clipping apparaissent : sa peut venir de l�. PARTIE PAS OPTIMIS�e
*				==> MIT EN COMMENTAIRE DANS Frustum::getClippingVolume. A D�COMMENT� SI BESOIN!!!
*		09/07/2004 : - Frustum utilise maintenant que des float* au lieu de vector3 pour etre plus
*					polyvalente. On peut passer des vector3 par &vector3.x comme param�tre
*		04/07/2004 : - ajout de la m�thode render dans BoundingBox
*					 - ajout de la m�thode genCubicFrustum dans frustum
*		03/07/2004 : premi�re version
*
*	- NOTE :
*		...
*
*/


#ifndef FRUSTUM
#define FRUSTUM

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



#include <math.h>
#include "MathLib.h"


/**
* D�finie un volume de clipping
*/
struct ClippingVolume
{
	/**
	* Les 6 �quations des plans de clipping
	*/
	double clipPlanes[6][4];

	/**
	* Permet d'activer le volume de clipping par l'activation des plan de clipping OpenGL
	* Seul les objets dessiner dans le volume seront afficher � l'�cran
	* Un et un seul clippingVolume peut �tre activ� � la fois
	*/
	void enableClipVolume();

	/**
	* Un appel a DisableClipVolume n'est n�cessaire seulement si l'on veut d�sactiver le clipping par volume
	* Lorsque l'on active un autre ClippingVolume, on "d�sactive" automatiquement l'ancien clippingVolume.
	*/
	void DisableClipVolume();
};


/**
* D�finie un volume sph�rique
*/
struct BoundingSphere
{
	/**
	*le rayon de la sphere
	*/
	float		radius;
	/**
	* la position du centre de la sphere
	*/
	Vector3		pos;
};


/**
* D�finie un volume form� de 8 extr�mit� (cube, pav�...)
*/
struct BoundingBox
{
	///////////////////// ANCIENNE VERSION /////////////////////
	/**
	* La position des extr�mit�
	*	- [0,1,2,3] : les vertices du haut du volume
	*	- [4,5,6,7] : les vertice du bas du volume
	* ==> Pour bien comprendre, visualis� un cube!
	*/
	//vector3		vertices[8];

	///////////////////// NOUVELLE VERSION /////////////////////
	/**
	* la position du centre du pave
	*/
	Vector3 pos;
	/**
	* la demi-taille du cube sue l'axe x,y et z
	*/
	float sizex,sizey,sizez;

	//Les bornes de la aabb
	float minx,miny,minz,maxx,maxy,maxz;

	/**
	* Rend le volume � l'�cran
	*/
	void render();

	/**
	*	Permet de calcul� les bornes de la aabb a partir de pos et sizex,sizey,sizez.
	* Obligatoire si l'on veut pouvoir test� la collsion entre deux aabb de mani�re rapide.

	* @pre sizex,sizey,sizez sont des tailles positives.
	*/
	void computeLimit();

	/**
	*	Permet de calcul� pos et sizex,sizey,sizez de la aabb a partir des bornes sur les 3 axes.
	* Obligatoire si l'on veut pouvoir test� la collision entre deux aabb de mani�re rapide.
	*/
	void computeLimitInverse();

	/**
	* Permet de savoir si deux aabb sont 'en collision'.
	*
	* @pre computeLimit a �t� appel� auparavent(avec l'�tat courant de la aabb)
	*
	* @param aabb : la aabb avec laquelle on doit test� la collision.
	* @return true s'il y a collision; false sinon.
	*/
	bool collide(BoundingBox* aabb);
};


/**
* Classe permettant de g�rer un Volum de vision
*/
class Frustum
{
private:


protected:

	/**
	* Les 6 plan du volume visionn�
	* frutsum[x] : a,b,c,d de l'�quation du plan (ax+by+cz+d)
	*/
	float		frustum[6][4];
	/**
	* La matrice de MODELISATION-VISUALISATION
	*/
	float		modl[16];
	/**
	* La matrice de PROJECTION
	*/
	float		proj[16]; 
	/**
	* Matrice contenant modl et proj combin�e.
	*/
	float		clip[16];

public:

	/**
	* Constructeur
	*/
	Frustum();

	/**
	* Destructeur
	*/
	~Frustum();

	/**
	* Permet de r�cup�rer le frustum de vision
	*/
	void update();

	/**
	* Permet de savoir si un point est dans le frustum
	*
	* @param pos la position du point
	*
	* @return true si le point est dans le volume
	*/
	bool pointInFrustum(float *pos);

	/**
	* Permet de savoir si une sphere est dans le frustum
	*
	* @param pos la position du centre de la sphere
	* @param radius le rayon de la sphere
	*
	* @return true si la sphere est dans le volume
	*/
	bool sphereInFrustum(float *pos, float radius);

	/**
	* Permet de savoir si un cube est dans le frustum
	*
	* @param pos la position du centre du cube
	* @param sideSize la demi-taille d'un cot� du cube
	*
	* @return true si le cube est dans le volume
	*/
	bool cubeInFrustum(float *pos, float sideSize);

	/**
	* Permet de savoir si un cube est dans le frustum
	*
	* @param pos la position du centre du cube
	* @param sideSizex la demi-taille d'un cot� du cube sur l'axe X
	* @param sideSizey la demi-taille d'un cot� du cube sur l'axe Y
	* @param sideSizez la demi-taille d'un cot� du cube sur l'axe Z
	*
	* @return true si le cube est dans le volume
	*/
	bool paveInFrustum(float *pos, float sideSizex, float sideSizey, float sideSizez);

	/**
	* Permet de savoir si un rectangle est dans le frustum
	*
	* @param vertices la position des vertices du rectangle
	*
	* @return true si le rectangle est dans le volume
	*/
	bool rectangleInFrustum(float *v1,float *v2,float *v3,float *v4);

	/**
	* Permet de savoir si un triangle est dans le frustum
	*
	* @param vertices la position des vertices du triangle
	*
	* @return true si le triangle est dans le volume
	*/
	bool triangleInFrustum(float *v1,float *v2,float *v3);

	/**
	* Permet de copier le volume de clipping du frustum courant pour utilisation ult�rieur
	*
	* @param cv le volume de clipping � g�nerer � partir du frustum courant
	*/
	void getClippingVolume(ClippingVolume *cv);

	/**
	* Permet de gen�rer un frustum cubic
	* Utilise l'acc�leration OpenGL mais ne modifie pas les matrice OpenGL courante
	* Sert surtout pour obtenir par la suite un volume de clipping
	*
	* @param pos la position du centre du cube
	* @param sideSizex la demi-taille d'un cot� du cube sur l'axe X
	* @param sideSizey la demi-taille d'un cot� du cube sur l'axe Y
	* @param sideSizez la demi-taille d'un cot� du cube sur l'axe Z
	*/
	void genCubicFrustum(float *pos, float sideSizex, float sideSizey, float sideSizez);

	/**
	*	Permet d'obtenir un pointeur sur la matrice de projection
	*/
	inline const float* getModelViewMat(){ return this->modl;}

	/**
	*	Permet de cr�er un frustum donc l'origine est en 'camera' est les plan de clipping passe
	* par les point de portal sp�cifier.
	*
	* REM : le portal doit etre orienter dans le sens ????? pour avoir un frustum valide
	*
	* @param camera pointeur sur les 3 float donannt la position de la cam�ra
	* @param portal les quatres points du portal.
	*/
	void setPortalv(float camera[3],float portal[4][3]);

	/**
	*	Permet de bouger le plan de clipping proche. Pour le faire pass� par le point p.
	* (utile pour frustum culling de portal)
	*
	* @param le point par lequel on veut que le plan de clipping proche passe.(pointeur sur 3 float xyz)
	*/
	void moveFrontClipPlane(float* p);

};


#endif