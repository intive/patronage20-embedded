/* Library that provide functions responsible for BlindsController */

/* Initialize basic variables */
int stepper_pin[4];     /* keeping pins number where stepper are connected, from IN1 - IN4 */
int switches_pin[2];    /* keeping pins number where switches are connected */

int _step = 0;          /* innitial step val to control the stepper */
int range = 0;          /* innitial Val for max range due to blinds dimension */
int currentVal = 0;     /* innitial val for position of blinds (stepper) */

/* Fnc responsible for moving the stepper */
void stepper_run (bool dir, int *stepper_pin) 
{    
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

    delay(1); 
}

/* Convert currentVal to prc (0-100) due to range */
int convert_currVal_to_prcVal (float a) 
{
    float b = range;
    float c = (a / b) * 100;
    return c;
}

/* Fnc responsible for moving blinds up to setted val until switch 1 or 2 pushed or achieve min range - 0 */
void blinds_move_up (int val, int *sw) 
{
    int i;
    bool dir = true;
    
    for (i = currentVal; i >= val; i--) {
        stepper_run(dir, stepper_pin);
        currentVal = i;
        if (digitalRead(sw[0]) == LOW || digitalRead(sw[1]) == LOW) {
            delay(300);
            break;
        }
    }
}

/* Fnc responsible for moving blinds down until switch 1 or 2 pushed or achieve max range - 100 */
void blinds_move_down (int val, int *sw) 
{
    int i;
    bool dir = false;
    
    for (i = currentVal; i <= val; i++) {
        stepper_run(dir, stepper_pin);
        currentVal = i;
        if (digitalRead(sw[0]) == LOW || digitalRead(sw[1]) == LOW) {
            delay(300);
            break;
        }
    }
}

/* Fnc responsible for calibrating the stepper/blinds */
int blinds_calibrate (int controlSwitch) 
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
        stepper_run(dir, stepper_pin);
        while (digitalRead(controlSwitch) == LOW) {
            delay(300);
            dir = !dir;
        }
    }
    
    Serial.println(" Blinds moving up - PRESS switch to stop.");
    
    while (dir == true) {
        stepper_run(dir, stepper_pin);
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
void blinds_move_settedPrcValue(float settedValue) 
{
    if (settedValue > 100) {
        settedValue = 100;
    }
    if (settedValue < 0) {
        settedValue = 0;
    }
    settedValue = (settedValue / 100) * range;
    settedValue = (int)settedValue;
    if (settedValue != currentVal) {
        if (settedValue > currentVal) {
            blinds_move_down(settedValue, switches_pin);
        } 
        else if (settedValue < currentVal) {
            blinds_move_up(settedValue, switches_pin);
        }
    }
}
  
