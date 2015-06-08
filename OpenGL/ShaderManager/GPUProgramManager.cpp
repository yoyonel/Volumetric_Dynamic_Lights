
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

#include "GPUProgramManager.h"

#include <string>
#include <fstream>


GPUProgramManager* GPUProgramManager::gpuProgramManager = NULL;


GPUProgramManager::GPUProgramManager()
{
    glslHeader = "#version 120\n";
}

GPUProgramManager::~GPUProgramManager()
{
}

GPUProgramManager::GPUProgramManager(GPUProgramManager&)
{
	//not used
}

GPUProgramManager& GPUProgramManager::getInstance()
{
	if(gpuProgramManager==NULL)
	{
		//instance creation
		gpuProgramManager = new GPUProgramManager();
	}
	return *gpuProgramManager;
}

void GPUProgramManager::destroy()
{
	if(gpuProgramManager!=NULL)
	{
		//unload and delete all gpu program
		std::map<std::string, GPUProgram*>::iterator it;
		for(it = gpuProgramManager->gpuPrograms.begin(); it!=gpuProgramManager->gpuPrograms.end(); ++it)
		{
			it->second->unload();
			delete it->second;
		}

		//delete instance
		delete gpuProgramManager;
	}
	gpuProgramManager = NULL;
}


bool GPUProgramManager::loadGPUProgram(const char* name, const char* vertexProgram, const char* fragmentProgram, const char* geometryProgram, int GS_inputPrimitiveType, int GS_outputPrimitiveType, int GS_maxVerticesOut)
{
	bool ret = false;
	//load the gpu program using vertex and fragment shader source
	GPUProgram* gpup = new GPUProgram();
	if(gpup->load(vertexProgram,fragmentProgram,geometryProgram,GS_inputPrimitiveType,GS_outputPrimitiveType,GS_maxVerticesOut))
	{
		this->gpuPrograms[name] = gpup;
		ret = true;
	}
	else
		delete gpup;
	return ret;
}


bool GPUProgramManager::loadGPUProgramFromDisk(const char* name, const char* vertexProgramFilepath, const char* fragmentProgramFilepath, const char* geometryProgramFilepath, int GS_inputPrimitiveType, int GS_outputPrimitiveType, int GS_maxVerticesOut)
{
	int length;
	char* buffer;
	std::string vpSource("");
	std::string fpSource("");
	std::string gpSource("");
	std::string headers("");
	std::string defines("");

	//test if specified files exist on the disk
	std::ifstream fileVP(vertexProgramFilepath, std::ios::binary);
	if((fileVP.rdstate() & std::ifstream::failbit ) != 0)
		return false;
	std::ifstream fileFP(fragmentProgramFilepath, std::ios::binary);
	if((fileFP.rdstate() & std::ifstream::failbit ) != 0)
	{
		fileVP.close();
		return false;
	}
	if(geometryProgramFilepath!=NULL)
	{
	    std::ifstream fileGP(geometryProgramFilepath, std::ios::binary);
	    if((fileGP.rdstate() & std::ifstream::failbit ) != 0)
	    {
			fileFP.close();
			fileVP.close();
			return false;
	    }
		

		//get length of file content
		fileGP.seekg (0, std::ios::end);
		length = int(fileGP.tellg());
		fileGP.seekg (0, std::ios::beg);
		// allocate memory:
		buffer = new char [length];
		// read data as a block
		fileGP.read (buffer,length);
		gpSource.assign(buffer,length);
		delete [] buffer;
	    fileGP.close();
	}

	//get length of file content
	fileVP.seekg (0, std::ios::end);
	length = int(fileVP.tellg());
	fileVP.seekg (0, std::ios::beg);
	// allocate memory:
	buffer = new char [length];
	// read data as a block
	fileVP.read (buffer,length);
	vpSource.assign(buffer,length);
	delete [] buffer;

	//get length of file content
	fileFP.seekg (0, std::ios::end);
	length = int(fileFP.tellg());
	fileFP.seekg (0, std::ios::beg);
	// allocate memory:
	buffer = new char [length];
	// read data as a block
	fileFP.read (buffer,length);
	fpSource.assign(buffer,length);
	delete [] buffer;
    
    // Load headers (this should really only be done once...)
    for(int i=0; i<glslIncludedHeaders.size();++i)
    {
        std::string path = glslIncludedHeaders.at(i);
        
        std::ifstream fileGLSL(path.c_str(), std::ios::binary);
        if((fileGLSL.rdstate() & std::ifstream::failbit ) != 0)
        {
            printf("Header glsl not found: %s.\n", path.c_str());
            continue;
        }
        
        //get length of file content
        fileGLSL.seekg (0, std::ios::end);
        length = int(fileGLSL.tellg());
        fileGLSL.seekg (0, std::ios::beg);
        // allocate memory:
        buffer = new char [length];
        // read data as a block
        fileGLSL.read (buffer,length);
        std::string bufferStr;
        bufferStr.assign(buffer,length);
        headers += bufferStr;
        delete [] buffer;
        
        //printf("*****************************\n%s#******************************",buffer);
    }
    
    
    // Load headers (this should really only be done once...)
    for(int i=0; i<glslDefines.size();++i)
    {
        std::string def = glslDefines.at(i);
        defines += "#define " + def + "\n";
    }
    
    std::string allHeaders = glslHeader + defines + headers;
    
    vpSource = allHeaders + vpSource;
    gpSource = allHeaders + gpSource;
    fpSource = allHeaders + fpSource;

	//load gpu program
    bool compilationSuccess = true;
	if(geometryProgramFilepath!=NULL)
    {
	    compilationSuccess = this->loadGPUProgram( name, vpSource.c_str(), fpSource.c_str(), gpSource.c_str(), GS_inputPrimitiveType, GS_outputPrimitiveType, GS_maxVerticesOut );
    }
	else
    {
	    compilationSuccess = this->loadGPUProgram( name, vpSource.c_str(), fpSource.c_str());
    }
    if(!compilationSuccess)
    {
        printf("--- ERROR building\n\n\n%s\n\n\n%s",vpSource.c_str(), fpSource.c_str());
    }

	//closing file
	fileVP.close();
	fileFP.close();

	return true;
}


const GPUProgram* GPUProgramManager::getGPUProgram(const char* name) const
{
	GPUProgram* ret = NULL;
	//search for the program
	std::map<std::string, GPUProgram*>::const_iterator it = this->gpuPrograms.find(name);
	if(it!=this->gpuPrograms.end())
		ret = it->second;
	else
		ret = NULL;

	return ret;
}

void GPUProgramManager::setGlslHeaderString(const char* _glslHeader)
{
    glslHeader = _glslHeader;
}

void GPUProgramManager::setIncludedHeaderFiles(std::vector<std::string>& _glslIncludedHeaders)
{
    glslIncludedHeaders = _glslIncludedHeaders;
}

void GPUProgramManager::setDefines(std::vector<std::string>& _defines)
{
    glslDefines = _defines;
}







