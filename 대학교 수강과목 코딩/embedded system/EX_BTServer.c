#include <stdio.h>
#include <unistd.h>   
#include <stdlib.h>   
#include <string.h> 
#include <wiringPi.h> 
#include <wiringSerial.h>
#include <wiringPiI2C.h>
#include <wiringPiSPI.h>
#include <softTone.h>
#include <errno.h>
#include <softPwm.h>
#include <pcf8574.h>
#include <lcd.h>
#include "TM1637.h"

//- 핀 번호 & 주소 선언 -------------------------------------------------------
// Temperature sensor
#define TEMP_ADDR            72
#define TEMP_DATA_REG        0
#define TEMP_CONF_REG        1

// SPI ADC
#define CHAN_CONFIG_SINGLE  8
#define CHAN_CONFIG_DIFF     0
#define BUZ_PIN		  21

//pot sensor
#define SPI_SPEED 1000000 //- 1MHz
#define SPI_CE_CHANNEL 1 //- SPI와 연결된 CE 채널
#define SPI_ADC_POTEN_CHANNEL 1 //- Potentionmeter 연결 채널
#define CS_MCP3208 11 //- MCP3208 연결 핀

//pir 
#define PIR_PIN 2

//dcmotor
#define DCMotorA 6
#define DCMotorB 31

//- PCF8574 &#8212; LCD 연결핀 주소값 선언 -----------------------------------------
#define BASE_ADDR 100
#define RS_PIN BASE_ADDR //- 100
#define RW_PIN BASE_ADDR+1 //- 101
#define EN_PIN BASE_ADDR+2 //- 102
#define BL_PIN BASE_ADDR+3 //- 103
#define D4_PIN BASE_ADDR+4 //- 104
#define D5_PIN BASE_ADDR+5 //- 105
#define D6_PIN BASE_ADDR+6 //- 106
#define D7_PIN BASE_ADDR+7 //- 107

//FND
#define CLK_PIN 9
#define DIO_PIN 8


//- 함수 선언 ---------------------------------------------------------------------
void hw_init();
void parserCmd();
void InitBT(); 
void CaptureImage();
void sendSensorData();

void sendImageData();
void GetTempValue();
void GetHumValue();
void LedOn(int type, bool bOn);
void BuzzerOn(bool bOn);
int  AnalogRead(int spiChannel, int channelConfig, int analogChannel);

//pot 함수.
int spiSetup(void);
int readAnalogData(int adcChannel);
void GetAdcValue();

//pir 함수
void GetPir();

//dcmotor함수
void speedUp();
void speedDown();
void motorStop();

//FND함수
void divide(int n);
void FNDdisplay();

//- 전역 변수 및 상수 -------------------------------------------------------------
enum { LED_RED, LED_GREEN, LED_YELLOW, LED_WHITE };
static unsigned char LED[] = {27, 28, 29, 25};

int  tempValue;
int  humValue;
int  adcValue;

bool bBuzzerRun = false;
bool bBuzzerOn = false;

char rx_buf[BUFSIZ] = "";

int  fd_i2c;
int  fd_spi;
int  g_hBT;

//pir변수
int pirSensor =0;

//dcmotor변수
int speedValue =1;

//lcd변수
int fd;
using namespace std;

//buzzer
unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};

//FND변수
int list[4] ={0, 0, 0, 0};
int FNDNum =0;
int cnt =0;
bool FNDincrease =false;

//- 엔트리 포인터 -----------------------------------------------------------------
int main()
{
    unsigned int nNextTime, nBuzzTime, nSensorTime;
	
    InitBT();                                  //- 블루투스 초기화
    hw_init();
 nNextTime = millis();                     //- 일정시간 간격 저장 변수
    nBuzzTime = millis();
    nSensorTime = millis();    
    while(1)
    {
        //- 0.25초 간격으로 BT로 수신된 데이터 여부 체크 후 읽어오기
        if(millis() > nNextTime + 250)
	{
		int i=0;
		memset(rx_buf, 0, sizeof(rx_buf));
		while(serialDataAvail(g_hBT) > 0)
		{
				rx_buf[i] = serialGetchar(g_hBT);
				if(rx_buf[i] == '#')
				{
					if(rx_buf[0] == '@') parserCmd();
					break;
				}
				i++;	
		}
              nNextTime = millis();
        }
	//- 1초 간격으로 부저 재생	
	if(bBuzzerRun)
	{
		if(millis() > nBuzzTime)
		{
			bBuzzerOn = !bBuzzerOn;
			BuzzerOn(bBuzzerOn);
			nBuzzTime = millis() + 1000;
		 }
	 }
 //- 1초 간격으로 습도, 온도 값 전송
               if(millis() > nSensorTime + 1000)
		{
			GetTempValue();
			GetHumValue();
      GetAdcValue();
      GetPir();
      if(cnt >FNDNum && FNDincrease)
      {
        FNDNum++;
      }
      FNDdisplay();
			sendSensorData();
			nSensorTime = millis();	
		}
    }
    printf ("\n") ;
    return 0 ;
}

//- 라이브러리 및 센서 초기화 --------------------------------------------------
void hw_init(void)
{
      wiringPiSetup();
	
	int i;
	for(i=0; i<sizeof(LED); i++)
	{
		pinMode(LED[i], OUTPUT);           //- LED 초기화
		digitalWrite(LED[i], LOW);
	}
 //adc
 spiSetup();
   pinMode(CS_MCP3208, OUTPUT);
   
   //pir
   pinMode(PIR_PIN, INPUT);
   digitalWrite(PIR_PIN, HIGH);
   
   //dcmotor
   pinMode(DCMotorA, SOFT_PWM_OUTPUT);
    pinMode(DCMotorB, SOFT_PWM_OUTPUT);
    softPwmCreate(DCMotorA, 0, 100);
    softPwmCreate(DCMotorB, 0, 100);
    
    
    //lcd
    //- I2C 통신기반 LCD 슬레이브 설정
    pcf8574Setup(RS_PIN, 0x27); //- P0핀 - LCD의 RS 연결, 주소 지정
    //- P1핀 - LCD의 RW 연결, 동작모드 설정  
    pinMode (RW_PIN, OUTPUT);
    digitalWrite (RW_PIN, LOW);
    //- P3핀 - LCD의 Backlight 연결
    pinMode (BL_PIN, OUTPUT);
    digitalWrite (BL_PIN, HIGH); //- Backlight ON
    
    fd = lcdInit (2, 16, 4, RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN,
D7_PIN, 0,0,0,0 );

  //buzzer
  pinMode(BUZ_PIN, SOFT_TONE_OUTPUT);
  softToneCreate(BUZ_PIN);
  
  //FND
  pinMode(CLK_PIN,INPUT);
  pinMode(DIO_PIN,INPUT);
  delay(200);
  TM1637_init(CLK_PIN,DIO_PIN);
  TM1637_set( BRIGHTEST, 0x40, 0xc0 );
    
 
	fd_i2c = wiringPiI2CSetup(TEMP_ADDR);     //- 온도 센서 초기화
	fd_spi = wiringPiSPISetup(1, 1000000);      //- SPI통신 초기화
	
	softToneCreate(BUZ_PIN);                  //- 부저 초기화
}
//- BT로부터 수신된 데이터 분석 후 처리 -------------------------------------
void parserCmd()
{
    int index = 1;
    if(!strncmp(&rx_buf[index], "SHT", 3))       //- 카메라 촬영 커맨드
    {
        index += 3;
        if(rx_buf[index] == '#')
        {
		printf("capture image\n");
              CaptureImage();

	       printf("Send image\n");
		sendImageData();
        }
    }else if(!strncmp(&rx_buf[1], "BUZ", 3)) {   //- 부저 제어
        index += 3;
        if(rx_buf[index] == ',')
        {
            bool bOn;
            index++;                
            if(rx_buf[index] == '0')
            {
		 printf("Buzzer off\n");
                bOn = false;
            }else if(rx_buf[index] == '1'){
		 printf("Buzzer on\n");
                bOn = true;
            }
index++;
            if(rx_buf[index]== '#')
            {
			if(bOn)
				bBuzzerOn = false;
			else
				BuzzerOn(bOn);
			bBuzzerRun = bOn;
            }
        }
    }else if(!strncmp(&rx_buf[1], "RED", 3)) {    //- LED 제어
        index += 3;
        if(rx_buf[index] == ',')
        {
            bool bOn = FALSE;
            index++;                
            if(rx_buf[index] == '0')
            {
		 printf("Red LED off\n");
                bOn = FALSE;
            }else if(rx_buf[index] == '1'){
		 printf("Red LED on\n");
                bOn = TRUE;
            }

            index++;
            if(rx_buf[index]== '#')
                LedOn(LED_RED, bOn);
       }
}else if(!strncmp(&rx_buf[1], "GED", 3)) {
        index += 3;
        if(rx_buf[index] == ',')
        {
            bool bOn = FALSE;
            index++;                
            if(rx_buf[index] == '0')
            {
		 printf("Green LED off\n");
                bOn = FALSE;
            }else if(rx_buf[index] == '1'){
		 printf("Green LED on\n");
                bOn = TRUE;
            }

            index++;
            if(rx_buf[index]== '#')
                LedOn(LED_GREEN, bOn);
        }
    }else if(!strncmp(&rx_buf[1], "YED", 3)){
        index += 3;
        if(rx_buf[index] == ',')
        {
            bool bOn = FALSE;
            index++;                
            if(rx_buf[index] == '0')
            {
		 printf("Yellow LED off\n");
                bOn = FALSE;
            }
else if(rx_buf[index] == '1'){
		 printf("Yellow LED on\n");
                bOn = TRUE;
            }
            index++;
            if(rx_buf[index]== '#')
                LedOn(LED_YELLOW, bOn);
        }
    }else if(!strncmp(&rx_buf[1], "LED", 3)){
        index += 3;
        if(rx_buf[index] == ',')
        {
            bool bOn = FALSE;
            index++;                
            if(rx_buf[index] == '0')
            {
		 printf("White LED off\n");
                bOn = FALSE;
            }else if(rx_buf[index] == '1'){
		 printf("White LED on\n");
                bOn = TRUE;
            }

            index++;
            if(rx_buf[index]== '#')
                LedOn(LED_WHITE, bOn);
        }
    }
    else if(!strncmp(&rx_buf[1], "DCM", 3)){
        index += 3;
        if(rx_buf[index] == ',')
        {
            bool bOn = FALSE;
            index++;                
            if(rx_buf[index] == '0')
            {
		             printf("speedUp\n");
                 speedUp();
            }else if(rx_buf[index] == '1'){
		             printf("speedDown\n");
                speedDown();
            }
            else if(rx_buf[index] == '2'){
		             printf("stop\n");
                motorStop();
            }

            index++;
          //  if(rx_buf[index]== '#')
                
        }
    }
    
    else if(!strncmp(&rx_buf[1], "LCD", 3)){
        index += 3;
        if(rx_buf[index] == ',')
        {
            index++;                
            if(rx_buf[index] == '0')          //on
            {
              digitalWrite (BL_PIN, HIGH); //- Backlight ON
              lcdPosition (fd, 0, 0); //- 0번 칸, 0번 줄 커서 위치
            }else if(rx_buf[index] == '1'){    //off
                digitalWrite (BL_PIN, LOW); //- Backlight ON
            }
            else if(rx_buf[index] == '2'){      //clear
		           lcdClear(fd);
            }
            else if(rx_buf[index] == '3'){      //send
              char lcdStr[17];
              lcdClear(fd);
              int idx =0;
              index++;
              while(rx_buf[index] !='\0')
              {
                lcdStr[idx++] =rx_buf[index++];
              }
              lcdStr[idx-1] ='\0';
              
              printf("%s\n", lcdStr);
            
		            lcdPuts (fd, lcdStr); //- 문자열 출력
              delay(2000);
            }

            index++;
          //  if(rx_buf[index]== '#')
                
        }
    }
    
    else if(!strncmp(&rx_buf[1], "PIE", 3)){
        index += 3;
        if(rx_buf[index] == ',')
        {
            index++;                
            if(rx_buf[index] == '0')
            {
             softToneWrite(BUZ_PIN, melody[0]);
            }else if(rx_buf[index] == '1'){
                softToneWrite(BUZ_PIN, melody[1]);
            }
            else if(rx_buf[index] == '2'){
		          softToneWrite(BUZ_PIN, melody[2]);
            }
            else if(rx_buf[index] == '3'){
              softToneWrite(BUZ_PIN, melody[3]);
            }
            else if(rx_buf[index] == '4'){
              softToneWrite(BUZ_PIN, melody[4]);
            }
            else if(rx_buf[index] == '5'){
              softToneWrite(BUZ_PIN, melody[5]);
            }
            else if(rx_buf[index] == '6'){
              softToneWrite(BUZ_PIN, melody[6]);
            }
            else if(rx_buf[index] == '7'){
              softToneWrite(BUZ_PIN, melody[7]);
            }
            else if(rx_buf[index] == '8'){
              softToneWrite(BUZ_PIN, melody[5]);	delay(600);
	softToneWrite(BUZ_PIN, melody[4]);	delay(200);
	softToneWrite(BUZ_PIN, melody[3]);	delay(300);
	softToneWrite(BUZ_PIN, melody[4]);	delay(400);

	softToneWrite(BUZ_PIN, melody[5]);	delay(300);
	softToneWrite(BUZ_PIN, melody[5]);	delay(300);
	softToneWrite(BUZ_PIN, melody[5]);	delay(400);

	softToneWrite(BUZ_PIN, melody[4]);	delay(300);
	softToneWrite(BUZ_PIN, melody[4]);	delay(300);
	softToneWrite(BUZ_PIN, melody[4]);	delay(400);

	softToneWrite(BUZ_PIN, melody[5]);	delay(300);
	softToneWrite(BUZ_PIN, melody[5]);	delay(300);
	softToneWrite(BUZ_PIN, melody[5]);	delay(400);

	softToneWrite(BUZ_PIN, melody[5]);	delay(600);
	softToneWrite(BUZ_PIN, melody[4]);	delay(200);
	softToneWrite(BUZ_PIN, melody[3]);	delay(300);
	softToneWrite(BUZ_PIN, melody[4]);	delay(400);

	softToneWrite(BUZ_PIN, melody[5]);	delay(300);
	softToneWrite(BUZ_PIN, melody[5]);	delay(300);
	softToneWrite(BUZ_PIN, melody[5]);	delay(400);

	softToneWrite(BUZ_PIN, melody[4]);	delay(300);
	softToneWrite(BUZ_PIN, melody[4]);	delay(400);

	softToneWrite(BUZ_PIN, melody[5]);	delay(600);
	softToneWrite(BUZ_PIN, melody[4]);	delay(300);
	softToneWrite(BUZ_PIN, melody[3]);	delay(400);

	softToneStop(BUZ_PIN);
 delay(500);
            }
            else if(rx_buf[index] == '9'){
              	softToneWrite(BUZ_PIN, melody[4]);	delay(600);
	softToneWrite(BUZ_PIN, melody[2]);	delay(300);
	softToneWrite(BUZ_PIN, melody[2]);	delay(500);

	softToneWrite(BUZ_PIN, melody[4]);	delay(500);
	softToneWrite(BUZ_PIN, melody[2]);	delay(300);
	softToneWrite(BUZ_PIN, melody[0]);	delay(500);

	softToneWrite(BUZ_PIN, melody[1]);	delay(600);
	softToneWrite(BUZ_PIN, melody[2]);	delay(400);
	softToneWrite(BUZ_PIN, melody[1]);	delay(500);

	softToneWrite(BUZ_PIN, melody[0]);	delay(400);
	softToneWrite(BUZ_PIN, melody[2]);	delay(300);
	softToneWrite(BUZ_PIN, melody[4]);	delay(500);

	softToneWrite(BUZ_PIN, melody[7]);	delay(500);
	softToneWrite(BUZ_PIN, melody[4]);	delay(300);
	softToneWrite(BUZ_PIN, melody[7]);	delay(500);
	softToneWrite(BUZ_PIN, melody[4]);	delay(400);

	softToneWrite(BUZ_PIN, melody[7]);	delay(600);
	softToneWrite(BUZ_PIN, melody[4]);	delay(300);
	softToneWrite(BUZ_PIN, melody[2]);	delay(500);

	softToneWrite(BUZ_PIN, melody[4]);	delay(600);
	softToneWrite(BUZ_PIN, melody[1]);	delay(300);
	softToneWrite(BUZ_PIN, melody[3]);	delay(400);

	softToneWrite(BUZ_PIN, melody[2]);	delay(400);
	softToneWrite(BUZ_PIN, melody[1]);	delay(300);
	softToneWrite(BUZ_PIN, melody[0]);	delay(500);

	softToneStop(BUZ_PIN);
	delay(500);
            }

            index++;
          //  if(rx_buf[index]== '#')
                
        }
    }
    
     else if(!strncmp(&rx_buf[1], "FND", 3)){
        index += 3;
        if(rx_buf[index] == ',')
        {

            index++;                
            if(rx_buf[index] == '0')
            {
		            char str[10];
                int idx =0;
                index++;
                
                while(rx_buf[index] !='#')
                {
                  str[idx++] =rx_buf[index++];
                }
                str[idx] ='\0';
                
                cnt =atoi(str);
                FNDincrease =true;
            }else if(rx_buf[index] == '1'){
		            FNDincrease =false;
            }
            

            index++;
          //  if(rx_buf[index]== '#')
                
        }
    }
    
    
    else{
	printf("%s\n", rx_buf);
        return;
    }
}
//- 블루투스 통신 초기화 -------------------------------------------------------
void InitBT() 
{
	g_hBT = serialOpen("/dev/rfcomm0", 115200);
	printf("Bluetooth init\n");
}

//- 온도, 습도 전송 --------------------------------------------------------------
void sendSensorData()
{
	serialPrintf(g_hBT, "@TMP,%d#@HUM,%d#@POT,%d#@TXT,%d#@MOT,%d#@FND,%d#", tempValue, humValue, adcValue, pirSensor,speedValue, FNDNum);
  
}

//- LED On/Off 제어 -------------------------------------------------------------
void LedOn(int type, bool bOn)
{
	digitalWrite(LED[type], bOn);
}
//- Buzzer On/Off 제어 ----------------------------------------------------------
void BuzzerOn(bool bOn)
{
	if(bOn)
		softToneWrite(BUZ_PIN, 500);
	else
		softToneWrite(BUZ_PIN, 0);
}
//- 아날로그 입력 데이터를 SPI로 전송 ----------------------------------------
void CaptureImage()
{
    system("raspistill -w 320 -h 240 -o /home/pi/test.jpg");
}
//- 아날로그 입력 데이터를 SPI로 전송 ----------------------------------------
int AnalogRead(int spiChannel, int channelConfig, int analogChannel)
{
	if(analogChannel<0 || analogChannel>7) return -1;
	
	unsigned char buffer[3] = {1};
	buffer[1] = (channelConfig+analogChannel) << 4;
	wiringPiSPIDataRW(spiChannel, buffer, 3);
	return ((buffer[1] & 3) << 8) + buffer[2];
}
//- 온도 측정하기 ----------------------------------------------------------------
void GetTempValue()
{
    wiringPiI2CWrite(fd_i2c, TEMP_DATA_REG);
    int rawData = wiringPiI2CReadReg16(fd_i2c, TEMP_DATA_REG);
    rawData = ((rawData & 0xF000)>>12) + ((rawData & 0xFF)<<4);
    float tempVal = rawData * 0.0625;
    tempValue = (int)tempVal;
}//- 습도 측정하기 ----------------------------------------------------------------
void GetHumValue()
{
     int spiChannel = 1;
     int channelConfig = CHAN_CONFIG_SINGLE;
     int analogChannel = 0;
	
     int rawHum=AnalogRead(spiChannel, channelConfig, analogChannel);
     float HumVolt = rawHum * 5.0 / 1023;
     float hum = (HumVolt - 0.958) / 0.0307;
     humValue = (int)hum;
}
//- 촬영된 이미지 데이터 전송 -------------------------------------------------
void sendImageData()
{
	char tx_buf[16];
	FILE *fp_read;
	long file_size;
	char *memblock;
	
	fp_read = fopen("/home/pi/test.jpg", "rb");
	if(fp_read != NULL)
	{
		fseek(fp_read, 0, SEEK_END);
		file_size = ftell(fp_read);
		memblock = (char*)malloc(file_size);
		fseek(fp_read, 0, SEEK_SET);
		fread(memblock, 1, file_size, fp_read);
		fclose(fp_read);
		
		serialPrintf(g_hBT, "@IMG,");		
		write(g_hBT, memblock, file_size);
		serialPrintf(g_hBT, "####");
		
		free(memblock);
	}else{
		printf("File read error!\n");
		return;
	}
}

//- SPI통신 라이브러리 초기화------------------------------------------------------------
int spiSetup(void)
{
if( wiringPiSPISetup (SPI_CE_CHANNEL, SPI_SPEED) == -1 )
{
fprintf (stderr, "wiringPiSPISetup Failed ! ERROR : %s\n", strerror (errno));
return 1;
}
}

//- 채널 1번에서 측정된 ADC센서 읽어오기 --------------------------------------------
int readAnalogData(int adcChannel)
{
int adcValue = 0;
unsigned char buffer[3] = {0};
//- MCP3008의 8개 채널 중 하나인지 체크
if(adcChannel<0 || adcChannel>7) return -1;
//- SPI 통신 버퍼 설정
buffer[0] = 0x01;
buffer[1] = (CHAN_CONFIG_SINGLE+adcChannel) << 4;
//- SPI 통신할 칩 활성화
digitalWrite(CS_MCP3208, LOW);
//- 채널 1에서 측정된 ADC 센서 읽어 오기
wiringPiSPIDataRW(SPI_CE_CHANNEL, buffer, 3);
//- 읽어온 값에서 10비트 데이터 추출하기
adcValue = ( (buffer[1] & 3 ) << 8 ) + buffer[2];
//- SPI 통신할 칩 비활성화
digitalWrite(CS_MCP3208, HIGH);
return adcValue;
}

void GetAdcValue()
{ 
  adcValue = readAnalogData(SPI_ADC_POTEN_CHANNEL);
}

//pir
void GetPir()
{
  if(digitalRead(PIR_PIN) == HIGH)
    pirSensor =1;
  else
    pirSensor =0;
}
//dcmotor
void speedUp()
{
  speedValue *=2;
  softPwmWrite(DCMotorA, speedValue);
}

void speedDown()
{
  speedValue /=2;
  if(speedValue ==0)
    speedValue =1;
  softPwmWrite(DCMotorA, speedValue);
}
void motorStop()
{
  speedValue =1;
  softPwmWrite(DCMotorA, 0);
}

void divide(int n)
{
  if(n >9999)
  {
    for(int i =0; i <4; i++)
      list[i] =8;
    delay(200);
    
    return;
  }else
  {
    int i =3;
    while(n !=0)
    {
      list[i] =n%10;
      n /=10;
      i--;
    }
  }
}

void FNDdisplay()
{
  for(int i =0; i <4; i++)
    {
      TM1637_display(i, list[i]);  
    }
}
