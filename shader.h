#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
        void setVec2(const std::string&, const glm::vec2&) const;
        void setVec2(const std::string&, float, float) const;
        void setVec3(const std::string&, const glm::vec3&) const;
        void setVec3(const std::string&, float, float, float) const;
        void setVec4(const std::string&, const glm::vec4&) const;
        void setVec4(const std::string&, float, float, float, float) const;
        void setMat2(const std::string&, const glm::mat2&) const;
        void setMat3(const std::string&, const glm::mat3&) const;
        void setMat4(const std::string&, const glm::mat4&) const;
};

#endif