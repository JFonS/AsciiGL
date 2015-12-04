#include "GenericMap.h"

GenericMap::GenericMap()
{
}

void GenericMap::set(const std::string& name, bool   value) { map_bool[name] = value;   }
void GenericMap::set(const std::string& name, int    value) { map_int[name]  = value;   }
void GenericMap::set(const std::string& name, float  value) { map_float[name] = value;  }
void GenericMap::set(const std::string& name, double value) { map_double[name] = value; }
void GenericMap::set(const std::string& name, const glm::vec2 &value) { map_vec2[name] = value; }
void GenericMap::set(const std::string& name, const glm::vec3 &value) { map_vec3[name] = value; }
void GenericMap::set(const std::string& name, const glm::vec4 &value) { map_vec4[name] = value; }
void GenericMap::set(const std::string& name, const glm::mat2 &value) { map_mat2[name] = value; }
void GenericMap::set(const std::string& name, const glm::mat3 &value) { map_mat3[name] = value; }
void GenericMap::set(const std::string& name, const glm::mat4 &value) { map_mat4[name] = value; }

bool GenericMap::getBool(const std::string&  name) const
{
    if (map_bool.find(name) != map_bool.end()) return map_bool.at(name);
    std::cerr << "WARNING: Bool not found: " << name << std::endl;
    return false;
}

int GenericMap::getInt(const std::string&  name) const
{
    if (map_int.find(name) != map_int.end()) return map_int.at(name);
    std::cerr << "WARNING: Int not found: " << name << std::endl;
    return 0;
}

float GenericMap::getFloat(const std::string&  name) const
{
    if (map_float.find(name) != map_float.end()) return map_float.at(name);
    std::cerr << "WARNING: Float not found: " << name << std::endl;
    return 0.0f;
}

double GenericMap::getDouble(const std::string&  name) const
{
    if (map_double.find(name) != map_double.end()) return map_double.at(name);
    std::cerr << "WARNING: Double not found: " << name << std::endl;
    return 0.0;
}

void GenericMap::getVec2(const std::string&  name, glm::vec2& vec2) const
{
    if (map_vec2.find(name) != map_vec2.end()) vec2 = map_vec2.at(name);
    else
    {
        std::cerr << "WARNING: Vec2 not found: " << name << std::endl;
        vec2 = glm::vec2(0.0f);
    }
}

void GenericMap::getVec3(const std::string&  name, glm::vec3& vec3) const
{
    if (map_vec3.find(name) != map_vec3.end()) vec3 = map_vec3.at(name);
    else
    {
        std::cerr << "WARNING: Vec3 not found: " << name << std::endl;
        vec3 = glm::vec3(0.0f);
    }
}

void GenericMap::getVec4(const std::string&  name, glm::vec4& vec4) const
{
    if (map_vec4.find(name) != map_vec4.end()) vec4 = map_vec4.at(name);
    else
    {
        std::cerr << "WARNING: Vec4 not found: " << name << std::endl;
        vec4 = glm::vec4(0.0f);
    }
}

void GenericMap::getMat2(const std::string&  name, glm::mat2& mat2) const
{
    if (map_mat2.find(name) != map_mat2.end()) mat2 = map_mat2.at(name);
    else
    {
        std::cerr << "WARNING: Mat2 not found: " << name << std::endl;
        mat2 = glm::mat2(1.0f);
    }
}

void GenericMap::getMat3(const std::string&  name, glm::mat3& mat3) const
{
    if (map_mat3.find(name) != map_mat3.end()) mat3 = map_mat3.at(name);
    else
    {
        std::cerr << "WARNING: Mat3 not found: " << name << std::endl;
        mat3 = glm::mat3(1.0f);
    }
}

void GenericMap::getMat4(const std::string&  name, glm::mat4& mat4) const
{
    if (map_mat4.find(name) != map_mat4.end()) mat4 = map_mat4.at(name);
    else
    {
        std::cerr << "WARNING: Mat4 not found: " << name << std::endl;
        mat4 = glm::mat4(1.0f);
    }
}

void GenericMap::interpolateTriangle(const std::vector<GenericMap> &attributes,
                                     const glm::vec3 &ws,
                                     GenericMap &interpolatedMap)
{
    float w0 = ws[0], w1 = ws[1], w2 = ws[2];
    for (auto &kv : attributes[0].map_bool) //Interpolate bool :)
    {
        float b0 = kv.second ? 1.0f : 0.0f;
        float b1 = attributes[1].map_bool.at(kv.first) ? 1.0f : 0.0f;
        float b2 = attributes[2].map_bool.at(kv.first) ? 1.0f : 0.0f;
        float b = w0 * b0 + w1 * b1 + w2 * b2;
        interpolatedMap.set(kv.first, b >= 0.5f);
    }

    for (auto &kv : attributes[0].map_int)
    {
        int i = int(round(w0 * (kv.second) + w1 * (attributes[1].map_int.at(kv.first)) + w2 * (attributes[2].map_int.at(kv.first))));
        interpolatedMap.set(kv.first, i);
    }

    for (auto &kv : attributes[0].map_float)
    {
        float f = w0 * (kv.second) + w1 * (attributes[1].map_float.at(kv.first)) + w2 * (attributes[2].map_float.at(kv.first));
        interpolatedMap.set(kv.first, f);
    }

    for (auto &kv : attributes[0].map_double)
    {
        double d = w0 * (kv.second) + w1 * (attributes[1].map_double.at(kv.first)) + w2 * (attributes[2].map_double.at(kv.first));
        interpolatedMap.set(kv.first, d);
    }

    for (auto &kv : attributes[0].map_vec2)
    {
        glm::vec2 v = w0 * (kv.second) + w1 * (attributes[1].map_vec2.at(kv.first)) + w2 * (attributes[2].map_vec2.at(kv.first));
        interpolatedMap.set(kv.first, v);
    }

    for (auto &kv : attributes[0].map_vec3)
    {
        glm::vec3 v = w0 * (kv.second) + w1 * (attributes[1].map_vec3.at(kv.first)) + w2 * (attributes[2].map_vec3.at(kv.first));
        interpolatedMap.set(kv.first, v);
    }

    for (auto &kv : attributes[0].map_vec4)
    {
        glm::vec4 v = w0 * (kv.second) + w1 * (attributes[1].map_vec4.at(kv.first)) + w2 * (attributes[2].map_vec4.at(kv.first));
        interpolatedMap.set(kv.first, v);
    }

    for (auto &kv : attributes[0].map_mat2)
    {
        glm::mat2 m = w0 * (kv.second) + w1 * (attributes[1].map_mat2.at(kv.first)) + w2 * (attributes[2].map_mat2.at(kv.first));
        interpolatedMap.set(kv.first, m);
    }

    for (auto &kv : attributes[0].map_mat3)
    {
        glm::mat3 m = w0 * (kv.second) + w1 * (attributes[1].map_mat3.at(kv.first)) + w2 * (attributes[2].map_mat3.at(kv.first));
        interpolatedMap.set(kv.first, m);
    }

    for (auto &kv : attributes[0].map_mat4)
    {
        glm::mat4 m = w0 * (kv.second) + w1 * (attributes[1].map_mat4.at(kv.first)) + w2 * (attributes[2].map_mat4.at(kv.first));
        interpolatedMap.set(kv.first, m);
    }
}

void GenericMap::interpolateLine(const std::vector<GenericMap> attributes,
                                 float w,
                                 GenericMap &interpolatedMap)
{
    float w0 = 1.0f - w, w1 = w;
    for (auto &kv : attributes[0].map_bool) //Interpolate bool :)
    {
        float b0 = kv.second ? 1.0f : 0.0f;
        float b1 = attributes[1].map_bool.at(kv.first) ? 1.0f : 0.0f;
        float b = w * b0 + w1 * b1;
        interpolatedMap.set(kv.first, b >= 0.5f);
    }

    for (auto &kv : attributes[0].map_int)
    {
        int i = int(round(w0 * (kv.second) + w1 * (attributes[1].map_int.at(kv.first))));
        interpolatedMap.set(kv.first, i);
    }

    for (auto &kv : attributes[0].map_float)
    {
        float f = w0 * (kv.second) + w1 * (attributes[1].map_float.at(kv.first));
        interpolatedMap.set(kv.first, f);
    }

    for (auto &kv : attributes[0].map_double)
    {
        double d = w0 * (kv.second) + w1 * (attributes[1].map_double.at(kv.first));
        interpolatedMap.set(kv.first, d);
    }

    for (auto &kv : attributes[0].map_vec2)
    {
        glm::vec2 v = w0 * (kv.second) + w1 * (attributes[1].map_vec2.at(kv.first));
        interpolatedMap.set(kv.first, v);
    }

    for (auto &kv : attributes[0].map_vec3)
    {
        glm::vec3 v = w0 * (kv.second) + w1 * (attributes[1].map_vec3.at(kv.first));
        interpolatedMap.set(kv.first, v);
    }

    for (auto &kv : attributes[0].map_vec4)
    {
        glm::vec4 v = w0 * (kv.second) + w1 * (attributes[1].map_vec4.at(kv.first));
        interpolatedMap.set(kv.first, v);
    }

    for (auto &kv : attributes[0].map_mat2)
    {
        glm::mat2 m = w0 * (kv.second) + w1 * (attributes[1].map_mat2.at(kv.first));
        interpolatedMap.set(kv.first, m);
    }

    for (auto &kv : attributes[0].map_mat3)
    {
        glm::mat3 m = w0 * (kv.second) + w1 * (attributes[1].map_mat3.at(kv.first));
        interpolatedMap.set(kv.first, m);
    }

    for (auto &kv : attributes[0].map_mat4)
    {
        glm::mat4 m = w0 * (kv.second) + w1 * (attributes[1].map_mat4.at(kv.first));
        interpolatedMap.set(kv.first, m);
    }
}
