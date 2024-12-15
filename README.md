# การควบคุมมอเตอร์ด้วย Blynk และการเชื่อมต่อ WiFi

## ฮาร์ดแวร์ที่ต้องใช้
1. **บอร์ด ESP8266 (เช่น NodeMCU หรือ WeMos D1 Mini)**
2. **โมดูลมอเตอร์ L9110**
3. **มอเตอร์ 2 ตัว**
4. **แบตเตอรี่หรือแหล่งจ่ายไฟสำหรับมอเตอร์**
5. **การเชื่อมต่อ WiFi**

## ขั้นตอนการใช้งาน
1. **เชื่อมต่อฮาร์ดแวร์**:
   - ต่อมอเตอร์กับโมดูล L9110 ตามพินที่กำหนดไว้ในโค้ด (D3, D4, D8, D9)
   - ต่อบอร์ด ESP8266 กับคอมพิวเตอร์เพื่ออัปโหลดโค้ด

2. **อัปโหลดโค้ด**:
   - เปิดโค้ดนี้ใน Arduino IDE
   - ติดตั้งไลบรารี **Blynk** และ **ESP8266WiFi** หากยังไม่ได้ติดตั้ง
   - ตรวจสอบให้แน่ใจว่าใส่ **BLYNK_AUTH_TOKEN**, **WiFi SSID**, และ **Password** ถูกต้อง
   - เลือกบอร์ดและพอร์ตให้ถูกต้อง จากนั้นอัปโหลดโค้ดไปยังบอร์ด

3. **ตั้งค่าในแอป Blynk**:
   - สร้างโปรเจกต์ใหม่และเพิ่ม Virtual Pin V5 ในรูปแบบ Button หรือ Joystick
   - ใช้ค่าที่กำหนด (1, 2, 3, 4) เพื่อควบคุมทิศทาง

4. **ตรวจสอบผลลัพธ์**:
   - เปิด Serial Monitor เพื่อดูการทำงานและข้อความที่แสดง
   - ทดสอบควบคุมหุ่นยนต์ผ่านแอป Blynk

---

## คำอธิบายโค้ด

### ส่วนที่ 1: การตั้งค่าพื้นฐาน
โค้ดนี้ควบคุมมอเตอร์ผ่านโมดูล **L9110** และเชื่อมต่อกับ WiFi รวมถึงแพลตฟอร์ม **Blynk** เพื่อควบคุมทิศทางการเคลื่อนที่ของหุ่นยนต์ผ่านแอปพลิเคชัน Blynk บนมือถือ

#### ข้อมูลสำคัญในโค้ด:
- **BLYNK_TEMPLATE_ID**, **BLYNK_TEMPLATE_NAME**, และ **BLYNK_AUTH_TOKEN** ใช้สำหรับระบุเทมเพลตและโทเค็นสำหรับการเชื่อมต่อ Blynk
- **WiFi credentials**: ชื่อ WiFi (`ssid`) และรหัสผ่าน (`password`) เพื่อเชื่อมต่อเครือข่าย
- การควบคุมมอเตอร์ถูกตั้งค่าผ่าน GPIO พินของบอร์ด ESP8266

```cpp
#define BLYNK_TEMPLATE_ID "TMPL6SiGRwvNN"
#define BLYNK_TEMPLATE_NAME "robot 2"
#define BLYNK_AUTH_TOKEN "gucBr-rhRemvclGwhikVfQAcIByDw2tn"

const char* ssid = "MyWiFi";
const char* password = "zxcvbnm1234";
```

### ส่วนที่ 2: การควบคุมมอเตอร์
ใช้พินดังนี้:
- **A1A และ A1B** สำหรับควบคุมมอเตอร์ A
- **B1A และ B1B** สำหรับควบคุมมอเตอร์ B

```cpp
#define A1A D3  // Motor A Pin 1
#define A1B D4  // Motor A Pin 2
#define B1A D8  // Motor B Pin 1
#define B1B D9  // Motor B Pin 2
```

ฟังก์ชัน `moveDirection(String direction)` ใช้สำหรับกำหนดทิศทางการเคลื่อนที่ของมอเตอร์ โดยสามารถเคลื่อนไป **ซ้าย (left)**, **ขวา (right)**, **เดินหน้า (straight)**, **ถอยหลัง (reverse)** หรือ **หยุด (stop)** ได้ตามค่าที่ส่งเข้าไปในฟังก์ชัน

```cpp
void moveDirection(String direction) {
  if (direction == "left") {
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, HIGH);
    digitalWrite(B1A, HIGH);
    digitalWrite(B1B, LOW);
    Serial.println("Moving: LEFT");
  } else if (direction == "straight") {
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, HIGH);
    digitalWrite(B1A, LOW);
    digitalWrite(B1B, HIGH);
    Serial.println("Moving: STRAIGHT");
  } else if (direction == "right") {
    digitalWrite(A1A, HIGH);
    digitalWrite(A1B, LOW);
    digitalWrite(B1A, LOW);
    digitalWrite(B1B, HIGH);
    Serial.println("Moving: RIGHT");
  } else if (direction == "reverse") {
    digitalWrite(A1A, HIGH);
    digitalWrite(A1B, LOW);
    digitalWrite(B1A, HIGH);
    digitalWrite(B1B, LOW);
    Serial.println("Moving: REVERSE");
  } else if (direction == "stop") {
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, LOW);
    digitalWrite(B1A, LOW);
    digitalWrite(B1B, LOW);
    Serial.println("Moving: STOP");
  }
}
```

### ส่วนที่ 3: การเชื่อมต่อกับ WiFi และ Blynk
- **ฟังก์ชัน connectToWiFi()**: ใช้สำหรับเชื่อมต่อกับเครือข่าย WiFi และแสดง IP Address บน Serial Monitor

```cpp
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
```
- **Blynk.begin()**: ใช้สำหรับเชื่อมต่อกับแพลตฟอร์ม Blynk

### ส่วนที่ 4: การรับค่าจาก Blynk
- **BLYNK_WRITE(V5)**: รับค่าจาก Virtual Pin V5 ในแอป Blynk เพื่อควบคุมทิศทางของหุ่นยนต์ โดยมีค่าดังนี้:
  - `1`: เดินหน้า (straight)
  - `2`: ถอยหลัง (reverse)
  - `3`: เลี้ยวซ้าย (left)
  - `4`: เลี้ยวขวา (right)

```cpp
BLYNK_WRITE(V5) {
  int buttonValue = param.asInt();

  Serial.println(buttonValue);
  switch (buttonValue) {
    case 1:
      moveDirection("straight");
      break;
    case 2:
      moveDirection("reverse");
      break;
    case 3:
      moveDirection("left");
      break;
    case 4:
      moveDirection("right");
      break;
    default:
      moveDirection("stop");
      break;
  }
}
```

---

## ผลลัพธ์ที่คาดหวัง
1. หุ่นยนต์สามารถเคลื่อนที่ไปในทิศทางที่กำหนด (ซ้าย, ขวา, เดินหน้า, ถอยหลัง) เมื่อได้รับคำสั่งจากแอป Blynk
2. แสดงข้อความการเคลื่อนไหวและการเชื่อมต่อ WiFi บน Serial Monitor

