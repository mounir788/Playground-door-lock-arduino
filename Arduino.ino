#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Keypad.h>

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(A5, A4, 9, 10, 11, 12);
Servo doorServo;

// Serial to ESP: RX, TX
SoftwareSerial espSerial(A0, A1);

const int limitSwitchPin = A3;
const int servoPin = 13;
const int buzzerPin = A2;

// Keypad setup
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String password = "";

void setup() {
  Serial.begin(9600);        // Debugging
  espSerial.begin(9600);     // To ESP

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);

  pinMode(limitSwitchPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  doorServo.attach(servoPin);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == '*' || key == '#') return;

    password += key;
    lcd.setCursor(password.length() - 1, 1);
    lcd.print(key);

    if (password.length() == 6) {
      espSerial.println(password);
      delay(100);
      espSerial.println("checking"); // Tell ESP to activate checking LED

      lcd.clear();
      lcd.print("Checking...");
      password = ""; // Reset password
    }
  }

  if (espSerial.available()) {
    String line = espSerial.readStringUntil('\n');
    line.trim();

    if (line == "true") {
      correct_response();
    } else if (line == "false") {
      wrong_response();
    } else {
      lcd.clear();
      lcd.print("Connecting...");
      delay(2000);
      resetToInitial();
    }
  }
}

void correct_response() {
  lcd.clear();
  lcd.print("Correct");
  lcd.setCursor(0, 1);
  lcd.print("Opening...");
  doorServo.write(90);
  buzz_correct();

  for (int i = 10; i >= 0; i--) {
    lcd.clear();
    lcd.print("Open - ");
    lcd.print(i);
    lcd.print("s");
    delay(1000);
    if (digitalRead(limitSwitchPin) == LOW) break;
  }

  doorServo.write(0);
  resetToInitial();
}

void wrong_response() {
  lcd.clear();
  lcd.print("Wrong password");
  buzz_wrong();

  for (int i = 3; i >= 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("Retry in ");
    lcd.print(i);
    lcd.print("s ");
    delay(1000);
  }

  resetToInitial();
}

void resetToInitial() {
  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
}


void buzz_correct() {
  tone(buzzerPin, 2000, 200);  // 1kHz tone for 200 ms
  delay(250);
  tone(buzzerPin, 3000, 200);  // 1.5kHz tone for 200 ms
  delay(250);
  noTone(buzzerPin);
}

void buzz_wrong() {
  tone(buzzerPin, 400, 800);   // 400Hz tone for 500 ms
  delay(500);
  noTone(buzzerPin);
}
