#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#define SPEED 500
HUSKYLENS huskylens;

void setup() {
  
  Serial.begin(1000000);
  Wire.begin();
//  while (!huskylens.begin(Wire))
//    {
//        Serial.println(F("Begin failed!"));
//        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
//        Serial.println(F("2.Please recheck the connection."));
//        delay(100);
//    }
}

void loop() {
  int psd = A2; // ananlogRead(psd);
  int Init[6] = {2548, 2558, 496, 1478, 2048, 2048};
 
  Driving_MX(Init,200);
  delay(1000);
  //Wheel_MX(200, 1024 + 200, 200, 1024+200);
  //LL(SPEED);
  delay(500);

  if(analogRead(psd) > 200);//stop
  
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
        }    
    }
    
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F("size=")+(result.widget*height)+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}
void Wheel_MX(unsigned int spd_a,unsigned int spd_b,unsigned int spd_c,unsigned int spd_d){
 unsigned char i;
  int t = 0;
  
  unsigned char Spd_L_a;
  Spd_L_a = (unsigned char)(spd_a & 0xFF);
  
  unsigned char Spd_H_a;
  Spd_H_a = (unsigned char)(spd_a >> 8) & 0xFF;
  unsigned char Spd_L_b;
  Spd_L_b = (unsigned char)(spd_b & 0xFF);
  
  unsigned char Spd_H_b;
  Spd_H_b = (unsigned char)(spd_b >> 8) & 0xFF;

  unsigned char Spd_L_c;
  Spd_L_c = (unsigned char)(spd_c & 0xFF);
  
  unsigned char Spd_H_c;
  Spd_H_c = (unsigned char)(spd_c >> 8) & 0xFF;
  unsigned char Spd_L_d;
  Spd_L_d = (unsigned char)(spd_d & 0xFF);
  
  unsigned char Spd_H_d;
  Spd_H_d = (unsigned char)(spd_d >> 8) & 0xFF;
  
  

  unsigned int txBuff[28];
  
  txBuff[0] = 0xFF;
  txBuff[1] = 0xFF;

  txBuff[2] = 0xFE;
  txBuff[3] = 0x18; //LENGTH
  
  txBuff[4] = 0x83;//0x03;
  
  txBuff[5] = 0x1E;
  
  txBuff[6] = 0x04; //갯수
  
  txBuff[7] = 0x01;
  txBuff[8] = 0;
  txBuff[9] = 0;
  txBuff[10] = Spd_L_a;
  txBuff[11] = Spd_H_a;
  t++;
  
  
  txBuff[12] = 0x02;
  txBuff[13] = 0;
  txBuff[14] = 0;
  txBuff[15] = Spd_L_b;
  txBuff[16] = Spd_H_b;
  t++;
  
  txBuff[17] = 0x03;
  txBuff[18] = 0;
  txBuff[19] = 0;
  txBuff[20] = Spd_L_c;
  txBuff[21] = Spd_H_c;
  t++;
  
  txBuff[22] = 0x04;
  txBuff[23] = 0;
  txBuff[24] = 0;
  txBuff[25] = Spd_L_d;
  txBuff[26] = Spd_H_d;
  t++;
  
  txBuff[27] = 0;
  
  for (i = 2;i<27;i++)
  txBuff[27] += txBuff[i];
  
  txBuff[27] = ~txBuff[27];
  
  for (i=0;i<28;i++)
  {
    Serial.write(txBuff[i]);
    //Uart_Putch_1(txBuff[i]);
    _delay_ms(1);
  }
}
void Driving_MX(int position[6], unsigned int spd){
  unsigned char i;
  int t = 0;
  
  unsigned char POS_L[6];
  for (int i = 0; i <6; i++)
  POS_L[i] = (unsigned char)(position[i] & 0xFF);
  
  unsigned char POS_H[6];
  for (int i = 0; i<6;i++)
  POS_H[i] = (unsigned char)(position[i] >> 8) & 0xFF;
  
  unsigned char Spd_L,Spd_L1;
  Spd_L = (unsigned char)(spd & 0xFF);
  Spd_L1 = (unsigned char)((spd*2) & 0xFF);
  unsigned char Spd_H,Spd_H1;
  Spd_H = (unsigned char)(spd >> 8) & 0xFF;
  Spd_H1 = (unsigned char)(((spd*2) >>8) & 0xFF);

  unsigned int txBuff[38];
  
  txBuff[0] = 0xFF;
  txBuff[1] = 0xFF;

  txBuff[2] = 0xFE;
  txBuff[3] = 0x22; //LENGTH
  
  txBuff[4] = 0x83;//0x03;
  
  txBuff[5] = 0x1E; // Goal position
  
  txBuff[6] = 0x04; //갯수
  
  txBuff[7] = 0x05;
  txBuff[8] = POS_L[t];
  txBuff[9] = POS_H[t];
  txBuff[10] = Spd_L;
  txBuff[11] = Spd_H;
  t++;
  
  
  txBuff[12] = 0x06;
  txBuff[13] = POS_L[t];
  txBuff[14] = POS_H[t];
  txBuff[15] = Spd_L;
  txBuff[16] = Spd_H;
  t++;
  
  txBuff[17] = 0x07;
  txBuff[18] = POS_L[t];
  txBuff[19] = POS_H[t];
  txBuff[20] = Spd_L;
  txBuff[21] = Spd_H;
  t++;
  
  txBuff[22] = 0x08;
  txBuff[23] = POS_L[t];
  txBuff[24] = POS_H[t];
  txBuff[25] = Spd_L;
  txBuff[26] = Spd_H;
  t++;
  
  txBuff[27] = 0x09;
  txBuff[28] = POS_L[t];
  txBuff[29] = POS_H[t];
  txBuff[30] = Spd_L;
  txBuff[31] = Spd_H;
  t++;
  
  txBuff[32] = 0x0A;
  txBuff[33] = POS_L[t];
  txBuff[34] = POS_H[t];
  txBuff[35] = Spd_L;
  txBuff[36] = Spd_H;
  t++;
  
  txBuff[37] = 0;
  
  for (i = 2;i<37;i++)
  txBuff[37] += txBuff[i];
  
  txBuff[37] = ~txBuff[37];
  
  for (i=0;i<38;i++)
  {
    Serial.write(txBuff[i]);
    //Uart_Putch_1(txBuff[i]);
    _delay_ms(1);
  }
}
void FF(int a){
  Wheel_MX(a,a+1024,a,a+1024);
}
void FL(int a){
  Wheel_MX(0,a+1024,a,0);
}
void FR(int a){
  Wheel_MX(a,0,0,a+1024);
}
void LL(int a){
  Wheel_MX(a+1024,a+1024,a,a);
}
void RR(int a){
  Wheel_MX(a+1024,a+1024,a,a);
}
void BB(int a){
  Wheel_MX(a+1024,a,a+1024,a);
}
void BL(int a){
  Wheel_MX(a+1024,0,0,a);
}
void BR(int a){
  Wheel_MX(0,a,a+1024,0);
}
void LT(int a){
  Wheel_MX(a,a,a,a);
}
void RT(int a){
  Wheel_MX(a+1024,a+1024,a+1024,a+1024);
}
void stp(){
  Wheel_MX(0,0,0,0);
}
