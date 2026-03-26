#include "clients.h"

const char row1[] PROGMEM = "Hederlige Harrys Bilar,Köp bil hos Harry,En god bilaffär(för Harry!),Hederlige Harrys Bilar,5000,3,1,1,0,2,0";
const char row2[] PROGMEM = "Farmor Ankas Pajer AB,Köp paj hos Farmor Anka,Fort innan Mårten ätit all paj,,3000,2,2,1,0,0";
const char row3[] PROGMEM = "Svarte Petters Svartbyggen,Låt Petter bygga åt dig,Bygga svart? Ring Petter,,1500,2,3,1,0,1";
const char row4[] PROGMEM = "Långbens detektivbyrå,Mysterier? Ring Långben,Långben fixar biffen,,4000,2,4,0,0,0";
const char row5[] PROGMEM = "IoT:s Reklambyrå,Synas här? IoT:s Reklambyrå,,,1000,1,5,0,0";

const char* const list[] PROGMEM = { row1, row2, row3, row4, row5};
const uint8_t rows_count = sizeof(list)/sizeof(list[0]);