/* Library that provide functions responsible for BlindsController */

#ifndef BLINDS_CONTROLLER_H
#define BLINDS_CONTROLLER_H

/* Initialize basic variables */
static int stepper_pin[4];     /* keeping pins number where stepper are connected, from IN1 - IN4 */
static int switches_pin[2];    /* keeping pins number where switches are connected */

static int range = 0;          /* innitial Val for max range due to blinds dimension */
static int currentVal = 0;     /* innitial val for position of blinds (stepper) */

/* Fnc responsible for moving the stepper */
static void stepper_run(bool dir, int *stepper_pin, int d) 
{    
    static int _step = 0;

    switch(_step){ 
        
        case 0: 
            digitalWrite(stepper_pin[0], LOW);  
            digitalWrite(stepper_pin[1], LOW); 
            digitalWrite(stepper_pin[2], LOW); 
            digitalWrite(stepper_pin[3], HIGH); 
        break;  
    
        case 1: 
            digitalWrite(stepper_pin[0], LOW);  
            digitalWrite(stepper_pin[1], LOW); 
            digitalWrite(stepper_pin[2], HIGH); 
            digitalWrite(stepper_pin[3], HIGH); 
        break;  
        
        case 2: 
            digitalWrite(stepper_pin[0], LOW);  
            digitalWrite(stepper_pin[1], LOW); 
            digitalWrite(stepper_pin[2], HIGH); 
            digitalWrite(stepper_pin[3], LOW); 
        break;  
        
        case 3: 
            digitalWrite(stepper_pin[0], LOW);  
            digitalWrite(stepper_pin[1], HIGH); 
            digitalWrite(stepper_pin[2], HIGH); 
            digitalWrite(stepper_pin[3], LOW); 
        break;  
        
        case 4: 
            digitalWrite(stepper_pin[0], LOW);  
            digitalWrite(stepper_pin[1], HIGH); 
            digitalWrite(stepper_pin[2], LOW); 
            digitalWrite(stepper_pin[3], LOW); 
        break;  
   
        case 5: 
            digitalWrite(stepper_pin[0], HIGH);  
            digitalWrite(stepper_pin[1], HIGH); 
            digitalWrite(stepper_pin[2], LOW); 
            digitalWrite(stepper_pin[3], LOW); 
        break;  
        
        case 6: 
            digitalWrite(stepper_pin[0], HIGH);  
            digitalWrite(stepper_pin[1], LOW); 
            digitalWrite(stepper_pin[2], LOW); 
            digitalWrite(stepper_pin[3], LOW); 
        break;  
        
        case 7: 
            digitalWrite(stepper_pin[0], HIGH);  
            digitalWrite(stepper_pin[1], LOW); 
            digitalWrite(stepper_pin[2], LOW); 
            digitalWrite(stepper_pin[3], HIGH); 
        break;  
   
        default: 
            digitalWrite(stepper_pin[0], LOW);  
            digitalWrite(stepper_pin[1], LOW); 
            digitalWrite(stepper_pin[2], LOW); 
            digitalWrite(stepper_pin[3], LOW); 
        break;  
    } 
    
    if (dir)  
        _step--;
    else  
        _step++;
     
    
    if (_step > 7)  
        _step = 0; 
    
    if (_step < 0)  
        _step = 7; 

    if (d < 1) 
        delay(1); 
    else 
        delay(d);
}

/* Convert currentVal to prc (0-100) due to range */
static int convert_to_percent(int a) 
{
    return (a * 100) / range;
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
    targetValue = (targetValue * range) / 100;
    if (targetValue != currentVal) {
        if (targetValue > currentVal) {
            blinds_move_down(targetValue, switches_pin);
        } 
        else if (targetValue < currentVal) {
            blinds_move_up(targetValue, switches_pin);
        }
    }
}
  
#endif /* BLINDS_CONTROLLER_H */
