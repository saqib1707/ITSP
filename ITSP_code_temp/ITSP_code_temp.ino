
#include<SoftwareSerial.h>
#define RX 3
#define TX 2
SoftwareSerial myBluetooth(TX,RX);


int acc_x_array[2];
int acc_x_array_index=0;
int acc_z_array[2];
int acc_z_array_index=0;//declaring array and its variables
boolean is_hello , is_yes , is_love ,is_you , is_like,is_fuckoff;
int hello_yes_threshold_acc_x = 60/* !!!! type a threshold */ , hello_yes_threshold_acc_y = -40/* !!!! */ , you_threshold_acc_z = 55/* !!!! */ , you_threshold_acc_y = -60/* !!!! */ ;

void setup(){
  Serial.begin(19200);
  while(!Serial){};
  myBluetooth.begin(9600);
  } 

  
void loop(){
  int acc_x = analogRead(A5)-352 , acc_y=analogRead(A6)-352 , acc_z=analogRead(A7)-352 ;//creating accelerometer readings
  int  hello_threshold_acc_x_count=0 , yes_threshold_acc_x_count=0 ,you_threshold_acc_z_count=0 ;
  int flex_thumb = analogRead(A0)-760 , flex_index = analogRead(A1)-720 , flex_middle = analogRead(A2)-697 , flex_ring = analogRead(A3)-770 , flex_pinky = analogRead(A4)-790 ;//reading flex sensor values
  
  Serial.print("ax = ");
  Serial.print(acc_x);
  Serial.print("\t");
  
  Serial.print("ay = ");
  Serial.print(acc_y);
  Serial.print("\t");
  
  Serial.print("az = ");
  Serial.print(acc_z);
  Serial.println("\t");
  Serial.print("\n") ;

  Serial.print("thumb = ");
  Serial.print(flex_thumb);
  Serial.print("\t");

  Serial.print("index = ");
  Serial.print(flex_index);
  Serial.print("\t");

  Serial.print("middle = ");
  Serial.print(flex_middle);
  Serial.print("\t");

  Serial.print("ring = ");
  Serial.print(flex_ring);
  Serial.print("\t");

  Serial.print("pinky = ");
  Serial.print(flex_pinky);
  Serial.println("\t");
  Serial.print("\n");
  delay(50);
  //check hello and yes
  acc_x_array[acc_x_array_index] = acc_x ;
  acc_x_array_index = (acc_x_array_index+1)%2; 

  acc_z_array[acc_x_array_index] = acc_z ;
  acc_z_array_index = (acc_x_array_index+1)%2; // creating acc_x_array and acc_z_array

    for(int i=0 ; i<2 ;i++){ 
    Serial.print(acc_z_array[i]) ;
    Serial.print("\t") ;
    
      }
Serial.print("\n") ;
  
  for(int j=0 ; j<2 ; j++) {
    if( acc_x_array[j] > hello_yes_threshold_acc_x && acc_y < hello_yes_threshold_acc_y ){
      if(/*higher values*/ flex_thumb < 30 && flex_index < 30 && flex_middle <35 && flex_ring <40 && flex_pinky<30) {
        hello_threshold_acc_x_count++ ;
        if(hello_threshold_acc_x_count ) is_hello = true ;
      }
      if(/*lower values*/flex_thumb > 70 && flex_index >110 && flex_middle > 100 && flex_ring >70&& flex_pinky>75) {
        yes_threshold_acc_x_count++ ;
        if(yes_threshold_acc_x_count == 4) is_yes = true ;
      }

    }
  }
//check for you 
 
    if(/*higher values*/ flex_thumb < 30 && flex_index < 30 && flex_middle <35 && flex_ring <40 && flex_pinky<30 && acc_z > you_threshold_acc_z && acc_y < you_threshold_acc_y  ){
       is_you = true ;
    }
  
  
  //check for "i love you"
  if(/*lower values*/flex_thumb > 5 && flex_index >0  && flex_middle > 90 && flex_ring >65 && flex_pinky>-10 &&
    /*higher values*/ flex_thumb <30 && flex_index < 30 && flex_middle <150 && flex_ring <100 && flex_pinky<16) {
    is_love = true ;
  }

  // check for like
  if(/*lower values*/flex_thumb > 15 && flex_index >100 && flex_middle > 100 && flex_ring >70 && flex_pinky>75 &&
    /*higher values*/ flex_thumb < 60 && flex_index < 150 && flex_middle <150 && flex_ring <110 && flex_pinky<110) {
    is_like = true ;
  }

  //check for fuckoff
  if( /*lower values*/flex_thumb > 95 && flex_index >80 && flex_middle >0 && flex_ring >65 && flex_pinky>45  &&
    /*higher values*/ flex_thumb < 150 && flex_index < 160 && flex_middle <30 && flex_ring <100 && flex_pinky<110) {
    is_fuckoff = true ;
  }

 
  
//give outputs
  if(is_hello) {

    Serial.println("hello") ;
    myBluetooth.write("#hello~");
    acc_x = 0 , acc_y= 0 , acc_z= 0  ;
    flex_thumb = 0 , flex_index = 0 , flex_middle = 0 , flex_ring = 0 , flex_pinky = 0 ;
    for(int i=0 ; i<4 ; i++){
      acc_x_array[i]=0 ;
    }
    is_hello = false ;
    delay(1000);
  }// give hello output

  if(is_yes) {

    Serial.println("yes") ;
    myBluetooth.write("#yes~");
    acc_x = 0 , acc_y= 0 , acc_z= 0  ;
    flex_thumb = 0 , flex_index = 0 , flex_middle = 0 , flex_ring = 0 , flex_pinky = 0 ;
    for(int i=0 ; i<4 ; i++){
      acc_x_array[i]=0 ;
    }
    is_yes=false ;
    delay(1000);
  }// give yes output

  if(is_love) {
    Serial.println("i love you") ;
    myBluetooth.write("#I love you~");
    flex_thumb = 0 , flex_index = 0 , flex_middle = 0 , flex_ring = 0 , flex_pinky = 0 ;
    is_love=false ;
    delay(1000);
  }// give "i love you " output

  if(is_you) {
    Serial.println("you") ;
    myBluetooth.write("#you~");
    is_you=false ;
    for(int i=0 ; i<4 ; i++){
      acc_z_array[i]=0 ;
    }
    acc_x = 0 , acc_y= 0 , acc_z= 0  ;
    flex_thumb = 0 , flex_index = 0 , flex_middle = 0 , flex_ring = 0 , flex_pinky = 0 ;
    delay(1000);
  }

  if(is_like) {
    Serial.println("like") ;
    myBluetooth.write("#like~");
    flex_thumb = 0 , flex_index = 0 , flex_middle = 0 , flex_ring = 0 , flex_pinky = 0 ;
    is_like=false ;
    delay(1000);
  }

  if(is_fuckoff) {
    Serial.println("Fuck Off") ;
    myBluetooth.write("#fuck off~");
    flex_thumb = 0 , flex_index = 0 , flex_middle = 0 , flex_ring = 0 , flex_pinky = 0 ;
    is_fuckoff=false ;
    delay(1000);
  }
}
