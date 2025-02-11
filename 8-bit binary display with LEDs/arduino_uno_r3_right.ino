#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

int pins[8] = {13, 12, 11, 10, 9, 8, 7, 6};
int plusPin = 3;
int minusPin = 2;
int num = 0;
int oldNum = NULL;

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 8; i++) {
    pinMode(pins[i], OUTPUT);
  }
  pinMode(plusPin, INPUT_PULLUP);
  pinMode(minusPin, INPUT_PULLUP);
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

int bin[8];

void loop() {
  static int keypadNum = NULL;
  buttonInput();
  if(Serial.available()>0){
  	keypadNum = Serial.parseInt();
  }
  if(keypadNum != 0){
    
    num = keypadNum == -1 ? 0 : keypadNum;
  }
  Serial.print("Received: ");
  Serial.println(keypadNum);
  if(num != oldNum){
    decimalToBinary(num, bin);
    for(int i = 0; i < 8; i++) {
      if(bin[i] == 1) {
        digitalWrite(pins[i], HIGH);
      } else {
        digitalWrite(pins[i], LOW);
      }
      oldNum = num;
  	}
    lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print(num);
  }

  delay(100);
}

void buttonInput() {
  static unsigned long lastDebounceTimePlus = 0;
  static unsigned long lastDebounceTimeMinus = 0;
  const unsigned long debounceDelay = 100;

  int statePlus = digitalRead(plusPin);
  int stateMinus = digitalRead(minusPin);

  unsigned long currentTime = millis();

  if(statePlus == LOW && (currentTime - lastDebounceTimePlus) > debounceDelay) {
    num++;
    num = constrain(num, 0, 255);
    lastDebounceTimePlus = currentTime;
  }

  if(stateMinus == LOW && (currentTime - lastDebounceTimeMinus) > debounceDelay) {
    num--;
    num = constrain(num, 0, 255);
    lastDebounceTimeMinus = currentTime;
  }
}

void decimalToBinary(int n, int binary[]) {
  for(int i = 0; i < 8; i++) {
    binary[7 - i] = n % 2;
    n /= 2;
  }
}
