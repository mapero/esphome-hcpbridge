#pragma once

#include "esphome/core/component.h"
#include "esphome/components/cover/cover.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/api/custom_api_device.h"

namespace esphome {
namespace hcpbridge {

static const char *TAG = "hcpbridge.cover";

class HCPBridgeComponent : public cover::Cover, public PollingComponent, public api::CustomAPIDevice {

    public:
        const uint32_t deviceId_ = 2020002;
        const std::string deviceName_ = "HCPBridge"; 

        explicit HCPBridgeComponent() : Cover(), unpair_(false), open_latch_(false), lock_n_go_(false) { this->traits.set_supports_open(true); }

        void setup() override;
        void update() override;

        float get_setup_priority() const override { return setup_priority::HARDWARE - 1.0f; }

        void dump_config() override;

    protected:
        void update_status();

};

} //namespace hcpbridge
} //namespace esphome