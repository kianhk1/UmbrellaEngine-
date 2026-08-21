#include "AssetManager.h"

namespace Engine {
    uint32_t AssetManager::NextTextureHandle = 5;
    DATA::TextureHandle AssetManager::LoadCubeMap(const std::vector<std::string>& paths) {
        uint64_t hash = CORE::Hash::FNV1a(paths);
        auto it = textures.find(hash);
        if (it != textures.end())
            return it->first;
        auto texture = std::make_shared<DATA::TextureData>();
        texture->paths = paths;
        for (auto& path : paths)
        {
            if (!CORE::FileSystem::Exists(path))
            {
                Error(CORE::LogCategory::Resource, "File:" + path + "not exists");
                return { 0 };
            }
        }
        if (Engine::API::loadcubemap(texture))
        {
            DATA::TextureHandle Handle(hash);
            textures.emplace(Handle.ID, texture);

            for(auto& path : paths)
            {
                datat["Texture"]["CubeMap"][std::to_string(hash)] = path;
            }

            std::ofstream file("Assets/AssetDatabase/Textures.json");
            Warn(Engine::CORE::LogCategory::Resource, file.is_open() ? "yes" : "no");
            file << datat.dump(4);
            return Handle;
        }
        else
            return { 0 };
    }
    DATA::TextureHandle AssetManager::LoadCubeMap(const std::string paths) {
        json data;
        std::ifstream file(paths);
        if (!CORE::FileSystem::Exists(paths))
        {
            Error(CORE::LogCategory::Resource, "File:" , paths , " not exists");
            return { 0 };
        }

        uint64_t hash = CORE::Hash::FNV1a(paths);
        auto it = textures.find(hash);
        if (it != textures.end())
            return it->first;

        auto texturedata = std::make_shared<DATA::TextureData>();

        file >> data;
        //texture->paths = paths;
        for (const auto& texture : data["textures"])
        {
            if (!CORE::FileSystem::Exists("Assets/" + texture.get<std::string>()))
            {
                Error(CORE::LogCategory::Resource, "File:" , "Assets/" , texture.get<std::string>() , "not exists");
                //return { 0 };
            }
            else
                texturedata->paths.push_back("Assets/" + texture.get<std::string>());
        }
        
        if (Engine::API::loadcubemap(texturedata))
        {
            DATA::TextureHandle Handle(hash);
            textures.emplace(Handle.ID, texturedata);

            datat["Texture"]["CubeMap"][std::to_string(hash)] = paths; 
            std::ofstream filet("Assets/AssetDatabase/Textures.json");
            Warn(Engine::CORE::LogCategory::Resource, filet.is_open() ? "yes" : "no"); 
            filet << datat.dump(4); 
            return Handle;
        }
        else
            return { 0 };
    }
    std::vector<DATA::TextureHandle> AssetManager::M_LoadTexture(const std::vector<DATA::TextureDesc>& paths) {
        std::vector<DATA::TextureHandle> handles;
        for (auto& path : paths) {
            handles.push_back(LoadTexture(path));
        }
        return handles;
    }
    DATA::TextureHandle AssetManager::LoadTexture(const DATA::TextureDesc& path) {

        uint64_t hash = CORE::Hash::FNV1a(path.paths);
        auto it = textures.find(hash);
        if (it != textures.end())
            return it->first;
        auto texture = std::make_shared<DATA::TextureData>();
        texture->paths.push_back(path.paths[0]);
        texture->isLinear = path.isLinear;
        if (!CORE::FileSystem::Exists(path.paths[0]))
        {
            Error(CORE::LogCategory::Resource, "File:" + path.paths[0] + "not exists");
            return { 0 };
        }
        if (Engine::API::loadtexture2d(texture))
        {
            texture->unit = NextTextureHandle++;
            DATA::TextureHandle Handle(hash);
            textures.emplace(Handle.ID, texture);

            for (auto& path : path.paths)
            {
                datat["Texture"]["2D"][std::to_string(hash)] = path; 
            }

            std::ofstream file("Assets/AssetDatabase/Textures.json"); 
            Warn(Engine::CORE::LogCategory::Resource, file.is_open() ? "yes" : "no"); 
            file << datat.dump(4); 

            return Handle;
        }
        else
            return { 0 };
    }

    std::shared_ptr<DATA::TextureData> AssetManager::GetTexture(DATA::TextureHandle Handle) {
        auto it = textures.find(Handle.ID);
        if (it != textures.end())
            return it->second;
        return nullptr;
    }
    json AssetManager::datat;
}