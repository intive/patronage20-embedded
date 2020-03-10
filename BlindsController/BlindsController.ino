/* BlindsController by Kuba Bartoszewicz */


/* defining pins for Stepper controller */
#define IN1 16 
#define IN2 5 
#define IN3 4 
#define IN4 0 

#define stepperR() stepperRun((dir), (IN1), (IN2), (IN3), (IN4));

/* defining pins for switch */
#define SW1 14
#define SW2 12

/* initialize basic variables */
int _step = 0; 
boolean dir = false; /* false = blinds go DOWN, true = blinds go UP */
int range = 0;
int currentVal = 0;
int settedValue = 0;
int i;

void setup() 
{ 
    Serial.begin(9600);
    
    pinMode(IN1, OUTPUT);  
    pinMode(IN2, OUTPUT);  
    pinMode(IN3, OUTPUT);  
    pinMode(IN4, OUTPUT); 
    pinMode(SW1, INPUT_PULLUP); 
    pinMode(SW2, INPUT_PULLUP); 

    /* Setting the range of blind */
    range = blindsCalibration(SW2);
    
} 

void loop() 
{ 
    /* manually move (by switch) blinds up until fully open or stopped by user */

    if (digitalRead(SW1) == LOW) {
        delay(300);
        blindsUp(0);
        Serial.println(currValPrc(currentVal));
    }
    
    /* manually move (by switch) blinds down until fully closed or stopped by user */
    if (digitalRead(SW2) == LOW) {
        delay(300);
        blindsDown(range);
        Serial.println(currValPrc(currentVal));
    }

    /* move blinds to specific value - outside signal (gatevay, serial port) */
    if (Serial.available() > 1) {
        setBlindsToValue(Serial.parseInt());
        Serial.println(currValPrc(currentVal));
    }
}




void setBlindsToValue(float settedValue) 
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
            blindsDown(settedValue);
        } else if (settedValue < currentVal) {
            blindsUp(settedValue);
        }
    }
}

int currValPrc (float a) 
{
    float b = range;
    float c = (a / b) * 100;
    return c;
}

void blindsUp (int val) 
{
    int i;
    dir = true;
    
    for (i = currentVal; i >= val; i--) {
        stepperR();
        currentVal = i;
        if (digitalRead(SW1) == LOW || digitalRead(SW2) == LOW) {
            delay(300);
            break;
        }
    }
}

void blindsDown (int val) 
{
    int i;
    dir = false;
    
    for (i = currentVal; i <= val; i++) {
        stepperR();
        currentVal = i;
        if (digitalRead(SW1) == LOW || digitalRead(SW2) == LOW) {
            delay(300);
            break;
        }
    }
}

int blindsCalibration (int controlSwitch) 
{
    int count = 0;
    Serial.print("Press switch to start calibration:");
    
    while (digitalRead(controlSwitch) == HIGH) {
        delay(10);
    }
    delay(300);
    
    Serial.println(" Starting calibration...");
    Serial.println(" Blinds moving down - PRESS switch to stop.");
    
    while (dir == false) {
        stepperR();
        while (digitalRead(controlSwitch) == LOW) {
            delay(300);
            dir = !dir;
        }
    }
    
    Serial.println(" Blinds moving up - PRESS switch to stop.");
    
    while (dir == true) {
        stepperR();
        count++;
        while (digitalRead(controlSwitch) == LOW) {
            delay(300);
            dir = !dir;
        }
    }
    
    Serial.println(count);
    return count;
}

void stepperRun (boolean dir, int Pin1, int Pin2, int Pin3, int Pin4) 
{    
    switch(_step){ 
        
        case 0: 
            digitalWrite(Pin1, LOW);  
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
            digitalWrite(Pin4, HIGH); 
        break;  
    
        case 1: 
            digitalWrite(Pin1, LOW);  
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, HIGH); 
            digitalWrite(Pin4, HIGH); 
        break;  
        
        case 2: 
            digitalWrite(Pin1, LOW);  
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, HIGH); 
            digitalWrite(Pin4, LOW); 
        break;  
        
        case 3: 
            digitalWrite(Pin1, LOW);  
            digitalWrite(Pin2, HIGH); 
            digitalWrite(Pin3, HIGH); 
            digitalWrite(Pin4, LOW); 
        break;  
        
        case 4: 
            digitalWrite(Pin1, LOW);  
            digitalWrite(Pin2, HIGH); 
            digitalWrite(Pin3, LOW); 
            digitalWrite(Pin4, LOW); 
        break;  
   
        case 5: 
            digitalWrite(Pin1, HIGH);  
            digitalWrite(Pin2, HIGH); 
            digitalWrite(Pin3, LOW); 
            digitalWrite(Pin4, LOW); 
        break;  
        
        case 6: 
            digitalWrite(Pin1, HIGH);  
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
            digitalWrite(Pin4, LOW); 
        break;  
        
        case 7: 
            digitalWrite(Pin1, HIGH);  
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
            digitalWrite(Pin4, HIGH); 
        break;  
   
        default: 
            digitalWrite(Pin1, LOW);  
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
            digitalWrite(Pin4, LOW); 
        break;  
    } 
    
    if (dir) { 
        _step++; 
    } else { 
        _step--; 
    } 
    
    if (_step > 7)  
        _step = 0; 
    
    if (_step < 0)  
        _step = 7; 

    delay(1); 
}
 
  
