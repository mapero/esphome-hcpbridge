#include "hcpbridge_cover.h"

namespace esphome
{
  namespace hcpbridge
  {

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
      // Do something with the call
    }

    void HCPBridgeCover::update()
    {
      // Do something with the state
    }

  }
}
