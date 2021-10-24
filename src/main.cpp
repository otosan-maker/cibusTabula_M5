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
boolean     iClockRestart;

pantalla scrPrincipal;
TaskHandle_t thEventHandler;
TaskHandle_t thTouchScreenHandler;
QueueHandle_t qTouchScreenQueue;


void setup() {
  setup_m5();
}






void loop() {

//Si salimos del bloqueo porque nos lo dice el reloj, nos volvemos a bloquear.
if (iClockRestart){
  Serial.printf("Reinicio tras un evento reloj\n" );
  apagamos();
}
/* 
  Sincroniza_download(szLocalizacion);
  loadJson(szLocalizacion);
  scrPrincipal.dibuja_cuerpo();
 */
  scrPrincipal.dibuja_top();
  scrPrincipal.dibuja_flush();

  // UBaseType_t memoria_usada=uxTaskGetStackHighWaterMark(thEventHandler);
  // Serial.printf("memoria libre por thEventHandler %d\n",memoria_usada );
  // memoria_usada=uxTaskGetStackHighWaterMark(thTouchScreenHandler);
  // Serial.printf("memoria libre por thTouchScreenHandler %d\n",memoria_usada );
  
  
  Serial.printf("last update %d\n",(millis()-iLastUpdate));

   delay(10000);
  if (millis()-iLastUpdate>40000){
    scrPrincipal.dibuja_apagado();
    apagamos();
  } 
      

}