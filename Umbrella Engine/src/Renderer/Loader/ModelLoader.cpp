#include "AssetManager.h"
#include "../../Input & Output Manager/Input & Output File/ReadFile.h"

namespace Engine {
	DATA::ModelHandle AssetManager::LoadModel(const std::string& path) {
        uint64_t hash = CORE::Hash::FNV1a(path);
        auto it = models.find(hash); 
        if (it != models.end()) 
            return it->first;
        auto model = std::make_shared<DATA::ModelData>();
        if (!CORE::FileSystem::Exists(path)) {
            Error(CORE::LogCategory::Resource, "File:" , path , "not exists");
            return NULL;
        }
        auto data = CORE::Reader::ReadModel(path);
        model->path = path;
        model->root = data.root;
        for (auto& part : data.parts) {
            DATA::MaterialData material;
            for (auto& texturedesc : part.materialdesc.texturedesc)
            {
                material.textures.emplace(texturedesc.first, LoadTexture(texturedesc.second));
            }
                
            material.uniforms = part.materialdesc.uniforms;
                
            DATA::MeshData mesh;
            mesh.vertices = part.mesh.vertices;
            mesh.indices = part.mesh.indices;
            model->parts.push_back({ mesh,material });
        }
        DATA::ModelHandle Handle(hash);
        models.emplace(Handle.ID, model);
        return Handle;
	}
    std::shared_ptr<DATA::ModelData> AssetManager::GetModel(DATA::ModelHandle Handle) {
        auto it = models.find(Handle.ID);
        if (!(it == models.end()))
            return it->second;
    }
}