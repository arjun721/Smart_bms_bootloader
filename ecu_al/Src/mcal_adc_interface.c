#include "mcal_adc_interface.h"
#include "mcal_sysctrl_interface.h"

volatile uint32_t u32AdcRestultAcc;
uint8_t u8tempflag;
uint32_t u32adcresult[5];
int16_t temperatureC[5];
int16_t gettempC[7];
int8_t g_externalTempValue[5];
extern struct oz3717_chip oz3717_data;
 
void ADC_DAC_IRQHandler(void)
{    
    if(TRUE == Adc_GetIrqStatus(AdcMskIrqSqr))
    {     
       u8tempflag = 1;
       
       Adc_ClrIrqStatus(AdcMskIrqSqr);
    }
}

void App_AdcPortInit(void)
{    
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
  Gpio_SetAnalogMode(BMS_EXT_NTC1_PORT, BMS_EXT_NTC1_PIN);      
  Gpio_SetAnalogMode(BMS_EXT_NTC2_PORT, BMS_EXT_NTC2_PIN);       
  Gpio_SetAnalogMode(BMS_EXT_NTC3_PORT, BMS_EXT_NTC3_PIN);         
  Gpio_SetAnalogMode(BMS_EXT_NTC4_PORT, BMS_EXT_NTC4_PIN);       
  Gpio_SetAnalogMode(BMS_INT_NTC_PORT, BMS_INT_NTC_PIN);       
}


void App_AdcInit(void)
{
  
    App_AdcPortInit();
  
    stc_adc_cfg_t              stcAdcCfg;

    DDL_ZERO_STRUCT(stcAdcCfg);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE); 
    
    //Bgr_BgrEnable();    
    M0P_BGR->CR |= 0x1u;
    
    stcAdcCfg.enAdcMode         = AdcScanMode;              ///<????-??
    stcAdcCfg.enAdcClkDiv       = AdcMskClkDiv1;            ///<????-1
    stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle8Clk;      ///<?????-8
    stcAdcCfg.enAdcRefVolSel    = AdcMskRefVolSelAVDD;      ///<??????-VCC
    stcAdcCfg.enAdcOpBuf        = AdcMskBufDisable;         ///<OP BUF??-?
    stcAdcCfg.enInRef           = AdcMskInRefDisable;       ///<????????-?
    stcAdcCfg.enAdcAlign        = AdcAlignRight;               ///<????????-?
    Adc_Init(&stcAdcCfg);
}

void App_AdcSQRCfg(void)
{
    stc_adc_sqr_cfg_t          stcAdcSqrCfg;
    
    DDL_ZERO_STRUCT(stcAdcSqrCfg);
 
    stcAdcSqrCfg.bSqrDmaTrig = FALSE;
    stcAdcSqrCfg.enResultAcc = AdcResultAccDisable;
    stcAdcSqrCfg.u8SqrCnt    = 5;
    Adc_SqrModeCfg(&stcAdcSqrCfg);

 
    Adc_CfgSqrChannel(AdcSQRCH0MUX, AdcExInputCH14);//PC4 //ok
    Adc_CfgSqrChannel(AdcSQRCH1MUX, AdcExInputCH15);//PC5  //ok
    Adc_CfgSqrChannel(AdcSQRCH2MUX, AdcExInputCH17);//PB10
    Adc_CfgSqrChannel(AdcSQRCH3MUX, AdcExInputCH18);//PB11
     
    Adc_CfgSqrChannel(AdcSQRCH4MUX, AdcExInputCH1);//PA1
    
    Adc_EnableIrq();
    EnableNvic(ADC_DAC_IRQn, IrqLevel3, TRUE);
 
    Adc_SQR_Start();
}    

 

void App_GetTempPoll(void)
{ 
  if(u8tempflag == 1)
  {
      for(uint8_t i = 0; i < 5; i++)
      {
        u32adcresult[i] = Adc_GetSqrResult(i);   
      }
      App_AdcTemp(u32adcresult, gettempC);
      u8tempflag = 0;
      Adc_SQR_Start();  // <-- restart scan
  }
}


 void App_GetTemp(int16_t* temp)
{
   for(uint8_t i =0; i< 5; i++)
   {
    temp[i] = temperatureC[i];
   }

}

void App_AdcTemp(uint32_t u32adcresult[5], int16_t* tempC)
{
    for (int i = 0; i < 5; i++)
    {
        uint32_t adcValue = u32adcresult[i];
         if(adcValue > 4000)
         {
            adcValue = 0;
            float voltage = (adcValue * VREF) / ADC_MAX;
            float R_ntc = (voltage * R_FIXED) / (VREF - voltage);

            float temperatureK = 1.0f / ((1.0f / T0_K) + (1.0f / BETA) * log(R_ntc / R0_temp));
            tempC[i] = (temperatureK - 273.15f)*100;
            g_externalTempValue[i] = (temperatureK - 273.15f);
         }
         else
         {  
            float voltage = (adcValue * VREF) / ADC_MAX;
            float R_ntc = (voltage * R_FIXED) / (VREF - voltage);

            float temperatureK = 1.0f / ((1.0f / T0_K) + (1.0f / BETA) * log(R_ntc / R0_temp));
            tempC[i] = (temperatureK - 273.15f)*100;
            g_externalTempValue[i] = (temperatureK - 273.15f);
         }
    }
}