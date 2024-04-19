from esphome.components import switch
from esphome.const import ICON_LIGHTBULB
import esphome.config_validation as cv
import esphome.codegen as cg
from .. import hcpbridge_ns, CONF_HCPBridge_ID, HCPBridge

DEPENDENCIES = ["hcpbridge"]

HCPBridgeSwitch = hcpbridge_ns.class_("HCPBridgeSwitch", switch.Switch, cg.Component)

CONFIG_SCHEMA = (
    switch.switch_schema(HCPBridgeSwitch, icon=ICON_LIGHTBULB)
    .extend(
        {
            cv.GenerateID(CONF_HCPBridge_ID): cv.use_id(HCPBridge),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)

    parent = await cg.get_variable(config[CONF_HCPBridge_ID])
    cg.add(var.set_hcpbridge_parent(parent))

