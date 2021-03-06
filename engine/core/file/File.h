
#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <string>

namespace Supernova {

    class File {
    public:
        virtual ~File();

        static File* newFile(bool useHandle = false);
        static File* newFile(const char *aFilename, bool useHandle = false);

        static char getDirSeparator();

        unsigned int read8();
        unsigned int read16();
        unsigned int read32();
        std::string readString();
        virtual int eof() = 0;
        virtual unsigned int read(unsigned char *aDst, unsigned int aBytes) = 0;
        virtual unsigned int length() = 0;
        virtual void seek(int aOffset) = 0;
        virtual unsigned int pos() = 0;
        virtual unsigned int open(const char *aFilename) = 0;
    };
    
}


#endif
