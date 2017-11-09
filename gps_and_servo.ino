#include "GPS_Module.h"
#include "my_servo.h"

#define SERIAL_BAUDRATE 9600

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  gps_init();
  servo_init();
}

void loop() {
  position_t pos = {0};

  get_position(&pos);

  Serial.print("Latitude = ");
  Serial.print(pos.latitude);
  Serial.print(", Longitude = ");
  Serial.println(pos.longitude);

  turn_servo(pos.latitude);  // 試しに緯度を渡す
}
