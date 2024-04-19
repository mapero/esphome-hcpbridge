#include "hcpbridge_textSensor.h"

namespace esphome
{
  namespace hcpbridge
  {

    static const char *const TAG = "hcpbridge.switch";

    void HCPBridgeTextSensor::update() {
      std::string stateText;
      switch (this->parent_->engine->state->state) {
        case HoermannState::OPENING:
          stateText = "opening";
          break;
        case HoermannState::MOVE_VENTING:
          stateText = "move venting";
          break;
        case HoermannState::MOVE_HALF:
          stateText = "move half";
          break;
        case HoermannState::CLOSING:
          stateText = "closing";
          break;
        case HoermannState::OPEN:
          stateText = "open";
          break;
        case HoermannState::CLOSED:
          stateText = "closing";
          break;
        case HoermannState::STOPPED:
          stateText = "stopped";
          break;
        case HoermannState::HALFOPEN:
          stateText = "half open";
          break;
        case HoermannState::VENT:
          stateText = "venting";
          break;
        default:
          stateText = "unknown";
          break;
      }
      ESP_LOGD(TAG, "HCPBridgeTextSensor::update() - %s", stateText);
      this->publish_state(stateText);
    }

  }
}
