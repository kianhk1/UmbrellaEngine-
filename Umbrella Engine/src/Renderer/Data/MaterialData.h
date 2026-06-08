#pragma once
#include <string>
#include <vector>
#include <memory> // برای std::shared_ptr یا std::unique_ptr
#include <glm/glm.hpp>

// فرض می‌کنیم کلاس Texture وجود داره یا از std::string برای مسیر تکسچر استفاده می‌کنیم.
// برای سادگی، اینجا از std::string استفاده می‌کنیم.
// اگر کلاس Texture دارید، می‌تونید به جای string از std::shared_ptr<Texture> استفاده کنید.
namespace DATA {
    
    struct MaterialData {
        // داده‌های رنگی معمولا با 4 مولفه (RGBA) نمایش داده می‌شوند
        glm::vec4 baseColor; // RGBA, مقادیر بین 0.0 تا 1.0

        // مسیر تکسچرها
        DATA::Texture2DData albedoTexturedata;
        DATA::Texture2DData normalMapdata;
        DATA::Texture2DData specularMapdata; // برای کنترل شدت بازتاب

        glm::vec3 specularColor; // RGB, رنگ بازتاب نور
        float shininess;                  // میزان براق بودن سطح (معمولا در سایه‌زن‌های کلاسیک)
        float transparency;               // میزان شفافیت (0.0 کاملا شفاف، 1.0 کاملا مات)
    };
}
