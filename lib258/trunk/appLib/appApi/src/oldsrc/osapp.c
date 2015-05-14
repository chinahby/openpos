#define OSAPPDATA
#include "sand_incs.h"

/* Filename: dl_call.c */
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int call_so(void)
{
    void *handle;	/* shared library �� 'handle' ָ�� */
    double (*manager)(void *);   /* ָ�� shared library �e�ĺ��� */
    char *error;	/* ӛ� dynamic loader ���e�`ӍϢ */
    char *file, i=2;
    file="./libapp1.so";
    while (i--)
    {


        /* �_�� shared library 'libm' */
        handle = dlopen (file, RTLD_LAZY);
        if (!handle)
        {
            fprintf (stderr, "%s\n", dlerror());
            exit(1);
        }

        dlerror();    /* Clear any existing error */

        /* �� handle ָ��� shared library �e�ҵ� "cos" ����,
         * �K�������� memory address
         */
        manager = dlsym(handle, "manager");
        if ((error = dlerror()) != NULL)
        {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }

        /* indirect function call (����ָ�˺���),
         * ������ָ���ĺ���
         */
        (*manager)("helloWWW");

        dlclose(handle);

        file = "./libapp2.so";
    }
    return 0;
}




unsigned char OSAPP_Main(void)
{
    ucOSCurrAppID = 0;
    ucOSMasterAppID = 0;

    OSAPP_List();

    if ( OSAPP_Load() )
    {
        return(OSERR_SUCCESS);
    }

    OSAPP_ConvOrder(aucBuf);
    if ( !ucOSAppNB )
    {
        OSAPP_Maintence();
        return(OSERR_SUCCESS);
    }
}



