#pragma once
#include <vector>
#include "System.h"

class RenderQueue
{
public:
	RenderQueue(std::vector<Entity*>& Entities,
				std::vector<SystemCallData>& Calldata) :
				entities(Entities), calldata(Calldata) {
		for (auto& call : calldata) {
			Entity* carrententyti = nullptr;
			for (auto& entity : entities) {
				if (entity->id == call.entityID) {
					carrententyti = entity;
					break;
				}
			}
			if(carrententyti){
				switch (call.call)
				{
				case SystemCalls::RENDER:
					
				default:
					break;
				}
			}
		}
	}

private:
	std::vector<Entity*> entities;
	std::vector<SystemCallData> calldata;
};
