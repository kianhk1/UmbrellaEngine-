#pragma once
#include <memory>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine {
    namespace API {
        namespace Physics {

            class PhysicsBody {
            public:
                virtual ~PhysicsBody() = default;

                virtual void setPosition(const glm::vec3& pos) = 0;
                virtual glm::vec3 getPosition() const = 0;
                virtual void setRotation(const glm::quat& rot) = 0;
                virtual glm::quat getRotation() const = 0;
                virtual void setLinearVelocity(const glm::vec3& vel) = 0;
                virtual void applyCentralForce(const glm::vec3& force) = 0;
                //virtual void setMass(float mass) = 0;
            };

        } // namespace Physics
    }
} // namespace Engine