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

#ifndef _AQ_OSD_MAX7456_BATTMONITOR_H_
#define _AQ_OSD_MAX7456_BATTMONITOR_H_

#include "GQ_OSD.h"

//////////////////////////////////////////////////////////////////////////////
/////////////////////////// Battery voltage Display //////////////////////////
//////////////////////////////////////////////////////////////////////////////

boolean descentWarningShown = false;

void displayVoltage() {

//  int currentValue = osd_info.batteryV;

  char buf[12];
  snprintf(buf,7,"%c%2d.%1dV",'\20', osd_info.batteryV/10,osd_info.batteryV%10);

  // Following blink only symbol on warning and all on alarm
  writeChars( buf,   1, osd_info.batteryW?1:0, VOLTAGE_ROW , VOLTAGE_COL );
  writeChars( buf+1, 5, osd_info.batteryA?1:0, VOLTAGE_ROW , VOLTAGE_COL + 1 );

/*
  // current sensor installed
  currentValue = osd_info.batteryI/10;

  if (abs(currentValue)>=100) { // > 10A only display whole amps
     //snprintf(buf,12,"%4dA%5ld\24  ", currentValue/10, batteryC/1000);
     snprintf(buf,12,"%4dA", currentValue/10 );
  }
    else {
    //snprintf(buf,12,"%c%1d.%1dA%5ld\24  ", currentValue<0?'-':' ',abs(currentValue/10),abs(currentValue%10),batteryC/1000);
//    snprintf(buf,12,"%c%1d.%1dA", currentValue<0?'-':' ',abs(currentValue/10),abs(currentValue%10));
        snprintf(buf,12,"%c%3dA", currentValue<0?'-':' ',currentValue);
  }

  writeChars( buf, 11, 0, VOLTAGE_ROW, VOLTAGE_COL+6 );
*/
  #if defined (BattMonitorAutoDescent)
    if (osd_info.batteryA&& osd_info.motorsArmed ) {
      if (!descentWarningShown) {
        notifyOSD(OSD_CENTER|OSD_CRIT|OSD_BLINK, "BAT. CRITICAL - DESCENTING");
        descentWarningShown = true;
      }
    }
    else {
      descentWarningShown = false;
    }
  #endif
}

#endif  // #define _AQ_OSD_MAX7456_BATTMONITOR_H_
