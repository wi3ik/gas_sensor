#include "web_handler.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>



char webpage[] PROGMEM = R"=====(
  <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      
      <style>
        @viewport {
           width: device-width ;
           zoom: 1.0 ;
        }
        .label {
          font-size: 26px;
          color: green;
        }
        
        .button {
            background-color: #4CAF50; /* Green */
            border: none;
            color: white;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            opacity: 0.8;
            border-radius: 5px;
          }
          .button:hover {opacity: 1}
          
      </style>
      
    </head>

    <script>
    
      function toggle_led() {
        // https://www.w3schools.com/xml/xml_http.asp
        var xhr = new XMLHttpRequest();
        var url = "ledstate";

        xhr.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("led-state").innerHTML = this.responseText;
            }
          };
          xhr.open("GET", url, true);
          xhr.send();
        
      }
      document.addEventListener("DOMContentLoaded", toggle_led, false);
    </script>

    
    <body>
      <p> ESP8266: Server mode </p>
      <p class="label"> LED state: <span id="led-state">__</span></p>
      <button class="button" onclick="toggle_led()"> TOGGLE </button>
    </body>
  </html>
)=====";





// wifi network credentials to connect in server mode
char *ssid = "wi-fi";
char *password = "4e6ypawka";

// wifi network credentials to run in Access Point mode
char *ap_ssid = "no_internet";

/*
IPAddress ip(192,168,8,8);
IPAddress gateway(192,168,8,1);
IPAddress subnet(255,255,255,0); // 1111 1111.1111 1111.1111 1111.0000 0000
*/
IPAddress ip(192,168,128,2);         // 1100 0000.0000 1000.0000 1000.0000 1000
IPAddress gateway(192,255,128,1);
IPAddress subnet(255,255,255,0); // 1111 1111.1111 1111.1111 1111.0000 0000
char *mDNS_name = ap_ssid;    // TODO




int tcp_port = 80;
ESP8266WebServer server(tcp_port);

  

int init_web_handlers() {
  //int tcp_port = 80;
  //ESP8266WebServer server(tcp_port);

  WiFi.mode(WIFI_AP_STA);            // pay attention here !!!! we state that there will be TWO modes !!
  //WiFi.mode(WIFI_STA);
  //WiFi.mode(WIFI_AP);


  configureAccessPointMode();
  configureServerMode(); // station-related code
  configure_mDNS();  



  // TODO
 server.on("/", []() {

if (is_ap_mode()) {
    server.send_P(200, "text/html", webpage);   // note that here is used 'send_P' !! because we are sending data from FLASH memory
} else {
    server.send_P(200, "text/html", webpage);   // note that here is used 'send_P' !! because we are sending data from FLASH memory      
}

    Serial.print("Received request from client with IP: "); Serial.println(server.client().remoteIP().toString());
    
      Serial.printf("[%d. %d. %d. %d]\n", server.client().remoteIP()[0], server.client().remoteIP()[1],
                                          server.client().remoteIP()[2], server.client().remoteIP()[3]);

    });
  
  server.on("/ledstate", HTTP_GET, get_led_state);
  server.onNotFound(handleNotFound);
  server.begin();

  
  return 0;
} ;




void handleNotFound() {
  // Send HTTP status 404 (Not Found) when there's no handler for the URL in the request 
  server.send(404, "text/plain", "404: Not found - verify URL address.");
}

void configureAccessPointMode() {
  
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ap_ssid, password); 

  Serial.println("");
  Serial.println("Soft-AP:");
  Serial.printf("network name: %s\n", ap_ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  //Serial.printf("MAC address = %s\n", WiFi.softAPmacAddress().c_str());
}

void configureServerMode() {
  WiFi.begin(ssid, password);
// https://www.arduino.cc/en/Reference/WiFi


  Serial.println("");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500); // wait 0.25 sec
  }
  Serial.print("Ready");
  Serial.println("");
  Serial.print("Network: "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
  Serial.print("IP Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("IP Gateway: "); Serial.println(WiFi.gatewayIP());
}


void configure_mDNS() {
  // Set up mDNS responder:
  // argument is the domain name, in this example the fully-qualified domain name is "<NAME>.local"
  char *mdns_name = "esp";
  if (!MDNS.begin(mdns_name)) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.print("mDNS responder started - "); Serial.printf("'%s.local'\n", mdns_name);

  // Start TCP (HTTP) server
  server.begin();
  Serial.println("TCP server started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", tcp_port);
}


void webServiceHandler(void) {
  server.handleClient();
  MDNS.update();
}




bool is_ap_mode() {
  bool mode = false;

  if (ip[2] & server.client().remoteIP()[2])
    mode = true;
// server.client().remoteIP()

  return mode;
}

void toggle_led() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void get_led_state() {
  toggle_led();
  String led_state = digitalRead(LED_BUILTIN) ? "OFF" : "ON";
  server.send(200, "text/plain", led_state);
}
