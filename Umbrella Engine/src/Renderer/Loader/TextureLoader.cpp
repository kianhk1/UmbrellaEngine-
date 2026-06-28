#include "AssetManager.h"

namespace Engine {
    DATA::TextureHandle AssetManager::LoadTexture(const std::vector<std::string>& paths) {
        
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
        if (Engine::API::loadtexture2d(texture))
        {
            DATA::TextureHandle Handle(hash);
            textures.emplace(Handle.ID, texture);
            return Handle;
        }
        else
            return { 0 };
    }
    std::vector<DATA::TextureHandle> AssetManager::M_LoadTexture(const std::vector<std::string>& paths) {
        std::vector<DATA::TextureHandle> handles;
        for (auto& path : paths) {
            handles.push_back(LoadTexture(path));
        }
        return handles;
    }
    DATA::TextureHandle AssetManager::LoadTexture(const std::string& path) {

        uint64_t hash = CORE::Hash::FNV1a(path);
        auto it = textures.find(hash);
        if (it != textures.end())
            return it->first;
        auto texture = std::make_shared<DATA::TextureData>();
        texture->paths.push_back(path);
        if (!CORE::FileSystem::Exists(path))
        {
            Error(CORE::LogCategory::Resource, "File:" + path + "not exists");
            return { 0 };
        }
        if (Engine::API::loadtexture2d(texture))
        {
            DATA::TextureHandle Handle(hash);
            textures.emplace(Handle.ID, texture);
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

}