#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>

class EventSystem {
  private:
    EventSystem() = default;
    static EventSystem * instance;

    std::unordered_map<std::string, std::vector<std::function<void(const std::unordered_map<std::string, std::string>&)>>> listeners;

  public:

    static EventSystem& GetInstance() {
      if (instance == nullptr) {
        instance = new EventSystem();
      }
      return *instance;
    }

    void Clear() {
      listeners.clear();
    }

    void addListener(const std::string& eventType, std::function<void(const std::unordered_map<std::string, std::string>&)> callback);

    void emitEvent(const std::string & event, const std::unordered_map<std::string, std::string>& param);

    static void Cleanup() {
      delete instance;
      instance = nullptr;
    }

  inline EventSystem& Events() {
    return EventSystem::GetInstance();
  }


};