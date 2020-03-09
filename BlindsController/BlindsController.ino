/* BlindsController by Kuba Bartoszewicz */

/* defining pins for Stepper controller */
#define Pin1 16 
#define Pin2 5 
#define Pin3 4 
#define Pin4 0 

/* defining pins for switch */
#define SW1 14

/* initialize basic variables */
int _step = 0; 
boolean dir = false; /* false = clockwise, true=counter clockwise */
int count = 0;

void setup() 
{ 
    pinMode(Pin1, OUTPUT);  
    pinMode(Pin2, OUTPUT);  
    pinMode(Pin3, OUTPUT);  
    pinMode(Pin4, OUTPUT); 
    pinMode(SW1, INPUT_PULLUP); 

 
} 

void loop() 
{ 
    StepperRun();
    
    while (digitalRead(SW1) == LOW) {
      dir = !dir;
      delay(300);
    }
    
    

}

void StepperRun () {
    
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
 
  
