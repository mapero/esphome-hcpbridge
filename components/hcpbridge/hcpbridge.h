#pragma once

#include "esphome/core/component.h"
#include "esphome/components/cover/cover.h"
#include "hoermann.h"


namespace esphome {
namespace hcpbridge {

static const char *TAG = "hcpbridge.cover";

class HCPBridgeComponent : public cover::Cover, public PollingComponent {

    public:
        const uint32_t deviceId_ = 2020002;
        const std::string deviceName_ = "HCPBridge"; 

        explicit HCPBridgeComponent() : Cover() {};

        void setup() override;
        void update() override;

        float get_setup_priority() const override { return setup_priority::HARDWARE - 1.0f; }
        cover::CoverTraits get_traits() override;


        void dump_config() override;

    protected:
      void control(const cover::CoverCall &call) override;

};

} //namespace hcpbridge
} //namespace esphome