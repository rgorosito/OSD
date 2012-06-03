

#ifndef _GQ_OSD_H
#define _GQ_OSD_H

#define CHAR_ALERT \253
#define CHAR_RADAR \254
#define CHAR_GPS1 \255
#define CHAR_GPS2 \256


struct osd_info_t {

	// generico
	uint32_t currentTime; // unsigned long

	// battery & timer
	boolean motorsArmed;

	// bateria
	uint16_t batteryV;	// unsigned short
	int16_t batteryI;	// short
	//long batteryC = 0;	// USED CAPACITY
	uint8_t batteryW;	// WARNING
	uint8_t batteryA;	// ALARM

	// heading	
	int16_t heading;

	// altitude
	float readedAltitude;
	float desiredAltitudeToKeep;
	boolean altHoldState;

	// gps
	int32_t lat;
	int32_t lon;
	int32_t hlat;
	int32_t hlon;
	int32_t speed;
	int32_t course;
	int16_t magheading;
	uint16_t numsats;

	// rssi
	uint8_t	rssi1;
	uint8_t	rssi2;

	// attitude /AH)
	float roll;
	float pitch;
	boolean attitude;

} osd_info ;

#endif


