#pragma once
#include "System.h"
class obj : public Entity
{
public:
	obj(std::shared_ptr<API::GraphicsAPI> Graphic, DATA::Texture2DData* t_data) {
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
		Entity::Start();
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
		//     x,    y,    z,        r,   g,   b,      u,  v,       nx,    ny,    nz,      tx,    ty,    tz
		// جلو (Front) - تانژانت در راستای X
		-0.5f, -0.5f,  0.5f,    1.0, 0.0, 0.0,    0.0, 0.0,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,    0.0, 1.0, 0.0,    1.0, 0.0,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,    0.0, 0.0, 1.0,    1.0, 1.0,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    1.0, 1.0, 0.0,    0.0, 1.0,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f,  0.0f,

		// پشت (Back) - تانژانت در راستای معکوس X (بسته به جهت بافت)
		-0.5f, -0.5f, -0.5f,    1.0, 0.0, 1.0,    0.0, 0.0,    0.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,    0.0, 1.0, 1.0,    1.0, 0.0,    0.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,    1.0, 1.0, 1.0,    1.0, 1.0,    0.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.2, 0.8, 0.3,    0.0, 1.0,    0.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,

		// چپ (Left) - تانژانت در راستای Z
		-0.5f, -0.5f, -0.5f,    1.0, 0.0, 0.0,    0.0, 0.0,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0, 1.0, 0.0,    1.0, 0.0,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0, 0.0, 1.0,    1.0, 1.0,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    1.0, 1.0, 0.0,    0.0, 1.0,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,  1.0f,

		// راست (Right) - تانژانت در راستای معکوس Z
		 0.5f, -0.5f, -0.5f,    1.0, 0.0, 1.0,    0.0, 0.0,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f,  0.5f,    0.0, 1.0, 1.0,    1.0, 0.0,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f,  0.5f,    1.0, 1.0, 1.0,    1.0, 1.0,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,    0.2, 0.8, 0.3,    0.0, 1.0,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f, -1.0f,

		 // بالا (Top) - تانژانت در راستای X
		-0.5f,  0.5f, -0.5f,    1.0, 0.0, 0.0,    0.0, 0.0,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,    0.0, 1.0, 0.0,    1.0, 0.0,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,    0.0, 0.0, 1.0,    1.0, 1.0,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    1.0, 1.0, 0.0,    0.0, 1.0,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,  0.0f,

		// پایین (Bottom) - تانژانت در راستای معکوس X
	   -0.5f, -0.5f, -0.5f,    1.0, 0.0, 1.0,    0.0, 0.0,    0.0f, -1.0f,  0.0f,   -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,    0.0, 1.0, 1.0,    1.0, 0.0,    0.0f, -1.0f,  0.0f,   -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,    1.0, 1.0, 1.0,    1.0, 1.0,    0.0f, -1.0f,  0.0f,   -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,    0.2, 0.8, 0.3,    0.0, 1.0,    0.0f, -1.0f,  0.0f,   -1.0f,  0.0f,  0.0f
	};
	std::vector<unsigned int> index = {
		0, 1, 2, 2, 3, 0,       // جلو
		4, 5, 6, 6, 7, 4,       // پشت
		8, 9,10,10,11, 8,       // چپ
	   12,13,14,14,15,12,       // راست
	   16,17,18,18,19,16,       // بالا
	   20,21,22,22,23,20        // پایین
	};
};
/*class Camera : public Entity
{
public:
	Camera(std::shared_ptr<API::GraphicsAPI> Graphic, DATA::Texture2DData* t_data) {
		Component* transform = new TransformComponent(Graphic);
		AddComponent(transform);
		Component* camera = new CameraComponent(Graphic, t_data, GetComponent<ShaderComponent>()->shaderdata);
		AddComponent(camera);
	}
	void Start() override {
		Entity::Start();

	}
	void Update(float dt) override {
		Entity::Update(dt);
		GetComponent<Texture2DComponent>()->Bind();
	}
private:
};*/
