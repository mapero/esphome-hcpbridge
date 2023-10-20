#include "hcpbridge.h"

namespace esphome
{
  namespace hcpbridge
  {

    void HCPBridge::setup()
    {
      this->is_connected_->publish_state(false);

      int8_t rx = this->rx_pin_ == nullptr ? 18 : this->rx_pin_->get_pin();
      int8_t tx = this->tx_pin_ == nullptr ? 17 : this->tx_pin_->get_pin();

      this->engine = &HoermannGarageEngine::getInstance();
      this->engine->setup(rx, tx);
    }

    void HCPBridge::update() {
      if (this->engine->state->valid) {
        if (this->is_connected_->state != true) {
          this->is_connected_->publish_state(true);
        }
      } else {
        if (this->is_connected_->state != false) {
          this->is_connected_->publish_state(false);
        }
      }
    }

  }
}