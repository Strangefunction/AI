#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// 1. Replace with your network credentials
const char* ssid = "PC-oddfunction";
const char* password = "20040801";

// 2. Replace with your API key
const char* doubao_apiKey = "7a2c5b34-faea-49cb-bf87-0baf88d65c20";

// Send request to API
String inputText = "你好，豆包！";
String apiUrl = "https://ark.cn-beijing.volces.com/api/v3/chat/completions"; // 修正了URL

String answer;
String getGPTAnswer(String inputText) {
  HTTPClient http;
  http.setTimeout(20000);
  http.begin(apiUrl);
  http.addHeader("Content-Type", "application/json");
  String token_key = String("Bearer ") + doubao_apiKey;
  http.addHeader("Authorization", token_key);
  String payload = "{\"model\":\"ep-20241216132909-549lj\",\"messages\":[{\"role\":\"system\",\"content\":\"你是我的AI助手vor,你必须用中文回答且字数不超过85个\"},{\"role\":\"user\",\"content\":\"" + inputText + "\"}],\"temperature\": 0.3}";

  int httpResponseCode = http.POST(payload);
  if (httpResponseCode == 200) { // 更严格的检查
    String response = http.getString();
    http.end();
    //Serial.println("Response: " + response); // 打印整个响应
    //Serial1.println("Response: " + response); // 打印整个响应

    // Parse JSON response
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, response);
    String outputText = jsonDoc["choices"][0]["message"]["content"];
    return outputText;
  } else {
    http.end();
    //Serial.printf("Error %i \n", httpResponseCode);
    //Serial1.printf("Error %i \n", httpResponseCode);
    return "<error>";
  }
}

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  // Explicitly set TX and RX pins for Serial1 (GPIO9 for RX and GPIO10 for TX)
  Serial1.begin(115200, SERIAL_8N1, 9, 10); // 使用 GPIO9 作为 RX，GPIO10 作为 TX

  // Connect to Wi-Fi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print('.');
    //Serial1.print('.');
    delay(1000);
  }
  //Serial.println(WiFi.localIP());
  //Serial1.println(WiFi.localIP());
  answer = getGPTAnswer(inputText);
  //Serial.println("Answer: " + answer);
  //Serial1.println("Answer: " + answer);
  //Serial.println("Enter a prompt:");
  //Serial1.println("Enter a prompt:");
}

void loop() {
  if (Serial.available()) {
    inputText = Serial.readStringUntil('\n'); // 改为换行符，更符合通常的输入习惯
    inputText.trim(); // 去除可能的空白字符
    //Serial.println("\nInput: " + inputText);
    //Serial1.println("\nInput: " + inputText);

    answer = getGPTAnswer(inputText);
    Serial.println("Answer: " + answer);
    //Serial.println("Enter a prompt:");
    Serial1.println("Answer: " + answer);
    //Serial1.println("Enter a prompt:");
  }

  if (Serial1.available()) {
    inputText = Serial1.readStringUntil('\n'); // 改为换行符，更符合通常的输入习惯
    inputText.trim(); // 去除可能的空白字符
    //Serial.println("\nInput: " + inputText);
    //Serial1.println("\nInput: " + inputText);

    answer = getGPTAnswer(inputText);
    Serial.println("Answer: " + answer);
    //Serial.println("Enter a prompt:");
    Serial1.println("Answer: " + answer);
    //Serial1.println("Enter a prompt:");
  }
}
