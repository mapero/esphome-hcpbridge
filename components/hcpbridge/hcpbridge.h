#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include "hoermann.h"

namespace esphome
{
  namespace hcpbridge
  {

    class HCPBridge : public PollingComponent
    {
      public:
        void setup() override;
        void update() override;
      
        void set_is_connected(binary_sensor::BinarySensor *is_connected) { this->is_connected_ = is_connected; }

        HoermannGarageEngine* engine;

      protected:
        binary_sensor::BinarySensor *is_connected_;
    };
  }
}