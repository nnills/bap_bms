#include "ADBMS1818.h"

#include "bms.h"

// #define maxTrials 3


// cell_asic slaves[32]
// uint8_t slavesNumber = 0;
// uint8_t ISenseSlave = -1;


void hvRelayOpen() {
    // DOE IETS
}

void hvRelayRelease() {
    // ...
}




void commFail() {
    // na max keer nog niet gelukt
}



// In Aux D en Stat B staan OV UV flags
// 
int checkOvpUvp(uint8_t total_ic, cell_asic *ic) {

    tryComm(ADBMS1818_rdaux(4, total_ic, ic)) // read AuxD
    tryComm(ADBMS1818_rdstat(2, total_ic, ic)) // read StatB


    for(int i = 0; i < total_ic; i++) {
        if (ic[i].stat.flags[0] | ic[i].stat.flags[1] | ic[i].stat.flags[2]) {
            return 1;
        }
    } else {
        return 0;
    }

}

uint8_t findISenseSlave(uint8_t total_ic, cell_asic *ic) {
    ADBMS1818_axow(MD_7KHZ_3KHZ, 0); // Read GPIO with pull-down
    while(ADBMS1818_pladc() == 0xff){} // Wait for completion

    tryComm(ADBMS1818_rdaux(1, total_ic, ic)) // read AuxA
    tryComm(ADBMS1818_rdaux(2, total_ic, ic)) // read AuxB

    for(int i = 0; i < total_ic; i++) {
        // [0] = GPIO 1, I Sense
        // [5] = VREF 2
        if(ic[i].aux.a_codes[5] < 31000 && ic[i].aux.a_codes[5] > 29000) {
            if(ic[i].aux.a_codes[0] < 31000 && ic[i].aux.a_codes[0] > 29000) {
                return i;
            }
        } else {
            // VREF2 stuk? ??? 
        }
    }
    return -1;
}


uint8_t countSlaves() {
    for(int i = bmsconf.slavesCount; i>0; i--){
            if(!ADBMS1818_rdcfg(i, &slaves)){
                return i;
            }
        }
    return 0;
}


uint8_t verifyConfig() {
    cell_asic readOut[32];

    


}