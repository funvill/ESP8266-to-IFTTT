/*
Read a button state, Then print the state to the serial port and togle a LED
This tests the button and the ESP code.

 This example code is in the public domain.
 */

// digital pin 3 has a pushbutton attached to it. Give it a name:
int pushButton = D3;

// the setup routine runs once when you press reset:
void setup() {  
  Serial.begin(115200);// initialize serial communication at 115200 bits per second:  
  pinMode(pushButton, INPUT_PULLUP);// make the pushbutton's pin an input  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
}

// the loop routine runs over and over again forever:
void loop() {  
  int buttonState = digitalRead(pushButton); // read the input pin:
  digitalWrite(BUILTIN_LED, buttonState); // Update the LED 
  
  // print out the state of the button:
  Serial.println("Button d3: " + String(buttonState));
  delay(10); 
}



