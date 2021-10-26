#include <M5EPD.h>
#include "producto.hpp"
#include "cal_interfaces.h"

#define uS_TO_S_FACTOR 1000000ull
//#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  3600        /* Time ESP32 will go to sleep (in seconds) */

extern producto vProductos[30];
extern int      vProductoSize;
extern int      iPrimerElemento;
extern char     szLocalizacion[25];
extern char     *szgLocalizaciones[];
extern boolean     iClockRestart;

void cargaEstado(){
  
  File fEstado = SD.open("/configuracion/estado.txt","r");
  String strTmp;
  
  Serial.println("cargamos el estado");
  strTmp=fEstado.readStringUntil('\n');
  Serial.printf("leemos el primer elemento %s\n",strTmp.c_str());
  if(strTmp!="")
    iPrimerElemento=atoi(strTmp.c_str());
  else  
    iPrimerElemento=0;

  strTmp=fEstado.readStringUntil('\n');
  Serial.printf("leemos la localizacion %s\n",strTmp.c_str());
  if(strTmp!="")
    strcpy(szLocalizacion,strTmp.c_str()); 
  else  
    strcpy(szLocalizacion,szgLocalizaciones[0]); 
  
  fEstado.close();
}
void guardaEstado(){
  Serial.println("Guardamos el estado");
  File fEstado = SD.open("/configuracion/estado.txt","w");
  fEstado.printf("%d\n",iPrimerElemento);
  fEstado.printf("%s\n",szLocalizacion);
  fEstado.close();
}

extern   rtc_time_t mRTCtime;
void apagamos(){
  
  uint64_t iTime2Sleep = TIME_TO_SLEEP;
  guardaEstado();
  writeBatt2SD();
if ((mRTCtime.hour>22) || (mRTCtime.hour<6) ){
    msg2BatteryLog("apagado largo");
    iTime2Sleep = 3600;
  }else{
    msg2BatteryLog("apagado pequeño");
    iTime2Sleep = 18000;
  }
  delay(200);
  M5.shutdown(iTime2Sleep);

  esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR*iTime2Sleep);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, LOW); // TOUCH_INT
  gpio_hold_en(GPIO_NUM_2); // M5EPD_MAIN_PWR_PIN
  gpio_deep_sleep_hold_en();
  esp_deep_sleep_start();
  

  Serial.printf("DeepSleep NO");
  msg2BatteryLog("NO hemos entrado en deep sleep");
}



void print_wakeup_reason(){
	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.printf("######################################################################   Wakeup caused %d \n",wakeup_reason);
  iClockRestart=false;
	switch(wakeup_reason)
	{
		case 1  : Serial.printf("Wakeup caused by external signal using RTC_IO\n"); break;
		case 2  : Serial.printf("Wakeup caused by external signal using RTC_CNTL\n"); break;
		case 3  : Serial.printf("Wakeup caused by timer\n"); break;
		case 4  : Serial.printf("Wakeup caused by RTC\n"); 
              //ESTA ES LA QUE SALE CON EL REINICIO POR EL RELOJ
              iClockRestart=true;
              break;
		case 5  : Serial.printf("Wakeup caused by ULP program\n"); break;
		default : Serial.printf("Wakeup was not caused by deep sleep\n"); break;
	}
}