#include "esphome/core/log.h"
#include "esphome/components/cover/cover.h"
#include "hcpbridge.h"

namespace esphome {
namespace hcpbridge {

const float COVER_OPEN = 1.0f;
const float COVER_CLOSED = 0.0f;

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
  } else if (call.get_position().has_value()) {
    auto pos = call.get_position();
    if (pos == COVER_OPEN) {
      ESP_LOGD(TAG, "Open");
      HoermannGarageEngine::getInstance().openDoor();
    } else if (pos == COVER_CLOSED) {
      ESP_LOGD(TAG, "Close");
      HoermannGarageEngine::getInstance().closeDoor();
    } else {
      HoermannGarageEngine::getInstance().setPosition(call.get_position().value());
    }
  } else {
    ESP_LOGD(TAG, "Unknown command");
  }

}

void HCPBridgeComponent::setup() {
  HoermannGarageEngine::getInstance().setup();
}

void HCPBridgeComponent::update() {
  if (HoermannGarageEngine::getInstance().state->changed) {
    HoermannGarageEngine::getInstance().state->clearChanged();
    this->position = HoermannGarageEngine::getInstance().state->currentPosition;
    switch (HoermannGarageEngine::getInstance().state->state) {
      case HoermannState::CLOSED:
        this->current_operation = cover::COVER_OPERATION_IDLE;
        break;
      case HoermannState::OPEN:
        this->current_operation = cover::COVER_OPERATION_IDLE;
        break;
      case HoermannState::CLOSING:
        this->current_operation = cover::COVER_OPERATION_CLOSING;
        break;
      case HoermannState::OPENING:
        this->current_operation = cover::COVER_OPERATION_OPENING;
        break;
      case HoermannState::HALFOPEN:
        this->current_operation = cover::COVER_OPERATION_IDLE;
        break;
      case HoermannState::VENT:
        this->current_operation = cover::COVER_OPERATION_IDLE;
        break;
    }
    this->publish_state();
  }
} 

void HCPBridgeComponent::dump_config(){
    ESP_LOGCONFIG(TAG, "HCPBridge", this);    
}

} //namespace hcpbridge
} //namespace esphome