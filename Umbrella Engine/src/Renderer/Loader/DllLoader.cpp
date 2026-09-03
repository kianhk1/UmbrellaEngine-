#include "AssetManager.h"
#include <Windows.h>


namespace Engine {
	std::shared_ptr<Script> Engine::AssetManager::LoadDLL(DATA::ScriptHandle Handle)
	{
		auto it = dlls.find(Handle.ID);
		if (it != dlls.end())
			return it->second.script; 

		HMODULE module = LoadLibraryA(GetScript(Handle).c_str());
		if (!module)
		{
			std::cout << "LoadLibrary failed!\n";
			std::cout << GetScript(Handle) << "\n";

			std::cout << "Error: "
				<< GetLastError()
				<< "\n";
			//Error(CORE::LogCategory::Resource, "module:", Handle.ID, "not exists"); 
			return nullptr;
		}
		using CreateScriptFn = Script * (*)();
		using DestroyScriptFn = void (*)(Script*);

		auto createScript =
			reinterpret_cast<CreateScriptFn>(
				GetProcAddress(module, "CreateScript")
				);

		auto destroyScript =
			reinterpret_cast<DestroyScriptFn>(
				GetProcAddress(module, "DestroyScript")
				);

		if (!createScript || !destroyScript)
		{
			// failed
			return nullptr;
		}

		std::shared_ptr<Script> script(
			createScript(),
			destroyScript
		);
		script->OnCreate();
		dlls.emplace(Handle.ID, LoadedDLL{ reinterpret_cast<void*>(module), script }); 
		
		return script;
	}
	void AssetManager::UnloadDLL(DATA::ScriptHandle handle)
	{
		auto it = dlls.find(handle.ID);

		if (it == dlls.end())
			return;

		it->second.script->OnDestroy();

		it->second.script.reset(); 

		FreeLibrary(reinterpret_cast<HMODULE>(it->second.module));

		dlls.erase(it); 
	}
}