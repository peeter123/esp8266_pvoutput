#define MAX_STATS 288

typedef struct {
  char date[8];
  char time[6];
  int instantaneousPower[MAX_STATS];
  int len;
} PvStats;
