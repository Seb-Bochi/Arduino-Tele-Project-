#include <ESP8266WiFi.h>
#include <NTPClient.h>


/**
 * @brief A function to connect the ESP8266 to the Wi-Fi network and start the web server
 * 
 * @param ssid the name of the Wi-Fi network
 * @param password the password of the Wi-Fi network
 */

void websiteBegin(const char** ssid,const char** password ) {
  Serial.print("\n");
  Serial.print("Connecting to: ");
  Serial.println(*ssid);
  WiFi.mode(WIFI_STA);        /* Configure ESP8266 in STA Mode */
  WiFi.begin(*ssid, *password); /* Connect to Wi-Fi based on above SSID and Password */
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");
    delay(500);
  }
  Serial.print("\n");
  Serial.print("Connected to Wi-Fi: ");
  Serial.println(WiFi.SSID());
  delay(100);
  Serial.print("\n");
  Serial.println("Starting ESP8266 Web Server...");
  espServer.begin(); /* Start the HTTP web Server */
  Serial.println("ESP8266 Web Server Started");
  Serial.print("\n");
  Serial.print("The URL of ESP8266 Web Server is: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.print("\n");
  Serial.println("Use the above URL in your Browser to access ESP8266 Web Server\n");
}

/**
 * @brief A function to create a web page for the security system
 * 
 * @param armed check if the alarm is armed
 * @param breakein check if there is a break-in
 * @param tag display the last tag registered
 */
void Websiteloop(bool* armed, bool* breakein, String tag) {
  WiFiClient client = espServer.available(); /* Check if a client is available */
  if (!client) {
    return;
  }

  Serial.println("New Client!!!");

  String request = client.readStringUntil('\r'); /* Read the first line of the request from client */
  Serial.println(request);                       /* Print the request on the Serial monitor */
  /* The request is in the form of HTTP GET Method */
  client.flush();

  /* Extract the URL of the request */
  if (request.indexOf("/armedON") != -1) {
    Serial.println("The alarm is armed");
    *armed = true;
  }
  if (request.indexOf("/armedOFF") != -1) {
    Serial.println("The alarm is disarmed");
    *armed = false;
  }
  if (request.indexOf("/breakeinON") != -1) {
    Serial.println("There is a break-in");
    *breakein = true;
  }
  if (request.indexOf("/breakeinOFF") != -1) {
    Serial.println("No break-in detected");
    *breakein = false;
  }

  /* HTTP Response in the form of HTML Web Page */
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();  //  IMPORTANT
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  /* CSS Styling for Buttons and Web Page */
  client.println("<style>");
  client.println("html { font-family: Courier New; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button {border: none; color: white; padding: 10px 20px; text-align: center;");
  client.println("text-decoration: none; font-size: 25px; margin: 2px; cursor: pointer;}");
  client.println(".button1 {background-color: #00FF00;}");
  client.println(".button2 {background-color: #FF0000;}");
  client.println("</style>");
  client.println("</head>");

  /* The main body of the Web Page */
  client.println("<body>");
  client.println("<h2>Security system</h2>");

  if (*armed == false) {
    client.println("<p>Security system is disarmed</p>");
    client.print("<p><a href=\"/armedON\"><button class=\"button button1\">Click to turn ON</button></a></p>");
  } else {
    client.println("<p>Security system is armed</p>");
    client.print("<p><a href=\"/armedOFF\"><button class=\"button button2\">Click to turn OFF</button></a></p>");
  }

  if (*breakein == false) {
    client.println("<p>the allarm is OFF</p>");
    client.print("<p><a href=\"/breakeinON\"><button class=\"button button1\">Click to turn ON</button></a></p>");
  } else {
    client.println("<p style=\"color: #FFFFFF; background-color:#FF0000;\">Allarm is on Hurry up</p>");
    client.print("<p><a href=\"/breakeinOFF\"><button class=\"button button2\">Click to turn OFF</button></a></p>");
  }

  client.println("<p>Last tag registered</p>");
  client.print("<p>");
  client.print(tag);
  client.print("</p>");

  client.println("<script>setTimeout(function(){window.location.href = '/' ;}, 5000); </script>");  //auto refresh

  client.println("</body>");
  client.println("</html>");
  client.print("\n");

  delay(1);
  /* Close the connection */
  client.stop();
  Serial.println("Client disconnected");
  Serial.print("\n");
}