#include "esphome/core/log.h"
#include "hoermann.h"

namespace esphome {
namespace hcpbridge {

void HCPBridgeComponent::update_status()
{

}

void HCPBridgeComponent::setup() {

}

void HCPBridgeComponent::update() {

} 

void HCPBridgeComponent::dump_config(){
    LOG_LOCK(TAG, "HCPBridge", this);    
}

} //namespace hcpbridge
} //namespace esphome