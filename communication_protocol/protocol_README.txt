Sample jsons containing information not shown in /dashboard

Keyboardcode.json - sample json containing information form Keyboard
    (int) id - keyboard's id
    (string) type - type of message (default set to "Keyboard")
    (string) value - code taken from keyboard (restrictions TBD)

lightSetup.json - sample json containing color value which is send from gateway to light controller
    (int) id - id of light
    (string) type - type of message (default "lightColor")
    (string) color - color value in HEX

RFIDtoGateway - sample json informing gateway about RFID sensos status
    (int) id - id of RFID sensor
    (string) type - type of message (default "RFID_status")
    (bool) triggered - RFID status

ServoToHVAC - sample json informing HVAC about servo position
    (int) servoId - id of servo
    (int) HVACID - id of HVAC
    (string) type - type of message (default "ServoPosition")
    (int) position - servo position (0-100)