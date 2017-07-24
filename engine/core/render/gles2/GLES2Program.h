
#ifndef GLES2Program_h
#define GLES2Program_h

#include "GLES2Header.h"
#include "render/ProgramRender.h"
#include <string>

namespace Supernova {

    class GLES2Program : public ProgramRender{

    private:
        
        GLuint program;
        
        std::string getVertexShader(std::string name);
        std::string getFragmentShader(std::string name);
        
        std::string getVertexShader(int shaderType);
        std::string getFragmentShader(int shaderType);
        
        GLuint loadShader(GLenum shaderType, const char* pSource);
        
        
    public:

        virtual void createProgram(std::string shaderName, std::string definitions);
        virtual void createProgram(int shaderType, bool hasLight, bool hasFog, bool hasTextureRect);
        virtual void deleteProgram();
        
        GLuint getProgram();
    };
    
}

#endif /* GLES2Program_h */
