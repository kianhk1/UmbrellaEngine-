#pragma once

#include <iostream>
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../Core/Log Managment/Logger.h"
#include <map>
#include <vector>
using namespace std;

class loadTextures2D{
public:
	void Load(string imag_path) {
		ipath = imag_path;
		auto it = Texturesid.find(imag_path);

		if (it != Texturesid.end()) {
			textureid = it->second;
			return;
		}
		Texturesid.emplace(imag_path, loadimage(imag_path));
	}
	unsigned int loadimage(string path) {
		glGenTextures(1, &textureid);
		glBindTexture(GL_TEXTURE_2D, textureid);
		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		stbi_set_flip_vertically_on_load(true);
		images.pixels = stbi_load(path.c_str(), &images.width, &images.height, &nrChannels, 0);
		if (images.pixels) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images.width, images.height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, images.pixels);
			glGenerateMipmap(GL_TEXTURE_2D);
			Logger::INFO("Texture loaded successfully: " + path + " (" + to_string(images.width) + "x" + to_string(images.height) + ")\n");
			stbi_image_free(images.pixels);
		}
		else {
			Logger::ERROR("Texture loaded unsuccessfully: " + path + " (" + to_string(images.width) + "x" + to_string(images.height) + ")\n"+ stbi_failure_reason()+"\n");
			stbi_image_free(images.pixels);
		}
		return textureid;
	}
	void Bind(int t,unsigned int Texture) {
		glActiveTexture(t);
		glBindTexture(GL_TEXTURE_2D, Texture);
	}
	unsigned int get_texture() {
		auto it = Texturesid.find(ipath);
		return it->second;
	}
private:
	string ipath;
	unsigned int textureid;
	GLFWimage images;
	int nrChannels;
	static std::map<string, unsigned int> Texturesid;
};
class loadTexturesCubemap {
public:
	void Load(vector<std::string> faces) {
		ifaces = faces;
		auto it = Textures.find(faces);

		if (it != Textures.end()) {
			textureid = it->second;
			return;
		}
		Textures.emplace(faces, loadCubemap(faces));
	}
	unsigned int loadCubemap(vector<std::string> faces) {
		glGenTextures(1, &textureid);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//stbi_set_flip_vertically_on_load(true);
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			images.pixels = stbi_load(faces[i].c_str(), &images.width, &images.height,
				&nrChannels, 0);
			if (images.pixels) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, images.width, images.height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, images.pixels);
				Logger::INFO("Cubemap loaded successfully: " + faces[i] + " (" + to_string(images.width) + "x" + to_string(images.height) + ")\n");
				stbi_image_free(images.pixels);
			}
			else {
				Logger::INFO("Cubemap loaded unsuccessfully: " + faces[i] + " (" + to_string(images.width) + "x" + to_string(images.height) + ")\n");
				stbi_image_free(images.pixels);
			}
		}
		return textureid;
	}
	void Bind(int t, unsigned int Texture) {
		glActiveTexture(t);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
	}
	unsigned int get_texture() {
		auto it = Textures.find(ifaces);
		return it->second;
	}
private:
	vector<std::string> ifaces;
	unsigned int textureid;
	GLFWimage images;
	int nrChannels;
	static std::map<vector<std::string>, unsigned int> Textures;
};