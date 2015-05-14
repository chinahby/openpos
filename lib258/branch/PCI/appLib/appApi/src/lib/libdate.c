/*
	Library Function
--------------------------------------------------------------------------
	FILE  libdate.c
--------------------------------------------------------------------------
    INTRODUCTION
    ============
    Created :		2002-10-22		Xiaoxi Jiang
    Last modified :	2002-10-22		Xiaoxi Jiang
    Module :
    Purpose :
        Source file.

    List of routines in file :

    File history :
*/

#include <tools.h>

typedef struct
{
    char acMonName[4];
    char acMon[3];
}MONTH;

const MONTH MonTab[12] =
{
    {"JAN",  "01"},
    {"FEB",  "02"},
    {"MAR",  "03"},
    {"APR",  "04"},
    {"MAY",  "05"},
    {"JUN",  "06"},
    {"JUL",  "07"},
    {"AUG",  "08"},
    {"SEP",  "09"},
    {"OCT",  "10"},
    {"NOV",  "11"},
    {"DEC",  "12"}
};


/*
EN:

zh_CN:
	��������ַ����Ƿ�������
��ڲ�����
	pcDate:			�꣬��ʽ"YYYY"
���ڲ�����
	��
����ֵ��
	����ִ�н��
������
*/
long DATE_LeapYearStr(char *pcDate)
{
	int iYear;

	if( !pcDate )
		return(DATE_PARAM_ERR);
	iYear = (int)CONV_AscLong((unsigned char *)pcDate,4);
	if(  (iYear%400 == 0)
	   ||(  (iYear%4 == 0)
	      &&(iYear%100)
	     )
	  )
	  	return(DATE_SUCCESS);
	else
		return(DATE_NOTLEAPYEAR_ERR);
}

/*
EN:

zh_CN:
	������Ƿ�������
��ڲ�����
	iYear:			�꣬��ʽ"YYYYMMDD"
���ڲ�����
	��
����ֵ��
	����ִ�н��
������
*/
long DATE_LeapYearInt(int iYear)
{
	if(  (iYear%400 == 0)
	   ||(  (iYear%4 == 0)
	      &&(iYear%100)
	     )
	  )
	  	return(DATE_SUCCESS);
	else
		return(DATE_NOTLEAPYEAR_ERR);
}

/*
EN:

zh_CN:
	�����ַ������Ƿ�Ϸ�
��ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
���ڲ�����
	��
����ֵ��
	����ִ�н��
������
*/
long DATE_ValideDateStr(char *pcDate)
{
	int iI;
	int iYear;
	int iMonth;
	int iDay;

	if( !pcDate )
		return(DATE_PARAM_ERR);

	for( iI = 0;iI<DATE_DATESTRLEN;iI++)
	{
		if(  (*(pcDate+iI) < '0')
		   ||(*(pcDate+iI) > '9')
		  )
		{
			return(DATE_INVALIDCHAR_ERR);
		}
	}
	iYear = (int)CONV_AscLong((unsigned char *)pcDate,4);
	iMonth = (int)CONV_AscLong((unsigned char *)pcDate+4,2);
	iDay = (int)CONV_AscLong((unsigned char *)pcDate+6,2);

	return(DATE_ValideDateInt(iYear,iMonth,iDay));
}

/*
EN:

zh_CN:
	���������Ƿ�Ϸ�
��ڲ�����
	iYear:			�꣬��ʽ"YYYY"
	iMonth:			�£���ʽ"MM"
	iDay:			�գ���ʽ"DD"
���ڲ�����
	��
����ֵ��
	����ִ�н��
������
*/
long DATE_ValideDateInt(int iYear,int iMonth,int iDay)
{
	long lResult;
	int iMonthDays;

	if(  (iYear<1900)
	   ||(iYear>2500)
	  )
	{
		return(DATE_INVALIDYEAR_ERR);
	}

	lResult = DATE_MonthDaysInt(iYear,iMonth,&iMonthDays);
	if( lResult != DATE_SUCCESS)
		return(lResult);
	if(  (iDay < 1)
	   ||(iDay > iMonthDays)
	  )
		return(DATE_INVALIDDAY_ERR);
	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�����ַ�ʱ���Ƿ�Ϸ�
��ڲ�����
	pcTime:			ʱ���룬��ʽ"HHMMSS"
���ڲ�����
	��
����ֵ��
	����ִ�н��
������
*/
long DATE_ValideTimeStr(char *pcTime)
{
	int iI;
	int iHour;
	int iMin;
	int iSec;

	if( !pcTime )
		return(DATE_PARAM_ERR);

	for( iI = 0;iI<6;iI++)
	{
		if(  (*(pcTime+iI) < '0')
		   ||(*(pcTime+iI) > '9')
		  )
		{
			return(DATE_INVALIDCHAR_ERR);
		}
	}
	iHour = (int)CONV_AscLong((unsigned char *)pcTime,2);
	iMin = (int)CONV_AscLong((unsigned char *)pcTime+2,2);
	iSec = (int)CONV_AscLong((unsigned char *)pcTime+4,2);

	return(DATE_ValideTimeInt(iHour,iMin,iSec));
}

/*
EN:

zh_CN:
	����ʱ���Ƿ�Ϸ�
��ڲ�����
	iHour:			ʱ����ʽ"HH"
	iMin:			�֣���ʽ"MM"
	iSec:			�룬��ʽ"SS"
���ڲ�����
	��
����ֵ��
	����ִ�н��
������
*/
long DATE_ValideTimeInt(int iHour,int iMin,int iSec)
{

	if(  (iHour<0)
	   ||(iHour>24)
	  )
	{
		return(DATE_INVALIDHOUR_ERR);
	}

	if(  (iMin<0)
	   ||(iMin>60)
	  )
	{
		return(DATE_INVALIDMIN_ERR);
	}

	if(  (iSec<0)
	   ||(iSec>60)
	  )
	{
		return(DATE_INVALIDSEC_ERR);
	}

	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�ַ�����ת����ֵ������
��ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
���ڲ�����
	piYear:			�꣬��ʽ"YYYY"
	piMonth:		�£���ʽ"MM"
	piDay:			�գ���ʽ"DD"
����ֵ��
	����ִ�н��
������
*/
long DATE_ConvStrInt(char *pcDate,int *piYear,int *piMonth,int *piDay)
{
	long lResult;
	if( !pcDate )
		return(DATE_PARAM_ERR);
	lResult = DATE_ValideDateStr(pcDate);
	if( lResult != DATE_SUCCESS)
		return(lResult);

	if(!piYear)
	{
		*piYear = (int)CONV_AscLong((unsigned char *)pcDate,4);
	}
	if(!piMonth)
	{
		*piMonth = (int)CONV_AscLong((unsigned char *)pcDate+4,2);
	}
	if(!piDay)
	{
		*piDay = (int)CONV_AscLong((unsigned char *)pcDate+6,2);
	}
	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	��ֵ������ת���ַ�����
��ڲ�����
	iYear:			�꣬��ʽ"YYYY"
	iMonth:			�£���ʽ"MM"
	iDay:			�գ���ʽ"DD"
���ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
����ֵ��
	����ִ�н��
������
*/
long DATE_ConvIntStr(int iYear,int iMonth,int iDay,char *pcDate)
{
	long lResult;
	char acDate[DATE_DATESTRLEN+1];

	if( !pcDate )
		return(DATE_PARAM_ERR);

	memset(acDate,0,sizeof(acDate));
	sprintf(acDate,"%04d%02d%02d",iYear,iMonth,iDay);
	lResult = DATE_ValideDateStr(pcDate);
	if( lResult != DATE_SUCCESS)
		return(lResult);

	strcpy(pcDate,acDate);
	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�ַ�����ת���ڲ���ʽ
��ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
���ڲ�����
	plDate:			�ڲ���ʽ������
������
	�ڲ���ʽ��������ָ��ǰ���ڵ�1899��12��31�ռ��������
	1900/01/01���ڲ���ʽΪ1��
*/
long DATE_ConvStrLong(char *pcDate,long *plDate)
{
	int iYear;
	int iYears;
	int iLeapYears;
	long lDays;
	int iYearDays;
	long lResult;

	if(  (!pcDate )
	   ||(!plDate )
	  )
		return(DATE_PARAM_ERR);
	lResult = DATE_ValideDateStr(pcDate);
	if( lResult != DATE_SUCCESS)
		return(lResult);

	iYear = (int)CONV_AscLong((unsigned char *)pcDate,4);

	iYears = iYear - 1900;
	if( iYears > 1)
	{
		iLeapYears = (iYears-1)/4;
	}else
	{
		iLeapYears = 0;
	}
	if( iYears > 100 )
	{
		iLeapYears = iLeapYears-((iYears-100-1)/100)+((iYears-100-1)/400);
	}

	lDays = iYears*365;
	lDays += iLeapYears;
	lResult = DATE_OneYearDaysStr(pcDate,&iYearDays );
	if( lResult != DATE_SUCCESS)
		return(lResult);
	lDays += iYearDays;

	*plDate = lDays;
	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�ڲ���ʽ����ת���ַ���ʽ
��ڲ�����
	lDate:			�ڲ���ʽ������
���ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
������
	�ڲ���ʽ��������ָ��ǰ���ڵ�1899��12��31�ռ��������
	1900/01/01���ڲ���ʽΪ1��
*/
long DATE_ConvLongStr(long lDate,char *pcDate)
{
	long lResult;
	int iYear;
	int iMonth;
	int iMonthDays;
	int iYearDays;
	int iDay;
	long lDays;
	long lLastDays;
	char acDate[DATE_DATESTRLEN+1];

	if(  (!pcDate )
	   ||( lDate<1 )
	  )
		return(DATE_PARAM_ERR);

	lDays = 0;
	lLastDays = 0;
	for(iYear = 1900;lDays<lDate;iYear++)
	{
		lLastDays = lDays;
		lDays += 365;
		if( DATE_LeapYearInt(iYear) == DATE_SUCCESS)
			lDays ++;
	}
	iYear --;
	iYearDays = lDate-lLastDays;
	lDays = 0;
	for(iMonth=1;lDays<iYearDays;iMonth++)
	{
		lLastDays = lDays;
		lResult = DATE_MonthDaysInt(iYear,iMonth,&iMonthDays);
		if( lResult != DATE_SUCCESS)
			return(lResult);
		lDays += iMonthDays;
	}
	iMonth --;
	iDay = (int)(iYearDays-lLastDays);

	memset(acDate,0,sizeof(acDate));
	sprintf(acDate,"%04d%02d%02d",iYear,iMonth,iDay);
	if( pcDate )
	    strcpy(pcDate,acDate);

	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�������Ƹ�ʽ������ת���ַ���ʽ
��ڲ�����
	pcDate1:		�����Ƹ�ʽ�����ڣ���ʽ"MMM DD YYYY"
���ڲ�����
	pcDate2:		�����գ���ʽ"YYYYMMDD"
������
	�ڲ���ʽ��������ָ��ǰ���ڵ�1899��12��31�ռ��������
	1900/01/01���ڲ���ʽΪ1��
*/
long DATE_ConvDateNameStr(char *pcDate1,char *pcDate2)
{
	unsigned char ucI;
	char acDate[DATE_DATESTRLEN+1];
	char acMon[3];

	if( !pcDate1 )
		return(DATE_PARAM_ERR);

	memset(acDate,0,sizeof(acDate));
	memcpy(&acDate[0],pcDate1+7,4);
	for(ucI=0;ucI<3;ucI++)
	{
	    if(  (*(pcDate1+ucI) >= 'a')
	       &&(*(pcDate1+ucI) <= 'z')
	      )
	    {
	        acMon[ucI] = *(pcDate1+ucI) - 0x20;
	        continue;
	    }
	    acMon[ucI] = *(pcDate1+ucI);
	}
	for(ucI=0;ucI<12;ucI++)
	{
	    if( !memcmp(acMon,MonTab[ucI].acMonName,3))
	    {
	        memcpy(&acDate[4],MonTab[ucI].acMon,2);
	        break;
	    }
	}
	memcpy(&acDate[6],pcDate1+4,2);
	if( pcDate2 )
	    strcpy(pcDate2,acDate);

	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	ȡ���ַ��������µ��������
��ڲ�����
	pcDate:			�꣬��ʽ"YYYYMM"
���ڲ�����
	piDays:			����
*/
long DATE_MonthDaysStr(char *pcDate, int *piDays )
{
	int iYear;
	int iMonth;

	if(  ( !pcDate )
	   ||( !piDays )
	  )
		return(DATE_PARAM_ERR);

	iYear = (int)CONV_AscLong((unsigned char *)pcDate,4);
	iMonth = (int)CONV_AscLong((unsigned char *)pcDate+4,2);
	return(DATE_MonthDaysInt(iYear,iMonth,piDays));
}

/*
EN:

zh_CN:
	ȡ��ĳ�µ��������
��ڲ�����
	iYear:			�꣬��ʽ"YYYY"
	iMonth:			�£���ʽ"MM"
���ڲ�����
	piDays:			����
*/
long DATE_MonthDaysInt(int iYear,int iMonth, int *piDays )
{
	if( !piDays )
		return(DATE_PARAM_ERR);

	switch(iMonth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		*piDays = 31;
		return(DATE_SUCCESS);
	case 2:
		if( DATE_LeapYearInt(iYear) == DATE_SUCCESS)
			*piDays = 29;
		else
			*piDays = 28;
		return(DATE_SUCCESS);
	case 4:
	case 6:
	case 9:
	case 11:
		*piDays = 30;
		return(DATE_SUCCESS);
	default:
		return(DATE_INVALIDMONTH_ERR);
	}
}

/*
EN:

zh_CN:
	ȡ������������
��ڲ�����
	pcDate:			�꣬��ʽ"YYYY"
���ڲ�����
	piDays:			����
*/
long DATE_YearDaysStr(char *pcDate,int *piDays )
{
	int iYear;

	if(  (!pcDate)
	   ||(!piDays)
	  )
		return(DATE_PARAM_ERR);

	iYear = (int)CONV_AscLong((unsigned char *)pcDate,4);

	return(DATE_YearDaysInt(iYear,piDays));
}

/*
EN:

zh_CN:
	ȡ������������
��ڲ�����
	iYear:			�꣬��ʽ"YYYY"
���ڲ�����
	piDays:			����
*/
long DATE_YearDaysInt(int iYear,int *piDays )
{
	if( !piDays )
		return(DATE_PARAM_ERR);

	if( DATE_LeapYearInt(iYear) == DATE_SUCCESS)
		*piDays = 366;
	else
		*piDays = 365;
	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�ַ������ڱ��������
��ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
���ڲ�����
	piDays:			������01/01��ʾ��1��
*/
long DATE_OneYearDaysStr(char *pcDate,int *piDays )
{
	long lResult;
	int iDays;
	int iYear;
	int iMonth;
	int iDay;
	int iI;
	int iTotalDays;

	if(  (!pcDate)
	   ||(!piDays )
	  )
		return(DATE_PARAM_ERR);

	lResult = DATE_ValideDateStr(pcDate);
	if( lResult != DATE_SUCCESS)
		return(lResult);

	iTotalDays = 0;
	iYear = (int)CONV_AscLong((unsigned char *)pcDate,4);
	iMonth = (int)CONV_AscLong((unsigned char *)pcDate+4,2);
	iDay = (int)CONV_AscLong((unsigned char *)pcDate+6,2);
	for(iI=1;iI<iMonth;iI++)
	{
		lResult = DATE_MonthDaysInt( iYear, iI, &iDays );
		if( lResult != DATE_SUCCESS)
			return(lResult);
		iTotalDays += iDays;
	}
	iTotalDays += iDay;
	*piDays = iTotalDays;
	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�������ַ������������
��ڲ�����
	pcDate1:		�����գ���ʽ"YYYYMMDD"
	pcDate2:		�����գ���ʽ"YYYYMMDD"
���ڲ�����
	piDays:			����
������
	pcDate1����pcDate2ʱ������������
	pcDate1����pcDate2ʱ����0
	pcDate1С��pcDate2ʱ���ظ�������
*/
long DATE_DiffDaysStr(char *pcDate1,char *pcDate2,long *plDays)
{
	long lResult;
	long lDate1;
	long lDate2;

	lResult = DATE_ValideDateStr(pcDate1);
	if( lResult )
		return(lResult);
	lResult = DATE_ValideDateStr(pcDate2);
	if( lResult )
		return(lResult);
	lResult = DATE_ConvStrLong(pcDate1,&lDate1);
	if( lResult )
		return(lResult);
	lResult = DATE_ConvStrLong(pcDate2,&lDate2);
	if( lResult )
		return(lResult);

	*plDays = lDate1-lDate2;

	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�������ڼ�
��ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
���ڲ���:
    piWeekDay:  	���ڼ�
    				0: ������;
    				1: ����һ;
    				.
    				.
    				.
    				6: ������.
*/
long DATE_WeekDayStr(char *pcDate,int *piWeekDay)
{
	long lResult;
	long lDate;
	int iWeekDay;

	if(  (!pcDate)
	   ||(!piWeekDay )
	  )
		return(DATE_PARAM_ERR);

	lResult = DATE_ConvStrLong(pcDate,&lDate);
	if( lResult )
		return(lResult);

	iWeekDay = (int)((lDate-1)%7);
	*piWeekDay = iWeekDay;

	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�ַ�������ǰ��(����)����
��ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
    iYearChange:	����  (������ʾ��ǰ, ������ʾ�˺�).
���ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
������
	�Զ���������Ĵ���
	2000/02/29��һ�����1999/02/28
*/
long DATE_YearChangeStr(char *pcDate,int iYearChange)
{
	long lResult;
	int iYear;
	int iMonth;
	int iDay;
/*
	unsigned char ucLeapYearFlag;
*/
	char acDate[DATE_DATESTRLEN+1];

	if(!pcDate)
		return(DATE_PARAM_ERR);

	lResult = DATE_ValideDateStr(pcDate);
	if( lResult != DATE_SUCCESS )
		return(lResult);

	iYear = (int)CONV_AscLong((unsigned char *)pcDate,4);
	iMonth = (int)CONV_AscLong((unsigned char *)pcDate+4,2);
	iDay = (int)CONV_AscLong((unsigned char *)pcDate+6,2);
/*
	if( DATE_LeapYearInt(iYear) == DATE_SUCCESS)
		ucLeapYearFlag = TRUE;
	else
		ucLeapYearFlag = FALSE;
*/
	iYear += iYearChange;
	if(  (DATE_LeapYearInt(iYear) != DATE_SUCCESS)
	   &&(iMonth == 2)
	   &&(iDay == 29)
	  )
	{
		iDay --;
	}
	lResult = DATE_ValideDateInt(iYear,iMonth,iDay);
	if( lResult != DATE_SUCCESS)
		return(lResult);

	memset(acDate,0,sizeof(acDate));
	sprintf(acDate,"%04d%02d%02d",iYear,iMonth,iDay);

	strcpy(pcDate,acDate);
	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�ַ�������ǰ��(����)����
��ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
    iMonthChange:	����  (������ʾ��ǰ, ������ʾ�˺�).
���ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
������
	�Զ�����ÿ�����ڱ߽�
*/
long DATE_MonthChangeStr(char *pcDate,int iMonthChange)
{
	long lResult;
	int iYear;
	int iMonth;
	int iDay;
	int iMonthDays;
	char acDate[DATE_DATESTRLEN+1];

	if(!pcDate)
		return(DATE_PARAM_ERR);

	lResult = DATE_ValideDateStr(pcDate);
	if( lResult != DATE_SUCCESS )
		return(lResult);

	iYear = (int)CONV_AscLong((unsigned char *)pcDate,4);
	iMonth = (int)CONV_AscLong((unsigned char *)pcDate+4,2);
	iDay = (int)CONV_AscLong((unsigned char *)pcDate+6,2);

	iYear += iMonthChange/12;
	iMonth += iMonthChange%12;

	if( iMonth>12)
	{
		iMonth -= 12;
		iYear += 1;
	}else
	{
		if( iMonth <=0 )
		{
			iMonth += 12;
			iYear -= 1;
		}
	}
	lResult = DATE_MonthDaysInt(iYear,iMonth,&iMonthDays);
	if( lResult != DATE_SUCCESS)
		return(lResult);
	if( iDay > iMonthDays )
		iDay = iMonthDays;

	memset(acDate,0,sizeof(acDate));
	sprintf(acDate,"%04d%02d%02d",iYear,iMonth,iDay);

	strcpy(pcDate,acDate);
	return(DATE_SUCCESS);
}

/*
EN:

zh_CN:
	�ַ�������ǰ��(����)����
��ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
    iDayChange:		����  (������ʾ��ǰ, ������ʾ�˺�).
���ڲ�����
	pcDate:			�����գ���ʽ"YYYYMMDD"
������
*/
long DATE_DayChangeStr(char *pcDate,long lDayChange)
{
	long lResult;
	long lDays;
	char acDate[DATE_DATESTRLEN+1];

	if(!pcDate)
		return(DATE_PARAM_ERR);

	lResult = DATE_ValideDateStr(pcDate);
	if( lResult != DATE_SUCCESS )
		return(lResult);

	lResult = DATE_ConvStrLong(pcDate,&lDays);
	if( lResult != DATE_SUCCESS)
		return(lResult);

	lDays += lDayChange;
	if( lDays < 1)
		return(DATE_OVERBOUND);

	memset(acDate,0,sizeof(acDate));
	lResult = DATE_ConvLongStr(lDays,acDate);
	if( lResult != DATE_SUCCESS)
		return(lResult);

	strcpy(pcDate,acDate);
	return(DATE_SUCCESS);
}
