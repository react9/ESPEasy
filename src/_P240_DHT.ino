//#######################################################################################################
//######################## Plugin 245: Temperature and Humidity sensor DHT 11/22 ########################
//#######################################################################################################

#define PLUGIN_240
#define PLUGIN_ID_240         240
#define PLUGIN_NAME_240       "Environment - DHT11/12/22 - Custom"
#define PLUGIN_VALUENAME1_240 "Temperature"
#define PLUGIN_VALUENAME2_240 "Humidity"

#include "DHT.h"

uint8_t Plugin_240_DHT_Pin;
uint8_t Plugin_240_En_Pin;
DHT *Plugin_240_DHT;

boolean Plugin_240(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_240;
        Device[deviceCount].Type = DEVICE_TYPE_DUAL;
        Device[deviceCount].VType = SENSOR_TYPE_TEMP_HUM;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = true;
        Device[deviceCount].ValueCount = 2;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].GlobalSyncOption = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_240);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_240));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_240));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        byte choice = Settings.TaskDevicePluginConfig[event->TaskIndex][0];
        byte enable = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        byte invert = Settings.TaskDevicePluginConfig[event->TaskIndex][2];

        String options[3];
        options[0] = F("DHT 11");
        options[1] = F("DHT 22");
        options[2] = F("DHT 12");
        int optionValues[3];
        optionValues[0] = 11;
        optionValues[1] = 22;
        optionValues[2] = 12;
        string += F("<TR><TD>DHT Type:<TD><select name='plugin_240_dhttype'>");
        for (byte x = 0; x < 3; x++)
        {
          string += F("<option value='");
          string += optionValues[x];
          string += "'";
          if (choice == optionValues[x])
            string += F(" selected");
          string += ">";
          string += options[x];
          string += F("</option>");
        }
        string += F("</select>");

        string += F("<TR><TD>DHT Use Enable Pin:<TD><select name='plugin_240_enable'>");
        string += F("<option value='0'");
        if (enable == 0)
        {
          string += F(" selected");
        }
        string += ">";
        string += F("No </option>");
        string += F("<option value='1'");
        if (enable == 1)
        {
          string += F(" selected");
        }
        string += ">";
        string += F("Yes </option>");

        string += F("<TR><TD>DHT Invert Enable Pin:<TD><select name='plugin_240_invert'>");
        string += F("<option value='0'");
        if (invert == 0)
        {
          string += F(" selected");
        }
        string += ">";
        string += F("No </option>");
        string += F("<option value='1'");
        if (invert == 1)
        {
          string += F(" selected");
        }
        string += ">";
        string += F("Yes </option>");

        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        Settings.TaskDevicePluginConfig[event->TaskIndex][0] = getFormItemInt(F("plugin_240_dhttype"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][1] = getFormItemInt(F("plugin_240_enable"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][2] = getFormItemInt(F("plugin_240_invert"));
        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
        pinMode(Settings.TaskDevicePin2[event->TaskIndex], OUTPUT);
        Plugin_240_DHT_Pin = Settings.TaskDevicePin1[event->TaskIndex];
        Plugin_240_En_Pin = Settings.TaskDevicePin2[event->TaskIndex];

        uint8_t type = Settings.TaskDevicePluginConfig[event->TaskIndex][0];

        if(!Plugin_240_DHT)
        {
          Plugin_240_DHT = new DHT(Plugin_240_DHT_Pin, type);
          Plugin_240_DHT->begin();
        }

        success = true;
        break;
      }

    case PLUGIN_READ:
      {
        uint8_t enable = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        uint8_t invert = Settings.TaskDevicePluginConfig[event->TaskIndex][2];

        float temperature = NAN;
        float humidity = NAN;

        if(enable && invert == 0)
        {
          digitalWrite(Plugin_240_En_Pin, HIGH);
          delay(1500);
        }
        else if(enable && invert == 1)
        {
          digitalWrite(Plugin_240_En_Pin, LOW);
          delay(1500);
        }

        humidity = Plugin_240_DHT->readHumidity();
        temperature = Plugin_240_DHT->readTemperature();

        if(!isnan(humidity) && !isnan(temperature)) {
          UserVar[event->BaseVarIndex] = temperature;
          UserVar[event->BaseVarIndex + 1] = humidity;
          String log = F("DHT  : Temperature: ");
          log += UserVar[event->BaseVarIndex];
          addLog(LOG_LEVEL_INFO, log);
          log = F("DHT  : Humidity: ");
          log += UserVar[event->BaseVarIndex + 1];
          addLog(LOG_LEVEL_INFO, log);
          success = true;
        }

        if(!success)
        {
          addLog(LOG_LEVEL_INFO, F("DHT  : No reading!"));
          UserVar[event->BaseVarIndex] = NAN;
          UserVar[event->BaseVarIndex + 1] = NAN;
        }

        if(enable && invert == 0)
        {
          digitalWrite(Plugin_240_En_Pin, LOW);
        }
        else if(enable && invert == 1)
        {
          digitalWrite(Plugin_240_En_Pin, HIGH);
        }

        break;
      }
  }
  return success;
}
