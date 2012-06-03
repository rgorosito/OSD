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

#ifndef _AQ_OSD_MAX7456_RSSI_H_
#define _AQ_OSD_MAX7456_RSSI_H_

#define RSSI_RAWVAL

//////////////////////////////////////////////////////////////////////////////
/////////////////////////// RSSI Display /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

short lastRSSI1 = 1234; //forces update at first run
short lastRSSI2 = 1234; //forces update at first run

void displayRSSI() {

  short rssi2 = osd_info.rssi2 - osd_info.rssi1;

  if ( osd_info.rssi1 != lastRSSI1) {
    lastRSSI1 = osd_info.rssi1;
    char buf[6];
    #ifdef RSSI_RAWVAL
      snprintf(buf, 6, "\255%3u", osd_info.rssi1);
      writeChars(buf, 5, 0, RSSI_ROW, RSSI_COL);
    #else
      snprintf(buf, 6, "\372%3u%%", osd_info.rssi1);
      writeChars(buf, 5, (RSSI_WARN>osd_info.rssi1)?1:0, RSSI_ROW, RSSI_COL);
    #endif
  }
  
  if ( rssi2 != lastRSSI2) {
    lastRSSI2 = rssi2;
    char buf[6];
    #ifdef RSSI_RAWVAL
      snprintf(buf, 6, "\255%3u", rssi2);
      writeChars(buf, 5, 0, RSSI_ROW+1, RSSI_COL);
    #else
      snprintf(buf, 6, "\372%3u%%", rssi2);
      writeChars(buf, 5, (RSSI_WARN>rssi2)?1:0, RSSI_ROW+1, RSSI_COL);
    #endif
  }
}

#endif  // #define _AQ_OSD_MAX7456_RSSI_H_
