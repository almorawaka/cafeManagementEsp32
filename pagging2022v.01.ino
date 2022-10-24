#include <ESP8266WiFi.h>

const char* ssid = "Dialog 4G"; //your WiFi Name
const char* password = "sema9454";  //Your Wifi Password
int ledPin = D1;
int ledPin2 = D0;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int red = HIGH;
  int green = LOW;
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, HIGH);
    red = HIGH;
    digitalWrite(ledPin2, LOW);
    green = LOW;
  }
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, LOW);
    red = LOW;
    digitalWrite(ledPin2, HIGH);
    green = HIGH;
    
  }

// Set ledPin according to the request
//digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<title>M01</title>");
  client.println("<style>");
  client.println("h1{ font-family: Arial, sans-serif; font-size: 40px; text-align: center;}");
  client.println("</style>");
  client.print("<h1> WElCOME TO CAFE..!  </h1>");
  client.print("<h2>  ORDER  NUMBER    M01  </h2>");

  
    if(red == HIGH) {
      client.print("<h2 style='color:red;'> NOT READY <h2>");}
     if(green == HIGH) {
      client.print("<h2 style='color:green;'> READY <h2>");}  
  
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>  READY </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button> NOT READY </button></a>");
 // client.println("<a href=\"/LED=OFF\"\"><button>Off </button></a><br />");
 
  
  client.println("<br><br>");
 
 // client.println("<a href=\"/LED=OFF\"\"><button>Off </button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}
