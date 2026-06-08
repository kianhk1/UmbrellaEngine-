#pragma once
#include <iostream>

namespace Engine {
	namespace API {
        struct Size { int width; int height; };
        struct Point { int x; int y; };
        struct window;
		class WindowAPI
		{
		public:
            virtual ~WindowAPI() = default;

            virtual std::shared_ptr<API::window> InitWindow(int width, int height, const char* title) = 0;
            virtual void CloseWindow(std::shared_ptr<API::window> win) = 0;
            virtual bool IsWindowShouldClose(std::shared_ptr<API::window> win) = 0;

            virtual void SetFullscreen(std::shared_ptr<API::window> win) = 0;
            virtual void ToggleFullscreen(std::shared_ptr<API::window> win) = 0;
            //virtual void ToggleBorderlessWindowed(void) = 0;                       // Toggle window state: borderless windowed, resizes window to match monitor resolution
            //virtual void MaximizeWindow(void) = 0;                                  // Set window state: maximized, if resizable
            //virtual void MinimizeWindow(void) = 0;                                  // Set window state: minimized, if resizable
            //virtual void RestoreWindow(void) = 0;                                  // Set window state: not minimized/maximized
            virtual void SetWindowIcon(std::shared_ptr<API::window> win, std::string imagePath) = 0;

            virtual void ShowCursor(std::shared_ptr<API::window> win) = 0;                                     /// Shows cursor
            virtual void HideCursor(std::shared_ptr<API::window> win) = 0;                                    /// Hides cursor
            //virtual void EnableCursor(void) = 0;                                    /// Enables cursor (unlock cursor)
            //virtual void DisableCursor(void) = 0;                                   /// Disables cursor (lock cursor)
            virtual bool IsCursorOnScreen(std::shared_ptr<API::window> win) = 0;
            virtual bool GetKey(std::shared_ptr<API::window> win, int keyname, int mode) = 0;
            virtual Size* GetSize(std::shared_ptr<API::window> win) = 0;


            // دریافت وضعیت API (مثلاً آیا اولیه شده است)
			virtual void update(std::shared_ptr<API::window> win) = 0;

		protected:

		};

	}
}