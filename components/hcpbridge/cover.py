import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import cover
from esphome.const import CONF_ID

AUTO_LOAD = []

hcpbridge_ns = cg.esphome_ns.namespace('hcpbridge')
HCPBridge = hcpbridge_ns.class_('HCPBridgeComponent', cover.Cover, cg.Component)

CONFIG_SCHEMA = cover.COVER_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(HCPBridge)
}).extend(cv.polling_component_schema("500ms"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cover.register_cover(var, config)
    await cg.register_component(var, config)
