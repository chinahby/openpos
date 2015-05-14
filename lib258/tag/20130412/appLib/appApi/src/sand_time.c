#include <stdio.h>
#include <stdlib.h>
#include <asm/param.h> //for HZ define
#include <fcntl.h>
#include <time.h>
#include <linux/rtc.h>

#include "sand_incs.h"
#include "timer_driver.h"


#ifdef EN_DBG
#define dbg Uart_Printf
#else
#define dbg
#endif

#define DEV_RTC "/dev/misc/rtc"
static int fd_timer;	//��ʱ��ʹ��

#if 1
#undef RTC_RD_TIME
#undef RTC_SET_TIME
#define RTC_RD_TIME	_IOR('p', 0x09, struct rtc_time )/* Read RTC time   */
#define RTC_SET_TIME	_IOW('p', 0x0a, struct rtc_time) /* Set RTC time    */
#endif

static inline void os_sleep(__time_t sec,__suseconds_t us)
{
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = us;
    select(0, NULL, NULL, NULL, &tv);
}


/*
����	��ʹ������ʱ�����ж�
����	�� unsigned char Os__xdelay (unsigned short Delay) ;
�ȴ�	����
˵��	��ʹ���̲���һ���ж�, ÿһ����λΪ10ms (HZ=100(1s), HZ/1000=1ms)
����ֵ 	�� OK = �ɹ�  �� OK    = ����
*/
unsigned char Os__xdelay (unsigned int Delay)
{
	
    unsigned int t=Delay*10000;
    os_sleep(t/1000000,t%1000000);
/*    
    unsigned int time=0;
    time=10000*Delay*(HZ/100);
    //printf("%d\n",time);
    usleep(time);
*/
    
    return 0;
}

/*
����	����EFT �ն����ں�ʱ��
����	: Os__read_date_time Os__read_date_time (unsigned char *ptDateTime) ;
�ȴ�	����
˵��	  ����ϵͳ�ĵ����ں�ʱ�䲢�����' ptDateTime 'ָ���BUFFER��
         'date and time' ����ΪASCII �������и�ʽ��ţ�
         HourHourMinuteMinuteSecondSecondDayDayMonthMonthYearYear\0.
         HHMMSSDDMMYY
         ���ʱ������ڷ���������, �������� '00000000\0'��
����ֵ����
*/
unsigned char Os__read_date_time (unsigned char *ptDateTime)
{
    unsigned char tm_mday[5], tm_sec[5], tm_mon[5], tm_year[5], tm_min[5],tm_hour[50],getDate[40];
	char year;
    time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);

    memset(getDate, 0, 40);
    memset(tm_mday,0,5);
    memset(tm_mon,0,5);
    memset(tm_year,0,5);
    memset(tm_hour,0,50);
    memset(tm_min,0,5);
    memset(tm_sec,0,5);


	p->tm_mon++;
	p->tm_year -= 100;
	//����δ����ʱ��ʱ��ȡ����
	if(p->tm_year < 0)
		p->tm_year = 70;

    sprintf(tm_hour, "%02d", p->tm_hour);
	sprintf(tm_min, "%02d", p->tm_min);
	sprintf(tm_sec, "%02d", p->tm_sec);
	sprintf(tm_mday, "%02d", p->tm_mday);
	sprintf(tm_mon, "%02d", p->tm_mon);	
	sprintf(tm_year, "%02d", p->tm_year);
	

	strcat(tm_hour, tm_min);
	//strcat(tm_hour, tm_sec);
	strcat(tm_hour, tm_mday);
	strcat(tm_hour, tm_mon);
	strcat(tm_hour, tm_year);
	
	#if 0 
	Uart_Printf("tm_hour is %s\n", tm_hour);
	Uart_Printf("tm_min is %s\n", tm_min);
	Uart_Printf("tm_sec is %s\n", tm_sec);
	Uart_Printf("tm_mday is %s\n", tm_mday);
	Uart_Printf("tm_mon is %s\n", tm_mon);
	Uart_Printf("tm_year is %s\n", tm_year);
	
	#endif
	
	strcpy(getDate, tm_hour);
	
    memcpy(ptDateTime, getDate, 11);

    return 0;
}



/*
����	����EFT�ն�ʱ�䡣
����	�� void Os__read_time (unsigned char *ptTime) ;
�ȴ�	����
˵��	��Os__read_time ���õ�ϵͳʱ�佫����� 'ptTime'ָ���BUFFER��,
ʱ�佫��ASCII ���, ��Ÿ�ʽΪ�� HHMM\0.
���ʱ��������, Os__read_time ���� '0000\0' ��
����ֵ	����
*/
void Os__read_time (unsigned char *ptTime)
{
    unsigned char tm_min[5],tm_hour[15],getDate[40], ret;
	unsigned char tbuff[10];

	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	
    memset(ptTime, 0, 4);
    memset(getDate, 0, 15);

	sprintf(tm_hour, "%02d", p->tm_hour);
	sprintf(tm_min, "%02d", p->tm_min);
	strcat(tm_hour, tm_min);
	strcpy(getDate, tm_hour);
	
	
    memcpy(ptTime, getDate, 5);
}


/*
����	����ʱ���ʽ��ϵͳʱ��
����	��void Os__read_time_format (unsigned char *ptTime) ;
�ȴ�	����
˵��	�� 	Os__read_time_format ��ʱ���ʽ����ϵͳʱ��, ���õ�ϵͳʱ������'ptTime'ָ���BUFFER�С�
			ʱ����ASCII��, �����и�ʽ��ţ�HourHour:MinuteMinute\0, �������ʱ��������, ��������'00:00\0' ��
����ֵ	����
*/
void Os__read_time_format (unsigned char *ptTime)
{
    unsigned char tm_min[5],tm_hour[12],getDate[15];
    
    memset(getDate, 0, 15);
    memset(tm_hour,0,12);
    memset(tm_min,0,5);

   
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);

 	sprintf(tm_hour, "%02d", p->tm_hour);
	sprintf(tm_min, "%02d", p->tm_min);
	strcat(tm_hour, ":");
	strcat(tm_hour, tm_min);
	strcpy(getDate, tm_hour);

    memcpy(ptTime, getDate, 6);
}

/*
����	����ϵͳʱ�䵽�롣
����	�� void Os__read_time_sec (unsigned char *ptTime) ;
�ȴ�	����
����    : Os__read_time_sec���õ�ϵͳʱ�佫����� 'ptTime'ָ���BUFFER��,
		ʱ�佫��ASCII ���, ��Ÿ�ʽΪ��HourHourMinuteMinuteSecondSecond\0.
���ʱ��������, Os__read_time_sec����'000000\0' ��
����ֵ  ����
*/
void Os__read_time_sec (unsigned char *ptTime)
{
    unsigned char tm_mday[5], tm_sec[5], tm_mon[5], tm_year[5], tm_min[5],tm_hour[50],getDate[40];
	char year;
    time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);

    memset(getDate, 0, 40);
    memset(tm_mday,0,5);
    memset(tm_mon,0,5);
    memset(tm_year,0,5);
    memset(tm_hour,0,50);
    memset(tm_min,0,5);
    memset(tm_sec,0,5);


	p->tm_mon++;
	p->tm_year -= 100;
	
    sprintf(tm_hour, "%02d", p->tm_hour);
	sprintf(tm_min, "%02d", p->tm_min);
	sprintf(tm_sec, "%02d", p->tm_sec);
	sprintf(tm_mday, "%02d", p->tm_mday);
	sprintf(tm_mon, "%02d", p->tm_mon);	
	sprintf(tm_year, "%02d", p->tm_year);
	

	strcat(tm_hour, tm_min);
	strcat(tm_hour, tm_sec);
	
	
	#if 0 
	Uart_Printf("tm_hour is %s\n", tm_hour);
	Uart_Printf("tm_min is %s\n", tm_min);
	Uart_Printf("tm_sec is %s\n", tm_sec);
	Uart_Printf("tm_mday is %s\n", tm_mday);
	Uart_Printf("tm_mon is %s\n", tm_mon);
	Uart_Printf("tm_year is %s\n", tm_year);
	
	#endif
	
	strcpy(getDate, tm_hour);
	
    memcpy(ptTime, getDate, 7);

   
}

/*
����	����EFT�ն�����
����	�� void Os__read_date (unsigned char *ptDate) ;
�ȴ�	����
˵��	��Os__read_date��ϵͳʱ�䵽bufferָ�� 'ptDate',
������ASCII ����ʽDayDayMonthMonthYearYear\0.���, ���ϵͳʱ��������, ϵͳ����'000000\0'��
		DDMMYY
����ֵ	����
*/
void Os__read_date (unsigned char *ptDate)
{
    unsigned char tm_mday[5], tm_sec[5], tm_mon[5], tm_year[5], tm_min[5],tm_hour[50],getDate[40];
	char year;
    time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);

    memset(getDate, 0, 40);
    memset(tm_mday,0,5);
    memset(tm_mon,0,5);
    memset(tm_year,0,5);
    memset(tm_hour,0,50);
    memset(tm_min,0,5);
    memset(tm_sec,0,5);


	p->tm_mon++;
	p->tm_year -= 100;
	//����δ����ʱ��ʱ��ȡ����
	if(p->tm_year < 0)
		p->tm_year = 70;
	
    sprintf(tm_hour, "%02d", p->tm_hour);
	sprintf(tm_min, "%02d", p->tm_min);
	sprintf(tm_sec, "%02d", p->tm_sec);
	sprintf(tm_mday, "%02d", p->tm_mday);
	sprintf(tm_mon, "%02d", p->tm_mon);	
	sprintf(tm_year, "%02d", p->tm_year);
	

	strcat(tm_mday, tm_mon);
	strcat(tm_mday, tm_year);
	
	
	#if 0 
	Uart_Printf("tm_hour is %s\n", tm_hour);
	Uart_Printf("tm_min is %s\n", tm_min);
	Uart_Printf("tm_sec is %s\n", tm_sec);
	Uart_Printf("tm_mday is %s\n", tm_mday);
	Uart_Printf("tm_mon is %s\n", tm_mon);
	Uart_Printf("tm_year is %s\n", tm_year);
	
	#endif
	
	strcpy(getDate, tm_mday);
	
    memcpy(ptDate, getDate, 7);

    
   

}

/*
����	����ϵͳ���ڲ���ʽ��
����	�� void Os__read_date_format (unsigned char *ptDate) ;
�ȴ�	����
˵��	�� Os__read_date_format ��ϵͳ����, �������'ptDate'.ָ���BUFFER��
         ��ASCII �Ĵ�Ÿ�ʽΪ��
         DayDay/MonthMonth/YearYear\0.
         �������������, �������� '00/00/00\0'��
����ֵ	����
*/
void Os__read_date_format (unsigned char *ptDate)
{

	 unsigned char tm_mday[5], tm_sec[5], tm_mon[5], tm_year[5], tm_min[5],tm_hour[50],getDate[40];
	char year;
    time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);

    memset(getDate, 0, 40);
    memset(tm_mday,0,5);
    memset(tm_mon,0,5);
    memset(tm_year,0,5);
    memset(tm_hour,0,50);
    memset(tm_min,0,5);
    memset(tm_sec,0,5);


	p->tm_mon++;
	p->tm_year -= 100;
	//����δ����ʱ��ʱ��ȡ����
	if(p->tm_year < 0)
		p->tm_year = 70;
			
    sprintf(tm_hour, "%02d", p->tm_hour);
	sprintf(tm_min, "%02d", p->tm_min);
	sprintf(tm_sec, "%02d", p->tm_sec);
	sprintf(tm_mday, "%02d", p->tm_mday);
	sprintf(tm_mon, "%02d", p->tm_mon);	
	sprintf(tm_year, "%02d", p->tm_year);
	
	strcat(tm_mday, "/");
	strcat(tm_mday, tm_mon);
	strcat(tm_mday, "/");
	strcat(tm_mday, tm_year);
	
	
	#if 0 
	Uart_Printf("tm_hour is %s\n", tm_hour);
	Uart_Printf("tm_min is %s\n", tm_min);
	Uart_Printf("tm_sec is %s\n", tm_sec);
	Uart_Printf("tm_mday is %s\n", tm_mday);
	Uart_Printf("tm_mon is %s\n", tm_mon);
	Uart_Printf("tm_year is %s\n", tm_year);
	
	#endif
	
	strcpy(getDate, tm_mday);
	
    memcpy(ptDate, getDate, 9);


	
}


#if 0
static void change_system_date(unsigned char *d)
{

	int p_status;
	pid_t PID = fork();
	
	if (PID == 0) //child
	{
		
		if (execl("/bin/date", "date", "-s", d, NULL) < 0) {
			Uart_Printf("date set failed\n");
			exit(1);
		
		}

	}

	else if (PID < 0)
	{
 		Uart_Printf("process failed\n");
		exit(1);
	}

	else //parent
	{
		
		wait(&p_status);
		
		
	}



}


static void sync_rtc_time()
{

	int p_status;
	pid_t PID = fork();
	
	if (PID == 0) //child
	{
		if (execl("/sbin/hwclock", "hwclock", "-w") < 0) {
			Uart_Printf("rtc set failed\n");
			exit(1);
		
		}

	}

	else if (PID < 0)
	{
 		Uart_Printf("process failed\n");
		exit(1);
	}

	else //parent
	{
		
		wait(&p_status);
		
		
	}



}
#endif

/** 2011/6/9   **/
static void change_system_date(unsigned char *d)
{
	char cmd[128];

	memset(cmd, 0, 128);
	sprintf(cmd, "/bin/date -s %s",d);
	system(cmd);
}


static void sync_rtc_time()
{
	char cmd[128];

	memset(cmd, 0, 128);
	sprintf(cmd, "/sbin/hwclock -w");
	system(cmd);

}

/** end 2011/6/9  **/



/*
����	�������µ�ϵͳ����
����	�� unsigned char Os__write_date (unsigned char *ptDate) ;
�ȴ�	����
˵��	�� Os__write_date �����µ�ϵͳ����,  'ptDate' �Ǵ��ʱ��BUFFERָ��,
	BUFFER ����ΪASCII, ��Ÿ�ʽΪ YearYear.MONTHMONTH.DAYDAY\0
����ֵ �� OK  = ��ȷ
          ��OK = ����
*/
unsigned char Os__write_date_wrap(unsigned char *ptDate)
{

	unsigned char tm_mday[5], tm_sec[5], tm_mon[5], tm_year[5], tm_min[5],tm_hour[50],getDate[40];
	char year;
    time_t timep;
	struct tm *p;
    char temp[50];
	
	strcpy(temp, ptDate);
	
	 
	time(&timep);
	p = localtime(&timep);

    memset(getDate, 0, 40);
    memset(tm_mday,0,5);
    memset(tm_mon,0,5);
    memset(tm_year,0,5);
    memset(tm_hour,0,50);
    memset(tm_min,0,5);
    memset(tm_sec,0,5);


	p->tm_mon++;
	p->tm_year -= 100;
	
    sprintf(tm_hour, "%02d", p->tm_hour);
	sprintf(tm_min, "%02d", p->tm_min);
	sprintf(tm_sec, "%02d", p->tm_sec);
	sprintf(tm_mday, "%02d", p->tm_mday);
	sprintf(tm_mon, "%02d", p->tm_mon);	
	sprintf(tm_year, "%02d", p->tm_year);
	

	strcat(temp, "-");
	strcat(temp, tm_hour);
	strcat(temp, ":");
	strcat(temp, tm_min);
	strcat(temp, ":");
	strcat(temp, tm_sec);

	
	
	change_system_date(temp);
	sync_rtc_time();
   
 	return OK;

}

/*
����	���޸�EFT�ն�ʱ�䡣
����	��UCHAR Os__write_time (unsigned char *ptTime) ;
�ȴ�	����
˵��	��Os__write_timeҪ�޸ĵ�ϵͳʱ�佫����� 'ptTime'ָ���BUFFER��,
ʱ�佫��ASCII ���, ��Ÿ�ʽΪ�� HourHour:MinuteMinute:SSSS\0.
����ֵ ��OK = ��ȷ
��	   ��OK = ����
*/
unsigned char Os__write_time_wrap(unsigned char *ptTime)
{

	change_system_date(ptTime);
	sync_rtc_time();
    return OK;
}



unsigned char Os__write_date(unsigned char *ptDate)
{
	
	char day_temp[3];
	char month_temp[3];
	char year_temp[9];
	char result[11];
	unsigned int d_temp;
	char *rt;
	rt = result;
	rt += 2;

/*Check and validate date month year*/

	day_temp[0] = ptDate[0];
	day_temp[1] = ptDate[1];	
	day_temp[0] = day_temp[0] - 48;
	day_temp[1] = day_temp[1] - 48;
	d_temp = day_temp[0] * 10 + day_temp[1];

	if (d_temp >= 32)
		return 87;


	month_temp[0] = ptDate[2];
	month_temp[1] = ptDate[3];	
	month_temp[0] = month_temp[0] - 48;
	month_temp[1] = month_temp[1] - 48;
	d_temp = month_temp[0] * 10 + month_temp[1];

	if (d_temp >= 13)
		return 87;
	
	
	day_temp[0] = ptDate[0];
	day_temp[1] = ptDate[1];
	day_temp[2] = '\0';

	month_temp[0] = ptDate[2];
	month_temp[1] = ptDate[3];
	month_temp[2] = '\0';

	year_temp[0] = ptDate[4];
	year_temp[1] = ptDate[5];
	year_temp[2] = '\0';
	
	strcat(year_temp, ".");
	
	strcat(year_temp, month_temp);
	strcat(year_temp, ".");
	strcat(year_temp, day_temp);

	strcpy(rt, year_temp);
	
	result[0] = '2';
	result[1] = '0';

	Os__write_date_wrap(result);
	return OK;
	

}


unsigned char Os__write_time(unsigned char *ptTime)
{

	char hour_temp[9];
	char min_temp[3];
	unsigned int d_temp;


	/*Check and validate time*/

	hour_temp[0] = ptTime[0];
	hour_temp[1] = ptTime[1];	
	hour_temp[0] = hour_temp[0] - 48;
	hour_temp[1] = hour_temp[1] - 48;
	d_temp = hour_temp[0] * 10 + hour_temp[1];

	if (d_temp >= 25)
		return 87;


	min_temp[0] = ptTime[2];
	min_temp[1] = ptTime[3];	
	min_temp[0] = min_temp[0] - 48;
	min_temp[1] = min_temp[1] - 48;
	d_temp = min_temp[0] * 10 + min_temp[1];

	if (d_temp >= 60)
		return 87;
	
	
	
	
	hour_temp[0] = ptTime[0];
	hour_temp[1] = ptTime[1];
	hour_temp[2] = '\0';

	min_temp[0] = ptTime[2];
	min_temp[1] = ptTime[3];
	min_temp[2] = '\0';

	
	
	strcat(hour_temp, ":");
	
	strcat(hour_temp, min_temp);
	strcat(hour_temp, ":");
	strcat(hour_temp, "30");


	Os__write_time_wrap(hour_temp);

	return OK;
}




/*
ʲô����?
������MSG_ReadMach_Date
*/
void OSDATE_ReadTimeSec(unsigned char *pucTime)
{
    Os__read_time_sec(pucTime);
}



#if 0
/*
����	��������ʱ��
����	�� unsigned char Os__timer_start (unsigned short *ptTimer) ;
�ȴ�	����
˵��	��Os__timer_start ����һ����ʱ��, 'ptTimer' �Ƕ�ʱ��ʱ���ַ, ʱ�����10ms�ݼ�һ��,
		��ʱ��ݼ�����ʱ, ��ʱ����, ���Զ�ʱ����ʱһ����λΪ10ms(HZ/1000=0.01s=10ms).
����ֵ��OK   = �ɹ�
		��OK   = ���� (ϵͳʹ����̫��Ķ�ʱ��).
*/
unsigned char Os__timer_start (unsigned int *ptTimer)
{
    unsigned int Timeout;
    Timeout=(unsigned int )*ptTimer;

    fd_timer= open(TIMER_DEV_NAME, O_RDWR);
    if ( fd_timer== -1 )
    {
        Uart_Printf("Cann't open file ");
        return 1;
    }
    
//	Uart_Printf("start timer %d \n", Timeout);
    ioctl(fd_timer, CMD_PIT_ADD, &Timeout);

    return 0;
}
/*
����	��ֹͣ��ʱ
����	��void Os__timer_stop (unsigned short * ptTimer) ;
�ȴ�	����
˵��	��ֹͣ'Os__timer_start'��ʼ�Ķ�ʱ��, Ӧ�ñ���ֹͣ'Os__timer_start'�����򿪵Ķ�ʱ��,
		����MONOS�������ݼ���ʱ����, ֱ�����������ڣ����������ȫ�ֱ�������
ע��	����������ֹͣ�Զ��ݼ���ʱ������, ����Ӱ�����ֵ, ����Ӧ����ֹͣ��ʱ��ǰ���Լ���������ȷ�ԡ�
����ֵ	����
*/
void Os__timer_stop (unsigned int * ptTimer)
{
    if (!fd_timer)
    {
        Uart_Printf("timer has been closed!\n");
        return;
    }
    ioctl(fd_timer, CMD_PIT_STOP);
    close(fd_timer);
    fd_timer = -1;
    return;
}

#endif


unsigned int Os__timer_remain ()
{
    unsigned int remain;
    if (!fd_timer)
    {
        Uart_Printf("timer has been closed!\n");
        return -1;
    }
    ioctl(fd_timer, CMD_PIT_GET, &remain);

    return remain;
}

//��ʱ����
unsigned char OS__timer_test()
{
    volatile unsigned int Timeout, T2, i;

    Timeout=1000;
    T2=50;

    Os__timer_start((unsigned int*)&Timeout);
    Uart_Printf("start to dec %ld!\n", Timeout);

    do
    {
        T2 = Os__timer_remain();
        if (T2 < 888 ) break;
    }
    while (T2!=0);
    Uart_Printf("%s Timeout=%d\n", __func__, T2);


    Os__timer_stop((unsigned int*)&Timeout);
    T2 = Os__timer_remain();
    Uart_Printf("end T2=%d\n", T2);

    return 0;

    Timeout =10000;
    Os__timer_start((unsigned int*)&Timeout);
    Uart_Printf("start to dec %ld!\n", Timeout);

    do
    {
        //Uart_Printf(".");
        T2 = Os__timer_remain();
        if (T2==678)
        {
            Os__timer_stop((unsigned int *)&Timeout);
            break;
        }
    }
    while (T2!=0);

    T2 = Os__timer_remain();
    Uart_Printf("T2�� %ld\n", T2);


    return 0;
}


/*************************************
 * ʱ�����ò��Գ���
 *************************************/
void Os_time_test(char *pDate)
{
    unsigned char test1[20], *test;

    test=test1;
    memset(test, 0, 20);

    memset(test, 0, 20);

#if 0
    if (pDate==NULL)
        strcpy(test, "010316465508");
    else
        memcpy(test, pDate, strlen(pDate) );

    OS__write_date_time(test);
    Uart_Printf("Os_write_date_time, HHMMSSDDMMYY: %s\n", test);
#endif


    memset(test, 0, 20);
    Os__read_date_time(test);
    Uart_Printf("Os__read_date_time, HHMMSSDDMMYY: %s\n", test);

#if 0
    memset(test, 0, 20);
    Os__read_date(test);
    Uart_Printf("\nOs__read_date, DDMMYY: %s\n", test);

    memset(test, 0, 20);
    Os__read_date_format(test);
    Uart_Printf("Os__read_date_format, DD/MM/YY: %s\n", test);


    memset(test, 0, 20);
    Os__read_time(test);
    Uart_Printf("Os__read_time, HHMM: %s\n", test);

    memset(test, 0, 20);
    Os__read_time_format(test);
    Uart_Printf("Os__read_time_format, HH:MM: %s\n", test);

    memset(test, 0, 20);
    Os__read_time_sec(test);
    Uart_Printf("Os__read_time_sec, HHMMSS: %s\n", test);
#endif


#if 0
    Uart_Printf("\n\nI will set time & date\n");
    memset(test,0, 20);
    strcpy(test, "251208");
    Os__write_date(test);

    memset(test, 0, 20);
    Os__read_date_time(test);
    Uart_Printf("Os__read_date_time, HHMMDDMMYY: %s\n", test);

    memset(test,0, 20);
    memcpy(test, "192030", 6);
    Os__write_time(test);

    memset(test, 0, 20);
    Os__read_date_time(test);
    Uart_Printf("Os__read_date_time, HHMMSSDDMMYY: %s\n", test);
#endif

}

/*************************************
 * ��ʱ�����Գ���
 *************************************/
int OS_timer_test()
{
    int fd;
    int result;
    int i;
    char buf[]="helloworld";
    char inbuf[20];
    unsigned long ioctltest;
    unsigned long delaytime=1000, remain=1;

    memset(inbuf, 0, 20);

    fd = open(TIMER_DEV_NAME, O_RDWR);
    if ( fd == -1 )
    {
        Uart_Printf("Cann't open file\n");
        exit(0);
    }

    ioctl(fd, CMD_PIT_ADD, &delaytime);

#if 1
    do
    {
        ioctl(fd, CMD_PIT_GET, &remain);
        Uart_Printf("remain=%d\n", remain);
    }
    while (remain>500);
#endif
    Uart_Printf("stop!\n");

exit:
    close(fd);
    return 0;
}


#if 0
int main(char argc, char *argv[])
{
    char *pDate=NULL;
    char cmd;
    unsigned char test[20];

    OS__timer_test();
    //OS_timer_test();
    return 0;

    memset(test, 0, 20);
    if (argc<3)
    {
        cmd='r';
        memset(test, 0, 20);
        Os__read_date_time(test);
        Uart_Printf("Os__read_date_time, HHMMSSDDMMYY: %s\n", test);

        memset(test, 0, 20);
        Os__read_date(test);
        Uart_Printf("\nOs__read_date, DDMMYY: %s\n", test);

        memset(test, 0, 20);
        Os__read_date_format(test);
        Uart_Printf("Os__read_date_format, DD/MM/YY: %s\n", test);


        memset(test, 0, 20);
        Os__read_time(test);
        Uart_Printf("Os__read_time, HHMM: %s\n", test);

        memset(test, 0, 20);
        Os__read_time_format(test);
        Uart_Printf("Os__read_time_format, HH:MM: %s\n", test);

        memset(test, 0, 20);
        Os__read_time_sec(test);
        Uart_Printf("Os__read_time_sec, HHMMSS: %s\n", test);
    }
    else if (argc==3)
    {
        cmd='w';
        pDate=argv[2];
        OS__write_date_time(pDate);
        Uart_Printf("Os_write_date_time, MMDDHHMMSSYY: %s\n", pDate);

        memset(test, 0, 20);
        Os__read_date_time(test);
        Uart_Printf("Os__read_date_time, HHMMSSDDMMYY: %s\n", test);

        Uart_Printf("set date(DDMMYY):\n");
        gets(test);
        Os__write_date(test);
        Uart_Printf("OS__write_date, DDMMYY: %s\n", test);

        memset(test, 0, 20);
        Os__read_date_time(test);
        Uart_Printf("Os__read_date_time, HHMMSSDDMMYY: %s\n", test);

        Uart_Printf("set time(HHMMSS):\n");
        gets(test);
        Os__write_time(test);
        Uart_Printf("OS__write_time, HHMMSS: %s\n", test);

        memset(test, 0, 20);
        Os__read_date_time(test);
        Uart_Printf("Os__read_date_time, HHMMSSDDMMYY: %s\n", test);
    }

}
#endif