
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

#include "GpuTimerQuery.h"

#if defined(WIN32)
#include <Windows.h>
#elif defined(LINUX)
#include <unistd.h>
#endif

#ifdef GL_TIME_ELAPSED_EXT

//#define GL_TIME_ELAPSED_EXT 0x88BF
//#define GL_TIMESTAMP        0x8E28




GpuTimerQuery::GpuTimerQuery()
{
	glGenQueries(1, &this->timeQueryObject);
}

GpuTimerQuery::~GpuTimerQuery()
{
	glDeleteQueries(1, &this->timeQueryObject);
}

GpuTimerQuery::GpuTimerQuery(GpuTimerQuery&)
{
}


void GpuTimerQuery::beginQuery()
{
	glBeginQuery(GL_TIME_ELAPSED_EXT, this->timeQueryObject);
}

void GpuTimerQuery::endQuery()
{
	glEndQuery(GL_TIME_ELAPSED_EXT);
}

bool GpuTimerQuery::isResultAvailable()
{
	GLint available = GL_FALSE;
	glGetQueryObjectiv(this->timeQueryObject, GL_QUERY_RESULT_AVAILABLE, &available);
	return (available==GL_TRUE);
}

GLuint64EXT GpuTimerQuery::getElapsedTime()
{
	//glGetQueryObjectui64vEXT(this->timeQueryObject, GL_QUERY_RESULT, &this->timeElapsed);
    
    this->timeElapsed = 0;
	return this->timeElapsed;
}

GLuint64EXT GpuTimerQuery::getElapsedTimeWaiting(unsigned int waitTime)
{
	GLint available = GL_FALSE;
	do
	{	//wait until result is available
		glGetQueryObjectiv(this->timeQueryObject, GL_QUERY_RESULT_AVAILABLE, &available);
		if(available!=GL_TRUE)
			break;
#if defined(WIN32)
		Sleep((DWORD)waitTime);
#elif defined(LINUX)
		usleep((unsigned long)ms*1000);
#endif
	}
	while(true);
	//get and return result
	return this->getElapsedTime();
}

#endif//GL_TIME_ELAPSED_EXT

