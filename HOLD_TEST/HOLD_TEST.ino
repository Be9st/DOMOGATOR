//#include <iarduino_IR_RX.h>
#include <VirtualWire.h>
#include <Wire.h>
#include <SPI.h>
//#include <Adafruit_PN532.h>
#include <Servo.h>
#include <SoftwareSerial.h>

int number;

SoftwareSerial softSerial(12, 13); // RX, TX> передача данных

Servo serv1, serv2, serv;
int servi1 = 90, servi2 = 90, servi = 0; //положение сервоприводов
int chs = 0, sn = -1;

//Adafruit_PN532 nfc(1, 100);
int tnfc = 1;

//iarduino_IR_RX IR(18);                            //ИК-датчик для пульта (4 pin)


uint8_t uidFirstCard[] = {0x53, 0x9B, 0xFA, 0x2E};// карты nfc
uint8_t uidSecondCard[] = {0xF2, 0x6F, 0x02, 0x2E};
uint8_t uidThirdCard[] = {0x92, 0xB2, 0xE9, 0x21};

int il, last = 0;

int pos2 = 90, pos1 = 90;


void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);

  vw_set_ptt_inverted(true); // Необходимо для DR3100
  vw_set_rx_pin(8);//pin
  vw_setup(2000); // Задаем скорость приема
  vw_rx_start(); // Начинаем мониторинг эфира
  
//  IR.begin();
  //nfc.begin();//сравнение nfc
  //nfc.SAMConfig();

  pinMode(A4, OUTPUT);//1
  pinMode(A5, OUTPUT);
  analogWrite(A4, 255);
  analogWrite(A5, 255);

  pinMode(11, OUTPUT);//1
  pinMode(10, OUTPUT);

  pinMode(9, OUTPUT);//2
  pinMode(8, OUTPUT);

  pinMode(7, OUTPUT);//3
  pinMode(6, OUTPUT);

  pinMode(5, OUTPUT);//4
  pinMode(4, OUTPUT);

  digitalWrite(11, LOW);
  digitalWrite(9, LOW);
  digitalWrite(7, LOW);
  digitalWrite(5, LOW);


  serv1.attach(2);
  serv1.write(90);
  serv2.attach(3);
  serv2.write(90);
  serv.attach(A5);
  serv.write(0);




}

void loop()
{
  if (softSerial.available()) {//trade macro
    int x = softSerial.read();
    switch (x) {
      case 10:
        macro();
    }
  }

  /*nfc
    Serial.println("Nfc");
    switch (nfc_check())
    {
    case 1://fire

      break;

    case 2://
      Serial.println("r");
      break;

    case 3:
      Serial.println("u");
      break;
    }*/

  int pult = com();
  if (pult!=0)
  {
    Serial.println(pult);
    switch (pult)
    {
      case 21://big up down (VAZHIY!!!)
        digitalWrite(10, HIGH);//10,11
        if (last != 11)il = 0;
        last = 11;
        il = 100;
        break;
      case 22:
        digitalWrite(10, LOW);
        if (last != 11)il = 0;
        last = 11;
        il = 100;
        break;

      case 11:// osnova left right (VAZHNIY!!!)
        digitalWrite(8, HIGH);
        if (last != 9)il = 0;
        last = 9;
        il = 250;

        break;
      case 12:
        digitalWrite(8, LOW);
        if (last != 9)il = 0;
        last = 9;
        il = 250;
        break;

      case 31://left right kogot' (bespolezno)
        digitalWrite(6, HIGH);
        if (last != 7)il = 0;
        last = 7;
        il = 250;
        break;
      case 32:
        digitalWrite(6, LOW);
        if (last != 7)il = 0;
        last = 7;
        il = 250;
        break;

      case 41://sredni (VAZHNIY!!!)
        digitalWrite(4, HIGH);
        if (last != 5)il = 0;
        last = 5;
        il= 250;
        break;
      case 42:
        digitalWrite(4, LOW);
        if (last != 5)il = 0;
        last = 5;
        il = 250;
        break;

      case 0x168930CF://serv 1 visota kogot'
        Serial.println("-----");
        if (servi1 < 180) {
          servi1 = servi1 + 5;
          serv1.write(servi1);
        }
        break;
      case 0x1689708F://расжать
        Serial.println("-00000-");
        if (servi1 > 0) {
          servi1 = servi1 - 5;
          serv1.write(servi1);
        }
        break;

      case 0x1689906F://serv 2 kogot' hvat
        if (servi2 < 180) {
          servi2 = servi2 + 5;
          serv2.write(servi2);
        }
        break;

      case 0x168940BF://серво вниз
        if (servi2 > 0) {
          servi2 = servi2 - 5;
          serv2.write(servi2);
        }
        break;

      case 0x168938C7:
        if (tnfc == 1) {
          tnfc = 5000;
          Serial.println(5000);
        }
        else tnfc = 1;
        break;

      case 0x1689A05F://macro button
        //softSerial.write(7);
        macro();
        delay(2000);
        break;
      //////////////////////////////////////////////2 plata
      case 51://engine вперед
        softSerial.write(1);
        break;
      case 52://engine назад
        softSerial.write(2);
        break;

      case 61://направо
        softSerial.write(3);
        break;
      case 62://налево
        softSerial.write(4);
        break;

      case 0xFF02FD://насос
        softSerial.write(5);
        break;

      case 0x1689B04F://цвет
        softSerial.write(6);
        break;
    }
  }
  if (il > 0) {
    onestep(600, last);
    il--;
  }

}

void macro() {

  serv1.write(90);
  serv2.write(0);

  back180();
  for(int i=0;i<3;i++){
  serv2.write(180);
  next360();

  delay(1000);
  serv2.write(0);


  ///////////////////////////////////////////////
  serv1.write(90);
  serv2.write(0);

  //back
  delay(2000);
  back360();
  delay(1000);
  leftright;
}
}
void leftright(){
    delay(2000);
    for (float i = 0; i < 100; i++) {
    digitalWrite(8, HIGH);//left right
    onestep(400, 9);
  }
}

void next360() {
  //upper
  delay(2000);
  for (float i = 0; i < 2600; i = i + 0.5) {
    digitalWrite(4, HIGH);//2600 sred
    onestep(400, 5);
    if (i <= 1400) {
      digitalWrite(10, HIGH);//big
      onestep(400, 11);
    }
  }
}

void back180() {
  for (int i = 0; i < 2600; i++) {
    digitalWrite(4, LOW);//2600 sred
    onestep(400, 5);
    if (i <= 1200) {
      digitalWrite(10, LOW);//big
      onestep(400, 11);
    }
  }
}

void back360() {
  for (float i = 0; i < 2600; i = i + 0.5) {
    digitalWrite(4, LOW);//2600 sred
    onestep(400, 5);
    if (i <= 1400) {
      digitalWrite(10, LOW);//big
      onestep(400, 11);
    }
  }
}

void macro2() {

}

void onestep(int pause, int eng) {              //шаг
  delayMicroseconds(pause);
  digitalWrite(eng, HIGH);
  digitalWrite(eng, LOW);
}

int com(){// прием по радио
   uint8_t buf[VW_MAX_MESSAGE_LEN]; // Буфер для сообщения
  uint8_t buflen = VW_MAX_MESSAGE_LEN; // Длина буфера

  if (vw_get_message(buf, &buflen)) // Если принято сообщение
  { int i;
  if (buf[0] != 'z')
    {
      return 0; 
    }
    i = 1; 
    int number = 0;
    // Поскольку передача идет посимвольно, то нужно преобразовать набор символов в число
    while (buf[i] != ' ')
    {
      number *= 10;
      number += buf[i] - '0';
      i++;
    }
    return(number);
  }
  return 0;
}

boolean comparisonOfUid(uint8_t uidRead[8], uint8_t uidComp[8], uint8_t uidLen) { //сравнение id
  for (uint8_t i = 0; i < uidLen; i++) {
    if (uidRead[i] != uidComp[i]) {
      return false;
    }
    if (i == (uidLen) - 0x01) {
      return true;
    }
  }
}

/*int nfc_check() {
  uint8_t success;
  // буфер для хранения ID карты
  uint8_t uid[8];
  // размер буфера карты
  uint8_t uidLength;
  // слушаем новые метки
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, tnfc);
  if(success){
  // если найдена карта
  if (comparisonOfUid(uid, uidFirstCard, uidLength)) {//сравнение карт
    return 1;
  } else {
    if (comparisonOfUid(uid, uidSecondCard, uidLength)) {
      return 2;
    } else {
      if (comparisonOfUid(uid, uidThirdCard, uidLength)) {
        return 3;
      }
      else return 0;
    }
  }
  }
  }*/
