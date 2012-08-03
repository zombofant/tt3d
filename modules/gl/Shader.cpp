/**********************************************************************
File name: Shader.cpp
This file is part of: tt3d — Freeform transport simulation

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/

#include "Shader.hpp"
#include "modules/io/Log.hpp"

namespace tt3d {
namespace GL {

/* tt3d::GL::Shader */

Shader::Shader(StreamHandle vertexShader, StreamHandle fragmentShader):
    Class::Class()
{
    loadFromStream(vertexShader, fragmentShader);
}

Shader::Shader(const std::string &vertexShader, const std::string &fragmentShader):
    Class::Class()
{
    loadFromString(vertexShader, fragmentShader);
}

GLuint Shader::compileShader(const GLenum kind, const std::string &source)
{
    GLuint result = glCreateShader(kind);
    const char *source_cstr = source.c_str();
    const GLint len = source.length();
    glShaderSource(result, 1, &source_cstr, &len);
    glCompileShader(result);
    
    GLsizei errorLen = 0;
    glGetShaderiv(result, GL_INFO_LOG_LENGTH, &errorLen);
    if (len > 1) {
        char *error = (char *)malloc(len);
        glGetShaderInfoLog(result, errorLen, &errorLen, error);
        IO::log << IO::ML_WARNING << "Shader compilation returned warnings/errors:" << std::endl << error << IO::submit;
        free(error);
    }
    
    return result;
}

void Shader::loadFromStream(StreamHandle vertexShader, StreamHandle fragmentShader) 
{
    std::string vertexShaderStr = vertexShader->readString(vertexShader->size());
    std::string fragmentShaderStr = fragmentShader->readString(fragmentShader->size());
    loadFromString(vertexShaderStr, fragmentShaderStr);
}

void Shader::loadFromString(const std::string &vertexShader, const std::string &fragmentShader) 
{
    clear();
    raiseLastGLError();
    glID = glCreateProgram();
    raiseLastGLError();
    try {
        GLuint vertexObj, fragmentObj;
        vertexObj = compileShader(GL_VERTEX_SHADER, vertexShader);
        fragmentObj = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
        
        glAttachShader(glID, vertexObj);
        glAttachShader(glID, fragmentObj);
        
        glLinkProgram(glID);
        const GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            glDeleteShader(vertexObj);
            glDeleteShader(fragmentObj);
            
            IO::log << IO::ML_ERROR << "Shader linking failed." << IO::submit;
            clear();
            return;
        }
        
        glDeleteShader(vertexObj);
        raiseLastGLError();
        glDeleteShader(fragmentObj);
        raiseLastGLError();
    } catch (...) {
        clear();
        throw;
    }
}

void Shader::bind() {
    if (glID == 0) {
        return;
    }
    glUseProgram(glID);
    raiseLastGLError();
}

void Shader::clear() {
    if (glID == 0) {
        return;
    }
    glDeleteProgram(glID);
    raiseLastGLError();
}

void Shader::unbind() {
    glUseProgram(0);
}

}
}
