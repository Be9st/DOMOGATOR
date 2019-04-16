// Тестировалось на Arduino IDE 1.0.1
#include <VirtualWire.h>
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
void setup()
{
  Serial.begin(9600);
  vw_set_ptt_inverted(true); // Необходимо для DR3100
  vw_set_rx_pin(8);//pin
  vw_setup(2000); // Задаем скорость приема
  vw_rx_start(); // Начинаем мониторинг эфира
}

void loop()
{
  int c=com();
  if(c!=0)
  Serial.println(c);
}
