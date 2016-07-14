/*
  DB.h
  Database library for Arduino 
  Written by Madhusudana das
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef DB_PROM
#define DB_PROM

#include "EEPROM.h"
#include<SoftwareSerial.h>

struct DB_Header
{
  byte n_recs;
  byte rec_size;
};
boolean check(int,int,int,int,int,int,int,int,int,int);
void checkForLetters();
#define RX 10
#define TX 11
SoftwareSerial myBluetooth(RX,TX);
int flex_thumb ,flex_index,flex_middle,flex_ring,flex_pinky;
int acc_x,acc_y,acc_z;

// slightly padded for the time being
#define DB_HEAD_SIZE 4

// DB_error values
#define DB_OK 0
#define DB_RECNO_OUT_OF_RANGE 1

#define DB_REC (byte*)(void*)&

typedef byte* DB_Rec;

class DB {
  public:
    void    create(int head_ptr, byte recsize);
    void    open(int head_ptr);
    boolean write(byte recno, const DB_Rec rec);
    boolean read(byte recno, DB_Rec rec);
    boolean deleteRec(byte recno);                  // delete is a reserved word
    boolean insert(byte recno, const DB_Rec rec);
    void    append(DB_Rec rec);
  byte   nRecs();
    DB_Header DB_head;
    byte DB_error;
  private:
    int writeHead();
    int readHead();
    int EEPROM_dbWrite(int ee, const byte* p);
    int EEPROM_dbRead(int ee, byte* p);
    int DB_head_ptr;
    int DB_tbl_ptr;
};

extern DB db;

#endif

// DB.cpp
/*
  Database library for Arduino 
  Written by Madhusudana das
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/



/**************************************************/
// private functions
int DB::writeHead()
{
    byte * p = (byte*)(void*)&DB_head;
    int ee = DB_head_ptr;
    int i;
    for (i = 0; i < (int)sizeof(DB_head); i++)
      EEPROM.write(ee++, *p++);
    return i;
}

int DB::readHead()
{
    byte* p = (byte*)(void*)&DB_head;
    int ee = DB_head_ptr;
    int i;
    for (i = 0; i < (int)sizeof(DB_head); i++)
      *p++ = EEPROM.read(ee++);
    return i;
}

int DB::EEPROM_dbWrite(int ee, const byte* p)
{
    int i;
    for (i = 0; i < DB_head.rec_size; i++)
      EEPROM.write(ee++, *p++);
    return i;
}

int DB::EEPROM_dbRead(int ee, byte* p)
{  
    int i;
    for (i = 0; i < DB_head.rec_size; i++)
      *p++ = EEPROM.read(ee++);
    return i;
}

/**************************************************/
// public functions

void DB::create(int head_ptr, byte recsize)
{
  DB_head_ptr = head_ptr;
  DB_head.n_recs   = 0;
  DB_head.rec_size = recsize;
  writeHead();
}

void DB::open(int head_ptr)
{
  DB_head_ptr = head_ptr;
  DB_tbl_ptr  = head_ptr + DB_HEAD_SIZE;
  readHead();
}
//other operations commit DB_head edits to EEPROM so no need for a DB_close


boolean DB::write(byte recno, const DB_Rec rec)
{
  DB_error = DB_OK;
  if (recno>0 && recno<=DB_head.n_recs+1)
    EEPROM_dbWrite(DB_tbl_ptr+((recno-1)*DB_head.rec_size), rec);
  else
    DB_error = DB_RECNO_OUT_OF_RANGE;
  return DB_error==DB_OK;
}


boolean DB::read(byte recno, DB_Rec rec)
{
  DB_error = DB_OK;
  if (recno>0 && recno<=DB_head.n_recs)
    EEPROM_dbRead(DB_tbl_ptr+((recno-1)*DB_head.rec_size), rec);
  else
    DB_error = DB_RECNO_OUT_OF_RANGE;
  return DB_error==DB_OK;
}


boolean DB::deleteRec(byte recno)
{
  DB_error = DB_OK;
  if (recno<0 || recno>DB_head.n_recs)
  {  Serial.println("recno out of range");
    DB_error = DB_RECNO_OUT_OF_RANGE;
    return false;
  }
  DB_Rec rec = (byte*)malloc(DB_head.rec_size);
  for (int i=recno+1; i<=DB_head.n_recs; i++)
  {
    read(i,rec);
    write(i-1,rec);
  }  
  free(rec);
  DB_head.n_recs--;
  EEPROM.write(DB_head_ptr,DB_head.n_recs);
  return true;
}


boolean DB::insert(byte recno, DB_Rec rec)
{
  DB_error = DB_OK;
  if (recno<0 || recno>DB_head.n_recs)
  {  Serial.println("recno out of range");
    DB_error = DB_RECNO_OUT_OF_RANGE;
    return false;
  }
  DB_Rec buf = (byte*)malloc(DB_head.rec_size);
  for (int i=DB_head.n_recs; i>=recno; i--)
  {
    read(i,buf);
    write(i+1,buf);
  }
  free(buf);
  write(recno,rec);  
  DB_head.n_recs++;
  EEPROM.write(DB_head_ptr,DB_head.n_recs);
  return true;
}

void DB::append(DB_Rec rec)
{
  DB_error = DB_OK;
  DB_head.n_recs++;
  write(DB_head.n_recs,rec);
  EEPROM.write(DB_head_ptr,DB_head.n_recs);
}

byte DB::nRecs()
{
  return DB_head.n_recs;
}







#include "SD.h"
#define SD_ChipSelectPin 4

#include "SPI.h"


#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>


#include<EEPROM.h>
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
  int minimum,maximum;
  } ;
struct words {
  minmax sensor[8];//sensor 0 to 4 for flex sensors and rest for adxl sensors.
  String names ;//it is the word associated with the sign.
  } ;  

std::vector<words> sign; //these are the words present in our list

String output ;

;//defining

int analog[8]={A0,A1,A2,A3,A4,A5,A6,A7};//so that for loop can be used easily..

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
    while (!Serial); // wait for Leonardo enumeration, others continue immediately
    myBluetooth.begin(9600);
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

     flex_thumb = analogRead(A0) ;
     flex_index = analogRead(A1) ; 
     flex_middle = analogRead(A2) ; 
     flex_ring = analogRead(A3) ; 
     flex_pinky = analogRead(A4) ;//reading flex sensor values

     acc_x = analogRead(A5) ; 
     acc_y=analogRead(A6) ; 
     acc_z=analogRead(A7) ;//creating accelerometer readings 
     
int option1,option2;
//option1=0;
//option2=0;
char readData;
String actionName;
if(myBluetooth.available()>0){
    String str="";
    while(myBluetooth.available()>0){
        readData=(char)myBluetooth.read();
        str+=readData;
    }
    if(str=="P"){
      option1=1;
      option2=0;
    }
    else if(str!="P" && str.length()>1){
      option1=1;
      option2=1;
      actionName=str;
    }
    else{}
    Serial.print(str);
    Serial.println(str.length());
  }   

if(option1==1){
    int sensorMax[8],sensorMin[8];                                //defining max and min.
   
    if(option2 == 1){                                             //option 2 =0 means calibrating
        for(int x=0;x<8;x++){
          sensorMax[x]=0;
          sensorMin[x]=1023;
        }//for setting the default values of max and min.
      
        unsigned int i = sizeof(sign);
        Serial.println(i);
        sign[i].names=actionName;
        Serial.println(sign[i].names);
      
        while (millis() < 4000) {
          for(int j=0;j< 8;j++) {
            sign[i].sensor[j].maxi  = analogRead(analog[j]);
      
            // record the maximum sensor value
            if (sign[i].sensor[j].maxi > sensorMax[j]) {
              sensorMax[j] = sign[i].sensor[j].maxi ;
            }
      
            // record the minimum sensor value
            if (sign[i].sensor[j].mini < sensorMin[j]) {
              sensorMin[j] = sign[i].sensor[j].mini;
            }
          }// end of loop
        }
        for(int j=0;j< 8;j++) {
            sign[i].sensor[j].maximum = sensorMax [j] + 20 ;
            sign[i].sensor[j].minimum = sensorMin [j] - 20 ;
          }
          db.append(DB_REC sign);
        //it will toggle back to play mode.
    }
     
    if(option2==0){
        unsigned int i = sizeof(sign);
       // Serial.println(i);
        for(int j=0;j<=i;j++){
          /* checking all the words now onwards*/
            if(/*higher values*/ flex_thumb < sign[j].sensor[0].maximum && flex_index < sign[j].sensor[1].maximum && flex_middle <sign[j].sensor[2].maximum && flex_ring <sign[j].sensor[3].maximum && flex_pinky<sign[j].sensor[4].maximum && acc_x < sign[j].sensor[5].maxi && acc_y < sign[j].sensor[6].maxi && acc_z < sign[j].sensor[7].maxi
          && /*lower values*/  flex_thumb > sign[j].sensor[0].minimum && flex_index > sign[j].sensor[1].minimum && flex_middle >sign[j].sensor[2].minimum && flex_ring >sign[j].sensor[3].minimum && flex_pinky>sign[j].sensor[4].minimum && acc_x > sign[j].sensor[5].mini && acc_y > sign[j].sensor[6].mini && acc_z > sign[j].sensor[7].mini  ){
                 myBluetooth.print(sign[j].names);
                 Serial.print("Word detected:");
                 Serial.println(sign[j].names);
                 break ;
            }
          }
          checkForLetters();
     }
    else {} ;
  }
}
void checkForLetters(){

    if(check(65,85,100,130,105,130,105,120,75,95)){
        myBluetooth.write("#a~");
        Serial.println("a");
        delay(1000);
      }
      else if(check(25,50,20,40,20,45,15,35,-15,5)){
        myBluetooth.write("#b~");
         Serial.println("b");
         delay(1000);
      }
      else if(check(15,35,40,70,79,103,70,100,30,60)){
        myBluetooth.write("#c~");
         Serial.println("c");
         delay(1000);
      }
      else if(check(55,75,30,55,130,155,100,125,60,85)){
        myBluetooth.write("#d~");
         Serial.println("d");
         delay(1000);
      }
      else if(check(65,95,125,150,115,145,105,125,-15,5)){
        myBluetooth.write("#i~");
         Serial.println("i");
         delay(1000);
      }
      else if(check(-5,15,25,50,30,55,100,130,60,90)){
        myBluetooth.write("#k~");
         Serial.println("k");
         delay(1000);
      }
      else if(check(75,100,95,125,100,130,85,120,75,100)){
        myBluetooth.write("#m~");
         Serial.println("m");
         delay(1000);
      }
      else if(check(20,45,20,40,35,55,110,140,70,100)){
        myBluetooth.write("#u~");
         Serial.println("u");
         delay(1000);
      }
      else if(check(20,60,15,40,15,40,30,65,80,110)){
        myBluetooth.write("#w~");
         Serial.println("w");
         delay(1000);
      }
      else if(check(90,115,115,140,120,140,95,120,75,100)){
        myBluetooth.write("#x~");
         Serial.println("x");
         delay(1000);
      }
      else if(check(-15,10,110,150,110,150,100,140,-15,10)){
        myBluetooth.write("#y~");
         Serial.println("y");
         delay(1000);
      }
      else if(check(30,70,40,75,85,115,70,100,30,60)){
          myBluetooth.write("#o~");
         Serial.println("o");
         delay(1000);
      }
      else if(check(-15,15,30,60,120,150,100,135,60,95)){
        myBluetooth.write("#l~");
         Serial.println("l");
         delay(1000);
      }
      else;
  
}

boolean check(int thumbL,int thumbH,int indexL,int indexH,int middleL,int middleH,int ringL,int ringH,int littleL,int littleH){
  if(flex_thumb>=thumbL && flex_thumb<=thumbH && flex_index>=indexL && flex_index<=indexH && flex_middle>=middleL && flex_middle<=middleH && flex_ring>=ringL && flex_ring<=ringH && flex_pinky>=littleL && flex_pinky<=littleH){
    return true;
  }
  else{
    return false;
  }
}
