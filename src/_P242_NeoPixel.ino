//#######################################################################################################
//#################################### Plugin 242: NeoPixel Advanced ####################################
//#######################################################################################################

// Format: NeoPixelAll,R,G,B,FADE_DELAY,MODE
// R,G,B = 0 - 255
// FADE_DELAY = 0 - 30
// MODE 0 = NORMAL, 1 = RAINBOW TASK

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel *Plugin_242_pixels;

#define PLUGIN_242
#define PLUGIN_ID_242         242
#define PLUGIN_NAME_242       "NeoPixel - Advanced"
#define PLUGIN_VALUENAME1_242 ""

uint8_t Plugin_242_red = 0;
uint8_t Plugin_242_green = 0;
uint8_t Plugin_242_blue = 0;

uint8_t Plugin_242_target_red = 0;
uint8_t Plugin_242_target_green = 0;
uint8_t Plugin_242_target_blue = 0;

uint8_t Plugin_242_rainbow_max = 0;
uint8_t Plugin_242_rainbow_stage = 0;

uint8_t Plugin_242_mode = 0;
uint8_t Plugin_242_saved_mode = 0;

uint8_t Plugin_242_fade_delay = 0; // Since the minimum is 20ms, its 0ms, 20ms or greater.
bool Plugin_242_running = 0;

String name;

boolean Plugin_242(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {

    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_242;
        Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
        Device[deviceCount].VType = SENSOR_TYPE_SINGLE;
        Device[deviceCount].ValueCount = 1;
        //Device[deviceCount].Custom = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].SendDataOption = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_242);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_242));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        char tmpString[128];
        sprintf_P(tmpString, PSTR("<TR><TD>Led Count:<TD><input type='text' name='plugin_242_leds' size='3' value='%u'>"), Settings.TaskDevicePluginConfig[event->TaskIndex][0]);
        string += tmpString;
        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        String plugin1 = WebServer.arg(F("plugin_242_leds"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][0] = plugin1.toInt();
        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
        if (!Plugin_242_pixels)
        {
          Plugin_242_pixels = new Adafruit_NeoPixel(Settings.TaskDevicePluginConfig[event->TaskIndex][0], Settings.TaskDevicePin1[event->TaskIndex], NEO_GRB + NEO_KHZ800);
          Plugin_242_pixels->begin(); // This initializes the NeoPixel library.

          // Turn of the LED on startup.
          Plugin_242_target_red = 0;
          Plugin_242_target_green = 0;
          Plugin_242_target_blue = 0;
          Plugin_242_mode = 0;
          Plugin_242_running = true;
        }
        success = true;
        break;
      }

    case PLUGIN_FIFTY_PER_SECOND:
    {
      String log = "";

      if(Plugin_242_running)
      {
        switch(Plugin_242_mode)
        {
          case 0: // Fader
          {
            SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0], true);
            Plugin_242_running = false;
            SendState(event, 0);
            break;
          }
          case 1:
          {
            if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0], false))
            {
              // Task completed
              Plugin_242_running = false;
              log = F("NeoPixel Advanced  : Task completed - ");
              log += Plugin_242_red;
              log += " ";
              log += Plugin_242_green;
              log += " ";
              log += Plugin_242_blue;
              addLog(LOG_LEVEL_INFO, log);
              SendState(event, 0);
            }
            break;
          }
          case 2: // Rainbow
          {
            if(Plugin_242_rainbow_stage == 0) // Red
            {
              Plugin_242_target_red = Plugin_242_rainbow_max;Plugin_242_target_green = 0;Plugin_242_target_blue = 0;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0], false))
              {
                Plugin_242_rainbow_stage++;
                SendState(event, 0);
              }
            }
            else if(Plugin_242_rainbow_stage == 1) // Orange
            {
              Plugin_242_target_red = Plugin_242_rainbow_max;Plugin_242_target_green = (uint8_t)(Plugin_242_rainbow_max*0.20);Plugin_242_target_blue = 0;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0], false))
              {
                Plugin_242_rainbow_stage++;
                SendState(event, 0);
              }
            }
            else if(Plugin_242_rainbow_stage == 2) // Yellow
            {
              Plugin_242_target_red = Plugin_242_rainbow_max;Plugin_242_target_green = Plugin_242_rainbow_max;Plugin_242_target_blue = 0;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0], false))
              {
                Plugin_242_rainbow_stage++;
                SendState(event, 0);
              }
            }
            else if(Plugin_242_rainbow_stage == 3) // Green
            {
              Plugin_242_target_red = 0;Plugin_242_target_green = Plugin_242_rainbow_max;Plugin_242_target_blue = 0;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0], false))
              {
                Plugin_242_rainbow_stage++;
                SendState(event, 0);
              }
            }
            else if(Plugin_242_rainbow_stage == 4) // Blue
            {
              Plugin_242_target_red = 0;Plugin_242_target_green = 0;Plugin_242_target_blue = Plugin_242_rainbow_max;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0], false))
              {
                Plugin_242_rainbow_stage++;
                SendState(event, 0);
              }
            }
            else if(Plugin_242_rainbow_stage == 5) // Purple
            {
              Plugin_242_target_red = Plugin_242_rainbow_max;Plugin_242_target_green = 0;Plugin_242_target_blue = Plugin_242_rainbow_max;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0], false))
              {
                Plugin_242_rainbow_stage++;
                SendState(event, 0);
              }
            }
            else if(Plugin_242_rainbow_stage == 6) // Pink
            {
              Plugin_242_target_red = Plugin_242_rainbow_max;Plugin_242_target_green = (uint8_t)(Plugin_242_rainbow_max*.41);Plugin_242_target_blue = (uint8_t)(Plugin_242_rainbow_max*.7);
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0], false))
              {
                Plugin_242_rainbow_stage++;
                SendState(event, 0);
              }
            }
            else
            {
              Plugin_242_rainbow_stage = 0;
            }

            break;
          }
        }
      }

      break;
    }

    case PLUGIN_WRITE:
      {
        String log1 = F("P242 - PLUGIN_WRITE reached. ");
        addLog(LOG_LEVEL_INFO, log1);
        addLog(LOG_LEVEL_INFO, ExtraTaskSettings.TaskDeviceName);

        if (Plugin_242_pixels && event->sensorType == 254)
        {
          String log = F("MQTT: ");
          getTaskIndexByName(event->String1);

          if (event->String1 == ExtraTaskSettings.TaskDeviceName)
          {
            log += " ";
            log += event->String1;
            log += " ";
            log += event->String2;
            addLog(LOG_LEVEL_INFO, log);

            StaticJsonBuffer<200> jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(event->String2.c_str());

            if (!root.success())
            {
                log = "NeoPixel: Failed to parse JSON string.";
                addLog(LOG_LEVEL_INFO, log);
                success = false;
                break;
            }

            const char* state = root[F("state")];
            const char* effect = root[F("effect")];
            uint8_t brightness = root[F("brightness")];
            uint8_t color_temp = root[F("color_temp")];
            uint8_t r = root[F("color")][F("r")];
            uint8_t g = root[F("color")][F("g")];
            uint8_t b = root[F("color")][F("b")];
            uint8_t transition = root[F("transition")];
            uint8_t white_value = root[F("white_value")];

            if(root.containsKey("color"))
            {
              Plugin_242_target_red = r;
              Plugin_242_target_green = g;
              Plugin_242_target_blue = b;
            }
            else if(!root.containsKey("color") && root.containsKey("brightness") && (strcasecmp_P(state, PSTR("on")) == 0))
            {
              Plugin_242_target_red = brightness;
              Plugin_242_target_green = brightness;
              Plugin_242_target_blue = brightness;

              Plugin_242_rainbow_max = brightness;
            }
            else if(!root.containsKey("color") && (strcasecmp_P(state, PSTR("on")) == 0))
            {
              Plugin_242_target_red = 255;
              Plugin_242_target_green = 255;
              Plugin_242_target_blue = 255;
            }

            if(root.containsKey("effect"))
            {
              if (strcasecmp_P(effect, PSTR("normal")) == 0)
                Plugin_242_mode = 0;
              else if (strcasecmp_P(effect, PSTR("fade")) == 0)
                Plugin_242_mode = 1;
              else if (strcasecmp_P(effect, PSTR("rainbow")) == 0)
                Plugin_242_mode = 2;
              else
                Plugin_242_mode = 1;

              Plugin_242_saved_mode = Plugin_242_mode;
            }
            else
            {
              Plugin_242_mode = Plugin_242_saved_mode;
            }

            if (strcasecmp_P(state, PSTR("on")) == 0)
            {
              log = "NeoPixel: Turning light ON.";
              log += " ";
              log += Plugin_242_target_red;
              log += " ";
              log += Plugin_242_target_green;
              log += " ";
              log += Plugin_242_target_blue;
              log += " ";
              addLog(LOG_LEVEL_INFO, log);

              Plugin_242_running = true;

            }
            else if (strcasecmp_P(state, PSTR("off")) == 0)
            {
              log = "NeoPixel: Turning light OFF.";
              addLog(LOG_LEVEL_INFO, log);

              Plugin_242_target_red = 0;
              Plugin_242_target_green = 0;
              Plugin_242_target_blue = 0;

              if( Plugin_242_mode != 0 || Plugin_242_mode != 1)
              {
                Plugin_242_mode = 0;
              }

              Plugin_242_running = true;
            }
          }
          success = true;
        }
        break;
      }
  } /* switch (function) */

  return success;
}

void SendState(struct EventStruct *e, uint8_t type)
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  String str;

  if (type == 0)
  {
    if (Plugin_242_red == 0 && Plugin_242_green == 0 && Plugin_242_blue == 0)
    {
      root[F("state")] = String(F("OFF"));
    }
    else if ((Plugin_242_red == Plugin_242_green && Plugin_242_red == Plugin_242_blue) &&
            (Plugin_242_green == Plugin_242_red && Plugin_242_green == Plugin_242_blue) &&
            (Plugin_242_blue == Plugin_242_red && Plugin_242_blue == Plugin_242_green))
    {
      root[F("brightness")] = Plugin_242_red;
      JsonObject& color = root.createNestedObject("color");
      color[F("r")] = Plugin_242_red;
      color[F("g")] = Plugin_242_green;
      color[F("b")] = Plugin_242_blue;
      root[F("state")] = String(F("ON"));
    }
    else
    {
      JsonObject& color = root.createNestedObject("color");
      color[F("r")] = Plugin_242_red;
      color[F("g")] = Plugin_242_green;
      color[F("b")] = Plugin_242_blue;
      root[F("state")] = String(F("ON"));
    }
  }
  else if (type == 1)
  {
    root[F("state")] = String(F("ON"));
  }
  else if (type == 2)
  {
    root[F("state")] = String(F("OFF"));
  }

  root.printTo(str);
  e->String3 = str;
  e->sensorType = 254;
  addLog(LOG_LEVEL_INFO, e->String3);
  sendData(e);
}

bool SetFadePixels(uint8_t r, uint8_t g, uint8_t b, uint8_t leds, bool force)
{

  String log = F("NeoPixel Advanced  : Task running - ");
  log += Plugin_242_red;
  log += " | ";
  log += r;
  log += " ";
  log += Plugin_242_green;
  log += " | ";
  log += g;
  log += " ";
  log += Plugin_242_blue;
  log += " | ";
  log += b;

  //addLog(LOG_LEVEL_INFO, log);

  if(force) {
    log = "NeoPixel: Forced SetFadePixels";
    addLog(LOG_LEVEL_INFO, log);

    Plugin_242_red = r;
    Plugin_242_green = g;
    Plugin_242_blue = b;

    for (int i = 0; i < leds; i++)
    {
      Plugin_242_pixels->setPixelColor(i, Plugin_242_pixels->Color(r, g, b));
    }
    Plugin_242_pixels->show();
    return true;
  }

  if((Plugin_242_red == r) && (Plugin_242_green == g) && (Plugin_242_blue == b))
  {
    //log = "NeoPixel: Range reached";
    //addLog(LOG_LEVEL_INFO, log);
    return true;
  }
  else
  {
    if(Plugin_242_red < r)
      Plugin_242_red++;
    else if (Plugin_242_red == r)
      Plugin_242_red = r;
    else
      Plugin_242_red--;

    if(Plugin_242_green < g)
      Plugin_242_green++;
    else if(Plugin_242_green == g)
      Plugin_242_green = g;
    else
      Plugin_242_green--;

    if(Plugin_242_blue < b)
      Plugin_242_blue++;
    else if ( Plugin_242_blue == b )
      Plugin_242_blue = b;
    else
      Plugin_242_blue--;
  }

  for (int i = 0; i < leds; i++)
  {
    Plugin_242_pixels->setPixelColor(i, Plugin_242_pixels->Color(Plugin_242_red, Plugin_242_green, Plugin_242_blue));
  }
  Plugin_242_pixels->show();

  return false;
}
