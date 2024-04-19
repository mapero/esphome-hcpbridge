#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "../hcpbridge.h"

namespace esphome
{
  namespace hcpbridge
  {
    class HCPBridgeTextSensor : public switch_::Switch, public PollingComponent
    {
      public:
        void set_hcpbridge_parent(HCPBridge *parent) { this->parent_ = parent; }
        void update() override;
        void write_state(bool state) override;

      private:
        HCPBridge *parent_;
        bool previousState_ = false;
    };
  }
}