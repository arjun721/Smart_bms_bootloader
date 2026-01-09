/*****************************************************************************
* Copyright(c) O2Micro, 2019. All rights reserved.
*       
* O2Micro [OZ3717] Source Code Reference Design
* File: oz3717_regdef.h
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

 #ifndef _OZ3717_REGDEF_H_
 #define _OZ3717_REGDEF_H_

/*****************************************************************************
 * #include section
 * add #include here
 *****************************************************************************/


/*****************************************************************************
 * const section
 * add const #define here
 *****************************************************************************/

/* Operation register map define */

#define OZ3717_REG_HWVN							0x00	
#define OZ3717_REG_CELL1						0x01	
#define OZ3717_REG_CELL2						0x02	
#define OZ3717_REG_CELL3						0x03	
#define OZ3717_REG_CELL4						0x04	
#define OZ3717_REG_CELL5						0x05	
#define OZ3717_REG_CELL6						0x06	
#define OZ3717_REG_CELL7						0x07	
#define OZ3717_REG_CELL8						0x08	
#define OZ3717_REG_CELL9						0x09	
#define OZ3717_REG_CELL10						0x0A	
#define OZ3717_REG_CELL11						0x0B	
#define OZ3717_REG_CELL12						0x0C	
#define OZ3717_REG_CELL13						0x0D	
#define OZ3717_REG_CELL14						0x0E	
#define OZ3717_REG_CELL15						0x0F	
#define OZ3717_REG_CELL16						0x10	
#define OZ3717_REG_CELL17						0x11	
#define OZ3717_REG_PACKC						0x12	
#define OZ3717_REG_THM0 						0x13	
#define OZ3717_REG_THM1 						0x14	
#define OZ3717_REG_THM2 						0x15		

#define OZ3717_REG_INTMP 						0x18	
#define OZ3717_REG_VAUX 						0x19	
#define OZ3717_REG_VPACK 						0x1A	
#define OZ3717_REG_VBATT 						0x1B	
#define OZ3717_REG_VV50 						0x1C	
#define OZ3717_REG_VMCU 						0x1D	


#define OZ3717_REG_CADCTRL 					0x30	
    #define CADCTRL_DISABLE   			0x00	
  	#define CADCTRL_TRIGGER_CADC	  0x02	
		#define CADCTRL_HI_ACY_MODE		  0x04	
		#define CADCTRL_CONSE_CADC_RDY  0x08	
		#define CADCTRL_CC_ENABLE 		  0x10	
		#define CADCTRL_HW_CADC         0x80	


#define OZ3717_REG_CADC_LSB     0.0078125 //mV		
      

#define OZ3717_REG_CCH  						0x32	
#define OZ3717_REG_CCL  						0x33	

#define OZ3717_REG_CTDATA  						0x38	
#define OZ3717_REG_CMDATA  						0x39


#define OZ3717_REG_CXCN  					0x44	
#define CXCN_MIN_MASK			        0x1F00	
#define CXCN_MAX_MASK       	    0x001F	

#define OZ3717_REG_MAX_CELL 					0x45	
#define OZ3717_REG_MIN_CELL 					0x46	
		
//Configuration area		
#define OZ3717_UVSTDN        					0x4F	
#define OZ3717_UVSTDN_3CYCLE  					(0x00 << 14)	
#define OZ3717_UVSTDN_6CYCLE  					(0x01 << 14)	
#define OZ3717_UVSTDN_9CYCLE  					(0x02 << 14)	
#define OZ3717_UVSTDN_12CYCLE  					(0x03 << 14)	
#define OZ3717_UVSTDN_TH_LSB      			100
#define OZ3717_UVSTDN_TH_BASE     			1500
#define OZ3717_UVSTDN_TH_START     			0	



#define OZ3717_OVP          					0x50	
#define OZ3717_OVP_DLY_2CYCLE  					(0x00 << 14)	
#define OZ3717_OVP_DLY_4CYCLE  					(0x01 << 14)	
#define OZ3717_OVP_DLY_6CYCLE  					(0x02 << 14)	
#define OZ3717_OVP_DLY_8CYCLE  					(0x03 << 14)	
#define OZ3717_OVP_REL_HYS_LSB 					10	
#define OZ3717_OVP_REL_HYS_START				8	
#define OZ3717_OVP_TH_LSB      					5
#define OZ3717_OVP_TH_BASE     					3300
#define OZ3717_OVP_TH_START     				0	

#define OZ3717_DOC1P          					0x51	
#define OZ3717_DOC1P_CELL_NUMBER_START        13	// Start at bit 13
#define OZ3717_DOC1P_EFETC_MODE_DSG_OFF_EFETC_HIGH	    (0x00 << 11)	
#define OZ3717_DOC1P_EFETC_MODE_CHG_DSG_OFF_EFETC_HIGH	(0x01 << 11)	
#define OZ3717_DOC1P_EFETC_MODE_DSG_OFF_EFETC_LOW	    (0x02 << 11)	
#define OZ3717_DOC1P_EFETC_MODE_CHG_DSG_OFF_EFETC_LOW	(0x03 << 11)	
#define OZ3717_DOC1P_DELAY_2_CYCLE              (0x00 << 8)	// Start at bit 8
#define OZ3717_DOC1P_DELAY_4_CYCLE              (0x01 << 8)	// Start at bit 8
#define OZ3717_DOC1P_DELAY_6_CYCLE              (0x02 << 8)	// Start at bit 8
#define OZ3717_DOC1P_DELAY_8_CYCLE              (0x03 << 8)	// Start at bit 8
#define OZ3717_DOC1P_DELAY_10_CYCLE              (0x04 << 8)	// Start at bit 8
#define OZ3717_DOC1P_DELAY_12_CYCLE              (0x05 << 8)	// Start at bit 8
#define OZ3717_DOC1P_DELAY_14_CYCLE              (0x06 << 8)	// Start at bit 8
#define OZ3717_DOC1P_DELAY_16_CYCLE              (0x07 << 8)	// Start at bit 8
#define OZ3717_DOC1P_TH_LSB      					0.625
#define OZ3717_DOC1P_TH_BASE     					(5-0.625)
#define OZ3717_DOC1P_TH_DISABLE    					0

#define OZ3717_COCP          					0x52	//

#define OZ3717_COCP_TYPE_CFG_2_TERMINAL    	    (0x00 << 11)	
#define OZ3717_COCP_TYPE_CFG_3_TERMINAL    	    (0x01 << 11)	
#define OZ3717_COCP_DELAY_2_CYCLE              (0x00 << 8)	// Start at bit 8
#define OZ3717_COCP_DELAY_4_CYCLE              (0x01 << 8)	// Start at bit 8
#define OZ3717_COCP_DELAY_6_CYCLE              (0x02 << 8)	// Start at bit 8
#define OZ3717_COCP_DELAY_8_CYCLE              (0x03 << 8)	// Start at bit 8
#define OZ3717_COCP_DELAY_10_CYCLE              (0x04 << 8)	// Start at bit 8
#define OZ3717_COCP_DELAY_12_CYCLE              (0x05 << 8)	// Start at bit 8
#define OZ3717_COCP_DELAY_14_CYCLE              (0x06 << 8)	// Start at bit 8
#define OZ3717_COCP_DELAY_16_CYCLE              (0x07 << 8)	// Start at bit 8
#define OZ3717_COCP_TH_LSB      				0.3125
#define OZ3717_COCP_TH_BASE     				2.5
#define OZ3717_COCP_TH_DISABLE     				0

#define OZ3717_DOC2SC          					0x53	
#define OZ3717_DOC2SC_SCP_TH_2X_DOC2			(0x00 << 14)	
#define OZ3717_DOC2SC_SCP_TH_4X_DOC2			(0x01 << 14)	
#define OZ3717_DOC2SC_SCP_TH_6X_DOC2			(0x02 << 14)	
#define OZ3717_DOC2SC_SCP_TH_8X_DOC2			(0x03 << 14)	
#define OZ3717_DOC2SC_INTR_TYPE_LOW_LEVEL		(0x00 << 13)	
#define OZ3717_DOC2SC_INTR_TYPE_100HZ    		(0x01 << 13)	
#define OZ3717_DOC2_DELAY_START     		    (8)	
#define OZ3717_SC_DELAY_START       	    	(4)	
#define OZ3717_SCP_DLY_BASE          	    	(62.5)	
#define OZ3717_SCP_DLY_LSB          	    	(62.5)	
#define OZ3717_DOC2_TH_LSB          	    	(10)	
#define OZ3717_DOC2_TH_BASE          	    	(10)	

#define TARGET_CURRENT           (300.0f)      // A //300
#define DOC2_TH_MIN              (10.0f)      // mV
#define DOC2_TH_MAX              (160.0f)     // mV
#define DOC2_TH_STEP             (10.0f)      // mV/step

#define OZ3717_SPIWDT          					0x54	
#define OZ3717_SPIWDT_DISABLE  					0x00	

#define OZ3717_IDLE_TH         					0x55	
#define OZ3717_IDLE_TH_DSG_START				8	
#define OZ3717_IDLE_TH_DSG_LSB		    		0.03125	
#define OZ3717_IDLE_TH_DSG_BASE			    	0.125	
#define OZ3717_IDLE_TH_CHG_LSB		    		0.03125	
#define OZ3717_IDLE_TH_CHG_BASE			    	0.125	

#define OZ3717_UNLOCK          					0x56	
#define OZ3717_UNLOCK_STATUS   					0x01	
#define OZ3717_PATTERN			    		    0x7717

#define OZ3717_PWRMD          					0x57	
#define OZ3717_PWRMD_ACTIVE_MODE				(1 << 7)	
#define OZ3717_PWRMD_WRITE_UNLOCK				(1 << 8)	
#define OZ3717_PWRMD_VMCU_INTERNAL_ENABLE   	(0x10 << 4)	
#define OZ3717_PWRMD_VMCU_EXTERNAL_ENABLE   	(0x01 << 4)	
#define OZ3717_PWRMD_MODE_MASK                   (0x000F)	
#define OZ3717_PWRMD_MODE_STANDBY                (0x03)	
#define OZ3717_PWRMD_MODE_ACTIVE                 (0x05)	
#define OZ3717_PWRMD_MODE_SHUTDOWN               (0x0A)	

#define OZ3717_AUTOSCAN        					0x58	
#define OZ3717_SW_MAPPING_REQ  					(1 << 8)	
#define OZ3717_AUTO_SCAN_SAVE  					(1 << 2)	
#define OZ3717_AUTO_SCAN_ONE  					(0 << 1)	
#define OZ3717_AUTO_SCAN_EIGHT 					(1 << 1)	
#define OZ3717_AUTO_SCAN_ENABLE					(1 << 0)	

#define OZ3717_FETCTRL        					    0x59	
#define OZ3717_FETCTRL_PRE_SET_SHIFT		    (8)	
#define OZ3717_FETCTRL_PRE_SET_MASK   		  (0x00FF)	
#define OZ3717_FETCTRL_LOAD_DETECT_ENABLE		(1 << 7)	
#define OZ3717_FETCTRL_CHARGE_DETECT_ENABLE	(1 << 6)	
#define OZ3717_FETCTRL_SEL_PRE_MASK   		  (0x03 << 2)	
#define OZ3717_FETCTRL_SEL_PRE_DISABLE		  (0x00 << 2)	
#define OZ3717_FETCTRL_SEL_PRE_CHARGE		    (0x01 << 2)	
#define OZ3717_FETCTRL_SEL_PRE_DISCHARGE	  (0x02 << 2)	
#define OZ3717_FETCTRL_CHG_ENABLE    		    (0x01 << 1)	
#define OZ3717_FETCTRL_CHG_DISABLE    		  (0x00 << 1)	
#define OZ3717_FETCTRL_DSG_ENABLE    		    (0x01 << 0)	
#define OZ3717_FETCTRL_DSG_DISABLE    		  (0x00 << 0)	

#define OZ3717_CRRTSEL        					0x5A	
#define OZ3717_VREF_ENABLE     					(1 << 4)
#define OZ3717_CRRTSEL_20uA        			(0x01)	
#define OZ3717_CRRTSEL_120uA        		(0x02)	

#define OZ3717_CBSEL1        					  0x5B	
#define OZ3717_IOTP_STATE               (1 << 15)	
#define OZ3717_OVP_EVENT                (1 << 14)	
#define OZ3717_IN_CHG_STATE             (1 << 13)	
#define OZ3717_IN_DSG_STATE             (1 << 12)	
#define OZ3717_PRE_DSG                  (1 << 11)	
#define OZ3717_PRE_CHG                  (1 << 10)	
#define OZ3717_CHG_DRV                  (1 << 9)	
#define OZ3717_DSG_DRV                  (1 << 8)	
#define OZ3717_OHTP_STATE               (1 << 7)	
#define OZ3717_CP_OK                    (1 << 6)	
#define OZ3717_EFETC_DIG                (1 << 5)	
#define OZ3717_LOAD_OFF_VALID           (1 << 4)	
#define OZ3717_LOAD_ON_VALID            (1 << 3)	
#define OZ3717_CHGRIN_VALID             (1 << 2)	
#define OZ3717_INDSG_DIG                (1 << 1)	
#define OZ3717_CBSEL_CELL17             (1 << 0)	

#define OZ3717_CBSEL2        					  0x5C	
#define OZ3717_CBSEL_CELL16             (1 << 15)	
#define OZ3717_CBSEL_CELL15             (1 << 14)	
#define OZ3717_CBSEL_CELL14             (1 << 13)	
#define OZ3717_CBSEL_CELL13             (1 << 12)	
#define OZ3717_CBSEL_CELL12             (1 << 11)	
#define OZ3717_CBSEL_CELL11             (1 << 10)	
#define OZ3717_CBSEL_CELL10             (1 << 9)	
#define OZ3717_CBSEL_CELL9              (1 << 8)	
#define OZ3717_CBSEL_CELL8              (1 << 7)	
#define OZ3717_CBSEL_CELL7              (1 << 6)	
#define OZ3717_CBSEL_CELL6              (1 << 5)	
#define OZ3717_CBSEL_CELL5              (1 << 4)	
#define OZ3717_CBSEL_CELL4              (1 << 3)	
#define OZ3717_CBSEL_CELL3              (1 << 2)	
#define OZ3717_CBSEL_CELL2              (1 << 1)	
#define OZ3717_CBSEL_CELL1              (1 << 0)	

#define OZ3717_IE        					      0x5D	
#define OZ3717_CADC_TRIGGER_IE          (1 << 15)	
#define OZ3717_CC_OVERFLOW_IE           (1 << 14)	
#define OZ3717_IOTP_IE                  (1 << 11)	
#define OZ3717_CB_TIMEOUT_IE            (1 << 10)	
#define OZ3717_LOAD_OFF_IE              (1 << 9)	
#define OZ3717_LOAD_ON_IE               (1 << 8)	
#define OZ3717_CHGRIN_IE                (1 << 7)	
#define OZ3717_SPIWDT_IE                (1 << 6)	
#define OZ3717_TRG_SCAN_IE              (1 << 5)	
#define OZ3717_OVP_IE                   (1 << 4)	
#define OZ3717_COCP_IE                  (1 << 3)	
#define OZ3717_DOC1P_IE                 (1 << 2)	
#define OZ3717_DOC2P_IE                 (1 << 1)	
#define OZ3717_SCP_IE                   (1 << 0)	


#define OZ3717_ALL_IE  (OZ3717_CADC_TRIGGER_IE | \
                        OZ3717_CC_OVERFLOW_IE | \
                        OZ3717_IOTP_IE | \
                        OZ3717_CB_TIMEOUT_IE | \
                        OZ3717_LOAD_OFF_IE | \
                        OZ3717_LOAD_ON_IE | \
                        OZ3717_CHGRIN_IE | \
                        OZ3717_SPIWDT_IE | \
                        OZ3717_TRG_SCAN_IE | \
                        OZ3717_OVP_IE | \
                        OZ3717_COCP_IE | \
                        OZ3717_DOC1P_IE | \
                        OZ3717_DOC2P_IE | \
                        OZ3717_SCP_IE)

#define OZ3717_STATUS        				       	0x5E	
#define OZ3717_CADC_TRIGGER_FLAG           	(1 << 15)	
#define OZ3717_CC_OVERFLOW_FLAG           	(1 << 14)	
#define OZ3717_SPI_CRC_WR_ERR              	(1 << 13)	
#define OZ3717_PRON_FLAG                		(1 << 12)	
#define OZ3717_IOTP_FLAG                  	(1 << 11)	
#define OZ3717_CB_TIMEOUT_FLAG              (1 << 10)	
#define OZ3717_LOAD_OFF_FLAG               	(1 << 9)	
#define OZ3717_LOAD_ON_FLAG                 (1 << 8)	
#define OZ3717_CHGRIN_FLAG                	(1 << 7)	
#define OZ3717_SPIWDT_FLAG                	(1 << 6)	
#define OZ3717_TRG_SCAN_FLAG               	(1 << 5)	
#define OZ3717_OVP_FLAG                   	(1 << 4)	
#define OZ3717_COCP_FLAG                  	(1 << 3)	
#define OZ3717_DOC1P_FLAG                 	(1 << 2)	
#define OZ3717_DOC2P_FLAG                 	(1 << 1)	
#define OZ3717_SCP_FLAG                   	(1 << 0)	


#define OZ3717_TRIG         					        0x5F	
#define OZ3717_TRIG_ENABLE                  	(1 << 7)	
#define OZ3717_TRIG_ONE                   	  (0 << 6)	
#define OZ3717_TRIG_EIGHT                  	  (1 << 6)	
#define OZ3717_CTO_DETECTION               	  (1 << 5)	
#define OZ3717_TRIG_CHANNEL_ALL             	(0x1F)	


#define OZ3717_IE2                					0x60	
#define OZ3717_CONSE_CADC_RDY_IE         		(1 << 2)	
#define OZ3717_UV_IE                    		(1 << 1)	
#define OZ3717_IN_DSG_IE                 		(1 << 0)	


#define OZ3717_STR2                 					0x61	
#define OZ3717_CONSE_CADC_RDY_FLAG         		(1 << 2)	
#define OZ3717_UV_FLAG                    		(1 << 1)	
#define OZ3717_IN_DSG_FLAG                 		(1 << 0)	
#define OZ3717_CONSE_CADC_CLEAR_FLAG         	(0 << 2)	
#define OZ3717_UV_CLEAR_FLAG                  (0 << 1)	
#define OZ3717_IN_DSG_CLEAR_FLAG              (0 << 0)	

#endif
