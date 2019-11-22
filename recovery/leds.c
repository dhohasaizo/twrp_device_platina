#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <utils/Log.h>
#include <stdlib.h>
#include <string.h> 

int check_usb_connection() {

   char cap[4];

   FILE *fptr;

   fptr = fopen("/sys/class/power_supply/battery/status", "rt");

	if (fptr) {
		fgets(cap, 2, fptr);		
   		fclose(fptr);
			if (cap[0] == 'C') 
				return 1;
			else
				return 0;
		}
    return -1;
}

int battery_level() {

   int val;

   FILE *fptr;

   fptr = fopen("/sys/class/power_supply/battery/capacity", "rt");

	if (fptr) {
                fscanf(fptr, "%d", &val);
   		fclose(fptr);

		if (val >= 90) 
			return 1;
		else
			return 0;
		}
   return -1;
}

void set_leds(int white) {

   FILE *fptr;

   fptr = fopen("/sys/class/leds/white/brightness", "wr");

   if (fptr == NULL) {
        ALOGV("could not read /sys/class/leds/white/brightness:%s", strerror(errno));
   exit(1);
   }

   fprintf(fptr, "%d", white);
   fclose(fptr);

}

int main(void){

	while(1) {
		if (check_usb_connection()) {

			if (battery_level())
			      set_leds(0);
			else
			      set_leds(16);

		}
		else {
		set_leds(0);
		}
	}

	return 0;
}

