#include "mcal_adc_interface.h"

#define VREF        3.3f
#define ADC_MAX     4095.0f
#define R_FIXED     10000.0f  // 10kO
#define R0          10000.0f  // Thermistor resistance at T0
#define T0_K        298.15f   // 25°C in Kelvin
#define BETA        3950.0f   // From thermistor datasheet
 

volatile uint32_t u32AdcRestultAcc;
float temperatureC[10];
uint32_t u32adcresult[5];

 

void ADC_DAC_IRQHandler(void)
{    
    if(TRUE == Adc_GetIrqStatus(AdcMskIrqSqr))
    {
        Adc_ClrIrqStatus(AdcMskIrqSqr);
       
      for(uint8_t i = 0; i<=4; i++)
      {
        u32adcresult[i] = Adc_GetSqrResult(i);  
      }

      
      get_temp(u32adcresult, temperatureC);
        Adc_SQR_Start();  // <-- restart scan
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
    delay10us(2);
    
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

void get_temp(uint32_t u32adcresult[5], float tempC[5])
{
    for (int i = 0; i < 5; ++i)
    {
        uint32_t adcValue = u32adcresult[i];
        float voltage = (adcValue * VREF) / ADC_MAX;
        float R_ntc = (voltage * R_FIXED) / (VREF - voltage);

        float temperatureK = 1.0f / ((1.0f / T0_K) + (1.0f / BETA) * log(R_ntc / R0));
        tempC[i] = (temperatureK - 273.15f)*100;
    }
}