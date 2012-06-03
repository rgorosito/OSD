

#include "Arduino.h"
#include "GQ_OSD.h"
#include "Device_SPI.h"
#include "UserConfiguration.h"
#include "OSDDisplayController.h"
#include "MAX7456.h"
#include "DECO_frsky.h"

#include <SoftwareSerial.h>


uint32_t lastRefresh=0;
boolean hayNovedades=false;

SoftwareSerial mySerial(2, 3);



void setup() {


	// inicializa SPI
	initializeSPI();

	// inicializa OSD
    	initializeOSD();

	// inicializa datos para el osd
	memset(&osd_info,0,sizeof(osd_info));

	// inicializa serie de telemetria
	Serial.begin(9600);
	mySerial.begin(9600);
	

}


void loop() {

	int c = mySerial.read();	// FIXME: otra opcion es leer los disponibles y parsear de una sola vez todos los disponibles haciendo OR en el hayNovedades

	if ( c != -1 ) {
		// parseamos los datos. hay novedades?
		hayNovedades = deco_frsky ( c );	// FIXME: hacemos algo distinto si actualizó algo?
//		if ( hayNovedades ) {
//			Serial.print("datos!");
//			Serial.println(millis(),DEC);
//		}
	}

	osd_info.currentTime=millis();
	if ( osd_info.currentTime - lastRefresh > 100 ) {
        	updateOSD();
		lastRefresh=osd_info.currentTime;
	}
}

// 10hz
//      #ifdef OSD_SYSTEM_MENU
//        updateOSDMenu();
//      #endif
//
//      #ifdef MAX7456_OSD
//        updateOSD();
//      #endif

