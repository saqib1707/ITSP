byte tosend[127];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){};
}

void loop() {
  // put your main code here, to run repeatedly:
 /* String str="#"+(String)20+"~";
 //byte tosend=getBytes(str);
  str.getBytes(tosend,24);
  for (int i=0;i<24;i++){
  Serial.write(tosend[i]);
  delay(1000);
  }
  delay(500);*/
  String arr[3];
  arr[0]="#";
  arr[1]="12000";
  arr[2]="~";
  Serial.write(arr);
  delay(10);
  Serial.println();

}
