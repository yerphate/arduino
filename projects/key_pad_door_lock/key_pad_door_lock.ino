#include <Keypad.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
char password[4];
char initial_password[4], new_password[4];
int vcc = 11;
int i = 0;
int relay_pin = 11;
char key_pressed = 0;
const byte rows = 4;
const byte columns = 4;
char hexaKeys[rows][columns] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte row_pins[rows] = { 2, 3, 4, 5 };
byte column_pins[columns] = { 6, 7, 8, 9 };
Keypad keypad_key = Keypad(makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(relay_pin, OUTPUT);
  pinMode(vcc, OUTPUT);


  lcd.print("Bruce's Lock Box");
  lcd.setCursor(0, 1);
  lcd.print("   Keep Out!    ");
  delay(3000);
  lcd.clear();
  lcd.print("Speak friend,");
  lcd.setCursor(0, 1);
  lcd.print("and enter:");
  initialpassword();
}
void loop() {
  digitalWrite(relay_pin, HIGH);

  key_pressed = keypad_key.getKey();
  if (key_pressed == '#')
    change();
  if (key_pressed) {
    password[i++] = key_pressed;
    lcd.print("*");
  }
  if (i == 4) {
    delay(200);
    for (int j = 0; j < 4; j++)
      initial_password[j] = EEPROM.read(j);
    if (!(strncmp(password, initial_password, 4))) {
      lcd.clear();
      lcd.print("Keep it secret!");
      lcd.setCursor(0, 1);
      lcd.print("Keep it safe!");
      digitalWrite(relay_pin, LOW);
      delay(3000);
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("Press #");
      lcd.setCursor(0, 1);
      lcd.print("to change pass");
      delay(3000);
      lcd.clear();
      lcd.print("Speak friend,");
      lcd.setCursor(0, 1);
      lcd.print("and enter:");
      i = 0;
    } else {
      digitalWrite(relay_pin, HIGH);
      lcd.clear();
      lcd.print("Your words have");
      lcd.setCursor(0, 1);
      lcd.print("no power here.");
      lcd.setCursor(0, 0);
      delay(3000);
      lcd.clear();
      lcd.print("Speak friend,");
      lcd.setCursor(0, 1);
      lcd.print("and enter:");
      i = 0;
    }
  }
}


void change() {
  int j = 0;
  lcd.clear();
  lcd.print("Current Password");
  lcd.setCursor(0, 1);
  while (j < 4) {
    char key = keypad_key.getKey();
    if (key) {
      new_password[j++] = key;
      lcd.print("*");
    }
    key = 0;
  }
  delay(500);

  if ((strncmp(new_password, initial_password, 4))) {
    lcd.clear();
    lcd.print("      YOU!      ");
    delay(2000);
    lcd.clear();
    lcd.print("   SHALL NOT!   ");
    delay(2000);
    lcd.clear();
    lcd.print("    PASS!!!     ");
    delay(3000);
    lcd.clear();
  }

  else {
    j = 0;
    lcd.clear();
    lcd.print("New Password:");
    lcd.setCursor(0, 1);

    while (j < 4) {
      char key = keypad_key.getKey();
      if (key) {
        initial_password[j] = key;
        lcd.print(key);
        EEPROM.write(j, key);
        j++;
      }
    }
    delay(500);
    lcd.clear();
    lcd.print("Pass Changed");
    lcd.setCursor(0, 1);
    delay(1000);
  }

  lcd.clear();
  lcd.print("Speak friend,");
  lcd.setCursor(0, 1);
  lcd.print("and enter.");
  key_pressed = 0;
}

void initialpassword() {
  int j;
  for (j = 0; j < 4; j++)
    EEPROM.write(j, j + 49);
  for (j = 0; j < 4; j++)
    initial_password[j] = EEPROM.read(j);
}