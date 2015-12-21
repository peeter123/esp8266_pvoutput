#define MAX_STATS 288

typedef struct {
  char date[8];
  char time[6];
  int instantaneousPower[MAX_STATS];
  int len;
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



