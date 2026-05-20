#include "Libraries/physx/PxPhysicsAPI.h"
#include <iostream>

using namespace physx;

// کلاسی برای مدیریت خطاهای موتور
class SimpleErrorCallback : public PxErrorCallback {
    void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) override {
        std::cerr << "Error: " << message << std::endl;
    }
};

int main() {
    // 1. ایجاد Foundation (مدیریت حافظه و خطا)
    SimpleErrorCallback errorCallback;
    PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION,
        PxDefaultAllocator(),
        errorCallback);
    if (!foundation) return -1;

    // 2. ساخت شیء Physics (ورژن 4.0.0 از PX_PHYSICS_VERSION استفاده می‌کند)
    PxPhysics* physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation,
        PxTolerancesScale(), false);
    if (!physics) return -1;

    // 3. ایجاد صحنه (Scene) برای شبیه‌سازی
    PxSceneDesc sceneDesc(physics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // گرانش زمین
    sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1); // استفاده از ۱ هسته
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    PxScene* scene = physics->createScene(sceneDesc);
    if (!scene) return -1;

    std::cout << "موتور PhysX با موفقیت راه‌اندازی شد!" << std::endl;

    // پاکسازی حافظه
    scene->release();
    physics->release();
    foundation->release();
    return 0;
}