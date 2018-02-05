
//========= IMPORT LIBRARY =========
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <DS3232RTC.h>        
#include <Streaming.h>       
#include <Time.h>          
#include <Wire.h>  
//================================== 

//========= WIFI SID/PASSWORD ===============================
const char* ssid = "Ruggeri-Senesi WIFI 2.4";//type your ssid
const char* pass = "12MaMaLoreChia12";//type your password
int status = WL_IDLE_STATUS;
int uploadInterval = 0;
//===========================================================

//========= CONSTANT ===========================================
#define OLED_RESET LED_BUILTIN    // 4 - for LCD
#define DHTPIN D5                 // what pin we're connected to - for temp sensor
#define DHTTYPE DHT22             // DHT 22  (AM2302) - for temp sensor
const int BUTTONPINA = 16;        // PIN for button A
const int BUTTONPINB = 0;         // PIN for button B
//===============================================================

//========= DEFINE OBJECTS ============================================================
DHT dht(DHTPIN, DHTTYPE);             // Initialize DHT sensor for normal 16mhz Arduino
Adafruit_SSD1306 display(OLED_RESET); // Initialize the LCD
WiFiServer server(80);                // Initialize the server
WiFiClient client;                    // Initialize the client
//=====================================================================================

//========== DEFINR VARIABLE ==========================================================
float humEx;    //Stores external humidity value
float tempEx;   //Stores external temperature value
float humIn;    //Stores internal humidity value
float tempIn;   //Stores internal temperature value

String data;  //To send to server for storing
unsigned long previousMillis = 0;  // last time update
long interval = 0;                 // interval at which to do something (milliseconds)
int buttonStateA = 0;              // variable for reading the pushbutton A status 
int buttonStateB = 0;              // variable for reading the pushbutton B status
//===================================================================================== 


// ===========================================================================================
// FUNCTION
// ===========================================================================================

//WELCOME DISPLAY MESSAGE
void welcomeMessage(){
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0,0);
  display.println("* * * * * * * * * * *");
  display.println("                     ");
  display.println("     METEO STATION   ");
  display.println("       STARTING      ");
  display.println("        DEVICE       ");
  display.println("                     ");
  display.println("/ / / / / / / / / / /");
  display.println("* * * * * * * * * * *");
  display.display();
  delay(5000);
  display.setTextColor(WHITE);
}

//TO UPLOAD DATA ONLINE
void uploadData(){
  data = "temp1=" + String(tempEx) + "&hum1=" + String(humEx);
  Serial.println("Sending data to server: " + String(data) );
  Serial.println ( "Server connection: " + String (client.connect("funnytech.atwebpages.com",80)) );
  if (client.connect("funnytech.atwebpages.com",80)) {
    Serial.println("Data sending...");
    client.println("POST /tms/add.php HTTP/1.1"); 
    client.println("Host: funnytech.atwebpages.com"); // SERVER ADDRESS HERE TOO
    client.println("Content-Type: application/x-www-form-urlencoded"); 
    client.print("Content-Length: "); 
    client.println(data.length()); 
    client.println(); 
    client.print(data); 
    Serial.println("Data sent...");
  }
  // client disconnection
  if (client.connected()) { 
    client.stop();  // DISCONNECT FROM THE SERVER
  }
}

// ===========================================================================================
// ===========================================================================================


void setup() {

   // initialize clock
  setSyncProvider(RTC.get);
  
  // ==== inizialize button pin =======================
  // button a is to upload immediately data on line
  // button b is to switch display
  pinMode(BUTTONPINA, INPUT);
  pinMode(BUTTONPINB, INPUT);
  // ==================================================
  
  // initialize serial:
  Serial.begin(115200);
  
  // ===== initialize display ===============
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Clear the buffer display
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  // ========================================

  welcomeMessage();

  // ==== Connection to home wifi ============================
  display.clearDisplay();
  display.setCursor(0,0);
  Serial.println("Attempting to connect to WPA network: ");
  Serial.print(" ");
  Serial.println(ssid);
  display.println("Attempting to connect to WPA network: ");
  display.println("");
  display.println(ssid);
  display.display();
  delay(2000);
 
  WiFi.begin(ssid, pass);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  
  display.println("");
  display.println("WiFi connected!");
  display.display();
  delay(4000);
  // ==========================================================
  
  // Start the server
  server.begin();
  display.setCursor(0,0);
  display.clearDisplay();
  Serial.println("Server started!");
  display.println("Server started!");
  display.println("");
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  display.println("");
  display.println("Connected to SSID: ");
  display.println(ssid);
  display.println("");
  display.print("With IP:");
  display.println(WiFi.localIP());
  //display the STARTUP information
  display.display();
  delay(4000);
}


void loop() {

  //reading temperature and humidity from DHT (external)
  humEx = dht.readHumidity();
  tempEx = dht.readTemperature();

  //reading temperature and humidity from DS3232 (internal)
  tempIn = RTC.temperature() / 4.;
  
  unsigned long currentMillis = millis();

  //only after selected time the data will be sent to server

  uploadInterval = map( analogRead( 0 ),0,1023,0,60 );
  interval = uploadInterval*60*1000;
  
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis; 
    uploadData();
  }
  
  buttonStateA = digitalRead(BUTTONPINA);
  buttonStateB = digitalRead(BUTTONPINB);

  Serial.println(buttonStateA);
  Serial.println(buttonStateB);
  Serial.println(uploadInterval);
  
  if (buttonStateB == HIGH) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println ("Sending data online...");
    display.display();
    uploadData();
    Serial.println("Data sent...");
    delay(2000);
    display.println ("");
    display.println ("...Data sent!");;
    display.display();
    delay(2000);
  }
  
  if (buttonStateA == HIGH) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.print ("Device IP: ");
    display.println (WiFi.localIP());
    display.println ("");
    display.println ("Additional space to add information");
    display.display();
       
  } else {
   
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    time_t t = now();
    if ( String(hour(t)).length() == 1 ) display.print ("0");
    display.print (hour(t));
    display.print (":");
    if ( String(minute(t)).length() == 1 ) display.print ("0"); 
    display.print (minute(t));
    display.print (" - ");
    display.print (day(t));
    display.print ("/");
    display.print (month(t));
    display.print ("/");
    display.println (year(t));
    display.println("");
    display.println("Tmp:" +  String(tempEx) + "C" );
    display.println("Hum:" + String(humEx)  + "%" );
    display.println("");
    display.println("Upload inter:" + String(uploadInterval) + (" mins.") );
    display.display();
  }
}
