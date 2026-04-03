#pragma once
#include "../API/GraphicsAPI/openglGraphicAPI.h"
#include "Component.h"
class Entity
{
public:
	// Name
	int id = 0;
	std::string name = "";

	std::vector<Component*> components;
protected:
	// State
	//static int count;
	bool isVisible = true;
	bool isActive = true;
public:
	Entity(){}
	~Entity() {
		for (auto c : components) delete c;
	}

	virtual void Start() {
		for (auto& c : components)
			if (c->isActive)
				c->Start();
	}

	virtual void Update(float dt) {
		if (!isActive) return;

		// بعد Tick بقیه Componentها
		for (auto c : components) {
			if (c->isActive) c->Update(dt);
			//Logger::ERROR("kttryrdrdhyrd");
		}
	}

	void AddComponent(Component* comp) {
		components.push_back(comp);
	}
	template<typename T>
	T* GetComponent() {
		for (auto c : components) {
			if (T* specificComp = dynamic_cast<T*>(c))
				return specificComp;
		}
		return nullptr;
	}
};

