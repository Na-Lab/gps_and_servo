#include "GPS_Module.h"
#include <Arduino.h>

// GPSモジュール初期処理関数
void gps_init() { Serial1.begin(GPS_BAUDRATE); }

// DEG表記変換関数
// 引数　*data DMM表記データへのポインタ
// 戻り値　DEG表記のデータ
double dmm_2_deg(char *data) {
  double temp, deg, min;

  temp = atof(data);
  deg  = (int)(temp / 100);
  min  = temp - deg * 100;
  return (double)(deg + min / 60.0);
}

// 位置情報取得関数
// 引数　*pos位置情報データ格納構造体
// 戻り値　なし
void get_position(position_t *pos) {
  // NMEAの取得
  String raw_nmea_data;
  do {
    raw_nmea_data = Serial1.readStringUntil('\n');
  } while (raw_nmea_data.indexOf("$GPGGA") == -1);
  char *lat_p, *long_p;
  strtok(raw_nmea_data.c_str(), DELIMITER);           // $GPGGA
  strtok(NULL, DELIMITER);          // UTC時刻
  lat_p = strtok(NULL, DELIMITER);  // 緯度
  strtok(NULL, DELIMITER);          // 南北
  long_p = strtok(NULL, DELIMITER); // 経度

  // データの加工
  pos->latitude  = dmm_2_deg(lat_p);
  pos->longitude = dmm_2_deg(long_p);
}
