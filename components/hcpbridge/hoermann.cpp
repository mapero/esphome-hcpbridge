// Credits to https://github.com/Gifford47/HCPBridgeMqtt for the initial code base

#include "hoermann.h"

// arg1,arg4> command start value
// arg2,arg5> command end   value
const HoermannCommand HoermannCommand::STARTOPENDOOR = HoermannCommand(0x0210, 0x0110, 0x0000, 0x0000); // Typo 0201
const HoermannCommand HoermannCommand::STARTCLOSEDOOR = HoermannCommand(0x0220, 0x0120, 0x0000, 0x0000);
const HoermannCommand HoermannCommand::STARTSTOPDOOR = HoermannCommand(0x0240, 0x0140, 0x0000, 0x0000);
const HoermannCommand HoermannCommand::STARTOPENDOORHALF = HoermannCommand(0x0200, 0x0100, 0x0400, 0x0400);
const HoermannCommand HoermannCommand::STARTVENTPOSITION = HoermannCommand(0x0200, 0x0100, 0x4000, 0x4000);
const HoermannCommand HoermannCommand::STARTTOGGLELAMP = HoermannCommand(0x0100, 0x0800, 0x0200, 0x0200);
const HoermannCommand HoermannCommand::WAITING = HoermannCommand(0x0000, 0x0000, 0x0000, 0x0000);

TaskHandle_t modBusTask;
void modbusServeTask(void *parameter);

void DelayHandler(void)
{
  HoermannGarageEngine::getInstance().handleModbus();
}

void modbusServeTask(void *parameter)
{
  while (true)
  {
    HoermannGarageEngine::getInstance().handleModbus();
    vTaskDelay(1);
  }
  vTaskDelete(NULL);
}

HoermannGarageEngine &HoermannGarageEngine::getInstance()
{
  static HoermannGarageEngine instance;
  return instance;
}

void HoermannGarageEngine::setup()
{
  RS485.begin(57600, SERIAL_8E1, PIN_RXD, PIN_TXD);
  mb.begin(&RS485);
  mb.slave(SLAVE_ID);

  xTaskCreatePinnedToCore(
      modbusServeTask, /* Function to implement the task */
      "ModBusTask",    /* Name of the task */
      10000,           /* Stack size in words */
      NULL,            /* Task input parameter */
      // 1,  /* Priority of the task */
      configMAX_PRIORITIES - 1,
      &modBusTask, /* Task handle. */
      1);          /* Core where the task should run */

  // Required for Write
  mb.addHreg(0x9C41, 0, 0x03); // Commands
  mb.addHreg(0x9D31, 0, 0x09); // Broadcast
  // For Read
  mb.addHreg(0x9CB9, 0, 0x08); // Internal State (e.g. Button Pressed)

  // on Request - Clear/Calc Response data
  mb.onRequest([this](Modbus::FunctionCode fc, const Modbus::RequestData data) -> Modbus::ResultCode
               { return this->onRequest(fc, data); });

  // On Write (update counter in Read Registers)
  mb.onSet(
      HREG(0x9C41 + 0), [this](TRegister *reg, uint16_t val) -> uint16_t
      { return this->onCounterWrite(reg, val); },
      0x01);
  // Broadcast Changes
  mb.onSet(
      HREG(0x9D31 + 2), [this](TRegister *reg, uint16_t val) -> uint16_t
      { return this->onCurrentStateChanged(reg, val); },
      0x01);
  mb.onSet(
      HREG(0x9D31 + 1), [this](TRegister *reg, uint16_t val) -> uint16_t
      { return this->onDoorPositonChanged(reg, val); },
      0x01);
  mb.onSet(
      HREG(0x9D31 + 6), [this](TRegister *reg, uint16_t val) -> uint16_t
      { return this->onLampState(reg, val); },
      0x01);
}

void HoermannGarageEngine::handleModbus()
{
  mb.task();
}

Modbus::ResultCode HoermannGarageEngine::onRequest(Modbus::FunctionCode fc, const Modbus::RequestData data)
{
  this->state->recordModbusResponse();

  // Command Requst (Internal State representation)
  if (fc == Modbus::FC_READWRITE_REGS && data.regWrite.address == 0x9C41 && data.regWriteCount == 0x02 && data.regRead.address == 0x9CB9 && data.regReadCount == 0x08)
  {
    mb.Reg(HREG(0x9CB9 + 0), (uint16_t)0x0000);
    mb.Reg(HREG(0x9CB9 + 1), (uint16_t)0x0001);
    setCommandValuesToRead();
    // mb.Reg(HREG(0x9CB9+2),(uint16_t)0x0000);
    // mb.Reg(HREG(0x9CB9+3),(uint16_t)0x0000);
    mb.Reg(HREG(0x9CB9 + 4), (uint16_t)0x0000);
    mb.Reg(HREG(0x9CB9 + 5), (uint16_t)0x0000);
    mb.Reg(HREG(0x9CB9 + 6), (uint16_t)0x0000);
    mb.Reg(HREG(0x9CB9 + 7), (uint16_t)0x0000);
  }
  // Empty Command Requst
  else if (fc == Modbus::FC_READWRITE_REGS && data.regWrite.address == 0x9C41 && data.regWriteCount == 0x02 && data.regRead.address == 0x9CB9 && data.regReadCount == 0x02)
  {
    mb.Reg(HREG(0x9CB9 + 0), (uint16_t)0x0004);
    mb.Reg(HREG(0x9CB9 + 1), (uint16_t)0x0000);
    ESP_LOGD(TAG_HCI, "executing empty command");
  }
  // BusScan
  else if (fc == Modbus::FC_READWRITE_REGS && data.regWrite.address == 0x9C41 && data.regWriteCount == 0x03 && data.regRead.address == 0x9CB9 && data.regReadCount == 0x05)
  {
    ESP_LOGD(TAG_HCI, "executing busscan");
    mb.Reg(HREG(0x9CB9 + 0), (uint16_t)0x0000);
    mb.Reg(HREG(0x9CB9 + 1), (uint16_t)0x0005);
    mb.Reg(HREG(0x9CB9 + 2), (uint16_t)0x0430);
    mb.Reg(HREG(0x9CB9 + 3), (uint16_t)0x10ff);
    mb.Reg(HREG(0x9CB9 + 4), (uint16_t)0xa845);
  }
  else if (fc == Modbus::FC_WRITE_REGS && data.reg.address == 0x9D31)
  {
    // ESP_LOGD("ON_REQ", "on Status Update (cnt: %d)",data.regCount);
  }
  else
  {
    this->state->debugMessage = "unknown function code fc=" + fc;
    this->state->debMessage = true;
    ESP_LOGW(TAG_HCI, "unknown function code fc=%x", fc);
  }
  this->state->setValid(true);
  return Modbus::EX_SUCCESS;
}

void HoermannGarageEngine::setCommandValuesToRead()
{
  uint16_t regPlug2Value = 0x0000;
  uint16_t regPlug3Value = 0x0000;

  // Command was set
  if (nextCommand != nullptr)
  {
    // But not yet sent
    if (commandWrittenOn == 0)
    {
      // Send it
      regPlug2Value = nextCommand->commandRegPlus2Value;
      regPlug3Value = nextCommand->commandRegPlus3Value;
      ESP_LOGI(TAG_HCI, "command start %x %x", regPlug2Value, regPlug3Value);
      commandWrittenOn = millis();
      // It was written and it can be cleared
    }
    else if (commandWrittenOn != 0 && (commandWrittenOn + SIMULATEKEYPRESSDELAYMS) < millis())
    {
      regPlug2Value = nextCommand->commandEndPlus2Value;
      regPlug3Value = nextCommand->commandEndPlus3Value;
      ESP_LOGI(TAG_HCI, "command dispose %x %x", regPlug2Value, regPlug3Value);
      // Reset Variables
      commandWrittenOn = 0;
      nextCommand = nullptr;
    }
  }
  mb.Reg(HREG(0x9CB9 + 2), regPlug2Value);
  mb.Reg(HREG(0x9CB9 + 3), regPlug3Value);
}

uint16_t HoermannGarageEngine::onDoorPositonChanged(TRegister *reg, uint16_t val)
{
  // on First Byte changed (current)
  if ((reg->value & 0x00FF) != (val & 0x00FF))
  {
    this->state->setCurrentPosition((float)(val & 0x00FF) / 200.0f);
    if ((this->state->gotoPosition > 0.0f && this->state->state == HoermannState::State::CLOSING && this->state->gotoPosition >= this->state->currentPosition) ||
        (this->state->gotoPosition > 0.0f && this->state->state == HoermannState::State::OPENING && this->state->gotoPosition <= this->state->currentPosition))
    {
      this->stopDoor();
      this->state->setGotoPosition(0.0f);
    }
  }
  // on Second Byte changed (target)
  if ((reg->value & 0xFF00) != (val & 0xFF00))
  {
    this->state->setTargetPosition((float)((val & 0xFF00) >> 8) / 200.0f);
  }

  return val;
}

uint16_t HoermannGarageEngine::onCurrentStateChanged(TRegister *reg, uint16_t val)
{
  // on First Byte changed
  if (((reg->value & 0xFF00) != (val & 0xFF00)))
  {
    ESP_LOGI(TAG_HCI, "onCurrentStateChanged. address=%x, value=%x (actual: %x)", reg->address.address, val, (val & 0xFF00) >> 8);

    switch ((val & 0xFF00) >> 8)
    {
    case 0x1:
      this->state->setState(HoermannState::State::OPENING);
      break;
    case 0x2:
      this->state->setState(HoermannState::State::CLOSING);
      break;
    case 0x20:
      this->state->setState(HoermannState::State::OPEN);
      break;
    case 0x40:
      this->state->setState(HoermannState::State::CLOSED);
      break;
    case 0x80:
      this->state->setState(HoermannState::State::HALFOPEN);
      break;
    case 0x09:
      this->state->setState(HoermannState::State::MOVE_VENTING);
      break;
    case 0x05:
      this->state->setState(HoermannState::State::MOVE_HALF);
      break;
    case 0x0A:
      this->state->setState(HoermannState::State::VENT);
      break;
    case 0x00:
      if (this->state->currentPosition == this->state->targetPosition && (int)(this->state->currentPosition * 200) == VENT_POS)
      {
        this->state->setState(HoermannState::State::VENT);
      }
      else
      {
        this->state->setState(HoermannState::State::STOPPED);
      }
      break;
    default:
      ESP_LOGW(TAG_HCI, "unknown State %x", (val & 0xFF00) >> 8);
    }
  }
  return val;
}

uint16_t HoermannGarageEngine::onLampState(TRegister *reg, uint16_t val)
{
  // On second byte changed
  if ((reg->value & 0x00FF) != (val & 0x00FF))
  {
    ESP_LOGI(TAG_HCI, "onLampState. address=%x, value=%x", reg->address.address, val);
    // 14 .. from docs (a indicator for automatic state maby?)
    // 10 .. on after turn on
    // 04 .. shut down after inactivy
    // 00 .. off after turn off
    this->state->setLigthOn((val & 0x00FF) == 0x14 || (val & 0x00FF) == 0x10);
  }
  return val;
}

/**
 * Write on 0x9C41 , byte1: counter, byte2: command
 */
uint16_t HoermannGarageEngine::onCounterWrite(TRegister *reg, uint16_t val)
{
  uint16_t counter = val & 0xFF00;
  uint16_t command = (val & 0x00FF) << 8;
  mb.Reg(HREG(0x9CB9 + 0), mb.Reg(HREG(0x9CB9 + 0)) | counter);
  mb.Reg(HREG(0x9CB9 + 1), mb.Reg(HREG(0x9CB9 + 1)) | command);
  return val;
}

/**
 * Helper to set next Command and *not* skip Current Command before end was sent
 */
void HoermannGarageEngine::setCommand(bool cond, const HoermannCommand *command)
{
  if (cond)
  {
    if (nextCommand != nullptr)
    {
      ESP_LOGW(TAG_HCI, "Last Command was not yet fetched by modbus!");
    }
    else
    {
      nextCommand = command;
    }
  }
}

/**
 * Control Functions
 */
void HoermannGarageEngine::stopDoor()
{
  setCommand(this->state->state == HoermannState::State::CLOSING || this->state->state == HoermannState::State::OPENING, &HoermannCommand::STARTSTOPDOOR);
}
void HoermannGarageEngine::closeDoor()
{
  setCommand(true, &HoermannCommand::STARTCLOSEDOOR);
}
void HoermannGarageEngine::openDoor()
{
  setCommand(true, &HoermannCommand::STARTOPENDOOR);
}
void HoermannGarageEngine::toogleDoor()
{
  setCommand(this->state->currentPosition < 1, &HoermannCommand::STARTOPENDOOR);
  setCommand(this->state->currentPosition >= 1, &HoermannCommand::STARTCLOSEDOOR);
}
void HoermannGarageEngine::halfPositionDoor()
{
  setCommand(true, &HoermannCommand::STARTOPENDOORHALF);
}
void HoermannGarageEngine::ventilationPositionDoor()
{
  setCommand(true, &HoermannCommand::STARTVENTPOSITION);
}
void HoermannGarageEngine::turnLight(bool on)
{
  setCommand((on && !this->state->lightOn) || (!on && this->state->lightOn), &HoermannCommand::STARTTOGGLELAMP);
}
void HoermannGarageEngine::toogleLight()
{
  setCommand(true, &HoermannCommand::STARTTOGGLELAMP);
}
void HoermannGarageEngine::setPosition(int setPosition)
{
  // First and last movement segments seem a bit inconsistent on Promatic4, so it's better to leave it to fully open or close.
  if (setPosition <= 5)
    closeDoor();
  else if (setPosition >= 95)
    openDoor();
  else if ((setPosition > 5) && (setPosition < 95))
  {
    this->state->setGotoPosition(static_cast<float>(setPosition) / 100.0f);
    setCommand(this->state->currentPosition < this->state->gotoPosition, &HoermannCommand::STARTOPENDOOR);
    setCommand(this->state->currentPosition > this->state->gotoPosition, &HoermannCommand::STARTCLOSEDOOR);
  }
}

void HoermannState::setTargetPosition(float targetPosition)
{
  this->targetPosition = targetPosition;
  this->changed = true;
}
void HoermannState::setGotoPosition(float setPosition)
{
  this->gotoPosition = setPosition;
  this->changed = true;
}
void HoermannState::setCurrentPosition(float currentPosition)
{
  this->currentPosition = currentPosition;
  this->changed = true;
}
void HoermannState::setLigthOn(bool lightOn)
{
  this->lightOn = lightOn;
  this->changed = true;
}
void HoermannState::recordModbusResponse()
{
  this->lastModbusRespone = millis();
}
void HoermannState::clearChanged()
{
  this->changed = false;
}
void HoermannState::clearDebug()
{
  this->debMessage = false;
  this->debugMessage = "Initial";
}
long HoermannState::responseAge()
{
  if (this->lastModbusRespone == 0)
  {
    return -1;
  }
  long diff = millis() - lastModbusRespone;
  if (diff < 0)
  {
    return -2;
  }
  return diff / 1000;
}
void HoermannState::setState(State state)
{
  this->state = state;
  this->changed = true;
}
void HoermannState::setValid(bool isValid)
{
  this->valid = isValid;
}
