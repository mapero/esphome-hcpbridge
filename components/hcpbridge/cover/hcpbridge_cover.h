#pragma once

#include "esphome/core/component.h"
#include "esphome/components/cover/cover.h"
#include "../hcpbridge.h"

namespace esphome
{
  namespace hcpbridge
  {
    class HCPBridgeCover : public cover::Cover, public PollingComponent
    {
    public:
      cover::CoverTraits get_traits() override;
      void control(const cover::CoverCall &call) override;
      void set_hcpbridge_parent(HCPBridge *parent) { this->parent_ = parent; }
      void update() override;
      bool get_light_state() {return this->parent_->engine->state->lightOn; }
      void set_light_state(bool state) {this->parent_->engine->turnLight(state);}
      HoermannState::State get_cover_state() {return this->parent_->engine->state->state;}
      // Home Assistant service handler
      void on_go_to_half();
      void on_go_to_open();
      void on_go_to_close();
      void on_go_to_vent();

    private:
      HCPBridge *parent_;
      float previousPosition_ = 0.0f;
      cover::CoverOperation previousOperation_ = cover::COVER_OPERATION_IDLE;
    };
  }
}