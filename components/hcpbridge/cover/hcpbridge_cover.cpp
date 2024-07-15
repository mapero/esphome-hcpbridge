#include "hcpbridge_cover.h"

namespace esphome
{
  namespace hcpbridge
  {
    static const char *const TAG = "hcpbridge.cover";

    void HCPBridgeCover::on_go_to_open()
    {
      ESP_LOGD(TAG, "HCPBridgeCover::on_go_to_open() - opening");
      this->parent_->engine->openDoor();
    }

    void HCPBridgeCover::on_go_to_close()
    {
      ESP_LOGD(TAG, "HCPBridgeCover::on_go_to_close() - closing");
      this->parent_->engine->closeDoor();
    }

    void HCPBridgeCover::on_go_to_half()
    {
      ESP_LOGD(TAG, "HCPBridgeCover::on_go_to_half() - half opening");
      this->parent_->engine->halfPositionDoor();
    }

    void HCPBridgeCover::on_go_to_vent()
    {
      ESP_LOGD(TAG, "HCPBridgeCover::on_go_to_vent() - venting");
      this->parent_->engine->ventilationPositionDoor();
    }

    cover::CoverTraits HCPBridgeCover::get_traits()
    {
      auto traits = cover::CoverTraits();
      traits.set_is_assumed_state(true);
      traits.set_supports_position(true);
      traits.set_supports_tilt(false);
      traits.set_supports_stop(true);
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
        else
        {
          this->parent_->engine->setPosition(call.get_position().value() * 100.0f);
        }
      }
    }

    void HCPBridgeCover::update()
    {
      if (!this->parent_->engine->state->valid)
      {
        if (!this->status_has_warning())
        {
          ESP_LOGD(TAG, "HCPBridgeCover::update() - state is invalid");
          this->status_set_warning();
        }
        return;
      }
      if (this->status_has_warning())
      {
        ESP_LOGD(TAG, "HCPBridgeCover::update() - clearing warning");
        this->status_clear_warning();
      }

      switch (this->parent_->engine->state->state)
      {
      case HoermannState::OPENING:
      case HoermannState::MOVE_VENTING:
      case HoermannState::MOVE_HALF:
        this->current_operation = cover::COVER_OPERATION_OPENING;
        break;
      case HoermannState::CLOSING:
        this->current_operation = cover::COVER_OPERATION_CLOSING;
        break;
      case HoermannState::OPEN:
      case HoermannState::CLOSED:
      case HoermannState::STOPPED:
      case HoermannState::HALFOPEN:
      case HoermannState::VENT:
        this->current_operation = cover::COVER_OPERATION_IDLE;
        break;
      }
      this->position = this->parent_->engine->state->currentPosition;
      if (this->previousPosition_ != this->position || this->previousOperation_ != this->current_operation)
      {
        ESP_LOGV(TAG, "HCPBridgeCover::update() - position is %f", this->position);
        ESP_LOGV(TAG, "HCPBridgeCover::update() - operation is %d", this->current_operation);
        ESP_LOGD(TAG, "HCPBridgeCover::update() - state changed");
        this->publish_state(false);
        this->previousPosition_ = this->position;
        this->previousOperation_ = this->current_operation;
      }
    }

  }
}
