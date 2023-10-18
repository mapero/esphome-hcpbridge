#include "hcpbridge_switch.h"

namespace esphome
{
  namespace hcpbridge
  {

    static const char *const TAG = "hcpbridge.switch";


    void HCPBridgeSwitch::update()
    {
      if (!this->parent_->engine->state->valid) {
        ESP_LOGD(TAG, "HCPBridgeSwitch::update() - state is invalid");
        this->status_set_warning();
        return;
      }
       ESP_LOGD(TAG, "HCPBridgeSwitch::update() - state is valid");
      if (this->status_has_warning()) {
        ESP_LOGD(TAG, "HCPBridgeSwitch::update() - clearing warning");
        this->status_clear_warning();
      }
      this->state = this->parent_->engine->state->lightOn;
      this->publish_state(false);
    }

    void HCPBridgeSwitch::write_state(bool state)
    {
        if (state)
        {
          this->parent_->engine->turnLight(true);
        }
        else
        {
          this->parent_->engine->turnLight(false);
        }
    }

  }
}
