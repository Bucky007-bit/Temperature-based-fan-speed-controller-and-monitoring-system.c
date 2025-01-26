#define BLYNK_TEMPLATE_ID "your template id"
#define BLYNK_TEMPLATE_NAME "your template name"

#define BLYNK_AUTH_TOKEN "your auth token"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DallasTemperature.h>
#include <OneWire.h>


char ssid[] = "ssid name";
char pass[] = "password";


#define TEMPPIN 15   
OneWire oneWire(TEMPPIN);
DallasTemperature
sensors(&oneWire);

#define FAN_PIN 5      

float temperature;
int fanSpeed;
bool fanswitch=false;

#define VPIN_TEMPERATURE V1
#define VPIN_FANSPEED V2
#define VPIN_FAN_SWITCH V3

 BLYNK_WRITE(VPIN_FAN_SWITCH) {
    fanswitch = 
    param.asInt();
   }
 
void setup()
{
  
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
 
  pinMode(FAN_PIN, OUTPUT);
  ledcAttach(FAN_PIN,5000,10); 
  sensors.begin();
}  

 


void loop()
{
  
  Blynk.run();
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  if (temperature==DEVICE_DISCONNECTED_C) {
    Serial.println(F("Failed to read from  sensor!"));
     delay(2000);
    return;
  }
  if(fanswitch){
    if (temperature < 23) {
       fanSpeed = 0;  
      } else if (temperature>= 23 && temperature < 37) {
       fanSpeed = map(temperature,23, 37, 0,1023 ); 
      } else {
        fanSpeed = 1023; 
  }

  ledcWrite(5, fanSpeed);

  
  Serial.print("Temperature:");
  Serial.print(temperature);
  Serial.print("C");
  
  Serial.print("Fan speed:");
  Serial.print(map(fanSpeed, 0,1023, 0, 100));
    Serial.print("%");
  
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature);
  Blynk.virtualWrite(VPIN_FANSPEED, map(fanSpeed, 0, 1023, 0, 100));
  }else{
    fanSpeed=0;
    ledcWrite(5,fanSpeed);
    Serial.println("Fan is off");
    Blynk.virtualWrite(VPIN_FANSPEED,0);
  }
 // delay(2000);
}
