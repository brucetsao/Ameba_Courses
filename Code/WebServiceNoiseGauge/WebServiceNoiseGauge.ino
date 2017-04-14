#include <WiFi.h>

char ssid[] = "BruceSonyC5";      // your network SSID (name)
char pass[] = "12345678";     // your network password
  // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

#define SoundPin 1
int SoundValue = 0;
int SoundValue1 = 0;
void setup() {
  Serial.begin(9600);      // initialize serial communication
   // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }
  
  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
//    status = WiFi.begin(ssid);    //no pass
    status = WiFi.begin(ssid, pass);    //wpa use
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  SoundValue = analogRead(SoundPin) ;
  SoundValue1 = map(analogRead(SoundPin),0,1023,0,100)  ;
  // wifi code here
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("Now Someone Access WebServer");

    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank)
        {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          client.println("<head>");
          client.println("<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>");
          client.println("<script type='text/javascript'>");
          client.println("google.charts.load('current', {'packages':['gauge']});");
          client.println("google.charts.setOnLoadCallback(drawChart);");
          client.println("  function drawChart() {");
          client.println("var data = google.visualization.arrayToDataTable([['Label', 'Value'],['Noise',");
          client.println(SoundValue1);
          client.println("],]);");
          client.println(" var options = {width: 300, height: 300,redFrom: 90, redTo: 100,yellowFrom:75, yellowTo: 90,minorTicks: 5};");
          client.println("var chart = new google.visualization.Gauge(document.getElementById('chart_div'));");
          client.println("chart.draw(data, options);");
          client.println("}");
          client.println("</script>");
          client.println("</head>");
          client.println("<meta charset='utf-8'>");
          client.println("<body>");
          client.println("<div id='chart_div' style='width: 300px; height: 300px;'></div>");
          client.println("</body>");
          /*
            client.print("Humidity: ");
            client.print(h);
            client.print(" % and ");
            client.print("Temperature: ");
            client.print(t);
            client.print("*C   and  ");
            client.print(f);
            client.print("*F  (end) ");
            client.println("<br />");
          */
          client.println("</html>");
          break;
        }
        if (c == '\n')
        {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r')
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }


    delay(800) ;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
