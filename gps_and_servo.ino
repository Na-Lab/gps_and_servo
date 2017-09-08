#include <SoftwareSerial.h>
#include <Servo.h>/*サーボ*/

//http://baticadila.dip.jp/arduino_105.html
//を参考


#define PIN_GPS_Rx 10
#define PIN_GPS_Tx 11

#define SERIAL_BAUDRATE 9600
#define GPS_BAUDRATE 9600

#define BUFFER_SIZE 256
//Servo myservo;/*サーボ*/

SoftwareSerial sGps(PIN_GPS_Rx, PIN_GPS_Tx);
bool isValid;
  
void setup() { /*ここで初期値をセットアップ*/
  // put your setup code here, to run once:
 
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("GPS Logger Start!"); 
  //sGps.begin(GPS_BAUDRATE);

  isValid = false;

//  myservo.attach(9,955,5000); /*(pin番号,最小パルス幅,最大パルス幅)*/


}

void loop() {  /*この部分でループする.*/
  // put your main code here, to run repeatedly,fai:

 char buf[BUFFER_SIZE];
 getLine(buf);  
 analyzeData(buf);
 led_func();

}



/*****************************************************
 * bufにgpsから取得するデータ1行分(改行コード0x0Aまで)の文字列を格納する。 
 * ただし、最大でBUFFER_SIZEの大きさまで。
 */
void getLine(char *buf){  /*関数を定義.名前はgetLine.  voidはその関数を呼び出した側になんの情報も返さないことを示す*/
  if(buf == NULL)
    return;
    
  int count = 0;
  
  do {
    if (sGps.available()) {
      buf[count] = sGps.read();
      count++;
    }
    if (count >= BUFFER_SIZE) break;
  } while(buf[count - 1] != 0x0A);
  buf[count] = '\0';
}

/*************
 *gpsデータ1行分(buf)を読み込んで解析する。 
 *
 *gpsデータが有効状態のときに、センテンスID=$GPGAAのデータから位置情報を取得する。
 */
 
void analyzeData(char *buf){  /*a関数analyzDataを宣言*/
  if(buf == NULL)
    return;
  
  checkValidity(buf);

  if(!isValid)
    return;
  
  char *gpsTime, *gpsLat, *gpsLon,*gpsLatDir, *gpsLonDir, *gpsTemp; 
  double d,r,y1,y2,x1,x2,dx,fai;
  double gpsLat_10, gpsLon_10;
  float pulseMin,pulseMax,pulse_deg,pulse,deg1;/*サーボ*/
 
  if (strncmp("$GPGGA", buf, 6) == 0) {
    strtok(buf, ",");
    gpsTime = strtok(NULL, ","); 
    gpsLat = strtok(NULL, ",");  
    gpsLatDir = strtok(NULL, ",");
    gpsLon = strtok(NULL, ","); 
    gpsLonDir = strtok(NULL, ",");
 //   gpsLat_10 = stod(gpsLat)；
  //  gpsLon_10 = stod(gpsLon);
    y1 = (stod(gpsLat)/180)*PI;  /*現在位置の緯度*/
    y2 = (24.335213/180)*PI;     /*目的地の緯度*/
    x1 = (stod(gpsLon)/180)*PI;  /*現在位置の経度*/
    x2 = ((123.978796)/180)*PI;  /*目的地の経度*/
    dx=x2-x1;  /*目的地と現在地の経度の差*/
    r=6378.137;

    d = r*acos(sin(y1)*sin(y2)+cos(y1)*cos(y2)*cos(dx));
    fai = 180*((atan2(sin(dx),cos(y1)*tan(y2)-sin(y1)*cos(dx))))/PI;
  
    Serial.print("Time = ");
    Serial.print(gpsTime);
    Serial.print(" : ");
    Serial.print("緯度(10進数じゃない) = ");  
    Serial.print(gpsLatDir);
    Serial.print(" - ");
    Serial.print(gpsLat);
    Serial.print(" : ");
    Serial.print("経度(10進数じゃない) = ");
    Serial.print(gpsLonDir);
    Serial.print(" - ");
    Serial.println(gpsLon);
    Serial.print("緯度");
    Serial.print(stod(gpsLat));
    Serial.print("経度");
    Serial.println(stod(gpsLon));

    Serial.print("目的地までの距離km = ");
    Serial.print(d);
    Serial.print("方位角 = ");
    Serial.println(fai);
    Serial.println(String(x1, 4));
    Serial.println(String(x2, 4));
    Serial.println(y1);
    Serial.println(String(dx, 4));

///*サーボ*//
//fai=90ならギコギコならない.

   if (fai > 0 && fai < 360){
   fai = fai;}
   else if (fai<0){
   fai = 360 + fai;
   }
//
//  fai=180;
//
//  pulseMin = 955; /* パルス幅最小値を360で割る*/
//  pulseMax = 2000;  /* パルス幅最大値を360で割る*/
// 
//  pulse_deg = (pulseMax-pulseMin)/360;  /*1度あたり何パルス幅増えるか*/
//
//  pulse = pulse_deg*fai+pulseMin;  /*degは何パルス幅か*/
//
//  myservo.writeMicroseconds(pulse);

  }
}


//*サーボ*/
  
    
 

float stod(char *s) { /*大内が定義した関数stod.  charをfloatにして返す*/
  float f;
  float deg, min;
   
  f = atof(s);
 
  deg = (int)f / 100;
  min = f - deg * 100;
   
  return deg + min / 60;
}
/**************
 * gps情報が有効かの判断をする。
 * 
 * センテンスID=$GPRMCの第２Fieldの値がAのとき、gps情報が有効であると判定する。
 * http://akizukidenshi.com/download/ds/canmore/GMS7-CR6_v1.0_ak.pdf
 */
void checkValidity(char *buf){
  if(buf == NULL){
    isValid = false;
    return;
  }
  
  char *stat;
  if (strncmp("$GPRMC", buf, 6) == 0) {
    char tmp[BUFFER_SIZE];
    strcpy(tmp, buf);
    
    stat = strtok(tmp, ","); 
    stat = strtok(NULL, ","); 
    stat = strtok(NULL, ","); 
    Serial.println(stat);
    
    isValid = (strncmp("A", stat, 1) == 0);
  }
}

