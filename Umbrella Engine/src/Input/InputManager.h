#pragma once
#include "../Renderer/API/GraphicsAPI/openglGraphicAPI.h"
struct KeyboardRawInput { 
	int KeyCode;
	bool isDown; 
	bool isUp;
};
struct MouseRawInput { 
	int KeyCode; 
	bool isLeftDown; 
	bool isLeftUp;
	bool isRightDown;
	bool isRightUp;
	float X_pos;
	float Y_pos;
};

struct KeyDownEvent { int KeyCode; };
struct KeyUpEvent { int KeyCode; };
struct KeyPressEvent { int KeyCode; };

struct MouseClickEvent { int button; int x, y; };

struct MovementInput { glm::vec3 direction; };

struct CurrentMousePosition { glm::vec2 position; };

struct Event {};
struct JumpRequestedEvent : public Event { 
	
};

class InputSystem
{
public:
	int isKeyDown() {

	}

private:

};
