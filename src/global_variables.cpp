#include "global_variables.h"
#include "libraries.h"

// LED Variables
CRGB leds[NUM_LEDS];

long maintimer = millis();
long RTCUpdateTimer = millis();
long reconnecttimer = millis();
int redval = 255;
int greenval = 255;
int blueval = 255;



// Adresses of individual LEDs in groups
int min1[1] = {102};
int min2[2] = {102, 0};
int min3[3] = {102, 0, 113};
int min4[4] = {102, 0, 113, 11};



// DE
int DE_five[4] = {1, 21, 22, 41};
int DE_ten[4] = {80, 83, 100, 104};
int DE_quarter[7] = {3, 19, 24, 39, 44, 59, 64};
int DE_twenty[7] = {2, 20, 23, 40, 43, 60, 63};
int DE_half[4] = {77, 86, 97, 107};
int DE_to[3] = {25, 38, 45};
int DE_past[4] = {58, 65, 78, 85};
int DE_ItIs[5] = {61, 62, 81, 101, 103};
int DE_hour1[4] = {56, 67, 76, 87};
int DE_hour2[4] = {76, 87, 96, 108};
int DE_hour3[4] = {68, 75, 88, 95};
int DE_hour4[4] = {8, 14, 29, 34};
int DE_hour5[4] = {7, 15, 28, 35};
int DE_hour6[5] = {52, 71, 72, 91, 92};
int DE_hour7[6] = {6, 16, 27, 36, 47, 56};
int DE_hour8[4] = {70, 73, 90, 93};
int DE_hour9[4] = {49, 54, 69, 74};
int DE_hour10[4] = {30, 33, 50, 53};
int DE_hour11[3] = {89, 94, 110};
int DE_hour12[5] = {17, 26, 37, 46, 57};
int DE_OClock[3] = {10, 12, 31};


// EN
int EN_five[4] = {44,39,24,19};
int EN_ten[3] = {23,20,2};
int EN_quarter[7] = {104,100,83,80,60,43,63};
int EN_twenty[6] = {105,99,84,79,64,59};
int EN_half[4] = {42,41,22,21};
int EN_to[2] = {58,45};
int EN_past[4] = {106,98,85,78};
int EN_ItIs[4] = {103,101,81,62};
int EN_hour1[4] = {37,26,17};
int EN_hour2[3] = {25,18,4};
int EN_hour3[5] = {49,34,29,14,8};
int EN_hour4[3] = {110,94,89};
int EN_hour5[4] = {96,87,76,67};
int EN_hour6[3] = {30,13,9};
int EN_hour7[5] = {47,36,27,16,6};
int EN_hour8[5] = {90,73,70,53,50};
int EN_hour9[4] = {12,92,91,72};
int EN_hour10[4] = {35,28,15,7};
int EN_hour11[6] = {107,97,86,77,66,57};
int EN_hour12[6] = {109,95,88,75,68,55};
int EN_OClock[6] = {52,51,32,31,12,10};


int *five[NUMBERLANGUAGES] = {DE_five,EN_five};
int *ten[NUMBERLANGUAGES] = {DE_ten,EN_ten};
int *quarter[NUMBERLANGUAGES] = {DE_quarter,EN_quarter};
int *twenty[NUMBERLANGUAGES] = {DE_twenty,EN_twenty};
int *half[NUMBERLANGUAGES] = {DE_half,EN_half};
int *to[NUMBERLANGUAGES] = {DE_to,EN_to};
int *past[NUMBERLANGUAGES] = {DE_past,EN_past};
int *ItIs[NUMBERLANGUAGES] = {DE_ItIs,EN_ItIs};
int *hour1[NUMBERLANGUAGES] = {DE_hour1,EN_hour1};
int *hour2[NUMBERLANGUAGES] = {DE_hour2,EN_hour2};
int *hour3[NUMBERLANGUAGES] = {DE_hour3,EN_hour3};
int *hour4[NUMBERLANGUAGES] = {DE_hour4,EN_hour4};
int *hour5[NUMBERLANGUAGES] = {DE_hour5,EN_hour5};
int *hour6[NUMBERLANGUAGES] = {DE_hour6,EN_hour6};
int *hour7[NUMBERLANGUAGES] = {DE_hour7,EN_hour7};
int *hour8[NUMBERLANGUAGES] = {DE_hour8,EN_hour8};
int *hour9[NUMBERLANGUAGES] = {DE_hour9,EN_hour9};
int *hour10[NUMBERLANGUAGES] = {DE_hour10,EN_hour10};
int *hour11[NUMBERLANGUAGES] = {DE_hour11,EN_hour11};
int *hour12[NUMBERLANGUAGES] = {DE_hour12,EN_hour12};
int *OClock[NUMBERLANGUAGES] = {DE_OClock,EN_OClock};


int len_five[NUMBERLANGUAGES] = {4,4};
int len_ten[NUMBERLANGUAGES] = {4,3};
int len_quarter[NUMBERLANGUAGES] = {7,7};
int len_twenty[NUMBERLANGUAGES] = {7,6};
int len_half[NUMBERLANGUAGES] = {4,4};
int len_to[NUMBERLANGUAGES] = {3,2};
int len_past[NUMBERLANGUAGES] = {4,4};
int len_ItIs[NUMBERLANGUAGES] = {5,4};
int len_hour1[NUMBERLANGUAGES] = {4,3};
int len_hour2[NUMBERLANGUAGES] = {4,3};
int len_hour3[NUMBERLANGUAGES] = {4,5};
int len_hour4[NUMBERLANGUAGES] = {4,3};
int len_hour5[NUMBERLANGUAGES] = {4,4};
int len_hour6[NUMBERLANGUAGES] = {5,3};
int len_hour7[NUMBERLANGUAGES] = {6,5};
int len_hour8[NUMBERLANGUAGES] = {4,5};
int len_hour9[NUMBERLANGUAGES] = {4,4};
int len_hour10[NUMBERLANGUAGES] = {4,4};
int len_hour11[NUMBERLANGUAGES] = {3,6};
int len_hour12[NUMBERLANGUAGES] = {5,6};
int len_OClock[NUMBERLANGUAGES] = {3,6};






// Configurations

// initialize config

String ColorInHex = "#FF0000";
String BrightnessPercent = "50";
String TimezoneUMT = "1";
String NightmodeFrom = "22:00";
String NightmodeTo = "07:00";
String NightmodeActive = "0";
String NightmodeBrightnessPercent = "10";
String WIFISSID = "setup";
String WIFIPassword = "12345678";
String Language = "1";
String DeviceName = "Wordclock";

// 0=DE
// 1=EN


// Timekeeping

// RTC
RTC_DS3231 rtc;
bool rtc_found=false;

int hours;
int minutes;
int seconds;
String formattedDate;
String dayStamp;
String timeStamp;
char timestamp[9];
char datestamp[11];
byte dow;
byte days;   // Day (0 to 31)
byte months; // Month (0 to 12)
byte years;
boolean DST = false;

int summertime = 1;

// WIFI SECTION
WiFiClient myclient;
Preferences preferences;
int numbervalues = 15;
const char *HOSTNAME = "wordclock";
String ConfigName[16];  // name of the configuration value
String ConfigValue[16]; // the value itself (String)

String Wlanssid[50];
String Wlanrssi[50];
String Wlanauth[50];

int numberWlan;
boolean wificonnected = false;
String wifiConnectionState = "disconnected";
boolean newnetwork = false;
int reconnectionCounter = 0;
// BLE SECTION
bool isConnected = false;
long heartbeatTimer = 0;
int connectionID = 0;
