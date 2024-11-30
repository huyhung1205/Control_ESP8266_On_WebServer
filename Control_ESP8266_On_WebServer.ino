#include <EEPROM.h>            // Thư viện lưu trữ non-volatile memory
#define EEPROM_SIZE 96         // Định nghĩa kích thước của bộ nhớ EEPROM
#include <ESP8266WiFi.h>       // Thư viện dùng để kết nối WiFi của ESP8266
#include <ESP8266WiFiMulti.h>  // Thư viện dùng để kết nối WiFi đa cấu hình của ESP8266
#include <ESP8266WebServer.h>  // Thư viện dùng để tạo WebServer
ESP8266WiFiMulti My_wifi;      // Khởi tạo đối tượng My_wifi từ ESP8266WiFiMulti
ESP8266WebServer server(80);   // Khởi tạo đối tượng WebServer trên cổng 80
char ssid[32] = "";            // Biến lưu SSID
char password[64] = "";        // Biến lưu Password

// Hàm nhập WiFi
void handleRoot() {
  // HTML form để nhập tên WiFi và mật khẩu
  const char* htmlForm = R"=====(
<!DOCTYPE html>
<html lang="vi">
<head>
    <title>CONNECTING WIFI</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            text-align: center;
            background-image: url(https://i.ibb.co/8PmRMCw/mm.jpg);
            background-size: 100%;
            background-repeat: repeat;
            padding: 100px
        }

        input {
            border: thick double #ff0000;
            font-size: 18px;
            padding: 10px;
            margin: 5px;
        }

        button {
            font-size: 30px;
            padding: 10px;
        }

        .error {
            color: red;
        }

        input {
            border: 3px solid rgb(255, 0, 0);
            width: 300px;
            height: 50px;
            padding: 10px;
            font-size: 20px;
            margin: 10px;
        }
    </style>
</head>
<body>
    <h1 style="font-size: 120px; color: rgb(255, 0, 0);text-align: center;">WiFi<br>Connect</h1>
    <form action="/setwifi" method="post"
        style="color: red; font-family:'Times New Roman', Times, serif; font-size: 40px; text-align:center;">
        <label for="ssid">SSID:</label><br>
        <input type="text" id="ssid" name="ssid" required><br>
        <label for="password">Password:</label><br>
        <input type="text" id="password" name="password" required><br><br>
        <button type="submit">Connect</button>
    </form>
    <div id="status"></div>
</body>
</html>
)=====";
  server.send(200, "text/html", htmlForm);
}
// Hàm xử lý yêu cầu kết nối WiFi
void handleSetWiFi() {
  String ssidStr = server.arg("ssid");
  String passwordStr = server.arg("password");

  if (ssidStr.length() > 0 && passwordStr.length() > 0) {
    // Lưu dữ liệu vào EEPROM
    ssidStr.toCharArray(ssid, sizeof(ssid));
    passwordStr.toCharArray(password, sizeof(password));
    saveWiFiConfig(ssid, password);

    WiFi.begin(ssid, password);
    server.send(200, "text/html", "<h1>Connecting to " + ssidStr + "...</h1><script>setTimeout(function(){ window.location = '/check'; }, 5000);</script>");
  } else {
    server.send(200, "text/html", "<h1>Invalid SSID or Password</h1>");
  }
}
// Hàm kiểm tra trạng thái kết nối WiFi
void handleCheck() {
  if (WiFi.status() == WL_CONNECTED) {
    server.send(200, "text/html", "<h1>Connected to WiFi!<br>Please reset board ESP8266</h1>");
  } else {
    server.send(200, "text/html", "<h1 style=\"color: red; font-family:'Times New Roman'; font-size: 50px;\" >Failed to Connect.<br>Please <a href='/input'>Try again</a>.</h1>");
  }
}
// Hàm trang chính
void handleMain() {
  // HTML
  const char* control = R"=====(
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset='UTF-8'>
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
    <script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>
    <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">
</head>
<body>
    <div class=\"container\">
        <style>
            .button {
                border: thick double #ff0000;
                font-size: 80px;
            }
            td {
                padding: 30px;
                margin-bottom: 30px;
            }
            body {
                background-image: url(https://i.ibb.co/8PmRMCw/mm.jpg);
                background-size: 100%;
                background-repeat: repeat;
                text-align: center;
                padding: 80px;
            }
        </style>
        <center>
            <h3 style=" font-size: 120px; color: rgb(255, 0, 0);">CONTROL<br>ESP8266</h3>
            <div class=\"row\">
                <table style="color: red; font-family:'Times New Roman', Times, serif; font-size: 80px; font-weight: bold;">
                    <tr>
                        <td>D1</td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D1' value='ON'></form>
                        </td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D1' value='OFF'></form>
                        </td>
                    </tr>
                    <tr>
                        <td>D2</td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D2' value='ON'></form>
                        </td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D2' value='OFF'></form>
                        </td>
                    </tr>
                    <tr>
                        <td>D3</td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D3' value='ON'></form>
                        </td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D3' value='OFF'></form>
                        </td>
                    </tr>
                    <tr>
                        <td>D4</td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D4' value='ON'></form>
                        </td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D4' value='OFF'></form>
                        </td>
                    </tr>
                    <tr>
                        <td>D5</td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D5' value='ON'></form>
                        </td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D5' value='OFF'></form>
                        </td>
                    </tr>
                    <tr>
                        <td>D6</td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D6' value='ON'></form>
                        </td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D6' value='OFF'></form>
                        </td>
                    </tr>
                    <tr>
                        <td>D7</td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D7' value='ON'></form>
                        </td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D7' value='OFF'></form>
                        </td>
                    </tr>
                    <tr>
                        <td>D8</td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D8' value='ON'></form>
                        </td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D8' value='OFF'></form>
                        </td>
                    </tr>
                    <tr>
                        <td>D</td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D' value='ON'></form>
                        </td>
                        <td>
                            <form menthod='get'><input class='button' type='submit' name='D' value='OFF'></form>
                        </td>
                    </tr>
                </table>
        </center>
    </div>
</body>
</html>
)=====";
  server.send(200, "text/html", control);
}
// Hàm kết nối WiFi trong danh sách
void Connect_listWiFi(char* ssid, char* password) {
  My_wifi.addAP(ssid, password);        // Mạng WiFi mới nhất
  My_wifi.addAP("ssid1", "password1");  // Kết nối vào mạng WiFi 1
  My_wifi.addAP("ssid2", "password2");  // Kết nối vào mạng WiFi 2
}
// Hàm lưu thông tin WiFi vào EEPROM
void saveWiFiConfig(const char* ssid, const char* password) {
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < 32; i++) {
    EEPROM.write(i, ssid[i]);
  }
  for (int i = 0; i < 64; i++) {
    EEPROM.write(32 + i, password[i]);
  }
  EEPROM.commit();
  EEPROM.end();
}
// Hàm đọc thông tin WiFi từ EEPROM
void loadWiFiConfig(char* ssid, char* password) {
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < 32; i++) {
    ssid[i] = EEPROM.read(i);
  }
  for (int i = 0; i < 64; i++) {
    password[i] = EEPROM.read(32 + i);
  }
  EEPROM.end();
}
const int digital[] = { D1, D2, D3, D4, D5, D6, D7, D8 };
//==============================================================
//                  SETUP
//==============================================================
void setup() {
  Serial.begin(9600);  // Khởi tạo kết nối Serial để truyền dữ liệu đến máy tính
  // Cấu hình các chân Digital
  for (int i = 0; i <= 7; i++)
    pinMode(digital[i], OUTPUT);
  EEPROM.begin(EEPROM_SIZE);
  // Đọc cấu hình WiFi từ EEPROM
  loadWiFiConfig(ssid, password);
  // Kết nối với WiFi trong danh sách
  Connect_listWiFi(ssid, password);
  Serial.print("Connecting : ");
  unsigned long startAttemptTime = millis();  // Biến đém thời gian
  wl_status_t wifiStatus;                     // Biến để lưu trạng thái WiFi
  // Kiểm tra kết nối WiFi
  while ((wifiStatus = My_wifi.run()) != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }
  // Nếu không kết nối được vào mạng WiFi, ESP8266 sẽ chuyển sang chế độ AP
  if (wifiStatus != WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi not connected !!!");
    Serial.println("Starting AP");
    WiFi.softAP("Setup WiFi", "00008888");
    Serial.println("Please connect to WiFi:\nSSID:Setup WiFi\nPassword: 00008888");
    Serial.print("AP IP address: ");
    Serial.print(WiFi.softAPIP());
    Serial.println("/input");
    // Thiết lập các URL handler cho WebServer trong chế độ AP
    server.on("/input", handleRoot);                  // Phục vụ trang nhập WiFi
    server.on("/setwifi", HTTP_POST, handleSetWiFi);  // Xử lý yêu cầu kết nối WiFi
    server.on("/check", handleCheck);                 // Kiểm tra trạng thái kết nối WiFi
    server.begin();  // Bắt đầu server
  } else {
    Serial.print("\nConnected to WiFi\nIP address: ");
    Serial.println(WiFi.localIP());
  }
  // Thiết lập URL handler cho trang chính
  server.on("/", handleMain);  // Phục vụ trang điều khiển
  server.begin();  // Bắt đầu server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop() {
  server.handleClient();    // Xử lý các yêu cầu từ client
  if (server.args() > 0) {  // Nếu có các tham số được nhận
    for (uint8_t i = 0; i < server.args(); i++) {
      Serial.print("------");
      if (server.argName(i) == "D1") {
        if (server.arg(i) == "ON") {
          Serial.print("D1 ON");
          digitalWrite(D1, HIGH);
        } else {
          Serial.print("D1 OFF");
          digitalWrite(D1, LOW);
        }
      } else if (server.argName(i) == "D2") {
        if (server.arg(i) == "ON") {
          Serial.print("D2 ON");
          digitalWrite(D2, HIGH);
        } else {
          Serial.print("D2 OFF");
          digitalWrite(D2, LOW);
        }
      } else if (server.argName(i) == "D3") {
        if (server.arg(i) == "ON") {
          Serial.print("D3 ON");
          digitalWrite(D3, HIGH);
        } else {
          Serial.print("D3 OFF");
          digitalWrite(D3, LOW);
        }
      } else if (server.argName(i) == "D4") {
        if (server.arg(i) == "ON") {
          Serial.print("D4 ON");
          digitalWrite(D4, HIGH);
        } else {
          Serial.print("D4 OFF");
          digitalWrite(D4, LOW);
        }
      } else if (server.argName(i) == "D5") {
        if (server.arg(i) == "ON") {
          Serial.print("D5 ON");
          digitalWrite(D5, HIGH);
        } else {
          Serial.print("D5 OFF");
          digitalWrite(D5, LOW);
        }
      } else if (server.argName(i) == "D6") {
        if (server.arg(i) == "ON") {
          Serial.print("D6 ON");
          digitalWrite(D6, HIGH);
        } else {
          Serial.print("D6 OFF");
          digitalWrite(D6, LOW);
        }
      } else if (server.argName(i) == "D7") {
        if (server.arg(i) == "ON") {
          Serial.print("D7 ON");
          digitalWrite(D7, HIGH);
        } else {
          Serial.print("D7 OFF");
          digitalWrite(D7, LOW);
        }
      } else if (server.argName(i) == "D8") {
        if (server.arg(i) == "ON") {
          Serial.print("D8 ON");
          digitalWrite(D8, HIGH);
        } else {
          Serial.print("D8 OFF");
          digitalWrite(D8, LOW);
        }
      } else if (server.argName(i) == "D") {
        if (server.arg(i) == "ON") {
          Serial.print("ALL ON");
          for(int i=0;i<=7; i++)
            digitalWrite(digital[i],1);
        } else {
          Serial.print("ALL OFF");
          for(int i=0;i<=7; i++)
            digitalWrite(digital[i],0);
        }
      }
      Serial.println("------");
    }
    readdigital();
  }
}
// Hàm đọc trạng thái các chân digital
void readdigital() {
  for (int i = 0; i < 8; i++) {
    Serial.print("D");
    Serial.print(i + 1);
    Serial.print("-");
    int state = digitalRead(digital[i]);  // Đọc trạng thái của chân i
    if (state == HIGH)
      Serial.println("HIGH");
    else
      Serial.println("LOW");
  }
  Serial.println("");
}