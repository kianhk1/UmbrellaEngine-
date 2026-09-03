#pragma once
#include "AssetManager.h"
//template<typename T>
void Add(std::string name, Engine::DATA::ModelHandle& Mhandle, Engine::DATA::TextureHandle& Thandle) {
	Engine::AssetManager::GetInstance().GetModel(Mhandle)->
		GetMaterial(0).material.textures.emplace("skybox", Thandle);
}