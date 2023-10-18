#pragma once

#include "esphome/core/component.h"
#include "hoermann.h"

namespace esphome
{
  namespace hcpbridge
  {

    class HCPBridge : public Component
    {
      public:
        void setup() override;
        HoermannGarageEngine* engine;
    };

  }
}