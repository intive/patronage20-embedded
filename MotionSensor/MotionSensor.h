#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H
#include <ArduinoJson.h>

/* returns true when motion is detected */
/* and false when there is no motion */
static bool detect_motion(int pin)
{
    bool state = digitalRead(pin);
  
    if (state == HIGH)
        return true;

    else
        return false;
}

/* Returns json string with sensor status */
static String motion_sensor(int pin)
{
    String output = "";
    StaticJsonDocument<100> root;

    root["id"] = 1;
    root["type"] = "MotionSensor";
    root["value"] = detect_motion(pin);

    serializeJson(root, output);
    return output;
}

#endif /* MOTION_SENSOR_H */
