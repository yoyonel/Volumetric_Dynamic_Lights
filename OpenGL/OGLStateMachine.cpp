
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

#include "OGLStateMachine.h"

#ifdef __linux__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <glew/glew.h>
	#include <glut/glut.h>
#endif


static float referenceAlpha = 0.8f;

unsigned long oglState = GLSM_STATE_DEFAULT;


void glsm_referenceAlpha(float refAlpha)
{
	referenceAlpha = refAlpha;
}



void glsm_init()
{
	oglState = GLSM_STATE_DEFAULT;
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}


void glsm_state(unsigned long newState)
{
	unsigned long diff = newState ^ oglState;	//XOR
	if ( !diff )	return;

	if(diff & GLSM_DEPTHMASK_BITS)
	{
		if ((newState&GLSM_DEPTHMASK_BITS) == GLSM_DEPTHMASK_TRUE)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}
	}
	
	if(diff & GLSM_POLYMODE_BITS)
	{
		switch(newState&GLSM_POLYMODE_BITS)
		{
		case GLSM_POLYMODE_FILL_FB:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case GLSM_POLYMODE_LINE_FB:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case GLSM_POLYMODE_POINT_FB:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		default:
			//log_printf("glsm_state : bad POLYGON MODE!",LOG_DEVELOPER|LOG_WARNING);
			break;
		}
	}
	
	if(diff & GLSM_DEPTHFUNC_BITS)
	{
		if((newState&GLSM_DEPTHFUNC_BITS) == GLSM_DEPTHFUNC_DISABLED) 
			glDisable(GL_DEPTH_TEST);
		else
		{
			if((oglState&GLSM_DEPTHFUNC_BITS) == GLSM_DEPTHFUNC_DISABLED) 
				glEnable(GL_DEPTH_TEST);

			switch(newState&GLSM_DEPTHFUNC_BITS)
			{
			case GLSM_DEPTHFUNC_LESS:
				glDepthFunc(GL_LESS);
				break;
			case GLSM_DEPTHFUNC_LEQUAL:
				glDepthFunc(GL_LEQUAL);
				break;
			case GLSM_DEPTHFUNC_EQUAL:
				glDepthFunc(GL_EQUAL);
				break;
			case GLSM_DEPTHFUNC_GEQUAL:
				glDepthFunc(GL_GEQUAL);
				break;
			case GLSM_DEPTHFUNC_GREATER:
				glDepthFunc(GL_GREATER);
				break;
			case GLSM_DEPTHFUNC_NOTEQUAL:
				glDepthFunc(GL_NOTEQUAL);
				break;
			case GLSM_DEPTHFUNC_NEVER:
				glDepthFunc(GL_NEVER);
				break;
			case GLSM_DEPTHFUNC_ALWAYS:
				glDepthFunc(GL_ALWAYS);
				break;
			default:
				//log_printf("glsm_state : bad DEPTH FUNC!",LOG_DEVELOPER|LOG_WARNING);
				break;
			}
		}
	}
	
	if(diff & GLSM_ALPHAFUNC_BITS)
	{
		if((newState&GLSM_ALPHAFUNC_BITS) == GLSM_ALPHAFUNC_DISABLED) 
			glDisable(GL_ALPHA_TEST);
		else
		{
			if((oglState&GLSM_ALPHAFUNC_BITS) == GLSM_ALPHAFUNC_DISABLED) 
				glEnable(GL_ALPHA_TEST);

			switch(newState&GLSM_ALPHAFUNC_BITS)
			{
			case GLSM_ALPHAFUNC_LESS:
				glAlphaFunc(GL_LESS,referenceAlpha);
				break;
			case GLSM_ALPHAFUNC_LEQUAL:
				glAlphaFunc(GL_LEQUAL,referenceAlpha);
				break;
			case GLSM_ALPHAFUNC_EQUAL:
				glAlphaFunc(GL_EQUAL,referenceAlpha);
				break;
			case GLSM_ALPHAFUNC_GEQUAL:
				glAlphaFunc(GL_GEQUAL,referenceAlpha);
				break;
			case GLSM_ALPHAFUNC_GREATER:
				glAlphaFunc(GL_GREATER,referenceAlpha);
				break;
			case GLSM_ALPHAFUNC_NOTEQUAL:
				glAlphaFunc(GL_NOTEQUAL,referenceAlpha);
				break;
			case GLSM_ALPHAFUNC_NEVER:
				glAlphaFunc(GL_NEVER,referenceAlpha);
				break;
			case GLSM_ALPHAFUNC_ALWAYS:
				glAlphaFunc(GL_ALWAYS,referenceAlpha);
				break;
			default:
				//log_printf("glsm_state : bad ALPHA FUNC!",LOG_DEVELOPER|LOG_WARNING);
				break;
			}
		}
	}
	
	if(diff & (GLSM_SRCBLEND_BITS|GLSM_DSTBLEND_BITS))
	{
		if( ((newState&GLSM_SRCBLEND_BITS) == GLSM_SRCBLEND_DISABLED) || ((newState&GLSM_SRCBLEND_BITS) == GLSM_DSTBLEND_DISABLED))
		{
			glDisable(GL_BLEND);
		}
		else
		{
			GLenum sfactor=GL_ONE, dfactor=GL_ONE;	//default value to not have OpenGL error

			if( ((oglState&GLSM_SRCBLEND_BITS) == GLSM_SRCBLEND_DISABLED) || ((oglState&GLSM_SRCBLEND_BITS) == GLSM_DSTBLEND_DISABLED))
				glEnable(GL_BLEND);

			switch(newState&GLSM_SRCBLEND_BITS)
			{
			case GLSM_SRCBLEND_ZERO:
				sfactor = GL_ZERO;
				break;
			case GLSM_SRCBLEND_ONE:
				sfactor = GL_ONE;
				break;
			case GLSM_SRCBLEND_DST_COLOR:
				sfactor = GL_DST_COLOR;
				break;
			case GLSM_SRCBLEND_ONE_MINUS_DST_COLOR:
				sfactor = GL_ONE_MINUS_DST_COLOR;
				break;
			case GLSM_SRCBLEND_SRC_COLOR:
				sfactor = GL_SRC_COLOR;
				break;
			case GLSM_SRCBLEND_ONE_MINUS_SRC_COLOR:
				sfactor = GL_ONE_MINUS_SRC_COLOR;
				break;
			case GLSM_SRCBLEND_SRC_ALPHA:
				sfactor = GL_SRC_ALPHA;
				break;
			case GLSM_SRCBLEND_ONE_MINUS_SRC_ALPHA:
				sfactor = GL_ONE_MINUS_SRC_ALPHA;
				break;
			case GLSM_SRCBLEND_DST_ALPHA:
				sfactor = GL_DST_ALPHA;
				break;
			case GLSM_SRCBLEND_ONE_MINUS_DST_ALPHA:
				sfactor = GL_ONE_MINUS_DST_ALPHA;
				break;
			default:
				//log_printf("glsm_state : bad SRC BLEND MODE!",LOG_DEVELOPER|LOG_WARNING);
				break;
			}

			switch(newState&GLSM_DSTBLEND_BITS)
			{
			case GLSM_DSTBLEND_ZERO:
				dfactor = GL_ZERO;
				break;
			case GLSM_DSTBLEND_ONE:
				dfactor = GL_ONE;
				break;
			case GLSM_DSTBLEND_DST_COLOR:
				dfactor = GL_DST_COLOR;
				break;
			case GLSM_DSTBLEND_ONE_MINUS_DST_COLOR:
				dfactor = GL_ONE_MINUS_DST_COLOR;
				break;
			case GLSM_DSTBLEND_SRC_COLOR:
				dfactor = GL_SRC_COLOR;
				break;
			case GLSM_DSTBLEND_ONE_MINUS_SRC_COLOR:
				dfactor = GL_ONE_MINUS_SRC_COLOR;
				break;
			case GLSM_DSTBLEND_SRC_ALPHA:
				dfactor = GL_SRC_ALPHA;
				break;
			case GLSM_DSTBLEND_ONE_MINUS_SRC_ALPHA:
				dfactor = GL_ONE_MINUS_SRC_ALPHA;
				break;
			case GLSM_DSTBLEND_DST_ALPHA:
				dfactor = GL_DST_ALPHA;
				break;
			case GLSM_DSTBLEND_ONE_MINUS_DST_ALPHA:
				dfactor = GL_ONE_MINUS_DST_ALPHA;
				break;
			default:
				//log_printf("glsm_state : bad SRC BLEND MODE!",LOG_DEVELOPER|LOG_WARNING);
				break;
			}

			glBlendFunc(sfactor,dfactor);
		}
	}

	oglState = newState;
}


