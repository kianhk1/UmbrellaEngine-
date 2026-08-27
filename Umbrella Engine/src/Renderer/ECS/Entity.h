#pragma once
#include <entt/entt.hpp>
#include "Component.h"
#include <string>

using Entity = entt::entity;

namespace entt{
    class Registry : public entt::registry
    {
    public:

        using entt::registry::create;

        entt::entity create(const std::string& name)
        {
            auto entity = entt::registry::create();

            names.emplace(entity, name);

            return entity;
        }
        std::string GetName(entt::entity entity)
        {
            auto it = names.find(entity);

            if (it != names.end())
                return it->second;
            
            return std::to_string(entt::to_integral(entity));
        }
        void destroy(entt::entity entity)
        {
            names.erase(entity);

            entt::registry::destroy(entity);
        }
    private:
        std::map<entt::entity, std::string> names;
    };
}
