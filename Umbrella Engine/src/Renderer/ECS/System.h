#pragma once
#include "Entity.h"

// --- Systems ---
class System {
public:
    void Render() {
        // برای هر موجودیت در دنیای بازی:
        for (Entity* entity : GetAllEntities()) {
            // چک کن که آیا این موجودیت کامپوننت های مورد نیاز را دارد؟
            TransformComponent* transform = entity->GetComponent<TransformComponent>();
            MeshComponent* mesh = entity->GetComponent<MeshComponent>();
            ShaderComponent* shader = entity->GetComponent<ShaderComponent>();

            if (transform && mesh && shader) {
                mesh->draw();
            }
        }
        glBindVertexArray(0); // Unbind VAO after loop
        glUseProgram(0);      // Unbind shader after loop
    }

    Entity* AddEntity() {
        auto entity = std::make_shared<Entity>();
        entity->id = nextEntityID++;
        entities.push_back(std::move(entity));
        return entities.back().get();
    }
    Entity* AddEntity(Entity* entity) {
        entity->id = nextEntityID++;
        entities.push_back(std::make_shared<Entity>(*entity));
        return entities.back().get();
    }
private:
    std::vector<std::shared_ptr<Entity>> entities;
    int nextEntityID = 0;
    std::vector<Entity*> GetAllEntities() {
        std::vector<Entity*> result;
        for (auto& ent_ptr : entities) {
            result.push_back(ent_ptr.get());
        }
        return result;
    }
};
