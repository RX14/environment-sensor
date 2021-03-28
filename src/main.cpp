#include <Adafruit_BME280.h>
#include <MHZ19.h>
#include <SoftwareSerial.h>

Adafruit_BME280 bme280;

SoftwareSerial mh_z19_serial(16, 17);
MHZ19 mh_z19;

void setup() {
  // Setup host serial
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("\"Init!\"");

  // Turn on BME280
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);

  // Setup MH-Z19
  mh_z19_serial.begin(9600);
  mh_z19.begin(mh_z19_serial);
  mh_z19.autoCalibration(true);

  // Initialize BME280
  bool initialized = bme280.begin(0x76);
  if (!initialized) {
    Serial.print("\"Failed!\\nSensorID was: 0x");
    Serial.print(bme280.sensorID(), 16);
    Serial.println("\"");
    while (1)
      delay(10);
  }

  bme280.setSampling(
      Adafruit_BME280::MODE_FORCED, Adafruit_BME280::SAMPLING_X16,
      Adafruit_BME280::SAMPLING_X16, Adafruit_BME280::SAMPLING_X16,
      Adafruit_BME280::FILTER_OFF);
}

void loop() {
  bme280.takeForcedMeasurement();

  Serial.print("{\"temperature\": ");
  Serial.print(bme280.readTemperature());

  Serial.print(", \"pressure\": ");
  Serial.print(bme280.readPressure());

  Serial.print(", \"humidity\": ");
  Serial.print(bme280.readHumidity());

  Serial.print(", \"co2\": ");
  Serial.print(mh_z19.getCO2());

  Serial.println("}");

  delay(850);
}
