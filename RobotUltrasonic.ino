/*
 * Control motor to move: left, straight, right, reverse, and stop
 * Connect to WiFi and Blynk
 * Motor Pins for L9110
 */
// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL6SiGRwvNN"
#define BLYNK_TEMPLATE_NAME "robot 2"
#define BLYNK_AUTH_TOKEN "gucBr-rhRemvclGwhikVfQAcIByDw2tn"

#include <ESP8266WiFi.h>         // Include ESP8266 WiFi library
#include <BlynkSimpleEsp8266.h>  // Include Blynk library

// WiFi credentials
const char* ssid = "MyWiFi";
const char* password = "zxcvbnm1234";

// Motor Pins for L9110
#define A1A D3  // Motor A Pin 1
#define A1B D4  // Motor A Pin 2
#define B1A D8  // Motor B Pin 1
#define B1B D9  // Motor B Pin 2

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("System initializing...");

  // Set motor pins as output
  pinMode(B1A, OUTPUT);
  pinMode(B1B, OUTPUT);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);

  // Connect to WiFi and Blynk
  connectToWiFi();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  Serial.println("Connected to Blynk!");
}

void loop() {
  Blynk.run();  // Process Blynk commands
}

/*
 * @connectToWiFi
 * Connects to the specified WiFi network
 */
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

/*
 * @moveDirection
 * Moves the robot in specified direction
 * direction: "left", "straight", "right", "reverse", or "stop"
 */
void moveDirection(String direction) {
  if (direction == "left") {
    // Motor A forward, Motor B backward
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, HIGH);
    digitalWrite(B1A, HIGH);
    digitalWrite(B1B, LOW);
    Serial.println("Moving: LEFT");
  } else if (direction == "straight") {
    // Both motors forward
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, HIGH);
    digitalWrite(B1A, LOW);
    digitalWrite(B1B, HIGH);
    Serial.println("Moving: STRAIGHT");
  } else if (direction == "right") {
    // Motor A backward, Motor B forward
    digitalWrite(A1A, HIGH);
    digitalWrite(A1B, LOW);
    digitalWrite(B1A, LOW);
    digitalWrite(B1B, HIGH);
    Serial.println("Moving: RIGHT");
  } else if (direction == "reverse") {
    // Both motors backward
    digitalWrite(A1A, HIGH);
    digitalWrite(A1B, LOW);
    digitalWrite(B1A, HIGH);
    digitalWrite(B1B, LOW);
    Serial.println("Moving: REVERSE");
  } else if (direction == "stop") {
    // Stop both motors
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, LOW);
    digitalWrite(B1A, LOW);
    digitalWrite(B1B, LOW);
    Serial.println("Moving: STOP");
  }
}

/*
 * @BLYNK_WRITE(V5)
 * Receives values from Virtual Pin V5
 */
BLYNK_WRITE(V5) {
  int buttonValue = param.asInt();  // Read the value from the button

  Serial.println(buttonValue);
  switch (buttonValue) {
    case 1:
      moveDirection("straight");  // Move Straight
      break;
    case 2:
      moveDirection("reverse");  // Move Reverse
      break;
    case 3:
      moveDirection("left");  // Move Left
      break;
    case 4:
      moveDirection("right");  // Move Right
      break;
    default:
      moveDirection("stop");  // Stop
      break;
  }
}
