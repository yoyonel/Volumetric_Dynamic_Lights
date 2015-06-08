/*
*	Frustum.cpp
*
*	Programmé par Hillaire Sébastien
*
*
*	Se référé au fichier en-tête .h pour plus de détail
*
*/

#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}


void Frustum::update()
{
	float   t;

	//On récupérer les matrice de modelisation-visualisation et de projection d'OpenGL
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );
	glGetFloatv( GL_PROJECTION_MATRIX, proj );


	/* On combine les 2, en multipliant la matrice de projection par celle de modelisation*/
	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

	/* Extrait le plan de DROITE */
	frustum[0][0] = clip[ 3] - clip[ 0];
	frustum[0][1] = clip[ 7] - clip[ 4];
	frustum[0][2] = clip[11] - clip[ 8];
	frustum[0][3] = clip[15] - clip[12];
	t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
	frustum[0][0] /= t;
	frustum[0][1] /= t;
	frustum[0][2] /= t;
	frustum[0][3] /= t;

	/* Extrait le plan de GAUCHE */
	frustum[1][0] = clip[ 3] + clip[ 0];
	frustum[1][1] = clip[ 7] + clip[ 4];
	frustum[1][2] = clip[11] + clip[ 8];
	frustum[1][3] = clip[15] + clip[12];
	t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
	frustum[1][0] /= t;
	frustum[1][1] /= t;
	frustum[1][2] /= t;
	frustum[1][3] /= t;

	/* Extrait le plan du BAS */
	frustum[2][0] = clip[ 3] + clip[ 1];
	frustum[2][1] = clip[ 7] + clip[ 5];
	frustum[2][2] = clip[11] + clip[ 9];
	frustum[2][3] = clip[15] + clip[13];
	t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
	frustum[2][0] /= t;
	frustum[2][1] /= t;
	frustum[2][2] /= t;
	frustum[2][3] /= t;

	/* Extrait le plan du HAUT */
	frustum[3][0] = clip[ 3] - clip[ 1];
	frustum[3][1] = clip[ 7] - clip[ 5];
	frustum[3][2] = clip[11] - clip[ 9];
	frustum[3][3] = clip[15] - clip[13];
	t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
	frustum[3][0] /= t;
	frustum[3][1] /= t;
	frustum[3][2] /= t;
	frustum[3][3] /= t;

	/* Extrait le plan ELOIGNE */
	frustum[4][0] = clip[ 3] - clip[ 2];
	frustum[4][1] = clip[ 7] - clip[ 6];
	frustum[4][2] = clip[11] - clip[10];
	frustum[4][3] = clip[15] - clip[14];
	t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
	frustum[4][0] /= t;
	frustum[4][1] /= t;
	frustum[4][2] /= t;
	frustum[4][3] /= t;

	/* Extrait le plan PROCHE */
	frustum[5][0] = clip[ 3] + clip[ 2];
	frustum[5][1] = clip[ 7] + clip[ 6];
	frustum[5][2] = clip[11] + clip[10];
	frustum[5][3] = clip[15] + clip[14];
	t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
	frustum[5][0] /= t;
	frustum[5][1] /= t;
	frustum[5][2] /= t;
	frustum[5][3] /= t;
}



bool Frustum::pointInFrustum(float *pos)
{
	float x = pos[0];
	float y = pos[1];
	float z = pos[2];

	for(int p=0;p<6;p++)
	{
		if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] <= 0 )
		{
			//le point est du mauvais coté d'un des plan donc n'est pas dans le frustum
			return false;
		}
	}

   //le point est du bon coté de tout les plan
   return true;

}

bool Frustum::sphereInFrustum(float *pos, float radius)
{
	float x = pos[0];
	float y = pos[1];
	float z = pos[2];
	int c = 0;	//le compteur
	/* 
	le nombre de fois qu on est dans le frustum : 
		c = 6 fois, la sphère est completement dans le frustum
		c = 0 fois, la sphère est completement en dehors du frustum
		sinon, sphère coupe le bord du frustum
	*/
	float d;	// la "distance orienté" du centre de la sphere au plan
	
	for(int p=0;p<6;p++)	// pour tous les plans
	{
		d = frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3];

		if( d <= -radius )		// 
		{
			// la boule est du mauvais coté du plan(signe négatif)
			//et elle est plus loin que son rayon donc elle est en dehors du frustum
			return false;
		}
		if( d > radius )
		{
			//la boule ne touche pas un plan et est du bon coté
			c++;
		}
	}
	return true;

}

bool Frustum::cubeInFrustum(float *pos, float sideSize)
{
	float x = pos[0];
	float y = pos[1];
	float z = pos[2];

	for(int p=0;p<6;p++)
	{
		if( frustum[p][0] * (x - sideSize) + frustum[p][1] * (y - sideSize) + frustum[p][2] * (z - sideSize) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x + sideSize) + frustum[p][1] * (y - sideSize) + frustum[p][2] * (z - sideSize) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x - sideSize) + frustum[p][1] * (y + sideSize) + frustum[p][2] * (z - sideSize) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x + sideSize) + frustum[p][1] * (y + sideSize) + frustum[p][2] * (z - sideSize) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x - sideSize) + frustum[p][1] * (y - sideSize) + frustum[p][2] * (z + sideSize) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x + sideSize) + frustum[p][1] * (y - sideSize) + frustum[p][2] * (z + sideSize) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x - sideSize) + frustum[p][1] * (y + sideSize) + frustum[p][2] * (z + sideSize) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x + sideSize) + frustum[p][1] * (y + sideSize) + frustum[p][2] * (z + sideSize) + frustum[p][3] > 0 )
			continue;

		//aucun des vertices ne sont du bon coté du plan p : le cube est en dehors du frustum
		return false;
	}

	//le cube est dans le frustum
	return true;

}

bool Frustum::paveInFrustum(float *pos, float sideSizex, float sideSizey, float sideSizez)
{
	float x = pos[0];
	float y = pos[1];
	float z = pos[2];

	for(int p=0;p<6;p++)
	{
		if( frustum[p][0] * (x - sideSizex) + frustum[p][1] * (y - sideSizey) + frustum[p][2] * (z - sideSizez) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x + sideSizex) + frustum[p][1] * (y - sideSizey) + frustum[p][2] * (z - sideSizez) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x - sideSizex) + frustum[p][1] * (y + sideSizey) + frustum[p][2] * (z - sideSizez) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x + sideSizex) + frustum[p][1] * (y + sideSizey) + frustum[p][2] * (z - sideSizez) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x - sideSizex) + frustum[p][1] * (y - sideSizey) + frustum[p][2] * (z + sideSizez) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x + sideSizex) + frustum[p][1] * (y - sideSizey) + frustum[p][2] * (z + sideSizez) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x - sideSizex) + frustum[p][1] * (y + sideSizey) + frustum[p][2] * (z + sideSizez) + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * (x + sideSizex) + frustum[p][1] * (y + sideSizey) + frustum[p][2] * (z + sideSizez) + frustum[p][3] > 0 )
			continue;

		//aucun des vertices ne sont du bon coté du plan p : le cube est en dehors du frustum
		return false;
	}

	//le cube est dans le frustum
	return true;

}

bool Frustum::rectangleInFrustum(float *v1,float *v2,float *v3,float *v4)
{
	for(int p=0;p<6;p++)
	{
		if( frustum[p][0] * v1[0] + frustum[p][1] * v1[1] + frustum[p][2] * v1[2] + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * v2[0] + frustum[p][1] * v2[1] + frustum[p][2] * v2[2] + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * v3[0] + frustum[p][1] * v3[1] + frustum[p][2] * v3[2] + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * v4[0] + frustum[p][1] * v4[1] + frustum[p][2] * v4[2] + frustum[p][3] > 0 )
			continue;

		//aucun des vertices ne sont du bon coté du plan p : le cube est en dehors du frustum
		return false;
	}

	//le cube est dans le frustum
	return true;
}

bool Frustum::triangleInFrustum(float *v1,float *v2,float *v3)
{
	for(int p=0;p<6;p++)
	{
		if( frustum[p][0] * v1[0] + frustum[p][1] * v1[1] + frustum[p][2] * v1[2] + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * v2[0] + frustum[p][1] * v2[1] + frustum[p][2] * v2[2] + frustum[p][3] > 0 )
			continue;
		if( frustum[p][0] * v3[0] + frustum[p][1] * v3[1] + frustum[p][2] * v3[2] + frustum[p][3] > 0 )
			continue;

		//aucun des vertices ne sont du bon coté du plan p : le cube est en dehors du frustum
		return false;
	}

	//le cube est dans le frustum
	return true;
}


void Frustum::getClippingVolume(ClippingVolume *cv)
{
	cv->clipPlanes[0][0] = frustum[0][0];
	cv->clipPlanes[0][1] = frustum[0][1];
	cv->clipPlanes[0][2] = frustum[0][2];
	cv->clipPlanes[0][3] = frustum[0][3];
	
	cv->clipPlanes[1][0] = frustum[1][0];
	cv->clipPlanes[1][1] = frustum[1][1];
	cv->clipPlanes[1][2] = frustum[1][2];
	cv->clipPlanes[1][3] = frustum[1][3];
	
	cv->clipPlanes[2][0] = frustum[2][0];
	cv->clipPlanes[2][1] = frustum[2][1];
	cv->clipPlanes[2][2] = frustum[2][2];
	cv->clipPlanes[2][3] = frustum[2][3];
	
	cv->clipPlanes[3][0] = frustum[3][0];
	cv->clipPlanes[3][1] = frustum[3][1];
	cv->clipPlanes[3][2] = frustum[3][2];
	cv->clipPlanes[3][3] = frustum[3][3];
	
	cv->clipPlanes[4][0] = frustum[4][0];
	cv->clipPlanes[4][1] = frustum[4][1];
	cv->clipPlanes[4][2] = frustum[4][2];
	cv->clipPlanes[4][3] = frustum[4][3];
	
	cv->clipPlanes[5][0] = frustum[5][0];
	cv->clipPlanes[5][1] = frustum[5][1];
	cv->clipPlanes[5][2] = frustum[5][2];
	cv->clipPlanes[5][3] = frustum[5][3];


	// CORRECTION D'UN BUG D'ORIENTATION DE NORMAL DE PLAN //
	// !!! PAS DU TOUT OPTIMISée !!! //
/*	cv->clipPlanes[0][0]=-cv->clipPlanes[0][0];
	cv->clipPlanes[0][1]=-cv->clipPlanes[0][1];
	cv->clipPlanes[0][2]=-cv->clipPlanes[0][2];

	cv->clipPlanes[3][0]=-cv->clipPlanes[3][0];
	cv->clipPlanes[3][1]=-cv->clipPlanes[3][1];
	cv->clipPlanes[3][2]=-cv->clipPlanes[3][2];
	
	cv->clipPlanes[1][0]=-cv->clipPlanes[1][0];
	cv->clipPlanes[1][1]=-cv->clipPlanes[1][1];
	cv->clipPlanes[1][2]=-cv->clipPlanes[1][2];
	
	cv->clipPlanes[2][0]=-cv->clipPlanes[2][0];
	cv->clipPlanes[2][1]=-cv->clipPlanes[2][1];
	cv->clipPlanes[2][2]=-cv->clipPlanes[2][2];*/
}


void Frustum::genCubicFrustum(float *pos, float sideSizex, float sideSizey, float sideSizez)
{
	//on positionne le frustum
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//négation de la position pour bonne translation
//	glOrtho( -(*pos)[0]-sideSizex, -(*pos)[0]+sideSizex, -(*pos)[1]-sideSizey, -(*pos)[1]+sideSizey, -(*pos)[2]-sideSizez, -(*pos)[2]+sideSizez);
	glOrtho( -pos[0]-sideSizex, -pos[0]+sideSizex, -pos[1]-sideSizey, -pos[1]+sideSizey, -pos[2]-sideSizez, -pos[2]+sideSizez);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//on calcul les nouvelles valeurs des plans
	this->update();

	//on restaure les matrices précedentes
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


void Frustum ::setPortalv(float camera[3],float portal[4][3])
{
	float vect1[3];
	float vect2[3];
	float normal[3];
	float norm;
	float d;

	// on calcule le vecteur reliant la camera et le point 0
	vect1[0]=camera[0]-portal[0][0];
	vect1[1]=camera[1]-portal[0][1];
	vect1[2]=camera[2]-portal[0][2];

	// on calcule le vecteur reliant la camera et le point 1
	vect2[0]=camera[0]-portal[1][0];
	vect2[1]=camera[1]-portal[1][1];
	vect2[2]=camera[2]-portal[1][2];

	// on calcule la normal au 2 vecteur precedents
	normal[0]=vect1[1]*vect2[2]-vect1[2]*vect2[1];
	normal[1]=vect1[2]*vect2[0]-vect1[0]*vect2[2];
	normal[2]=vect1[0]*vect2[1]-vect1[1]*vect2[0];

	// on calcule la norme du vecteur normal
	norm=sqrt(pow(normal[0],2)+pow(normal[1],2)+pow(normal[2],2));
	// et on normalize la normal
	normal[0]/=-norm;
	normal[1]/=-norm;
	normal[2]/=-norm;

	// on calcule la distance planaire (ax+by+cz+d=0)
	d=-(portal[0][0]*normal[0]+portal[0][1]*normal[1]+portal[0][2]*normal[2]);

	// et on met le tout dans le frustum de gauche
	frustum[1][0]=normal[0];
	frustum[1][1]=normal[1];
	frustum[1][2]=normal[2];
	frustum[1][3]=d;

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// on calcule le vecteur reliant la camera et le point 1
	vect1[0]=camera[0]-portal[1][0];
	vect1[1]=camera[1]-portal[1][1];
	vect1[2]=camera[2]-portal[1][2];

	// on calcule le vecteur reliant la camera et le point 2
	vect2[0]=camera[0]-portal[2][0];
	vect2[1]=camera[1]-portal[2][1];
	vect2[2]=camera[2]-portal[2][2];

	// on calcule la normal au 2 vecteur precedents
	normal[0]=vect1[1]*vect2[2]-vect1[2]*vect2[1];
	normal[1]=vect1[2]*vect2[0]-vect1[0]*vect2[2];
	normal[2]=vect1[0]*vect2[1]-vect1[1]*vect2[0];

	// on calcule la norme du vecteur normal
	norm=sqrt(pow(normal[0],2)+pow(normal[1],2)+pow(normal[2],2));
	// et on normalize la normal
	normal[0]/=-norm;
	normal[1]/=-norm;
	normal[2]/=-norm;

	// on calcule la distance planaire (ax+by+cz+d=0)
	d=-(portal[1][0]*normal[0]+portal[1][1]*normal[1]+portal[1][2]*normal[2]);

	// et on met le tout dans le frustum bas
	frustum[2][0]=normal[0];
	frustum[2][1]=normal[1];
	frustum[2][2]=normal[2];
	frustum[2][3]=d;

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// on calcule le vecteur reliant la camera et le point 2
	vect1[0]=camera[0]-portal[2][0];
	vect1[1]=camera[1]-portal[2][1];
	vect1[2]=camera[2]-portal[2][2];

	// on calcule le vecteur reliant la camera et le point 3
	vect2[0]=camera[0]-portal[3][0];
	vect2[1]=camera[1]-portal[3][1];
	vect2[2]=camera[2]-portal[3][2];

	// on calcule la normal au 2 vecteur precedents
	normal[0]=vect1[1]*vect2[2]-vect1[2]*vect2[1];
	normal[1]=vect1[2]*vect2[0]-vect1[0]*vect2[2];
	normal[2]=vect1[0]*vect2[1]-vect1[1]*vect2[0];

	// on calcule la norme du vecteur normal
	norm=sqrt(pow(normal[0],2)+pow(normal[1],2)+pow(normal[2],2));
	// et on normalize la normal
	normal[0]/=-norm;
	normal[1]/=-norm;
	normal[2]/=-norm;

	// on calcule la distance planaire (ax+by+cz+d=0)
	d=-(portal[2][0]*normal[0]+portal[2][1]*normal[1]+portal[2][2]*normal[2]);

	// et on met le tout dans le frustum de droite
	frustum[0][0]=normal[0];
	frustum[0][1]=normal[1];
	frustum[0][2]=normal[2];
	frustum[0][3]=d;

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// on calcule le vecteur reliant la camera et le point 2
	vect1[0]=camera[0]-portal[3][0];
	vect1[1]=camera[1]-portal[3][1];
	vect1[2]=camera[2]-portal[3][2];

	// on calcule le vecteur reliant la camera et le point 3
	vect2[0]=camera[0]-portal[0][0];
	vect2[1]=camera[1]-portal[0][1];
	vect2[2]=camera[2]-portal[0][2];

	// on calcule la normal au 2 vecteur precedents
	normal[0]=vect1[1]*vect2[2]-vect1[2]*vect2[1];
	normal[1]=vect1[2]*vect2[0]-vect1[0]*vect2[2];
	normal[2]=vect1[0]*vect2[1]-vect1[1]*vect2[0];

	// on calcule la norme du vecteur normal
	norm=sqrt(pow(normal[0],2)+pow(normal[1],2)+pow(normal[2],2));
	// et on normalize la normal
	normal[0]/=-norm;
	normal[1]/=-norm;
	normal[2]/=-norm;

	// on calcule la distance planaire (ax+by+cz+d=0)
	d=-(portal[3][0]*normal[0]+portal[3][1]*normal[1]+portal[3][2]*normal[2]);

	// et on met le tout dans le frustum du haut
	frustum[3][0]=normal[0];
	frustum[3][1]=normal[1];
	frustum[3][2]=normal[2];
	frustum[3][3]=d;

}


void Frustum::moveFrontClipPlane(float* p)
{
	float dotp = p[0]*frustum[5][0] + p[1]*frustum[5][1] + p[2]*frustum[5][2];
	float dist = -dotp-frustum[5][3];//distance du point au plan perpendiculairement
	frustum[5][3] += dist;//nouvelle équation de plan: seul 'd' change
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////



void BoundingBox::render()
{
	glBegin(GL_QUADS);
	glVertex3f(pos[0]+sizex, pos[1]+sizey, pos[2]+sizez);
	glVertex3f(pos[0]-sizex, pos[1]+sizey, pos[2]+sizez);
	glVertex3f(pos[0]-sizex, pos[1]-sizey, pos[2]+sizez);
	glVertex3f(pos[0]+sizex, pos[1]-sizey, pos[2]+sizez);
	glVertex3f(pos[0]+sizex, pos[1]+sizey, pos[2]-sizez);
	glVertex3f(pos[0]-sizex, pos[1]+sizey, pos[2]-sizez);
	glVertex3f(pos[0]-sizex, pos[1]-sizey, pos[2]-sizez);
	glVertex3f(pos[0]+sizex, pos[1]-sizey, pos[2]-sizez);
	glVertex3f(pos[0]+sizex, pos[1]+sizey, pos[2]+sizez);
	glVertex3f(pos[0]-sizex, pos[1]+sizey, pos[2]+sizez);
	glVertex3f(pos[0]-sizex, pos[1]+sizey, pos[2]-sizez);
	glVertex3f(pos[0]+sizex, pos[1]+sizey, pos[2]-sizez);
	glVertex3f(pos[0]+sizex, pos[1]-sizey, pos[2]+sizez);
	glVertex3f(pos[0]-sizex, pos[1]-sizey, pos[2]+sizez);
	glVertex3f(pos[0]-sizex, pos[1]-sizey, pos[2]-sizez);
	glVertex3f(pos[0]+sizex, pos[1]-sizey, pos[2]-sizez);
	glVertex3f(pos[0]+sizex, pos[1]+sizey, pos[2]+sizez);
	glVertex3f(pos[0]+sizex, pos[1]-sizey, pos[2]+sizez);
	glVertex3f(pos[0]+sizex, pos[1]-sizey, pos[2]-sizez);
	glVertex3f(pos[0]+sizex, pos[1]+sizey, pos[2]-sizez);
	glVertex3f(pos[0]-sizex, pos[1]+sizey, pos[2]+sizez);
	glVertex3f(pos[0]-sizex, pos[1]-sizey, pos[2]+sizez);
	glVertex3f(pos[0]-sizex, pos[1]-sizey, pos[2]-sizez);
	glVertex3f(pos[0]-sizex, pos[1]+sizey, pos[2]-sizez);
	glEnd();
}

void BoundingBox::computeLimit()
{
	minx = this->pos.x - this->sizex;
	miny = this->pos.y - this->sizey;
	minz = this->pos.z - this->sizez;
	maxx = this->pos.x + this->sizex;
	maxy = this->pos.y + this->sizey;
	maxz = this->pos.z + this->sizez;
}

void BoundingBox::computeLimitInverse()
{
	this->pos.x = (this->maxx+this->minx) / 2.0f;
	this->pos.y = (this->maxy+this->miny) / 2.0f;
	this->pos.z = (this->maxz+this->minz) / 2.0f;
	this->sizex = (this->maxx-this->minx) / 2.0f;
	this->sizey = (this->maxy-this->miny) / 2.0f;
	this->sizez = (this->maxz-this->minz) / 2.0f;
}

bool BoundingBox::collide(BoundingBox* aabb)
{
	if( ( (this->minx < aabb->maxx) && (this->maxx > aabb->minx) ) &&	//sur x
		( (this->miny < aabb->maxy) && (this->maxy > aabb->miny) ) &&	//sur y
		( (this->minz < aabb->maxz) && (this->maxz > aabb->minz) ) )	//sur z
	{
		return true;
	}
	return false;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////



void ClippingVolume::enableClipVolume()
{
	glEnable(GL_CLIP_PLANE0);
	glClipPlane( GL_CLIP_PLANE0, &clipPlanes[0][0] );
	glEnable(GL_CLIP_PLANE1);
	glClipPlane( GL_CLIP_PLANE1, &clipPlanes[1][0] );
	glEnable(GL_CLIP_PLANE2);
	glClipPlane( GL_CLIP_PLANE2, &clipPlanes[2][0] );
	glEnable(GL_CLIP_PLANE3);
	glClipPlane( GL_CLIP_PLANE3, &clipPlanes[3][0] );
	glEnable(GL_CLIP_PLANE4);
	glClipPlane( GL_CLIP_PLANE4, &clipPlanes[4][0] );
	glEnable(GL_CLIP_PLANE5);
	glClipPlane( GL_CLIP_PLANE5, &clipPlanes[5][0] );
}

void ClippingVolume::DisableClipVolume()
{
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	glDisable(GL_CLIP_PLANE4);
	glDisable(GL_CLIP_PLANE5);
}



