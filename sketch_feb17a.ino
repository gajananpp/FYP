/*     
 *    Name:           final-year-project-arduino
 *    Date Created:   16/02/17
 *    Date Modified:  17/02/17
 */

/*    
 *    SENSOR    QUANTITY MEASURED   ASSOCIATED LOAD
 *    DHT11     humidity            -
 *    DHT11     temperature         -
 *    LDR       brightness          LED Bulb
 *    FSR       force               Conveyer Motor
 *    MQ2       gasLevel            RGB LED               
 */

 /*
  *                                     LOAD BEHAVIOUR
  *   
  *   LOAD              QUANTIY INCREASE                      QUANTIY DECREASE          APPLICATION CONTROL       
  *   LED Bulb          (brightness>60%) ON                   (brightness<60%) OFF      (brightness>60%)  Intensity of LED Bulb can be controlled manually using slider
  *   Conveyer Belt     (force>75%) OFF                       (force<75%) ON            (force<75%) Motor speed can be controlled manually using slider
  *   RGB LED           [
  *                       (gasLevel>8%)  ON G(safe)            -                        (gasLevel>24%) Alert tone will be played
  *                       (gasLevel>12%) ON B(not so safe)
  *                       (gasLevel>24%) ON R(danger)
  *                     ]
  */


  /*
   *  Reference Links: [
   *    DHT11               - http://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/
   *    LDR                 - https://diyhacking.com/arduino-ldr-sensor/
   *    FSR                 - https://learn.adafruit.com/force-sensitive-resistor-fsr/using-an-fsr
   *    MQ2                 - http://www.instructables.com/id/Ardunio-Smoke-And-Gas-SensorMQ-2/?ALLSTEPS
   *    
   *    ArduinoJson library - https://github.com/bblanchon/ArduinoJson/wiki/Examples , https://bblanchon.github.io/ArduinoJson/api/
   *    ESP8266 library     - https://github.com/esp8266/Arduino
   *  ]
   */


   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PROGRAM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

//   #include <ESP8266WiFi.h>
//   #include <ESP8266WiFiMulti.h>
//   #include <ESP8266HTTPClient.h>

   #include "dht.h"                                                                 // library for DHT11
   #include "ArduinoJson.h"                                                         // library to suppport data-interchange format between arduino and server

   #define N_SENSORS 3                                                              //  no. of sensors used [NOTE:- DHT11 adds to two as it calculates 2 quantities]
    
   #define DHT11_PIN 7                                                              //  data from DHT11 is available on pin 7 of arduino
   #define LDR_PIN   A0                                                             //  data from LDR is available on pin A0 of arduino

//   ESP8266WiFiMulti WiFiMulti;
//   const char* SSID     = "Tenda";
//   const char* PASSWORD = "pRpatil@7225";
//   HTTPClient http;

   int manualLoadControl = 0;                                                       // if 1 loads can be controlled manually through application else will be controlled according to sensor readings

   dht DHT;                                                                         // declared variable DHT of type dht defined in dht.h

   /* structure of data to sent to server */
   struct SensorData {
        char* sensorName;
        char* quantityMeasured;
        float value;
        char* loadType;
   };

   /* initialise qunatity variables */
   float humidity    = 0;
   float temperature = 0;
   float brightness  = 0;

   /* declare variable which will hold structered data */
   SensorData formattedData[N_SENSORS];
   
   String responseFromServer = "";                                                   //  initialise string received from server 

   /* Wifi related functions */
//   void connectToAp(char*, char*);                                                   // calling this function connects MCU to Wifi using given ssid and password
//   void sendPostRequest(JsonArray&);                                                 // send data to server in POST request payload

   /* calling below functions gives us current readings */
   float getHumidity(int);                                                           //  calling this function returns current humidity reading (in %)
   float getTemperature(int);                                                        //  calling this function returns current temperature reading (in deg. Celsius)
   float getBrightness(int);                                                         //  calling this function returns current brightness reading (in %)

   void formatData(float[], int);                                                    //  calling this function populates formattedData[] with structured data to be sent to server
   JsonArray& generateJson(SensorData[], int);                                       //  calling this function generate json from formatted data and returns it

   void sendDataToServer(JsonArray&);                                                // calling this function sends given data to server
   
   void setup() {
      Serial.begin(115200);                                                          // begin serial communication at a baud-rate of 115200 bps
      delay(50);
//      connectToAp(SSID, PASSWORD);
   }

   void loop() {
      /* get current readings from sensor */
      humidity    = getHumidity(DHT11_PIN);
      temperature = getTemperature(DHT11_PIN);
      brightness  = getBrightness(LDR_PIN);

      /* foramt data in json form to be sent to the server */
      float sensorArray[N_SENSORS] = {humidity, temperature, brightness};            //  declared array variable that will contain measured quantities from sensor
      formatData(sensorArray, N_SENSORS);
      JsonArray& dataToServer      =  generateJson(formattedData, N_SENSORS);

      /* send data to server */
      sendDataToServer(dataToServer);
//      sendPostRequest(dataToServer);
      
      delay(2000);                                                                   // readings taken every 2 seconds  
   }


   /**
    * connectToAp
    * connects to WiFi Access Point
    * 
    * @param   ssid      char*   WiFi name
    * @param   password  char*   WiFi password
    */
//    void connectToAp(const char* ssid, const char* password) {
//        WiFiMulti.addAP(ssid, password);
//        Serial.println();
//        Serial.println();
//        Serial.print("Wait for Wifi... ");
//
//        while (WiFiMulti.run()!= WL_CONNECTED) {
//            Serial.print(".");
//            delay(500);
//        }
//        Serial.println("");
//        Serial.println("WiFi Connected!!!");
//        Serial.println("IP Address: ");
//        Serial.println(WiFi.localIP());
//        delay(500);
//    }

    /**
      * sendPostRequest
      * sends POST request to server
      * 
      * @param    payload     JsonArray&      JSON Array containing readings from sensor
      */
//     void sendPostRequest(JsonArray& payload) {
//        char buffer[500];
//        payload.prettyPrintTo(buffer, sizeof(buffer));
//        Serial.println("[HTTP] begin...");
//        http.begin("http://192.168.0.103/");
//        http.addHeader("Content-Type", "application/json");
//        http.POST(buffer);
//        http.writeToStream(&Serial);
//        http.end();
//     }

   
   /**
    *  getHumidity
    *  reads current humidity reading from DHT11 sensor
    *  
    *  @param   sensorPin     int     Pin of arduino to which DHT11 sensors's data pin is connected  
    *  @return                float   current humidity reading
    */
   float getHumidity(int sensorPin) {
      int chk               = DHT.read11(sensorPin);
      float currentHumidity = DHT.humidity;
      return currentHumidity; 
   }

   /**
    *  getTemperature
    *  reads current temperature reading from DHT11 sensor
    *  
    *  @param   sensorPin     int     Pin of arduino to which DHT11 sensors's data pin is connected  
    *  @return                float   current temperature reading
    */
   float getTemperature(int sensorPin) {
      delay(100);
      int chk                  = DHT.read11(sensorPin);
      float currentTemperature = DHT.temperature;
      return currentTemperature; 
   }

   /**
    *  getBrightness
    *  reads current brightness reading from LDR sensor
    *  
    *  @param   sensorPin     int     Pin of arduino to which LDR sensors is connected  
    *  @return                float   current brightness reading
    */
   float getBrightness(int sensorPin) {
      int rawReading           = analogRead(sensorPin);
      float currentBrightness  = (rawReading/1024.00)*100;                // convert rawReading from sensor to percentage brightness [NOTE:- resolution of analog pins is 10-bits i.e. 2^10 = 1024]
      return currentBrightness; 
   }

   /** 
    *  formatData
    *  formats input data to convert it into suitable form to be sent to server
    *  
    *  @param   arr         float[]         float array containing current readings
    *  @param   n           int             length of arr[]   
    */
   void formatData(float arr[], int n) {
      SensorData obj;
      char* quantities[N_SENSORS] = {"humidity", "temperature", "brightness"};
      char* loadTypes[N_SENSORS]  = {"-", "-", "LED Bulb"};
      char* sensors[N_SENSORS]    = {"DHT11", "DHT11", "LDR"};
      for (int i = 0; i < n; i++) {
          obj.sensorName          = sensors[i];
          obj.quantityMeasured    = quantities[i];
          obj.loadType            = loadTypes[i];
          obj.value               = arr[i];
          formattedData[i]        = obj;  
      }
   }

   /**
    *   generateJson
    *   generates json object from the structured data, ready to be sent to server
    *   
    *   @param    arr   SensorData[]    array of structured data 
    *   @param    n     int             length of arr[]
    *   @return         &JsonArray      generated json array
    */
   JsonArray& generateJson(SensorData arr[], int n) {
      const size_t bufferSize           = JSON_ARRAY_SIZE(N_SENSORS) + JSON_OBJECT_SIZE(4) + 100;
      StaticJsonBuffer<bufferSize> jsonBuffer;
      JsonArray& root                   = jsonBuffer.createArray();
      for (int i; i < n; i++) {
          JsonObject& sensorObj         = root.createNestedObject();
          sensorObj["sensorName"]       = arr[i].sensorName;
          sensorObj["quantityMeasured"] = arr[i].quantityMeasured;
          sensorObj["loadType"]         = arr[i].loadType;
          sensorObj["value"]            = arr[i].value;
      }
      return root;
   }

   /**
    *   sendDataToServer
    *   sends given data to server
    *   
    *   @param    data    JsonArray&    data to be sent to server
    */
   void sendDataToServer(JsonArray& data) {
      char buffer[500];
      data.printTo(buffer, sizeof(buffer));
      Serial.println(buffer);
   }
   
   
