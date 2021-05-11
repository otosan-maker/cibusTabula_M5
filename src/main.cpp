#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "cal_interfaces_m5.h"
#include "pantalla.hpp"
#include "jsonCom.hpp"
#include "cal_interfaces.h"

#define __M5PAPER__
//#define __LILYGO5__

extern char     szLocalizacion[25];
extern int wifi_status ;
long unsigned int iLastUpdate=0;


pantalla scrPrincipal;
TaskHandle_t thEventHandler;
TaskHandle_t thTouchScreenHandler;
QueueHandle_t qTouchScreenQueue;


void setup() {
#ifdef __M5PAPER__
  setup_m5();
#endif
#ifdef __LILYGO5__
  setup_lilygo();
#endif


}






void loop() {
  
/* 
  Sincroniza_download(szLocalizacion);
  loadJson(szLocalizacion);
  scrPrincipal.dibuja_cuerpo();
 */
  scrPrincipal.dibuja_top();
  scrPrincipal.dibuja_flush();

  UBaseType_t memoria_usada=uxTaskGetStackHighWaterMark(thEventHandler);
  Serial.printf("memoria usada por thEventHandler %d\n",memoria_usada );
  memoria_usada=uxTaskGetStackHighWaterMark(thTouchScreenHandler);
  Serial.printf("memoria usada por thTouchScreenHandler %d\n",memoria_usada );
  
  
  Serial.printf("last update %d\n",(millis()-iLastUpdate));

   delay(10000);
  if (millis()-iLastUpdate>20000){
    scrPrincipal.dibuja_apagado();
    apagamos();
  } 
      

}