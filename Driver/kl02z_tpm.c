/**
 * @file kl02z_tpm.c
 * @author imyumeng (imyumeng@qq.com)
 * @brief 
 * @version 0.1
 * @date 2019-08-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "MKL02Z4.h" // Device header
#include "kl02z_tpm.h"

void TPM_Init(TPM_Type *TPMx, uint32_t modulo, uint8_t ps)
{
  //时钟来源MCGFLL
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

  //开启时钟
  if (TPM0 == TPMx)
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
  else if (TPM1 == TPMx)
    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

  //禁止TPM
  TPMx->SC = 0;

  //计数器清零
  TPMx->CNT = 0;
  /* set modulo */
  TPMx->MOD = modulo;

  /* set ps, this must be done after set modulo */
  TPMx->SC &= ~TPM_SC_PS_MASK;
  TPMx->SC |= TPM_SC_PS(ps);
}

/*******************************
功能：TPM_PWM初始化
说明TPM 模块
      ch 通道
     freq 频率
     duty 占空比
Created on:2015/5/27
Author: Kem.chen
*******************************/

void TPM_PWM_Init(TPM_Type *TPMx, u8 ch, u32 freq, u32 duty)
{
  u32 clk_hz;
  u16 mod;
  u16 tmp;
  u8 ps;
  u16 cv;
  //求MOD值
  clk_hz = SystemBusClock;
  tmp = (clk_hz >> 16) / freq;
  ps = 0;
  while ((tmp >> ps) > 1)
  {
    ps++;
  }
  mod = (clk_hz >> (ps)) / freq - 1;
  cv = (duty * (mod - 0 + 1)) / 100;

  //时钟来源MCGFLL
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

  //开启时钟
  if (TPM0 == TPMx)
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
  else if (TPM1 == TPMx)
    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

  //禁止TPM
  TPMx->SC = 0;

  //计数器清零
  TPMx->CNT = 0;

  //边缘对其
  TPMx->CONTROLS[ch].CnSC = 0;
  TPMx->CONTROLS[ch].CnSC &= ~TPM_CnSC_ELSA_MASK;
  TPMx->CONTROLS[ch].CnSC |= TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

  TPMx->SC = TPM_SC_CPWMS_MASK | TPM_SC_PS(ps) | TPM_SC_CMOD(1);

  TPMx->MOD = mod;
  TPMx->CONTROLS[ch].CnV = cv;

  //计数器清零
  TPMx->CNT = 0;
}
/*******************************
功能：TPM_PWM占空比设置
说明：TPM 模块
      ch 通道
     duty 占空比
Created on:2015/5/27
Author: Kem.chen
*******************************/
void TPM_PWM_Duty(TPM_Type *TPMx, u8 ch, u32 duty)
{
  u32 cv;
  u32 mod;

  mod = TPM_MOD_REG(TPMx);

  cv = (duty * (mod - 0 + 1)) / 100;

  TPMx->CONTROLS[ch].CnV = cv;
}
