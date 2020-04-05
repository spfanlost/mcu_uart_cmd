#ifndef __kl02z_tpm_H
#define __kl02z_tpm_H

void TPM_Init(TPM_Type *TPMx, uint32_t modulo, uint8_t ps);
void TPM_PWM_Init(TPM_Type *TPMx, u8 ch, u32 freq, u32 duty);
void TPM_PWM_Duty(TPM_Type *TPMx, u8 ch, u32 duty);

#endif
