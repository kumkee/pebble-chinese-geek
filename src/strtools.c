#include "pebble_os.h"
#include "strtools.h"

/* is c the start of a utf8 sequence? */
#define isutf(c) (((c)&0xC0)!=0x80)

/* byte offset => charnum
  by Jeff Bezanson
  placed in the public domain Fall 2005
 */
int u8_charnum(char *s, int offset)
{
    int charnum = 0, offs=0;

    while (offs < offset && s[offs]) {
        (void)(isutf(s[++offs]) || isutf(s[++offs]) ||
               isutf(s[++offs]) || ++offs);
        charnum++;
    }
    return charnum;
}

//-------------------------------------------------------------

int str_len(char* s)
{
    char c;
    int i=0;

    do{
	c = s[i];
	i++;
    }
    while(c!=0);

    return i-1;
}


void str_reverse(char* s)
{
    char c;
    int n = str_len(s);
    int h = n/2;
    int i;

    for(i=0;i<h;i++)
    {
	c = s[i];
	s[i] = s[n-1-i];
	s[n-1-i] = c;
    }
}


void str_verticize(char* s)
{
    int n = str_len(s);
    str_reverse(s);
    char cpy[30];
    int i;
 
    //cpy[n-1] = 0;
    //for(i=0;i<n;i++)
//	cpy[i] = s[i];
    memcpy(cpy, s, n+1);

    for(i=1;i<n;i++) {
	s[2*i] = cpy[i];
	s[2*i-1] = '\n';
    }
    s[2*n-1] = 0;
}


#define zhLen 3
void str_verticize_zh(char* s)
{
    int m = u8_charnum(s,25);
    int n = str_len(s);
    char cpy[50];
    int i;
    int offs = m>=6?0:1;

    memcpy(cpy, s, n+1);

    for(i=0;i<m;i++)
    {
	if(i==0 && offs==0)
	    s[0] = '\n';
	else
	    s[i*(zhLen+1)-1+offs] = '\n';
	memcpy(s+i*(zhLen+1)+offs, cpy+i*zhLen, zhLen);
    }

    s[m*zhLen + m-1 + offs] = 0;
}
