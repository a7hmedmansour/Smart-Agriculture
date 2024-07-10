#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>
#include <ADC.h>
#include <BIT_MATH.h>
#include <DIO_INTERFACE.h>
#include <GLOBAL_INT1.h>
#include <MOTOR.h>
#include <STD_TYPES.h>
#include <Sensors_Interface.h>



SoftwareSerial espSerial(5,4); // RX, TX on Arduino Uno

char ssid[] = "ssid";       // your network SSID (name)
char pass[] = "pass";         // your network password

unsigned long myChannelNumber = 0000;
unsigned long myTalkBackID = 0000;
const char* myTalkBackKey = "---------";
  WiFiEspClient client;
void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);           // Initialize serial for debugging
  espSerial.begin(9600);
  delay(500);

  Sensors_Init();
  I2C_init(); 
   WiFi.init(&espSerial);

}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  
      Serial.print(".");
      delay(5000);
      digitalWrite(13, LOW);
    } 
    Serial.println("\nConnected.");
     digitalWrite(13, HIGH);  
  }
  int humidity, temperature;
  GET_Tempreature(&humidity, &temperature);
  Serial.println(humidity);
  Serial.println(temperature);
  int tank = Check_Water();
  Serial.println(tank);
  int rain = Check_Rain();
  Serial.println(rain);
  int fire = Check_Fire();
  Serial.println(fire);
  int soil = GET_Soil_Moisture();
  Serial.println(soil);

  // Create the postMessage string
  String postMessage = "field1=" + String(soil) +
                       "&field2=" + String(temperature) +
                       "&field3=" + String(humidity) +
                       "&field4=" + String(fire) +
                       "&field5=" + String(rain) +
                       "&field6=" + String(tank) +
                       "&api_key=" + "YI3TRAF1JXTE2WP4" +
                       "&talkback_key=" + String(myTalkBackKey);

  // Make a string for any commands in the queue
  String newCommand = String();

  // Make the POST to ThingSpeak
  int x = httpPOST(postMessage, newCommand);
client.stop();
  // Check the result
  if (x == 200) {
    Serial.println("checking queue..."); 
    // Check for a command returned from TalkBack
    if (newCommand.length() != 0) {
      Serial.print("  Latest command from queue: ");
      Serial.println(newCommand);

      if (matchCommand(newCommand, "TURN_ON_PUMP")) {
         MOTOR_ON(0); 
      }

      if (matchCommand(newCommand, "TURN_OFF_PUMP")) {
         MOTOR_OFF();
      }
       if (matchCommand(newCommand, "OPEN_ROOF")) {
        
         MOTOR_DC_ON(0);
        delay(300);
        MOTOR_DC_OFF();
        roof=0;
      }

      if (matchCommand(newCommand ,"CLOSE_ROOF")) {
        MOTOR_DC_ON(1);
        delay(400);
        MOTOR_DC_OFF();
        roof=1;
      }
    } else { 
      Serial.println("  Nothing new.");  
    }
  } else {
    Serial.println("Problem checking queue. HTTP error code " + String(x));
  }
delay(2000);
}

int httpPOST(String postMessage, String &response) {
  
  bool connectSuccess = client.connect("api.thingspeak.com", 80);

  if (!connectSuccess) {
    return -301;   
  }

  postMessage += "&headers=false";

  String headers = String("POST /update HTTP/1.1\r\n") +
                   "Host: api.thingspeak.com\r\n" +
                   "Content-Type: application/x-www-form-urlencoded\r\n" +
                   "Connection: close\r\n" +
                   "Content-Length: " + String(postMessage.length()) +
                   "\r\n\r\n";

  client.print(headers);
  client.print(postMessage);

  long startWaitForResponseAt = millis();
  while (client.available() == 0 && millis() - startWaitForResponseAt < 5000) {
    delay(100);
  }

  if (client.available() == 0) {       
    return -304; // Didn't get server response in time
  }

  if (!client.find("HTTP/1.1")) {
    return -303; // Couldn't parse response (didn't find HTTP/1.1)
  }

  int status = client.parseInt();
  if (status != 200) {
    return status;
  }

  if (!client.find("\n\r\n")) {
    return -303;
  }

  String tempString = client.readString();
  response = tempString;

  return status;
}

bool matchCommand(const String& input, const String& command) {
    int pos = input.indexOf(command);
    if (pos != -1) {
        // Extract the substring and compare
        String extractedCommand = input.substring(pos, pos + command.length());
        return extractedCommand == command;
    }
    return false;
}
