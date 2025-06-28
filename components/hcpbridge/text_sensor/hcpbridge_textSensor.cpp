#include "hcpbridge_textSensor.h"

namespace esphome
{
  namespace hcpbridge
  {

    static const char *const TAG = "hcpbridge.text_sensor";

    void HCPBridgeTextSensor::update() {
      std::string stateText;

      if (this->parent_->engine->state->state != this->previousState_){
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
            stateText = "closed";
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

        this->previousState_ = this->parent_->engine->state->state;
        ESP_LOGD(TAG, "HCPBridgeTextSensor::update() - %s", stateText.c_str());
        this->publish_state(stateText);
      }
    }

  }
}
