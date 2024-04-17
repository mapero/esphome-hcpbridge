#pragma once

#include "esphome/core/component.h"
#include "esphome/components/button/button.h"
#include "../hcpbridge.h"

namespace esphome
{
  namespace hcpbridge
  {
    class HCPBridgeButton : public button_::Button, public PollingComponent
    {
      public:
        void set_hcpbridge_parent(HCPBridge *parent) { this->parent_ = parent; }
        void press_action() override;

      private:
        HCPBridge *parent_;
        bool previousState_ = false;
    };
  }
}