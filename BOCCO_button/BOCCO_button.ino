#include <WiFiManager.h>
#include <WiFi.h>

#include "config.h"
#include "bocco_api.h"

//BOCCO API ライブラリ
//BOCCO アカウント情報
#define BOCCO_EMAIL     "xxxxxxxxxx"
#define BOCCO_PASSWORD  "xxxxxxxxxx"
#define BOCCO_API_KEY   "xxxxxxxxxx"
//初期設定の為のアクセスポイント名
#define AP_NAME "test"

BoccoAPI boccoAPI(BOCCO_EMAIL, BOCCO_PASSWORD, BOCCO_API_KEY);


//プッシュスイッチ
#define SW 0
#define PIN 2
#define PIN2 4

//変数初期化
int valueSW = HIGH;
int valueSW2 = 1;
int valueSW3 = 1;

//ボタン割り込み
void clickSw(){
  Serial.println("clickSw");
  valueSW = digitalRead(SW);
  Serial.println(valueSW);
}

void greetingSw(){
  Serial.println("Sw");
  //valueSW2 = digitalRead(PIN);
  Serial.println(digitalRead(PIN));
  valueSW2 = digitalRead(PIN);
}

void greetingSw2(){
  Serial.println("Sw");
  //valueSW2 = digitalRead(PIN);
  Serial.println(digitalRead(PIN2));
  valueSW3 = digitalRead(PIN2);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFiManager wifiManager;
  wifiManager.setTimeout(180);
  if(!wifiManager.autoConnect(AP_NAME)) {
    Serial.println("timeout");
    delay(3000);
    //初期設定が成功シない場合 reset して初期設定をリトライする
    ESP.restart();
    delay(5000);
    return;
  }
  //ルーターとの接続に成功
  Serial.println("connected...");
  delay(1000);

//  BOCCO アクセストークンを取得する
//  if(!boccoAPI.createSessions() ){
//    Serial.println("BOCCO アクセストークン取得に失敗");
//    return;
//  }

  //アクセストークンを設定
  boccoAPI.setAccessToken("xxxxxxxxxxxxxxxx");

  //BOCCO 1番目のルームを取得
  if(!boccoAPI.getFirstRoom()){
      Serial.println("BOCCO ルーム取得に失敗");
      return;
  }

  //boccoAPI.postMessageText("テストテスト");

  //ボタン割り込み設定
  pinMode(SW, INPUT_PULLUP);
  attachInterrupt(SW, clickSw, CHANGE );
  //ボタンがHIGHからLOWになったときに割り込み処理
  pinMode(PIN, INPUT_PULLUP);
  attachInterrupt(PIN, greetingSw, CHANGE );
  pinMode(PIN2, INPUT_PULLUP);
  attachInterrupt(PIN2, greetingSw2, CHANGE );
}



void loop() {
  if(valueSW == LOW){
    boccoAPI.postMessageText("テストに成功！");
    Serial.println("テストに成功！");
  }
  if(valueSW2 == 0){
    boccoAPI.postMessageText("おはよう！");
    Serial.println("おはよう！");
  }
  if(valueSW3 == 0){
    boccoAPI.postMessageText("ごはんがたべたい！");
    Serial.println("ごはんがたべたい！");
  }
  delay(100);
}
