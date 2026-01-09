#include "mcal_can_interface.h"

can_buff_t g_canBuffer[255] = {0};

can_frame_info_t std_can_pdu = {0};

QUEUE(g_can_queue, g_canBuffer);

uint8_t g_can_conneted,can_flag;

uint32_t CAN_ID;
uint8_t g_bmsinfo_setting_candata[8];
uint8_t g_cell_setting_candata[8];
uint8_t g_cell_recovery_candata[8];
uint8_t g_temp_setting_candata[8];
uint8_t g_current_setting_candata[8];
uint8_t g_cellBalance_setting_candata[8];
 extern void App_CanThresholdSetting(void);
 extern void Send_All_Config_OverCAN(void);
 
 
 

void App_CanGpioInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
 
    stcGpioCfg.enDir = GpioDirIn; 
    stcGpioCfg.enDrv = GpioDrvL; 
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable; 
    stcGpioCfg.enOD = GpioOdDisable; 
    stcGpioCfg.enCtrlMode = GpioAHB;

    Gpio_Init(BMS_CAN_RX_PORT, BMS_CAN_RX_PIN, &stcGpioCfg);
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(BMS_CAN_TX_PORT, BMS_CAN_TX_PIN, &stcGpioCfg);

    Gpio_SetAfMode(BMS_CAN_RX_PORT, BMS_CAN_RX_PIN, GpioAf3);
    Gpio_SetAfMode(BMS_CAN_TX_PORT, BMS_CAN_TX_PIN, GpioAf5);
}

void App_CanInit(void)
{
    App_CanGpioInit();
  
    stc_can_init_config_t   stcCanInitCfg;
    stc_can_filter_t        stcFilter;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralCan, TRUE);
 
    stcCanInitCfg.stcCanBt.PRESC = 4 - 1;    // Divide by 4
    stcCanInitCfg.stcCanBt.SEG_1 = 16 - 2;   // SEG_1 = 14
    stcCanInitCfg.stcCanBt.SEG_2 = 9 - 2;    // SEG_2 = 8
    stcCanInitCfg.stcCanBt.SJW   = 2 - 1;    // SJW = 1

    stcCanInitCfg.stcWarningLimit.CanErrorWarningLimitVal = 16-1;
    stcCanInitCfg.stcWarningLimit.CanWarningLimitVal = 10;

    stcCanInitCfg.enCanRxBufAll  = CanRxNormal;
    stcCanInitCfg.enCanRxBufMode = CanRxBufNotStored;
    stcCanInitCfg.enCanSTBMode   = CanSTBFifoMode;

    CAN_Init(&stcCanInitCfg);

    //CAN filter configuration
    stcFilter.enAcfFormat = CanAllFrames;
    stcFilter.enFilterSel = CanFilterSel1;
    stcFilter.u32CODE     = 0x00000352;
    stcFilter.u32MASK     = 0x1FFFFFFF;
    CAN_FilterConfig(&stcFilter, TRUE);
    

    //Can Irq Enable
    CAN_IrqCmd(CanRxIrqEn, TRUE);
    EnableNvic(CAN_IRQn, IrqLevel0, TRUE);
}

void App_CanTransmit(uint32_t can_id, uint8_t *data, uint8_t dlc)
{
    stc_can_txframe_t stcTxFrame;
  
    stcTxFrame.StdID = can_id;
    stcTxFrame.Control_f.DLC = dlc;
    stcTxFrame.Control_f.IDE = 0;  
    stcTxFrame.Control_f.RTR = 0;  
 
    for (uint8_t u8Idx = 0; u8Idx < dlc; u8Idx++)
    {
        stcTxFrame.Data[u8Idx] = data[u8Idx];
    }
    CAN_SetFrame(&stcTxFrame);
    CAN_TransmitCmd(CanPTBTxCmd);
}

uint8_t
App_CanReceive(uint32_t id, uint8_t* data, uint8_t len)
{
    can_buff_t buff = {0};
    buff.id = id;
    buff.length = len;
    memcpy(buff.data, data, len);

//    ENQUEUE(g_can_queue, buff);
    
    
   if(id == 0x1806E54F)
    {        
      for(uint8_t i=0; i < len; i++)   // careful: use < not <=
       {
        g_cell_setting_candata[i] = data[i];
       }    
    }
    
    if(id == 0x1806E55F)
    {
       for(uint8_t i=0; i < len; i++)   // careful: use < not <=
       {
        g_temp_setting_candata[i] = data[i];
       } 
    }
    
    if(id == 0x1806E57F)
    { 
       for(uint8_t i=0; i < len; i++)   // careful: use < not <=
       {
        g_current_setting_candata[i] = data[i];
       }           
    }
    
   if(id == 0x1806E56F)  // for  cell balnce settings
    { 
       for(uint8_t i=0; i < len; i++)   // careful: use < not <=
       {
        g_cellBalance_setting_candata[i] = data[i];
       }           
    }
    
    if(id == 0x1806E58F)  // for  cell balnce settings
    { 
       for(uint8_t i=0; i < len; i++)   // careful: use < not <=
       {
        g_bmsinfo_setting_candata[i] = data[i];
       }           
    }
    
    
    if(id == 0x1806E59F)  // for  cell balnce settings
    { 
       for(uint8_t i=0; i < len; i++)   // careful: use < not <=
       {
        g_cell_recovery_candata[i] = data[i];
       }           
    } 
    
    
    if(id == 0x60F)
    {
      can_flag = 1;
    }
    else
    {
     // App_CanThresholdSetting();
    }

       
    return 0;
}
      
void 
App_CanMessageHook(can_frame_info_t *p_can_frame)
{
  switch(p_can_frame -> u32_can_id)
    {
    case 0x1806E5F4:
    // _handle_bms_hb_response(p_can_frame -> u8_data, p_can_frame -> u8_dlc);
     break;
     default: break;
    }
}

void
App_CanFrameFromQueue(can_frame_info_t *pkt)  //   App_CanFrameFromQueue(&std_can_pdu);  call this in every 10ms;viz timer
{
    can_buff_t buff = { 0x00 };

    if (IS_EMPTY(g_can_queue))
    {
        return;
    }
    DEQUEUE(g_can_queue, buff);
    memcpy(pkt -> u8_data, buff.data, buff.length);
    pkt -> u32_can_id = buff.id;
    pkt -> u8_dlc = buff.length;
    
    g_can_conneted++;
    CAN_ID = pkt->u32_can_id;
    
//    if(CAN_ID == 0x1806E54F)
//    {        
//      for(uint8_t i=0; i < pkt->u8_dlc; i++)   // careful: use < not <=
//       {
//        g_cell_setting_candata[i] = pkt->u8_data[i];
//       }    
//    }
//    
//    if(CAN_ID == 0x1806E55F)
//    {
//       for(uint8_t i=0; i < pkt->u8_dlc; i++)   // careful: use < not <=
//       {
//        g_temp_setting_candata[i] = pkt->u8_data[i];
//       } 
//    }
//    
//    if(CAN_ID == 0x1806E57F)
//    { 
//       for(uint8_t i=0; i < pkt->u8_dlc; i++)   // careful: use < not <=
//       {
//        g_current_setting_candata[i] = pkt->u8_data[i];
//       }           
//    }
//    
//   if(CAN_ID == 0x1806E56F)  // for  cell balnce settings
//    { 
//       for(uint8_t i=0; i < pkt->u8_dlc; i++)   // careful: use < not <=
//       {
//        g_cellBalance_setting_candata[i] = pkt->u8_data[i];
//       }           
//    }
//    
//    
////bms id = 0x1234
////battery typ = 'LFP'/'NMC'
////noof cell = 16
////battery ah = 1000
//    
//    if(CAN_ID == 0x1806E58F)  // for  cell balnce settings
//    { 
//       for(uint8_t i=0; i < pkt->u8_dlc; i++)   // careful: use < not <=
//       {
//        g_bmsinfo_setting_candata[i] = pkt->u8_data[i];
//       }           
//    }
//    
//    
//    if(CAN_ID == 0x1806E59F)  // for  cell balnce settings
//    { 
//       for(uint8_t i=0; i < pkt->u8_dlc; i++)   // careful: use < not <=
//       {
//        g_cell_recovery_candata[i] = pkt->u8_data[i];
//       }           
//    }   
}

void 
CAN_IRQHandler(void)
{
    stc_can_rxframe_t  stcRxFrame;
    
    if(TRUE == CAN_IrqFlgGet(CanRxIrqFlg))
     {
        CAN_IrqFlgClr(CanRxIrqFlg);
        CAN_IrqCmd(CanRxIrqEn, FALSE);
        CAN_Receive(&stcRxFrame);
        App_CanReceive(stcRxFrame.ExtID,stcRxFrame.Data,stcRxFrame.Cst.Control_f.DLC);
        CAN_IrqCmd(CanRxIrqEn, TRUE);
     }
    // App_CanFrameFromQueue(&std_can_pdu);
}

 



 