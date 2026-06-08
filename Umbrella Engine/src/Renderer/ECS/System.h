#pragma once
#include "../API/GraphicsAPI/openglGraphicAPI.h"
#include "Entity.h"
class SystemManager {
public:

};

class System {
public:
	System(std::shared_ptr<Engine::API::GraphicsAPI> graphic) : Graphic(graphic) {}
	virtual void Start() = 0;
	virtual void Update(float dt) = 0;
	std::shared_ptr<Engine::API::GraphicsAPI> Graphic;
};

class RenderSystem : public System {
public:
	RenderSystem(std::shared_ptr<Engine::API::GraphicsAPI> graphic) : System(graphic) {}
	void Start() override {
		auto view = registry.view<TransformComponent, MeshComponent, ShaderComponent>();
		
		view.each(
			[this](auto entity,
				TransformComponent& transform,
				MeshComponent& mesh,
				ShaderComponent& shader){
					// init shader
					shader.shader.programID = Graphic->createShader(); 
					unsigned int vs = Graphic->compileShader(GL_VERTEX_SHADER, Read_File(shader.shader.path_Vertex_Shader.c_str())); 
					unsigned int fs = Graphic->compileShader(GL_FRAGMENT_SHADER, Read_File(shader.shader.path_Fragment_Shader.c_str())); 
					Graphic->linkShaderProgram(shader.shader.programID, vs, fs); 
					Graphic->useShader(shader.shader);
					// init mesh
					mesh.meshdata = Graphic->createMesh(mesh.meshdata.vertices, mesh.meshdata.indices);  
					Graphic->setAttrib(mesh.meshdata, 0, 3, 14, 0);
					Graphic->setAttrib(mesh.meshdata, 1, 3, 14, 3); 
					Graphic->setAttrib(mesh.meshdata, 2, 2, 14, 6); 
					Graphic->setAttrib(mesh.meshdata, 3, 3, 14, 8); 
					Graphic->setAttrib(mesh.meshdata, 4, 3, 14, 11); 
			});
	}
	void Update(float dt) override {
		auto view = registry.view<TransformComponent, MeshComponent, ShaderComponent>();

		view.each( 
			[this](auto entity,
				TransformComponent& transform, 
				MeshComponent& mesh, 
				ShaderComponent& shader) {
				Graphic->useShader(shader.shader); 
				Graphic->drawMesh(mesh.meshdata);
			});
	}
private:
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
		if (content.empty())
			Logger::ERROR("files are empty!");
		return content;
	}
};