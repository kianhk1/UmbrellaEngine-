#pragma once
#include <entt/entt.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "../Input & Output Manager/Input & Output File/FileSystem.h"
#include "../Renderer/ECS/Entity.h"
#include "../Renderer/Loader/AssetManager.h"


namespace Engine {
    namespace Scene {
        class Scene {
        public:
            Scene(std::string Name) : name(Name) {}
            Entity CreateEntity() {
                return registry.create();
            }
            void DestroyEntity(Entity entity) {
                registry.destroy(entity);
            }

            entt::registry& Registry() {
                return registry;
            }
            bool isActive = false;
            std::string GetName() const { return name; }
        private:
            entt::registry registry;
            std::string name;
            
        };
        class SceneManager {
        public:
            SceneManager() {
                
                std::ifstream filet("Assets/AssetDatabase/Textures.json");
                std::ifstream files("Assets/AssetDatabase/Shaders.json");
                if (filet.is_open() and files.is_open()) 
                {
                    filet >> textureDatabase;
                    files >> shaderDatabase;
                    //std::ifstream filet("Assets/Scene/Textures.json");
                    for (auto& path : CORE::FileSystem::GetFiles("Assets/Scene")) {
                        json data;
                        std::ifstream file(path);
                        if (!CORE::FileSystem::Exists(path))
                        {
                            Error(CORE::LogCategory::Resource, "File:", path, " not exists");
                            continue;
                        }
                        file >> data;
                        auto scene = Creat(data["Scene name:"]);
                        

                        if (data["isActive"].get<bool>())
                            Load(scene);
                        

                        auto& registry = scene->Registry(); 

                        for (auto& [id, entityJson] : data["entities"].items())
                        {
                            Entity entity = scene->CreateEntity();

                            for (auto& [name, component] : entityJson["component"].items())
                            {
                                if (name == "transform")
                                {
                                    TransformComponent t;

                                    auto& pos = component["position"];
                                    t.position = { pos[0], pos[1], pos[2] };

                                    auto& rot = component["rotation"];
                                    t.rotation = { rot[0], rot[1], rot[2] };

                                    auto& scale = component["scale"];
                                    t.scale = { scale[0], scale[1], scale[2] };

                                    auto& forward = component["forward"];
                                    t.forward = { forward[0], forward[1], forward[2] };

                                    registry.emplace<TransformComponent>(entity, t);
                                }
                                if (name == "MeshRenderer")
                                {
                                    MeshRendererComponent m;

                                    // ۱. بررسی وجود کلید modelpath
                                    if (component.contains("modelpath")) {
                                        m.modelhandle = AssetManager::GetInstance().LoadModel(component["modelpath"]);
                                    }

                                    // ۲. بارگذاری صحیح بافت‌ها
                                    if (component.contains("Texture") && !component["Texture"].empty()) {
                                        auto model = Engine::AssetManager::GetInstance().GetModel(m.modelhandle);
                                        if (model) {
                                            model->GetMaterial(0).material.textures.emplace(
                                                "skybox",
                                                Engine::AssetManager::GetInstance().LoadCubeMap(component["Texture"][0].get<std::string>())
                                            );
                                        }
                                    }

                                    // ۳. خواندن امن شیدرها
                                    if (component.contains("shader") && component["shader"].size() >= 2) {
                                        m.shader = Engine::AssetManager::GetInstance().LoadShader(
                                            component["shader"][0].get<std::string>(),
                                            component["shader"][1].get<std::string>()
                                        );
                                    }

                                    // ۴. خواندن صحیح تنظیمات Render State
                                    if (component.contains("stat")) {
                                        auto& stat = component["stat"];
                                        m.state.depthfunc = stat.value("depthfunc", 0);
                                        m.state.shadow = stat.value("shadow", false);
                                        m.state.depthtest = stat.value("depthtest", true);
                                    }

                                    registry.emplace<MeshRendererComponent>(entity, m);
                                }
                                if (name == "Camera")
                                {
                                    CameraComponent c;
                                    int i = 0;
                                    for (auto& d : component["cameraTarget"])
                                        c.cameradata.cameraTarget[i++] = d;
                                    c.cameradata.fov = component["fov"];
                                    c.firstMouse = component["firstMouse"];
                                    c.ismoving = component["ismoving"];
                                    c.type = component["cameratype"];
                                    registry.emplace<CameraComponent>(entity, c);
                                }
                                if (name == "Light")
                                {
                                    LightComponent l;
                                    int i = 0;
                                    for (auto& d : component["lightcolor"])
                                        l.light.lightcolor[i++] = d;
                                    l.type = component["lighttype"];
                                    registry.emplace<LightComponent>(entity, l);
                                }
                            }
                        }
                    }
                }
            }
            void Load(std::shared_ptr<Scene> scene) {
                scene->isActive = true;
                ActiveScene = scene;
            }
            bool Save() {
                return Save(ActiveScene); 
            }
            bool Save(std::shared_ptr<Scene> scene) {
                json data;
                data["Scene name:"] = scene->GetName(); 
                data["isActive"] = scene->isActive;
                auto& registry = scene->Registry(); 

                auto transformview = registry.view<TransformComponent>();
                transformview.each([&](auto entity,
                    TransformComponent& transform) {
                        data["entities"][to_string(entt::to_integral(entity))]["component"]["transform"] = {
                            {"position", {transform.position.x, transform.position.y, transform.position.z}},
                            {"rotation", {transform.rotation.x, transform.rotation.y, transform.rotation.z}},
                            {"scale", {transform.scale.x, transform.scale.y, transform.scale.z}},
                            {"forward", {transform.forward.x, transform.forward.y, transform.forward.z}} };

                    });

                auto Rendererview = registry.view<MeshRendererComponent>();
                Rendererview.each([&](auto entity, MeshRendererComponent& Renderer) {
                    auto model = AssetManager::GetInstance().GetModel(Renderer.modelhandle);

                    // ساختار اصلاح شده
                    data["entities"][to_string(entt::to_integral(entity))]["component"]["MeshRenderer"] = {
                        {"modelpath", model->path}, // کلمه paht اصلاح شد
                        {"stat", {
                            {"depthtest", Renderer.state.depthtest},
                            {"shadow", Renderer.state.shadow},
                            {"depthfunc", Renderer.state.depthfunc}
                        }}
                    };

                    auto& shaders = shaderDatabase["shader"][to_string(Renderer.shader.ID)];
                    for (auto& shader : shaders)
                    {
                        data["entities"][std::to_string(entt::to_integral(entity))]
                            ["component"]["MeshRenderer"]["shader"]
                            .push_back(shader);
                    }
                    // ذخیره صحیح بافت‌های 2D و CubeMap (حذف and false)
                    for (auto& part : model->parts) {
                        for (auto& texture : part.material.textures) {
                            auto id = std::to_string(texture.second.ID);

                            if (textureDatabase["Texture"]["CubeMap"].contains(id)) {
                                data["entities"][to_string(entt::to_integral(entity))]["component"]["MeshRenderer"]["Texture"]
                                    .push_back(textureDatabase["Texture"]["CubeMap"][id]);
                            }
                            if (textureDatabase["Texture"]["2D"].contains(id) and false) { // شرط false برداشته شد
                                data["entities"][to_string(entt::to_integral(entity))]["component"]["MeshRenderer"]["Texture"]
                                    .push_back(textureDatabase["Texture"]["2D"][id]);
                            }
                        }
                    }
                    });

                auto Cameraview = registry.view<CameraComponent>();
                Cameraview.each([&](auto entity,
                    CameraComponent& Camera) {
                        data["entities"][to_string(entt::to_integral(entity))]["component"]["Camera"] = {
                            {"cameraTarget", {Camera.cameradata.cameraTarget.x,Camera.cameradata.cameraTarget.y,Camera.cameradata.cameraTarget.z}},
                            {"cameratype", Camera.type},
                            {"firstMouse", Camera.firstMouse},
                            {"ismoving", Camera.ismoving},
                            {"fov", Camera.cameradata.fov} };
                    });

                auto Lightview = registry.view<LightComponent>();
                Lightview.each([&](auto entity,
                    LightComponent& Light) {
                        data["entities"][to_string(entt::to_integral(entity))]["component"]["Light"] = {
                            {"lightcolor", {Light.light.lightcolor.x,Light.light.lightcolor.y,Light.light.lightcolor.z}},
                            {"lighttype", Light.type} };
                    });

                std::ofstream file("Assets/Scene/" + scene->GetName() + ".json");

                if (!file.is_open())
                    return false;

                file << data.dump(4);

                return true;
            }

            std::shared_ptr<Scene> Creat(std::string name) {
                for (auto& scene : Scenes) {
                    if (scene->GetName() == name)
                        Load(scene);
                        return scene;
                }
                std::shared_ptr<Scene> scene = std::make_shared<Scene>(name);
                ActiveScene = scene; 
                Scenes.push_back(scene); 
                return scene;
            }

            std::shared_ptr<Scene> GetActivescene() {
                return ActiveScene;
            }

            static SceneManager& GetInstance() {
                static SceneManager scenemanager;
                return scenemanager;
            }

        private:
            std::vector <std::shared_ptr<Scene>> Scenes;
            std::shared_ptr<Scene> ActiveScene;
            json textureDatabase;
            json shaderDatabase;
        };
    }
}
