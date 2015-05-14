#include <rs232yth.h>

HEADDATA HeadData;

unsigned char Rs232yth_UnPackPortData(unsigned char* aucInData,RECEIVETRANS *ReceiveData,unsigned char *aucIdentifer)
{

	 aucInData += RSLEN_LEN;    //����ƫ��
	 if(memcmp(aucInData,aucIdentifer,IDENTIFER_LEN))
	 {
	      return ERR_RESULT;
	 }
	 memcpy(&HeadData,aucInData,sizeof(HEADDATA));
	 aucInData += sizeof(HEADDATA);//��Ϣͷƫ��
	 memcpy(ReceiveData,aucInData,sizeof(RECEIVETRANS));

	 return SUCCESS_RESULT;
}


/*
ucFlag 0x00 = �ص���Ӧ�÷������ݺ�ص���Ӧ��    //0x03
       0x01 = �ص���Ӧ�÷������ݺ󲻻ص���Ӧ��   //0x01,0x02,0x04
       0x02 = �ص���Ӧ�ò����κβ������ϻص���Ӧ��  //0x08
       0x03 = �ص���Ӧ�ã������κβ���Ҳ�����ظ���Ӧ��//0x05,0x06,0x07
*/
unsigned char Rs232yth_PackPortData(SENDTRANS *SendTransData,unsigned char *aucOutData,unsigned char ucFlag)
{
	  unsigned long ulLength = 0;

	  switch(ucFlag)
		{
		     case 0:
				  	memcpy(HeadData.aucObligate,"\x03\x00\x00\x00\x00",OBLIGATE_LEN);
				  	break;
		     case 1:
				  	memcpy(HeadData.aucObligate,"\x01\x00\x00\x00\x00",OBLIGATE_LEN);
				  	break;
		     case 2:
				  	memcpy(HeadData.aucObligate,"\x08\x00\x00\x00\x00",OBLIGATE_LEN);
				  	break;
		     case 3:
				  	memcpy(HeadData.aucObligate,"\x05\x00\x00\x00\x00",OBLIGATE_LEN);
				  	break;
		     default:
		     	  memcpy(HeadData.aucObligate,"\x00\x00\x00\x00\x00",OBLIGATE_LEN);
		     	  return ERR_RESULT;
			      break;
		}

		ulLength = sizeof(SENDTRANS)+sizeof(HEADDATA);
		long_bcd(aucOutData,RSLEN_LEN,&ulLength);
		aucOutData += RSLEN_LEN;
		memcpy(aucOutData,&HeadData,sizeof(HEADDATA));
		aucOutData += sizeof(HEADDATA);
		memcpy(aucOutData,SendTransData,sizeof(SENDTRANS));aucOutData += sizeof(SENDTRANS);

		return SUCCESS_RESULT;
}
