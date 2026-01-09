/******************************************************************************
 * Copyright (C) 2021, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************/

/******************************************************************************
 * @file   spi.h
 *
 * @brief  Header file for SPI functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __SPI_H__
#define __SPI_H__

/******************************************************************************
 * Include files
 *****************************************************************************/
#include "ddl.h"


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//@{

/******************************************************************************
 * Global type definitions
 *****************************************************************************/
 /**
 ******************************************************************************
 ** \brief SPI ????????
 ******************************************************************************/ 

/**
 ******************************************************************************
 ** \brief SPI ??????
 ******************************************************************************/ 
typedef enum en_spi_en
{
    SpiMskEnable     = 0x4u,    ///< SPI????
    SpiMskDisable    = 0u,      ///< SPI????
}en_spi_en_t;
/**
 ******************************************************************************
 ** \brief SPI ????
 ******************************************************************************/ 
typedef enum en_spi_mode
{
    SpiMskMaster = 0x10u,  ///<??
    SpiMskSlave  = 0x00u,  ///<??
}en_spi_mode_t;
/**
 ******************************************************************************
 ** \brief SPI ??????
 ******************************************************************************/ 
typedef enum en_spi_cpol
{
    SpiMskcpollow  = 0u,       ///<????
    SpiMskcpolhigh = 0x08u,    ///<????
}en_spi_cpol_t;
/**
 ******************************************************************************
 ** \brief SPI ??????
 ******************************************************************************/ 
typedef enum en_spi_cpha
{
    SpiMskCphafirst  = 0u,      ///<??????
    SpiMskCphasecond = 0x4u,    ///<??????
}en_spi_cpha_t;
/**
 ******************************************************************************
 ** \brief SPI ??????
 *****************************************************************************/
typedef enum en_spi_clk_div
{
    SpiClkMskDiv2   = 0x00u,   ///<PCLK 2??       
    SpiClkMskDiv4   = 0x01u,   ///<PCLK 4??            
    SpiClkMskDiv8   = 0x02u,   ///<PCLK 8??            
    SpiClkMskDiv16  = 0x03u,   ///<PCLK 16??            
    SpiClkMskDiv32  = 0x80u,   ///<PCLK 32??            
    SpiClkMskDiv64  = 0x81u,   ///<PCLK 64??            
    SpiClkMskDiv128 = 0x82u,   ///<PCLK 128??            
}en_spi_clk_div_t;

/**
 ******************************************************************************
 ** \brief SPI ???????
 *****************************************************************************/
typedef enum en_spi_cspin
{
    SpiCsLow  = 0u,      ///<?????         
    SpiCsHigh = 1u,      ///<?????            
}en_spi_cspin_t;

/**
 ******************************************************************************
 ** \brief SPI ??
 *****************************************************************************/
typedef enum en_spi_status
{
    SpiIf              = 0x80u,   ///<????????       
    SpiSserr           = 0x20u,   ///<????????    
    SpiMdf             = 0x10u,   ///<???????? 
    SpiBusy            = 0x08u,   ///<SPI?????
    SpiTxe             = 0x04u,   ///<?????????
    SpiRxne            = 0x02u,   ///<?????????
}en_spi_status_t;
/**
 ******************************************************************************
 ** \brief SPI ??????
 *****************************************************************************/
typedef enum en_spi_func
{
    SpiMskRxNeIe          = 0x40u,   ///<???????????       
    SpiMskTxEIe           = 0x20u,   ///<??????????    
    SpiMskDmaTxEn         = 0x10u,   ///<DMA????????
    SpiMskDmaRxEn         = 0x08u,   ///<DMA????????
}en_spi_func_t;
/**
 ******************************************************************************
 ** \brief SPI ???????
 *****************************************************************************/
typedef struct stc_spi_cfg
{
    en_spi_mode_t       enSpiMode;          ///< ??????
    en_spi_clk_div_t    enPclkDiv;          ///< PCLK????(?????)
    en_spi_cpol_t       enCPOL;             ///< ??????
    en_spi_cpha_t       enCPHA;             ///< ??????
}stc_spi_cfg_t;

//SPI ????  
boolean_t Spi_GetStatus(M0P_SPI_TypeDef* SPIx,en_spi_status_t enStatus);

///<SPI ????/??
en_result_t Spi_IrqEnable(M0P_SPI_TypeDef* SPIx);
en_result_t Spi_IrqDisable(M0P_SPI_TypeDef* SPIx);
//SPI ??????
en_result_t Spi_ClearStatus(M0P_SPI_TypeDef* SPIx);
//SPI?????
en_result_t Spi_Init(M0P_SPI_TypeDef* SPIx,stc_spi_cfg_t* pstcSpiCfg);
//SPI ????????
en_result_t Spi_FuncEnable(M0P_SPI_TypeDef* SPIx, en_spi_func_t enFunc);
en_result_t Spi_FuncDisable(M0P_SPI_TypeDef* SPIx, en_spi_func_t enFunc);
//SPI????
en_result_t Spi_DeInit(M0P_SPI_TypeDef* SPIx);

//SPI ????????
void Spi_SetCS(M0P_SPI_TypeDef* SPIx,boolean_t bFlag);

//SPI ????????
uint8_t Spi_RWByte(M0P_SPI_TypeDef* SPIx, uint8_t u8Data);
//SPI ????
en_result_t Spi_SendData(M0P_SPI_TypeDef* SPIx, uint8_t u8Data);
void Spi_Slave_DummyWriteData(M0P_SPI_TypeDef* SPIx, uint8_t u8Data);
en_result_t Spi_SendBuf(M0P_SPI_TypeDef* SPIx, uint8_t* pu8Buf, uint32_t u32Len);
//SPI ????
uint8_t Spi_ReceiveData(M0P_SPI_TypeDef* SPIx);
en_result_t Spi_ReceiveBuf(M0P_SPI_TypeDef* SPIx, uint8_t* pu8Buf, uint32_t u32Len);
//@} // Spi Group

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/

