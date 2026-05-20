#pragma once
#include "Object.h"
struct Cam_Data
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 position;
};
// --- Systems ---
class System {
public:
    System() {
        Manager = new EntityManager();
    }
    ~System() {
        delete Manager;
    }
    EntityManager* GetManager() {
        return Manager;
    }
    void Start() {
        for (auto& it : Manager->GetAllEntities()) {
            // چک کن که آیا این موجودیت کامپوننت های مورد نیاز را دارد؟
            for (auto& comp : it.second)
                comp->Start();

            TransformComponent* transform = Manager->GetComponent<TransformComponent>(it.first);
            CameraComponent* camera = Manager->GetComponent<CameraComponent>(it.first);
            LightComponent* light = Manager->GetComponent<LightComponent>(it.first);
            auto& graphic = transform->graphic;
            if (camera && transform) {
                Manager->GetComponent<CameraComponent>(it.first)->UBO_ID = graphic->createUBO(sizeof(Cam_Data));
                graphic->BindBuffer(camera->UBO_ID, 0, 0, sizeof(Cam_Data));
            }
            if (light && transform) {
                Manager->GetComponent<LightComponent>(it.first)->UBO_ID = graphic->createUBO(32);
                graphic->BindBuffer(light->UBO_ID, 1, 0, 32);
            }
        }
        glBindVertexArray(0); // Unbind VAO after loop
        glUseProgram(0);      // Unbind shader after loop
    }
    void Update(float dt) {
        for (auto& it : Manager->GetAllEntities()) {
            // چک کن که آیا این موجودیت کامپوننت های مورد نیاز را دارد؟
            for (auto& comp : it.second)
                comp->Update(dt);

            TransformComponent* transform = Manager->GetComponent<TransformComponent>(it.first);
            CameraComponent* camera = Manager->GetComponent<CameraComponent>(it.first);
            LightComponent* light = Manager->GetComponent<LightComponent>(it.first);
            auto& graphic = transform->graphic;
            if (camera && transform->isActive) {
                //Logger::WARN(to_string(camera->cameradata.projection[2].a));
               // Cam_Data data{ camera->cameradata.projection,camera->cameradata.view,transform->position };
                graphic->UpdateBuffer(camera->UBO_ID, glm::value_ptr(camera->cameradata.projection),0, sizeof(glm::mat4));
                graphic->UpdateBuffer(camera->UBO_ID, glm::value_ptr(camera->cameradata.view), sizeof(glm::mat4), sizeof(glm::mat4));
                graphic->UpdateBuffer(camera->UBO_ID, glm::value_ptr(transform->position), 2 * sizeof(glm::mat4), sizeof(glm::vec4));
            }
            if (light && transform->isActive) {
                graphic->UpdateBuffer(light->UBO_ID, glm::value_ptr(light->light.lightcolor), 0, sizeof(glm::vec4));
                graphic->UpdateBuffer(light->UBO_ID, glm::value_ptr(transform->position), sizeof(glm::vec4), sizeof(glm::vec4));
            }
        }
        Move(dt);
        Render();
    }
    void Render() {
        // برای هر موجودیت در دنیای بازی:
        for (auto& it : Manager->GetAllEntities()) {
            // چک کن که آیا این موجودیت کامپوننت های مورد نیاز را دارد؟
            TransformComponent* transform = Manager->GetComponent<TransformComponent>(it.first);
            MeshComponent* mesh = Manager->GetComponent<MeshComponent>(it.first);
            MaterialComponent* material = Manager->GetComponent<MaterialComponent>(it.first);

            if (transform->isActive && mesh && material) {
                mesh->draw();
            }
        }
        glBindVertexArray(0); // Unbind VAO after loop
        glUseProgram(0);      // Unbind shader after loop
    }
    void Move(float dt) {
        for (auto& it : Manager->GetAllEntities()) {
            // چک کن که آیا این موجودیت کامپوننت های مورد نیاز را دارد؟
            TransformComponent* transform = Manager->GetComponent<TransformComponent>(it.first);
            RigidBodyComponent* Body = Manager->GetComponent<RigidBodyComponent>(it.first);
            auto& graphic = transform->graphic;
            float velocity = 2 * dt;
            if (transform->isActive && Body) {
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
private:
    EntityManager* Manager;
};
