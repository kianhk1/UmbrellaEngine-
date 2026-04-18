#pragma once
#include "../../../Libraries/btBulletDynamicsCommon.h"
#include "../API/PhysicsWorldAPI.h"
#include "../BuletAPI/BulletPhysicsBody.h"
#include <vector>

namespace Engine {
    namespace API {
        namespace Physics {
            class BulletPhysicsWorld : public PhysicsWorld {
            public:
                std::unique_ptr<btCollisionConfiguration> collisionConfig;
                std::unique_ptr<btDispatcher> dispatcher;
                std::unique_ptr<btBroadphaseInterface> broadphase;
                std::unique_ptr<btConstraintSolver> solver;
                std::unique_ptr<btDiscreteDynamicsWorld> world;

                std::vector<std::unique_ptr<BulletPhysicsBody>> bodies;

                BulletPhysicsWorld(DATA::WorldConfig& Config) : PhysicsWorld(Config) {
                    //collisionConfig = new btDefaultCollisionConfiguration();
                    collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
                    //dispatcher = new btCollisionDispatcher(collisionConfig);
                    dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfig.get());
                    //broadphase = new btDbvtBroadphase();
                    broadphase = std::make_unique<btDbvtBroadphase>();
                    //solver = new btSequentialImpulseConstraintSolver();
                    solver = std::make_unique<btSequentialImpulseConstraintSolver>();
                    
                }
                void makeword() override {
                    //world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
                    world = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), broadphase.get(), solver.get(), collisionConfig.get());

                    world->setGravity(btVector3(config.gravity.x, config.gravity.y, config.gravity.z));
                }
                void update(float deltaTime) override {
                    world->stepSimulation(deltaTime, config.maxSubSteps, config.fixedTimeStep);
                }

                PhysicsBody* createBody(BodyType type, const CollisionShape& shape,
                    const glm::vec3& position, float mass) override {
                    auto body = std::make_unique<BulletPhysicsBody>(type, shape, position, mass);
                    world->addRigidBody(body->getRigidBody());
                    bodies.push_back(std::move(body));
                    return bodies.back().get();
                }

                void destroyBody(PhysicsBody* body) override {
                    auto it = std::find_if(bodies.begin(), bodies.end(),
                        [body](const auto& b) { return b.get() == body; });
                    if (it != bodies.end()) {
                        world->removeRigidBody((*it)->getRigidBody());
                        bodies.erase(it);
                    }
                }
                void setGravity(const glm::vec3& gravity) override {
                    config.gravity = gravity;
                }
                glm::vec3 getGravity() const override {
                    return config.gravity;
                }
                ~BulletPhysicsWorld() {
                    //delete world;
                    //delete solver;
                    //delete broadphase;
                    //delete dispatcher;
                    //delete collisionConfig;
                }
            };
        }
    }
}