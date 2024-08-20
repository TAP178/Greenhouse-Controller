/** @brief Gh control functions
*   @file ghcontrol.c
*   @version CENG153, Serial:6cb15a84
*   @author Tuan Anh Pham
*   @since 2024-02-07
*   @param sname string TuanAnhPham
*   @file ghc.c
*   @return void
 */

#include "ghcontrol.h"
//ALARM MESSAGE ARRAY
const char alarmnames[NALARMS][ALARMNMSZ]=
    {"No Alarms","High Temperature","Low Temperature","High Humidity","Low Humidiy"
    ,"High Pressure","Low Pressure"};
#include <time.h>
#include <string.h>

void GhDisplayHeader(const char * sname)
{
	fprintf(stdout,"%s's CENG153 Greenhouse Controller\n",sname);
}

int GhGetRandom(int range)
{
	return rand() % range;
}
void GhDisplayControls(control_s ctrl)
{
	fprintf(stdout," Controls\tHeater: %i\tHumidifier: %i\n",ctrl.heater,ctrl.humidifier);
}
void GhDisplayReadings(reading_s rdata)
{
	fprintf(stdout,"\nUnit:%LX %s Readings\tT: %5.1fC\tH: %5.1f%\tP: %6.1fmb\n",ShGetSerial(),ctime(&rdata.rtime),rdata.temperature,rdata.humidity,rdata.pressure);
}
void GhDisplayTargets(setpoint_s spts)
{
	fprintf(stdout,"Setpoints\tT: %5.lfC\tH: %5.lf%\n",STEMP,SHUMID);
}
control_s GhSetControls(setpoint_s target,reading_s rdata)
{
	control_s cset  ;
	if(rdata.temperature<target.temperature){ cset.heater = ON;}
	else{
		cset.heater = OFF;
	}
	if(rdata.humidity< target.humidity){
		cset.humidifier = ON;
}
	else{
		cset.humidifier = OFF;
	}
	return cset;
}
setpoint_s GhSetTargets(void)
{

	setpoint_s cpoints  ;
	setpoint_s object;
    object = GhRetrieveSetpoints("setpoint.dat");
    if(cpoints.temperature == 0)
	{
	cpoints.temperature = STEMP;
	cpoints.humidity = SHUMID;
	GhSaveSetpoints("setpoints.dat",cpoints);
	}
	return cpoints;
}
float GhGetHumidity(void)
{
	#if SIMHUMIDITY
		return GhGetRandom(USHUMID-LSHUMID)+LSHUMID;
	#else
		return ShGetHumidity();
	#endif
}
float GhGetPressure(void)
{
	#if SIMPRESSURE
		return GhGetRandom(USPRESS-LSPRESS)+LSPRESS;
	#else
		return ShGetPressure();
	#endif
}
float GhGetTemperature(void)
{
	#if SIMTEMPERATURE
		return GhGetRandom(USTEMP-LSTEMP)+LSTEMP;
	#else
		return ShGetTemperature();
#endif
}

reading_s GhGetReadings(void)
{
	reading_s now = {0};

	now.rtime = time(NULL);
	now.temperature = GhGetTemperature();
	now.humidity = GhGetHumidity();
	now.pressure = GhGetPressure();
	return now;
}
void GhDelay(int milliseconds)
{
	long wait;
	clock_t now,start;

	wait = milliseconds*(CLOCKS_PER_SEC/1000);
	start = clock();
	now = start;
	while( (now-start) < wait)
	{
		now = clock();
	}
}
int GhLogData(char * fname,reading_s ghdata)
{
FILE *fp;
char ltime[CTIMESTRSZ];
fp = fopen(fname,"a");
if(fp == NULL)
{
    fprintf(stderr,"\nCan't open file, data not retrieved!\n");
    return 0;
}
strcpy(ltime,ctime(&ghdata.rtime));
ltime[3]=ltime[7]=ltime[10]=ltime[19]=',';
fprintf(fp,"\n%.24s,%5.1lf,%5.1lf,%6.1lf",ltime,ghdata.temperature,ghdata.humidity,ghdata.pressure);
fclose(fp);
return 1;
}
int GhSaveSetpoints(char * fname, setpoint_s spts)
{
FILE *fp;
fp = fopen(fname,"wd");
if(fp==NULL)
{
    return 0;
    }
fwrite(&spts,sizeof(struct setpoints),1,fp);
fclose(fp);
return 1;
}
setpoint_s GhRetrieveSetpoints(char * fname)
{
setpoint_s spts = {0.0};
FILE *fp;
fp = fopen(fname,"r");
if(fp==NULL)
{
    return spts;
}
fread(&spts, sizeof(setpoint_s),1,fp);
fclose(fp);
return spts;
}
int GhSetVerticalBar(int bar,COLOR_SENSEHAT pxc,uint8_t value,struct fb_t *fb)
{
int i;
if(value > 7)
    {
        value = 7;
}
if((bar < 0 || bar >= 8) || (value < 0 || value >= 8))
    {
        return EXIT_FAILURE;
}
for(i=0;i<= value;i++)
{
    ShLightPixel(i,bar,pxc,fb);
}
for(i=value+1;i < 8; i++)
{
    ShLightPixel(i,BLACK,pxc,fb);
}
return EXIT_SUCCESS;
}
void GhDisplayAll(reading_s rd,setpoint_s sd,struct fb_t *fb)
{
int rv,sv,avh,avl;
ShWipeScreen(BLACK,fb);
COLOR_SENSEHAT pxc;

rv = (8.0 * (((rd.temperature - LSTEMP)/(USTEMP - LSTEMP)) + .05)) - 1.0;
avh = (8.0 * (((rd.humidity - LSHUMID)/(USHUMID - LSHUMID)) + .05)) - 1.0;
avl = (8.0 * (((rd.pressure - LSPRESS)/(USPRESS - LSPRESS)) + .05)) - 1.0;


GhSetVerticalBar(TBAR,GREEN,rv,fb);
GhSetVerticalBar(HBAR,GREEN,avh,fb);
GhSetVerticalBar(PBAR,GREEN,avl,fb);

sv = (8.0 * (((rd.temperature - LSTEMP)/(USTEMP - LSTEMP)) + .05)) - 1.0;
GhSetVerticalBar(TBAR,MAGENTA,sv,fb);
GhSetVerticalBar(HBAR,MAGENTA,sv,fb);


}
alarmlimit_s GhSetAlarmLimits(void)
{
    alarmlimit_s calarm;
    calarm.hight = UPPERATEMP;
    calarm.lowt = LOWERATEMP;
    calarm.highh = UPPERAHUMID;
    calarm.lowh = LOWERAHUMID;
    calarm.highp = UPPERAPRESS;
    calarm.lowp = LOWERAPRESS;
    return calarm;
}
alarm_s * GhSetAlarm(alarm_s * head,alarmlimit_s alarmpt, reading_s rdata)
{

if(rdata.temperature >= alarmpt.hight)
{
    GhSetOneAlarm(HTEMP,rdata.rtime,rdata.temperature,head);
}
else
{
    head = GhClearOneAlarm(HTEMP,head);
}
if(rdata.temperature <= alarmpt.lowt)
{
    GhSetOneAlarm(LTEMP,rdata.rtime,rdata.temperature,head);
}
else
{
    head = GhClearOneAlarm(LTEMP,head);
}
if(rdata.humidity >= alarmpt.highh)
{
   GhSetOneAlarm(HHUMID,rdata.rtime,rdata.humidity,head);
}
else
{
    head = GhClearOneAlarm(HHUMID,head);
}
if(rdata.humidity <= alarmpt.lowh)
{
    GhSetOneAlarm(LHUMID,rdata.rtime,rdata.humidity,head);
}
else
{
    head = GhClearOneAlarm(LHUMID,head);
}
if(rdata.pressure >= alarmpt.highp)
{
    GhSetOneAlarm(HPRESS,rdata.rtime,rdata.pressure,head);
}
else
{
    head = GhClearOneAlarm(HPRESS,head);
}
if(rdata.pressure <= alarmpt.lowp)
{
   GhSetOneAlarm(LPRESS,rdata.rtime,rdata.pressure,head);
}
else
{
    head = GhClearOneAlarm(LPRESS,head);
}
return head;
}
void GhDisplayAlarms(alarm_s * head)
{
alarm_s *cur;
cur = head;
printf("Alarms\n");
while(cur != NULL)
{
if(cur->code != NOALARM)
{
    printf("%s %s",alarmnames[cur->code], ctime(&cur->atime));
}
cur = cur -> next;
}
}
int GhSetOneAlarm(alarm_e code,time_t atime,double value,alarm_s * head)
{
alarm_s *last,*cur;
cur = head;
if(cur == NULL )
{
head = calloc(1,sizeof(alarm_s));
if(head == NULL){
return 0;
}
}
else
{
    while(cur != NULL)
    {
    if(cur->code == code)
    {
    return 0;
    }
    last = cur;
    cur  = cur -> next;
    }
    cur = calloc(1,sizeof(alarm_s));
    if(cur == NULL){
    return 0;
    }
    last->next = cur;
    cur->code = code;
    cur->atime = atime;
    cur->value = value;
    cur->next = NULL;
    return 1;
}
}
alarm_s * GhClearOneAlarm(alarm_e code,alarm_s * head)
{
alarm_s *cur ,*last;
cur = head;
last = head;
if(cur->code == code && cur->next == NULL)
{
cur->code = NOALARM;
return head;
}
else if(cur->code == code && cur->next != NULL)
{
head = cur->next;
free(cur);
return head;

}
else
{
while(cur != NULL)
{
if(cur->code == code){
cur->next = last->next;
free(cur);
return head;
}
last = cur;
cur = cur->next;

}
}
return head;
}
