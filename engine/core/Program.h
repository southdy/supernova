#ifndef PROGRAM_H
#define PROGRAM_H

#define S_SHADER_POINTS 1
#define S_SHADER_MESH 2
#define S_SHADER_DEPTH_RTT 3

#define S_VERTEXATTRIBUTE_VERTICES 1
#define S_VERTEXATTRIBUTE_TEXTURECOORDS 2
#define S_VERTEXATTRIBUTE_NORMALS 3
#define S_VERTEXATTRIBUTE_POINTSIZES 4
#define S_VERTEXATTRIBUTE_POINTCOLORS 5
#define S_VERTEXATTRIBUTE_POINTROTATIONS 6
#define S_VERTEXATTRIBUTE_TEXTURERECTS 7

#define S_PROPERTY_MVPMATRIX 1
#define S_PROPERTY_MODELMATRIX 2
#define S_PROPERTY_NORMALMATRIX 3
#define S_PROPERTY_DEPTHVPMATRIX 4
#define S_PROPERTY_CAMERAPOS 5
#define S_PROPERTY_TEXTURERECT 6
#define S_PROPERTY_COLOR 7

#define S_PROPERTY_NUMSHADOWS2D 8

#define S_PROPERTY_AMBIENTLIGHT 9

#define S_PROPERTY_NUMPOINTLIGHT 10
#define S_PROPERTY_POINTLIGHT_POS 11
#define S_PROPERTY_POINTLIGHT_POWER 12
#define S_PROPERTY_POINTLIGHT_COLOR 13
#define S_PROPERTY_POINTLIGHT_SHADOWIDX 14

#define S_PROPERTY_NUMSPOTLIGHT 15
#define S_PROPERTY_SPOTLIGHT_POS 16
#define S_PROPERTY_SPOTLIGHT_POWER 17
#define S_PROPERTY_SPOTLIGHT_COLOR 18
#define S_PROPERTY_SPOTLIGHT_TARGET 19
#define S_PROPERTY_SPOTLIGHT_CUTOFF 20
#define S_PROPERTY_SPOTLIGHT_OUTERCUTOFF 21
#define S_PROPERTY_SPOTLIGHT_SHADOWIDX 22

#define S_PROPERTY_NUMDIRLIGHT 23
#define S_PROPERTY_DIRLIGHT_DIR 24
#define S_PROPERTY_DIRLIGHT_POWER 25
#define S_PROPERTY_DIRLIGHT_COLOR 26
#define S_PROPERTY_DIRLIGHT_SHADOWIDX 27

#define S_PROPERTY_FOG_MODE 28
#define S_PROPERTY_FOG_COLOR 29
#define S_PROPERTY_FOG_VISIBILITY 30
#define S_PROPERTY_FOG_DENSITY 31
#define S_PROPERTY_FOG_START 32
#define S_PROPERTY_FOG_END 33

#define S_PROPERTY_SHADOWLIGHT_POS 34
#define S_PROPERTY_SHADOWCAMERA_NEARFAR 35

#define S_PROPERTY_ISPOINTSHADOW 36

#define S_PROPERTY_SHADOWBIAS2D 37
#define S_PROPERTY_SHADOWBIASCUBE 38
#define S_PROPERTY_SHADOWCAMERA_NEARFAR2D 39
#define S_PROPERTY_SHADOWCAMERA_NEARFARCUBE 40

#define S_PROPERTY_NUMCASCADES2D 41

#include "render/ProgramRender.h"
#include <string>
#include <vector>

namespace Supernova{

    class Program{
    private:
        std::shared_ptr<ProgramRender> programRender;
        
        int shaderType;
        int numLights;
        int numShadows2D;
        int numShadowsCube;
        bool hasFog;
        bool hasTextureCoords;
        bool hasTextureRect;
        bool hasTextureCube;
        bool isSky;
        bool isText;
        
        std::vector<int> shaderVertexAttributes;
        std::vector<int> shaderProperties;

    public:
        Program();

        void setShader(int shaderType);
        void setDefinitions(int numLights = 0, int numShadows2D = 0, int numShadowsCube = 0, bool hasFog = false,
                            bool hasTexture = false, bool hasTextureRect = false, bool hasTextureCube = false,
                            bool isSky = false, bool isText = false);
        
        bool existVertexAttribute(int vertexAttribute);
        bool existProperty(int property);
        
        bool load();
        void destroy();
        
        Program(const Program& p);
        virtual ~Program();

        Program& operator = (const Program& p);

        std::shared_ptr<ProgramRender> getProgramRender();

    };

}


#endif //PROGRAM_H
