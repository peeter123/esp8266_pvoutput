#define MAX_STATS 288

typedef struct {
  char date[8];
  char startTime[6];
  char endTime[6];
  int len;
  int instantaneousPower[MAX_STATS];
} PvStats;


typedef struct {
  char systemName[33]; //name (maximum 32 characters. 
  int systemSize;
  int postcode;
  int numberOfPanels;
  int panelPower;
  char panelBrand[17];
  int numberOfInverters;
  int inverterPower;
  char inverterBrand[17];
  char orientation[4];
  float arrayTilt;
  char shade[6];
  char installDate[8];
  float latitude;
  float longitude;
  int statusInterval; //interval in minuts between updates
} PvSystemService;



