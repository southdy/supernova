
#ifndef Text_h
#define Text_h

#include "Mesh2D.h"

namespace Supernova {

    class STBText;

    class Text: public Mesh2D {

    private:
        
        STBText* stbtext;

        std::string font;
        std::string text;
        unsigned int fontSize;
        bool multiline;
        
        bool userDefinedWidth;
        bool userDefinedHeight;
        
    protected:
        void createText();

    public:
        Text();
        Text(const char* font);
        virtual ~Text();
        
        Text& operator = ( const char* v );
        bool operator == ( const char* v ) const;
        bool operator != ( const char* v ) const;
        
        virtual void setSize(int width, int height);
        virtual void setInvertTexture(bool invertTexture);
        
        void setWidth(int width);
        void setHeight(int height);

        void setMinBufferSize(unsigned int characters);

        float getAscent();
        float getDescent();
        float getLineGap();
        int getLineHeight();
        
        void setFont(const char* font);
        void setText(const char* text);
        void setFontSize(unsigned int fontSize);
        void setMultiline(bool multiline);

        virtual bool load();
    };
    
}

#endif /* Text_h */
