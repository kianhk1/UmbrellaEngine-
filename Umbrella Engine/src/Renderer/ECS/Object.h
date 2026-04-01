#pragma once
#include "System.h"
class obj : public Entity
{
public:
	obj(std::shared_ptr<API::GraphicsAPI> Graphic) {
		Component* shader = new ShaderComponent(Graphic, "Shader/vertex_shader.glsl", "Shader/fragment_shader.glsl");
		AddComponent(shader);
		Component* mesh = new MeshComponent(Graphic, vertex, index, GetComponent<ShaderComponent>()->shaderdata);
		AddComponent(mesh);
		Component* transform = new TransformComponent(Graphic, GetComponent<ShaderComponent>()->shaderdata);
		AddComponent(transform);
	}
	void Start() override {
		GetComponent<MeshComponent>()->setAttrib(0, 3, 14, 0);
		GetComponent<MeshComponent>()->setAttrib(1, 3, 14, 3);
	}
	void Update(float dt) override {
		Entity::Update(dt);
		GetComponent<MeshComponent>()->draw();
	}
private:
	std::vector<float> vertex = {
		// position                color          texcoord            normal            tangent
		-0.5f, -0.5f, 0.0f,   1.0f,1.0f,1.0f,    0.0f, 0.0f,     0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,  // 0
		 0.5f, -0.5f, 0.0f,   1.0f,1.0f,1.0f,    1.0f, 0.0f,     0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,  // 1
		 0.5f,  0.5f, 0.0f,   1.0f,1.0f,1.0f,    1.0f, 1.0f,     0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,  // 2
		-0.5f,  0.5f, 0.0f,   1.0f,1.0f,1.0f,    0.0f, 1.0f,     0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f   // 3
	};
	std::vector<unsigned int> index = {
		0, 1, 2,  // first triangle
		2, 3, 0   // second triangle
	};
};