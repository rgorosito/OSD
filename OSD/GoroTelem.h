
/*
 * Aca van a ir la lista de comandos que le envia el mega al 328
 *
 */

#ifndef _GOROTELEM_H
#define _GOROTELEM_H

#define GOROT_CLIENT Serial2
#define GOROT_SPEED 9600

#define GOROT_MAX_LONG_DATO 64

#define GOROT_ESCAPE	   0xF0
#define GOROT_PING	   0xF1
#define GOROT_ATTITUDE	   0xF2
#define GOROT_POSITION	   0xF3
#define GOROT_NAVIGATION   0xF4
#define GOROT_HOME         0xF5
#define GOROT_ALTITUDE     0xF6
#define GOROT_FLAGS        0xF7

#define GOROT_FLAG_ARMED      1
#define GOROT_FLAG_ATTITUDE   2
#define GOROT_FLAG_GPSOK      4
#define GOROT_FLAG_ALTHOLD    8
#define GOROT_FLAG_POSHOLD   16
#define GOROT_FLAG_BATTERYW  32
#define GOROT_FLAG_BATTERYA  64

struct gorot_attitude_t {	//9+1 * 4Hz = 40bytes/seg
	float roll;		//4
	float pitch;		//4
	uint8_t crc;		//1
};

struct gorot_position_t {	//10+1 * 2Hz = 22bytes/seg
        int32_t lat;		//4
        int32_t lon;		//4
        uint8_t numsats;	//1
	uint8_t crt;		//1
};

struct gorot_navigation_t {	//11+1 * 2Hz = 24bytes/seg
        int32_t speed;		//4
        int32_t course;		//4
        int16_t magheading;	//2
	uint8_t crt;		//1
};

struct gorot_home_t {		//9+1 * 1Hz = 10bytes/seg
        int32_t lat;		//4
        int32_t lon;		//4
	uint8_t crt;		//1
};

struct gorot_altitude_t {		//9+1 * 1Hz = 10bytes/seg
        float readedAltitude;		//4
        float desiredAltitudeToKeep;	//4
	uint8_t crt;			//1
};

struct gorot_flags_t {	//3+1 * 1Hz = 3bytes/seg
	uint16_t flags;	//2
	uint8_t	 crt;	//1	
};


// 

union binary_telem_data_t {
	uint8_t dato[GOROT_MAX_LONG_DATO];
	gorot_attitude_t   attitude;
	gorot_position_t   position;
	gorot_navigation_t navigation;
	gorot_home_t	   home;
	gorot_altitude_t   altitude;
	gorot_flags_t      flags;
  
};

#endif
