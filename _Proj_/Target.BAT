SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static;C:\Program Files\Git\cmd;C:\MyProgramFiles\ARMCompiler5.05u1\bin;C:\MyProgramFiles\GnuWin32\bin;C:\Program Files\Microsoft VS Code\bin;C:\MyProgramFiles\MinGW\bin;C:\Users\thinkpad\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=MKL02Z32xxx4
SET CPU_VENDOR=NXP
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"armcc" --Via ".\objects\main.__i"
"armcc" --Via ".\objects\share.__i"
"armcc" --Via ".\objects\delay.__i"
"armcc" --Via ".\objects\kl02z_it.__i"
"armcc" --Via ".\objects\kl02z_tpm.__i"
"armcc" --Via ".\objects\kl02z_uart.__i"
"armcc" --Via ".\objects\systeminfo.__i"
"armasm" --Via ".\objects\startup_mkl02z4._ia"
"armcc" --Via ".\objects\system_mkl02z4.__i"
"armlink" --Via ".\Objects\proj.lnp"
"fromelf.exe" ".\Objects\proj.axf" --i32combined --output ".\Objects\proj.hex"
