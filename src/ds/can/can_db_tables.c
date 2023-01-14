
/*--------------------------------------------------------------------------*/
/** \file     can_db_tables.c
*   \brief    CAN bus database
*   \date     20230105
*   \author   
*   \Applics Studio
*
*   \platform 2P
* --------------------------------------------------------------------------*/

#pragma ghs nowarning 1981 // suppress warning for "empty initializer is non-standard"

#include "can_db_tables.h" 
#include "sfl_can_db_tables_data.h"



/** \ingroup can_tables
*   \brief           Table of all CAN datapoints
* --------------------------------------------------------------------------*
*                    Every line defines a datapoint within a CAN block.
*                    Every datapoint is assigned to a CAN block ID
*                    #can_block_id, the order of the IDs has to be the same as
*                    the CAN block table #can_block_db_const.
*                    The table contains bit position, bit length and datatype.
*                    The datapoints are used with #can_db_set_value() or #can_db_get_value().
*                    As parameter use the name from the list #can_dp_id verwendet,
*                    the order has to be the same as in this table.
* --------------------------------------------------------------------------*/
 volatile const can_datenpunkt_db_const_typ can_datenpunkt_db_const[CAN_DP_MAX+1]__attribute__((section(".rodata#"))) = { // Array can1_....  with the dp of one CAN interface
//  CAN block index           Pos bit0           Length           Data type           Data format                     LineNr: datapoint-ID            
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//  (16 Bit)                  (0-63)             (1-32)                               (0=Intel, 1=Motorola)                                           
//-----------------------------------------------------------------------------------------------------------------------------------------------------
{   CanTelegramm_0            ,0                 ,32              ,UINT               ,0                              },   //_RPM_Average             
{   CanTelegramm_0            ,32                ,32              ,UINT               ,0                              },   //_ENG_HRS_EEPROM          
{   CanTelegramm_1            ,0                 ,32              ,UINT               ,0                              },   //_ENG_HRS_1sec            
{   CanTelegramm_2            ,0                 ,32              ,UINT               ,0                              },   //_ENGINE_RPM              
{   CanTelegramm_2            ,32                ,32              ,UINT               ,0                              },   //_RPM_Total               
};



/** \ingroup can_tables
*   \brief           Table of all CAN blocks
* --------------------------------------------------------------------------*
*                    Every line defines a CAN block and sets the CAN-ID,
*                    the minimal and maximal time between to messages for sending,
*                    the length of a message and the direction (0=Rx, 1=Tx).
*                    The CAN-IDs must be defined in can_db.h.
*                    Every CAN block is assigned to a ID, the order has to be
*                    the same as in this table.
* --------------------------------------------------------------------------*/
 volatile const can_block_db_const_typ can_block_db_const[CAN_BLOCK_MAX+1]__attribute__((section(".rodata#"))) = { // Array can1_....  with the CAN blocks of one CAN interface
//  CAN bus index            CAN-ID             EXT - ID           Max                Min                Msg-Len DLC           TX Flag           CAN bus gw                 CAN-ID-Mask           Mux start           Mux length           Mux val                                        
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  8 Bit                    (32 Bit)           (0,1)              (32 Bit)           (32 Bit)           (0-8)                 (0,1)             (NONE,CAN_BUS_x)           (29bit)               0-62                0-8                  0-255                                          
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
{   CAN_BUS_0                ,0x100             ,0                 ,500               ,50                ,8                    ,1                ,NONE                      ,0x0                  ,0                  ,0                   ,0               },  //CanTelegramm_0          
{   CAN_BUS_0                ,0x101             ,0                 ,500               ,50                ,8                    ,1                ,NONE                      ,0x0                  ,0                  ,0                   ,0               },  //CanTelegramm_1          
{   CAN_BUS_0                ,0x102             ,0                 ,100               ,50                ,8                    ,1                ,NONE                      ,0x0                  ,0                  ,0                   ,0               },  //CanTelegramm_2          
};

 volatile const can_bus_db_const_typ can_bus_db_const[CAN_BUS_MAX+1]__attribute__((section(".rodata#"))) = 
{
 //                                                                             Manual setting of segment / jumper-with register
 //           HW CAN-Modul          , HW CAN-Modul active          ,  CAN Baudrate              , Gateway Input           , {           Name Str.           , SJ               , Prescaler          , T_SEG1           , T_SEG2           , CLK_SRC          , PROP              }, {           Name Str. FD          , SJ               , Prescaler FD          , T_SEG1 FD          , T_SEG2 FD          , CLK_SRC FD          , PROP FD          } , can fd flag           }          , 
 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 //           (8 Bit)               , (8 Bit)                      ,  (8 Bit)                   , (8 Bit)                 , {           (10 Byte)           , (8 Bit)          , (8 Bit)            , (8 Bit)          , (8 Bit)          , (8 Bit)          , (8 Bit)           }, {           (10 Byte)             , (8 Bit)          , (8 Bit)               , (8 Bit)            , (8 Bit)            , (8 Bit)             , (8 Bit)          } , (8 Bit)               }          , 
 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 {            CAN_BUS_0             , TRUE                         ,  BIOS_CAN_250KBIT          , IS_NO_GW_INPUT          , {           "250 kBit"          , 3                , 15                 , 3                , 3                , 0                , 0                 }, {           ""                    , 0                , 0                     , 0                  , 0                  , 0                   , 0                } , 0                     }          , 
 {            CAN_BUS_1             , TRUE                         ,  BIOS_CAN_250KBIT          , IS_NO_GW_INPUT          , {           "250 kBit"          , 3                , 15                 , 3                , 3                , 0                , 0                 }, {           ""                    , 0                , 0                     , 0                  , 0                  , 0                   , 0                } , 0                     }          , 
 
};


 volatile const can_gateway_db_const_typ can_gateway_db_const[CAN_GATEWAY_DB_MAX+1]__attribute__((section(".rodata#"))) =
 {
 //Input Bus ID           Output Bus ID                     
 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 //(8 Bit)                (8 Bit)                           
 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 
};

#pragma ghs endnowarning
