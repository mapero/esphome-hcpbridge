
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome import pins
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_CONNECTIVITY,
    CONF_RX_PIN,
    CONF_TX_PIN,
)

AUTO_LOAD = ["binary_sensor"]

CONF_IS_CONNECTED = "is_connected"
CONF_RTS_PIN = "rts_pin"


hcpbridge_ns = cg.esphome_ns.namespace("hcpbridge")
HCPBridge = hcpbridge_ns.class_("HCPBridge", cg.Component)

CONF_HCPBridge_ID = "hcpbridge_id"

CONFIG_SCHEMA = (
  cv.Schema({
    cv.GenerateID(): cv.declare_id(HCPBridge),
    cv.Required(CONF_IS_CONNECTED): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_CONNECTIVITY,
            ),
    cv.Optional(CONF_RX_PIN): pins.gpio_input_pin_schema,
    cv.Optional(CONF_TX_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_RTS_PIN): pins.gpio_output_pin_schema,
  })
)

async def to_code(config):
  var = cg.new_Pvariable(config[CONF_ID])
  await cg.register_component(var, config)

  if CONF_IS_CONNECTED in config:
    sens = await binary_sensor.new_binary_sensor(config[CONF_IS_CONNECTED])
    cg.add(var.set_is_connected(sens))
  if CONF_RX_PIN in config:
    rx_pin = await cg.gpio_pin_expression(config[CONF_RX_PIN])
    cg.add(var.set_rx_pin(rx_pin))
  if CONF_TX_PIN in config:
    tx_pin = await cg.gpio_pin_expression(config[CONF_TX_PIN])
    cg.add(var.set_tx_pin(tx_pin))
  if CONF_RTS_PIN in config:
    rts_pin = await cg.gpio_pin_expression(config[CONF_RTS_PIN])
    cg.add(var.set_rts_pin(rts_pin))
