#include "bms.h"


#define cold_state 1
#define init_state 2
#define idle_state 3
#define soc_state 4
#define fault_state 5


uint8_t state = 1;

struct BSMconfig
{
    uint8_t slavesCount = 0;
} bmsconf;




FSM_update(){
    switch (state)
    {
    case cold_state:
        hvRelayOpen();
        state = init_state;
        break;



    case init_state:
        // read stm eeprom for config



        // CAN connectivity check




        ADBMS1818_init_reg_limits(bmsconf.slavesCount, &slaves); // Prepare structs



        // count daisy chain slave
        slavesNumber = countSlaves();
        if(slavesNumber != bmsconf.slavesCount) {
            // Not the explected amount of slaves
        }

        ISenseSlave = findISenseSlave(slavesNumber, &slaves);
        if(ISenseSlave == -1) {
            // No I sense
        }



        // Setup config, put it in the struct and write it to the slaves.

        bool gpio_set[] = {true, true, true, true, true};
        bool dcc_set[] = {false, false, false, false, false, false, false, false, false, false, false, false};
        bool dcto_set[] = {false, true, true, true};
        bool ps_set[] = {false, false};

        // oa REFON ADCOPT = 0
        ADBMS1818_set_cfgr(slavesNumber, &slaves, true, false, gpio_set, dcc_set, dcto_set, 25000, 42000);
        ADBMS1818_set_cfgrb(slavesNumber, &slaves, false, false, ps_set, gpio_set, dcc_set);

        ADBMS1818_wrcfg(slavesNumber, &slaves);
        ADBMS1818_wrcfgb(slavesNumber, &slaves);



        ADBMS1818_diagn(); // MUX selftest
        ADBMS1818_pollAdc(); // Blocking wait
        ADBMS1818_rdstat(2, slavesNumber, &slaves);
        for(int i = 0; i < slavesNumber; i++) {
            if(slaves[i].stat.mux_fail){
                // MUX failure
            }
        }

        // 7khz ST 01  0x9555 p.36 Datasheet
        ADBMS1818_cvst(0b10, 0b01); // cell ADC test
        ADBMS1818_pollAdc(); // Blocking wait
        ADBMS1818_rdcv(1, slavesNumber, &slaves);
        for(int i = 0; i < slavesNumber; i++) {
            if(slaves[i].cells.c_codes[0] != 0x9555){
                // Cell ADC failure
            }
        }
        
        ADBMS1818_axst(0b10, 0b01); // AUX ADC test
        ADBMS1818_pollAdc(); // Blocking wait
        ADBMS1818_rdaux(1, slavesNumber, &slaves);
        for(int i = 0; i < slavesNumber; i++) {
            if(slaves[i].aux.a_codes[0] != 0x9555){
                // AUX ADC failure
            }
        }
        
        ADBMS1818_statst(0b10, 0b01); // STAT ADC test
        ADBMS1818_pollAdc(); // Blocking wait
        ADBMS1818_rdstat(1, slavesNumber, &slaves);
        for(int i = 0; i < slavesNumber; i++) {
            if(slaves[i].stat.stat_codes[0] != 0x9555){
                // STAT ADC failure
            }
        }

        break;


    
    case soc_state:

        break;


    case fault_state:

        break;
    
    default:
        break;
    }
}