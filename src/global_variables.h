#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include "libraries.h"

// LED Variables
#define DATA_PIN 13
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS 115
#define FRAMES_PER_SECOND 60
#define NUMBERLANGUAGES 2
extern CRGB leds[NUM_LEDS];

// Timer Variables
extern long maintimer;
extern long RTCUpdateTimer;
extern long reconnecttimer;

// RGB Color Values
extern int redval;
extern int greenval;
extern int blueval;

// LED Addresses for Groups
extern int min1[1];
extern int min2[2];
extern int min3[3];
extern int min4[4];

// DE Arrays
extern int DE_five[4];
extern int DE_ten[4];
extern int DE_quarter[7];
extern int DE_twenty[7];
extern int DE_half[4];
extern int DE_to[3];
extern int DE_past[4];
extern int DE_ItIs[5];
extern int DE_hour1[4];
extern int DE_hour2[4];
extern int DE_hour3[4];
extern int DE_hour4[4];
extern int DE_hour5[4];
extern int DE_hour6[5];
extern int DE_hour7[6];
extern int DE_hour8[4];
extern int DE_hour9[4];
extern int DE_hour10[4];
extern int DE_hour11[3];
extern int DE_hour12[5];
extern int DE_OClock[3];

// EN Arrays
extern int EN_five[4];
extern int EN_ten[3];
extern int EN_quarter[7];
extern int EN_twenty[6];
extern int EN_half[4];
extern int EN_to[2];
extern int EN_past[4];
extern int EN_ItIs[4];
extern int EN_hour1[4];
extern int EN_hour2[3];
extern int EN_hour3[5];
extern int EN_hour4[3];
extern int EN_hour5[4];
extern int EN_hour6[3];
extern int EN_hour7[5];
extern int EN_hour8[5];
extern int EN_hour9[4];
extern int EN_hour10[4];
extern int EN_hour11[6];
extern int EN_hour12[6];
extern int EN_OClock[6];

// Language-based pointers and lengths
extern int *five[NUMBERLANGUAGES];
extern int *ten[NUMBERLANGUAGES];
extern int *quarter[NUMBERLANGUAGES];
extern int *twenty[NUMBERLANGUAGES];
extern int *half[NUMBERLANGUAGES];
extern int *to[NUMBERLANGUAGES];
extern int *past[NUMBERLANGUAGES];
extern int *ItIs[NUMBERLANGUAGES];
extern int *hour1[NUMBERLANGUAGES];
extern int *hour2[NUMBERLANGUAGES];
extern int *hour3[NUMBERLANGUAGES];
extern int *hour4[NUMBERLANGUAGES];
extern int *hour5[NUMBERLANGUAGES];
extern int *hour6[NUMBERLANGUAGES];
extern int *hour7[NUMBERLANGUAGES];
extern int *hour8[NUMBERLANGUAGES];
extern int *hour9[NUMBERLANGUAGES];
extern int *hour10[NUMBERLANGUAGES];
extern int *hour11[NUMBERLANGUAGES];
extern int *hour12[NUMBERLANGUAGES];
extern int *OClock[NUMBERLANGUAGES];

extern int len_five[NUMBERLANGUAGES];
extern int len_ten[NUMBERLANGUAGES];
extern int len_quarter[NUMBERLANGUAGES];
extern int len_twenty[NUMBERLANGUAGES];
extern int len_half[NUMBERLANGUAGES];
extern int len_to[NUMBERLANGUAGES];
extern int len_past[NUMBERLANGUAGES];
extern int len_ItIs[NUMBERLANGUAGES];
extern int len_hour1[NUMBERLANGUAGES];
extern int len_hour2[NUMBERLANGUAGES];
extern int len_hour3[NUMBERLANGUAGES];
extern int len_hour4[NUMBERLANGUAGES];
extern int len_hour5[NUMBERLANGUAGES];
extern int len_hour6[NUMBERLANGUAGES];
extern int len_hour7[NUMBERLANGUAGES];
extern int len_hour8[NUMBERLANGUAGES];
extern int len_hour9[NUMBERLANGUAGES];
extern int len_hour10[NUMBERLANGUAGES];
extern int len_hour11[NUMBERLANGUAGES];
extern int len_hour12[NUMBERLANGUAGES];
extern int len_OClock[NUMBERLANGUAGES];

// Configuration Strings
extern String ColorInHex;
extern String BrightnessPercent;
extern String TimezoneUMT;
extern String NightmodeFrom;
extern String NightmodeTo;
extern String NightmodeActive;
extern String NightmodeBrightnessPercent;
extern String WIFISSID;
extern String WIFIPassword;
extern String Language;
extern String DeviceName;

// RTC and Timekeeping
extern RTC_DS3231 rtc;
extern bool rtc_found;
extern int hours;
extern int minutes;
extern int seconds;
extern String formattedDate;
extern String dayStamp;
extern String timeStamp;
extern char timestamp[9];
extern char datestamp[11];
extern byte dow;
extern byte days;
extern byte months;
extern byte years;
extern boolean DST;
extern int summertime;

// WiFi Variables
extern WiFiClient myclient;
extern Preferences preferences;
extern int numbervalues;
extern const char *HOSTNAME;
extern String ConfigName[16];
extern String ConfigValue[16];
extern String Wlanssid[50];
extern String Wlanrssi[50];
extern String Wlanauth[50];
extern int numberWlan;
extern boolean wificonnected;
extern String wifiConnectionState;
extern boolean newnetwork;
extern int reconnectionCounter;

// BLE Variables
extern bool isConnected;
extern long heartbeatTimer;
extern int connectionID;

#endif // GLOBAL_VARIABLES_H
