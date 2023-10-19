
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_CONNECTIVITY,
)

AUTO_LOAD = ["binary_sensor"]

CONF_IS_CONNECTED = "is_connected"

hcpbridge_ns = cg.esphome_ns.namespace("hcpbridge")
HCPBridge = hcpbridge_ns.class_("HCPBridge", cg.Component)

CONF_HCPBridge_ID = "hcpbridge_id"

CONFIG_SCHEMA = (
  cv.Schema({
    cv.GenerateID(): cv.declare_id(HCPBridge),
    cv.Required(CONF_IS_CONNECTED): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_CONNECTIVITY,
            ),
  })
)

async def to_code(config):
  var = cg.new_Pvariable(config[CONF_ID])
  await cg.register_component(var, config)

  if CONF_IS_CONNECTED in config:
    sens = await binary_sensor.new_binary_sensor(config[CONF_IS_CONNECTED])
    cg.add(var.set_is_connected(sens))