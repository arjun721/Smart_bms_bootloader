/*****************************************************************************
* Copyright(c) O2Micro, 2019. All rights reserved.
*       
* O2Micro [OZ3717] Source Code Reference Design
* File: es_oz3717.c
*       
* This Source Code Reference Design for O2MICRO [OZ3717] access 
* ("Reference Design") is solely for the use of PRODUCT INTEGRATION REFERENCE ONLY, 
* and contains confidential and privileged information of O2Micro International 
* Limited. O2Micro shall have no liability to any PARTY FOR THE RELIABILITY, 
* SERVICEABILITY FOR THE RESULT OF PRODUCT INTEGRATION, or results from: (i) any 
* modification or attempted modification of the Reference Design by any party, or 
* (ii) the combination, operation or use of the Reference Design with non-O2Micro 
* Reference Design. Use of the Reference Design is at user's discretion to qualify 
* the final work result.
*****************************************************************************/
/*****************************************************************************
 *	Include Section
 *	add all #include here
 *****************************************************************************/
#define DEBUG 0

#include "OZ3717_regdef.h"
#include "es_OZ3717.h"
#include "lut_therm.h"
 
#if DEBUG
#include <stdio.h>
#endif

#define AUTO_SCAN 1
#define CC_ALWAYS_ENABLE 1

uint16_t cell_volt[17];
uint16_t status1x;
uint16_t fetCtrlx;
float  system_currentx;
extern float temperatureC[10];

struct oz3717_chip oz3717_data;


/*****************************************************************************
 * Define section
 * add all #define here
 *****************************************************************************/
#define	RETRY_TIMEOUT					1000 

/******************************************************************************
 * Function prototype section
 * add prototypes for all functions called by this file,execept those 
 * declared in header file
 *****************************************************************************/
void DelayMS(unsigned int ms);

void Delay(unsigned int ms)
{
	if (ms)
		DelayMS(ms);
		
}


uint8_t spi_access(uint8_t bout_number_bytes,uint8_t * dataout,uint8_t bin_number_bytes,uint8_t * datain)
{
    uint8_t i;
    uint8_t rx;
    uint8_t total = (bout_number_bytes > bin_number_bytes) ? bout_number_bytes : bin_number_bytes;

    Spi_SetCS(M0P_SPI1, FALSE);  // Assert chip select

    for (i = 0; i < total; i++)
    {
        uint8_t tx = (i < bout_number_bytes) ? dataout[i] : 0xFF;
        rx = Spi_RWByte(M0P_SPI1, tx);

        if (i >= (total - bin_number_bytes) && datain != NULL) {
            datain[i - (total - bin_number_bytes)] = rx;
        }

        if (rx == 0xFF && tx == 0xFF) {
            // Could optionally treat this as a timeout/error
            Spi_SetCS(M0P_SPI1, TRUE);
            return ES_SPI_STATUS_TIMEOUT;
        }
    }
    Spi_SetCS(M0P_SPI1, TRUE);  // Deassert chip select

    return 0;
}

 
void DelayMS(unsigned int ms)
{
  //  delay1ms(100);
}

/****************************************************************************
 * Description:
 *		SPI Access function
 * Parameters:
 *      bout_number_bytes:		Number of data to send out
 *		dataout: 				pointer to buffer to store out data
 *      bin_number_bytes:       Number of data to receive
 *      datain :                point to buffer to receive data
 * Return:
 *		0	
 ****************************************************************************/

//-----------------------------------------------------------------------------
// SPI Access Function Implement
//-----------------------------------------------------------------------------

/*****************************************************************************
 * global variables section
 * add declaration of global variables here
 * e.g. 
 *	int8_t foo;
 ****************************************************************************/
 
 
/*****************************************************************************
 * local variables section
 * define local variables(will be refered only in this file) here,
 * static keyword should be used to limit scope of local variable to this file
 *****************************************************************************/



/****************************************************************************
 *
 *			I N I T I A L 	M O D U L E
 *
 ****************************************************************************/
/*****************************************************************************
 * Description:
 *		Initial module for MCU using.
 * Parameters:
 *		N/A
 *		
 * Return: 0
 *		
 *****************************************************************************/
uint8_t o2_es_init()
{
// 0. Before enter this function, should initial SPI bus first. SPI MODE 3, up to 1Mhz.
// 1. Write protection area (oz3717_init)
// 2. Save data in Afe Device
// 3. Change power mode to normal
// 4. Change sacn mode to auto sacn

	uint8_t ret = 0;
	uint16_t bufdata = 0;
	uint16_t buftmp = 0;

	
	ret = afe_register_read(OZ3717_REG_HWVN, 1,&bufdata);
	
	//Set to Normal power mode
	ret = o2m_register_read(OZ3717_PWRMD, &bufdata);
  if((bufdata & OZ3717_PWRMD_ACTIVE_MODE)==0)
	{
		//
	//	ret = afe_register_read(buftmp, 1,&bufdata);
#if DEBUG		
		printf("Set to normal mode\r\n");
#endif		
		o2m_register_write(OZ3717_UNLOCK,OZ3717_PATTERN);		
		o2m_register_write(OZ3717_PWRMD,OZ3717_PATTERN);		
		oz3717_PowerMode(OZ3717_PWRMD_MODE_ACTIVE);
		
#if AUTO_SCAN		
		o2m_register_write(OZ3717_AUTOSCAN,OZ3717_SW_MAPPING_REQ|OZ3717_AUTO_SCAN_SAVE|OZ3717_AUTO_SCAN_EIGHT|OZ3717_AUTO_SCAN_ENABLE);	
#endif
		
		o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_CHG_ENABLE|OZ3717_FETCTRL_DSG_ENABLE);			
		oz3717_init();
		o2m_register_write(OZ3717_UNLOCK,0);		
	}else
	{
		o2m_register_write(OZ3717_UNLOCK,OZ3717_PATTERN);	
        oz3717_init();
#if AUTO_SCAN		
		o2m_register_write(OZ3717_AUTOSCAN,OZ3717_SW_MAPPING_REQ|OZ3717_AUTO_SCAN_SAVE|OZ3717_AUTO_SCAN_EIGHT|OZ3717_AUTO_SCAN_ENABLE);	
#endif		
		o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_CHG_ENABLE|OZ3717_FETCTRL_DSG_ENABLE);			
		
		o2m_register_write(OZ3717_UNLOCK,0);		
		
	}
#if	CC_ALWAYS_ENABLE
	
		ret =  o2m_register_write(OZ3717_REG_CADCTRL, CADCTRL_CC_ENABLE);
#else

ret =  o2m_register_write(OZ3717_REG_CADCTRL, 0);
		
	
#endif	
	//Status update
	ret = o2m_register_read(OZ3717_STATUS, &bufdata);
	oz3717_data.status1 = bufdata;
	if(bufdata & OZ3717_SPIWDT_FLAG)
	{
	//	Set to Normal power mode when SPI watchdog event
	//	afe_PowerMode(OZ3717_PWRMD_MODE_NORMAL);
	}
	//Action for status
	//User defined
	//Clear status,
if(bufdata != 0)	
	o2m_register_write(OZ3717_STATUS,bufdata);

#if DEBUG
	buftmp = 0x4F;

	//Dump 0x4F~55 user config data
//	while(1)
//	{

//	ret = o2m_register_read(buftmp, &bufdata);
//	printf("REG 0x%X = 0x%4.4X\r\n",buftmp,bufdata);
//		buftmp++;
//		if(buftmp == 0x56)
//			break;
//	}

	
#endif

	return ret;
}


/****************************************************************************
 *
 *			E N T R Y 	M O D U L E
 *
 ****************************************************************************/
/*****************************************************************************
 * Description:
 *		Entry module for MCU using.
 * Parameters:
 *		N/A
 *		
 * Return: 0
 *		
 *****************************************************************************/
uint8_t o2_es_entry(void)
{
// 0. May called every 1 second
// 1. Update thm 0~2 and internal temperature
// 2. Update Max, Min cell voltage.
// 3. Update Current
// 4. Update fet status
// 5. Update Status
	uint8_t ret = 0;
	uint16_t bufdata = 0;
	float ftmpdata = 0;

	
	//Set to Normal power mode
	ret = o2m_register_read(OZ3717_PWRMD, &bufdata);
  if(!(bufdata & OZ3717_PWRMD_ACTIVE_MODE))
	{
    
#if DEBUG		
		//printf("Set to normal mode\r\n");
#endif		
		o2m_register_write(OZ3717_UNLOCK,OZ3717_PATTERN);		
		o2m_register_write(OZ3717_PWRMD,OZ3717_PATTERN);		
		oz3717_PowerMode(OZ3717_PWRMD_MODE_ACTIVE);
		o2m_register_write(OZ3717_AUTOSCAN,OZ3717_SW_MAPPING_REQ|OZ3717_AUTO_SCAN_SAVE|OZ3717_AUTO_SCAN_EIGHT|OZ3717_AUTO_SCAN_ENABLE);	
		o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_CHG_ENABLE|OZ3717_FETCTRL_DSG_ENABLE);			
		o2m_register_write(OZ3717_UNLOCK,0);		
	}

	// 1. update temperature
	oz3717_read_temperature();

	//2. Update Max, Min cell, and cell voltage
	
	ret = oz3717_read_max_volt_cell(&oz3717_data.which_cell_max,&oz3717_data.max_cell_data);

	ret = oz3717_read_min_volt_cell(&oz3717_data.which_cell_min,&oz3717_data.min_cell_data);
	
	
	ret =  o2m_register_read(OZ3717_AUTOSCAN, &bufdata);
  
	if(bufdata & OZ3717_AUTO_SCAN_ENABLE)
	{
    // Auto-scan on
    for (uint8_t i = 0; i < CELL_NUM - 1; i++)
    {
        ret = oz3717_read_cell_volt_ascan(i + 1, &oz3717_data.cell_volt[i]);
       cell_volt[i] = oz3717_data.cell_volt[i];
    }

    // Last cell must be cell 17
    ret = oz3717_read_cell_volt_ascan(17, &oz3717_data.cell_volt[CELL_NUM - 1]);
    cell_volt[16] = oz3717_data.cell_volt[16];

    ret = oz3717_read_pack_current_ascan(&oz3717_data.system_current);
    system_currentx = oz3717_data.system_current;

    // printf("pack current value = %f mA\r\n", oz3717_data.system_current);
		//Auto-scan on 
//		for(bufdata = 0;bufdata<CELL_NUM-1 ; bufdata++)
//    
//			ret = oz3717_read_cell_volt_ascan(bufdata+1,&oz3717_data.cell_volt[bufdata]);
//		
//		//Last Cell must be cell 17
//					ret = oz3717_read_cell_volt_ascan(17,&oz3717_data.cell_volt[CELL_NUM-1]);


//		ret = oz3717_read_pack_current_ascan(&oz3717_data.system_current);

	
			//printf("pack current value = %f mA\r\n",oz3717_data.system_current);	
	}
	else
	{//auto sacn off
		for(bufdata = 0;bufdata<CELL_NUM-1 ; bufdata++)
			ret = oz3717_read_cell_volt_tscan(bufdata+1,&oz3717_data.cell_volt[bufdata]);

		//Last Cell must be cell 17
					ret = oz3717_read_cell_volt_tscan(17,&oz3717_data.cell_volt[CELL_NUM-1]);		
		
		ret = oz3717_read_pack_current_tscan(&oz3717_data.system_current);

	
			//printf("pack current value = %f mA\r\n",oz3717_data.system_current);			
	}
	
	o2m_register_read(OZ3717_REG_CADCTRL,&bufdata);			
  //3. Read CADC value according to CC_EANBLE by using different method
	if(bufdata & CADCTRL_CC_ENABLE)
	{
	
	oz3717_read_cadc_current_conse(&oz3717_data.system_current);
		oz3717_read_coulomb_count(&ftmpdata);
			//printf("CC value = %f mAh\r\n",ftmpdata);	
	}
	else
	{
		oz3717_read_cadc_current_trigger(&oz3717_data.system_current);
	}

  // Update CC Enable according to CADC value and	CHG/DSG threshold

	oz3717_coulomb_count_setting(oz3717_data.system_current,CHG_TH,(-1)*DSG_TH);
	
	
	//4. Fet and CHG/DSG update, (included in register cbsel1)
	ret = oz3717_read_balance_status(&oz3717_data.cbsel1, &oz3717_data.cbsel2);
	o2m_register_read(OZ3717_FETCTRL,&oz3717_data.fetCtrl);		
    fetCtrlx = oz3717_data.fetCtrl;	

	//5. Status update
	 oz3717_read_status(&oz3717_data.status1 , &oz3717_data.status2 );
   status1x = oz3717_data.status1;
	
	if(oz3717_data.status1 & OZ3717_SPIWDT_FLAG)
	{
		//Set to Normal power mode when SPI watchdog event
	
	}
	//Action for status
	//User defined
	//Clear status, 
	o2m_register_write(OZ3717_STATUS,oz3717_data.status1);
	//7. Cell balance fucntion
	oz3717_cb_function();

#if DEBUG
		printf("Cell Numbers = %d \r\n",CELL_NUM);
	
		for(bufdata=0;bufdata < CELL_NUM; bufdata++)
		printf("%d mV, ",oz3717_data.cell_volt[bufdata]);	
		
	  printf("\r\n");	
	
	
		printf("Highest Cell = %d mV, Cell Number = %d\r\n",oz3717_data.max_cell_data,oz3717_data.which_cell_max);	
		printf("Lowest Cell = %d mV, Cell Number = %d\r\n",oz3717_data.min_cell_data,oz3717_data.which_cell_min);	
		

		printf("FETCTRL = 0x%4.4x\r\n",oz3717_data.fetCtrl);	
		
		//Print external tmp
		printf("THM0 = %d 0.1C\r\n",oz3717_data.external_temperature[0]-2730);	
		printf("THM1 = %d 0.1C\r\n",oz3717_data.external_temperature[1]-2730);	
		printf("THM2 = %d 0.1C\r\n",oz3717_data.external_temperature[2]-2730);		
		//Print internal tmp
		printf("Internal temperature = %d 0.1C\r\n",oz3717_data.internal_temperature-2730);	
		
		
	   //CHG/DSG
		printf("fet Status = 0x%4.4X\r\n",oz3717_data.cbsel1);	
		if(oz3717_data.cbsel1 & OZ3717_IN_DSG_STATE)
			printf("In DISCHARGE\r\n");
		else if(oz3717_data.cbsel1 & OZ3717_IN_CHG_STATE)
			printf("In CHARGE\r\n");
		else
			printf("In IDLE\r\n");
		//Print current
		printf("Current = %f mA\r\n",oz3717_data.system_current);	
			
			
		//Print status
		printf("Status1 = 0x%4.4X\r\n",oz3717_data.status1);		
		printf("Status2 = 0x%4.4X\r\n",oz3717_data.status2);
#endif
	
	return ret;
}

/****************************************************************************
 *
 *			R E G I S T E R		C O N T R O L		M O D U L E
 *
 ****************************************************************************/

 /****************************************************************************
 * Description:
 *		read OZ3717 cmd 
 * Parameters:
 *		cmd:		OZ3717 command: 0x3n->Register
 *      index:		OZ3717 memory address index 
 *      read_number:		Number of data to read 
 *		buf: 		pointer to buffer to store read data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t spi_read_cmd(uint8_t cmd, uint8_t index, uint8_t read_number,uint16_t *buf)
{
	uint8_t ret=0;
	uint8_t btmp=0;
	uint8_t pec_cal = 0;


	uint8_t bdata_buf[6];
	uint8_t bdata_buf_read[33+3];
	

	if(read_number > SPI_READ_DATA_MAX)
	return ES_SPI_STATUS_READ_DATA_NUMBER_ERROR;

	if(read_number == 0)
	return ES_SPI_STATUS_READ_DATA_NUMBER_ERROR;


	bdata_buf[0] = cmd | (read_number-1);
	bdata_buf[1] = index;


	//Same Read/write buffer lentgh write with 2bytes, read 2n+1
	ret = spi_access(read_number*2+1+2,bdata_buf,read_number*2+1+2,bdata_buf_read);
	
	pec_cal = crc8_calc(bdata_buf_read, read_number*2+2);
	
	
	if(pec_cal != bdata_buf_read[read_number*2+2]){ 
		return ES_SPI_STATUS_PECERR;
	}

	if(buf != NULL)
	{
		for(btmp=0;btmp<read_number;btmp++)
		{
			//Index 0 store command, index 1 store index
			buf[btmp] = bdata_buf_read[btmp*2+2] << 8;
			buf[btmp] |= bdata_buf_read[btmp*2+1+2];
	
		}

	}


	return ret;
	
}


/****************************************************************************
 * Description:
 *		write OZ3717 cmd word
 * Parameters:
 *		cmd:		OZ3717 command: 0xC5->Register
 *      index:		OZ3717 memory address index 
 *		buf: 		pointer to buffer to store write data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t spi_write_cmd( uint8_t cmd,uint8_t index, uint16_t buf)
{
	uint8_t ret = 0;
	uint8_t bdata_buf[8];
	
	bdata_buf[0] = cmd;
	bdata_buf[1] = index;
	bdata_buf[2] = (uint8_t)(buf>>8);
	bdata_buf[3] = (uint8_t)buf;
	
	bdata_buf[4] = crc8_calc(bdata_buf, 4);
	
	ret = spi_access(5,bdata_buf,0,bdata_buf);
	
	
	return ret;
}



/****************************************************************************
 * Description:
 *		read OZ3717 operation register
 * Parameters:
 *      index:		OZ3717 operation register index
 *		read_number: 		Number of read data (Max 15)
 *		buf: 		pointer to buffer to store read data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t afe_register_read(uint8_t index, uint8_t read_number,uint16_t *buf)
{
	uint8_t ret;
	ret = spi_read_cmd(RD_REG_CMD, index, read_number, buf);
	return ret;
}

/****************************************************************************
 * Description:
 *		read OZ3717 operation register
 * Parameters:
 *      index:		OZ3717 operation register index
 *		buf: 		pointer to buffer to store read data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t o2m_register_read(uint8_t index,uint16_t *buf)
{
	uint8_t ret;
	ret = spi_read_cmd(RD_REG_CMD, index, 1, buf);
	return ret;
}

/****************************************************************************
 * Description:
 *		write OZ3717 operation register
 * Parameters:
 *    index:		OZ3717 operation register index
 *		buf: 		the data we write to register
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t o2m_register_write(uint8_t index, uint16_t buf)
{
	uint8_t ret;
	ret = spi_write_cmd(WR_REG_CMD, index, buf);
	return ret;
}



/****************************************************************************
 * Description:
 *		write OZ3717 operation register
 * Parameters:
 *      index:		OZ3717 operation register index
 *		buf: 		the data we write to register
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t afe_register_write(uint8_t index, uint16_t buf)
{
	uint8_t ret;
	ret = spi_write_cmd(WR_REG_CMD, index, buf);
	return ret;
}


/*****************************************************************************
 * Description:
 *		calculate PEC check sum. it's based on CRC-8 poly x^8 + x^2 + x + 1
 * Parameters:
 *		pdata:	Data array for PEC calculation.
 *		n:	Data length to be calculated.
 * Return:
 *		the crc result
 *****************************************************************************/
uint8_t crc8_calc(uint8_t * pdata, uint16_t n)
{
    uint8_t crc = 0;
    uint8_t crcdata;
    uint16_t i, j;

    for (i = 0; i < n; i++)
    {
        crcdata = pdata[i];
        for (j = 0x80; j != 0; j >>= 1)
        {
            if ((crc & 0x80) != 0)
            {
                crc <<= 1;
                crc ^= 0x07;
            }
            else
                crc <<= 1;

						if ((crcdata & j) != 0)

						crc ^= 0x07;
				}        
    }            
		return crc;
}


/*****************************************************************************
 * Description:
 *		EAGLE Senior version 
 * Parameters:
 *		N/A
 *		
 * Return:
 *		EAGLE Senior version for OZ3717
 *****************************************************************************/

uint16_t o2_es_version(void)
{
	return ES_OZ3717_VER;
}

/*****************************************************************************
 * Description:
 *		EAGLE Senior ID for OZ3717
 * Parameters:
 *		N/A
 *		
 * Return:
 *		EAGLE Senoir for OZ3717 id
 *****************************************************************************/

uint16_t o2_es_id(void)
{
	return AFE_ID_OZ3717;
}

/****************************************************************************
 *
 *			A P P L I C A N T I O N 	M O D U L E
 *
 ****************************************************************************/

/****************************************************************************
 * Description:
 *		OZ3717 user configuration setting
 * Parameters:
 *      None
 *		
 *		
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_init(void)
{
	uint8_t ret = 0;
	uint16_t ustmp = 0;

	// 0x4F,UVSTDN REG write
	ustmp = UVSTDN_TH_REG | UVSTDN_DLY_REG;
	ret =  o2m_register_write(OZ3717_UVSTDN, ustmp);
	
	// 0x50,OVP REG write
	ustmp = OVP_DLY_REG | OVP_TH_REG | OVP_REL_REG;
	ret =  o2m_register_write(OZ3717_OVP, ustmp);

	//0x52,COCP REG write
	ustmp = TYPE_CFG_REG | COCP_DLY_REG | COCP_TH_REG;
	ret =  o2m_register_write(OZ3717_COCP, ustmp);

	//0x52,DOC2SC REG write
	ustmp = SCPTH_REG | INT_TYPE_REG |DOC2P_DLY_REG|DOC2P_TH_REG | SCPP_DLY_REG;// (BYTE)COCP_TH_REG;
	ret =  o2m_register_write(OZ3717_DOC2SC, ustmp);

	//0x53,SPI Watchdog
  ustmp = (SPIWDT_REG & 0xFF);	
	if(ustmp != 0)
	{
	ret =  o2m_register_write(OZ3717_SPIWDT, ustmp);
	}

	//0x54,IDLE TH
	ustmp = DSG_TH_REG | CHG_TH_REG;
	ret =  o2m_register_write(OZ3717_IDLE_TH, ustmp);

	//0x51,DOC1P REG write
	ustmp = CELL_NUMBER_REG | EFETC_REG |DOC1P_DLY_REG | DOC1P_TH_REG;
	ret =  o2m_register_write(OZ3717_DOC1P, ustmp);
  
  	//0x5D,IE REG write
  ustmp =  OZ3717_ALL_IE;
	 ret =  o2m_register_write(OZ3717_IE, ustmp);
 
	return ret;
	
}

/****************************************************************************
 * Description:
 *		read OZ3717 status
 * Parameters:
 *    status1:	pointer to buffer to store status1 
 *		status2: 	pointer to buffer to store status2
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_read_status(uint16_t *status1, uint16_t *status2)
{
	uint8_t ret=0;
	uint16_t ustmp;
	ret =  o2m_register_read(OZ3717_STATUS, &ustmp);
	*status1 = ustmp;
	
	ret =  o2m_register_read(OZ3717_STR2, &ustmp);
	*status2 = ustmp;
	
	return ret;
}


/****************************************************************************
 * Description:
 *		read OZ3717 Max Cell voltage
 * Parameters:
 *    cell_num:	pointer to buffer to store which channel is max cell voltage 
 *		volt: 		pointer to buffer to store cell voltage
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/

uint8_t oz3717_read_max_volt_cell(uint8_t *cell_num,uint16_t *volt)
{
	uint8_t ret=0;
	uint16_t ustmp;
	uint16_t reg_save;
	
	ret =  o2m_register_read(OZ3717_AUTOSCAN, &reg_save);
	
	ret =  o2m_register_write(OZ3717_AUTOSCAN, reg_save & 0xFE);
	//Read Max Cell
	ret =  o2m_register_read(OZ3717_REG_CXCN, &ustmp);
	*cell_num = (BYTE)(ustmp & 0x1F);
	ret =  o2m_register_read(OZ3717_REG_MAX_CELL, &ustmp);
	//Convert raw data to voltage
	*volt = (uint16_t )((float)ustmp*OZ3717_CELL_VOLTAGE_LSB);

	ret =  o2m_register_write(OZ3717_AUTOSCAN, reg_save);
	
    return ret;	 
}


/****************************************************************************
 * Description:
 *		read OZ3717 Min Cell voltage
 * Parameters:
 *    cell_num:	pointer to buffer to store which channel is min cell voltage 
 *		volt: 		pointer to buffer to store cell voltage
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/

uint8_t oz3717_read_min_volt_cell(uint8_t *cell_num,uint16_t *volt)
{
	uint8_t ret=0;
	uint16_t ustmp;	
	uint16_t reg_save;
	
	ret =  o2m_register_read(OZ3717_AUTOSCAN, &reg_save);
	
	ret =  o2m_register_write(OZ3717_AUTOSCAN, reg_save & 0xFE);
	
	//Read Min Cell
	ret =  o2m_register_read(OZ3717_REG_CXCN, &ustmp);
	*cell_num = (BYTE)(ustmp>>8 & 0x1F);
		
	ret =  o2m_register_read(OZ3717_REG_MIN_CELL, &ustmp);
	//Convert raw data to voltage
	*volt = (uint16_t )((float)ustmp*OZ3717_CELL_VOLTAGE_LSB);

	ret =  o2m_register_write(OZ3717_AUTOSCAN, reg_save);
	
	return ret;
}

/****************************************************************************
 * Description:
 *		read OZ3717 Cell voltage with auto-scan on
 * Parameters:
 *    channel:		OZ3717 channel number(1~17) 
 *		volt: 		pointer to buffer to store read data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_read_cell_volt_ascan(uint8_t channel,uint16_t *volt)
{
	uint8_t ret=0;
	uint16_t ustmp1,ustmp2;// = 0;

	
	if(channel <= 17)
	{
	
	   ret =  o2m_register_read(channel-1+0x01,&ustmp1);
		
	   ret =  o2m_register_read(channel-1+0x01,&ustmp2);

		if(ustmp1 != ustmp2)
	   ret =  o2m_register_read(channel-1+0x01,&ustmp2);
		
		
     //Convert raw data to voltage
	     *volt = (uint16_t )((float)ustmp2*OZ3717_CELL_VOLTAGE_LSB);

  }
	return ret;
}

/****************************************************************************
 * Description:
 *		read OZ3717 pack current with auto-scan on
 * Parameters:
 *		curr: 		pointer to buffer to store read data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_read_pack_current_ascan(float *curr)
{
	uint8_t ret=0;
	uint16_t ustmp1,ustmp2;// = 0;
	
	int16_t stmp;
	
	   ret =  o2m_register_read(0x12,&ustmp1);
		
	   ret =  o2m_register_read(0x12,&ustmp2);

		if(ustmp1 != ustmp2)
	   ret =  o2m_register_read(0x12,&ustmp2);
		
		stmp = (int16_t) (ustmp2);
		
     //Convert raw data to current
	     *curr = ((float)stmp*OZ3717_CURRENT_LSB)/(RSENSE/1000.0);


	
	return ret;
}

/****************************************************************************
 * Description:
 *		read OZ3717 cadc current with cc_always_enable on
 * Parameters:
 *		curr: 		pointer to buffer to store read data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_read_cadc_current_conse(float *curr)
{
	uint8_t ret=0;
	uint16_t ustmp1,ustmp2;// = 0;
	
	int16_t stmp;
	
	   ret =  o2m_register_read(OZ3717_REG_CMDATA,&ustmp1);
		
	   ret =  o2m_register_read(OZ3717_REG_CMDATA,&ustmp2);

		if(ustmp1 != ustmp2)
	   ret =  o2m_register_read(OZ3717_REG_CMDATA,&ustmp2);
		
		stmp = (int16_t) (ustmp2);
		
     //Convert raw data to current
	     *curr = ((float)stmp*OZ3717_CURRENT_LSB)/(RSENSE/1000.0);


	
	return ret;
}

/****************************************************************************
 * Description:
 *		read OZ3717 coulomb count data
 * Parameters:
 *		ccdata: 		pointer to buffer to store read data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t  oz3717_read_coulomb_count(float *cc_data)
{
	uint8_t ret=0;
	
	uint16_t ustmp1[4],ustmp2[4];
	unsigned int cc_raw_data;
	
	
	ret = afe_register_read(OZ3717_REG_CCH, 2,ustmp1);

	ret = afe_register_read(OZ3717_REG_CCH, 2,ustmp2);

	if((ustmp1[0] != ustmp2[0]) || (ustmp1[1] != ustmp2[1]))
	ret = afe_register_read(OZ3717_REG_CCH, 2,ustmp2);
		
	cc_raw_data = ustmp2[0] <<16;
	cc_raw_data |= ustmp2[1];
	
	//Convert to CC value mAh
	*cc_data = (int)cc_raw_data *0.5 /(RSENSE); //mAs
	*cc_data = *cc_data / 3600.0; //mAh


	
	return ret;
}

/****************************************************************************
 * Description:
 *		read OZ3717 pack current with auto-scan OFF
 * Parameters:
 *		curr: 		pointer to buffer to store read data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_read_pack_current_tscan(float *curr)
{
	uint8_t ret=0;
	uint16_t ustmp;
	uint8_t retry_cnt = 30;
	uint16_t reg_save;
	int16_t stmp;
	
	ret =  o2m_register_read(OZ3717_TRIG,&reg_save);
	
	ret =  o2m_register_write(OZ3717_STATUS,OZ3717_TRG_SCAN_FLAG);
	
	
	ret =  o2m_register_write(OZ3717_TRIG, OZ3717_TRIG_ENABLE | 0x12);
	do
	{
	
	 Delay(20);
	
	//Read again, until finished
	ret =  o2m_register_read(OZ3717_STATUS, &ustmp);
		retry_cnt--;
		if(retry_cnt == 0)
			break;
	}while((ustmp & OZ3717_TRG_SCAN_FLAG)==0);	
		
	ret =  o2m_register_read(0x12,&ustmp);
		
	stmp = (int16_t) (ustmp);
		
  //Convert raw data to current
	*curr = ((float)stmp*OZ3717_CURRENT_LSB)/(RSENSE/1000.0);


	//Restore trig data
	ret =  o2m_register_write(OZ3717_TRIG,reg_save);
	
	return ret;
}


/****************************************************************************
 * Description:
 *		read OZ3717 Cell voltage with auto-scan off
 * Parameters:
 *    channel:		OZ3717 channel number(1~17) 
 *		volt: 		pointer to buffer to store read data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_read_cell_volt_tscan(uint8_t channel,uint16_t *volt)
{
	uint8_t ret=0;
	uint16_t ustmp;
	uint8_t retry_cnt = 30;
	uint16_t reg_save;

	if(channel <= 17){
		
	
	ret =  o2m_register_read(OZ3717_TRIG,&reg_save);
	
	ret =  o2m_register_write(OZ3717_STATUS,OZ3717_TRG_SCAN_FLAG);
	
	ret =  o2m_register_write(OZ3717_TRIG, OZ3717_TRIG_ENABLE | 0x1F);
	do
	{
	
	 Delay(20);
	
	//Read again, until finished
	ret =  o2m_register_read(OZ3717_STATUS, &ustmp);
		retry_cnt--;
		if(retry_cnt == 0)
			break;
	}while((ustmp & OZ3717_TRG_SCAN_FLAG)==0);	
		
	ret =  o2m_register_read((0x01+channel-1),&ustmp);
		
  }
		
 //Convert raw data to voltage
    *volt = (uint16_t )((float)ustmp*OZ3717_CELL_VOLTAGE_LSB);

	//Restore trig data
	ret =  o2m_register_write(OZ3717_TRIG,reg_save);
	
	return ret;	
}


/****************************************************************************
 * Description:
 *		OZ3717 power mode change
 * Parameters:
 *      pmode:		OZ3717 power mode setting
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_PowerMode(uint16_t pmode)
{
	uint8_t ret;
	uint16_t ustmp = 0;
	
	ret =  o2m_register_read(OZ3717_PWRMD, &ustmp);
//Unlock power mode for write	
	ret =  o2m_register_write(OZ3717_PWRMD, OZ3717_PATTERN);
	Delay(5);

	ustmp &= ~0x10F;
	ustmp |= pmode;
	ret =  o2m_register_write(OZ3717_PWRMD, ustmp);

	
	return ret;
}

/****************************************************************************
 * Description:
 *		OZ3717 Pre-Disable enable function
 * Parameters:
 *     
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_PreDischarge_Enable(void)
{
	uint8_t ret;
	uint16_t ustmp = 0;
	
	ret =  o2m_register_read(OZ3717_FETCTRL, &ustmp);
//Disable DSG fet	
	ustmp &= ~OZ3717_FETCTRL_DSG_ENABLE;
	ret =  o2m_register_write(OZ3717_FETCTRL, ustmp);
	Delay(2);
	//Set pre-discharge, pre-sel = 0
	ustmp &= ~OZ3717_FETCTRL_SEL_PRE_MASK;
	ustmp |= OZ3717_FETCTRL_SEL_PRE_DISCHARGE;
	ret =  o2m_register_write(OZ3717_FETCTRL, ustmp);
	Delay(2);
	//Enable DSG
	ustmp |= OZ3717_FETCTRL_DSG_ENABLE;
	ret =  o2m_register_write(OZ3717_FETCTRL, ustmp);
	
	return ret;
}

/****************************************************************************
 * Description:
 *		OZ3717 Pre-Disable disable function
 * Parameters:
 *     
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_PreDischarge_Disable(void)
{
	uint8_t ret;
	uint16_t ustmp = 0;
	
	ret =  o2m_register_read(OZ3717_FETCTRL, &ustmp);
	//Set pre-discharge disable
	ustmp &= ~OZ3717_FETCTRL_SEL_PRE_MASK;
	ret =  o2m_register_write(OZ3717_FETCTRL, ustmp);
	Delay(2);
	
	return ret;
}

/****************************************************************************
 * Description:
 *		OZ3717 Pre-Discharge/charge step change function
 * Parameters:
 *     preset: 		Pre-Discharge/charge steps 100uV/step
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_Pre_Set_Modify(uint8_t bpreset)
{
	uint8_t ret;
	uint16_t ustmp = 0;
	
	ret =  o2m_register_read(OZ3717_FETCTRL, &ustmp);

	ustmp &= OZ3717_FETCTRL_PRE_SET_MASK;
	ustmp |= bpreset << 8;
	ret =  o2m_register_write(OZ3717_FETCTRL, ustmp);
	
	return ret;
}

/****************************************************************************
 * Description:
 *		read OZ3717 CADC data with CC_ALWAYS_ENABLE is OFF
 * Parameters:
 *		curr: 		CADC data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_read_cadc_current_trigger(float *curr)
{
	uint8_t ret;
	uint16_t ustmp = 0;
	
	uint8_t retry_cnt = 30;
	float ftmpdata = 0.0;
	
	ret =  o2m_register_write(OZ3717_STATUS, OZ3717_CADC_TRIGGER_FLAG);
	
	ret =  o2m_register_write(OZ3717_REG_CADCTRL, CADCTRL_TRIGGER_CADC |CADCTRL_HI_ACY_MODE |CADCTRL_CONSE_CADC_RDY);
	do
	{
	
	 Delay(20);
	
	//Read again, until finished
	ret =  o2m_register_read(OZ3717_STATUS, &ustmp);
		retry_cnt--;
		if(retry_cnt == 0)
		{
			return ES_SPI_STATUS_EETIMEOUT;
		}	
	}while((ustmp & OZ3717_CADC_TRIGGER_FLAG)==0);	

	ret =  o2m_register_read(OZ3717_REG_CTDATA, &ustmp);
	//Convert to current to mA
	ftmpdata = (int16_t)ustmp;
	*curr = (ftmpdata * OZ3717_REG_CADC_LSB)/(RSENSE/1000.0);


	
	return ret;
}
/****************************************************************************
 * Description:
 *		Set OZ3717 with CC_ALWAYS_ENABLE to ON/OFF according to current (From CADC)
 * Parameters:
 *		curr: 		CADC current .
 *		chg_threshold: 		Charge current threshold. (positve value)
 *		dsg_threshold: 		Discharge current threshold. (Negative value)
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_coulomb_count_setting(float curr,float chg_threshold,float dsg_threshold)
{
	uint8_t ret;
	uint16_t ustmp = 0;
	
	uint8_t cc_enable = 0;
	
	if(curr > chg_threshold)
		cc_enable =1;
	else if(curr < dsg_threshold)
		cc_enable =1;
	else 
		cc_enable =0;
	
	ret =  o2m_register_read(OZ3717_REG_CADCTRL, &ustmp);
	
	if(cc_enable)
		ustmp |= CADCTRL_CC_ENABLE;
	else
	{
		ustmp &= ~CADCTRL_CC_ENABLE;
		//Set sw_cadc_ctrl to 0
		ustmp &= ~0x03;
	}
	
	ret =  o2m_register_write(OZ3717_REG_CADCTRL, ustmp);
	return ret;
}


/****************************************************************************
 * Description:
 *		read OZ3717 CADC data with CC_ALWAYS_ENABLE is ON
 * Parameters:
 *		curr: 		CADC data (Posive value means charge current, Negative value means discharge current.
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_read_current_conse(float *curr)
{
	uint8_t ret;
	uint16_t ustmp = 0;
	
	uint8_t retry_cnt = 30;
	float ftmpdata = 0.0;
	
	ret =  o2m_register_write(OZ3717_STATUS, OZ3717_CADC_TRIGGER_FLAG);
	
	ret =  o2m_register_write(OZ3717_REG_CADCTRL, CADCTRL_CC_ENABLE|CADCTRL_TRIGGER_CADC |CADCTRL_HI_ACY_MODE |CADCTRL_CONSE_CADC_RDY);
	do
	{
	
	 Delay(20);
	
	//Read again, until finished
	ret =  o2m_register_read(OZ3717_STATUS, &ustmp);
		retry_cnt--;
		if(retry_cnt == 0)
		{
			break;
		}
	}while((ustmp & OZ3717_CADC_TRIGGER_FLAG)==0);	

	ret =  o2m_register_read(OZ3717_REG_CTDATA, &ustmp);
	//Convert to current to mA
	ftmpdata = (int16_t)ustmp;
	*curr = (ftmpdata * OZ3717_REG_CADC_LSB)/(RSENSE/1000.0);


	
	return ret;
}


/****************************************************************************
 * Description:
 *		read OZ3717 internal thermal temperature
 * Parameters:
 *		
 *		buf: 		pointer to buffer to store read data
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_internal_temperature_read(int16_t *buf)
{
	uint8_t ret = 0;
	uint16_t ustmp = 0;
	float Vt;
	ret =  o2m_register_read(OZ3717_REG_INTMP, &ustmp);
	Vt = ustmp * 0.3125/4;
	Vt = (Vt-1252.5)/4.345 +23;
	*buf = (int16_t)(Vt*10)+2730;
	return ret;
}

/****************************************************************************
 * Description:
 *		read all OZ3717 temperature 
 * Parameters:
 *		N/A
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_read_temperature(void)
{
	uint8_t ret;
	uint16_t ustmp = 0;
	uint16_t thmcrrt_store;
	signed int uitemp = 0;
	signed int temp = 0;
	uint16_t thmADC1[3] = {0},thmADC2[3] = {0};
	float ftmp = 0.0;
	uint16_t tmp_max=0,tmp_min=30000;

	uint16_t adc_data;
	float	adc_crrt;
	
	
	//Store thm crrt setting
	ret =  o2m_register_read(OZ3717_CRRTSEL, &ustmp);
	thmcrrt_store = ustmp;
	
	// Round 1 use 20uA
	//Set 20uA source
	ret =  o2m_register_write(OZ3717_CRRTSEL, OZ3717_CRRTSEL_20uA);
	
	//Clear trigger req
	ret =  o2m_register_write(OZ3717_STATUS, OZ3717_TRG_SCAN_FLAG);	
	//Request trigger
	ret =  o2m_register_read(OZ3717_TRIG, &ustmp);
  ustmp &= 0xff20; 
 ustmp |= (OZ3717_TRIG_ENABLE | OZ3717_TRIG_ONE | OZ3717_TRIG_CHANNEL_ALL);
	ret =  o2m_register_write(OZ3717_TRIG, ustmp);	


	//Wait for trig completed
	uitemp =100;
	do{
		Delay(1);
		ret =  o2m_register_read(OZ3717_STATUS, &ustmp);
		if(OZ3717_TRG_SCAN_FLAG & ustmp)
			break;
		uitemp --;
		if(uitemp == 0)
			break;
	}while(1);
	
	
	// Store round 1 (20uA) in thmADC1
	// Read 5 channels data in one SPI access
	ret =  afe_register_read(OZ3717_REG_THM0, 3,&thmADC1[0]);
	
	// Round 2 use 120uA
	//Set 120uA source
	ret =  o2m_register_write(OZ3717_CRRTSEL, OZ3717_CRRTSEL_120uA);
	
	//Clear trigger req
	ret =  o2m_register_write(OZ3717_STATUS, OZ3717_TRG_SCAN_FLAG);	
	//Request trigger
	ret =  o2m_register_read(OZ3717_TRIG, &ustmp);
  ustmp &= 0xff20; 
	ustmp |= (OZ3717_TRIG_ENABLE | OZ3717_TRIG_ONE | OZ3717_TRIG_CHANNEL_ALL);
	ret =  o2m_register_write(OZ3717_TRIG, ustmp);	

	//Wait for trig completed
	uitemp =100;
	do{
		Delay(1);
		ret =  o2m_register_read(OZ3717_STATUS, &ustmp);
		if(OZ3717_TRG_SCAN_FLAG & ustmp)
			break;
		uitemp --;
		if(uitemp == 0)
			break;
	}while(1);
	
	// Store round 2 (120uA) in thmADC2
	ret =  afe_register_read(OZ3717_REG_THM0, 3,&thmADC2[0]);
	
	//Convert to temperature
	for(uitemp=0;uitemp<3;uitemp++)
	{
		//User 120uA first with its raw data < 32700
		if( thmADC2[uitemp] < 32700)
		{
				adc_data = thmADC2[uitemp] ;
				adc_crrt = 120.0;
		}
		else //otherwise choose 20uA
		{
				adc_data = thmADC1[uitemp] ;
				adc_crrt = 20.0;
		}
		ftmp = (float)adc_data*1000.0*OZ3717_TEMP_VOLT_LSB/adc_crrt;
		oz3717_convert_temperature((unsigned int)ftmp,&temp);
		
		oz3717_data.external_temperature[uitemp] = temp;
		  if(temp > tmp_max)
				tmp_max = temp;
		
		  if(temp < tmp_min)
				tmp_min = temp;
			
	}
  temperatureC[6] = oz3717_data.external_temperature[0];
  temperatureC[7] = oz3717_data.external_temperature[1];
  temperatureC[8] = oz3717_data.external_temperature[2];

	//set thm crrt setting
	ret =  o2m_register_write(OZ3717_CRRTSEL, thmcrrt_store);

	//Read internal temperature
	oz3717_internal_temperature_read(&oz3717_data.internal_temperature);
  temperatureC[5] = oz3717_data.internal_temperature;
				
	return ret;
}

/****************************************************************************
 * Description:
 *		read OZ3717 Cell balance status
 * Parameters:
 *    cbsel1:	pointer to buffer to store cbsel1 
 *		cbsel2:	pointer to buffer to store cbsel2
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_read_balance_status(uint16_t *cbsel1, uint16_t *cbsel2)
{
	uint8_t ret=0;
	uint16_t ustmp;
	ret =  o2m_register_read(OZ3717_CBSEL1, &ustmp);
	*cbsel1 = ustmp;
	
	ret =  o2m_register_read(OZ3717_CBSEL2, &ustmp);
	*cbsel2 = ustmp;
	
	return ret;
}

/****************************************************************************
 * Description:
 *		OZ3717 cell balance function
 * Parameters:
 *		None
 *		
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
uint8_t oz3717_cb_function(void)
{
	uint8_t ret = 0;
	
	//We may stop cb function if we have protection event.
	
	
	if(oz3717_data.cbsel1 & OZ3717_IN_CHG_STATE){
		//CHARGE balance
		if(CELL_BALANCE_OPTION & CELL_BALANCE_CHARGE_ENABLE)
		{
			if ((oz3717_data.max_cell_data > CELL_BALANCE_START) && 
				  ((oz3717_data.max_cell_data-oz3717_data.min_cell_data)>CELL_BALANCE_DELTA))
			{
						ret = oz3717_cb_set(oz3717_data.which_cell_max);
#if DEBUG				
				printf("CB in charge, set n=%d\r\n",oz3717_data.which_cell_max);
#endif				
			}		else
			ret  = oz3717_cb_set(0);
		}
		else
			ret  = oz3717_cb_set(0);
			

		
	}else if(oz3717_data.cbsel1 & OZ3717_IN_DSG_STATE){
		//DISCHARGE
		ret = oz3717_cb_set(0);
	}else
	{ //IDLE
		if(CELL_BALANCE_OPTION & CELL_BALANCE_IDLE_ENABLE)
		{
			if ((oz3717_data.max_cell_data > CELL_BALANCE_START) && 
				  ((oz3717_data.max_cell_data-oz3717_data.which_cell_min)>CELL_BALANCE_DELTA))
						ret = oz3717_cb_set(oz3717_data.which_cell_max);
			else
			ret  = oz3717_cb_set(0);
		}
		else
			ret = oz3717_cb_set(0);
		
	}
	

	return ret;
}

/****************************************************************************
 * Description:
 *		Set OZ3717 cell balance chanel
 * Parameters:
 *		channel: 		cell balance channel number (1~17)
 * Return:
 *		see SPI_STATUS_XXX define	
 ****************************************************************************/
unsigned char oz3717_cb_set(unsigned short channel)
{
	unsigned char ret;
	unsigned short ustmp = 0;
	unsigned short therr = 0;
	
	if((channel >CELL_NUM))
	return ES_INVALID_PARAMETER;
	if(channel == 0)
	{
		ret =  o2m_register_write(OZ3717_CBSEL1, 0);
		ret =  o2m_register_write(OZ3717_CBSEL2, 0);

	}
	else if(channel == 17)
	{
			ret =  o2m_register_write(OZ3717_CBSEL2, 0);
			ret =  o2m_register_write(OZ3717_CBSEL1, 1);

	}
	else
	{
		ret =  o2m_register_write(OZ3717_CBSEL1, 0);
		ret =  o2m_register_write(OZ3717_CBSEL2, 1 << (channel - 1));
	}
#if DEBUG	
	therr = OZ3717_CBSEL1;
		ret =  o2m_register_read(therr, &ustmp);	
	printf("REG 0x%X CSE1=0x%4.4X\r\n",therr,ustmp & 0x01);
	
	therr = OZ3717_CBSEL2;
		ret =  o2m_register_read(therr, &ustmp);	
	printf("REG 0x%X CSE2=0x%4.4X\r\n",therr,ustmp);
#endif		
	return ret;
}

/*****************************************************************************
* Description:
*		one_latitude_table
* Parameters:
*	    number: number of the temperature data array 	
*	    data: data buffer
*	    value: x data in the array
* Return:
*		what does this function returned?
*****************************************************************************/
static signed int  one_latitude_table(signed int number,one_latitude_data_t *data,signed int value)
{
	int j;
	int res;
	
	for (j = 0;j < number;j++)
	{
		if (data[j].x ==value)
		{
			res = data[j].y;
			return res;
		}
		if(data[j].x > value)
			break;
	}
	
	if(j == 0)
		res = data[j].y;
	else if(j == number)
		res = data[j -1].y;
	else
	{
		res = ((value - data[j -1].x) * (data[j].y - data[j -1].y));
		
		if((data[j].x - data[j -1].x) != 0)
			res = res / (data[j].x  - data[j-1].x );
		res += data[j-1].y;
	}

	return res;
}	


/*****************************************************************************
 * Description:
 *		oz3717_convert_temperature 
 * Parameters:
 *	    dataR: thermal resistor value
 *	    temperature: temperature data buffer	(0.1C), Convert to 0.1K
 * Return:
 *      negative errno, zero on success.
 *****************************************************************************/
uint8_t oz3717_convert_temperature(unsigned int dataR,signed int *temperature)
{
   uint8_t ret;

	*temperature =	one_latitude_table(TEMPERATURE_DATA_NUM,temp_data_table,dataR)+2730;

    return ret;
    
}


//Endoffile
