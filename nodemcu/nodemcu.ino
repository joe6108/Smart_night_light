#include <WiFi.h>
#include <SimpleDHT.h>
#include <ArduinoJson.h>
// DHT11設定
int pinDHT11 = 15; 
int pinLED_1 = 9; 
SimpleDHT11 dht11;

// WiFi設定
const char* ssid     = ""; //WiFi-名稱
const char* password = "";//WiFi-密碼
// 阜
WiFiServer server(80);

void setup()
{
  // band值設定
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinLED_1, OUTPUT);
  delay(10);

  digitalWrite(LED_BUILTIN, LOW);

  // We start by connecting to a WiFi network

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
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  // 至此為連線至wifi的過程
}

int value = 0;

void loop() {

  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;

  WiFiClient client = server.available();   // 監聽有無客戶端連線

  if (client) {                             // 連線成功
    Serial.println("New Client.");           // 通知有新客戶
    String currentLine = "";                // 此變數用來儲存客戶端的資料
    while (client.connected()) {            // 當持續連線時
      if (client.available()) {             // 如果有新的資料傳遞
        char c = client.read();             // 用來儲存單一byte的數據
        Serial.write(c);                    // 將其打印
        if (c == '\n') {                    // 如果讀取到換行符號，代表輸入完畢

          // 如果行數是空的，代表收到了兩次換行符號，代表http請求結束。
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");
            client.print("Click <a href=\"/GET\">here</a> to get temperature and humidity values in json format.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // 如果是收到了換行符號，則代表目前的行為空行，就清空變數
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
          digitalWrite(pinLED_1, LOW);
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
          digitalWrite(pinLED_1, HIGH);
        }
        if (currentLine.endsWith("GET /GET")) {
          // start working...
          // 開始監測溫溼度
          Serial.println("=================================");
          if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
            Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(1000);
            return;
          }
          Serial.print("Humidity = ");
          Serial.print((int)humidity);
          Serial.print("% , ");
          Serial.print("Temperature = ");
          Serial.print((int)temperature);
          Serial.println("C ");
          Serial.println("");
          String json = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";

          // 組合HTTP回應內容
          String response = "HTTP/1.1 200 OK\r\n";
          response += "Content-Type: application/json\r\n";
          response += "Content-Length: " + String(json.length()) + "\r\n";
          response += "\r\n";
          response += json;

          client.print(response);  // 發送HTTP回應給客戶端
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
