#include "address_map.h"
#include <unistd.h>
#include <sys/alt_alarm.h>
#include <stdio.h>
#include<stdlib.h>
/* The function below takes two valuements: value which is the value in decimal
 * to be displayed by the seven-segment displays and a character array that have the encoding
 * to display the provided value in decimal on the seven-segment displays. The provided value
 * can range from 0 - 999. The seven-segment displays must output the entire range correctly.
 */

int HEX3to0_display (int value, unsigned char * table);
int deposit_sum =00;
int NUM_T = 5;
alt_alarm alarm1;
alt_u32 alarm_handler(void * context);
void stop_alarm_succeed();
unsigned int start_alarm_succeed();
volatile int * pushbutton_ptr = (int *) PUSHBUTTON_BASE;
volatile int * redLED_base = (int *) RED_LED_BASE;
volatile int * green_led_base = (int *)	GREEN_LED_BASE;
volatile int * switch_base = (int *) SLIDER_SWITCH_BASE;
volatile int * hex3_hex0_base = (int *) HEX3_HEX0_BASE;
unsigned char table[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x7, 0xff, 0x6};


int main(void){
	
	alarmStop(&alarm1);
	alarmStart(&alarm1,5 * alt_ticks_per_second(),alarm_handler, NULL);
	
	int keyVal;
	int unitsLeft; //Units set by swtiches
	int swtchVal; //Value of the switch_base
	int Uprice; //Unit price of the vending machine
	int set = 0; //Boolean to notify if machine is set
	int loaded = 0; //Boolean to notify machine is loaded
	*hex3_hex0_base = 0x404040;



    while(1){
// swtchVal = *(switch_base);
// keyVal = *(pushbutton_ptr);
    
//Initialization
    	while(set==0){ //& swtchVal >= 512){
    	    swtchVal = *(switch_base);
            keyVal = *(pushbutton_ptr);
            if (swtchVal >= 512){
                if (keyVal == 1){
                    swtchVal-= 512;
                    *hex3_hex0_base = HEX3to0_display(swtchVal, table);
                    usleep(2500);
                    set = 1;
                }
            }
    	}
    		
//Preperation	
    	while(set==1 ){
    	    swtchVal = *(switch_base);
            keyVal = *(pushbutton_ptr);
            if (swtchVal >= 256 && swtchVal <= 511){ //511 Max Sw8: [011 111 111]
            *hex3_hex0_base = 0x404040;
    		    if (keyVal & 1){//Third button pressed.
    			    unitsLeft = swtchVal-256;
    			    loaded = 1; //Vending machine is loaded.
    			    *hex3_hex0_base = HEX3to0_display(unitsLeft, table);
    			    usleep(2000);
    		    }
            }
            
//Money Deposit	    			    
    		if(loaded == 1){
    		    if (swtchVal >= 1 && swtchVal < 256 ){ //Checks to see if SW0 is on
    		        Vend(deposit_sum, unitsLeft, Uprice); 
    		        }
    		        else{
        			    if (keyVal & 8){ //First Key
            				* hex3_hex0_base =HEX3to0_display (25, table);
            				deposit_sum += 25;
            				usleep(2000);
            				stop_alarm_succeed();
            				start_alarm_succeed();
            				}
            				else if(keyVal & 4){ //Second Key
            				* hex3_hex0_base =	HEX3to0_display (10, table);
            				deposit_sum += 10;
            				usleep(2000);
            				stop_alarm_succeed();
            				start_alarm_succeed();
            				}
            				else if(keyVal & 2){ //Third Key
            				* hex3_hex0_base =	HEX3to0_display (5, table);
            				deposit_sum += 5;
            				usleep(2000);
            				stop_alarm_succeed();
            				start_alarm_succeed();
            				}
            				else if(keyVal & 1){ //Fourth Key
            				* hex3_hex0_base =	HEX3to0_display (5, table);
            				deposit_sum +=0;
            				usleep(2000);
            				stop_alarm_succeed();
            				start_alarm_succeed();
            				}
            				
            				* hex3_hex0_base =HEX3to0_display (deposit_sum, table);
            				usleep(150000);
            				loaded=0;
            				set=0;
    		        }
    		}
	   }
    }
}

void Vend(int deposit, int units, int price){ //Function completes vending cycle
	if (swtchVal = 1 && set > 0){
		if (deposit < price){
			deposit = 0;
			* hex3_hex0_base =HEX3to0_display (units, table);
			*(redLED_base) = 000000001;
		}
		else{
			deposit_sum = deposit - price;
			if (deposit_sum < 0){
				deposit_sum = 0;
			}
		}
			
			unitsLeft = units - 1
			*(redLED_base) = 001000000;
	}
}

unsigned int start_alarm_succeed() {

return alarmStart(&alarm1, 5 * alt_ticks_per_second(),alarm_handler, NULL);


}

void stop_alarm_succeed() {
alarmStop(&alarm1);

}

alt_u32 alarm_handler(void * context){

	deposit_sum = 00;
	* hex3_hex0_base =HEX3to0_display (deposit_sum, table);
	return  5 * alt_ticks_per_second();
}

int HEX3to0_display (int value, unsigned char * table){
	int one;
	int ten;
	int hund;
    unsigned int a = 0x00;	
	unsigned int b = 0x00;
	unsigned int c = 0x00;
    unsigned int hex;


if(	value > 99  && value <1000){

	one = value % 10;
	value /= 10;
	ten = value % 10;
	value /= 10;
	hund = value % 10;

	c = table[one];
	b = table[ten];
	a = table[hund];

	hex = a << 16 | b << 8 | c;
	}else if(value > 9 && value < 100){

	one = value % 10;
	value /= 10;
	ten = value % 10;


	c = table[one];
	b = table[ten];

	hex = b << 8 | c;

	}else if( value < 10){
	one = value;
	c = table[one];;
	  hex = c;
	}
	return  hex;
}