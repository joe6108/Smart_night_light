# Smart_night_light

使用Android APP透過WiFi控制小夜燈的開關以及自動收集周圍的環境狀況。

## 材料

- NodeMCU - 32s x1
- LED 數量不計
- Micro USB x1
## 環境

- Arduino IDE
- Android studio

## 功能

1. 遠端開關燈
2. 傳送溫溼度

## 實現辦法 

透過HTTP協定運作

1. http://IP/H 開燈
2. http://IP/L 關燈
3. http://IP/GET 取得當下溫溼度

