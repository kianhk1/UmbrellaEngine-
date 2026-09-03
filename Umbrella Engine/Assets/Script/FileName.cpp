#include "../../src/Script/Script.h"
#include <iostream>

class kir : public Script
{
public:
    void OnCreate() override {
        std::cout << "ccccccccccccccckiiiiiiiiiirrrr\n";
    }
    void OnStart() override {
        std::cout << "sssssssssssskiiiiiiiiiirrrr\n";
    }
    void OnUpdate(float dt) override {

    }
    void OnDestroy() override {

    }
};

extern "C" __declspec(dllexport)
Script* CreateScript()
{
    return new kir();
}

extern "C" __declspec(dllexport)
void DestroyScript(Script* script)
{
    delete script;
}