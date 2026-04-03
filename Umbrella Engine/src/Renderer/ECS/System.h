#pragma once
#include "Entity.h"
struct Cam_Data
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 position;
};
// --- Systems ---
class System {
public:
    void Start() {
        for (Entity* entity : GetAllEntities()) {
            // چک کن که آیا این موجودیت کامپوننت های مورد نیاز را دارد؟
            TransformComponent* transform = entity->GetComponent<TransformComponent>();
            CameraComponent* camera = entity->GetComponent<CameraComponent>();
            auto& graphic = camera->graphic;
            if (camera && transform) {

                entity->GetComponent<CameraComponent>()->UBO_ID = graphic->createUBO(sizeof(Cam_Data));
                Logger::WARN(to_string(camera->UBO_ID));
                graphic->BindBuffer(camera->UBO_ID, 0, 0, sizeof(Cam_Data));
            }
        }
        glBindVertexArray(0); // Unbind VAO after loop
        glUseProgram(0);      // Unbind shader after loop
    }
    void Update() {
        for (Entity* entity : GetAllEntities()) {
            // چک کن که آیا این موجودیت کامپوننت های مورد نیاز را دارد؟
            TransformComponent* transform = entity->GetComponent<TransformComponent>();
            CameraComponent* camera = entity->GetComponent<CameraComponent>();
            auto& graphic = camera->graphic;
            if (camera && transform) {
                //Logger::WARN(to_string(camera->cameradata.projection[2].a));
               // Cam_Data data{ camera->cameradata.projection,camera->cameradata.view,transform->position };
                graphic->UpdateBuffer(camera->UBO_ID, glm::value_ptr(camera->cameradata.projection),0, sizeof(glm::mat4));
                graphic->UpdateBuffer(camera->UBO_ID, glm::value_ptr(camera->cameradata.view), sizeof(glm::mat4), sizeof(glm::mat4));
                graphic->UpdateBuffer(camera->UBO_ID, glm::value_ptr(transform->position), 2 * sizeof(glm::mat4), sizeof(glm::vec4));
            }
        }
    }
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
    void Move(float dt) {
        for (Entity* entity : GetAllEntities()) {
            // چک کن که آیا این موجودیت کامپوننت های مورد نیاز را دارد؟
            TransformComponent* transform = entity->GetComponent<TransformComponent>();
            aa* aaa = entity->GetComponent<aa>();
            auto& graphic = transform->graphic;
            float velocity = 2 * dt;
            if (transform && aaa) {
                if (graphic->GetKey(GLFW_KEY_W, GLFW_PRESS)) {
                    transform->position += transform->forward * velocity;// جلو
                }
                if (graphic->GetKey(GLFW_KEY_S, GLFW_PRESS)) {
                    transform->position -= transform->forward * velocity;// عقب
                }
                if (graphic->GetKey(GLFW_KEY_A, GLFW_PRESS)) {
                    transform->position -= transform->right * velocity;
                    //transform->position.x -= transform->forward.x * velocity;// چپ               
                }
                if (graphic->GetKey(GLFW_KEY_D, GLFW_PRESS)) {
                    transform->position += transform->right * velocity;
                    //transform->position.x += transform->forward.x * velocity;// راست               
                }
                if (graphic->GetKey(GLFW_KEY_SPACE, GLFW_PRESS)) {
                    transform->position += transform->up * velocity;// بالا
                }
                if (graphic->GetKey(GLFW_KEY_LEFT_SHIFT, GLFW_PRESS)) {
                    transform->position -= transform->up * velocity;// پایین
                }
            }
        }
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
