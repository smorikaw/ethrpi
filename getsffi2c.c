////////////////////////////////////////////////
//
// Raeberry Pi I2C SFP EEPROM decode bt ethtool lib
//      by morikawa@wavesplitter.com 2024/02/11
//
// cc getsffi2c.c -o getsffi2c qsfp.c sfpdiag.c cmis.c sff-common.c -lm -lwiringPi -lwiringPiDev
///////////////////////////////////////////////
#include	<stdio.h>
#include	<string.h>
#include	<stdint.h>
#include	<time.h>
#include	<wiringPi.h>
#include	<wiringPiI2C.h>

#include	"internal.h"

int fd;

//
//  main MAIN main MAIN
//

int main(){
	int i;
	char EEPROM[256];
	char PN[16+1];

	fprintf(stderr, "EEPROM dump by ethtool 6.7\n");

//	initialize rasberry PI I2C interface
	wiringPiSetupGpio();
	pinMode(4, INPUT);

	fd = wiringPiI2CSetup(0x50);	// check A0h page 00 get BASIC info
	wiringPiI2CWriteReg8(fd, 127, 0x00);	// page select 00h
//
	for(i=0; i < 255; i++){
		delay(2);
		EEPROM[i] = wiringPiI2CReadReg8(fd,i);
	}

	fprintf(stderr, "ID = %02X\n", EEPROM[0]);

	switch(EEPROM[0]){
	case 0x0d:	// QSFP+
	case 0x11:	// QSFP28
	case 0x18:	// QSFP-DD
	case 0x19:	// OSFP
		sff8636_show_all_ioctl(EEPROM, 256);
		break;
	case 0x03:	// SFP/SFP+/SFP28
		sff8079_show_all_ioctl(EEPROM);
		sff8472_show_all(EEPROM);	
		break;
	}	
}
