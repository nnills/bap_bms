// The header file for BMS functions and structs

#include "ADBMS1818.h"

#define maxTrials 3


cell_asic slaves[32]
uint8_t slavesNumber = 0;
uint8_t ISenseSlave = -1;

void hvRelayOpen();
void hvRelayRelease();
void commFail();

int checkOvpUvp(uint8_t total_ic, cell_asic *ic);

uint8_t findISenseSlave(uint8_t total_ic, cell_asic *ic);

uint8_t countSlaves();


#define tryComm(f)\
    for(int i = 0; i < maxTrials; i++){\
        if(f != 0){break;}\
        if(i == (maxTrials - 1)) {\
            commFail();\
        }\
    }