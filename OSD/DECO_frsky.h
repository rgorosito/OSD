
#ifndef _DECO_FRSKY_H
#define _DECO_FRSKY_H

#include "DECO_user.h"

#define FRSKY_START_STOP 0x7E
#define FRSKY_REMOTE_VOLTAGE_AND_LINK_QUALITY 0xFE
#define FRSKY_USER_DATA 0xFD
#define FRSKY_INVALID -1

#define VSENSOR_MAXV 3.3
#define VSENSOR_STEP 0.06369
#define VSENSOR_MAXN 255.0
#define VSENSOR_SCALE VSENSOR_MAXV*10.0/(VSENSOR_STEP*VSENSOR_MAXN)

uint8_t numdato=0;
uint8_t numEfectivoDato=0;
boolean esperoInicio=true;
int anterior=0;
boolean escapeado=false;
int tipoPaquete;
uint8_t numUsuario=0;
uint8_t numUsuarioActual=0;

boolean deco_frsky(int dato) {

	numdato++;

        if ( esperoInicio ) {
                if ( dato == 0x7E && anterior == 0x7E ) {
                        esperoInicio=false;
                        anterior=dato;
                        numdato=1;
                        numEfectivoDato=1;
                        return false;
                } else {
                        anterior=dato;
                        return false;
                }
        } else if ( escapeado ) {
                anterior=dato;
                escapeado=false;
                dato=dato ^ 0x20;
        } else if ( dato == 0x7D ) {
                escapeado = true;
                anterior=dato;
                return false;
        } else {
                anterior=dato;
        }

        numEfectivoDato++;

        if ( numdato == 2 ) {

                tipoPaquete=dato;

                switch(tipoPaquete) {

                case FRSKY_REMOTE_VOLTAGE_AND_LINK_QUALITY:
                        return false;

                case FRSKY_USER_DATA:
                        return false;

                default:
                        tipoPaquete=FRSKY_INVALID;
                        esperoInicio=true;
                        return false;
                }

        }

	boolean actualizamosTelemtria=false;

        switch ( tipoPaquete ) {
                case FRSKY_USER_DATA:
                        if ( numdato == 3 ) {
                                numUsuario=dato;
                                numUsuarioActual=0;
                        } else if ( numdato == 4 ) {
				// nada
                        } else if ( numdato == 11 ) {
                                if ( dato == FRSKY_START_STOP ) {
					// fin paquete
                                } else {
					// no es el fin esperado
                                }
                                esperoInicio=true;
                                numdato=0;
                        } else {
                                numUsuarioActual++;
                                if ( numUsuarioActual > numUsuario ) {
					// fruta
                                } else {
                                  //Serial.println(dato,DEC);
                                  actualizamosTelemtria = user_interpretar(dato);
                                }
                        }
                        break;
		
                case FRSKY_REMOTE_VOLTAGE_AND_LINK_QUALITY:
                        switch ( numEfectivoDato ) {
                                case 3:	// A1
                                        actualizamosTelemtria=true;
                                        osd_info.batteryI = dato; //* VSENSOR_SCALE; // FIXME: escalar
                                        break;
                                case 4: // A2
                                        actualizamosTelemtria=true;
                                        osd_info.batteryV = dato * VSENSOR_SCALE; // FIXME: escalar
                                        break;
                                case 5: // LINK1
                                        actualizamosTelemtria=true;
                                        osd_info.rssi1 = dato;
                                        break;
                                case 6: // LINK2
                                        actualizamosTelemtria=true;
                                        osd_info.rssi2 = dato;
                                        break;
                                case 11:
                                        if ( dato == FRSKY_START_STOP ) {
						// fin paquete
                                        } else {
						// no es el fin esperado
                                        }
                                        esperoInicio=true;
                                        numdato=0;
                                        break;
                                default:
					// blanco
                                        break;
                        }
                default:
                       break;
        }

        return actualizamosTelemtria;

}


#endif
