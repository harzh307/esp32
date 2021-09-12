#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif


#define LED1 2
#define LED2 2
const char* ssid = "DSP";
const char* password = "Mp@#05092005";
//const char* ssid = "Akpatel";
//const char* password = "iekd6510";

char webpage[] PROGMEM = R"=====(

<!doctype html>
  <html>
  <head>
    <title>LED</title>
  </head>
  <body>
  <style>
body
{
  margin: 0;
  padding: 0;
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
  background-image: url("https://images.hdqwalls.com/download/wolf-howling-to-moon-3840x2400.jpg");
  background-repeat: no-repeat;
  background-size: 100%;
  font-family: consolas;
  flex-direction: column;
}
h1{
  font-family: consolas;
  padding-bottom: 30px;
  color: antiquewhite;
}
h3{
  font-family: consolas;
  padding-bottom: 10px;
  color: antiquewhite;
}
div{
  display:flex;
  align-self: center;
}
a
{
  position:relative;
  display: block;
  margin: 30px;
  padding: 15px 30px;
  color: antiquewhite;
  text-transform: uppercase;
  text-decoration: none;
  font-size: 24px;
  overflow: hidden;
  transition: 0.2s;
}
a.on:hover
{
  color: rgb(0, 0, 0);
  background: #21f32c;
  box-shadow: 0 0 10px #21f32c, 0 0 40px #21f32c, 0 0 80px #21f32c;
  transition-delay: 1s;
}
a.on span
{
  position: absolute;
  display: block;
}
a.on span:nth-child(1)
{
  top: 0;
  left: -100%;
  height: 100%;
  width: 2px;
  background: linear-gradient(90deg,transparent,#21f32c);
}
a.on:hover span:nth-child(1)
{
  left: 100%;
  transition: 1s;
}
a.on span:nth-child(3)
{
  bottom: 0;
  right: -100%;
  height: 100%;
  width: 2px;
  background: linear-gradient(270deg,transparent,#21f32c);
}
a.on:hover span:nth-child(3)
{
  right: 100%;
  transition: 1s;
  transition-delay: 0.5s;
}
a.on span:nth-child(2)
{
  top: -100%;
  right: 0;
  height: 2px;
  width: 100%;
  background: linear-gradient(180deg,transparent,#21f32c);
}
a.on:hover span:nth-child(2)
{
  top: 100%;
  transition: 1s;
  transition-delay: 0.25s;
}
a.on span:nth-child(4)
{
  bottom: -100%;
  left: 0;
  height: 2px;
  width: 100%;
  background: linear-gradient(360deg,transparent,#21f32c);
}
a.on:hover span:nth-child(4)
{
  bottom: 100%;
  transition: 1s;
  transition-delay: 0.50s;
}


a.off:hover
{
  color: rgb(0, 0, 0);
  background: #f32121;
  box-shadow: 0 0 10px #f32121, 0 0 40px #f32121, 0 0 80px #f32121;
  transition-delay: 1s;
}
a.off span
{
  position: absolute;
  display: block;
}
a.off span:nth-child(1)
{
  top: 0;
  left: -100%;
  height: 100%;
  width: 2px;
  background: linear-gradient(90deg,transparent,#f32121);
}
a.off:hover span:nth-child(1)
{
  left: 100%;
  transition: 1s;
}
a.off span:nth-child(3)
{
  bottom: 0;
  right: -100%;
  height: 100%;
  width: 2px;
  background: linear-gradient(270deg,transparent,#f32121);
}
a.off:hover span:nth-child(3)
{
  right: 100%;
  transition: 1s;
  transition-delay: 0.5s;
}
a.off span:nth-child(2)
{
  top: -100%;
  right: 0;
  height: 2px;
  width: 100%;
  background: linear-gradient(180deg,transparent,#f32121);
}
a.off:hover span:nth-child(2)
{
  top: 100%;
  transition: 1s;
  transition-delay: 0.25s;
}
a.off span:nth-child(4)
{
  bottom: -100%;
  left: 0;
  height: 2px;
  width: 100%;
  background: linear-gradient(360deg,transparent,#f32121);
}
a.off:hover span:nth-child(4)
{
  bottom: 100%;
  transition: 1s;
  transition-delay: 0.50s;
}
  </style>
  <h1>ESP32 IOT Project</h1>

  <h3>LED1</h3>
                
  <div>
  <a href="#" class="on" onclick= "window.location = 'http://'+location.hostname+'/led1/on'">
                        <span></span>
                        <span></span>
                        <span></span>
                        <span></span>
                         ON
  </a>
  <a href="#" class="off" onclick= "window.location = 'http://'+location.hostname+'/led1/off'">
                        <span></span>
                        <span></span>
                        <span></span>
                        <span></span>
                         OFF 
  </a>          
  </div>

  <h3>LED2</h3>

  <div>
  <a href="#" class="on" onclick= "window.location = 'http://'+location.hostname+'/led2/on'">
                        <span></span>
                        <span></span>
                        <span></span>
                        <span></span>
                         ON
  </a>
  <a href="#" class="off" onclick= "window.location = 'http://'+location.hostname+'/led2/off'">
                        <span></span>
                        <span></span>
                        <span></span>
                        <span></span>
                         OFF 
  </a>  
  </div>
</body>
</html>     



)=====";

// ipaddress/led1/on
//ipaddress/led1/off

// ipaddress/led2/on
//ipaddress/led2/off
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80); // server port 80



void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}

void setup(void)
{
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  
//  WiFi.softAP("Dhruv", "12345678");
//  Serial.println("softap");
//  Serial.println("");
//  Serial.println(WiFi.softAPIP());


  if (MDNS.begin("ESP")) //esp.local/
  { 
    Serial.println("MDNS responder started");
  }



  server.on("/", [](AsyncWebServerRequest * request)
  { 
   
  request->send_P(200, "text/html", webpage);
  });

   server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    digitalWrite(LED1,HIGH);
  request->send_P(200, "text/html", webpage);
  });

   server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    digitalWrite(LED1,LOW);
  request->send_P(200, "text/html", webpage);
  });

   server.on("/led2/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    digitalWrite(LED2,HIGH);
  request->send_P(200, "text/html", webpage);
  });

   server.on("/led2/off", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    digitalWrite(LED2,LOW);
  request->send_P(200, "text/html", webpage);
  });

  server.onNotFound(notFound);

  server.begin();  // it will start webserver
}
void loop(void)
{
}
