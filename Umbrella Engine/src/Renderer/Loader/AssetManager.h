#pragma once
#include <memory>
#include <unordered_map>
#include "../../Core/Data/Data.h"
#include "../../Renderer/API/GraphicsAPI/GraphicsAPI.h"
#include "../../Core/Hash/HashMaker.h"
#include "../../Input & Output Manager/Input & Output File/FileSystem.h"

namespace Engine {
    enum class TextureType
    {
        Texture2D,
        Cubemap,
        HDR,
        TextureArray,
        Texture3D
    };
	class AssetManager {
	public:
        DATA::TextureHandle LoadCubeMap(const std::vector<std::string>& paths);
        DATA::TextureHandle LoadCubeMap(const std::string paths);
        std::vector<DATA::TextureHandle> M_LoadTexture(const std::vector<DATA::TextureDesc>& path);
        DATA::TextureHandle LoadTexture(const DATA::TextureDesc& path); 
        std::shared_ptr<DATA::TextureData> GetTexture(DATA::TextureHandle Handle);

        DATA::ShaderHandle LoadShader(
                const std::string& vertex,
                const std::string& fragment);
        std::shared_ptr<DATA::ShaderData> GetShader(DATA::ShaderHandle Handle);

        DATA::ModelHandle LoadModel(const std::string& path);
        std::shared_ptr<DATA::ModelData> GetModel(DATA::ModelHandle Handle);

        DATA::ScriptHandle LoadScript(const std::filesystem::path& path); 
        std::string GetScript(DATA::ScriptHandle Handle);

        static AssetManager& GetInstance() {
            static AssetManager instance;
            return instance;
        }
	private:
		AssetManager() = default;

        std::unordered_map<
            uint64_t,
            std::shared_ptr<DATA::TextureData>
        > textures;

        std::unordered_map<
            uint64_t,
            std::shared_ptr<DATA::ShaderData>
        > shaders;

        std::unordered_map<
            uint64_t,
            std::shared_ptr<DATA::ModelData>
        > models;

        std::unordered_map<
            uint64_t,
            std::string
        > Scripts; 

       static uint32_t NextTextureHandle;
    public:
        static json datat;
        static json datas;
        bool operator==(const AssetManager& other) const = default;
    };
}