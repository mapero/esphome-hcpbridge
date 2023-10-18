#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "../hcpbridge.h"

namespace esphome
{
  namespace hcpbridge
  {
    class HCPBridgeSwitch : public switch_::Switch, public PollingComponent
    {
      public:
        void set_hcpbridge_parent(HCPBridge *parent) { this->parent_ = parent; }
        void update() override;
        void write_state(bool state) override;

      private:
        HCPBridge *parent_;
    };
  }
}