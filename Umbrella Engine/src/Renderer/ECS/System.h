#pragma once
#include "Entity.h"

enum class SystemCalls
{
	RENDER
};

struct SystemCallData
{
	SystemCalls call;
	int entityID;
};

class System
{
public:

	static SystemCallData SystemCall(Entity* entity) {
		if (entity->GetComponent<ShaderComponent>() &&
			entity->GetComponent<MeshComponent>() &&
			entity->GetComponent<TransformComponent>()) {
			return { SystemCalls::RENDER, entity->id };
		}
	}
	static void RenderSystem(Entity* entity) {
		entity->GetComponent<MeshComponent>()->draw();
	}

private:

};
