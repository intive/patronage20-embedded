#include <ArduinoJson.h>

#define PIN 13

/* returns true when motion is detected */
/* and false when there is no motion */
bool detect_motion()
{
    long state = digitalRead(PIN);
  
    if (state == HIGH)
        return true;

    else
        return false;
}

/* Returns json string with sensor status */
String motion_sensor()
{
    String output = "";
    StaticJsonDocument<100> root;

    root["id"] = 1;
    root["type"] = "MotionSensor";
    root["value"] = detect_motion();

    serializeJson(root, output);
    return output;
}
