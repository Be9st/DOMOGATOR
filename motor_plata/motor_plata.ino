#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Color.h>

#define SLAVE_ADDRESS 0x04
int number = 0;

SoftwareSerial softSerial(12, 13); // RX, TX

Color Color(52, 50, 48, 46, 44, 42, 40);

byte lastVal;

int RPWM_Output1 = 7;
int LPWM_Output1 = 6;

int RPWM_Output2 = 9;
int LPWM_Output2 = 8;

int i = 0;
int r1, l1, r2, l2;

bool rgb = false;

void setup()
{
  Serial.begin(9600);
  softSerial.begin(9600);
  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);

  Color.power(LOW);
  pinMode(RPWM_Output1, OUTPUT);//engine
  pinMode(LPWM_Output1, OUTPUT);

  pinMode(RPWM_Output2, OUTPUT);
  pinMode(LPWM_Output2, OUTPUT);

  pinMode(5, OUTPUT);//насос и датчик работы
  pinMode(4, OUTPUT);

  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  digitalWrite(36, 255);
  digitalWrite(37, 255);
}

void loop()
{
  if (softSerial.available())
  {
    int x = softSerial.read();
    Serial.println(x);
    switch (x)
    {
      case 1://engine вперед
        i = 100;
        l1 = 0;
        r1 = 150;
        l2 = 150;
        r2 = 0;
        break;
      case 2://engine назад
        i = 100;
        l1 = 150;
        r1 = 0;
        l2 = 0;
        r2 = 150;
        break;

      case 3://направо
        i = 100;
        l1 = 150;
        r1 = 0;
        l2 = 150;//
        r2 = 0;
        break;
      case 4://налево
        i = 100;
        l1 = 0;
        r1 = 150;
        l2 = 0;
        r2 = 150;//
        break;

      case 5://насос
        if (digitalRead(5) == HIGH) {
          digitalWrite(5, LOW);
          digitalWrite(4, LOW);
        }
        else {
          digitalWrite(5, HIGH);
          digitalWrite(4, HIGH);
        }
        break;

      case 6://проверка цвета
        if (rgb == true) {
          Color.power(LOW);
          Color.light(LOW);
          rgb = false;
          Serial.println(00);
        }
        else {
          Color.power(HIGH);
          Color.light(HIGH);
          rgb = true;
          Serial.println(99);
        }
        break;

      case 7://macro
        macro();
        break;
    }

  }
  if (i > 0) {
    analogWrite(LPWM_Output1, l1);
    analogWrite(RPWM_Output1, r1);
    analogWrite(LPWM_Output2, l2);
    analogWrite(RPWM_Output2, r2);
    i--;
    delay(1);
  }
  else {
    analogWrite(LPWM_Output1, 0);
    analogWrite(RPWM_Output1, 0);
    analogWrite(LPWM_Output2, 0);
    analogWrite(RPWM_Output2, 0);
  }

  if (rgb == true) {
    Color.read();             //макросы цвета
    Color.getColor();
    lastVal = Color.filtr();
    if (lastVal == 1) {
      Serial.println("RED");
      Color.power(LOW);
      Color.light(LOW);
      rgb = false;
      Serial.println("Macro!");
      macro();
      lastVal=0;
      delay(5000);
    }

    //белый
    if (lastVal == 4) {
      Serial.println("WHITE");
    }
  }

  receiveData();
}

void macro() {
  Serial.println("A");
  analogWrite(14, 255);
  delay(1000);
  analogWrite(14, 0);

  analogWrite(LPWM_Output1, 0);
  analogWrite(RPWM_Output1, 150);
  analogWrite(LPWM_Output2, 150);
  analogWrite(RPWM_Output2, 0);

  delay(3000);

  analogWrite(LPWM_Output1, 0);
  analogWrite(RPWM_Output1, 0);
  analogWrite(LPWM_Output2, 0);
  analogWrite(RPWM_Output2, 0);
  delay(5000);
  softSerial.write(10);

}

void receiveData() {
  while (Wire.available()) {
    //считывание значений
    number = Wire.read();
    Serial.print("data received: ");
    Serial.println(number);

    if (number == 3) {
      Serial.println("Macro!");
      analogWrite(14, 255);
  delay(3000);
  analogWrite(14, 0);
      macro();
    }
    /*if (number == 6) {
      Serial.println("B");
      analogWrite(A0, 255);
      delay(3000);
      analogWrite(A0, 0);
      }*/
  }
}



