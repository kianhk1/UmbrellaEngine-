#pragma once
//#include "../Core/Log Managment/Logger.h"
//#include "../Renderer/ECS/Entity.h"


class Script
{
public:
    virtual ~Script() = default;

    virtual void OnCreate() = 0;
    virtual void OnStart() = 0; 
    virtual void OnUpdate(float dt) = 0;
    virtual void OnDestroy() = 0;
protected:
    //Entity entity;
};