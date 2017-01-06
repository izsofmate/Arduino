#include <LiquidCrystal.h>
//#include <DateTime.h>
//#include <DateTimeStrings.h>

#define dt_SHORT_DAY_STRINGS
#define dt_SHORT_MONTH_STRINGS

// simple sketch to display a digital clock on an LCD
// see the LiquidCrystal documentation for more info on this

LiquidCrystal lcd(1,0,5,4,3,2);
enum Edited{Hour,Minute,Second};	//mit változtassuk


const int led_activate=12;			//aktiválja a változtatás lehetőséget
int activate=0;						//változtatás állapota

const int led_second=10;			//second jelző led
const int led_minute=9;				//minute jelző led
const int led_hour=8;                            //hour jelző led

const int button_switch_hour_minute_second=13;	//ezzel váltjuk, hogy melyiket használjuk
int button_switch_hour_minute_second_state=0;

const int buttonPin_activate=11;		//váltás gomb aktiválója
int buttonPin_activate_state=0;			//alap állapota a váltó gombnak

const int buttonPin_up=6;				//felfele lépés gombja
int buttonPin_up_state=0;				//felfele lépés állapota

const int buttonPin_down=7;				//lefele lépés gombja
int buttonPin_down_state=0;				//lefele lépés állapota
	
class Time{
  
  private:
		byte hour;
		byte minute;
		byte second;
		static const int counter_pin=25;
		int counter_int;
		int hour_counter;
		Edited edited;					//példány a váltóhoz
        unsigned long prevMillis;
		
		
public:
		Time(){
            prevMillis=millis();
			atalakit();
		};
		
		void atalakit(){
                        char time[]=__TIME__;
			int a;
			for(int i=0;i<9;i++){
				switch (time[i]){
					case '0': a=0; break;
					case '1':	a=1; break;
					case '2': a=2; break;
					case '3': a=3; break;
					case '4': a=4; break;
					case '5': a=5; break;
					case '6': a=6; break;
					case '7': a=7; break;
					case '8': a=8; break;
					case '9':	a=9; break;
				}
		
				switch (i){
					case 0: hour+=a*10; break;
					case 1:	hour+=a*1;break;
					case 2:break;
					case 3:	minute+=a*10;break;
					case 4:	minute+=a*1;break;
					case 5:break;
					case 6:	second+=a*10;break;
					case 7:	second+=a*1;break;
				}
							
			}
		};
		
		void szamol(){
			if (millis()-prevMillis>=1000){
				prevMillis=millis();
				second+=1;
			}
                
			if (second>=60) {
				second=0;
				minute+=1;
			}
			if (minute>=60) {
				minute=0;
				hour+=1;
			}
			if (hour>=24){
				hour=0;
			}
                        kiir();
		};
		
		byte getHour(){
            return hour;
		};
		byte getMinute(){
			return minute;
		};
		byte getSecond(){
			return second;
        };
		void hour_up(){
                        if (hour==23) hour=0;
                        else hour++;
		}
		void hour_down(){
                        if (hour==0) hour=23;
                        else hour--;
		}
		void minute_up(){
			if (minute==59) minute=0;
                        else minute++;
		}
		void minute_down(){
			if (minute==0) minute=59;
                        else minute--;
		}
		void second_up(){
                        if (second>=50) second=second+10-60;
                        else second=second+10;
		}
		void second_down(){
                        if (second<=9) second=second-10+60;
                        else second=second-10;
		}
		void kiir_hour(){
			byte akthour=hour;
			if ((activate==0)||((activate==1)&&(edited==Hour)&&(counter_int<counter_pin))||((activate==1)&&(edited!=Hour))){
				for(int i=8;i>3;i--){
					lcd.setCursor(i,0);
					lcd.print(akthour%2);
					akthour/=2;
				}
				if (hour<10){
					lcd.setCursor(0,1);
					lcd.print('0');
					lcd.print(hour);
				}
				else {
					lcd.setCursor(0,1);
					lcd.print(hour);
				}

			}
			else {
				for(int i=8;i>3;i--){
					lcd.setCursor(i,0);
					lcd.print(' ');
				}	
				lcd.setCursor(0,1);
				lcd.print("  ");
			}
		};

		void kiir_minute(){
			byte aktminute=minute;
			if ((activate==0)||((activate==1)&&(edited==Minute)&&(counter_int<counter_pin))||((activate==1)&&(edited!=Minute))){
				for(int i=15;i>9;i--){
					lcd.setCursor(i,0);
					lcd.print(aktminute%2);
					aktminute/=2;
				}
				lcd.setCursor(3,1);
				if (minute<10){
					lcd.print('0');
					lcd.print(minute);
				}
				else {
					lcd.print(minute);
				}
			}
			else {
				for(int i=15;i>9;i--){
					lcd.setCursor(i,0);
					lcd.print(' ');
				}
				lcd.setCursor(3,1);
				lcd.print("  ");
				
			}
		};
		
		void kiir_second(){
			byte aktsecond=second;
			if ((activate==0)||((activate==1)&&(edited==Second)&&(counter_int<counter_pin))||((activate==1)&&(edited!=Second))){
				for(int i=15;i>9;i--){
					lcd.setCursor(i,1);
					lcd.print(aktsecond%2);
					aktsecond/=2;
				}
				lcd.setCursor(6,1);
				if (second<10){
					lcd.print('0');
					lcd.print(second);
				}
				else {
					lcd.print(second);
				}
			}
			else {
				for(int i=15;i>9;i--){
					lcd.setCursor(i,1);
					lcd.print(' ');
				}
				lcd.setCursor(6,1);
				lcd.print("  ");
			}
		};

		void kiir_kettospont(){
			lcd.setCursor(9,0);
			lcd.print(':');
			lcd.setCursor(2,1);
			lcd.print(':');
			lcd.setCursor(5,1);
			lcd.print(':');
		};
		
		void kiir(){
			counter_int++;
			if(counter_int>=2*counter_pin) counter_int=0;
			kiir_hour();
			kiir_minute();
			kiir_second();
			kiir_kettospont();
		};
        void gombok(){
            if(digitalRead(buttonPin_activate)!=buttonPin_activate_state){
                buttonPin_activate_state=digitalRead(buttonPin_activate);
				if(digitalRead(buttonPin_activate)==HIGH){
					activate=!activate;
					edited=Hour;
				}
            }
            if(activate==1){
                digitalWrite(led_activate,HIGH);
				if(digitalRead(button_switch_hour_minute_second)!=button_switch_hour_minute_second_state){
					button_switch_hour_minute_second_state=digitalRead(button_switch_hour_minute_second);
					if(digitalRead(button_switch_hour_minute_second)==HIGH){
						if (edited==Hour) edited=Minute;
						else if (edited==Minute) edited=Second;
								else if (edited==Second) edited=Hour;
						delay(500);
					}
				}
				if(digitalRead(buttonPin_up)!=buttonPin_up_state){
					buttonPin_up_state=digitalRead(buttonPin_up);
					if(digitalRead(buttonPin_up)==HIGH){
						if (edited==Hour) {hour_up();delay(500);}
						if (edited==Minute) {minute_up();delay(250);}
						if (edited==Second) {second_up();delay(50);}
					}
				}
				if(digitalRead(buttonPin_down)!=buttonPin_down_state){
					buttonPin_down_state=digitalRead(buttonPin_down);
					if(digitalRead(buttonPin_down)==HIGH){
						if (edited==Hour) {hour_down();delay(500);}
						if (edited==Minute) {minute_down();delay(250);}
						if (edited==Second) {second_down();delay(50);}
					}
				}
            if (edited==Hour) digitalWrite(led_hour,HIGH);
              else digitalWrite(led_hour,LOW);
            if (edited==Minute) digitalWrite(led_minute,HIGH);
              else digitalWrite(led_minute,LOW);
            if (edited==Second) digitalWrite(led_second,HIGH);
              else digitalWrite(led_second,LOW);
            }
            if(activate==0){
                digitalWrite(led_activate,LOW);
                digitalWrite(led_hour,LOW);
                digitalWrite(led_minute,LOW);
                digitalWrite(led_second,LOW);
            }

        }
                
} time;

void setup(){
  lcd.begin(16, 2);
  pinMode(led_activate,OUTPUT);
  pinMode(led_hour,OUTPUT);
  pinMode(led_minute,OUTPUT);
  pinMode(led_second,OUTPUT);
  pinMode(buttonPin_activate,INPUT);
  pinMode(button_switch_hour_minute_second,INPUT);
  pinMode(buttonPin_up,INPUT);
  pinMode(buttonPin_down,INPUT);
}

void loop(){ 
  time.szamol();
  time.gombok();
  //lcd.setCursor(0,0);
  //lcd.print(__TIME__);
  

}


