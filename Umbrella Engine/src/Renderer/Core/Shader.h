#pragma once
#include <iostream>
#include <fstream>
#include "../../Core/Log Managment/Logger.h"
struct Size { int width; int height; };
struct Point { int x; int y; };
using namespace std;
class ShaderData {
public:
	ShaderData(string vs_path, string fs_path) : path_Vertex_Shader(vs_path), path_Fragment_Shader(fs_path){}
	string Read_File(const char* path) {
		ifstream file(path);
		stringstream buf;
		buf << file.rdbuf();
		string content = buf.str();
		if (content.size() >= 3 &&
			(unsigned char)content[0] == 0xEF &&
			(unsigned char)content[1] == 0xBB &&
			(unsigned char)content[2] == 0xBF) {
			content = content.substr(3);
		}
		return content;
	}
private:
	string path_Vertex_Shader;
	string path_Fragment_Shader;
};

/*class ShaderManager { 
public: 
Shader* Load(string vs_path, string fs_path) {
	string path = vs_path + fs_path; 
	auto it = shaders.find(path); 
	if (it != shaders.end()) { 
		return it->second;
	} 
	Shader* shader = new Shader(vs_path.c_str(), fs_path.c_str());
	shader->createshader();
	shaders.emplace(path, shader);
	return shader; 
} 
private: 
	static std::map<string, Shader*> shaders; 
};*/
