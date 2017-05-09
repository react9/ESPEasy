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
uint8_t Plugin_242_fade_delay = 0; // Since the minimum is 20ms, its 0ms, 20ms or greater.
bool Plugin_242_running = 0;

boolean Plugin_242(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {

    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_242;
        Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
        Device[deviceCount].Custom = true;
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

        string += F("<TR><TD>GPIO:<TD>");
        addPinSelect(false, string, "taskdevicepin1", Settings.TaskDevicePin1[event->TaskIndex]);

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
        }
        success = true;
        break;
      }

    case PLUGIN_FIFTY_PER_SECOND:
    {
      if(Plugin_242_running)
      {
        switch(Plugin_242_mode)
        {
          case 0: // Fader
          {
            if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0]))
            {
              // Task completed
              Plugin_242_running = false;
              String log = F("NeoPixel Advanced  : Task completed - ");
              log += Plugin_242_red;
              log += " ";
              log += Plugin_242_green;
              log += " ";
              log += Plugin_242_blue;
              addLog(LOG_LEVEL_INFO, log);
            }
            break;
          }
          case 1: // Rainbow
          {
            if(Plugin_242_rainbow_stage == 0) // Red
            {
              Plugin_242_target_red = Plugin_242_rainbow_max;Plugin_242_target_green = 0;Plugin_242_target_blue = 0;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0]))
              {
                Plugin_242_rainbow_stage++;
              }
            }
            else if(Plugin_242_rainbow_stage == 1) // Orange
            {
              Plugin_242_target_red = Plugin_242_rainbow_max;Plugin_242_target_green = (uint8_t)(Plugin_242_rainbow_max*0.20);Plugin_242_target_blue = 0;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0]))
              {
                Plugin_242_rainbow_stage++;
              }
            }
            else if(Plugin_242_rainbow_stage == 2) // Yellow
            {
              Plugin_242_target_red = Plugin_242_rainbow_max;Plugin_242_target_green = Plugin_242_rainbow_max;Plugin_242_target_blue = 0;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0]))
              {
                Plugin_242_rainbow_stage++;
              }
            }
            else if(Plugin_242_rainbow_stage == 3) // Green
            {
              Plugin_242_target_red = 0;Plugin_242_target_green = Plugin_242_rainbow_max;Plugin_242_target_blue = 0;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0]))
              {
                Plugin_242_rainbow_stage++;
              }
            }
            else if(Plugin_242_rainbow_stage == 4) // Blue
            {
              Plugin_242_target_red = 0;Plugin_242_target_green = 0;Plugin_242_target_blue = Plugin_242_rainbow_max;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0]))
              {
                Plugin_242_rainbow_stage++;
              }
            }
            else if(Plugin_242_rainbow_stage == 5) // Purple
            {
              Plugin_242_target_red = Plugin_242_rainbow_max;Plugin_242_target_green = 0;Plugin_242_target_blue = Plugin_242_rainbow_max;
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0]))
              {
                Plugin_242_rainbow_stage++;
              }
            }
            else if(Plugin_242_rainbow_stage == 6) // Pink
            {
              Plugin_242_target_red = Plugin_242_rainbow_max;Plugin_242_target_green = (uint8_t)(Plugin_242_rainbow_max*.41);Plugin_242_target_blue = (uint8_t)(Plugin_242_rainbow_max*.7);
              if(SetFadePixels(Plugin_242_target_red, Plugin_242_target_green, Plugin_242_target_blue, Settings.TaskDevicePluginConfig[event->TaskIndex][0]))
              {
                Plugin_242_rainbow_stage++;
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
        if (Plugin_242_pixels)
        {
          String tmpString  = string;
          int argIndex = tmpString.indexOf(',');
          if (argIndex)
            tmpString = tmpString.substring(0, argIndex);

          if (tmpString.equalsIgnoreCase(F("NeoPixelAll")))
				  {
					  char Line[80];
					  char TmpStr1[80];
            int Par4 = 0;
	  				int Par5 = 0;

					  TmpStr1[0] = 0;
					  string.toCharArray(Line, 80);

		  			if (GetArgv(Line, TmpStr1, 5)) Par4 = str2int(TmpStr1);
			  		if (GetArgv(Line, TmpStr1, 6)) Par5 = str2int(TmpStr1);

            if(Par4 <= 0) Par4 = 0;
            if(Par4 >= 30) Par4 = 30;

            if(Par5 <= 0) Par5 = 0;
            if(Par5 >= 1) Par5 = 1;

            Plugin_242_fade_delay = Par4;
            Plugin_242_mode = Par5;

            Plugin_242_target_red = event->Par1;
            Plugin_242_target_green = event->Par2;
            Plugin_242_target_blue = event->Par3;

            if(Par5 == 1)
            {
              Plugin_242_rainbow_stage = 0;
              Plugin_242_rainbow_max = event->Par1;
            }

            Plugin_242_running = true;

            String log = F("NeoPixel Advanced  : Task started - ");
            log += Plugin_242_target_red;
            log += " ";
            log += Plugin_242_target_green;
            log += " ";
            log += Plugin_242_target_blue;
            log += " ";
            log += Par4;
            log += " ";
            log += Par5;
            log += " ";
            addLog(LOG_LEVEL_INFO, log);

					  success = true;
          }

        }
        break;
      }

  }
  return success;
}

boolean SetFadePixels(uint8_t r, uint8_t g, uint8_t b, uint8_t leds)
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

  addLog(LOG_LEVEL_DEBUG, log);

  if((Plugin_242_red == r) && (Plugin_242_green == g) && (Plugin_242_blue == b))
  {
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
