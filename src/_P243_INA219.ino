//#######################################################################################################
//######################### Plugin 243: INA219 DC Voltage/Current sensor ################################
//#######################################################################################################

#define PLUGIN_243
#define PLUGIN_ID_243         243
#define PLUGIN_NAME_243       "INA219 (custom)"
#define PLUGIN_VALUENAME1_243 "Voltage"

#define P243_INA219_ADDRESS                         (0x40)    // 1000000 (A0+A1=GND)
#define P243_INA219_ADDRESS2                         (0x41)    // 1000000 (A0+A1=GND)
#define P243_INA219_ADDRESS3                         (0x44)    // 1000000 (A0+A1=GND)
#define P243_INA219_ADDRESS4                         (0x45)    // 1000000 (A0+A1=GND)
#define P243_INA219_READ                            (0x01)
#define P243_INA219_REG_CONFIG                      (0x00)
#define P243_INA219_CONFIG_RESET                    (0x8000)  // Reset Bit

#define P243_INA219_CONFIG_BVOLTAGERANGE_MASK       (0x2000)  // Bus Voltage Range Mask
#define P243_INA219_CONFIG_BVOLTAGERANGE_16V        (0x0000)  // 0-16V Range
#define P243_INA219_CONFIG_BVOLTAGERANGE_32V        (0x2000)  // 0-32V Range

#define P243_INA219_CONFIG_GAIN_MASK                (0x1800)  // Gain Mask
#define P243_INA219_CONFIG_GAIN_1_40MV              (0x0000)  // Gain 1, 40mV Range
#define P243_INA219_CONFIG_GAIN_2_80MV              (0x0800)  // Gain 2, 80mV Range
#define P243_INA219_CONFIG_GAIN_4_160MV             (0x1000)  // Gain 4, 160mV Range
#define P243_INA219_CONFIG_GAIN_8_320MV             (0x1800)  // Gain 8, 320mV Range

#define P243_INA219_CONFIG_BADCRES_MASK             (0x0780)  // Bus ADC Resolution Mask
#define P243_INA219_CONFIG_BADCRES_9BIT             (0x0080)  // 9-bit bus res = 0..511
#define P243_INA219_CONFIG_BADCRES_10BIT            (0x0100)  // 10-bit bus res = 0..1023
#define P243_INA219_CONFIG_BADCRES_11BIT            (0x0200)  // 11-bit bus res = 0..2047
#define P243_INA219_CONFIG_BADCRES_12BIT            (0x0400)  // 12-bit bus res = 0..4097

#define P243_INA219_CONFIG_SADCRES_MASK             (0x0078)  // Shunt ADC Resolution and Averaging Mask
#define P243_INA219_CONFIG_SADCRES_9BIT_1S_84US     (0x0000)  // 1 x 9-bit shunt sample
#define P243_INA219_CONFIG_SADCRES_10BIT_1S_148US   (0x0008)  // 1 x 10-bit shunt sample
#define P243_INA219_CONFIG_SADCRES_11BIT_1S_276US   (0x0010)  // 1 x 11-bit shunt sample
#define P243_INA219_CONFIG_SADCRES_12BIT_1S_532US   (0x0018)  // 1 x 12-bit shunt sample
#define P243_INA219_CONFIG_SADCRES_12BIT_2S_1060US  (0x0048)	 // 2 x 12-bit shunt samples averaged together
#define P243_INA219_CONFIG_SADCRES_12BIT_4S_2130US  (0x0050)  // 4 x 12-bit shunt samples averaged together
#define P243_INA219_CONFIG_SADCRES_12BIT_8S_4260US  (0x0058)  // 8 x 12-bit shunt samples averaged together
#define P243_INA219_CONFIG_SADCRES_12BIT_16S_8510US (0x0060)  // 16 x 12-bit shunt samples averaged together
#define P243_INA219_CONFIG_SADCRES_12BIT_32S_17MS   (0x0068)  // 32 x 12-bit shunt samples averaged together
#define P243_INA219_CONFIG_SADCRES_12BIT_64S_34MS   (0x0070)  // 64 x 12-bit shunt samples averaged together
#define P243_INA219_CONFIG_SADCRES_12BIT_128S_69MS  (0x0078)  // 128 x 12-bit shunt samples averaged together

#define P243_INA219_CONFIG_MODE_MASK                (0x0007)  // Operating Mode Mask
#define P243_INA219_CONFIG_MODE_POWERDOWN           (0x0000)
#define P243_INA219_CONFIG_MODE_SVOLT_TRIGGERED     (0x0001)
#define P243_INA219_CONFIG_MODE_BVOLT_TRIGGERED     (0x0002)
#define P243_INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED (0x0003)
#define P243_INA219_CONFIG_MODE_ADCOFF              (0x0004)
#define P243_INA219_CONFIG_MODE_SVOLT_CONTINUOUS    (0x0005)
#define P243_INA219_CONFIG_MODE_BVOLT_CONTINUOUS    (0x0006)
#define P243_INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS (0x0007)

#define P243_INA219_REG_SHUNTVOLTAGE                (0x01)
#define P243_INA219_REG_BUSVOLTAGE                  (0x02)
#define P243_INA219_REG_POWER                       (0x03)
#define P243_INA219_REG_CURRENT                     (0x04)
#define P243_INA219_REG_CALIBRATION                 (0x05)

uint8_t p243_ina219_i2caddr[TASKS_MAX];
uint32_t p243_ina219_calValue[TASKS_MAX];
// The following multipliers are used to convert raw current and power
// values to mA and mW, taking into account the current config settings
uint32_t p243_ina219_currentDivider_mA[TASKS_MAX];

boolean Plugin_243(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {

    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_243;
        Device[deviceCount].Type = DEVICE_TYPE_I2C;
        Device[deviceCount].VType = SENSOR_TYPE_QUAD;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = true;
        Device[deviceCount].ValueCount = 4;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].GlobalSyncOption = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_243);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_243));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        byte choice2 = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        String options2[4];
        options2[0] = F("0x40 - (default)");
        options2[1] = F("0x41");
        options2[2] = F("0x44");
        options2[3] = F("0x45");
        int optionValues2[3];
        optionValues2[0] = P243_INA219_ADDRESS;
        optionValues2[1] = P243_INA219_ADDRESS2;
        optionValues2[2] = P243_INA219_ADDRESS3;
        optionValues2[3] = P243_INA219_ADDRESS4;
        string += F("<TR><TD>I2C Address:<TD><select name='Plugin_243_i2c'>");
        for (byte x = 0; x < 4; x++)
        {
          string += F("<option value='");
          string += optionValues2[x];
          string += "'";
          if (choice2 == optionValues2[x])
            string += F(" selected");
          string += ">";
          string += options2[x];
          string += F("</option>");
        }
        string += F("</select>");

        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        String plugin1 = WebServer.arg(F("Plugin_243_value"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][0] = plugin1.toInt();

        String plugin2 = WebServer.arg(F("Plugin_243_i2c"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][1] = plugin2.toInt();
        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
      	p243_ina219_i2caddr[event->TaskIndex] = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        Plugin_243_begin(event);
        success = true;
        break;
      }

    case PLUGIN_READ:
      {
        // shuntvoltage = Plugin_243_getShuntVoltage_mV();
        // busvoltage = Plugin_243_getBusVoltage_V();
        // current_mA = Plugin_243_getCurrent_mA();
        // loadvoltage = Plugin_243_getBusVoltage_V() + (Plugin_243_getShuntVoltage_mV() / 1000);
        float bus_voltage=0.0;
        float shunt_voltage=0.0;
        float load_voltage=0.0;
        float current=0.0;
        float power=0.0;

        bus_voltage = Plugin_243_getBusVoltage_V(event);
        shunt_voltage = Plugin_243_getShuntVoltage_mV(event);
        load_voltage = bus_voltage + (shunt_voltage / 1000);
        current = Plugin_243_getCurrent_mA(event);
        power = ((bus_voltage) * (current/1000))*1000;

        UserVar[event->BaseVarIndex] = bus_voltage;
        UserVar[event->BaseVarIndex+1] = load_voltage;
        UserVar[event->BaseVarIndex+2] = current;
        UserVar[event->BaseVarIndex+3] = power;

        String log = F("INA ");
        log += event->TaskIndex;
        log += " : bus voltage: ";
        log += bus_voltage;
        log += " load voltage: ";
        log += load_voltage;
        log += " current: ";
        log += current;
        log += " power: ";
        log += power;
        addLog(LOG_LEVEL_INFO,log);
        success = true;
        break;
      }
  }
  return success;
}

//**************************************************************************/
// Sends a single command byte over I2C
//**************************************************************************/
void Plugin_243_wireWriteRegister (uint8_t reg, uint16_t value, struct EventStruct *event)
{
  Wire.beginTransmission(p243_ina219_i2caddr[event->TaskIndex]);
  Wire.write(reg);                       // Register
  Wire.write((value >> 8) & 0xFF);       // Upper 8-bits
  Wire.write(value & 0xFF);              // Lower 8-bits
  Wire.endTransmission();
}

//**************************************************************************/
// Reads a 16 bit values over I2C
//**************************************************************************/
void Plugin_243_wireReadRegister(uint8_t reg, uint16_t *value, struct EventStruct *event)
{

  Wire.beginTransmission(p243_ina219_i2caddr[event->TaskIndex]);
  Wire.write(reg);                       // Register
  Wire.endTransmission();

  delay(1); // Max 12-bit conversion time is 586us per sample

  Wire.requestFrom(p243_ina219_i2caddr[event->TaskIndex], (uint8_t)2);
  // Shift values to create properly formed integer
  *value = ((Wire.read() << 8) | Wire.read());
}

//**************************************************************************/
// Configures to INA219 to be able to measure up to 32V and 2A
/**************************************************************************/
void Plugin_243_setCalibration_32V_2A(struct EventStruct *event)
{
  p243_ina219_calValue[event->TaskIndex] = 4027;

  // Set multipliers to convert raw current/power values
  p243_ina219_currentDivider_mA[event->TaskIndex] = 10;  // Current LSB = 100uA per bit (1000/100 = 10)

  // Set Calibration register to 'Cal' calculated above
  Plugin_243_wireWriteRegister(P243_INA219_REG_CALIBRATION, p243_ina219_calValue[event->TaskIndex], event);

  // Set Config register to take into account the settings above
  uint16_t config = P243_INA219_CONFIG_BVOLTAGERANGE_32V |
                    P243_INA219_CONFIG_GAIN_8_320MV |
                    P243_INA219_CONFIG_BADCRES_12BIT |
                    P243_INA219_CONFIG_SADCRES_12BIT_1S_532US |
                    P243_INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  Plugin_243_wireWriteRegister(P243_INA219_REG_CONFIG, config, event);
}

//**************************************************************************/
// Configures to INA219 to be able to measure up to 32V and 1A
//**************************************************************************/
void Plugin_243_setCalibration_32V_1A(struct EventStruct *event)
{

  p243_ina219_calValue[event->TaskIndex] = 10240;

  // Set multipliers to convert raw current/power values
  p243_ina219_currentDivider_mA[event->TaskIndex] = 25;      // Current LSB = 40uA per bit (1000/40 = 25)

  // Set Calibration register to 'Cal' calculated above
  Plugin_243_wireWriteRegister(P243_INA219_REG_CALIBRATION, p243_ina219_calValue[event->TaskIndex], event);

  // Set Config register to take into account the settings above
  uint16_t config = P243_INA219_CONFIG_BVOLTAGERANGE_32V |
                    P243_INA219_CONFIG_GAIN_8_320MV |
                    P243_INA219_CONFIG_BADCRES_12BIT |
                    P243_INA219_CONFIG_SADCRES_12BIT_1S_532US |
                    P243_INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  Plugin_243_wireWriteRegister(P243_INA219_REG_CONFIG, config, event);
}

//**************************************************************************/
// Configures to INA219 to be able to measure up to 16V and 400mA
//**************************************************************************/
void Plugin_243_setCalibration_16V_400mA(struct EventStruct *event) {

  p243_ina219_calValue[event->TaskIndex] = 8192;

  // Set multipliers to convert raw current/power values
  p243_ina219_currentDivider_mA[event->TaskIndex] = 20;  // Current LSB = 50uA per bit (1000/50 = 20)

  // Set Calibration register to 'Cal' calculated above
  Plugin_243_wireWriteRegister(P243_INA219_REG_CALIBRATION, p243_ina219_calValue[event->TaskIndex], event);

  // Set Config register to take into account the settings above
  uint16_t config = P243_INA219_CONFIG_BVOLTAGERANGE_16V |
                    P243_INA219_CONFIG_GAIN_1_40MV |
                    P243_INA219_CONFIG_BADCRES_12BIT |
                    P243_INA219_CONFIG_SADCRES_12BIT_1S_532US |
                    P243_INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  Plugin_243_wireWriteRegister(P243_INA219_REG_CONFIG, config, event);
}


//**************************************************************************/
//  Setups the HW (defaults to 32V and 2A for calibration values)
//**************************************************************************/

void Plugin_243_begin(struct EventStruct *event) {
  p243_ina219_currentDivider_mA[event->TaskIndex] = 0;

  // Set chip to large range config values to start
  Plugin_243_setCalibration_32V_2A(event);
}

//**************************************************************************/
// Gets the raw bus voltage (16-bit signed integer, so +-32767)
//**************************************************************************/
int16_t Plugin_243_getBusVoltage_raw(struct EventStruct *event) {
  uint16_t value;
  Plugin_243_wireReadRegister(P243_INA219_REG_BUSVOLTAGE, &value, event);

  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  return (int16_t)((value >> 3) * 4);
}

//**************************************************************************/
// Gets the raw shunt voltage (16-bit signed integer, so +-32767)
//**************************************************************************/
int16_t Plugin_243_getShuntVoltage_raw(struct EventStruct *event) {
  uint16_t value;
  Plugin_243_wireReadRegister(P243_INA219_REG_SHUNTVOLTAGE, &value, event);
  return (int16_t)value;
}

//**************************************************************************/
// Gets the raw current value (16-bit signed integer, so +-32767)
//**************************************************************************/
int16_t Plugin_243_getCurrent_raw(struct EventStruct *event) {
  uint16_t value;

  // Sometimes a sharp load will reset the INA219, which will
  // reset the cal register, meaning CURRENT and POWER will
  // not be available ... avoid this by always setting a cal
  // value even if it's an unfortunate extra step
  Plugin_243_wireWriteRegister(P243_INA219_REG_CALIBRATION, p243_ina219_calValue[event->TaskIndex], event);

  // Now we can safely read the CURRENT register!
  Plugin_243_wireReadRegister(P243_INA219_REG_CURRENT, &value, event);

  return (int16_t)value;
}

//**************************************************************************/
// Gets the shunt voltage in mV (so +-327mV)
//**************************************************************************/
float Plugin_243_getShuntVoltage_mV(struct EventStruct *event) {
  int16_t value;
  value = Plugin_243_getShuntVoltage_raw(event);
  return value * 0.01;
}

//**************************************************************************/
// Gets the shunt voltage in volts
//**************************************************************************/
float Plugin_243_getBusVoltage_V(struct EventStruct *event) {
  int16_t value = Plugin_243_getBusVoltage_raw(event);
  return value * 0.001;
}

//**************************************************************************/
// Gets the current value in mA, taking into account the
//            config settings and current LSB
//**************************************************************************/
float Plugin_243_getCurrent_mA(struct EventStruct *event) {
  float valueDec = Plugin_243_getCurrent_raw(event);
  valueDec /= p243_ina219_currentDivider_mA[event->TaskIndex];
  return valueDec;
}
