#include "esphome/core/log.h"
#include "hcpbridge.h"

namespace esphome {
namespace hcpbridge {

cover::CoverTraits HCPBridgeComponent::get_traits() {
    auto traits = cover::CoverTraits();
    traits.set_is_assumed_state(false);
    traits.set_supports_position(false);
    traits.set_supports_tilt(false);

    return traits;
}

void HCPBridgeComponent::control(const cover::CoverCall &call) {
    
}

void HCPBridgeComponent::setup() {
  hoermannEngine->setup();
}

void HCPBridgeComponent::update() {

} 

void HCPBridgeComponent::dump_config(){
    ESP_LOGCONFIG(TAG, "HCPBridge", this);    
}

} //namespace hcpbridge
} //namespace esphome