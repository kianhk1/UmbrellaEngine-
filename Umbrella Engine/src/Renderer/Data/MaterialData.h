#pragma once
#include <string>
#include <vector>
#include <memory> // برای std::shared_ptr یا std::unique_ptr

// فرض می‌کنیم کلاس Texture وجود داره یا از std::string برای مسیر تکسچر استفاده می‌کنیم.
// برای سادگی، اینجا از std::string استفاده می‌کنیم.
// اگر کلاس Texture دارید، می‌تونید به جای string از std::shared_ptr<Texture> استفاده کنید.

class Material {
public:
    // سازنده پیش‌فرض
    Material() {
        setBaseColor(1.0f, 1.0f, 1.0f, 1.0f);
        setSpecularColor(1.0f, 1.0f, 1.0f);
        setShininess(32.0f);
        setTransparency(1.0f);
    }     // کاملا مات پیش‌فرض

// --- تنظیم کننده‌ها (Setters) ---

// تنظیم رنگ پایه
    void setBaseColor(float r, float g, float b, float a = 1.0f) {
        baseColor = { r, g, b, a };
    }
    void setBaseColor(const std::vector<float>& color) {
        if (color.size() >= 3) {
            baseColor = { color[0], color[1], color[2], (color.size() >= 4 ? color[3] : 1.0f) };
        }
    }

    // تنظیم مسیر تکسچر رنگ پایه
    void setAlbedoTexture(const std::string& path) {
        albedoTexturePath = path;
    }

    // تنظیم مسیر تکسچر معمولی
    void setNormalMapTexture(const std::string& path) {
        normalMapPath = path;
    }

    // تنظیم مسیر تکسچر بازتاب (Specular Map)
    void setSpecularMapTexture(const std::string& path) {
        specularMapPath = path;
    }

    // تنظیم رنگ بازتاب (Specular Color)
    void setSpecularColor(float r, float g, float b) {
        specularColor = { r, g, b };
    }

    // تنظیم میزان درخشندگی (Shininess)
    void setShininess(float shininess_value) {
        shininess = shininess_value;
    }

    // تنظیم میزان شفافیت (Transparency/Opacity)
    void setTransparency(float alpha) {
        transparency = alpha;
        // اگر شفافیت کمتر از 1 باشه، ممکنه نیاز باشه flag ای برای فعال کردن blending تنظیم بشه.
    }

    // --- دریافت کننده‌ها (Getters) ---

    const std::vector<float>& getBaseColor() const { return baseColor; }
    const std::string& getAlbedoTexturePath() const { return albedoTexturePath; }
    const std::string& getNormalMapTexturePath() const { return normalMapPath; }
    const std::string& getSpecularMapTexturePath() const { return specularMapPath; }
    const std::vector<float>& getSpecularColor() const { return specularColor; }
    float getShininess() const { return shininess; }
    float getTransparency() const { return transparency; }

    // --- متدهای کمکی ---

    // بررسی اینکه آیا تکسچر رنگ پایه تنظیم شده است؟
    bool hasAlbedoTexture() const { return !albedoTexturePath.empty(); }
    bool hasNormalMapTexture() const { return !normalMapPath.empty(); }
    bool hasSpecularMapTexture() const { return !specularMapPath.empty(); }


private:
    // داده‌های رنگی معمولا با 4 مولفه (RGBA) نمایش داده می‌شوند
    std::vector<float> baseColor; // RGBA, مقادیر بین 0.0 تا 1.0

    // مسیر تکسچرها
    std::string albedoTexturePath;
    std::string normalMapPath;
    std::string specularMapPath; // برای کنترل شدت بازتاب

    std::vector<float> specularColor; // RGB, رنگ بازتاب نور
    float shininess;                  // میزان براق بودن سطح (معمولا در سایه‌زن‌های کلاسیک)
    float transparency;               // میزان شفافیت (0.0 کاملا شفاف، 1.0 کاملا مات)
};
