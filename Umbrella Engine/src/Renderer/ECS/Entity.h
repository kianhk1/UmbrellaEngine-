#pragma once
#include "../API/GraphicsAPI/openglGraphicAPI.h"
#include "Component.h"
#include <map>
#include <unordered_map>
using Entity = uint32_t;
class EntityManager
{
public:
	// Name
	static Entity entityId;
protected:
	// State
	//static int count;
	std::unordered_map<Entity, std::vector<Component*>> entityComponents;
	bool isVisible = true;
	bool isActive = true;
public:
	EntityManager() {}
	~EntityManager() {
		for (auto& [id, components] : entityComponents) {
			for (auto* comp : components) {
				delete comp;  // آزادسازی حافظه
			}
		}
		entityComponents.clear();
	}
	Entity AddEntity() {
		entityComponents[++entityId];
		return entityId;
	}

	void AddComponent(Component* comp, Entity entity) {
		entityComponents[entity].push_back(comp);
	}
	template<typename T>
	T* GetComponent(Entity entity) {
		for (auto c : entityComponents[entity]) {
			if (T* specificComp = dynamic_cast<T*>(c))
				return specificComp;
		}
		return nullptr;
	}
	std::unordered_map<Entity, std::vector<Component*>>& GetAllEntities() {
		return entityComponents;
	}
	void ClearAllComponent(Entity entity) {
		entityComponents[entity].clear();
	}
};
Entity EntityManager::entityId = 0;
