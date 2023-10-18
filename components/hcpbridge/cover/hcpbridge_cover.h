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

      private:
        HCPBridge *parent_;
    };
  }
}