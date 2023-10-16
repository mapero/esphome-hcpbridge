import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import cover
from esphome.const import CONF_ID, CONF_BATTERY_LEVEL, DEVICE_CLASS_CONNECTIVITY, DEVICE_CLASS_BATTERY, DEVICE_CLASS_DOOR, UNIT_PERCENT, ENTITY_CATEGORY_CONFIG

AUTO_LOAD = ["binary_sensor", "text_sensor", "sensor", "switch"]

CONF_IS_CONNECTED = "is_connected"
CONF_IS_PAIRED = "is_paired"
CONF_UNPAIR = "unpair"
CONF_BATTERY_CRITICAL = "battery_critical"
CONF_BATTERY_LEVEL = "battery_level"
CONF_DOOR_SENSOR = "door_sensor"
CONF_DOOR_SENSOR_STATE = "door_sensor_state"

hcpbridge_ns = cg.esphome_ns.namespace('hcpbridge')
HCPBridge = hcpbridge_ns.class_('HCPBridgeComponent', cover.Cover, cg.Component)

CONFIG_SCHEMA = cover.LOCK_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(HCPBridge),
}).extend(cv.polling_component_schema("500ms"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cover.register_cover(var, config)
