#ifndef ProgramRender_h
#define ProgramRender_h

#include <string>
#include <unordered_map>
#include <regex>

namespace Supernova {

    class ProgramRender {
        
    private:
        
        typedef std::unordered_map< std::string, std::shared_ptr<ProgramRender> >::iterator it_type;
        static std::unordered_map< std::string, std::shared_ptr<ProgramRender> > programsRender;
        
        bool loaded;
        
        static ProgramRender* getProgramRender();
        static ProgramRender::it_type findToRemove();
        
    protected:

        int maxLights;
        int maxShadows2D;
        int maxShadowsCube;


        std::string regexReplace(std::string_view haystack, const std::regex& rx, std::function<std::string(const std::cmatch&)> f);
        std::string replaceAll(std::string source, const std::string from, const std::string to);
        std::string unrollLoops(std::string source);
        
        ProgramRender();
        
    public:
        
        virtual ~ProgramRender();
        
        static std::shared_ptr<ProgramRender> sharedInstance(std::string id);
        static void deleteUnused();
        
        bool isLoaded();

        int getMaxLights();
        int getMaxShadows2D();
        int getMaxShadowsCube();

        virtual void createProgram(int shaderType, int numLights, int numShadows2D, int numShadowsCube, bool hasFog, bool hasTextureCoords, bool hasTextureRect, bool hasTextureCube, bool isSky, bool isText);
        virtual void deleteProgram();
        
    };
    
}

#endif /* ProgramRender_h */
