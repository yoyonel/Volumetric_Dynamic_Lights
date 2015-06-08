/*
*	Frustum.h
*
*	Programmé par Hillaire Sébastien
*
*
*	- utilité :
*		Gere un volume de vision. Permet de savoir si des volumes sont dedans pour leur affichage.
*		Permet, de plus, de génerer des volum de clipping
*
*	- version :
*		1.0
*
*	- update :
*		28/08/2004 : - ajout de l'inversion de 4 normal pour corriger activation plan de clipping avec OpenGL
*				==> si des problèmes de clipping apparaissent : sa peut venir de là. PARTIE PAS OPTIMISée
*				==> MIT EN COMMENTAIRE DANS Frustum::getClippingVolume. A DéCOMMENTé SI BESOIN!!!
*		09/07/2004 : - Frustum utilise maintenant que des float* au lieu de vector3 pour etre plus
*					polyvalente. On peut passer des vector3 par &vector3.x comme paramètre
*		04/07/2004 : - ajout de la méthode render dans BoundingBox
*					 - ajout de la méthode genCubicFrustum dans frustum
*		03/07/2004 : première version
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
* Définie un volume de clipping
*/
struct ClippingVolume
{
	/**
	* Les 6 équations des plans de clipping
	*/
	double clipPlanes[6][4];

	/**
	* Permet d'activer le volume de clipping par l'activation des plan de clipping OpenGL
	* Seul les objets dessiner dans le volume seront afficher à l'écran
	* Un et un seul clippingVolume peut être activé à la fois
	*/
	void enableClipVolume();

	/**
	* Un appel a DisableClipVolume n'est nécessaire seulement si l'on veut désactiver le clipping par volume
	* Lorsque l'on active un autre ClippingVolume, on "désactive" automatiquement l'ancien clippingVolume.
	*/
	void DisableClipVolume();
};


/**
* Définie un volume sphérique
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
* Définie un volume formé de 8 extrémité (cube, pavé...)
*/
struct BoundingBox
{
	///////////////////// ANCIENNE VERSION /////////////////////
	/**
	* La position des extrémité
	*	- [0,1,2,3] : les vertices du haut du volume
	*	- [4,5,6,7] : les vertice du bas du volume
	* ==> Pour bien comprendre, visualisé un cube!
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
	* Rend le volume à l'écran
	*/
	void render();

	/**
	*	Permet de calculé les bornes de la aabb a partir de pos et sizex,sizey,sizez.
	* Obligatoire si l'on veut pouvoir testé la collsion entre deux aabb de manière rapide.

	* @pre sizex,sizey,sizez sont des tailles positives.
	*/
	void computeLimit();

	/**
	*	Permet de calculé pos et sizex,sizey,sizez de la aabb a partir des bornes sur les 3 axes.
	* Obligatoire si l'on veut pouvoir testé la collision entre deux aabb de manière rapide.
	*/
	void computeLimitInverse();

	/**
	* Permet de savoir si deux aabb sont 'en collision'.
	*
	* @pre computeLimit a été appelé auparavent(avec l'état courant de la aabb)
	*
	* @param aabb : la aabb avec laquelle on doit testé la collision.
	* @return true s'il y a collision; false sinon.
	*/
	bool collide(BoundingBox* aabb);
};


/**
* Classe permettant de gérer un Volum de vision
*/
class Frustum
{
private:


protected:

	/**
	* Les 6 plan du volume visionné
	* frutsum[x] : a,b,c,d de l'équation du plan (ax+by+cz+d)
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
	* Matrice contenant modl et proj combinée.
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
	* Permet de récupérer le frustum de vision
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
	* @param sideSize la demi-taille d'un coté du cube
	*
	* @return true si le cube est dans le volume
	*/
	bool cubeInFrustum(float *pos, float sideSize);

	/**
	* Permet de savoir si un cube est dans le frustum
	*
	* @param pos la position du centre du cube
	* @param sideSizex la demi-taille d'un coté du cube sur l'axe X
	* @param sideSizey la demi-taille d'un coté du cube sur l'axe Y
	* @param sideSizez la demi-taille d'un coté du cube sur l'axe Z
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
	* Permet de copier le volume de clipping du frustum courant pour utilisation ultérieur
	*
	* @param cv le volume de clipping à génerer à partir du frustum courant
	*/
	void getClippingVolume(ClippingVolume *cv);

	/**
	* Permet de genérer un frustum cubic
	* Utilise l'accéleration OpenGL mais ne modifie pas les matrice OpenGL courante
	* Sert surtout pour obtenir par la suite un volume de clipping
	*
	* @param pos la position du centre du cube
	* @param sideSizex la demi-taille d'un coté du cube sur l'axe X
	* @param sideSizey la demi-taille d'un coté du cube sur l'axe Y
	* @param sideSizez la demi-taille d'un coté du cube sur l'axe Z
	*/
	void genCubicFrustum(float *pos, float sideSizex, float sideSizey, float sideSizez);

	/**
	*	Permet d'obtenir un pointeur sur la matrice de projection
	*/
	inline const float* getModelViewMat(){ return this->modl;}

	/**
	*	Permet de créer un frustum donc l'origine est en 'camera' est les plan de clipping passe
	* par les point de portal spécifier.
	*
	* REM : le portal doit etre orienter dans le sens ????? pour avoir un frustum valide
	*
	* @param camera pointeur sur les 3 float donannt la position de la caméra
	* @param portal les quatres points du portal.
	*/
	void setPortalv(float camera[3],float portal[4][3]);

	/**
	*	Permet de bouger le plan de clipping proche. Pour le faire passé par le point p.
	* (utile pour frustum culling de portal)
	*
	* @param le point par lequel on veut que le plan de clipping proche passe.(pointeur sur 3 float xyz)
	*/
	void moveFrontClipPlane(float* p);

};


#endif