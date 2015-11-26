#include "GenericMap.h"

GenericMap::GenericMap()
{
}

void GenericMap::set(std::string name, bool value) { map_bool[name] = value;}
void GenericMap::set(std::string name, int value)  { map_int[name]  = value;}
void GenericMap::set(std::string name, float value) { map_float[name] = value;}
void GenericMap::set(std::string name, double value) { map_double[name] = value;}
void GenericMap::set(std::string name, glm::vec2 value) { map_vec2[name] = value;}
void GenericMap::set(std::string name, glm::vec3 value) { map_vec3[name] = value;}
void GenericMap::set(std::string name, glm::vec4 value) { map_vec4[name] = value;}
void GenericMap::set(std::string name, glm::mat2 value) { map_mat2[name] = value;}
void GenericMap::set(std::string name, glm::mat3 value) { map_mat3[name] = value;}
void GenericMap::set(std::string name, glm::mat4 value) { map_mat4[name] = value;}

bool GenericMap::getBool(std::string name)
{
    if (map_bool.find(name) != map_bool.end()) return map_bool[name];
    return false;
}

int GenericMap::getInt(std::string name)
{
    if (map_int.find(name) != map_int.end()) return map_int[name];
    return 0;
}

float GenericMap::getFloat(std::string name)
{
    if (map_float.find(name) != map_float.end()) return map_float[name];
    return 0.0f;
}

double GenericMap::getDouble(std::string name)
{
    if (map_double.find(name) != map_double.end()) return map_double[name];
    return 0.0;
}

glm::vec2 GenericMap::getVec2(std::string name)
{
    if (map_vec2.find(name) != map_vec2.end()) return map_vec2[name];
    return glm::vec2(0.0f);
}

glm::vec3 GenericMap::getVec3(std::string name)
{
    if (map_vec3.find(name) != map_vec3.end()) return map_vec3[name];
    return glm::vec3(0.0f);
}

glm::vec4 GenericMap::getVec4(std::string name)
{
    if (map_vec4.find(name) != map_vec4.end()) return map_vec4[name];
    return glm::vec4(0.0f);
}

glm::mat2 GenericMap::getMat2(std::string name)
{
    if (map_mat2.find(name) != map_mat2.end()) return map_mat2[name];
    return glm::mat2(1.0f);
}

glm::mat3 GenericMap::getMat3(std::string name)
{
    if (map_mat3.find(name) != map_mat3.end()) return map_mat3[name];
    return glm::mat3(1.0f);
}

glm::mat4 GenericMap::getMat4(std::string name)
{
    if (map_mat4.find(name) != map_mat4.end()) return map_mat4[name];
    return glm::mat4(1.0f);
}
