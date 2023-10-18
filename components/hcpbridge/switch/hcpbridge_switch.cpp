#include "hcpbridge_switch.h"

namespace esphome
{
  namespace hcpbridge
  {

    void HCPBridgeSwitch::update()
    {
      this->publish_state(this->parent_->engine->state->lightOn);
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
