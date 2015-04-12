typedef struct {
    char date[9];
    char time[5];
    int energyGeneration[288];
    int energyConsumption;
    int powerGeneration;
    int powerConsumption;
    float efficiency;
    float temperature;
    float voltage;
} PvStatus;
