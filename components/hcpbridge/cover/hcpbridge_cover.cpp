#include "hcpbridge_cover.h"

namespace esphome
{
  namespace hcpbridge
  {
    static const char *const TAG = "hcpbridge.cover";


    cover::CoverTraits HCPBridgeCover::get_traits()
    {
      auto traits = cover::CoverTraits();
      traits.set_is_assumed_state(true);
      traits.set_supports_position(false);
      traits.set_supports_tilt(false);
      return traits;
    }

    void HCPBridgeCover::control(const cover::CoverCall &call)
    {
      if (call.get_stop())
      {
        this->parent_->engine->stopDoor();
      }
      if (call.get_position().has_value())
      {
        if (call.get_position().value() == 1.0f)
        {
          this->parent_->engine->openDoor();
        }
        else if (call.get_position().value() == 0.0f)
        {
          this->parent_->engine->closeDoor();
        }
      }
    }

    void HCPBridgeCover::update()
    {
      if (!this->parent_->engine->state->valid) {
        ESP_LOGD(TAG, "HCPBridgeCover::update() - state is invalid");
        this->status_set_warning();
        return;
      }
      ESP_LOGD(TAG, "HCPBridgeCover::update() - state is valid");
      if (this->status_has_warning()) {
        ESP_LOGD(TAG, "HCPBridgeCover::update() - clearing warning");
        this->status_clear_warning();
      }
      this->position = this->parent_->engine->state->currentPosition/100.0f;
      this->publish_state(false);
    }

  }
}
