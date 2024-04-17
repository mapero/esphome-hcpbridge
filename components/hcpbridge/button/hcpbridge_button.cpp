#include "hcpbridge_button.h"

namespace esphome
{
  namespace hcpbridge
  {

    static const char *const TAG = "hcpbridge.button";

    void HCPBridgeButton::press()
    {
      ESP_LOGD(TAG, "HCPBridgeButton::press_action() - trigger vent position");
      this->parent_->engine->ventilationPositionDoor();
    }

  }
}
