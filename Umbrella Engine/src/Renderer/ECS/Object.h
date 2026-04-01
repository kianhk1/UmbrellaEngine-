#pragma once
#include "System.h"
class obj : public Entity
{
public:
	obj(std::shared_ptr<API::GraphicsAPI> Graphic, Texture2DData* t_data) {
		Component* shader = new ShaderComponent(Graphic, "Shader/vertex_shader.glsl", "Shader/fragment_shader.glsl");
		AddComponent(shader);
		Component* mesh = new MeshComponent(Graphic, vertex, index, GetComponent<ShaderComponent>()->shaderdata);
		AddComponent(mesh);
		Component* transform = new TransformComponent(Graphic, GetComponent<ShaderComponent>()->shaderdata);
		AddComponent(transform);
		Component* texture = new Texture2DComponent(Graphic, t_data, GetComponent<ShaderComponent>()->shaderdata);
		AddComponent(texture);
	}
	void Start() override {
		GetComponent<MeshComponent>()->setAttrib(0, 3, 14, 0);
		GetComponent<MeshComponent>()->setAttrib(1, 3, 14, 3);
		GetComponent<MeshComponent>()->setAttrib(2, 2, 14, 6);
		GetComponent<Texture2DComponent>()->Bind();
		GetComponent<Texture2DComponent>()->set_texture("texture1");
	}
	void Update(float dt) override {
		Entity::Update(dt);
		GetComponent<Texture2DComponent>()->Bind();
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