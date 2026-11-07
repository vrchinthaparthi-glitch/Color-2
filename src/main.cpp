#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_APDS9960.h>

Adafruit_SSD1306 oled(128, 64, &Wire);
Adafruit_APDS9960 apds;

void print(int line, String s) {
    oled.setCursor(0, line*8);
    oled.print(s);
}
void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.setSDA(0);
    Wire.setSCL(1);
    Wire.begin();

    uint8_t voltage = 0x02;
    uint8_t i2c_addr = 0x3D;

    if(oled.begin(voltage, i2c_addr) == 0) {
        Serial.println("OLED init failed");
        while(1);
    } 
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
    oled.setTextSize(1);
    print(1, "OLED ok");

    if(apds.begin() == 0) {
        Serial.println("APDS init failed");
        print(2, "COLOR FAILED");
        oled.display();
        while(1);
    }
    apds.enableColor(true);
    print(2, "COLOR ok");
  
    oled.display();

    apds.begin(10, APDS9960_AGAIN_4X, APDS9960_ADDRESS, &Wire);
    apds.begin(30, APDS9960_AGAIN_4X, APDS9960_ADDRESS, &Wire);
    apds.begin(50, APDS9960_AGAIN_16X, APDS9960_ADDRESS, &Wire);
}
void loop() {
    char buff[100];
    uint16_t r, g, b, c;
    apds.getColorData(&r, &g, &b, &c);

    oled.clearDisplay();

    sprintf(buff, "r: %3d", r);
    print(0, buff);

    sprintf(buff, "g: %3d", g);
    print(2, buff);

    sprintf(buff, "b: %3d", b);
    print(4, buff);

    sprintf(buff, "c: %3d", c);
    print(6, buff);
    oled.display();
}