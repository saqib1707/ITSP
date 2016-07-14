#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>

#include "WProgram.h"
#include <EEPROM.h>
#include <DB.h>
#include <string.h>

DB db;

#define MY_TBL 1


using namespace std;

// <iostream> declares cout/cerr, but the application must define them
// because it's up to you what to do with them.
namespace std
{
  ohserialstream cout(Serial);
}




struct minmax {
  int mini, maxi ;
  } ;
struct words {
  minmax sensor[8];//sensor 0 to 4 for flex sensors and rest for adxl sensors.
  string names ;//it is the word associated with the sign.
  } ;  

std::vector<words> sign; //these are the words present in our list

string output ;

int analog[8];//defining

int analog[0]=A0,analog[1]=A1,analog[2]=A2,analog[3]=A3,analog[4]=A4,analog[5]=A5,analog[6]=A6,analog[7]=A7;//so that for loop can be used easily..

void selectAll()
{
  if (db.nRecs()) Serial.println("-----");
  for (int i = 1; i <= db.nRecs(); i++)
  {
    db.read(i, DB_REC sign);
      
  } 
}

void setup(){
  Serial.begin(115200);
  Serial.println("EEPROM DB Library Demo");
  Serial.println();


  Serial.print("Creating Table...");
  db.create(MY_TBL,sizeof(sign));
  db.open(MY_TBL);
  Serial.println("DONE");

  Serial.print("Creating records...");
 
   
  db.append(DB_REC sign);
  Serial.println("DONE");
  
  Serial.println();
  Serial.println("Reading records from EEPROM...");
  Serial.println();
  selectAll();

  } 
  

void loop(){


option1 = analogRead(/*pin for input*/); //option = 0 for calibration, 1 for play mode .
option2 = analogRead(/*pin for input*/); //option = 0 for calibration, 1 for play mode .
if(option1==1){
int sensorMax[8],sensorMin[8];//defining max and min.


if(option2 == 0){
for(int x=0;x<8;x++){
	sensorMax[x]=0;
	sensorMin[x]=1023;
}//for setting the default values of max and min.

unsigned int i = sign.size() ; 

 while (millis() < 4000) {
 for(int j=0;j< 8;j++) {
   sign[i].sensor[j].maxi  = analogRead(analog[j]);

    // record the maximum sensor value
    if (a[i].sensor[j].maxi > sensorMax[j]) {
      sensorMax[j] = a[i].sensor[j].maxi ;
    }

    // record the minimum sensor value
    if (a[i].sensor[j].mini < sensorMin[j]) {
      a[i].sensor[j].mini = a[i].sensor[j].mini;
    }
  }// end of loop

  for(int j=0;j< 8;j++) {
  	sensorMax[j] = sensorMax [j] + 20 ;
  	sensorMin[j] = sensorMin [j] - 20 ;
  }
 }

  db.append(DB_REC sign);
 //it will toggle back to play mode.
    }








 int acc_x = analogRead(A5) , acc_y=analogRead(A6) , acc_z=analogRead(A7) ;//creating accelerometer readings
 
 int flex_thumb = analogRead(A0) , flex_index = analogRead(A1) , flex_middle = analogRead(A2) , flex_ring = analogRead(A3) , flex_pinky = analogRead(A4) ;//reading flex sensor values
 

 if(option2==1){
 	for(int j=0;j<i;j++){
 		/* checking all the words now onwards*/
 		  if(/*higher values*/ flex_thumb < sign[j].sensor[0].maxi && flex_index < sign[j].sensor[1].maxi && flex_middle <sign[j].sensor[2].maxi && flex_ring <sign[j].sensor[3].maxi && flex_pinky<sign[j].sensor[4].maxi && acc_x < sign[j].sensor[5].maxi && acc_y < sign[j].sensor[6].maxi && acc_z < sign[j].sensor[7].maxi
&& 		/*lower values*/  flex_thumb > sign[j].sensor[0].mini && flex_index > sign[j].sensor[1].mini && flex_middle >sign[j].sensor[2].mini && flex_ring >sign[j].sensor[3].mini && flex_pinky>sign[j].sensor[4].mini && acc_x > sign[j].sensor[5].mini && acc_y > sign[j].sensor[6].mini && acc_z > sign[j].sensor[7].mini  ){
       output = sign[j].names ;
       Serial.println(output);
       delay(100);
       break ;
    }
  
 	}


 }
}
else {} ;
}
