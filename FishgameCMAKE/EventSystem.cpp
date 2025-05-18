#include "EventSystem.h"

EventSystem * EventSystem::instance = nullptr;

void EventSystem::addListener(const std::string& eventType, std::function<void(const std::unordered_map<std::string, std::string>&)> callback){
  listeners[eventType].push_back(callback);
}
void EventSystem::emitEvent(const std::string & eventType, const std::unordered_map<std::string, std::string>& param = {}){

  // finds the eventType in the param
  if(param.find(eventType) != param.end()){
    // for every callback for the param type it sends the signal to listeners
    for(auto & callback : listeners[eventType]){
      callback(param);
    }
  }

}