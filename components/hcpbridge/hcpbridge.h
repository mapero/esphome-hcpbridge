#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"

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
        void set_tx_pin(InternalGPIOPin *tx_pin) { this->tx_pin_ = tx_pin; }
        void set_rx_pin(InternalGPIOPin *rx_pin) { this->rx_pin_ = rx_pin; }


        HoermannGarageEngine* engine;

      protected:
        binary_sensor::BinarySensor *is_connected_;
        InternalGPIOPin *tx_pin_;
        InternalGPIOPin *rx_pin_;
    };
  }
}