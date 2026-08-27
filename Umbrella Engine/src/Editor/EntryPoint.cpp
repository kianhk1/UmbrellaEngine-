#pragma once
#include "Application.h"
#define UE_PLATFORM_WINDOWS 
// NVIDIA GPU
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
// AMD GPU
extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#ifdef UE_PLATFORM_WINDOWS

extern Engine::Editor::Application* Engine::Editor::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

namespace Engine {
	namespace Editor {

		int Main(int argc, char** argv)
		{
			
			Application* app = CreateApplication(argc, argv);
			app->Run();
			delete app;

			return 0;
		}

	}
}

#ifdef UE_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return Walnut::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Engine::Editor::Main(argc, argv);
}

#endif // WL_DIST

#endif // WL_PLATFORM_WINDOWS
