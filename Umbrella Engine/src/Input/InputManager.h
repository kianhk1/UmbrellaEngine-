#pragma once
#include "../Renderer/API/GraphicsAPI/openglGraphicAPI.h"

enum class EventType {
	Unknown,
	KeyPressed,
	MouseMoved,
	JumpRequested,
	EntityDestroyed
	// ... انواع دیگر
};
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

struct Event {
	EventType type = EventType::Unknown;
	virtual ~Event() = default; // Destructor مجازی مهم است
};
struct KeyPressedEvent : public Event {
	int key;
	KeyPressedEvent(int k) : key(k) { type = EventType::KeyPressed; }
};
struct JumpRequestedEvent : public Event {
	JumpRequestedEvent() { type = EventType::JumpRequested; }
};

class EventBus
{
public:
	void addEvent(Event* event) {
		m_pendingEvents.push_back(event);
	}
	bool findEvent(EventType type) {
		for (auto& event : m_pendingEvents) {
			if (event->type == type)
				return true;
		}
		return false;
	}
	void clearEvent() {
		m_pendingEvents.clear();
	}
private:
	std::vector<Event*> m_pendingEvents;
};
class InputSystem {
public:
	InputSystem(EventBus& bus) : m_eventBus(bus) {}

	void SimulateKeyPress(int key) {
		KeyPressedEvent* keyEvent = new KeyPressedEvent(key);
		m_eventBus.addEvent(keyEvent);
	}
private:
	EventBus& m_eventBus;
};