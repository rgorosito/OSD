/*
  AeroQuad v3.0 - Nov 2011
 www.AeroQuad.com
 Copyright (c) 2011 Ted Carancho.  All rights reserved.
 An Open Source Arduino based multicopter.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _AQ_OSD_MAX7456_GPS_H_
#define _AQ_OSD_MAX7456_GPS_H_

//////////////////////////////////////////////////////////////////////////////
//////////////////////////// GPS Display /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Show GPS information

#include "GpsAdapterRecortado.h"

byte osdGPSState=0;
#define GPS_DONAV 0x80 // display navigation info next time
#define GPS_NOFIX 0x40 // no fix displayed
#define GPS_NONAV 0x20 // nav info hidden (no fix or no target)

void displayGPS() {

  if (osdGPSState & GPS_DONAV) {
    if ((osd_info.hlat==GPS_INVALID_ANGLE) || (osd_info.lat==GPS_INVALID_ANGLE)) {
      if (!(osdGPSState&GPS_NONAV)) {
        // clear info
        writeChars(NULL, 2, 0, GPS_HA_ROW, GPS_HA_COL);
        writeChars(NULL, 4, 0, GPS_HA_ROW + 1, GPS_HA_COL - 1);
        writeChars(NULL, 4, 0, GPS_HA_ROW + 2, GPS_HA_COL - 1);
        osdGPSState|=GPS_NONAV;
      }
    }
    else {
      char buf[5];
      computeDistanceAndBearing(osd_info.lat, osd_info.lon, osd_info.hlat, osd_info.hlon);
      #ifdef USUnits
        const unsigned int distance = getDistanceFoot(); // dist to home in feet
	    if (distance<1000) {
          snprintf(buf,5,"%3df",(int)distance);
        }
        else if (distance<5280) {
          snprintf(buf,5,".%02dm", (int)(distance * 10 / 528));
        }
        else {
          snprintf(buf,5,"%d.%1dm", (int)(distance/5280), (int)(distance / 528 % 10));
        }
      #else //metric
        const unsigned int distance = getDistanceMeter(); // dist to home in meters
	    if (distance<1000) {
          snprintf(buf,5,"%3dm",(int)distance);
        }
        else {
          snprintf(buf,5,"%d.%1d\032", (int)(distance/1000), (int)(distance / 100 % 10));
        }
      #endif
      writeChars(buf, 4, 0, GPS_HA_ROW + 1, GPS_HA_COL - 1);

      short homearrow = gpsBearing - osd_info.magheading; // direction of home vs. craft orientation

      homearrow = ((homearrow + 11) * 16 / 360 + 16) % 16; // map to the 16 direction arrow
      buf[0]=176 + homearrow * 2;
      buf[1]=buf[0]+1;
      writeChars(buf, 2, 0, GPS_HA_ROW, GPS_HA_COL);
    
      //  calculate course correction 
      short courseCorrection = (gpsBearing - osd_info.course/100);
      // normalize to -180 - 180 
      if (courseCorrection>180) courseCorrection-=360;
      if (courseCorrection<-180) courseCorrection+=360;
      
      snprintf(buf,5,"%c%d",courseCorrection>0?'R':'L', abs(courseCorrection));
      writeChars(buf, 4, 0, GPS_HA_ROW + 2, GPS_HA_COL - 1);
    
      osdGPSState&=~GPS_NONAV;
    }
  }
  else {
    // update position and speed
    if (osd_info.lat == GPS_INVALID_ANGLE) {
      if (!(osdGPSState&GPS_NOFIX)) {
        char buf[29];
        snprintf(buf,29,"Waiting for GPS fix (%d/%d)",osd_info.numsats,6);
        writeChars(buf, 28, 0, GPS_ROW, GPS_COL);
        osdGPSState|=GPS_NOFIX;
      }
    } else {
      char buf[29];
#ifdef USUnits
      snprintf(buf,29,"%d:%c%02ld.%06ld%c%03ld.%06ld %3ld\031",osd_info.numsats,
               (lat>=0)?'N':'S',labs(lat)/10000000L,labs(lat)%10000000L/10,
               (lon>=0)?'E':'W',labs(lon)/10000000L,labs(lon)%10000000L/10,
	       osd_info.speed*32/1609);
#else
      snprintf(buf,29,"%d:%c%02ld.%06ld%c%03ld.%06ld %3ld\030",osd_info.numsats,
               (osd_info.lat>=0)?'N':'S',labs(osd_info.lat)/10000000L,labs(osd_info.lat)%10000000L/10,
               (osd_info.lon>=0)?'E':'W',labs(osd_info.lon)/10000000L,labs(osd_info.lon)%10000000L/10,
	       osd_info.speed*36/1000);
#endif
      writeChars(buf, 28, 0, GPS_ROW, GPS_COL);
      osdGPSState&=~GPS_NOFIX;
    }
  }
  osdGPSState^=GPS_DONAV;
}

#endif  // #define _AQ_OSD_MAX7456_GPS_H_
