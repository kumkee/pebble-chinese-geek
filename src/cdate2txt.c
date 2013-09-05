//#include "string.h"
#include "chical.h"


void CDateDisplayNo(Date *d, char* text)
{
  int i;
  i = d->leap?1:0;
	
  if(i)	text[0] = 'L';
  text[i+0] = d->month/10 + '0';
  text[i+1] = d->month%10 + '0';
  text[i+2] = 'M';
  text[i+3] = '/';
  text[i+4] = d->day/10 + '0';
  text[i+5] = d->day%10 + '0';
  text[i+6] = 'D';
  text[i+7] = 0;
}


#define zhLen 3
#define MvChar(zhi) memcpy(text + place*zhLen, zhi, zhLen)

void CDateDisplayZh(Date *d, char* text)
{
  char ZhDigit[10][zhLen+1] = { "正", "一", "二", "三", "四", "五", "六", "七", "八", "九" };
  char ZhDigit2[3][zhLen+1] = { "初", "十", "廿" }; //digit in ten's place
  char ZhLeap[] = "閏";
  char ZhMonth[] = "月";
  char* ZhTen[3] = { ZhDigit2[0], ZhDigit[2], ZhDigit[3] }; //初, 二, 三

  int i,j;
  int place = 0;
  i = d->leap?1:0;
  j = (d->month-1)/10==0 ? 0 : 1;

  if(i)	MvChar(ZhLeap);	//閏

  place += i;
  if(j) MvChar(ZhDigit2[1]);		//十 of 十某月

  place += j;
  if(d->month==1)	MvChar(ZhDigit[0]);		//正 of 正月
  else if(d->month==10)	MvChar(ZhDigit2[1]);		//十 of 十月
  else 			MvChar(ZhDigit[d->month%10]);	//某 of 十某月 or 某月

  place++;

  MvChar(ZhMonth);	//月
  place++;

  if(d->day%10==0){
	MvChar(ZhTen[d->day/10 - 1]);	//某 of 某十日
	place++;
	MvChar(ZhDigit2[1]);		//十 of 某十日
	place++;
  }
  else{
	MvChar(ZhDigit2[d->day/10]);	//某 of 某甲日
	place++;
	MvChar(ZhDigit[d->day%10]);	//甲 of 某甲日
	place++;
  }

  text[place*zhLen] = 0;
  
}


void GanZhiDisplay(Date* gz, char* text, bool G_or_Z)
{
  static char ZhGan[] = {"甲乙丙丁戊己庚辛壬癸"};
  static char ZhZhi[] = {"子丑寅卯辰巳午未申酉戌亥"};
  int place = 0;
  static bool firstcall[2] = {true, true};
  #define GZcpy(col)  memcpy(text + place*zhLen, Zh + (gz->col)*zhLen, zhLen)

  char* Zh = G_or_Z?ZhGan:ZhZhi;
  
  GZcpy(hour);
  if(gz->hour == 23 || firstcall[G_or_Z])
  {
	firstcall[G_or_Z] = false;
	place++;
	GZcpy(day);
	place++;
	GZcpy(month);
	place++;
	GZcpy(year);
	place++;

	text[place*zhLen] = 0;
  }
}


void GenerateCDateText(PblTm *t, char* cdtext, char* gantxt, char* zhitxt, bool ZhDisplay)
{

  Date today;
  static bool firstcall = true;

  today.year  = t->tm_year + 1900;
  today.month = t->tm_mon + 1;
  today.day   = t->tm_mday;
  today.hour  = t->tm_hour;

  Date gan,zhi;

  Solar2Lunar(&today, &gan, &zhi);

  GanZhiDisplay(&gan,gantxt,true);
  GanZhiDisplay(&zhi,zhitxt,false);

  if(today.hour != 23 && !firstcall) return;
  if(firstcall) firstcall = false;
  
  if(ZhDisplay) 
	CDateDisplayZh(&today,cdtext);

  else
	CDateDisplayNo(&today,cdtext);

}


void GenerateKeText(PblTm *t, char *text)
{
  static char ZhQuarter[4][zhLen+1] = { "初", "一", "二", "三" };
  static char ZhHour[2][zhLen+1] = { "初", "正" };

  memcpy(text, ZhHour[(t->tm_hour+1)%2], zhLen );
  memcpy(text+zhLen+1, ZhQuarter[t->tm_min/15], zhLen );

}


#define nHex	64
#define minl	60
#define qual	900
#define hexal	14
#define nblock	4
#define blockl	225
#define blockl_hex	16

bool IfNewHexa(PblTm *t)
{
  bool result = false;
  static bool first_call = true;
  static int sec = 0;

  if(first_call) {
  	sec = (minl*t->tm_min + t->tm_sec) % qual % blockl;
	first_call = false;
	result = true;
  }
  else if(sec%hexal==0 && sec!=blockl-1)
	result = true;
  else if(sec==blockl) {
	sec = 0;
	result = true;
  }

  sec++;

  return result;
}

void GenerateHexagram(PblTm *t, char *text)
{

  static bool is_hexa_def = false;
  static char Hexa[nHex][zhLen+2] ={"䷀ \0", "䷁ \0", "䷂ \0", "䷃ \0", "䷄ \0", "䷅ \0", "䷆ \0", "䷇ \0", "䷈ \0", "䷉ \0", "䷊ \0", "䷋ \0", "䷌ \0", "䷍ \0", "䷎ \0", "䷏ \0", "䷐ \0", "䷑ \0", "䷒ \0", "䷓ \0", "䷔ \0", "䷕ \0", "䷖ \0", "䷗ \0", "䷘ \0", "䷙ \0", "䷚ \0", "䷛ \0", "䷜ \0", "䷝ \0", "䷞ \0", "䷟ \0", "䷠ \0", "䷡ \0", "䷢ \0", "䷣ \0", "䷤ \0", "䷥ \0", "䷦ \0", "䷧ \0", "䷨ \0", "䷩ \0", "䷪ \0", "䷫ \0", "䷬ \0", "䷭ \0", "䷮ \0", "䷯ \0", "䷰ \0", "䷱ \0", "䷲ \0", "䷳ \0", "䷴ \0", "䷵ \0", "䷶ \0", "䷷ \0", "䷸ \0", "䷹ \0", "䷺ \0", "䷻ \0", "䷼ \0", "䷽ \0", "䷾ \0", "䷿ \0"};
  static int idx[nHex];
  
  static int hexa_count = 0;

  if(!is_hexa_def)
  {
	int valhex[nHex] = {000,077,056,035,050,005,075,057,010,004,070,007,002,020,073,067,046,031,074,017,026,032,037,076,006,030,036,041,055,022,043,061,003,060,027,072,012,024,053,065,034,016,040,001,047,071,045,051,042,021,066,033,013,064,062,023,011,044,015,054,014,063,052,025};
	int sec = 0, i;
  	for(i=0;i<nHex;i++)  idx[valhex[i]] = i;

  	sec = (minl*t->tm_min + t->tm_sec) % qual;
	hexa_count = sec/blockl*blockl_hex + ((sec+1)%blockl==0?sec-1:sec)%blockl/hexal;
 	is_hexa_def = true;
  }
  else
	hexa_count++;

  if(hexa_count%nHex==0) hexa_count = 0;

  memcpy(text, Hexa[idx[hexa_count]], zhLen );

}
