#pragma once
#include <iostream>
#include "stb_image.h"
#include "FileSystem.h"

namespace Engine {
    namespace CORE {
        struct ImageData {
			int width = 0;
            int height = 0;
			int nrChannels = 0;
            unsigned char* pixels = 0;
			std::string log;
        };
		class Reader
		{
		public:
			static ImageData Readimage(const std::string& path) {
				if (!FileSystem::Exists(path))
					return {};
				stbi_set_flip_vertically_on_load(true);
				ImageData image;
				image.pixels = stbi_load(path.c_str(), &image.width, &image.height, &image.nrChannels, 0);
				if (image.pixels) {
					return image;
				}
				else {
					image.log = stbi_failure_reason();
					return image;
				}
			}
			static void Freeimage(ImageData& data) {
				stbi_image_free(data.pixels);
				data.pixels = nullptr;
				data.width = 0;
				data.height = 0;
				data.nrChannels = 0;
				data.log.clear();
			}

		private:
			Reader() = default;
		};

    }
}