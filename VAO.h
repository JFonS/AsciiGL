#ifndef VAO_H
#define VAO_H

#include <vector>
#include <map>
#include <tuple>

#include "glm/glm.hpp"

class Pipeline;

class VAO
{

friend class Pipeline;

private:
    int VBOsize;
    std::map<std::string, std::vector<bool> >      vbos_bool;
    std::map<std::string, std::vector<int> >       vbos_int;
    std::map<std::string, std::vector<float> >     vbos_float;
    std::map<std::string, std::vector<double> >    vbos_double;
    std::map<std::string, std::vector<glm::vec2> > vbos_vec2;
    std::map<std::string, std::vector<glm::vec3> > vbos_vec3;
    std::map<std::string, std::vector<glm::vec4> > vbos_vec4;
    std::map<std::string, std::vector<glm::mat2> > vbos_mat2;
    std::map<std::string, std::vector<glm::mat3> > vbos_mat3;
    std::map<std::string, std::vector<glm::mat4> > vbos_mat4;

    int currentVertexIndex;

    std::map<std::string, bool>      uniforms_bool;
    std::map<std::string, int>       uniforms_int;
    std::map<std::string, float>     uniforms_float;
    std::map<std::string, double>    uniforms_double;
    std::map<std::string, glm::vec2> uniforms_vec2;
    std::map<std::string, glm::vec3> uniforms_vec3;
    std::map<std::string, glm::vec4> uniforms_vec4;
    std::map<std::string, glm::mat2> uniforms_mat2;
    std::map<std::string, glm::mat3> uniforms_mat3;
    std::map<std::string, glm::mat4> uniforms_mat4;

public:

    VAO();
    int size();

    //TO DO check if addng VBOs of different size
    void addVBO(const std::string &name, const std::vector<bool> &data)      { vbos_bool[name] = data;   if(VBOsize<0)VBOsize=data.size();}
    void addVBO(const std::string &name, const std::vector<int> &data)       { vbos_int[name] = data;    if(VBOsize<0)VBOsize=data.size();}
    void addVBO(const std::string &name, const std::vector<float> &data)     { vbos_float[name] = data;  if(VBOsize<0)VBOsize=data.size();}
    void addVBO(const std::string &name, const std::vector<double> &data)    { vbos_double[name] = data; if(VBOsize<0)VBOsize=data.size();}
    void addVBO(const std::string &name, const std::vector<glm::vec2> &data) { vbos_vec2[name] = data;   if(VBOsize<0)VBOsize=data.size();}
    void addVBO(const std::string &name, const std::vector<glm::vec3> &data) { vbos_vec3[name] = data;   if(VBOsize<0)VBOsize=data.size();}
    void addVBO(const std::string &name, const std::vector<glm::vec4> &data) { vbos_vec4[name] = data;   if(VBOsize<0)VBOsize=data.size();}
    void addVBO(const std::string &name, const std::vector<glm::mat2> &data) { vbos_mat2[name] = data;   if(VBOsize<0)VBOsize=data.size();}
    void addVBO(const std::string &name, const std::vector<glm::mat3> &data) { vbos_mat3[name] = data;   if(VBOsize<0)VBOsize=data.size();}
    void addVBO(const std::string &name, const std::vector<glm::mat4> &data) { vbos_mat4[name] = data;   if(VBOsize<0)VBOsize=data.size();}

    void setBool(std::string name, bool value) { uniforms_bool[name] = value;}
    void setInt(std::string name,  int value)  { uniforms_int[name]  = value;}
    void setFloat(std::string name, float value) { uniforms_float[name] = value;}
    void setDouble(std::string name, double value) { uniforms_double[name] = value;}
    void setVec2(std::string name, glm::vec2 value) { uniforms_vec2[name] = value;}
    void setVec3(std::string name, glm::vec3 value) { uniforms_vec3[name] = value;}
    void setVec4(std::string name, glm::vec4 value) { uniforms_vec4[name] = value;}
    void setMat2(std::string name, glm::mat2 value) { uniforms_mat2[name] = value;}
    void setMat3(std::string name, glm::mat3 value) { uniforms_mat3[name] = value;}
    void setMat4(std::string name, glm::mat4 value) { uniforms_mat4[name] = value;}

    template <class T>
    bool getBool(std::string name) {
        if(vbos_bool.find(name) != vbos_bool.end()) return vbos_bool[name][currentVertexIndex];
        else if (uniforms_bool.find(name) != uniforms_bool.end()) return uniforms_bool[name];}
    int getInt(std::string name) {
        if(vbos_int.find(name) != vbos_int.end()) return vbos_int[name][currentVertexIndex];
        else if (uniforms_int.find(name) != uniforms_int.end()) return uniforms_int[name];}
    float getFloat(std::string name) {
        if(vbos_float.find(name) != vbos_float.end()) return vbos_float[name][currentVertexIndex];
        else if (uniforms_float.find(name) != uniforms_float.end()) return uniforms_float[name];}
    double getDouble(std::string name) {
        if(vbos_double.find(name) != vbos_double.end()) return vbos_double[name][currentVertexIndex];
        else if (uniforms_double.find(name) != uniforms_double.end()) return uniforms_double[name];}
    glm::vec2 getVec2(std::string name) {
        if(vbos_vec2.find(name) != vbos_vec2.end()) return vbos_vec2[name][currentVertexIndex];
        else if (uniforms_vec2.find(name) != uniforms_vec2.end()) return uniforms_vec2[name];}
    glm::vec3 getVec3(std::string name) {
        if(vbos_vec3.find(name) != vbos_vec3.end()) return vbos_vec3[name][currentVertexIndex];
        else if (uniforms_vec3.find(name) != uniforms_vec3.end()) return uniforms_vec3[name];}
    glm::vec4 getVec4(std::string name) {
        if(vbos_vec4.find(name) != vbos_vec4.end()) return vbos_vec4[name][currentVertexIndex];
        else if (uniforms_vec4.find(name) != uniforms_vec4.end()) return uniforms_vec4[name];}
    glm::mat2 getMat2(std::string name) {
        if(vbos_mat2.find(name) != vbos_mat2.end()) return vbos_mat2[name][currentVertexIndex];
        else if (uniforms_mat2.find(name) != uniforms_mat2.end()) return uniforms_mat2[name];}
    glm::mat3 getMat3(std::string name) {
        if(vbos_mat3.find(name) != vbos_mat3.end()) return vbos_mat3[name][currentVertexIndex];
        else if (uniforms_mat3.find(name) != uniforms_mat3.end()) return uniforms_mat3[name];}
    glm::mat4 getMat4(std::string name) {
        if(vbos_mat4.find(name) != vbos_mat4.end()) return vbos_mat4[name][currentVertexIndex];
        else if (uniforms_mat4.find(name) != uniforms_mat4.end()) return uniforms_mat4[name];}

    // TO DO (V2.0) void removeVBO(std::string name);

    std::vector<glm::vec3> vertices;
};

#endif // VAO_H
