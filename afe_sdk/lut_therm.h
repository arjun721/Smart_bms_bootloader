/*****************************************************************************
* Copyright(c) O2Micro, 2019. All rights reserved.
*       
* O2Micro [OZ3717] Source Code Reference Design
* File: lut_therm.h
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

 #ifndef _OZ3717_THERMAL_TABLE_H_
 #define _OZ3717_THERMAL_TABLE_H_

/*****************************************************************************
 * #include section
 * add #include here
 *****************************************************************************/
 
typedef struct tag_one_latitude_data {
 	 signed int 			 x;//Resistor value
 	 signed int 			 y;//0.1 Degree C
} one_latitude_data_t;

//0.1C with R table
#define TEMPERATURE_DATA_NUM   25  
one_latitude_data_t			temp_data_table[TEMPERATURE_DATA_NUM] = {                                                                                                               
			{1266,    900}, {1451 ,   850}, {1668,   800},
      {1924,   750}, {2228,   700}, {2588,   650},
      {3020,   600}, {3563,   550}, {4160,   500},
      {4911,   450}, {5827,   400}, {6940,   350},
      {8313,   300}, {10000,  250}, {12090,  200},
      {14490,  150}, {17960,  100}, {22050,   50},
      {27280,   0}, {33900,  -50}, {42470, -100},
      {53410, -150}, {67770 ,-200},	{86430, -250},
			{111300, -300},

};

#endif

