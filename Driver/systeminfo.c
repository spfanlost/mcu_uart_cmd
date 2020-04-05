/**
 * @file systeminfo.c
 * @author imyumeng (imyumeng@qq.com)
 * @brief 
 * @version 0.1
 * @date 2019-08-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "MKL02Z4.h" // Device header
#include "systeminfo.h"
#include "share.h"

void SystemInfo(void)
{
    uint16_t temp;
    /* Determine the Kinetis family */
    LOG(LOG_INFO,"SystemInfo:\n");
    LOG(LOG_INFO,"Kinetis ");
    switch ((SIM_SDID & SIM_SDID_FAMID(0x7)) >> SIM_SDID_FAMID_SHIFT)
    {
    case 0x0:
        LOG(LOG_INFO,"K0");
        break;
    case 0x1:
        LOG(LOG_INFO,"K1");
        break;
    case 0x2:
        LOG(LOG_INFO,"K2");
        break;
    case 0x3:
        LOG(LOG_INFO,"K3");
        break;
    case 0x4:
        LOG(LOG_INFO,"K4");
        break;
    default:
        LOG(LOG_INFO,"\nUnrecognized Kinetis family device.\n");
        break;
    }
    /* Determine Sub-Family ID */
    switch ((SIM_SDID & SIM_SDID_SUBFAMID(0x7)) >> SIM_SDID_SUBFAMID_SHIFT)
    {
    case 0x2:
        LOG(LOG_INFO,"2 ");
        break;
    case 0x4:
        LOG(LOG_INFO,"4 ");
        break;
    case 0x5:
        LOG(LOG_INFO,"5 ");
        break;
    case 0x6:
        LOG(LOG_INFO,"6 ");
        break;
    default:
        LOG(LOG_INFO,"\nUnrecognized Kinetis sub-family device.\n");
        break;
    }

    /* Determine the package size */
    switch ((SIM_SDID & SIM_SDID_PINID(0xF)) >> SIM_SDID_PINID_SHIFT)
    {
    case 0x2:
        LOG(LOG_INFO,"32pin ");
        break;
    case 0x4:
        LOG(LOG_INFO,"48pin ");
        break;
    case 0x5:
        LOG(LOG_INFO,"64pin ");
        break;
    case 0x6:
        LOG(LOG_INFO,"80pin ");
        break;
    case 0x7:
        LOG(LOG_INFO,"81pin ");
        break;
    case 0x8:
        LOG(LOG_INFO,"100pin ");
        break;
    case 0x9:
        LOG(LOG_INFO,"121pin ");
        break;
    case 0xA:
        LOG(LOG_INFO,"144pin ");
        break;
    default:
        LOG(LOG_INFO,"\nUnrecognized Kinetis package code.      ");
        break;
    }
    
    /* Kinetis Series ID */
    switch((SIM_SDID & SIM_SDID_SERIESID(0x1))>>SIM_SDID_SERIESID_SHIFT) 
    {  
        case 0x1:
            LOG(LOG_INFO,"KL family\n"); //Low Power Line with Cortex M0+
            break;
        default:
            LOG(LOG_INFO,"\nUnrecognized Kinetis family attribute.\n");  
            break;  	
    }
    /* Determine the System SRAM Size */
    switch ((SIM_SDID & SIM_SDID_SRAMSIZE(0x7)) >> SIM_SDID_SRAMSIZE_SHIFT)
    {
    case 0x0:
        LOG(LOG_INFO,"SRAM Size: 0.5 KB\n");
        break;
    case 0x1:
        LOG(LOG_INFO,"SRAM Size:  1 KB\n");
        break;
    case 0x2:
        LOG(LOG_INFO,"SRAM Size:  2 KB\n");
        break;
    case 0x3:
        LOG(LOG_INFO,"SRAM Size:  4 KB\n");
        break;
    case 0x4:
        LOG(LOG_INFO,"SRAM Size:  8 KB\n");
        break;
    case 0x5:
        LOG(LOG_INFO,"SRAM Size:  16 KB\n");
        break;
    case 0x6:
        LOG(LOG_INFO,"SRAM Size:  32 KB\n");
        break;
    case 0x7:
        LOG(LOG_INFO,"SRAM Size:  64 KB\n");
        break;
    default:
        LOG(LOG_INFO,"\nUnrecognized SRAM Size.\n");
        break;
    }

    /* Determine the revision ID */
    temp = ((SIM_SDID_REVID(0xF)) >> SIM_SDID_REVID_SHIFT);
    LOG(LOG_INFO,"Silicon rev %d\n", temp);

    /* Determine the flash revision */
    flash_identify();

    /* Determine the P-flash size */
    switch ((SIM_FCFG1 & SIM_FCFG1_PFSIZE(0xF)) >> SIM_FCFG1_PFSIZE_SHIFT)
    {
    case 0x0:
        LOG(LOG_INFO,"Flash size:  8 KB program flash, 0.25 KB protection region\n");
        break;
    case 0x1:
        LOG(LOG_INFO,"Flash size:  16 KB program flash, 0.5 KB protection region\n");
        break;
    case 0x3:
        LOG(LOG_INFO,"Flash size:  32 KB program flash, 1 KB protection region\n");
        break;
    case 0x5:
        LOG(LOG_INFO,"Flash size:  64 KB program flash, 2 KB protection region\n");
        break;
    case 0x7:
        LOG(LOG_INFO,"Flash size:  128 KB program flash, 4 KB protection region\n");
        break;
    case 0x9:
        LOG(LOG_INFO,"Flash size:  256 KB program flash, 4 KB protection region\n");
        break;
    case 0xF:
        LOG(LOG_INFO,"Flash size:  128 KB program flash, 4 KB protection region\n");
        break;
    default:
        LOG(LOG_INFO,"ERR!! Undefined flash size\n");
        break;
    }
    LOG(LOG_INFO,"SystemClockInfo:\n");
    LOG(LOG_INFO,"SystemCoreClock:%fM SystemBusClock:%fM\n", SystemCoreClock / 10e5, SystemBusClock / 10e5);
}
/********************************************************************/
/*!
* \brief   flash Identify
* \return  None
*
* This is primarly a reporting function that displays information
* about the specific flash parameters and flash version ID for 
* the current device. These parameters are obtained using a special
* flash command call "read resource." The first four bytes returned
* are the flash parameter revision, and the second four bytes are
* the flash version ID.
*/
void flash_identify(void)
{
    /* Get the flash parameter version */

    /* Write the flash FCCOB registers with the values for a read resource command */
    FTFA_FCCOB0 = 0x03;
    FTFA_FCCOB1 = 0x00;
    FTFA_FCCOB2 = 0x00;
    FTFA_FCCOB3 = 0x00;
    FTFA_FCCOB8 = 0x01;

    /* All required FCCOBx registers are written, so launch the command */
    FTFA_FSTAT = FTFA_FSTAT_CCIF_MASK;

    /* Wait for the command to complete */
    while (!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK))
        ;

    //    LOG(LOG_INFO,"Flash parameter version %d.%d.%d.%d\n",FTFA_FCCOB4,FTFA_FCCOB5,FTFA_FCCOB6,FTFA_FCCOB7);

    /* Get the flash version ID */

    /* Write the flash FCCOB registers with the values for a read resource command */
    FTFA_FCCOB0 = 0x03;
    FTFA_FCCOB1 = 0x00;
    FTFA_FCCOB2 = 0x00;
    FTFA_FCCOB3 = 0x04;
    FTFA_FCCOB8 = 0x01;

    /* All required FCCOBx registers are written, so launch the command */
    FTFA_FSTAT = FTFA_FSTAT_CCIF_MASK;

    /* Wait for the command to complete */
    while (!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK))
        ;

    //    LOG(LOG_INFO,"Flash version ID %d.%d.%d.%d\n",FTFA_FCCOB4,FTFA_FCCOB5,FTFA_FCCOB6,FTFA_FCCOB7);
}
