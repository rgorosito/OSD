
#ifndef _DECO_USER_H
#define _DECO_USER_H

#include "GoroTelem.h"

#define USER_SYNC GOROT_PING

uint8_t user_datosEsperados=0;
uint8_t user_datosRecibidos=0;
int user_datoAnterior=0xF0;
boolean user_esperandoSync=true;
boolean user_escapeado=true;
int user_comandoActual;

binary_telem_data_t user_data;

/*

        private ArrayList<Integer> datos;
        private UserCommands comandos = new UserCommands();
        private GenericData datoGenerico;
*/

#define USER_ESPERANDO_INICIO user_datosRecibidos==user_datosEsperados



void user_reseteaDatos() {
	memset(&user_data,0,sizeof(binary_telem_data_t));	// es necesario????
	user_datosRecibidos=0;
        user_datosEsperados=0;
}

boolean user_checkRedundancia() {

	// si no hay suficientes datos, esta OK
	if ( user_datosEsperados < 2 ) {
		return true;
	} 

	uint8_t	redundancia=0;

        for ( uint8_t i=0 ; i<user_datosEsperados-1  ; i ++) {
	        redundancia ^= user_data.dato[i];
        }

        if ( redundancia == 0x7E )
           redundancia = 0x5E;
        else if ( redundancia == 0x7D )
            redundancia = 0x5D;
        else if ( redundancia >> 4 == 0xF )
            redundancia &= 0x0F;
            
        if ( redundancia == user_data.dato[user_datosEsperados-1] ) {
            return true;
        } else {
            Serial.print(millis(),DEC);
            Serial.println(":ERROR DE REDUNDANCIA.");
            return false;
        }

}

boolean user_interpretar(int dato) {

	boolean actualizamosTelemetria = false;

	// para engancharse, busco un PING
	if ( user_esperandoSync ) {
		if (user_datoAnterior!= 0xF0 && dato==USER_SYNC  ) { // FIXME: no le encuentro sentido a comparar el dato anterior
			user_datosEsperados=0;
		        user_esperandoSync=false;
			user_reseteaDatos();
		}
	} else if ( USER_ESPERANDO_INICIO ) {
               if ( dato > 0xF0) {
			// es un dato, tal como lo esperabamos
			user_reseteaDatos();
			user_comandoActual=dato;
			switch ( user_comandoActual ) {
				case GOROT_PING:
					user_reseteaDatos();
					break;
				case GOROT_ATTITUDE:
					user_datosEsperados=sizeof(gorot_attitude_t);
					break;
				case GOROT_POSITION:
					user_datosEsperados=sizeof(gorot_position_t);
					break;
				case GOROT_NAVIGATION:
					user_datosEsperados=sizeof(gorot_navigation_t);
					break;
				case GOROT_HOME:
					user_datosEsperados=sizeof(gorot_home_t);
					break;
				case GOROT_ALTITUDE:
					user_datosEsperados=sizeof(gorot_altitude_t);
					break;
				case GOROT_FLAGS:
					user_datosEsperados=sizeof(gorot_flags_t);
					break;
				default: // dato no conocido
					user_esperandoSync=true;
					break;
			}
		} else {
			// se esperaba un tipo de dato
			user_esperandoSync=true;
		}
	} else {
		if ( dato > 0xF0) {
			// no se espera un comando como dato
			user_esperandoSync=true;
			return false;
		} else if ( dato == 0xF0 ) {
			// escape
			user_escapeado=true;
			return false;
		} else if ( user_escapeado ) {
			dato = dato | 0xF0;
			user_escapeado = false;
		}
		user_data.dato[user_datosRecibidos]=dato;
		user_datosRecibidos++;
		// ya recibí todos los datos?
		if ( USER_ESPERANDO_INICIO ) {
			// parsea estructura en base a cata tipo de paquete y chequea redundancia -----------------------------------------------------------------
			// chequeo redundancia
			if ( ! user_checkRedundancia() ) {
				user_esperandoSync=true;
				return false;
			}
			actualizamosTelemetria = true;
                        //Serial.println(millis(),DEC);
			switch ( user_comandoActual ) {
				case GOROT_PING:
					// nada que hacer (FIXME: quizás podriamos toglear el estado de un led)
					break;
				case GOROT_ATTITUDE:
					osd_info.roll  = user_data.attitude.roll;
					osd_info.pitch = user_data.attitude.pitch;
                                        displayArtificialHorizon();
					break;
				case GOROT_POSITION:
					osd_info.lat = user_data.position.lat;
					osd_info.lon = user_data.position.lon;
					osd_info.numsats = user_data.position.numsats;
                                        displayGPS();
					break;
				case GOROT_NAVIGATION:
					osd_info.speed = user_data.navigation.speed;
					osd_info.course = user_data.navigation.course;
					osd_info.magheading = user_data.navigation.magheading;
                                        displayHeading();
					break;
				case GOROT_HOME:
					osd_info.hlat = user_data.home.lat;
					osd_info.hlon = user_data.home.lon;
					// #############################################
                                        break;
				case GOROT_ALTITUDE:
					osd_info.readedAltitude = user_data.altitude.readedAltitude;
					osd_info.desiredAltitudeToKeep = user_data.altitude.desiredAltitudeToKeep;
                                        displayAltitude();
					break;
				case GOROT_FLAGS:
					osd_info.motorsArmed  = user_data.flags.flags & GOROT_FLAG_ARMED;
					osd_info.altHoldState = user_data.flags.flags & GOROT_FLAG_ALTHOLD;
					osd_info.attitude     = user_data.flags.flags & GOROT_FLAG_ATTITUDE;
                                        osd_info.batteryA     = user_data.flags.flags & GOROT_FLAG_BATTERYA;
                                        osd_info.batteryW     = user_data.flags.flags & GOROT_FLAG_BATTERYW;
					break;
				default: // dato no conocido
					actualizamosTelemetria = false;
					user_esperandoSync=true;
					break;
			}
			// tambien actualizamod actualizar telemetria

			user_reseteaDatos();
		}
	}
	user_datoAnterior=dato;
	return actualizamosTelemetria;
               

}



#endif
