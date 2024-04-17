from esphome.components import button
from esphome.const import ICON_FAN
import esphome.config_validation as cv
import esphome.codegen as cg
from .. import hcpbridge_ns, CONF_HCPBridge_ID, HCPBridge

DEPENDENCIES = ["hcpbridge"]

HCPBridgeButton = hcpbridge_ns.class_("HCPBridgeButton", button.Button, cg.Component)

CONFIG_SCHEMA = (
    button.button_schema(HCPBridgeButton, icon=ICON_FAN)
    .extend(
        {
            cv.GenerateID(CONF_HCPBridge_ID): cv.use_id(HCPBridge),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = await button.new_button(config)
    await cg.register_component(var, config)

    parent = await cg.get_variable(config[CONF_HCPBridge_ID])
    cg.add(var.set_hcpbridge_parent(parent))

