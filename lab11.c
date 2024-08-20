/**
*   @version CENG153, Serial:6cb15a84
*   @author Tuan Anh Pham
*   @since 2024-02-07
*   @param sname string TuanAnhPham
*   @file lab11.c
*   @return void
 */
struct student
{
	char name[50];
	int student_id;
	float mark;
};


#include "ghcontrol.h"
float generateDistance(void);
int main(void)
{
	int logged;
	time_t now;
	srand; time(NULL);
	struct student myInfo = {"Tuan Anh Pham",1238459,99.99};
	printf("%s has an ID number of N0%d, and get %.2f percent for this lab test\n",myInfo.name,myInfo.student_id,myInfo.mark);
	float randomNum,sum;
	randomNum = generateDistance();
	sum = randomNum + 9;
	printf("\nThe sum of my last digit student ID and random distance is %f\n", sum);
    struct fb_t *fb;
    fb=ShInit(fb);
    GhDisplayHeader("TuanAnhPham");
	struct readings creadings = {0};
	struct controls ctrl = {0};
	struct setpoints sets = {0};
	sets = GhSetTargets();
	alarmlimit_s alimits = {0};
	alarm_s * arecord;
	arecord = (alarm_s *) calloc(1,sizeof(alarm_s));
	if(arecord == NULL)
	{
        fprintf(stderr,"\nCannot allocate memory\n");
        return EXIT_FAILURE;
	}
	alimits = GhSetAlarmLimits();
	while(1)
	{
		now = time(NULL);
		creadings = GhGetReadings();
		logged = GhLogData("ghdata.txt",creadings);
		ctrl = GhSetControls(sets,creadings) ;
        arecord = GhSetAlarm(arecord, alimits, creadings);
		GhDisplayAll(creadings, sets, fb);
		GhDisplayReadings(creadings);
		GhDisplayTargets(sets);
		GhDisplayControls(ctrl);
		GhDisplayAlarms(arecord);
		GhDelay(GHUPDATE);
	}
	fprintf(stdout,"Press Enter to continue...");
	fgetc(stdin);
	return EXIT_FAILURE;
}

float generateDistance(void)
{
	return rand();
}









































