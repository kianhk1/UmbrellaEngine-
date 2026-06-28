#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include "../Core/Data/Data.h"

namespace Engine {
	namespace Event {
		using eventcallback = std::function<void(void* Payload)>;
		struct EventListener
		{
			eventcallback callback;
			size_t id;
		};
		class EventManager {
		public:
			size_t Subscribe(const std::string& eventName, eventcallback callback) {
				eventCallbacks[eventName].push_back({ callback , ++nextListenerId });
				return nextListenerId;
			}
			void Unsubscribe(const std::string& eventName, size_t listenerid) {
				if (eventCallbacks.find(eventName) == eventCallbacks.end()) return;

				auto& callbacks = eventCallbacks[eventName];
				callbacks.erase(
					std::remove_if(callbacks.begin(), callbacks.end(),
						[listenerid](const EventListener& listener) {
							return listener.id == listenerid;
						}),
					callbacks.end()
				);
			}
			
			void Broadcast(const std::string& eventName, void* payload) {
				if (eventCallbacks.find(eventName) != eventCallbacks.end()) {
					auto callbacksCopy = eventCallbacks[eventName];
					for (const auto& listener : callbacksCopy) { 
						listener.callback(payload); 
					}
				}
			}
			void Clear() {
				eventCallbacks.clear();
			}
			bool HasSubscribers(const std::string& eventName) const {
				return eventCallbacks.find(eventName) != eventCallbacks.end() && !eventCallbacks.at(eventName).empty();
			}
			static EventManager& GetInstance() {
				static EventManager instance;
				return instance;
			}
		private:
			EventManager() = default;
			std::unordered_map<std::string, std::vector<EventListener>> eventCallbacks;
			size_t nextListenerId = 0;
		};
	}
}