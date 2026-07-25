#pragma once
#include <iostream>
#include "stb_image.h"
#include "FileSystem.h"
#include "../../Core/Data/Data.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include "../../Core/Log Managment/Logger.h"

namespace Engine {
    namespace CORE {
        struct ImageData {
			int width = 0;
            int height = 0;
			int nrChannels = 0;
            unsigned char* pixels = 0;
			std::string log;
        };
		

		class Reader
		{
		public:
			struct Data;
			static ImageData Readimage(const std::string& path) {
				if (!FileSystem::Exists(path))
					return {};
				stbi_set_flip_vertically_on_load(true);
				ImageData image;
				image.pixels = stbi_load(path.c_str(), &image.width, &image.height, &image.nrChannels, 0);
				if (image.pixels) {
					return image;
				}
				else {
					image.log = stbi_failure_reason();
					return image;
				}
			}
			static void Freeimage(ImageData& data) {
				stbi_image_free(data.pixels);
				data.pixels = nullptr;
				data.width = 0;
				data.height = 0;
				data.nrChannels = 0;
				data.log.clear();
			}

			static Data ReadModel(const std::string& path) {
				Assimp::Importer importer;
				const aiScene* scene =
					importer.ReadFile(path,
						aiProcess_Triangulate |
						aiProcess_CalcTangentSpace |
						aiProcess_JoinIdenticalVertices |
						aiProcess_FlipUVs |
						aiProcess_OptimizeMeshes |
						aiProcess_GenSmoothNormals |
						aiProcess_ImproveCacheLocality);

				if (!scene || !scene->mRootNode) {
					Error(CORE::LogCategory::Resource, importer.GetErrorString());
					return {};
				}
				Data data;
				data.root = std::make_shared<DATA::Node>();
				data.root->name = scene->mRootNode->mName.C_Str();
				data.root->localTransform = ConvertMatrix(scene->mRootNode->mTransformation);
				load_model(scene->mRootNode, scene, data.root, data);
				return data;
			}

		private:
			
			struct vertic { std::vector<float> vertices; std::vector<unsigned int> indices; };
			static vertic load_mesh(aiMesh* mesh, const aiScene* scene) {

				vertic Mesh;

				for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
					// Position
					Mesh.vertices.push_back(mesh->mVertices[i].x);
					Mesh.vertices.push_back(mesh->mVertices[i].y);
					Mesh.vertices.push_back(mesh->mVertices[i].z);

					// Color
					if (mesh->HasVertexColors(0)) { // ÇÕáÇÍ: ˜ ˜ÑÏä ÇíäÏ˜Ó 0
						Mesh.vertices.push_back(mesh->mColors[0][i].r);
						Mesh.vertices.push_back(mesh->mColors[0][i].g);
						Mesh.vertices.push_back(mesh->mColors[0][i].b);
					}
					else {
						Mesh.vertices.push_back(1.0f); 
						Mesh.vertices.push_back(1.0f); 
						Mesh.vertices.push_back(1.0f);
					}

					// UV
					if (mesh->mTextureCoords[0]) {
						Mesh.vertices.push_back(mesh->mTextureCoords[0][i].x);
						Mesh.vertices.push_back(mesh->mTextureCoords[0][i].y);
					}
					else {
						Mesh.vertices.push_back(0.0f); 
						Mesh.vertices.push_back(0.0f);
					}

					// Normals
					if (mesh->HasNormals()) {
						Mesh.vertices.push_back(mesh->mNormals[i].x);
						Mesh.vertices.push_back(mesh->mNormals[i].y);
						Mesh.vertices.push_back(mesh->mNormals[i].z);
					}
					else {
						Mesh.vertices.push_back(0.0f); 
						Mesh.vertices.push_back(1.0f); 
						Mesh.vertices.push_back(0.0f);
					}

					// Tangents
					if (mesh->HasTangentsAndBitangents()) {
						Mesh.vertices.push_back(mesh->mTangents[i].x);
						Mesh.vertices.push_back(mesh->mTangents[i].y);
						Mesh.vertices.push_back(mesh->mTangents[i].z);
					}
					else {
						Mesh.vertices.push_back(1.0f); 
						Mesh.vertices.push_back(0.0f); Mesh.
							vertices.push_back(0.0f);
					}
				}

				for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
					aiFace face = mesh->mFaces[i];
					for (unsigned int j = 0; j < face.mNumIndices; j++) {
						Mesh.indices.push_back(face.mIndices[j]);
					}
				}
				return Mesh;
			}
			struct Materialdesc
			{
				std::unordered_multimap<std::string, DATA::Uniform> uniforms;
				std::vector<std::string> texturedesc;
			};
			static Materialdesc processMaterial(aiMaterial* material) {
				aiString path;
				Materialdesc matrialdesc;
				// ÇÓÊÎÑÇÌ ÈÇÝÊåÇ (ÇÖÇÝå ˜ÑÏä ÔÑØ ÈÑÇí ÌáæíÑí ÇÒ ãÓíÑåÇí ÇÔÊÈÇå)
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
					matrialdesc.texturedesc.push_back(std::string("Assets/") + path.C_Str());
				if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
					matrialdesc.texturedesc.push_back(std::string("Assets/") + path.C_Str());
				if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
					matrialdesc.texturedesc.push_back(std::string("Assets/") + path.C_Str());
				//else texturedesc.push_back("Assets/sun.png");
				aiColor4D color; 
				if (material->Get(AI_MATKEY_BASE_COLOR, color) == AI_SUCCESS)
				{
					matrialdesc.uniforms.emplace("color", ConvertVctor4(color));
				}
				else if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
				{
					matrialdesc.uniforms.emplace("color", ConvertVctor4(color));
				}
				return matrialdesc;
			}

			struct rawModelpart
			{
				vertic mesh;
				Materialdesc materialdesc;
			};
			struct Data {
				std::shared_ptr<DATA::Node> root;
				std::vector<rawModelpart> parts;
			};

			static void load_model(aiNode* node, const aiScene* scene, std::shared_ptr<DATA::Node> currentNode, Data& data) {

				for (unsigned int i = 0; i < node->mNumMeshes; i++) {
					unsigned int meshIndex = node->mMeshes[i];
					aiMesh* mesh = scene->mMeshes[meshIndex];
					data.parts.push_back({ load_mesh(mesh, scene) ,processMaterial(scene->mMaterials[mesh->mMaterialIndex]) });
					currentNode->meshIndices.push_back(data.parts.size() - 1);
				}

				for (unsigned int i = 0; i < node->mNumChildren; i++) {
					aiNode* child = node->mChildren[i];

					auto ChildrenNode = std::make_shared<DATA::Node>();
					ChildrenNode->name = child->mName.C_Str();
					ChildrenNode->localTransform = ConvertMatrix(child->mTransformation); 
					ChildrenNode->worldTransform = currentNode->localTransform * ChildrenNode->localTransform; 

					currentNode->children.push_back(ChildrenNode);
					load_model(child, scene, ChildrenNode, data);
					//Info("load node " + child->mName.C_Str() + " successfully");
				}
			}

			static glm::mat4 ConvertMatrix(const aiMatrix4x4& m)
			{
				glm::mat4 result;

				result[0][0] = m.a1;
				result[1][0] = m.a2;
				result[2][0] = m.a3;
				result[3][0] = m.a4;

				result[0][1] = m.b1;
				result[1][1] = m.b2;
				result[2][1] = m.b3;
				result[3][1] = m.b4;

				result[0][2] = m.c1;
				result[1][2] = m.c2;
				result[2][2] = m.c3;
				result[3][2] = m.c4;

				result[0][3] = m.d1;
				result[1][3] = m.d2;
				result[2][3] = m.d3;
				result[3][3] = m.d4;

				return result;
			}
			static glm::vec4 ConvertVctor4(const aiColor4D& v4)
			{
				glm::vec4 result(v4.r, v4.g, v4.b, v4.a);

				return result;
			}
			Reader() = default;

		};
    }
}