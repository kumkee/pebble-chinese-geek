#include "pebble_os.h"
#ifndef LYLUNAR_H
#define LYLUNAR_H

typedef char byte;

typedef struct {
	int year, month, day, hour, weekday;
	int leap;	/* the lunar month is a leap month */
} Date;

void Solar2Lunar(Date*,Date*,Date*);

//void GenerateCDateText(PblTm*, char*,bool);
void GenerateCDateText(PblTm*, char*,char*,char*,bool);
void GenerateKeText(PblTm *, char *);

#endif
