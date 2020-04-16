/* Library that provide functions responsible for BlindsController */

#ifndef BLINDS_CONTROLLER_H
#define BLINDS_CONTROLLER_H

#include <ArduinoJson.h>

/* Initialize basic variables */
static int stepper_pin[4];     /* keeping pins number where stepper are connected, from IN1 - IN4 */
static int switches_pin[2];    /* keeping pins number where switches are connected */

static int range = 0;          /* innitial Val for max range due to blinds dimension */
static int currentVal = 0;     /* innitial val for position of blinds (stepper) */

/* Fnc responsible for moving the stepper */
static void stepper_run(bool dir, int *stepper_pin, int d) 
{    
    static int _step = 0;
    const int gray = _step ^ (_step >> 1);
    
    digitalWrite(stepper_pin[0], gray >> 1);
    digitalWrite(stepper_pin[1], gray & 0x1);
    digitalWrite(stepper_pin[2], (gray ^ 3) >> 1);
    digitalWrite(stepper_pin[3], (gray ^ 3) & 0x1);
  
    if (dir)  
        _step--;
    else  
        _step++;

    _step &= 0x3;

    if (d < 2) 
        delay(2); 
    else 
        delay(d);
}

/* Convert currentVal to prc (0-100) due to range */
static int convert_to_percent(int a) 
{
    return ((a * 100) + (range / 2)) / range;
}

/* Fnc responsible for moving blinds up to setted val until switch 1 or 2 pushed or achieve min range - 0 */
static void blinds_move_up(int val, int *sw) 
{
    int i;
    bool dir = true;
    
    for (i = currentVal; i >= val; i--) {
        stepper_run(dir, stepper_pin, 1);
        currentVal = i;
        if (digitalRead(sw[0]) == LOW || digitalRead(sw[1]) == LOW) {
            delay(300);
            break;
        }
    }
}

/* Fnc responsible for moving blinds down until switch 1 or 2 pushed or achieve max range - 100 */
static void blinds_move_down(int val, int *sw) 
{
    int i;
    bool dir = false;
    
    for (i = currentVal; i <= val; i++) {
        stepper_run(dir, stepper_pin, 1);
        currentVal = i;
        if (digitalRead(sw[0]) == LOW || digitalRead(sw[1]) == LOW) {
            delay(300);
            break;
        }
    }
}

/* Fnc responsible for calibrating the stepper/blinds */
static int blinds_calibrate(int controlSwitch) 
{
    bool dir = false; 
    int count = 0;
    Serial.print("Press switch to start calibration:");
    
    while (digitalRead(controlSwitch) == HIGH) {
        delay(10);
    }
    delay(300);
    
    Serial.println(" Starting calibration...");
    Serial.println(" Blinds moving down - PRESS switch to stop.");
    
    while (dir == false) {
        stepper_run(dir, stepper_pin, 1);
        while (digitalRead(controlSwitch) == LOW) {
            delay(300);
            dir = !dir;
        }
    }
    
    Serial.println(" Blinds moving up - PRESS switch to stop.");
    
    while (dir == true) {
        stepper_run(dir, stepper_pin, 1);
        count++;
        while (digitalRead(controlSwitch) == LOW) {
            delay(300);
            dir = !dir;
        }
    }
    
    Serial.println(count);
    return count;
}

/* Move blinds into setted value, value in prc 0-100  */
static void blinds_move_targetValue(int targetValue) 
{
    if (targetValue > 100) {
        targetValue = 100;
    }
    if (targetValue < 0) {
        targetValue = 0;
    }
    targetValue = ((targetValue * range) + 50) / 100;
    if (targetValue != currentVal) {
        if (targetValue > currentVal) {
            blinds_move_down(targetValue, switches_pin);
        } 
        else if (targetValue < currentVal) {
            blinds_move_up(targetValue, switches_pin);
        }
    }
}

/* Preparing JSON filled with station ID and currentValue */
static String return_JSON(int id, int pos) 
{
    String output = "";
    StaticJsonDocument<100> doc;
    
    doc["id"] = id;
    doc["type"] = "BlindsController";
    doc["position"] = pos;
    
    serializeJsonPretty(doc, output);
    return output;
}
  
#endif /* BLINDS_CONTROLLER_H */
