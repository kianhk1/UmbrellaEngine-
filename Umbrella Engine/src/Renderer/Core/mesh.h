#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

using namespace std;


/*class Mesh
{
public:
   
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
};*/