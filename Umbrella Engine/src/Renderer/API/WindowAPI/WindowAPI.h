#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include "../../../Core/Data/Data.h"

#define opengl 

namespace Engine {
	namespace API {

        std::shared_ptr<DATA::windowData> InitWindow(int width, int height, const char* title);
        void CloseWindow(std::shared_ptr<DATA::windowData> win);
        bool IsWindowShouldClose(std::shared_ptr<DATA::windowData> win);
        void Setviewport(DATA::Size windowsize);
        void UpdateWindowStatus(std::shared_ptr<DATA::windowData> win);

        void SetFullscreen(std::shared_ptr<DATA::windowData> win);
        void ToggleFullscreen(std::shared_ptr<DATA::windowData> win);
        //virtual void ToggleBorderlessWindowed(void) = 0;                       // Toggle window state: borderless windowed, resizes window to match monitor resolution
        //virtual void MaximizeWindow(void) = 0;                                  // Set window state: maximized, if resizable
        //virtual void MinimizeWindow(void) = 0;                                  // Set window state: minimized, if resizable
        //virtual void RestoreWindow(void) = 0;                                  // Set window state: not minimized/maximized
        void SetWindowIcon(std::shared_ptr<DATA::windowData> win, std::string imagePath);

        void ShowCursor(std::shared_ptr<DATA::windowData> win);                                     /// Shows cursor
        void HideCursor(std::shared_ptr<DATA::windowData> win);                                    /// Hides cursor
        //virtual void EnableCursor(void) = 0;                                    /// Enables cursor (unlock cursor)
        //virtual void DisableCursor(void) = 0;                                   /// Disables cursor (lock cursor)
        bool IsCursorOnScreen(std::shared_ptr<DATA::windowData> win);
        bool GetKey(std::shared_ptr<DATA::windowData> win, int keyname, int mode);
        DATA::Size GetSize(std::shared_ptr<DATA::windowData> win);


        // دریافت وضعیت API (مثلاً آیا اولیه شده است)
        void update(std::shared_ptr<DATA::windowData> win);

	}
}