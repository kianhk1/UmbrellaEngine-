#pragma once
#include <memory>
#include <glm/vec3.hpp> // فرض بر استفاده از glm در موتور شما
namespace DATA {
    struct WorldConfig {
        glm::vec3 gravity = glm::vec3(0, -9.81f, 0);
        float fixedTimeStep = 1.0f / 60.0f;
        int maxSubSteps = 4;
    };
}
namespace Engine {
    
    namespace API {
        namespace Physics {

            class PhysicsBody;
            // CollisionShape.h (عمومی)
            class CollisionShape {
            public:
                virtual ~CollisionShape() = default;
                virtual void* getInternalShape() const = 0;  // بازگرداندن void* به جای btCollisionShape*
            };

            enum class BodyType { Static, Dynamic, Kinematic };

            class PhysicsWorld {
            public:
                PhysicsWorld(const DATA::WorldConfig& Config) : config(Config) {}
                ~PhysicsWorld() = default;

                // جلوگیری از کپی
                PhysicsWorld(const PhysicsWorld&) = delete;
                PhysicsWorld& operator=(const PhysicsWorld&) = delete;

                 virtual void makeword() = 0;
                 virtual void update(float deltaTime) =0;

                // مدیریت بدنه‌ها
                 virtual PhysicsBody* createBody(BodyType type, const CollisionShape& shape,
                    const glm::vec3& position, float mass) = 0;
                 virtual void destroyBody(PhysicsBody* body) = 0;

                 virtual void setGravity(const glm::vec3& gravity) = 0;
                 virtual glm::vec3 getGravity() const = 0;

            protected:
                DATA::WorldConfig config;
            };

        } // namespace Physics
    } // namespace Physics
} // namespace Engine