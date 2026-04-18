#pragma once
#include "../../../Libraries/btBulletDynamicsCommon.h"
#include "../API/PhysicsBody.h"
enum class BodyType { Static, Dynamic, Kinematic };
namespace Engine {
    namespace API {
        namespace Physics {

            btVector3 toBtVector3(const glm::vec3& v) {
                return btVector3(v.x, v.y, v.z);
            }

            glm::vec3 toGlmVec3(const btVector3& v) {
                return glm::vec3(v.x(), v.y(), v.z());
            }

            btQuaternion toBtQuat(const glm::quat& q) {
                return btQuaternion(q.x, q.y, q.z, q.w);
            }

            glm::quat toGlmQuat(const btQuaternion& q) {
                return glm::quat(q.w(), q.x(), q.y(), q.z());
            }

            // BulletCollisionShape.h (داخلی)
            class BulletBoxShape : public CollisionShape {
                btBoxShape* m_shape;
            public:
                BulletBoxShape(const glm::vec3& halfExtents) {
                    m_shape = new btBoxShape(toBtVector3(halfExtents));
                }
                void* getInternalShape() const override { return m_shape; }
                ~BulletBoxShape() { delete m_shape; }
            };


            class BulletPhysicsBody : public PhysicsBody {
            public:
                BulletPhysicsBody(BodyType type, const CollisionShape& shape,
                    const glm::vec3& position, float mass)
                    : m_mass(mass), m_type(type), m_rigidBody(nullptr), m_motionState(nullptr), m_shape(nullptr) {

                    // گرفتن shape واقعی Bullet از لایه انتزاعی shape
                    btCollisionShape* bulletShape = static_cast<btCollisionShape*>(shape.getInternalShape());
                    m_shape = bulletShape;

                    btVector3 startPos = toBtVector3(position);
                    btQuaternion startRot = btQuaternion::getIdentity();

                    // ایجاد motion state
                    btTransform startTransform;
                    startTransform.setIdentity();
                    startTransform.setOrigin(startPos);
                    startTransform.setRotation(startRot);

                    m_motionState = new btDefaultMotionState(startTransform);

                    // محاسبه اینرسی
                    btVector3 localInertia(0, 0, 0);
                    if (type == BodyType::Dynamic && mass > 0.0f) {
                        bulletShape->calculateLocalInertia(mass, localInertia);
                    }

                    // ساخت rigid body
                    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
                        static_cast<btMotionState*>(m_motionState),
                        bulletShape, localInertia);

                    m_rigidBody = new btRigidBody(rbInfo);

                    // تنظیم نوع بدنه
                    if (type == BodyType::Static) {
                        static_cast<btRigidBody*>(m_rigidBody)->setCollisionFlags(
                            static_cast<btRigidBody*>(m_rigidBody)->getCollisionFlags() |
                            btCollisionObject::CF_STATIC_OBJECT);
                    }
                    else if (type == BodyType::Kinematic) {
                        static_cast<btRigidBody*>(m_rigidBody)->setCollisionFlags(
                            static_cast<btRigidBody*>(m_rigidBody)->getCollisionFlags() |
                            btCollisionObject::CF_KINEMATIC_OBJECT);
                        static_cast<btRigidBody*>(m_rigidBody)->setActivationState(DISABLE_DEACTIVATION);
                    }
                }
                ~BulletPhysicsBody() override {
                    if (m_rigidBody) {
                        delete static_cast<btRigidBody*>(m_rigidBody);
                        m_rigidBody = nullptr;
                    }
                    if (m_motionState) {
                        delete static_cast<btMotionState*>(m_motionState);
                        m_motionState = nullptr;
                    }
                }

                void setPosition(const glm::vec3& pos) override {
                    if (!m_rigidBody) return;
                    btTransform trans = static_cast<btRigidBody*>(m_rigidBody)->getWorldTransform();
                    trans.setOrigin(toBtVector3(pos));
                    static_cast<btRigidBody*>(m_rigidBody)->setWorldTransform(trans);

                    // برای kinematic bodies باید motion state را هم آپدیت کنیم
                    if (m_type == BodyType::Kinematic && m_motionState) {
                        static_cast<btMotionState*>(m_motionState)->setWorldTransform(trans);
                    }
                }
                glm::vec3 getPosition() const override {
                    if (!m_rigidBody) return glm::vec3(0);
                    btTransform trans = static_cast<btRigidBody*>(m_rigidBody)->getWorldTransform();
                    return toGlmVec3(trans.getOrigin());
                }
                void setRotation(const glm::quat& rot) override {
                    if (!m_rigidBody) return;
                    btTransform trans = static_cast<btRigidBody*>(m_rigidBody)->getWorldTransform();
                    trans.setRotation(toBtQuat(rot));
                    static_cast<btRigidBody*>(m_rigidBody)->setWorldTransform(trans);
                }
                glm::quat getRotation() const override {
                    if (!m_rigidBody) return glm::quat(1, 0, 0, 0);
                    btTransform trans = static_cast<btRigidBody*>(m_rigidBody)->getWorldTransform();
                    return toGlmQuat(trans.getRotation());
                }
                void setLinearVelocity(const glm::vec3& vel) override {
                    if (!m_rigidBody || m_type == BodyType::Static) return;
                    static_cast<btRigidBody*>(m_rigidBody)->setLinearVelocity(toBtVector3(vel));
                }
                /*void setMass(float mass) override {
                    if (!m_rigidBody || m_type != BodyType::Dynamic) return;
                    m_mass = mass;
                    btVector3 localInertia(0, 0, 0);

                    // محاسبه اینرسی جدید
                    static_cast<btCollisionShape*>(m_shape)->calculateLocalInertia(mass, localInertia);

                    btRigidBody* body = static_cast<btRigidBody*>(m_rigidBody);

                    // روش مستقیم: تغییر جرم و اینرسی با استفاده از توابع عمومی Bullet
                    body->setMassProps(mass, localInertia);

                    // گاهی اوقات نیاز به بازنشانی وضعیت حرکت داره
                    if (!body->isStaticOrKinematicObject()) {
                        body->activate();
                    }
                }*/
               
                void applyCentralForce(const glm::vec3& force) override {
                    if (!m_rigidBody || m_type != BodyType::Dynamic) return;
                    static_cast<btRigidBody*>(m_rigidBody)->applyCentralForce(toBtVector3(force));
                }

                btRigidBody* getRigidBody() { return m_rigidBody; }

            private:
                btRigidBody* m_rigidBody;
                btMotionState* m_motionState;
                btCollisionShape* m_shape; // shape از Bullet
                BodyType m_type;
                float m_mass;
            };
        } // namespace Physics
    }
} // namespace Engine