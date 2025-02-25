#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {
  char key = keypad.getKey();
  static int num = 0;
  static int oldNum = -1;
  static int sendFlag = false;
  switch(key){
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
    num = num * 10 + key - '0';
    break;
    case 'A': num /= 10; break;
    case 'B': num = 0; break;
    case 'C': case 'D': sendFlag = true; break;
  }
  num = constrain(num, 0, 255);
  if(num!=oldNum){
    lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print(num);
    oldNum=num;
  }
  if(sendFlag){
    num = num == 0 ? -1 : num;
    Serial.println(num);
    sendFlag = false;
  }
  delay(100);
}
