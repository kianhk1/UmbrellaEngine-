#include "AssetManager.h"
#include "../../Input & Output Manager/Input & Output File/ReadFile.h"

namespace Engine {
    DATA::ScriptHandle Engine::AssetManager::LoadScript(const std::filesystem::path& path)
    {
        uint64_t hash = CORE::Hash::FNV1a(path.string());
        auto it = Scripts.find(hash);
        if (it != Scripts.end())
            return it->first;

        if (!CORE::FileSystem::Exists(path))
        {
            Error(CORE::LogCategory::Resource, "File:" , path , "not exists");
            return NULL;
        }

        if (CORE::Reader::Compile(path.string(), "Assets/Script/dll/" + path.stem().string() + ".dll"))
        {
            DATA::ScriptHandle Handle(hash);
            Scripts.emplace(Handle.ID, "Assets/" + path.stem().string() + ".dll");
            //datas["shader"][std::to_string(hash)] = { vertex ,fragment };
            //std::ofstream filet("Assets/AssetDatabase/Shaders.json");
            //filet << datas.dump(4);
            return Handle;
        }
        else
        {
            DATA::ScriptHandle Handle(0);
            return Handle;
        }
    }

    std::string Engine::AssetManager::GetScript(DATA::ScriptHandle Handle) {
        auto it = Scripts.find(Handle.ID);
        if (!(it == Scripts.end()))
            return it->second; 
        return nullptr;
    }



    //json AssetManager::datas; 
}