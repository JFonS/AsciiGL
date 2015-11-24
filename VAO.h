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
public:

    VAO();

    void addVBO(const std::string &name, const std::vector<bool> &data) { vbos_bool[name] = data; }
    void addVBO(const std::string &name, const std::vector<int> &data) { vbos_int[name] = data; }
    void addVBO(const std::string &name, const std::vector<float> &data) { vbos_float[name] = data; }
    void addVBO(const std::string &name, const std::vector<double> &data) { vbos_double[name] = data; }
    void addVBO(const std::string &name, const std::vector<glm::vec2> &data) { vbos_vec2[name] = data; }
    void addVBO(const std::string &name, const std::vector<glm::vec3> &data) { vbos_vec3[name] = data; }
    void addVBO(const std::string &name, const std::vector<glm::vec4> &data) { vbos_vec4[name] = data; }
    void addVBO(const std::string &name, const std::vector<glm::mat2> &data) { vbos_mat2[name] = data; }
    void addVBO(const std::string &name, const std::vector<glm::mat3> &data) { vbos_mat3[name] = data; }
    void addVBO(const std::string &name, const std::vector<glm::mat4> &data) { vbos_mat4[name] = data; }

    template <class T>
    bool getBool(std::string name) {
        if(vbos_bool.find(name) != vbos_bool.end()) return vbos_bool[name][currentVertexIndex];}
    int getInt(std::string name) {
        if(vbos_int.find(name) != vbos_int.end()) return vbos_int[name][currentVertexIndex];}
    float getFloat(std::string name) {
        if(vbos_float.find(name) != vbos_float.end()) return vbos_float[name][currentVertexIndex];}
    double getDouble(std::string name) {
        if(vbos_double.find(name) != vbos_double.end()) return vbos_double[name][currentVertexIndex];}
    glm::vec2 getVec2(std::string name) {
        if(vbos_vec2.find(name) != vbos_vec2.end()) return vbos_vec2[name][currentVertexIndex];}
    glm::vec3 getVec3(std::string name) {
        if(vbos_vec3.find(name) != vbos_vec3.end()) return vbos_vec3[name][currentVertexIndex];}
    glm::vec4 getVec4(std::string name) {
        if(vbos_vec4.find(name) != vbos_vec4.end()) return vbos_vec4[name][currentVertexIndex];}
    glm::mat2 getMat2(std::string name) {
        if(vbos_mat2.find(name) != vbos_mat2.end()) return vbos_mat2[name][currentVertexIndex];}
    glm::mat3 getMat3(std::string name) {
        if(vbos_mat3.find(name) != vbos_mat3.end()) return vbos_mat3[name][currentVertexIndex];}
    glm::mat4 getMat4(std::string name) {
        if(vbos_mat4.find(name) != vbos_mat4.end()) return vbos_mat4[name][currentVertexIndex];}

    // V2.0 void removeVBO(std::string name);

    std::vector<glm::vec3> vertices;
};

#endif // VAO_H
