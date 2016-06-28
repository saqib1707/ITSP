#include<SoftwareSerial.h>
int ledpin=9;
#define RX 3
#define TX 2
SoftwareSerial myBluetooth(TX,RX);
char readData;
boolean isLedOn=false;
int sensorvalue=10;
int count=0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
 /* while(!Serial){
  };*/
  pinMode(ledpin,OUTPUT);
  myBluetooth.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(myBluetooth.available()>0){
    String str="";
    while(myBluetooth.available()>0){
        readData=(char)myBluetooth.read();
        str+=readData;
    }
    if(str=="TO"){
      digitalWrite(ledpin,HIGH);
      delay(10);
      isLedOn=true;
    }
    else if(str=="TF"){
      digitalWrite(ledpin,LOW);
      delay(10);
      isLedOn=false;
    }
    else if(str.toInt()>=0 && str.toInt()<=255 && isLedOn==true ){
      analogWrite(ledpin,str.toInt());
      isLedOn=true;
      delay(10);
    }else;
    Serial.println(str);
  }
  /*if(Serial.available()>0){
     String dataToSend="";
     String temp="";
    while(Serial.available()>0){
      temp+=(char)Serial.read();
    }
    dataToSend+=temp;
    myBluetooth.write(dataToSend);
    delay(5);
  }*/
  if(Serial.available()>0){
    if(count%2==0){
  myBluetooth.write("#My name is saqib1707~");
  Serial.println();
  delay(10);
  count++;
    }else{
      myBluetooth.write("#Your name is Sankhe~");
      delay(10);
      count++;
    }
  }
 
}
