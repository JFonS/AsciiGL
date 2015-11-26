#include "VAO.h"

VAO::VAO() : currentVertexIndex(0), VBOsize(-1)
{
}

int VAO::size()
{
    return VBOsize;
}

void VAO::addVBO(const std::string &name, const std::vector<bool> &data)      { vbos_bool[name] = data;   if(VBOsize < 0) VBOsize = data.size();}
void VAO::addVBO(const std::string &name, const std::vector<int> &data)       { vbos_int[name] = data;    if(VBOsize < 0) VBOsize = data.size();}
void VAO::addVBO(const std::string &name, const std::vector<double> &data)    { vbos_double[name] = data; if(VBOsize < 0) VBOsize = data.size();}
void VAO::addVBO(const std::string &name, const std::vector<float> &data)     { vbos_float[name] = data;  if(VBOsize < 0) VBOsize = data.size();}
void VAO::addVBO(const std::string &name, const std::vector<glm::vec2> &data) { vbos_vec2[name] = data;   if(VBOsize < 0) VBOsize = data.size();}
void VAO::addVBO(const std::string &name, const std::vector<glm::vec3> &data) { vbos_vec3[name] = data;   if(VBOsize < 0) VBOsize = data.size();}
void VAO::addVBO(const std::string &name, const std::vector<glm::vec4> &data) { vbos_vec4[name] = data;   if(VBOsize < 0) VBOsize = data.size();}
void VAO::addVBO(const std::string &name, const std::vector<glm::mat2> &data) { vbos_mat2[name] = data;   if(VBOsize < 0) VBOsize = data.size();}
void VAO::addVBO(const std::string &name, const std::vector<glm::mat3> &data) { vbos_mat3[name] = data;   if(VBOsize < 0) VBOsize = data.size();}
void VAO::addVBO(const std::string &name, const std::vector<glm::mat4> &data) { vbos_mat4[name] = data;   if(VBOsize < 0) VBOsize = data.size();}

bool VAO::getBool(std::string name)
{
    if(vbos_bool.find(name) != vbos_bool.end()) return vbos_bool[name][currentVertexIndex];
    return false;
}

int VAO::getInt(std::string name)
{
    if(vbos_int.find(name) != vbos_int.end()) return vbos_int[name][currentVertexIndex];
    return 0;
}

float VAO::getFloat(std::string name)
{
    if(vbos_float.find(name) != vbos_float.end()) return vbos_float[name][currentVertexIndex];
    return 0.0f;
}

double VAO::getDouble(std::string name)
{
    if(vbos_double.find(name) != vbos_double.end()) return vbos_double[name][currentVertexIndex];
    return 0.0;
}

glm::vec2 VAO::getVec2(std::string name)
{
    if(vbos_vec2.find(name) != vbos_vec2.end()) return vbos_vec2[name][currentVertexIndex];
    return glm::vec2(0.0f);
}

glm::vec3 VAO::getVec3(std::string name)
{
    if(vbos_vec3.find(name) != vbos_vec3.end()) return vbos_vec3[name][currentVertexIndex];
    return glm::vec3(0.0f);
}

glm::vec4 VAO::getVec4(std::string name)
{
    if(vbos_vec4.find(name) != vbos_vec4.end()) return vbos_vec4[name][currentVertexIndex];
    return glm::vec4(0.0f);
}

glm::mat2 VAO::getMat2(std::string name)
{
    if(vbos_mat2.find(name) != vbos_mat2.end()) return vbos_mat2[name][currentVertexIndex];
    return glm::mat2(1.0f);
}

glm::mat3 VAO::getMat3(std::string name)
{
    if(vbos_mat3.find(name) != vbos_mat3.end()) return vbos_mat3[name][currentVertexIndex];
    return glm::mat3(1.0f);
}

glm::mat4 VAO::getMat4(std::string name)
{
    if(vbos_mat4.find(name) != vbos_mat4.end()) return vbos_mat4[name][currentVertexIndex];
    return glm::mat4(1.0f);
}
