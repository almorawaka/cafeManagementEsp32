// Load Wi-Fi library
#include <Adafruit_NeoPixel.h>
#define PIN 15
#include <WiFi.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
// Replace with your network credentials
const char* ssid = "Dialog 4G"; //your WiFi Name
const char* password = "sema9454";  //Your Wifi Password

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 2;
const int output27 = 4;
const int output5 = 5;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  pinMode(output5, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, HIGH);
  digitalWrite(output27, LOW);
  digitalWrite(output5, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if ((header.indexOf("GET /26/on") >= 0)) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
              for (int i = 255; i < 2; i--) {
              for (int j = 255; j < 2; j--) {
              for (int k = 255; k < 2; k--) {
               
              pixels.setPixelColor(0, pixels.Color(i  , j , k )); // Moderately bright green color.
              pixels.show(); // This sends the updated pixel color to the hardware.
              delay(200); // Delay for a period of time (in milliseconds).
                       }
                    }
                  }
              digitalWrite(output26, HIGH);
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
              digitalWrite(output5, LOW);
              
            } else if ((header.indexOf("GET /26/off") >= 0)) {
              Serial.println("GPIO 26 off");   
              output26State = "off";
              digitalWrite(output26, LOW);
              Serial.println("GPIO 27 on");
              output27State = "on";
              // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
              for (int i = 0; i < 2; i++) {
              for (int j = 0; j < 2; j++) {
              for (int k = 0; k < 2; k++) {
               
              pixels.setPixelColor(0, pixels.Color(i * 255, j * 255, k * 255)); // Moderately bright green color.
              pixels.show(); // This sends the updated pixel color to the hardware.
              delay(200); // Delay for a period of time (in milliseconds).
                       }
                    }
                  }
               digitalWrite(output27, HIGH);
              
              int i = 0;
              do{
                i++;
                digitalWrite(output5, HIGH);
                delay(100);
                digitalWrite(output5, LOW);
                delay(100);
                }while(i<5);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<title>M01</title>");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            if (output26State=="off") {
            client.println("body {background-color: #50C878;}");
              }
            else{
               client.println("body {background-color: #ed2939;}");
              }
            client.println("h1 {color: #E6E6FA;}");
            
            client.println(".button2 {background-color:#ad0e0e;}</style></head>");
            
            // Web Page Heading
            client.println("<body ><h1>WELCOME TO CAFE... </h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            //client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ORDER IS READY </button></a></p>");
             
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\"> ORDER BEING PROCESSED</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
//            client.println("<p>GPIO 27 - State " + output27State + "</p>");
//            // If the output27State is off, it displays the ON button       
//            if (output27State=="off") {
//              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
//            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
