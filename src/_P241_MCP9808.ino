//#######################################################################################################
//######################## Plugin 241: Temperature sensor MCP9808 (I2C) #################################
//#######################################################################################################

#define PLUGIN_241
#define PLUGIN_ID_241         241
#define PLUGIN_NAME_241       "Environment - MCP9808 (I2C)"
#define PLUGIN_VALUENAME1_241 "Temperature"

#include "Adafruit_MCP9808.h"

Adafruit_MCP9808 *Plugin_241_MCP9808;
boolean Plugin_241_init = false;

boolean Plugin_241(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_241;
        Device[deviceCount].Type = DEVICE_TYPE_I2C;
        Device[deviceCount].VType = SENSOR_TYPE_SINGLE;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = true;
        Device[deviceCount].ValueCount = 1;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].GlobalSyncOption = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_241);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_241));
        break;
      }

      case PLUGIN_INIT:
        {

          if(!Plugin_241_MCP9808)
          {
            Plugin_241_MCP9808 = new Adafruit_MCP9808();
            if(!Plugin_241_MCP9808->begin())
            {
              String log = F("MCP9808 sensor not found.");
              addLog(LOG_LEVEL_ERROR, log);
            }
          }

          success = false;

          break;
        }

    case PLUGIN_READ:
      {

        Plugin_241_MCP9808->shutdown_wake(0);
        float temperature = Plugin_241_MCP9808->readTempC();
        if(!isnan(temperature)) {
          UserVar[event->BaseVarIndex] = temperature;
          String log = F("MCP9808  : Temperature: ");
          log += UserVar[event->BaseVarIndex];
          addLog(LOG_LEVEL_INFO, log);
          success = true;
        }

        if(!success)
        {
          addLog(LOG_LEVEL_INFO, F("MCP9808  : No reading!"));
          UserVar[event->BaseVarIndex] = NAN;
        }
        delay(250);

        Plugin_241_MCP9808->shutdown_wake(1);
        break;
      }
  }
  return success;
}
