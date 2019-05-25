char *babes[]={"+639078388756","+639082665764"};   //paltan mo ng String(capital S) pag di gumana ang char * (pati * tatanggalin)

void setup() {
  Serial.begin(9600);
  delay(1000); 

  for(int i = 0; i<=1; i++){
    Serial.println("AT+CMGF=1");    
    delay(2000);
    Serial.print("AT+CMGS="); //kasi AT+CMGS=babes[0] ---> AT+CMGS="+639078388756"
    Serial.print(babes[i]); //sa una babes[0] = which is equal sa unang number. sa next loop babes[1] which is 9082665764 naman
    Serial.write(0x22);  // ????????
    Serial.write(0x0D);  // hex equivalent of Carraige return     //????
    Serial.write(0x0A);  // hex equivalent of newline //???????  // pede naman ata na Serial.println(babes[i]); para wala na itong 0x0A
    delay(2000);
    Serial.print("Hi mga Babe!");
    delay(5000);
    Serial.println (char(26));//the ASCII code of the ctrl+z is 26
    Serial.println ("nagsend na");
  }
}

void loop(){
}
