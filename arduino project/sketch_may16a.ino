#include <Wire.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int Button = 10, fn, wn, dn, mn, wnn, security_notification;
char safeoff = 0;
char door, window, move, fire, waters;
char incomingbyte;

const int BUTTON_PIN = 25, BUTTONSEC = 23, BUTTONW, BUTTONM = 24, SENSORS = A1, threshold = 500, SENSORM = 29, SENSORW = A2;
const int LED_PIN =  22, LEDSEC = 26, LEDON = 13;
const int BUZZER1 = 27;

int buttonState = 0, btd = 0, btw = 0, btm = 0, level, sensorm1, water;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

char keypressed;
char code[] = {'1', '2', '3', '4'};

char check1[sizeof(code)];
char check2[sizeof(code)];

short a = 0, i = 0, s = 0, j = 0;

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup(){
  Serial.begin(9600);
  indicarots_setup();
  lcd.init();
  lcd.backlight();
  lcd.begin (16, 2);
  lcd.setCursor(0, 0);
  lcd.print("PRESS * TO ENTER");
  lcd.setCursor(1 , 1);
  lcd.print("PASSWORD!");
  pinMode(Button, INPUT);
}

void loop(){
  BTconnect();
  BTrecieve();
  digitalWrite(LEDON, HIGH);
  cleanSystem();
  security_ind();
  indicators();
  keypressed = myKeypad.getKey();
  if (keypressed == '*' && safeoff == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("*ENTER THE CODE*");
    ReadCode();
    if (a == sizeof(code))
      OpenDoor();
       if(safeoff == 1){
        SafeMessage();
        security_notification = 1;
       }
    else {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("CODE");
      lcd.setCursor(6, 0);
      lcd.print("INCORRECT");
      lcd.setCursor(15, 1);
      lcd.print(" ");
      lcd.setCursor(1, 1);
      lcd.print("PRESS * TO BACK!");
    }
  }

  if (keypressed == '#' && safeoff == 1){
    ChangeCode();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PRESS * TO ENTER");
    lcd.setCursor(1 , 1);
    lcd.print("PASSWORD!");
  }

if (keypressed == 'D' && safeoff == 1){
    safeoff = 0;
    lcd.clear();
    lcd.backlight();
    lcd.begin (16, 2);
    lcd.setCursor(0, 0);
    lcd.print("PRESS * TO ENTER");
    lcd.setCursor(1 , 1);
    lcd.print("PASSWORD!");
    pinMode(Button, INPUT);
    Serial.print("10");
  }
  if (digitalRead(Button) == HIGH){
  }
}

void ReadCode(){
  i = 0;
  a = 0;
  j = 0;
  while (keypressed != 'A') {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {
      lcd.setCursor(j, 1);
      lcd.print("*");
      j++;
      if (keypressed == code[i] && i < sizeof(code)) {
        a++;
        i++;
      }
      else
        a--;
    }
  }
  keypressed = NO_KEY;
}

void ChangeCode(){
  lcd.clear();
  lcd.print("Changing code");
  delay(1000);
  lcd.clear();
  lcd.print("Enter old code");
  ReadCode();

  if (a == sizeof(code)) {
    lcd.clear();
    lcd.print("Changing code");
    GetNewCode1();
    GetNewCode2();
    s = 0;
    for (i = 0 ; i < sizeof(code) ; i++) {
      if (check1[i] == check2[i])
        s++;
    }
    if (s == sizeof(code)) {

      for (i = 0 ; i < sizeof(code) ; i++) {
        code[i] = check2[i];
        EEPROM.put(i, code[i]);

      }
      lcd.clear();
      lcd.print("Code Changed");
      safeoff = 0;
      delay(2000);
    }
    else {
      lcd.clear();
      lcd.print("Codes are not");
      lcd.setCursor(0, 1);
      lcd.print("matching !!");
      delay(2000);
    }

  }

  else{
    lcd.clear();
    lcd.print("Wrong");
    delay(2000);
  }
}

void GetNewCode1() {
  i = 0;
  j = 0;
  lcd.clear();
  lcd.print("Enter new code");
  lcd.setCursor(0, 1);
  lcd.print("and press A");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Entering");

  while (keypressed != 'A') {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {
      lcd.setCursor(j, 0);
      lcd.print("*");
      check1[i] = keypressed;
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}

void GetNewCode2() {
  i = 0;
  j = 0;

  lcd.clear();
  lcd.print("Confirm code");
  lcd.setCursor(0, 1);
  lcd.print("and press A");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("and press A");

  while (keypressed != 'A') {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {
      lcd.setCursor(j, 0);
      lcd.print("*");
      check2[i] = keypressed;
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}

void OpenDoor(){
  safeoff = 1;
  security_notification = 1;
}

void SafeMessage(){
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Security OFF!");
  lcd.setCursor(1, 1);
  lcd.print("Press D to ON");
}

void cleanSystem(){
if (keypressed == 'C'){
  door = 0;
  window = 0;
  move = 0;
  fire = 0;
  fn = 0;
  wn = 0;
  dn = 0;
  mn = 0;
  Serial.print("0");
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LEDSEC, LOW);
  noTone(BUZZER1);
  sensorm1 = LOW;
  if (safeoff == 0){
      lcd.clear();
      lcd.backlight();
      lcd.begin (16, 2);
      lcd.setCursor(0, 0);
      lcd.print("PRESS * TO ENTER");
      lcd.setCursor(1 , 1);
      lcd.print("PASSWORD!");
      pinMode(Button, INPUT);
    }
  if (safeoff == 1){
    SafeMessage();
  }
  }
}

void indicators(){
  buttonState = digitalRead(BUTTON_PIN);
  if(buttonState == LOW){
    delay(250);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("fire alarm!");
    fire = 1;
    fn = 1;
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER1, 1000);
  }
  water = analogRead(SENSORW);
  if (water >= 150){
    delay(250);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("water alarm!");
    waters = 1;
    wnn = 1;
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER1, 1000);
  }
}

void security_ind(){
  btd = digitalRead(BUTTONSEC);
    if(btd == LOW && safeoff == 0){
      delay(250);
      door = 1;
    if (door == 1){
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Door alarm!");
      dn = 1;
      digitalWrite(LEDSEC, HIGH);
      tone(BUZZER1, 1000);
    }
  }
    level = analogRead(SENSORS);
    if (level > threshold && safeoff == 0){
      delay(250);
      window = 1;
    if (window == 1){
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Window alarm!");
      wn = 1;
      digitalWrite(LEDSEC, HIGH);
      tone(BUZZER1, 1000);
    }
  }
  sensorm1 = digitalRead(SENSORM);
    if(sensorm1 == HIGH && safeoff == 0){
      delay(250);
      move = 1;
    if (move == 1){
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("move alarm!");
      mn = 1;
      digitalWrite(LEDSEC, HIGH);
      tone(BUZZER1, 1000);
    }
  }
}

void indicarots_setup(){
  pinMode(LEDON, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LEDSEC, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTONSEC, INPUT_PULLUP);
  pinMode(BUTTONW, INPUT_PULLUP);
  pinMode(BUTTONM, INPUT_PULLUP);
  pinMode(BUZZER1, OUTPUT);
  pinMode(SENSORM, INPUT_PULLUP);
  digitalWrite(SENSORM, LOW);
}

void BTconnect(){
  if (Serial.available() > 0) {
      incomingbyte = Serial.read();
      if (incomingbyte == '0'){
        safeoff = 1;
        security_notification = 1;
        SafeMessage();
      }
      if (incomingbyte == '1') {
        safeoff = 0;
        lcd.clear();
        lcd.backlight();
        lcd.begin (16, 2);
        lcd.setCursor(0, 0);
        lcd.print("PRESS * TO ENTER");
        lcd.setCursor(1 , 1);
        lcd.print("PASSWORD!");
        pinMode(Button, INPUT);
      }
      if (incomingbyte == '2'){
        door = 0;
        window = 0;
        move = 0;
        fire = 0;
        fn = 0;
        wn = 0;
        dn = 0;
        mn = 0;
        if (safeoff == 0){
          lcd.clear();
          lcd.backlight();
          lcd.begin (16, 2);
          lcd.setCursor(0, 0);
          lcd.print("PRESS * TO ENTER");
          lcd.setCursor(1 , 1);
          lcd.print("PASSWORD!");
          pinMode(Button, INPUT);
    }
  if (safeoff == 1){
    SafeMessage();
  }
        Serial.print("0");
        digitalWrite(LED_PIN, LOW);
        digitalWrite(LEDSEC, LOW);
        noTone(BUZZER1);
      }
  }
}

void BTrecieve(){
  if(fn == 1){
  fn = 0;
  Serial.print("f");
  }
  if(wn == 1){
  wn = 0;
  Serial.print("w");
  }
  if(dn == 1){
  dn = 0;
  Serial.print("d");
  }
  if(mn == 1){
  mn = 0;
  Serial.print("m");
  }
  if(wnn == 1){
  wnn = 0;
  Serial.print("w1");
  }
  if (safeoff == 1){
    if (security_notification == 1){
      security_notification = 0;
      Serial.print("5");
    }
  }
}