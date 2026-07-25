#include "AssetManager.h"

namespace Engine {
	DATA::ShaderHandle Engine::AssetManager::LoadShader(const std::string& vertex, const std::string& fragment)
	{
        uint64_t hash = CORE::Hash::FNV1a(vertex + "|" + fragment);
        auto it = textures.find(hash);
        if (it != textures.end())
            return it->first;
        auto shader = std::make_shared<DATA::ShaderData>();
        for (auto& path : { vertex,fragment })
        {
            if (!CORE::FileSystem::Exists(path))
            {
                Error(CORE::LogCategory::Resource, "File:" + path + "not exists");
                return NULL;
            }
        }
        shader->programID = Engine::API::createShader();
        unsigned int vs = Engine::API::compileShader(GL_VERTEX_SHADER, CORE::FileSystem::ReadTextFile(vertex));
        unsigned int fs = Engine::API::compileShader(GL_FRAGMENT_SHADER, CORE::FileSystem::ReadTextFile(fragment));
        if (Engine::API::linkShaderProgram(shader->programID, vs, fs))
        {
            DATA::ShaderHandle Handle(hash);
            shaders.emplace(Handle.ID, shader);
            Engine::API::useShader(*shader);
            return Handle;
        }
        else
            return NULL;
	}

    std::shared_ptr<DATA::ShaderData> AssetManager::GetShader(DATA::ShaderHandle Handle) {
        auto it = shaders.find(Handle.ID);
        if (!(it == shaders.end()))
            return it->second;
        return nullptr;
    }
}