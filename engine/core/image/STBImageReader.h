#ifndef STBImageReader_h
#define STBImageReader_h

#include "ImageReader.h"

namespace Supernova {

    class STBImageReader: public ImageReader{
        
    public:
        
        TextureData* getRawImage(FileData* filedata);
        
    };
    
}

#endif /* STBImageReader_h */
