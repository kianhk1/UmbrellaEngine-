#pragma once
#include <cstdint>
#include <string_view>
#include <vector>
#include <filesystem>

namespace Engine {
	namespace CORE {
        class Hash {
        public:

            static constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037ull;
            static constexpr uint64_t FNV_PRIME = 1099511628211ull;

            static uint64_t FNV1a(std::string_view str)
            {
                uint64_t hash = FNV_OFFSET_BASIS;
                std::string canonical = std::filesystem::weakly_canonical(str).string();
                for (char c : canonical)
                {
                    hash ^= static_cast<uint8_t>(c);
                    hash *= FNV_PRIME;
                }
                return hash;
            }

            static uint64_t FNV1a(std::vector<std::string> strs)
            {
                uint64_t hash = FNV_OFFSET_BASIS;

                for (auto& str : strs) {
                    std::string canonical = std::filesystem::weakly_canonical(str).string();
                    for (char c : canonical)
                    {
                        hash ^= static_cast<uint8_t>(c);
                        hash *= FNV_PRIME;
                    }
                    hash ^= 0xFF;
                    hash *= FNV_PRIME;
                }
                return hash;
            }
        };
	}
}