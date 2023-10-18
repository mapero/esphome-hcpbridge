
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
)

hcpbridge_ns = cg.esphome_ns.namespace("hcpbridge")
HCPBridge = hcpbridge_ns.class_("HCPBridge", cg.Component)

CONF_HCPBridge_ID = "hcpbridge_id"

CONFIG_SCHEMA = (
  cv.Schema({
    cv.GenerateID(): cv.declare_id(HCPBridge),
  })
)

async def to_code(config):
  var = cg.new_Pvariable(config[CONF_ID])
  await cg.register_component(var, config)