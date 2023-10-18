from esphome.components import cover
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import (
  CONF_OUTPUT_ID
)

from .. import hcpbridge_ns, HCPBridge, CONF_HCPBridge_ID

DEPENDENCIES = ['hcpbridge']

HCPBridgeCover = hcpbridge_ns.class_("HCPBridgeCover", cover.Cover, cg.PollingComponent)

CONFIG_SCHEMA = cv.All(
  cover.COVER_SCHEMA.extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(HCPBridgeCover),
    cv.GenerateID(CONF_HCPBridge_ID): cv.use_id(HCPBridge),
  }).extend(cv.polling_component_schema("500ms")),
)

async def to_code(config):
  var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
  await cg.register_component(var, config)
  await cover.register_cover(var, config)
  parent = await cg.get_variable(config[CONF_HCPBridge_ID])
  cg.add(var.set_hcpbridge_parent(parent))