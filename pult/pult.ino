#include <VirtualWire.h>//передатчик

void setup(void)
{
  Serial.begin(9600);
  vw_set_ptt_inverted(true); // Необходимо для DR3100
  vw_setup(2000); // Устанавливаем скорость передачи (бит/с)
}

void loop(void)
{
  int a=0;
  if(analogRead(A0)>1000){
  a=11;
  }
  if(analogRead(A0)<100){
  a=12;
  }
  
  if(analogRead(A1)>1000){
  a=21;
  }
  if(analogRead(A1)<100){
  a=22;
  }
  
  if(analogRead(A2)>900){
  a=31;
  }
  if(analogRead(A2)<100){
  a=32;
  }
  
  if(analogRead(A3)>900){
  a=41;
  }
  if(analogRead(A3)<100){
  a=42;
  }
  
  if(analogRead(A4)>900){
  a=51;
  }
  if(analogRead(A4)<100){
  a=52;
  }
  if(analogRead(A5)>1000){
  a=61;
  }
  if(analogRead(A5)<100){
  a=62;
  }
  if(a!=0){
    Serial.println(a);
   String strMsg="z";
  strMsg += a;
  strMsg += " ";
  char msg[255];
  strMsg.toCharArray(msg, 255);
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); 
  }
}
