
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;

const int SPEED_PIN = 34;
const int LED_PIN = 25;
const int BUZZER_PIN = 27;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Wire.begin(21, 22);

  if (!mpu.begin()) {
    Serial.println("MPU6050 not detected!");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("Smart Vehicle Driver Behavior Detection");
  Serial.println("System ready!");
}

void loop() {
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  float ax = accel.acceleration.x;
  float ay = accel.acceleration.y;
  float gz = gyro.gyro.z * 180.0 / PI;

  int rawSpeed = analogRead(SPEED_PIN);
  float speed = rawSpeed * 150.0 / 4095.0;

  bool suddenMovement = (fabs(ax) > 2.5);
  bool sharpTurn = (fabs(ay) > 2.5 || fabs(gz) > 40.0);
  bool overspeed = (speed > 100.0);

  bool risky = suddenMovement || sharpTurn || overspeed;

  if (risky) {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 2000);
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  Serial.println("------------------------------");
  Serial.print("Speed: ");
  Serial.print(speed);
  Serial.println(" km/h");

  Serial.print("Acceleration X: ");
  Serial.println(ax);

  Serial.print("Acceleration Y: ");
  Serial.println(ay);

  Serial.print("Gyroscope Z: ");
  Serial.println(gz);

  Serial.print("Driving status: ");
  Serial.println(risky ? "RISKY" : "SAFE");

  delay(500);
}
