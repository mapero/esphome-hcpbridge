#include "hcpbridge_switch.h"

namespace esphome
{
  namespace hcpbridge
  {

    static const char *const TAG = "hcpbridge.switch";

    void HCPBridgeSwitch::update()
    {
      if (!this->parent_->engine->state->valid)
      {
        if (!this->status_has_warning())
        {
          ESP_LOGD(TAG, "HCPBridgeSwitch::update() - state is invalid");
          this->status_set_warning();
        }
        return;
      }
      if (this->status_has_warning())
      {
        ESP_LOGD(TAG, "HCPBridgeSwitch::update() - clearing warning");
        this->status_clear_warning();
      }
      if (this->previousState_ != this->parent_->engine->state->lightOn)
      {
        ESP_LOGD(TAG, "HCPBridgeSwitch::update() - state changed");
        this->publish_state(this->parent_->engine->state->lightOn);
        this->previousState_ = this->parent_->engine->state->lightOn;
      }
    }

    void HCPBridgeSwitch::write_state(bool state)
    {
      if (state)
      {
        ESP_LOGD(TAG, "HCPBridgeSwitch::write_state() - turning light on");
        this->parent_->engine->turnLight(true);
      }
      else
      {
        ESP_LOGD(TAG, "HCPBridgeSwitch::write_state() - turning light off");
        this->parent_->engine->turnLight(false);
      }
    }

  }
}
