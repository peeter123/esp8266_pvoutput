typedef struct {
    char date[9];
    char time[6];
    int energyGeneration;
    float energyEfficiency;
    int instantaneousPower;
    int averagePower;
    float normalisedOutput;
    int energyConsumption;
    int powerConsumption;
    float temperature;
    float voltage;
} PvStatus;
