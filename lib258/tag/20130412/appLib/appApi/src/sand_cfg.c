#include <stdio.h>

#include "sand_cfg.h"



//�ſ����ú���
extern void OSCFG_MAGSettingLoad(OSMAGCFG  *pOsMagCfg);
//IC��
extern void OSCFG_ICSettingLoad(OSICCFG  *pOsICCfg);


int mag_cfg_demo()
{
		OSMAGCFG magcfg;
	    
		magcfg.ucMagDebug = 1;
    OSCFG_MAGSettingLoad(&magcfg);
    Uart_Printf("ver=%s\n", magcfg.ucMagVer);
		
}