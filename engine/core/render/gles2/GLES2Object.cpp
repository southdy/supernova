#include "GLES2Object.h"
#include "GLES2Util.h"
#include "GLES2Program.h"
#include "GLES2Texture.h"
#include "Engine.h"
#include "platform/Log.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define BUFFER_OFFSET(i) ((void*)(i))



using namespace Supernova;


GLES2Object::GLES2Object(): ObjectRender(){

    usageBuffer = GL_STATIC_DRAW;
    
}

GLES2Object::~GLES2Object(){

}

void GLES2Object::loadVertexAttribute(int type, attributeData att){
    
    attributeGlData vp = attributesGL[type];

    if (vp.size == 0){
        vp.buffer = GLES2Util::createVBO();
    }
    if (vp.size >= att.size){
        GLES2Util::updateVBO(vp.buffer, GL_ARRAY_BUFFER, att.size * att.elements * sizeof(GLfloat), att.data);
    }else{
        vp.size = std::max((unsigned int)att.size, minBufferSize);
        GLES2Util::dataVBO(vp.buffer, GL_ARRAY_BUFFER, vp.size * att.elements * sizeof(GLfloat), att.data, usageBuffer);
    }

    attributesGL[type] = vp;
}

void GLES2Object::loadIndex(indexData att){
    
    indexGlData ib = indexGL;
    
    if (ib.size == 0){
        ib.buffer = GLES2Util::createVBO();
    }
    if (ib.size >= att.size){
        GLES2Util::updateVBO(ib.buffer,GL_ELEMENT_ARRAY_BUFFER, att.size * sizeof(unsigned int), att.data);
    }else{
        ib.size = std::max((unsigned int)att.size, minBufferSize);
        GLES2Util::dataVBO(ib.buffer, GL_ELEMENT_ARRAY_BUFFER, ib.size * sizeof(unsigned int), att.data, usageBuffer);
    }

    indexGL = ib;
}

void GLES2Object::updateVertexAttribute(int type, unsigned long size, void* data){
    ObjectRender::updateVertexAttribute(type, size, data);
    if (vertexAttributes.count(type))
        loadVertexAttribute(type, vertexAttributes[type]);
}

void GLES2Object::updateIndex(unsigned long size, void* data){
    ObjectRender::updateIndex(size, data);
    if (indexAttribute.data)
        loadIndex(indexAttribute);
}

bool GLES2Object::load(){
    if (!ObjectRender::load()){
        return false;
    }
    
    //Log::Debug(LOG_TAG, "Start load object");
    attributesGL.clear();
    propertyGL.clear();
    
    if (dynamicBuffer)
        usageBuffer = GL_DYNAMIC_DRAW;
    
    GLuint glesProgram = ((GLES2Program*)program->getProgramRender().get())->getProgram();
    
    useTexture = glGetUniformLocation(glesProgram, "uUseTexture");
    
    for (std::unordered_map<int, attributeData>::iterator it = vertexAttributes.begin(); it != vertexAttributes.end(); ++it)
    {
        int type = it->first;
        
        std::string attribName;
        
        if (type == S_VERTEXATTRIBUTE_VERTICES){
            attribName = "a_Position";
        }else if (type == S_VERTEXATTRIBUTE_TEXTURECOORDS){
            attribName = "a_TextureCoordinates";
        }else if (type == S_VERTEXATTRIBUTE_NORMALS){
            attribName = "a_Normal";
        }else if (type == S_VERTEXATTRIBUTE_POINTSIZES){
            attribName = "a_pointSize";
        }else if (type == S_VERTEXATTRIBUTE_POINTCOLORS){
            attribName = "a_pointColor";
        }else if (type == S_VERTEXATTRIBUTE_TEXTURERECTS){
            attribName = "a_textureRect";            
        }
        
        loadVertexAttribute(type, it->second);
        attributesGL[type].handle = glGetAttribLocation(glesProgram, attribName.c_str());
        //Log::Debug(LOG_TAG, "Load attribute buffer: %s, size: %lu, handle %i", attribName.c_str(), it->second.size, attributesGL[type].handle);
    }
    
    if (indexAttribute.data){
        loadIndex(indexAttribute);
        //Log::Debug(LOG_TAG, "Load index, size: %lu", indexAttribute.size);
    }
    
    if (texture) {
        uTextureUnitLocation = glGetUniformLocation(glesProgram, "u_TextureUnit");
    }else{
        if (Engine::getPlatform() == S_WEB){
            GLES2Util::generateEmptyTexture();
            uTextureUnitLocation = glGetUniformLocation(glesProgram, "u_TextureUnit");
        }
    }
    
    for (std::unordered_map<int, propertyData>::iterator it = properties.begin(); it != properties.end(); ++it)
    {
        int type = it->first;
        
        std::string propertyName;
        
        if (type == S_PROPERTY_MVPMATRIX){
            propertyName = "u_mvpMatrix";
        }else if (type == S_PROPERTY_MODELMATRIX){
            propertyName = "u_mMatrix";
        }else if (type == S_PROPERTY_NORMALMATRIX){
            propertyName = "u_nMatrix";
        }else if (type == S_PROPERTY_CAMERAPOS){
            propertyName = "u_EyePos";
        }else if (type == S_PROPERTY_TEXTURERECT){
            propertyName = "u_textureRect";
        }else if (type == S_PROPERTY_COLOR){
            propertyName = "u_Color";
        }else if (type == S_PROPERTY_AMBIENTLIGHT){
            propertyName = "u_AmbientLight";
        }else if (type == S_PROPERTY_NUMPOINTLIGHT){
            propertyName = "u_NumPointLight";
        }else if (type == S_PROPERTY_POINTLIGHT_POS){
            propertyName = "u_PointLightPos";
        }else if (type == S_PROPERTY_POINTLIGHT_POWER){
            propertyName = "u_PointLightPower";
        }else if (type == S_PROPERTY_POINTLIGHT_COLOR){
            propertyName = "u_PointLightColor";
        }else if (type == S_PROPERTY_NUMSPOTLIGHT){
            propertyName = "u_NumSpotLight";
        }else if (type == S_PROPERTY_SPOTLIGHT_POS){
            propertyName = "u_SpotLightPos";
        }else if (type == S_PROPERTY_SPOTLIGHT_POWER){
            propertyName = "u_SpotLightPower";
        }else if (type == S_PROPERTY_SPOTLIGHT_COLOR){
            propertyName = "u_SpotLightColor";
        }else if (type == S_PROPERTY_SPOTLIGHT_TARGET){
            propertyName = "u_SpotLightTarget";
        }else if (type == S_PROPERTY_SPOTLIGHT_CUTOFF){
            propertyName = "u_SpotLightCutOff";
        }else if (type == S_PROPERTY_NUMDIRLIGHT){
            propertyName = "u_NumDirectionalLight";
        }else if (type == S_PROPERTY_DIRLIGHT_DIR){
            propertyName = "u_DirectionalLightDir";
        }else if (type == S_PROPERTY_DIRLIGHT_POWER){
            propertyName = "u_DirectionalLightPower";
        }else if (type == S_PROPERTY_DIRLIGHT_COLOR){
            propertyName = "u_DirectionalLightColor";
        }else if (type == S_PROPERTY_FOG_MODE){
            propertyName = "u_fogMode";
        }else if (type == S_PROPERTY_FOG_COLOR){
            propertyName = "u_fogColor";
        }else if (type == S_PROPERTY_FOG_DENSITY){
            propertyName = "u_fogDensity";
        }else if (type == S_PROPERTY_FOG_VISIBILITY){
            propertyName = "u_fogVisibility";
        }else if (type == S_PROPERTY_FOG_START){
            propertyName = "u_fogStart";
        }else if (type == S_PROPERTY_FOG_END){
            propertyName = "u_fogEnd";
        }
        
        propertyGL[type].handle = glGetUniformLocation(glesProgram, propertyName.c_str());
        //Log::Debug(LOG_TAG, "Get property handle: %s, size: %lu, handle %i", propertyName.c_str(), it->second.size, propertyGL[type].handle);
    }
    
    GLES2Util::checkGlError("Error on load GLES2");

    return true;
}

bool GLES2Object::prepareDraw(){
    if (!ObjectRender::prepareDraw()){
        return false;
    }
    
    GLuint glesProgram = ((GLES2Program*)program->getProgramRender().get())->getProgram();
    if (programOwned){
        glUseProgram(glesProgram);
        GLES2Util::checkGlError("glUseProgram");
    }
    
    for (std::unordered_map<int, propertyData>::iterator it = properties.begin(); it != properties.end(); ++it)
    {
        propertyGlData pb = propertyGL[it->first];
        if (pb.handle != -1){
            if (it->second.datatype == S_PROPERTYDATA_FLOAT1){
                glUniform1fv(pb.handle, (GLsizei)it->second.size, (GLfloat*)it->second.data);
            }else if (it->second.datatype == S_PROPERTYDATA_FLOAT2){
                glUniform2fv(pb.handle, (GLsizei)it->second.size, (GLfloat*)it->second.data);
            }else if (it->second.datatype == S_PROPERTYDATA_FLOAT3){
                glUniform3fv(pb.handle, (GLsizei)it->second.size, (GLfloat*)it->second.data);
            }else if (it->second.datatype == S_PROPERTYDATA_FLOAT4){
                glUniform4fv(pb.handle, (GLsizei)it->second.size, (GLfloat*)it->second.data);
            }else if (it->second.datatype == S_PROPERTYDATA_INT1){
                glUniform1iv(pb.handle, (GLsizei)it->second.size, (GLint*)it->second.data);
            }else if (it->second.datatype == S_PROPERTYDATA_INT2){
                glUniform2iv(pb.handle, (GLsizei)it->second.size, (GLint*)it->second.data);
            }else if (it->second.datatype == S_PROPERTYDATA_INT3){
                glUniform3iv(pb.handle, (GLsizei)it->second.size, (GLint*)it->second.data);
            }else if (it->second.datatype == S_PROPERTYDATA_INT4){
                glUniform4iv(pb.handle, (GLsizei)it->second.size, (GLint*)it->second.data);
            }else if (it->second.datatype == S_PROPERTYDATA_MATRIX2){
                glUniformMatrix2fv(pb.handle, (GLsizei)it->second.size, GL_FALSE, (GLfloat*)it->second.data);
            }else if (it->second.datatype == S_PROPERTYDATA_MATRIX3){
                glUniformMatrix3fv(pb.handle, (GLsizei)it->second.size, GL_FALSE, (GLfloat*)it->second.data);
            }else if (it->second.datatype == S_PROPERTYDATA_MATRIX4){
                glUniformMatrix4fv(pb.handle, (GLsizei)it->second.size, GL_FALSE, (GLfloat*)it->second.data);
            }
        }
        //Log::Debug(LOG_TAG, "Use property handle: %i", propertyGL[it->first].handle);
    }
    GLES2Util::checkGlError("Error on use property on draw");
    
    for (std::unordered_map<int, attributeData>::iterator it = vertexAttributes.begin(); it != vertexAttributes.end(); ++it)
    {
        attributeGlData att = attributesGL[it->first];
        if (att.handle != -1){
            glEnableVertexAttribArray(att.handle);
            glBindBuffer(GL_ARRAY_BUFFER, att.buffer);
            glVertexAttribPointer(att.handle, it->second.elements, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(0));
        }
        //Log::Debug(LOG_TAG, "Use attribute handle: %i", att.handle);
    }
    GLES2Util::checkGlError("Error on bind attribute vertex buffer");
    
    if (indexAttribute.data){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexGL.buffer);
    }
    GLES2Util::checkGlError("Error on bind index buffer");
    
    if (texture){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(((GLES2Texture*)(texture->getTextureRender().get()))->getTextureType(),
                      ((GLES2Texture*)(texture->getTextureRender().get()))->getTexture());
        glUniform1i(uTextureUnitLocation, 0);
    }else{
        if (Engine::getPlatform() == S_WEB){
            //Fix Chrome warnings of no texture bound
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, GLES2Util::emptyTexture);
            glUniform1i(uTextureUnitLocation, 0);
        }
    }
    GLES2Util::checkGlError("Error on bind texture");
    
    return true;
}

bool GLES2Object::draw(){
    if (!ObjectRender::draw()){
        return false;
    }
    
    //Log::Debug(LOG_TAG, "Start draw object");
    if ((!vertexAttributes.count(S_VERTEXATTRIBUTE_VERTICES)) and (indexAttribute.size == 0)){
        Log::Debug(LOG_TAG, "Cannot draw object: no vertices or indices");
        return false;
    }
        
    glUniform1i(useTexture, (texture?true:false));
        
    GLenum modeGles = GL_TRIANGLES;
    if (primitiveType == S_PRIMITIVE_TRIANGLES_STRIP){
        modeGles = GL_TRIANGLE_STRIP;
    }
    if (primitiveType == S_PRIMITIVE_POINTS){
        modeGles = GL_POINTS;
    }
    
    if (indexAttribute.data){
        glDrawElements(modeGles, (GLsizei)indexAttribute.size, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    }else{
        glDrawArrays(modeGles, 0, (GLsizei)vertexAttributes[S_VERTEXATTRIBUTE_VERTICES].size);
    }
    
    GLES2Util::checkGlError("Error on draw GLES2");

    return true;
}

bool GLES2Object::finishDraw(){
    if (!ObjectRender::finishDraw()){
        return false;
    }
    
    for (std::unordered_map<int, attributeGlData>::iterator it = attributesGL.begin(); it != attributesGL.end(); ++it)
        if (it->second.handle != -1)
            glDisableVertexAttribArray(it->second.handle);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    return true;
}

void GLES2Object::destroy(){
    
    for (std::unordered_map<int, attributeGlData>::iterator it = attributesGL.begin(); it != attributesGL.end(); ++it)
        if (it->second.handle != -1)
            glDeleteBuffers(1, &it->second.buffer);
    
    if (indexAttribute.data)
        glDeleteBuffers(1, &indexGL.buffer);

    ObjectRender::destroy();
}