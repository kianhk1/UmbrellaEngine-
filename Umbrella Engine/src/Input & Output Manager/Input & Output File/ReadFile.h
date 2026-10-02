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
			static ImageData Readimage(const std::string& path,bool flip = true) {
				if (!FileSystem::Exists(path))
					return {};
				stbi_set_flip_vertically_on_load(flip);
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
						//aiProcess_FlipUVs |
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
				data.globalInverseTransform =
					glm::inverse(ConvertMatrix(scene->mRootNode->mTransformation));
				for (unsigned int meshIndex = 0;meshIndex < scene->mNumMeshes;meshIndex++)
				{
					aiMesh* mesh = scene->mMeshes[meshIndex];

					for (unsigned int boneIndex = 0;boneIndex < mesh->mNumBones;boneIndex++)
					{
						aiBone* bone = mesh->mBones[boneIndex];

						std::string boneName = bone->mName.C_Str();

						if (data.boneMap.find(boneName) != data.boneMap.end())
							continue;

						DATA::Bone newBone;

						newBone.id =
							static_cast<uint32_t>(data.boneMap.size());

						newBone.name = boneName;

						newBone.offsetMatrix =
							ConvertMatrix(bone->mOffsetMatrix);

						//data.boneMap[boneName] = newBone;
						data.boneMap.emplace(boneName, newBone);
					}
				}
				for (unsigned int i = 0; i < scene->mNumAnimations; i++)
				{
					aiAnimation* animation = scene->mAnimations[i];

					DATA::AnimationData animData;

					animData.name = animation->mName.C_Str();
					animData.duration = animation->mDuration;
					animData.ticksPerSecond = animation->mTicksPerSecond;

					Info(CORE::LogCategory::Resource, "Animation:", animation->mName.C_Str());

					Info(CORE::LogCategory::Resource, "Channels:", animation->mNumChannels);

					for (unsigned int j = 0; j < animation->mNumChannels; j++)
					{
						aiNodeAnim* channel = animation->mChannels[j];

						DATA::AnimationChannel channelData;

						channelData.nodeName = channel->mNodeName.C_Str();
						Info(CORE::LogCategory::Resource, "Channels Name:", channel->mNodeName.C_Str());
						// Position
						for (unsigned int k = 0; k < channel->mNumPositionKeys; k++)
						{
							aiVectorKey& key = channel->mPositionKeys[k];

							DATA::PositionKey positionKey;

							positionKey.time = key.mTime;

							positionKey.value = glm::vec3(
								key.mValue.x,
								key.mValue.y,
								key.mValue.z
							);

							channelData.positionKeys.push_back(positionKey);
						}

						// Rotation
						for (unsigned int k = 0; k < channel->mNumRotationKeys; k++)
						{
							aiQuatKey& key = channel->mRotationKeys[k];

							DATA::RotationKey rotationKey;

							rotationKey.time = key.mTime;

							rotationKey.value = glm::quat(
								key.mValue.w,
								key.mValue.x,
								key.mValue.y,
								key.mValue.z
							);

							channelData.rotationKeys.push_back(rotationKey);
						}

						// Scale
						for (unsigned int k = 0; k < channel->mNumScalingKeys; k++)
						{
							aiVectorKey& key = channel->mScalingKeys[k];

							DATA::ScaleKey scaleKey;

							scaleKey.time = key.mTime;

							scaleKey.value = glm::vec3(
								key.mValue.x,
								key.mValue.y,
								key.mValue.z
							);

							channelData.scaleKeys.push_back(scaleKey);
						}

						animData.channels.push_back(channelData);
					}

					data.animations.push_back(animData);
				}

				load_model(scene->mRootNode, scene, data.root, data);
				return data;
			}

			static bool Compile(const std::string& cppFile,const std::string& outputDll);

		private:
			
			struct vertic { std::vector<DATA::Vertex> vertices; std::vector<unsigned int> indices; };

			static vertic load_mesh(aiMesh* mesh,const aiScene* scene,Data& data)
			{
				vertic Mesh;
				DATA::Vertex vertex{};

				// -------------------------
				// Vertices
				// -------------------------
				for (unsigned int i = 0; i < mesh->mNumVertices; i++)
				{
					vertex = {};

					// Position
					vertex.position = {
						mesh->mVertices[i].x,
						mesh->mVertices[i].y,
						mesh->mVertices[i].z
					};

					// Color
					if (mesh->HasVertexColors(0))
					{
						vertex.color = {
							mesh->mColors[0][i].r,
							mesh->mColors[0][i].g,
							mesh->mColors[0][i].b
						};
					}
					else
					{
						vertex.color = { 1.0f, 1.0f, 1.0f };
					}

					// UV
					if (mesh->mTextureCoords[0])
					{
						vertex.uv = {
							mesh->mTextureCoords[0][i].x,
							mesh->mTextureCoords[0][i].y
						};
					}
					else
					{
						vertex.uv = { 0.0f, 0.0f };
					}

					// Normals
					if (mesh->HasNormals())
					{
						vertex.normal = {
							mesh->mNormals[i].x,
							mesh->mNormals[i].y,
							mesh->mNormals[i].z
						};
					}
					else
					{
						vertex.normal = { 0.0f, 1.0f, 0.0f };
					}

					// Tangents
					if (mesh->HasTangentsAndBitangents())
					{
						vertex.tangent = {
							mesh->mTangents[i].x,
							mesh->mTangents[i].y,
							mesh->mTangents[i].z
						};
					}
					else
					{
						vertex.tangent = { 1.0f, 0.0f, 0.0f };
					}

					Mesh.vertices.push_back(vertex);
				}


				// -------------------------
				// Bones / Weights
				// -------------------------
				for (unsigned int boneIndex = 0;
					boneIndex < mesh->mNumBones;
					boneIndex++)
				{
					aiBone* bone = mesh->mBones[boneIndex];

					std::string boneName = bone->mName.C_Str();

					auto it = data.boneMap.find(boneName);

					if (it == data.boneMap.end())
						continue;

					uint32_t boneID = it->second.id;

					for (unsigned int weightIndex = 0;
						weightIndex < bone->mNumWeights;
						weightIndex++)
					{
						unsigned int vertexID =
							bone->mWeights[weightIndex].mVertexId;

						float weight =
							bone->mWeights[weightIndex].mWeight;

						AddBoneData(
							Mesh.vertices[vertexID],
							boneID,
							weight
						);
					}
				}


				// -------------------------
				// Indices
				// -------------------------
				for (unsigned int i = 0; i < mesh->mNumFaces; i++)
				{
					aiFace face = mesh->mFaces[i];

					for (unsigned int j = 0; j < face.mNumIndices; j++)
					{
						Mesh.indices.push_back(face.mIndices[j]);
					}
				}

				return Mesh;
			}
			static void load_model(aiNode* node, const aiScene* scene, std::shared_ptr<DATA::Node> currentNode, Data& data) {

				for (unsigned int i = 0; i < node->mNumMeshes; i++) {
					unsigned int meshIndex = node->mMeshes[i];
					aiMesh* mesh = scene->mMeshes[meshIndex];
					data.parts.push_back({load_mesh(mesh, scene, data),processMaterial(scene->mMaterials[mesh->mMaterialIndex])});
					//PrintMaterial(scene->mMaterials[mesh->mMaterialIndex]);
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

			static void AddBoneData(DATA::Vertex& vertex,int boneID,float weight)
			{
				for (int i = 0; i < 4; i++)
				{
					if (vertex.boneWeights[i] == 0.0f)
					{
						vertex.boneIDs[i] = boneID;
						vertex.boneWeights[i] = weight;
						return;
					}
				}
			}
			struct Materialdesc
			{
				std::unordered_map<std::string, DATA::Uniform> uniforms;
				std::unordered_map<std::string, DATA::TextureDesc> texturedesc;
			};
			static Materialdesc processMaterial(aiMaterial* material) {
				aiString path;
				Materialdesc matrialdesc;
				
				// استخراج بافت‌ها (اضافه کردن شرط براي جلوگيري از مسيرهاي اشتباه)
				if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS)
				{
					DATA::TextureDesc texturedesc; 
					texturedesc.isLinear = false;
					texturedesc.paths.push_back("Assets/" + std::string(path.C_Str()));
					matrialdesc.texturedesc.emplace("albedoTexture", texturedesc);
				}
				else {
					aiColor4D color;
					if (material->Get(AI_MATKEY_BASE_COLOR, color) == AI_SUCCESS)
					{
						matrialdesc.uniforms.emplace("color", ConvertVctor4(color));
					}
					else if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
					{
						matrialdesc.uniforms.emplace("color", ConvertVctor4(color));
					}
				}
				if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
				{
					DATA::TextureDesc texturedesc; 
					texturedesc.paths.push_back("Assets/" + std::string(path.C_Str())); 
					matrialdesc.texturedesc.emplace("normalTexture", texturedesc);
				}

				if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &path) == AI_SUCCESS)
				{
					DATA::TextureDesc texturedesc; 
					texturedesc.paths.push_back("Assets/" + std::string(path.C_Str())); 
					matrialdesc.texturedesc.emplace("metallicRoughnessTexture", texturedesc);
				}
				if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &path) == AI_SUCCESS)
				{
					DATA::TextureDesc texturedesc; 
					texturedesc.paths.push_back("Assets/" + std::string(path.C_Str()));
					matrialdesc.texturedesc.emplace("aoTexture", texturedesc);
				}

				if (material->GetTexture(aiTextureType_EMISSION_COLOR, 0, &path) == AI_SUCCESS)
				{
					DATA::TextureDesc texturedesc; 
					texturedesc.isLinear = false; 
					//texturedesc.paths.push_back("Assets/" + std::string(path.C_Str())); 
					//matrialdesc.texturedesc.emplace("emissiveTexture", texturedesc);
				}
				else {
					aiColor3D emissive;
					if (material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive) == AI_SUCCESS)
					{
						//matrialdesc.uniforms.emplace("emissiveColor",
						//	glm::vec3(emissive.r, emissive.g, emissive.b));
					}
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
				std::vector<DATA::AnimationData> animations;
				std::unordered_map<std::string, DATA::Bone> boneMap;
				glm::mat4 globalInverseTransform{ 1.0f };
			};

			

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
			static void PrintMaterial(aiMaterial* material) {
				for (int t = aiTextureType_NONE; t <= aiTextureType_TRANSMISSION; t++)
				{
					auto type = static_cast<aiTextureType>(t);

					unsigned int count = material->GetTextureCount(type);

					if (count == 0)
						continue;

					Warn(CORE::LogCategory::Resource, "TextureType ", t, '\n');

					for (unsigned int i = 0; i < count; i++)
					{
						aiString path;
						material->GetTexture(type, i, &path);

						std::cout << "    " << path.C_Str() << '\n';
						Warn(CORE::LogCategory::Resource, "    ", path.C_Str(), '\n');
					}
				}
			}
			Reader() = default;

		};
    }
}