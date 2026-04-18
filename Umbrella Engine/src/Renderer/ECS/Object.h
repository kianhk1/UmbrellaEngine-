#pragma once
#include "Entity.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Objbase
{
public:
	Objbase(EntityManager* manager, std::shared_ptr<API::GraphicsAPI> Graphic) : Manager(manager) {
		entity = Manager->AddEntity();
		Component* transform = new TransformComponent(Graphic);
		Manager->AddComponent(transform, entity);
	}
	void AddComponent(Component* comp) {
		Manager->AddComponent(comp, entity);
	}
	template<typename T>
	T* GetComponent() {
		if (Manager) {
			return Manager->GetComponent<T>(entity);
		}
		return nullptr;
	}
private:
	EntityManager* Manager;
	Entity entity;
};

class objcube
{
public:
	objcube(EntityManager* manager, std::shared_ptr<API::GraphicsAPI> Graphic, const std::string& t_a, const std::string& t_n, const std::string& t_s) : Manager(manager){
		entity = Manager->AddEntity();
		Component* material = new MaterialComponent(Graphic, t_a, t_n, t_s, "Shader/vertex_shader.glsl", "Shader/fragment_shader.glsl");
		Manager->AddComponent(material, entity);
		Component* mesh = new MeshComponent(Graphic, vertex, index, Manager->GetComponent<MaterialComponent>(entity)->shader->shaderdata);
		Manager->AddComponent(mesh, entity);
		Component* transform = new TransformComponent(Graphic, Manager->GetComponent<MaterialComponent>(entity)->shader->shaderdata);
		Manager->AddComponent(transform, entity);
	}
	void Start() {
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(0, 3, 14, 0);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(1, 3, 14, 3);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(2, 2, 14, 6);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(3, 3, 14, 8);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(4, 3, 14, 11);
	}
	void AddComponent(Component* comp) {
		Manager->AddComponent(comp, entity);
	}
	template<typename T>
	T* GetComponent() {
		if (Manager) {
			return Manager->GetComponent<T>(entity);
		}
		return nullptr;
	}
private:
	EntityManager* Manager;
	Entity entity;
	std::vector<float> vertex = {
		//  x,    y,    z,        r,   g,   b,      u,  v,       nx,    ny,    nz,      tx,    ty,    tz
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
class Camera
{
public:
	Camera(EntityManager* manager, std::shared_ptr<API::GraphicsAPI> Graphic,
		ProjectionType Type,
		float AspectRatio) : Manager(manager) {
		entity = Manager->AddEntity();
		Component* c_trans = new TransformComponent(Graphic);
		Manager->AddComponent(c_trans, entity);
		Component* c_cam = new CameraComponent(Graphic, Type, AspectRatio, Manager->GetComponent<TransformComponent>(entity));
		Manager->AddComponent(c_cam, entity);
	}
	void AddComponent(Component* comp) {
		Manager->AddComponent(comp, entity);
	}
	template<typename T>
	T* GetComponent() {
		if (Manager) {
			return Manager->GetComponent<T>(entity);
		}
		return nullptr;
	}
private:
	EntityManager* Manager;
	Entity entity;
};
class object_load
{
public:
	// نگه داشتن اینپورتور به صورت فیلد کلاس برای جلوگیری از حذف داده‌های scene
	Assimp::Importer importer;
	const aiScene* scene;
	vector<float> vertices;
	vector<unsigned int> indices;
	string DIFFUSE, SPECULAR, NORMALS;

	object_load(EntityManager* manager, std::shared_ptr<API::GraphicsAPI> Graphic, const std::string& mpdle_path) : Manager(manager) {
		// فلگ‌های بهینه شده
		scene = importer.ReadFile(mpdle_path,
			aiProcess_Triangulate |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_FlipUVs |
			aiProcess_OptimizeMeshes);

		if (!scene || !scene->mRootNode) {
			Logger::ERROR(importer.GetErrorString());
			return;
		}
		load_model(scene->mRootNode, scene);
		Info("load model " + scene->mName.C_Str() + "successfully: " + mpdle_path + '\n');
		entity = Manager->AddEntity();
		Component* material = new MaterialComponent(Graphic, DIFFUSE, NORMALS, SPECULAR, "Shader/vertex_shader.glsl", "Shader/fragment_shader.glsl");
		Manager->AddComponent(material, entity);
		Component* mesh = new MeshComponent(Graphic, vertices, indices, Manager->GetComponent<MaterialComponent>(entity)->shader->shaderdata);
		Manager->AddComponent(mesh, entity);
		Component* transform = new TransformComponent(Graphic, Manager->GetComponent<MaterialComponent>(entity)->shader->shaderdata);
		Manager->AddComponent(transform, entity);
	}
	void Start() {
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(0, 3, 14, 0);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(1, 3, 14, 3);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(2, 2, 14, 6);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(3, 3, 14, 8);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(4, 3, 14, 11);
	}
	void AddComponent(Component* comp) {
		Manager->AddComponent(comp, entity);
	}
	template<typename T>
	T* GetComponent() {
		if (Manager) {
			return Manager->GetComponent<T>(entity);
		}
		return nullptr;
	}
private:
	void load_mesh(aiMesh* mesh, const aiScene* scene) {
		// اصلاح: پردازش متریال فقط یکبار برای هر مِش، نه برای هر وجه!
		unsigned int baseIndex = (unsigned int)(vertices.size() / 14);
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			processMaterial(material);
		}

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			// Position
			vertices.push_back(mesh->mVertices[i].x);
			vertices.push_back(mesh->mVertices[i].y);
			vertices.push_back(mesh->mVertices[i].z);

			// Color
			if (mesh->HasVertexColors(0)) { // اصلاح: چک کردن ایندکس 0
				vertices.push_back(mesh->mColors[0][i].r);
				vertices.push_back(mesh->mColors[0][i].g);
				vertices.push_back(mesh->mColors[0][i].b);
			}
			else {
				vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
			}

			// UV
			if (mesh->mTextureCoords[0]) {
				vertices.push_back(mesh->mTextureCoords[0][i].x);
				vertices.push_back(mesh->mTextureCoords[0][i].y);
			}
			else {
				vertices.push_back(0.0f); vertices.push_back(0.0f);
			}

			// Normals
			if (mesh->HasNormals()) {
				vertices.push_back(mesh->mNormals[i].x);
				vertices.push_back(mesh->mNormals[i].y);
				vertices.push_back(mesh->mNormals[i].z);
			}
			else {
				vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
			}

			// Tangents
			if (mesh->HasTangentsAndBitangents()) {
				vertices.push_back(mesh->mTangents[i].x);
				vertices.push_back(mesh->mTangents[i].y);
				vertices.push_back(mesh->mTangents[i].z);
			}
			else {
				vertices.push_back(1.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j] + baseIndex);
			}
		}
	}

	void processMaterial(aiMaterial* material) {
		aiString path;
		// استخراج بافت‌ها (اضافه کردن شرط برای جلوگیری از مسیرهای اشتباه)
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			DIFFUSE = string("Assets/") + path.C_Str();
		else DIFFUSE = "textures/sun.png";

		if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
			SPECULAR = string("Assets/") + path.C_Str();
		else SPECULAR = "textures/sun.png";

		if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
			NORMALS = string("Assets/") + path.C_Str();
		else NORMALS = "textures/sunn.png";
	}

	void load_model(aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			load_mesh(mesh, scene);
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			load_model(node->mChildren[i], scene);
		}
	}

	EntityManager* Manager;
	Entity entity;
};


class Obj
{
public:
	Obj(EntityManager* manager, std::shared_ptr<API::GraphicsAPI> Graphic, std::vector<float>& vertices,
		std::vector<unsigned int>& indices, Component* material) {
		entity = Manager->AddEntity();
		Manager->AddComponent(material, entity);
		Component* mesh = new MeshComponent(Graphic, vertices, indices, Manager->GetComponent<MaterialComponent>(entity)->shader->shaderdata);
		Manager->AddComponent(mesh, entity);
		Component* transform = new TransformComponent(Graphic, Manager->GetComponent<MaterialComponent>(entity)->shader->shaderdata);
		Manager->AddComponent(transform, entity);
	}
	void Start() {
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(0, 3, 14, 0);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(1, 3, 14, 3);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(2, 2, 14, 6);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(3, 3, 14, 8);
		Manager->GetComponent<MeshComponent>(entity)->setAttrib(4, 3, 14, 11);
	}
	void AddComponent(Component* comp) {
		Manager->AddComponent(comp, entity);
	}
	template<typename T>
	T* GetComponent() {
		if (Manager) {
			return Manager->GetComponent<T>(entity);
		}
		return nullptr;
	}
private:
	EntityManager* Manager;
	Entity entity;
};
struct Node
{
	vector<std::shared_ptr<Node>> nodes;
	vector<std::shared_ptr<Obj>> objects;
	TransformComponent* trans;
	std::string name = "";
	Node(std::string Name = "") : name(Name) {
	}
	void addNode(std::shared_ptr<Node> node) {
		nodes.push_back(node);
	}
	void addEntity(std::shared_ptr<Obj> obj) {
		objects.push_back(obj);
	}
};
class objectload
{
public:
	// نگه داشتن اینپورتور به صورت فیلد کلاس برای جلوگیری از حذف داده‌های scene
	Assimp::Importer importer;
	const aiScene* scene;
	string DIFFUSE, SPECULAR, NORMALS;
	std::unique_ptr<Node> rootNode;
	MaterialComponent* material;

	objectload(EntityManager* manager, std::shared_ptr<API::GraphicsAPI> Graphic, const std::string& mpdle_path) : Manager(manager) {
		// فلگ‌های بهینه شده
		scene = importer.ReadFile(mpdle_path,
			aiProcess_Triangulate |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_FlipUVs |
			aiProcess_OptimizeMeshes);

		if (!scene || !scene->mRootNode) {
			Logger::ERROR(importer.GetErrorString());
			return;
		}
		rootNode = std::make_unique<Node>();
		aiMaterial* aimaterial = scene->mMaterials[scene->mMeshes[0]->mMaterialIndex];
		processMaterial(aimaterial);
		material = new MaterialComponent(Graphic, DIFFUSE, NORMALS, SPECULAR, "Shader/vertex_shader.glsl", "Shader/fragment_shader.glsl");
		TransformComponent* transform = new TransformComponent(Graphic);
		rootNode->trans = transform;
		load_model(Graphic, material, scene->mRootNode, scene, rootNode.get());
		Info("load model " + scene->mName.C_Str() + "successfully: " + mpdle_path);
	}
	vector<std::shared_ptr<Obj>>& getEntities() {
		return objects;
	}
	void Start() {
		for (auto& entity : objects)
			entity->Start();
	}
	void AddComponent(Component* comp) {
		for (auto& obj : objects)
			obj->AddComponent(comp);
	}
	template<typename T>
	T* GetComponent() {
		if (Manager) {
			return objects[0]->GetComponent<T>();
		}
		return nullptr;
	}
private:

	std::shared_ptr<Obj> load_mesh(std::shared_ptr<API::GraphicsAPI> Graphic ,Component* material ,aiMesh* mesh, const aiScene* scene) {

		vector<float> vertices;
		vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			// Position
			vertices.push_back(mesh->mVertices[i].x);
			vertices.push_back(mesh->mVertices[i].y);
			vertices.push_back(mesh->mVertices[i].z);

			// Color
			if (mesh->HasVertexColors(0)) { // اصلاح: چک کردن ایندکس 0
				vertices.push_back(mesh->mColors[0][i].r);
				vertices.push_back(mesh->mColors[0][i].g);
				vertices.push_back(mesh->mColors[0][i].b);
			}
			else {
				vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
			}

			// UV
			if (mesh->mTextureCoords[0]) {
				vertices.push_back(mesh->mTextureCoords[0][i].x);
				vertices.push_back(mesh->mTextureCoords[0][i].y);
			}
			else {
				vertices.push_back(0.0f); vertices.push_back(0.0f);
			}

			// Normals
			if (mesh->HasNormals()) {
				vertices.push_back(mesh->mNormals[i].x);
				vertices.push_back(mesh->mNormals[i].y);
				vertices.push_back(mesh->mNormals[i].z);
			}
			else {
				vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
			}

			// Tangents
			if (mesh->HasTangentsAndBitangents()) {
				vertices.push_back(mesh->mTangents[i].x);
				vertices.push_back(mesh->mTangents[i].y);
				vertices.push_back(mesh->mTangents[i].z);
			}
			else {
				vertices.push_back(1.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}
		std::shared_ptr<Obj> obj = make_shared<Obj>(Manager, Graphic, vertices, indices, material);
		return obj;
	}

	void processMaterial(aiMaterial* material) {
		aiString path;
		// استخراج بافت‌ها (اضافه کردن شرط برای جلوگیری از مسیرهای اشتباه)
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			DIFFUSE = string("Assets/") + path.C_Str();
		else DIFFUSE = "textures/sun.png";

		if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
			SPECULAR = string("Assets/") + path.C_Str();
		else SPECULAR = "textures/sun.png";

		if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
			NORMALS = string("Assets/") + path.C_Str();
		else NORMALS = "textures/sunn.png";
	}

	void load_model(std::shared_ptr<API::GraphicsAPI> Graphic, MaterialComponent* material, aiNode* node, const aiScene* scene, Node* Rootnode) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			unsigned int meshIndex = node->mMeshes[i];
			aiMesh* mesh = scene->mMeshes[meshIndex];

			auto entity = load_mesh(Graphic, material, mesh, scene);
			entity->GetComponent< TransformComponent>()->parent = Rootnode->trans;
			objects.push_back(entity);
			Rootnode->addEntity(entity);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			aiNode* child = node->mChildren[i];

			auto cnode = std::make_shared<Node>(child->mName.C_Str());
			cnode->trans = new TransformComponent(Graphic);
			cnode->trans->parent = Rootnode->trans;

			Rootnode->addNode(cnode);
			load_model(Graphic, material, child, scene, cnode.get());
			Info("load node " + child->mName.C_Str() + " successfully");
		}
	}
	vector<std::shared_ptr<Obj>> objects;
	EntityManager* Manager;
	Entity entity;
};