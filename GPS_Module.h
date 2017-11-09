#ifndef GPS_MODULE_h
#define GPS_MODULE_h

#define PIN_GPS_RX 10           // GPSモジュール受信ピン
#define PIN_GPS_TX 11           // GPSモジュール送信ピン
#define GPS_BAUDRATE 9600       // GPSモジュールボーレート
#define DELIMITER (",")         // NMEAフォーマットの区切り

typedef struct position
{
    double latitude;  // 緯度
    double longitude; // 軽度
} position_t;

void gps_init();

void get_position(position_t *pos);

#endif
