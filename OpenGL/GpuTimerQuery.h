
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

#ifndef OGLTOOLS_GPUTIMERQUERY
#define OGLTOOLS_GPUTIMERQUERY

#ifdef __linux__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <glew/glew.h>
	#include <glut/glut.h>
#endif


//#ifdef GL_TIME_ELAPSED_EXT

/*#ifndef GLuint64EXT
typedef unsigned long GLuint64EXT;
#endif*/

/**
 *	Implementation of a class encapsulating the OpenGL extension GL_EXT_timer_query
 * For more details, please refer to http://www.opengl.org/registry/specs/EXT/timer_query.txt
 */
class GpuTimerQuery
{
public:
	/**
	 *	Default constructor
	 * !!! Only create a GpuTimerQuery object once a OpenGL context exits !!!
	 */
	GpuTimerQuery();

	/**
	 * Default destructor
	 */
	~GpuTimerQuery();

	/**
	 *
	 */
	void beginQuery();
	/**
	 *
	 */
	void endQuery();

	/**
	 *
	 */
	bool isResultAvailable();
	/**
	 * @return: rendering time in nanoseconds
	 */
	GLuint64EXT getElapsedTime();
	/**
	*waititme in ms
	 * @return: rendering time in nanoseconds (millisecond=nanosecond/1000000)
	 */
	GLuint64EXT getElapsedTimeWaiting(unsigned int waitTime=5);

private:
	/**
	 *	copy constructor forbidden
	 */
	GpuTimerQuery(GpuTimerQuery&);

	/**
	 *
	 */
	GLuint				timeQueryObject;
	/**
	 * The last time request value
	 */
	GLuint64EXT			timeElapsed;
};

//#endif //GL_TIME_ELAPSED_EXT

#endif
