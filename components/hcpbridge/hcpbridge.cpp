#include "esphome/core/log.h"
#include "hcpbridge.h"

namespace esphome {
namespace hcpbridge {

cover::CoverTraits HCPBridgeComponent::get_traits() {
    auto traits = cover::CoverTraits();
    traits.set_is_assumed_state(true);
    traits.set_supports_position(true);
    traits.set_supports_tilt(false);

    return traits;
}

void HCPBridgeComponent::control(const cover::CoverCall &call) {
  if(!HoermannGarageEngine::getInstance().state->valid) {
    ESP_LOGD(TAG, "Not valid");
    return;
  }
  
  if (call.get_stop()) {
    ESP_LOGD(TAG, "Stop");
    HoermannGarageEngine::getInstance().stopDoor();
    this->publish_state();
  } else if (call.get_position().has_value()) {
    ESP_LOGD(TAG, "Open");
    HoermannGarageEngine::getInstance().setPosition(call.get_position().value());
  } else {
    ESP_LOGD(TAG, "Unknown command");
  }

}

void HCPBridgeComponent::setup() {
  HoermannGarageEngine::getInstance().setup();
}

void HCPBridgeComponent::update() {

} 

void HCPBridgeComponent::dump_config(){
    ESP_LOGCONFIG(TAG, "HCPBridge", this);    
}

} //namespace hcpbridge
} //namespace esphome