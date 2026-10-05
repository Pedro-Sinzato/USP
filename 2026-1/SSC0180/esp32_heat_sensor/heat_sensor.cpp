#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637Display.h>

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// TM1637 pins
#define CLK 18
#define DIO 19
TM1637Display display(CLK, DIO);

const uint8_t SEG_CHAR_C = 0b00111001;
const uint8_t SEG_BLANK  = 0x00;

void setup() {
  Serial.begin(115200);

  sensors.begin();

  display.setBrightness(7);
  display.clear();
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  Serial.println(tempC);

  if (tempC == DEVICE_DISCONNECTED_C) {
    uint8_t error[] = {
      SEG_BLANK,
      SEG_BLANK,
      SEG_BLANK,
      SEG_CHAR_C
    };
    display.setSegments(error);
    delay(1000);
    return;
  }

  // Split temperature
  int intPart = (int)tempC;
  int decPart = (int)((tempC - intPart) * 10);

  if (decPart < 0) decPart *= -1;

  if (intPart > 99) intPart = 99;

  int tens = intPart / 10;
  int units = intPart % 10;

  // Build display: XX.XC
  uint8_t data[] = {
    display.encodeDigit(tens),
    display.encodeDigit(units) | 0x80
    display.encodeDigit(decPart),
    SEG_CHAR_C
  };

  display.setSegments(data);

  delay(2000);
}