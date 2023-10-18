#include "hcpbridge.h"

namespace esphome
{
  namespace hcpbridge
  {

    void HCPBridge::setup()
    {
      this->engine = &HoermannGarageEngine::getInstance();
      this->engine->setup();
    }

  }
}