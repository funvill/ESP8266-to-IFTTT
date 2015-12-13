/*
Read a button state, Send a message to IFTTT when the button is pressed. 

This example code is in the public domain.
https://github.com/funvill/ESP8266-to-IFTTT 
*/

#include <ESP8266WiFi.h>

const char* ssid     = "...YOUR_WIFI_SSID...";
const char* password = "...YOUR_WIFI_PASSWORD...";

const char* host = "maker.ifttt.com";
const char* privateKey = "...IFTTT_MAKER_CHANNEL_KEY..."; // Found here https://ifttt.com/maker
const char* eventName = "...EVENT_NAME...";

const unsigned long buttonTimeout = 10 ; // The time in seconeds between presses. 
int pushButton = D3; // Digital pin 3 has a pushbutton attached to it. Give it a name:

// setup code here, to run once:
void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Set up the LED pin and the Button PIN 
  pinMode(pushButton, INPUT_PULLUP);// make the pushbutton's pin an input  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

  // We start by connecting to a WiFi network  
  Serial.println(String(millis()) + " Connecting to " + String(ssid) );  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  
  Serial.println(); 
  Serial.println(String(millis()) + " WiFi connected");  
  Serial.println(String(millis()) + " IP address: " + String(WiFi.localIP()) );
  Serial.println(String(millis()) + " Waiting for Button press... ");
} 
void loop() {

  /**
   *  Read the stat of the button. 
   *  IF the button has changed then
   *     Check to see if the button is down and 
   *     Check to see if we have pressed it the timeout period.
   *        Then Send the message to IFTTT
   */  
  
  static unsigned long buttonStateLastPressed = 0 ; 
  static int buttonStateDebounce = HIGH ; 
  int buttonState = digitalRead(pushButton); // read the input pin:
  
  if( buttonStateDebounce != buttonState && 
      millis() > buttonStateLastPressed + (buttonTimeout * 1000)) 
  {
    buttonStateDebounce = buttonState ; 
    buttonStateLastPressed = millis() ; 
    
    if( buttonState == LOW ) {
      digitalWrite(BUILTIN_LED, HIGH); // Update the LED 
      clickFunction() ; 
      digitalWrite(BUILTIN_LED, LOW); // Update the LED       
      Serial.println(String(millis()) + " Waiting for Button press... ");
    }
  }
}




// This function is called when someone pressed the button down. 
void clickFunction() {
  Serial.println(String(millis()) + " Button pressed...");
  
  // Connect to the server. 
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println(String(millis()) + " connection failed");
    return;
  }
  Serial.println(String(millis()) + " Connected to "+ String( host ) + "...");

  // https://maker.ifttt.com/trigger/{EVENT}/with/key/{KEY}
  String url = "/trigger/" + String( eventName ) + "/with/key/" + privateKey ; 
  
  Serial.print(String(millis()) + " Requesting URL: "+ String( url ) );

  Serial.println(String(millis()) + " Making request... ");
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");    
  Serial.println(String(millis()) + " Waiting for response...");
  
  unsigned long timeout = millis() + 1000 * 3 ; 
  while(!client.available() ){
    if( millis() > timeout ) {
      break ; 
    }
  }

  while(client.available() ){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println(String(millis()) + " Done.\n\n");
}
