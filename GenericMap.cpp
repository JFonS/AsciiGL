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

bool GenericMap::getBool(std::string name) const
{
    if (map_bool.find(name) != map_bool.end()) return map_bool[name];
    return false;
}

int GenericMap::getInt(std::string name) const
{
    if (map_int.find(name) != map_int.end()) return map_int[name];
    return 0;
}

float GenericMap::getFloat(std::string name) const
{
    if (map_float.find(name) != map_float.end()) return map_float[name];
    return 0.0f;
}

double GenericMap::getDouble(std::string name) const
{
    if (map_double.find(name) != map_double.end()) return map_double[name];
    return 0.0;
}

glm::vec2 GenericMap::getVec2(std::string name) const
{
    if (map_vec2.find(name) != map_vec2.end()) return map_vec2[name];
    return glm::vec2(0.0f);
}

glm::vec3 GenericMap::getVec3(std::string name) const
{
    if (map_vec3.find(name) != map_vec3.end()) return map_vec3[name];
    return glm::vec3(0.0f);
}

glm::vec4 GenericMap::getVec4(std::string name) const
{
    if (map_vec4.find(name) != map_vec4.end()) return map_vec4[name];
    return glm::vec4(0.0f);
}

glm::mat2 GenericMap::getMat2(std::string name) const
{
    if (map_mat2.find(name) != map_mat2.end()) return map_mat2[name];
    return glm::mat2(1.0f);
}

glm::mat3 GenericMap::getMat3(std::string name) const
{
    if (map_mat3.find(name) != map_mat3.end()) return map_mat3[name];
    return glm::mat3(1.0f);
}

glm::mat4 GenericMap::getMat4(std::string name) const
{
    if (map_mat4.find(name) != map_mat4.end()) return map_mat4[name];
    return glm::mat4(1.0f);
}

void GenericMap::interpolateTriangle(const std::vector<GenericMap> &attributes, const glm::vec3 &ws,
                                     GenericMap &interpolatedMap)
{
    float w0 = ws[0], w1 = ws[1], w2 = ws[2];
    for (auto &kv : attributes[0].map_bool) //Interpolate bool :)
    {
        float b0 = kv.second ? 1.0f : 0.0f;
        float b1 = attributes[1].map_bool[kv.first] ? 1.0f : 0.0f;
        float b2 = attributes[2].map_bool[kv.first] ? 1.0f : 0.0f;
        float b = w0 * b0 + w1 * b1 + w2 * b2;
        interpolatedMap.set(kv.first, b >= 0.5f);
    }

    for (auto &kv : attributes[0].map_int)
    {
        int i = int(w0 * (kv.second) + w1 * (attributes[1].map_int[kv.first]) + w2 * (attributes[1].map_int[kv.first]));
        interpolatedMap.set(kv.first, i);
    }

    for (auto &kv : attributes[0].map_float)
    {
        float f = w0 * (kv.second) + w1 * (attributes[1].map_float[kv.first]) + w2 * (attributes[1].map_float[kv.first]);
        interpolatedMap.set(kv.first, f);
    }

    for (auto &kv : attributes[0].map_double)
    {
        double d = w0 * (kv.second) + w1 * (attributes[1].map_double[kv.first]) + w2 * (attributes[1].map_double[kv.first]);
        interpolatedMap.set(kv.first, d);
    }

    for (auto &kv : attributes[0].map_vec2)
    {
        glm::vec2 v = w0 * (kv.second) + w1 * (attributes[1].map_vec2[kv.first]) + w2 * (attributes[1].map_vec2[kv.first]);
        interpolatedMap.set(kv.first, v);
    }

    for (auto &kv : attributes[0].map_vec3)
    {
        glm::vec3 v = w0 * (kv.second) + w1 * (attributes[1].map_vec3[kv.first]) + w2 * (attributes[1].map_vec3[kv.first]);
        interpolatedMap.set(kv.first, v);
    }

    for (auto &kv : attributes[0].map_vec4)
    {
        glm::vec4 v = w0 * (kv.second) + w1 * (attributes[1].map_vec4[kv.first]) + w2 * (attributes[1].map_vec4[kv.first]);
        interpolatedMap.set(kv.first, v);
    }

    for (auto &kv : attributes[0].map_mat2)
    {
        glm::mat2 m = w0 * (kv.second) + w1 * (attributes[1].map_mat2[kv.first]) + w2 * (attributes[1].map_mat2[kv.first]);
        interpolatedMap.set(kv.first, m);
    }

    for (auto &kv : attributes[0].map_mat3)
    {
        glm::mat3 m = w0 * (kv.second) + w1 * (attributes[1].map_mat3[kv.first]) + w2 * (attributes[1].map_mat3[kv.first]);
        interpolatedMap.set(kv.first, m);
    }

    for (auto &kv : attributes[0].map_mat4)
    {
        glm::mat4 m = w0 * (kv.second) + w1 * (attributes[1].map_mat4[kv.first]) + w2 * (attributes[1].map_mat4[kv.first]);
        interpolatedMap.set(kv.first, m);
    }
}
