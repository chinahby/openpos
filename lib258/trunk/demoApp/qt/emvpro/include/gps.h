#ifndef GPS_H_INCLUDE
#define GPS_H_INCLUDE

#ifdef __cplusplus
extern "C"{
#endif

#define C_GPGGA "GPGGA"
#define C_GPGSA "GPGSA"
#define C_GPGSV "GPGSV"
#define C_GPGLL "GPGLL"
#define C_GPRMC "GPRMC"
#define C_GPVTG "GPVTG"
#define C_GPZDA "GPZDA"
 

/**
 * @defgroup NMEA-0183Э����������
 * @author �ܽ�
 * @brief �ṩ��NMEA-0183Э����$GPGGA��$GPGSA��$GPGSV��$GPRMC��$GPVTG��$GPGLL�Ƚ��н���
 * @{
 */

/*
** GGA - Global Positioning System Fix Data
** Time, Position and fix related data fora GPS receiver.
**
**                                                      11
**        1         2       3 4        5 6 7  8   9  10 |  12 13  14   15
**        |         |       | |        | | |  |   |   | |   | |   |    |
** $--GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>
**
** Field Number:
**  1) Universal Time Coordinated (UTC)
**  2) Latitude
**  3) N or S (North or South)
**  4) Longitude
**  5) E or W (East or West)
**  6) GPS Quality Indicator,
**     0 - fix not available,
**     1 - GPS fix,
**     2 - Differential GPS fix
**  7) Number of satellites in view, 00 - 12
**  8) Horizontal Dilution of precision
**  9) Antenna Altitude above/below mean-sea-level (geoid)
** 10) Units of antenna altitude, meters
** 11) Geoidal separation, the difference between the WGS-84 earth
**     ellipsoid and mean-sea-level (geoid), "-" means mean-sea-level
**     below ellipsoid
** 12) Units of geoidal separation, meters
** 13) Age of differential GPS data, time in seconds since last SC104
**     type 1 or 9 update, null field when DGPS is not used
** 14) Differential reference station ID, 0000-1023
** 15) Checksum
*/

typedef struct
{
    char    UTCTime[11];                    //UTC ʱ�⣬hhmmss.sss��ʱ�����ʽ
    double  LatitudePositionFieldNumber;    //γ��ddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��
    char    Latitude;                       //γ��N����γ����S����γ��
    double  LongitudePositionFieldNumber;   //����dddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��
    char    Longitude;                      //����E����������W��������
    int     GPSQuality;                     //GPS״̬��0=δ��λ��1=�ǲ�ֶ�λ��2=��ֶ�λ��3=��ЧPPS��6=���ڹ���
    int     NumberOfSatellitesInUse;        //����ʹ�õ�����������00 - 12����ǰ��λ��������0��
    double  HorizontalDilutionOfPrecision;  //HDOPˮƽ�������ӣ�0.5 - 99.9��
    double  AntennaAltitudeMeters;          //���θ߶ȣ�-9999.9 - 99999.9��
    //char    UnitsOfAntennaAltitudeMeters;   //���θ߶ȵ�λ(m) ����m
    double  GeoidalSeparationMeters;        //������������Դ��ˮ׼��ĸ߶�
    //char    UnitsOfGeoidalSeparationMeters; //���ˮ׼�������λ(m)   ����m
    double  AgeOfDifferentialGPSDataSeconds;//���ʱ�䣨�����һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ��Ϊ�գ�
    int     DifferentialReferenceStationID; //���վID��0000 - 1023��ǰ��λ��������0��������ǲ�ֶ�λ��Ϊ�գ�
}GPGGA;



/*
** GSA - GPS DOP and Active Satellites
**
**        1 2 3  4  5  6  7  8  9  10 11 12 13 14 15  16  17  18
**        | | |  |  |  |  |  |  |  |  |  |  |  |  |   |   |   |
** $--GSA,a,x,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,x.x,x.x,x.x*hh<CR><LF>
**
** Field Number:
**  1) Operating Mode, A = Automatic, M = Manual
**  2) Fix Mode, 1 = Fix not available, 2 = 2D, 3 = 3D
**  3) Satellite PRN #1
**  4) Satellite PRN #2
**  5) Satellite PRN #3
**  6) Satellite PRN #4
**  7) Satellite PRN #5
**  8) Satellite PRN #6
**  9) Satellite PRN #7
** 10) Satellite PRN #8
** 11) Satellite PRN #9
** 12) Satellite PRN #10
** 13) Satellite PRN #11
** 14) Satellite PRN #12
** 15) PDOP
** 16) HDOP
** 17) VDOP
** 18) Checksum
*/

typedef struct
{
    char    OperatingMode;                  //��λģʽ��A=�����ֶ�2D/3D��M=�ֶ�2D/3D
    int     FixMode;                        //��λ���ͣ�1=δ��λ��2=2D��λ��3=3D��λ
    int     SatelliteNumber[12];            //PRN�루α��������룩����1~12�ŵ�����ʹ�õ�����PRN���ţ�00����ǰ��λ��������0��
    double  PDOP;                           // PDOP�ۺ�λ�þ������ӣ�0.5 - 99.9��
    double  HDOP;                           //HDOPˮƽ�������ӣ�0.5 - 99.9��
    double  VDOP;                           //VDOP��ֱ�������ӣ�0.5 - 99.9��
}GPGSA;


/*
** GSV - TRANSIT Position - Latitude/Longitude
** Location and time of TRANSIT fix at waypoint
**
**        1 2 3  4  5  6   7  8  9  10  11 12 13 14  15 16 17 18  19  20
**        | | |  |  |  |   |  |  |  |   |  |  |  |   |  |  |  |   |   |
** $--GSV,x,x,xx,xx,xx,xxx,xx,xx,xx,xxx,xx,xx,xx,xxx,xx,xx,xx,xxx,xx,*hh<CR><LF>
**
**  1) Total number of messages, 1-3
**  2) Message Number, 1-3
**  3) Total number of satellites in view
**  4) Satellite Number #1
**  5) Elevation #1
**  6) Azimuth, Degrees True #1
**  7) SNR #1, NULL when not tracking
**  8) Satellite Number #2
**  9) Elevation #2
** 10) Azimuth, Degrees True #2
** 11) SNR #2, NULL when not tracking
** 12) Satellite Number #3
** 13) Elevation #3
** 14) Azimuth, Degrees True #3
** 15) SNR #3, NULL when not tracking
** 16) Satellite Number #4
** 17) Elevation #4
** 18) Azimuth, Degrees True #4
** 19) SNR #4, NULL when not tracking
** 20) Checksum
*/

typedef struct
{
    int     SatelliteNumber;                    //���Ǻš���1~32
    int     Elevation;                          //���ǡ���00��90
    int     AzimuthDegrees;                     //��λ�ǡ���000��359
    int     SNR;                                //����ȡ���00��99dB�ޱ�δ���յ�Ѷ��
}SATELLITE_DATA;


typedef struct
{
    int     TotalNumberOfMessages;              //�ܵ�GSV������������0~12
    int     MessageNumber;                      //��ǰGSV����
    int     NumberOfSatellites;                 //�ɼ����ǵ�������00~12��ǰ���0Ҳ�������䣩
    SATELLITE_DATA  SatellitesInView[4];       //����������Ϣ
}GPGSV;


/*
** GLL - Geographic Position - Latitude/Longitude
** Latitude, N/S, Longitude, E/W, UTC, Status
**
**        1       2 3        4 5         6 7
**        |       | |        | |         | |
** $--GLL,llll.ll,a,yyyyy.yy,a,hhmmss.ss,A*hh<CR><LF>
**
** Field Number:
**  1) Latitude
**  2) N or S (North or South)
**  3) Longitude
**  4) E or W (East or West)
**  5) Universal Time Coordinated (UTC)
**  6) Status A - Data Valid, V - Data Invalid
**  7) Checksum
*/
typedef struct
{
    double  LatitudePositionFieldNumber;    //γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
    char    Latitude;                       //γ�Ȱ���N�������򣩻�S���ϰ���
    double  LongitudePositionFieldNumber;   //����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
    char    Longitude;                      //���Ȱ���E����������W��������
    char    UTCTime[11];                    //UTCʱ�䣬hhmmss��ʱ���룩��ʽ
    char    IsDataValid;                    //��λ״̬��A=��Ч��λ��V=��Ч��λ
}GPGLL;


/*
** RMC - Recommended Minimum Navigation Information
**                                                            12
**        1         2 3       4 5        6 7   8   9    10  11|
**        |         | |       | |        | |   |   |    |   | |
** $--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a*hh<CR><LF>
**
** Field Number:
**  1) UTC Time
**  2) Status, V = Navigation receiver warning
**  3) Latitude
**  4) N or S
**  5) Longitude
**  6) E or W
**  7) Speed over ground, knots
**  8) Track made good, degrees true
**  9) Date, ddmmyy
** 10) Magnetic Variation, degrees
** 11) E or W
** 12) Checksum
*/
typedef struct
{
    char    UTCTime[11];                    //UTCʱ�䣺hhmmss.ss����000000.00~235959.99
    char    IsDataValid;                    //��λ״̬��A=��Ч��λ��V=��Ч��λ
    double  LatitudePositionFieldNumber;    //γ��ddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��
    char    Latitude;                       //γ��N����γ����S����γ��
    double  LongitudePositionFieldNumber;   //����dddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��
    char    Longitude;                      //����E����������W��������
    double  SpeedOverGroundKnots;           //�����ٶȡ���000.00~999.999
    double  TrackMadeGoodDegreesTrue;       //�ٶȷ��򡪡�000.00~359.99
    char    Date[7];                        //���ڸ�ʽ�������ꡪ��010100~123199
    double  MagneticVariation;              //��ƫ�ǣ���λ���ȡ���00.00~99.99
    char    MagneticVariationDirection;     //��ƫ�Ƿ��򡪡�E��ʾ����W��ʾ��
}GPRMC;


/*
** VTG - Track made good and Ground speed
**
**        1   2 3   4 5	 6 7   8 9
**        |   | |   | |	 | |   | |
** $--VTG,x.x,T,x.x,M,x.x,N,x.x,K*hh<CR><LF>
**
** Field Number:
**  1) Track Degrees
**  2) T = True
**  3) Track Degrees
**  4) M = Magnetic
**  5) Speed Knots
**  6) N = Knots
**  7) Speed Kilometers Per Hour
**  8) K = Kilometers Per Hour
**  9) Checksum
*/
typedef struct
{
    double TrackDegreesTrue;                //���汱Ϊ�ο���׼�ĵ��溽�򡪡�000.000~359.999
    double TrackDegreesMagnetic;            //�Դű�Ϊ�ο���׼�ĵ��溽�򡪡�000.000~359.999
    double SpeedKnots;                      //�������ʡ���000.000~999.999��
    double SpeedKilometersPerHour;          //�������ʡ���0000.0~1851.8����/Сʱ
}GPVTG;


/*
** ZDA - Time & Date
** UTC, day, month, year and local time zone
**
** $--ZDA,hhmmss.ss,xx,xx,xxxx,xx,xx*hh<CR><LF>
**        |         |  |  |    |  |
**        |         |  |  |    |  +- Local zone minutes description, same sign as local hours
**        |         |  |  |    +- Local zone description, 00 to +- 13 hours
**        |         |  |  +- Year
**        |         |  +- Month, 01 to 12
**        |         +- Day, 01 to 31
**        +- Universal Time Coordinated (UTC)
*/
typedef struct
{
    char    UTCTime[11];                    ////UTCʱ�䣺hhmmss.ss����000000.00~235959.99
    int     Day;                            //�ա���00��31
    int     Month;                          //�¡���00��12
    int     Year;                           //�ꡪ��0000��9999
    int     LocalHourDeviation;             //�ط�ʱ������ʱ֮���λСʱ����00~+-13
    int     LocalMinutesDeviation;          //�ط�ʱ������ʱ֮���λ���ӡ���
}GPZDA;


/**
 * @fn int getGPSData(char *cmd, void *gpsStruct)
 * @brief 	��������GPS NMEA-0183�����н���������������浽gpsStruct�ṹ����
 *
 * @param cmd           ��Ҫ����������������Ϊ$GPGGA��$GPGSA��$GPGSV��$GPRMC��$GPVTG��$GPGLL
 * @param gpsStruct  	��Ҫ��ŵĽ��������ݵĽṹ���ַ
 * @param sentence		NMEA-0183����ַ
 *
 * @return              �ɹ�����0,ʧ�ܷ���-1
 * @code
 *int main(void)
 *{
 *   GPGGA gpGGA;
 *   GPGSA gpGSA;
 *   GPGSV gpGSV[6];
 *   GPGLL gpGLL;
 *   GPRMC gpRMC;
 *   GPVTG gpVTG;
 *   int result;
 *   char tempBuf[] = {"13:37:34  $GPRMC,133734.00,A,4717.14058,N,00833.86305,E,0.003,208.16,190203,,,A*65\n13:37:34  $GPVTG,208.16,T,,M,0.003,N,0.005,K,A*36\n13:37:34  $GPGGA,133734.00,4717.14058,N,00833.86305,E,1,10,1.48,484.9,M,48.0,M,,0*61\n13:37:34  $GPGSA,A,3,03,17,15,18,16,23,02,31,27,26,,,2.37,1.48,1.85*0A\n13:37:34  $GPGSV,3,1,12,03,58,297,50,17,36,055,39,15,70,056,48,18,49,110,44*7E\n13:37:34  $GPGSV,3,2,12,16,59,205,48,23,40,065,46,02,59,251,48,31,25,292,49*7A\n13:37:34  $GPGSV,3,3,12,27,09,323,43,26,06,045,39,29,05,029,,08,02,341,*73\n13:37:34  $GPGLL,4717.14058,N,00833.86305,E,133734.00,A,A*65"};
 *
 *   memset(&gpGGA,0x00,sizeof(GPGGA));
 *   memset(&gpGSA,0x00,sizeof(GPGSA));
 *   memset(gpGSV,0x00,sizeof(GPGSV)*6);
 *   memset(&gpGLL,0x00,sizeof(GPGLL));
 *   memset(&gpRMC,0x00,sizeof(GPRMC));
 *   memset(&gpVTG,0x00,sizeof(GPVTG));
 *
 *   result = getGPGGAData(&gpGGA,tempBuf);
 *   if(result == 0)
 *   {
 *       printf("##############GPGGA######################\n");
 *       printf("UTCTime:%s\n",gpGGA.UTCTime);
 *       printf("LatitudePositionFieldNumber:%f\n",gpGGA.LatitudePositionFieldNumber);
 *       printf("Latitude:%c\n",gpGGA.Latitude);
 *       printf("LongitudePositionFieldNumber:%f\n",gpGGA.LongitudePositionFieldNumber);
 *       printf("Longitude:%c\n",gpGGA.Longitude);
 *       printf("GPSQuality:%d\n",gpGGA.GPSQuality);
 *       printf("NumberOfSatellitesInUse:%d\n",gpGGA.NumberOfSatellitesInUse);
 *       printf("HorizontalDilutionOfPrecision:%f\n",gpGGA.HorizontalDilutionOfPrecision);
 *       printf("AntennaAltitudeMeters:%f\n",gpGGA.AntennaAltitudeMeters);
 *       printf("GeoidalSeparationMeters:%f\n",gpGGA.GeoidalSeparationMeters);
 *       printf("AgeOfDifferentialGPSDataSeconds:%f\n",gpGGA.AgeOfDifferentialGPSDataSeconds);
 *       printf("DifferentialReferenceStationID:%d\n",gpGGA.DifferentialReferenceStationID);
 *   }
 *
 *
 *   result = getGPGSAData(&gpGSA,tempBuf);
 *   if(result == 0)
 *   {
 *       printf("##############GPGSA######################\n");
 *       printf("OperatingMode:%c\n",gpGSA.OperatingMode);
 *       printf("FixMode:%d\n",gpGSA.FixMode);
 *       int i;
 *       for(i=0;i<12;i++)
 *           printf("SatelliteNumber[%d]:%d\n",i,gpGSA.SatelliteNumber[i]);
 *       printf("PDOP:%f\n",gpGSA.PDOP);
 *       printf("HDOP:%f\n",gpGSA.HDOP);
 *       printf("VDOP:%f\n",gpGSA.VDOP);
 *   }
 *
 *
 *   result = getGPGSVData(gpGSV,tempBuf);
 *   if(result == 0)
 *   {
 *       int i;
 *       for(i=0;i<6;i++)
 *       {
 *           if(gpGSV[i].TotalNumberOfMessages <= 0)
 *               break;
 *           printf("##############GPGSV %d/%d######################\n",gpGSV[i].MessageNumber,gpGSV[i].TotalNumberOfMessages);
 *           printf("TotalNumberOfMessages:%d\n",gpGSV[i].TotalNumberOfMessages);
 *           printf("MessageNumber:%d\n",gpGSV[i].MessageNumber);
 *           printf("NumberOfSatellites:%d\n",gpGSV[i].NumberOfSatellites);
 *           int j;
 *           for(j=0;j<4;j++)
 *           {
 *               printf("SatelliteNumber[%d]:%d\n",j,gpGSV[i].SatellitesInView[j].SatelliteNumber);
 *               printf("Elevation[%d]:%d\n",j,gpGSV[i].SatellitesInView[j].Elevation);
 *               printf("AzimuthDegrees[%d]:%d\n",j,gpGSV[i].SatellitesInView[j].AzimuthDegrees);
 *               printf("SNR[%d]:%d\n",j,gpGSV[i].SatellitesInView[j].SNR);
 *           }
 *
 *       }
 *
 *   }
 *
 *
 *   result = getGPGLLData(&gpGLL,tempBuf);
 *   if(result == 0)
 *   {
 *       printf("##############GPGLL######################\n");
 *       printf("LatitudePositionFieldNumber:%f\n",gpGLL.LatitudePositionFieldNumber);
 *       printf("Latitude:%c\n",gpGLL.Latitude);
 *       printf("LongitudePositionFieldNumber:%f\n",gpGLL.LongitudePositionFieldNumber);
 *       printf("Longitude:%c\n",gpGLL.Longitude);
 *       printf("UTCTime:%s\n",gpGLL.UTCTime);
 *       printf("IsDataValid:%c\n",gpGLL.IsDataValid);
 *   }
 *
 *   result = getGPRMCData(&gpRMC,tempBuf);
 *   if(result == 0)
 *   {
 *       printf("##############GPRMC######################\n");
 *       printf("UTCTime:%s\n",gpRMC.UTCTime);
 *       printf("IsDataValid:%c\n",gpRMC.IsDataValid);
 *       printf("LatitudePositionFieldNumber:%f\n",gpRMC.LatitudePositionFieldNumber);
 *       printf("Latitude:%c\n",gpRMC.Latitude);
 *       printf("LongitudePositionFieldNumber:%f\n",gpRMC.LongitudePositionFieldNumber);
 *       printf("Longitude:%c\n",gpRMC.Longitude);
 *       printf("SpeedOverGroundKnots:%f\n",gpRMC.SpeedOverGroundKnots);
 *       printf("TrackMadeGoodDegreesTrue:%f\n",gpRMC.TrackMadeGoodDegreesTrue);
 *       printf("Date:%s\n",gpRMC.Date);
 *       printf("MagneticVariation:%f\n",gpRMC.MagneticVariation);
 *       printf("MagneticVariationDirection:%c\n",gpRMC.MagneticVariationDirection);
 *   }
 *
 *   result = getGPVTGData(&gpVTG,tempBuf);
 *   if(result == 0)
 *   {
 *       printf("##############GPVTG######################\n");
 *       printf("TrackDegreesTrue:%f\n",gpVTG.TrackDegreesTrue);
 *       printf("TrackDegreesMagnetic:%f\n",gpVTG.TrackDegreesMagnetic);
 *       printf("SpeedKnots:%f\n",gpVTG.SpeedKnots);
 *       printf("SpeedKilometersPerHour:%f\n",gpVTG.SpeedKilometersPerHour);
 *
 *   }
 *   return 0;
 *}
 * ���:
 * ##############GPGGA######################
 * UTCTime:133734.00
 * LatitudePositionFieldNumber:4717.140580
 * Latitude:N
 * LongitudePositionFieldNumber:833.863050
 * Longitude:E
 * GPSQuality:1
 * NumberOfSatellitesInUse:10
 * HorizontalDilutionOfPrecision:1.480000
 * AntennaAltitudeMeters:484.900000
 * GeoidalSeparationMeters:48.000000
 * AgeOfDifferentialGPSDataSeconds:0.000000
 * DifferentialReferenceStationID:0
 * ##############GPGSA######################
 * OperatingMode:A
 * FixMode:3
 * SatelliteNumber[0]:3
 * SatelliteNumber[1]:17
 * SatelliteNumber[2]:15
 * SatelliteNumber[3]:18
 * SatelliteNumber[4]:16
 * SatelliteNumber[5]:23
 * SatelliteNumber[6]:2
 * SatelliteNumber[7]:31
 * SatelliteNumber[8]:27
 * SatelliteNumber[9]:26
 * SatelliteNumber[10]:0
 * SatelliteNumber[11]:0
 * PDOP:2.370000
 * HDOP:1.480000
 * VDOP:1.850000
 * ##############GPGSV 1/3######################
 * TotalNumberOfMessages:3
 * MessageNumber:1
 * NumberOfSatellites:12
 * SatelliteNumber[0]:3
 * Elevation[0]:58
 * AzimuthDegrees[0]:297
 * SNR[0]:50
 * SatelliteNumber[1]:17
 * Elevation[1]:36
 * AzimuthDegrees[1]:55
 * SNR[1]:39
 * SatelliteNumber[2]:15
 * Elevation[2]:70
 * AzimuthDegrees[2]:56
 * SNR[2]:48
 * SatelliteNumber[3]:18
 * Elevation[3]:49
 * AzimuthDegrees[3]:110
 * SNR[3]:44
 * ##############GPGSV 2/3######################
 * TotalNumberOfMessages:3
 * MessageNumber:2
 * NumberOfSatellites:12
 * SatelliteNumber[0]:16
 * Elevation[0]:59
 * AzimuthDegrees[0]:205
 * SNR[0]:48
 * SatelliteNumber[1]:23
 * Elevation[1]:40
 * AzimuthDegrees[1]:65
 * SNR[1]:46
 * SatelliteNumber[2]:2
 * Elevation[2]:59
 * AzimuthDegrees[2]:251
 * SNR[2]:48
 * SatelliteNumber[3]:31
 * Elevation[3]:25
 * AzimuthDegrees[3]:292
 * SNR[3]:49
 * ##############GPGSV 3/3######################
 * TotalNumberOfMessages:3
 * MessageNumber:3
 * NumberOfSatellites:12
 * SatelliteNumber[0]:27
 * Elevation[0]:9
 * AzimuthDegrees[0]:323
 * SNR[0]:43
 * SatelliteNumber[1]:26
 * Elevation[1]:6
 * AzimuthDegrees[1]:45
 * SNR[1]:39
 * SatelliteNumber[2]:29
 * Elevation[2]:5
 * AzimuthDegrees[2]:29
 * SNR[2]:0
 * SatelliteNumber[3]:8
 * Elevation[3]:2
 * AzimuthDegrees[3]:341
 * SNR[3]:0
 * ##############GPGLL######################
 * LatitudePositionFieldNumber:4717.140580
 * Latitude:N
 * LongitudePositionFieldNumber:833.863050
 * Longitude:E
 * UTCTime:133734.00
 * IsDataValid:A
 * ##############GPRMC######################
 * UTCTime:133734.00
 * IsDataValid:A
 * LatitudePositionFieldNumber:4717.140580
 * Latitude:N
 * LongitudePositionFieldNumber:833.863050
 * Longitude:E
 * SpeedOverGroundKnots:0.003000
 * TrackMadeGoodDegreesTrue:208.160000
 * Date:190203
 * MagneticVariation:0.000000
 * MagneticVariationDirection:
 * ##############GPVTG######################
 * TrackDegreesTrue:208.160000
 * TrackDegreesMagnetic:0.000000
 * SpeedKnots:0.003000
 * SpeedKilometersPerHour:0.005000
 * @endcode
 */
int getGPSData(char *cmd, void *gpsStruct);

#ifdef __cplusplus
}
#endif

#endif // GPS_H
/** @} end of etc_fns */
