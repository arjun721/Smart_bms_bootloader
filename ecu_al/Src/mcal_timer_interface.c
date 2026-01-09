#include "mcal_timer_interface.h"

volatile uint8_t u8OneSecToggle ;  // Toggles every 1 second


void App_TimerInit(void)
{
    uint16_t                    u16ArrValue;
    uint16_t                    u16CntValue;
    stc_tim3_mode0_cfg_t     stcTim3BaseCfg;
    
    DDL_ZERO_STRUCT(stcTim3BaseCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE); //Base Timer
    
    stcTim3BaseCfg.enWorkMode = Tim3WorkMode0;              //?????
    stcTim3BaseCfg.enCT       = Tim3Timer;                  //?????,???????PCLK
    stcTim3BaseCfg.enPRS      = Tim3PCLKDiv64;              //PCLK/16
    stcTim3BaseCfg.enCntMode  = Tim316bitArrMode;           //????16????/???
    stcTim3BaseCfg.bEnTog     = FALSE;
    stcTim3BaseCfg.bEnGate    = FALSE;
    stcTim3BaseCfg.enGateP    = Tim3GatePositive;
    
    Tim3_Mode0_Init(&stcTim3BaseCfg);                       //TIM3 ???0?????
        
    u16ArrValue = 0x10000 - 62500 ;
    
    Tim3_M0_ARRSet(u16ArrValue);                            //?????(ARR = 0x10000 - ??)
    
    u16CntValue = 0x10000 - 62500;
    
    Tim3_M0_Cnt16Set(u16CntValue);                          //??????
    
    Tim3_ClearIntFlag(Tim3UevIrq);                          //?????
    Tim3_Mode0_EnableIrq();                                 //??TIM3??(??0???????)
    EnableNvic(TIM3_IRQn, IrqLevel3, TRUE);                 //TIM3 ??? 
}  


void App_TimerStart(void)
{
   Tim3_M0_Run();
}

