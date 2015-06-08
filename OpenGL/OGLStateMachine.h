
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

#ifndef OGLTOOLS_OGLSTATEMACHINE
#define OGLTOOLS_OGLSTATEMACHINE


//
//inspired from Quake III Arena source code
//
//OpenGL state machine often used features
//
// IDEA : stencil, other mask, culling, etc.
//
#define GLSM_SRCBLEND_DISABLED					0x00000000
#define GLSM_SRCBLEND_ZERO						0x00000001
#define GLSM_SRCBLEND_ONE						0x00000002
#define GLSM_SRCBLEND_DST_COLOR					0x00000003
#define GLSM_SRCBLEND_ONE_MINUS_DST_COLOR		0x00000004
#define GLSM_SRCBLEND_SRC_COLOR					0x00000005
#define GLSM_SRCBLEND_ONE_MINUS_SRC_COLOR		0x00000006
#define GLSM_SRCBLEND_SRC_ALPHA					0x00000007
#define GLSM_SRCBLEND_ONE_MINUS_SRC_ALPHA		0x00000008
#define GLSM_SRCBLEND_DST_ALPHA					0x00000009
#define GLSM_SRCBLEND_ONE_MINUS_DST_ALPHA		0x0000000a
#define	GLSM_SRCBLEND_BITS						0x0000000f

#define GLSM_DSTBLEND_DISABLED					0x00000000
#define GLSM_DSTBLEND_ZERO						0x00000010
#define GLSM_DSTBLEND_ONE						0x00000020
#define GLSM_DSTBLEND_DST_COLOR					0x00000030
#define GLSM_DSTBLEND_ONE_MINUS_DST_COLOR		0x00000040
#define GLSM_DSTBLEND_SRC_COLOR					0x00000050
#define GLSM_DSTBLEND_ONE_MINUS_SRC_COLOR		0x00000060
#define GLSM_DSTBLEND_SRC_ALPHA					0x00000070
#define GLSM_DSTBLEND_ONE_MINUS_SRC_ALPHA		0x00000080
#define GLSM_DSTBLEND_DST_ALPHA					0x00000090
#define GLSM_DSTBLEND_ONE_MINUS_DST_ALPHA		0x000000a0
#define	GLSM_DSTBLEND_BITS						0x000000f0

#define GLSM_DEPTHMASK_TRUE						0x00000000
#define GLSM_DEPTHMASK_FALSE					0x00000100
#define GLSM_DEPTHMASK_BITS						0x00000f00

#define GLSM_POLYMODE_FILL_FB					0x00000000
#define GLSM_POLYMODE_LINE_FB					0x00001000
#define GLSM_POLYMODE_POINT_FB					0x00002000
#define GLSM_POLYMODE_BITS						0x0000f000

#define GLSM_DEPTHFUNC_DISABLED					0x00000000
#define GLSM_DEPTHFUNC_LESS						0x00010000
#define GLSM_DEPTHFUNC_LEQUAL					0x00020000
#define GLSM_DEPTHFUNC_EQUAL					0x00030000
#define GLSM_DEPTHFUNC_GEQUAL					0x00040000
#define GLSM_DEPTHFUNC_GREATER					0x00050000
#define GLSM_DEPTHFUNC_NOTEQUAL					0x00060000
#define GLSM_DEPTHFUNC_NEVER					0x00070000
#define GLSM_DEPTHFUNC_ALWAYS					0x00080000
#define GLSM_DEPTHFUNC_BITS						0x000f0000

#define GLSM_ALPHAFUNC_DISABLED					0x00000000
#define GLSM_ALPHAFUNC_GREATER					0x00100000
#define GLSM_ALPHAFUNC_GEQUAL					0x00200000
#define GLSM_ALPHAFUNC_EQUAL					0x00300000
#define GLSM_ALPHAFUNC_LEQUAL					0x00400000
#define GLSM_ALPHAFUNC_LESS						0x00500000
#define GLSM_ALPHAFUNC_NOTEQUAL					0x00600000
#define GLSM_ALPHAFUNC_ALWAYS					0x00700000
#define GLSM_ALPHAFUNC_NEVER					0x00800000
#define GLSM_ALPHAFUNC_BITS						0x00f00000


//
//	States
//

#define GLSM_STATE_DEFAULT						GLSM_DEPTHFUNC_LESS

#define GLSM_STATE_DEPTH_PASS					GLSM_STATE_DEFAULT
#define GLSM_STATE_ADDITIVE_PASS				GLSM_SRCBLEND_ONE|GLSM_DSTBLEND_ONE
#define GLSM_STATE_LIGHT_PASS					GLSM_STATE_ADDITIVEPASS|GLSM_DEPTHMASK_FALSE|GLSM_DEPTHTEST_LEQUAL

#define GLSM_STATE_DECALS						GLSM_DEPTHMASK_FALSE|GLSM_DEPTHFUNC_LEQUAL|GLSM_ALPHAFUNC_GREATER|GLSM_SRCBLEND_SRC_ALPHA|GLSM_SRCBLEND_ONE_MINUS_SRC_ALPHA



/**
 *	Initialize the state machine OpenGL
 *
 * By default :
 *	- no blend, no alpha test
 *	- depth mask = GL_TRUE, other mask are GL_TRUE
 *	- front and back face are filled
 *	- depth test is GL_LESS
 *	- no stencil op & func
 *	- 
 */
void glsm_init();


/**
 *	Set the alpha reference value for alpha test comparison
 *
 * @param refAlpha : alpha reference value
 */
void glsm_referenceAlpha(float refAlpha);


/**
 *	Set the current OpenGL state
 *
 * @param state : the desired state (non specified feature value will be set by default)
 */
void glsm_state(unsigned long newState);


#endif
