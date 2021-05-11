#include <Arduino.h>
#include <M5EPD.h>
#include <SPI.h>
#include <SD.h>
#include "pantalla.hpp"
#include "jsonCom.hpp"


#include "cal_interfaces.h"

extern char     szLocalizacion[25];
extern int wifi_status ;
extern long unsigned int iLastUpdate;

M5EPD_Canvas canvas(&M5.EPD);


extern pantalla scrPrincipal;
extern TaskHandle_t thEventHandler;
extern TaskHandle_t thTouchScreenHandler;
extern QueueHandle_t qTouchScreenQueue;



void setup_m5() {
  M5.begin(true,true,true,true,true);
  M5.EPD.SetRotation(90);
  M5.TP.SetRotation(90);
  M5.EPD.Clear(true);
  M5.RTC.begin();
  canvas.createCanvas(540, 960);
  

  SPI.begin(14, 13, 12, 4);
  SD.begin(4, SPI, 20000000);
  cargaEstado();
  print_wakeup_reason();

  InitWifi();
  Sincroniza_download(szLocalizacion);
  loadJson(szLocalizacion);
  CloseWifi();

  //Creamos las colas de trabajo
  qTouchScreenQueue = xQueueCreate( 10, sizeof( tp_finger_t ) );
  // arrancamos las tareas
  xTaskCreatePinnedToCore(tskEventHandler, "EventHandler",5000,NULL,1,&thEventHandler,tskNO_AFFINITY );
  xTaskCreatePinnedToCore(tskTouchScreen, "EventHandler",5000,NULL,1,&thTouchScreenHandler,tskNO_AFFINITY );

  
  scrPrincipal.set_canvas(&canvas);
  scrPrincipal.dibuja_fondo();
  scrPrincipal.dibuja_top();
  scrPrincipal.dibuja_cuerpo();
  
  scrPrincipal.dibuja_menu();
  scrPrincipal.dibuja_flush();

  writeBatt2SD();
  iLastUpdate=millis();
}




