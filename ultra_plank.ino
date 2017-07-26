#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define TRIG 8 // ultrasound
#define ECHO 9 // ultrasound
#define RS 12 // lcd
#define BUTTON_START 10
#define BUTTON_CALI 13
#define BT_TX 2
#define BT_RX 3

LiquidCrystal lcd(12, 11, 4, 5, 6, 7);
SoftwareSerial BTSerial(2, 3);
// creates a 'virtual' serial port/UART
// TX-2 RX-3 VCC-3.3V GND-GND

int flag_cali = 0;
long calibration = 0;
int timesec = 0;

void setup() {
  // ultrasound setup
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // button setup
  pinMode(BUTTON_START, INPUT);
  pinMode(BUTTON_CALI, INPUT);

  // lcd setup
  lcd.begin(16, 2);

  // bluetooth setup
  BTSerial.begin(9600);
  BTSerial.println("Hello from Arduino");

  Serial.begin(9600);
}

void loop() {
  timesec = 10;
  /*
    Serial.println(BTSerial.available());
    if (BTSerial.available()) {
      Serial.write(BTSerial.read());
    }
    if (Serial.available()) {
      BTSerial.write(Serial.read());
      Serial.println("Hello from the other side");
    }
    delay(500);
  */

  digitalWrite(8, HIGH); // Ultrasound Trig
  delayMicroseconds(10);
  digitalWrite(8, LOW);

  long duration = pulseIn(9, HIGH);

  if (duration == 0 ) {
    return;
  }
  long distance = duration / 58.2;

  //calibration process
  if (digitalRead(BUTTON_CALI) == HIGH) {
    flag_cali = 1;
    Serial.print("BUTTON PRESSED : ");
    Serial.println(BUTTON_CALI);
    calibration = distance;
    lcd.clear();
    lcd.print("CALI SUCCESS!");
    lcd.setCursor(0, 1);
    lcd.print("DIST.");
    lcd.print(calibration);
    lcd.print("cm");
  }
  // press start after calibration
  else if (digitalRead(BUTTON_START) == HIGH && flag_cali != 0) {
    lcd.clear();

    for (int i = timesec; i > 0; i--) {
      lcd.clear();
      lcd.print(timesec);
      lcd.print("(sec)");
      timesec--;

     // find distance
      digitalWrite(8, HIGH);
      delayMicroseconds(10);
      digitalWrite(8, LOW);

      long duration = pulseIn(9, HIGH);

      if (duration == 0 ) {
        return;
      }
      long distance = duration / 58.2;


      //print on lcd
      lcd.setCursor(0, 1);
      lcd.print(distance);
      lcd.print("cm");
      if (distance > calibration * 1.5 || distance < calibration * 0.5) {
        lcd.print("CHEER UP!");
      }
      else {
        lcd.print("KEEP GOING!");
      }
      delay(1000);
    
}
    lcd.clear();
    lcd.print("GOOD JOB!");
    delay(1000);
  }
  else {
    // press start but no set calibration exist
    if (digitalRead(BUTTON_START) == HIGH && flag_cali == 0) {
      lcd.clear();
      lcd.print("Press the button");
      lcd.setCursor(0, 1);
      lcd.print("CALI NEEDED");
    }
    // very first state
    else {
      lcd.clear();
      lcd.print("Press the button");
    }
  }

  for (int i = 0;; i++) {
    delay(500);
    if (digitalRead(BUTTON_START) == 0 || digitalRead(BUTTON_CALI) == 0) {
      break;
    }
  }

}

