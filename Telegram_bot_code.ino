#include "CTBot.h"
#include "DHT.h"

DHT dht;

CTBot myBot;

String ssid = "SSID";     // REPLACE mySSID WITH YOUR WIFI SSID
String pass = "Password"; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "";   // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN
      

void setup() {
  // initialize the Serial
  Serial.begin(115200);
  Serial.println("Starting TelegramBot...");

  // connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, pass);

  // set the telegram bot token
  myBot.setTelegramToken(token);

  // check if all things are ok
  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");

  // set the Node Mcu pins as output pin
  pinMode(D0, OUTPUT); // Blue Light
  pinMode(D1, OUTPUT); // Red Light
  pinMode(D2, OUTPUT); // Green Light 
  pinMode(D3, OUTPUT); // DC FAN
  dht.setup(D4); // DHT11 Sensor

}

void loop() {
  // a variable to store telegram message data
  TBMessage msg;

  // if there is an incoming message...
  if (CTBotMessageText == myBot.getNewMessage(msg)) {

    if (msg.text.equalsIgnoreCase("Automatic Mode")) {              // if the received message is "Automatic Mode"...
                                                                   
      myBot.sendMessage(msg.sender.id, "Automatic Mode is Selected");
      while(CTBotMessageText != myBot.getNewMessage(msg))
      {
        myBot.sendMessage(msg.sender.id, (String)"Temperature now is: " 
        + dht.getTemperature() + (String)"\nHumidity now is: " + dht.getHumidity());
        if(dht.getTemperature() <= 25 ){
          digitalWrite(D0,HIGH);
          digitalWrite(D1,LOW);
          digitalWrite(D2,LOW);
          digitalWrite(D3,LOW);
          myBot.sendMessage(msg.sender.id, (String)"Blue light is now ON" + (String)"\nRed light is now OFF" + (String)"\nGreen light is now OFF");
          }
        if(dht.getTemperature() <= 30 && dht.getTemperature() > 25){
          digitalWrite(D0,LOW);
          digitalWrite(D1,HIGH);
          analogWrite(D3,160);
          digitalWrite(D2,LOW);
          myBot.sendMessage(msg.sender.id, (String)"Blue light is now OFF" + (String)"\nRed light is now OFF" + (String)"\nGreen light is now ON");  
          }
        if(dht.getTemperature() > 30 ){
          digitalWrite(D0,LOW);
          digitalWrite(D1,LOW);
          digitalWrite(D2,HIGH);
          digitalWrite(D3,HIGH);  
          myBot.sendMessage(msg.sender.id, (String)"Blue light is now OFF" + (String)"\nRed light is now ON" + (String)"\nGreen light is now OFF");
          }
        if(dht.getHumidity() > 50 ){
          digitalWrite(D3,HIGH);  
          
          }
        delay(120000);
        }
    }
    else if (msg.text.equalsIgnoreCase("Manual Mode")) {        // if the received message is "Manual Mode"...
                                
      myBot.sendMessage(msg.sender.id, (String)"Manual Mode is Selected" + (String)"\n1. Turn on blue light." + (String)"\n2. Turn off blue light." +
      (String)"\n3. Turn on red light." + (String)"\n4. Turn off red light." + (String)"\n5. Turn on green light." + (String)"\n6. Turn off green light."
      + (String)"\n7. Turn on DC fan."  + (String)"\n8. Turn off DC fan."  + (String)"\n9. Turn on DC fan 50% speed."  + (String)"\n10. Display Temperature and Humidity."); // notify the sen1der
      
    }
    else if (msg.text.equalsIgnoreCase("Turn on blue light")){       
      digitalWrite(D0,HIGH);                          // turn on blue light
      myBot.sendMessage(msg.sender.id, "Blue light is now ON"); // notfiy the user
    }
     else if (msg.text.equalsIgnoreCase("Turn off blue light")) {        
      digitalWrite(D0,LOW);                          
      myBot.sendMessage(msg.sender.id, "Blue light is now OFF");
    }
    else if (msg.text.equalsIgnoreCase("Turn on red light")) {        
      digitalWrite(D1,HIGH);                          
      myBot.sendMessage(msg.sender.id, "Red light is now ON");
    }
     else if (msg.text.equalsIgnoreCase("Turn off red light")) {        
      digitalWrite(D1,LOW);                          
      myBot.sendMessage(msg.sender.id, "Red light is now OFF");
    }
      else if (msg.text.equalsIgnoreCase("Turn on green light")) {        
      digitalWrite(D2,HIGH);                          
      myBot.sendMessage(msg.sender.id, "Green light is now ON");
    }
     else if (msg.text.equalsIgnoreCase("Turn off Green light")) {        
      digitalWrite(D2,LOW);                          
      myBot.sendMessage(msg.sender.id, "Green light is now OFF");
    }
        else if (msg.text.equalsIgnoreCase("Turn on DC fan")) {        
      digitalWrite(D3,HIGH);                          
      myBot.sendMessage(msg.sender.id, "DC fan is now ON");
    }
     else if (msg.text.equalsIgnoreCase("Turn off DC fan")) {        
      digitalWrite(D3,LOW);                          
      myBot.sendMessage(msg.sender.id, "DC fan is now OFF");
    }
     else if (msg.text.equalsIgnoreCase("Turn on DC fan 50% speed")) {        
      analogWrite(D3,150);                          
      myBot.sendMessage(msg.sender.id, "DC fan is now on 50% speed");
    }
     else if (msg.text.equalsIgnoreCase("Display temperature and humidity")) {                            
      myBot.sendMessage(msg.sender.id, (String)"Temperature is:" + dht.getTemperature() + (String)"\nHumidity is:" + dht.getHumidity());
    }
    else if (msg.text.equalsIgnoreCase("Status")) {        
      if(digitalRead(D0) == HIGH) {
        myBot.sendMessage(msg.sender.id, "Blue Status: ON");
        }
      if(digitalRead(D0) == LOW) {
        myBot.sendMessage(msg.sender.id, "Blue Status: OFF");
        }
      if(digitalRead(D1) == HIGH) {
        myBot.sendMessage(msg.sender.id, "Red Status: ON");
        }
      if(digitalRead(D1) == LOW) {
        myBot.sendMessage(msg.sender.id, "Red Status: OFF");
        }
      if(digitalRead(D2) == HIGH) {
        myBot.sendMessage(msg.sender.id, "Green Status: ON");
        }
      if(digitalRead(D2) == LOW) {
        myBot.sendMessage(msg.sender.id, "Green Status: OFF");
        }
      if(digitalRead(D3) == HIGH) {
        myBot.sendMessage(msg.sender.id, "DC fan Status: ON");
        }
      if(digitalRead(D3) == LOW) {
        myBot.sendMessage(msg.sender.id, "DC fan Status: OFF");
        }
    }
    else {                                                
      // generate the message for the sender
      String reply;
      reply = (String)"Welcome on Temperature and Humidity Control System"  + (String)"\n Please Select:" + (String)"\n 1. Automatic Mode" + (String)"\n 2. Manual Mode";
      myBot.sendMessage(msg.sender.id, reply);
      if(digitalRead(D0) == HIGH) {
        myBot.sendMessage(msg.sender.id, "Blue Status: ON");
        }
      if(digitalRead(D0) == LOW) {
        myBot.sendMessage(msg.sender.id, "Blue Status: OFF");
        }
      if(digitalRead(D1) == HIGH) {
        myBot.sendMessage(msg.sender.id, "Red Status: ON");
        }
      if(digitalRead(D1) == LOW) {
        myBot.sendMessage(msg.sender.id, "Red Status: OFF");
        }
      if(digitalRead(D2) == HIGH) {
        myBot.sendMessage(msg.sender.id, "Green Status: ON");
        }
      if(digitalRead(D2) == LOW) {
        myBot.sendMessage(msg.sender.id, "Green Status: OFF");
        }
      if(digitalRead(D3) == HIGH) {
        myBot.sendMessage(msg.sender.id, "DC fan Status: ON");
        }
      if(digitalRead(D3) == LOW) {
        myBot.sendMessage(msg.sender.id, "DC fan Status: OFF");
        }            
    }
  }
  // wait 500 milliseconds
  delay(500);
}
