#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

using namespace std;

struct MeshData {
    unsigned int vaoID = 0;
    unsigned int vboID = 0;
    unsigned int iboID = 0;
    int indexCount = 0;
};
class Mesh
{
public:
    ~Mesh() {
        if (data.vaoID != 0) glDeleteVertexArrays(1, &data.vaoID);
        if (data.vboID != 0) glDeleteBuffers(1, &data.vboID);
        if (data.iboID != 0) glDeleteBuffers(1, &data.iboID);
        data.vaoID = data.vboID = data.iboID = 0;
        data.indexCount = 0;
    }
    void createVAO() {
        glGenVertexArrays(1, &data.vaoID);
        glBindVertexArray(data.vaoID);
    }
    void BindVao() {
        glBindVertexArray(data.vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, data.vboID);
        glEnableVertexAttribArray(data.iboID);
    }
    void createVBO(float* vertex, int a) {
        glBindVertexArray(data.vaoID);
        glGenBuffers(1, &data.vboID);
        glBindBuffer(GL_ARRAY_BUFFER, data.vboID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * a, vertex, GL_STATIC_DRAW);
    }
    void setAttrib(int a, int b, int c, int d) {
        glBindVertexArray(data.vaoID);
        glEnableVertexAttribArray(a);
        glBindBuffer(GL_ARRAY_BUFFER, data.vboID);
        glVertexAttribPointer(a, b, GL_FLOAT, GL_FALSE, c * sizeof(float), (void*)(d * sizeof(float)));
    }
    void createEBO(unsigned int* indexes, int a) {
        glBindVertexArray(data.vaoID);
        glGenBuffers(1, &data.iboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.iboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * a, indexes, GL_STATIC_DRAW);
    }
    void draw(int indexCount) {
        glBindVertexArray(data.vaoID);
        glDrawElements(GL_TRIANGLES, data.indexCount, GL_UNSIGNED_INT, 0);
    }
    int GetindexCount() const { return data.indexCount; }
    int SetindexCount(int indexCount) { data.indexCount = indexCount; }
private:
    MeshData data;
};

class MeshManager {
public:
	Mesh* Loadplane(float* vertex, unsigned int* indexe) {

		auto it = meshes.find("plane");
		if (it != meshes.end())
			return it->second;

		Mesh* m = new Mesh();
		m->createVAO();
		m->createVBO(vertex, 56);
		m->createEBO(indexe, 6);

		meshes.emplace("plane", m);
		return m;
	}
	Mesh* Loadcube(float* vertex, unsigned int* indexe) {

		auto it = meshes.find("cube");
		if (it != meshes.end())
			return it->second;

		Mesh* m = new Mesh();
		m->createVAO();
		m->createVBO(vertex, 336);
		m->createEBO(indexe, 36);

		meshes.emplace("cube", m);
		return m;
	}
private:
	static std::map<string, Mesh*> meshes;
};