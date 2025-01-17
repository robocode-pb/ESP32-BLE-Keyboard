#include <BleKeyboard.h>  // Бібліотека для створення Bluetooth-клавіатури

#define PWMpin    2
#define PWMchanel 0
#define PWMvolume 120

#define touchSens 40 // 10-50
#define touchLen 4

int touchPin[touchLen] = {32, 33, 27, 14};
int noteFreq[7] = {524, 588, 660, 698, 784, 880, 988}; // Частоти для другої октави
int keys[7] = {KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW};

// int keys[7] = {KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, 
//                ' ', KEY_MEDIA_VOLUME_UP, KEY_MEDIA_VOLUME_DOWN};

BleKeyboard bleKeyboard("ESP32 Piano", "MyCompany", 100); // Ім'я Bluetooth-пристрою, виробник
#include "driver/dac.h"

void playTone(int frequency) {
//  ledcSetup(PWMchanel, frequency, 10);  // Канал, частота, розрядність
//  ledcAttachPin(PWMpin, PWMchanel);
//  ledcWrite(PWMchanel, PWMvolume);     // Гучність (0-255)
analogWrite(PWMpin,frequency);
}

void offTone() {
//  ledcWrite(0, 0);
analogWrite(PWMpin,0);
}

void setup() {
  Serial.begin(115200);
  pinMode(PWMpin, OUTPUT);

  bleKeyboard.begin();
}

void loop() {
  delay(50);
  if (bleKeyboard.isConnected()) { // Перевірка з'єднання
    bool needOff = false;
    for (int i = 0; i < touchLen; i++) {
      int touchValue = touchRead(touchPin[i]);
      Serial.print(String(i)+"_touchPin="+String(touchValue)+",");
      if (touchValue < touchSens) {
        playTone(noteFreq[i]);
        bleKeyboard.press(keys[i]); 
      } else {
        needOff = true;
        bleKeyboard.release(keys[i]); 
      }
    }
    Serial.println();

    if (needOff) offTone();
  } else {
    Serial.println("Waiting for Bluetooth connection...");
  }
}
