#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:
        //program id
        uint32_t ID;

        //reads and builds the shader
        void load(const GLchar*, const GLchar*);
        //use/activate the shader
        void use();
        //utility uniform functions
        void setBool(const std::string&, bool) const;
        void setInt(const std::string&, int) const;
        void setFloat(const std::string&, float) const;
};

#endif