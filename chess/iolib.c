/*
  These routines are just misc routines used by IO.C
*/

#include <ozinput.h>

#define HEADER
#include "arvar.c"
/* #define BENCHMARK */

/* The boards at the start of a game */
static tiny sboard[64]=
    {ROOK  ,KNIGHT,BISHOP,QUEEN ,KING  ,BISHOP,KNIGHT,ROOK  ,
     PAWN  ,PAWN  ,PAWN  ,PAWN  ,PAWN  ,PAWN  ,PAWN  ,PAWN  ,
     EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,
     EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,
     EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,
     EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,EMPTY ,
     PAWN  ,PAWN  ,PAWN  ,PAWN  ,PAWN  ,PAWN  ,PAWN  ,PAWN  ,
     ROOK  ,KNIGHT,BISHOP,QUEEN ,KING  ,BISHOP,KNIGHT,ROOK};

static tiny scolor[64]=
    {WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,
     WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,
     NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,
     NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,
     NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,
     NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,NONE ,
     BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
     BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK};

void clearboard(void)
/*this clears the board*/
{register int x;
for (x=63;x>=0;x--)
   {board[x]=EMPTY;brdcolor[x]=NONE;mvboard[x]=0;}
newgame();
}

void newboard(void)
/* this sets up a new board */
{register int x;
for (x=63;x>=0;x--)
   {board[x]=sboard[x];brdcolor[x]=scolor[x];mvboard[x]=0;}
newgame();
}

void newgame(void)
{int x;
game50=movenum=0;
gameptr=&gamehist[movenum];
epsquare=-1;
findpieces();
mate=draw=FALSE;
/* pvsmsg[0]=*/ msg[0]=status[0]=0;
timeout=FALSE;extratime=0;resetclock();
for (x=63;x>=0;x--)
   if ((board[x]==sboard[x]) && (brdcolor[x]==scolor[x])) mvboard[x]=0;
   else mvboard[x]=10;
castled[WHITE]=castled[BLACK]=FALSE;
}

void initgame(void)
{
newboard();responsetime=30;extratime=0;playlevel=3;tcontrol=TRUE;
msg[0]=/* pvsmsg[0]=*/ status[0]=0;
epsquare=-1;
estscore=0;
/* breakpress=*/ quit=mate=bothsides=reverse=/* pvsflag=*/FALSE;
qsearch=TRUE;
computer=BLACK;human=WHITE;tomove=human;
resetclock();elaptime();
}

int cmdis(char *str)
/* this is just an eaiser way to check if str is the cmdline */
{ return(strcmp(cmdline,str)==0); }

void getline(char *s,int len)
{
    register char *p=s;
    static unsigned c;
    while(1)
    {
        switch(c=ozgetch())
        {
            case KEY_MYPROGRAMS:
            case KEY_UPPER_ESC:
            case KEY_LOWER_ESC:
                strcpy(s,"QUIT");
                return;
            case KEY_BACKSPACE:
                if(p>s)
                {
                    *--p=0;
                    termputs("\8 \8");
                }
                break;
            case KEY_LOWER_ENTER:
            case KEY_UPPER_ENTER:
            case '\r':
                *p=0;
                return;
            default:
                if(c>='a') c&=0xff-0x20;
                if(p+1<s+len)
                {
                    *p++=c;
                    termputch(c);
                }
                break;
        }
    }
}

void readcmd(void)
/* Read a string from the keyboard.  Maximum of 70 characters long.
   Also, convert all lowercase to UPPER case so we can compare commands */
{ int len,x;char c;
/* len=readln(cmdline,70);*/   /* read max 70 chars */
 cmdline[0]=0;
 getline(cmdline,70);
 len=strlen(cmdline);
 if (len)
   if (cmdline[len-1]=='\n') {cmdline[len-1]='\0';len--;}
}


bool verifymove(void)
/* verify that the humans move is valid */
{
 int f,t,prom,x,start;
 char c,r,m[10];
 bool valid;
 movetype *mv,*tmv;
 movetype *last;
 movetype buffer[MAXPLYMVS];

findpieces();
valid=FALSE;prom=0;
strcpy(msg,"INVALID CMD");
f=kingloc[tomove];
if (cmdis("O-O")) t=f+2;
else if (cmdis("O-O-O")) t=f-2;
else {if (strlen(cmdline)<=3) return(0);
      c=(cmdline[0])-'A';r=(cmdline[1])-'1';f=r*8+c;
      c=(cmdline[2])-'A';r=(cmdline[3])-'1';t=r*8+c;
      if (cmdline[4]=='Q') prom=QUEEN;
      if (cmdline[4]=='R') prom=ROOK;
      if (cmdline[4]=='B') prom=BISHOP;
      if (cmdline[4]=='N') prom=KNIGHT;
     }

last=genlegal(buffer,tomove);

mv=NULL;tmv=buffer;
while (tmv < last)
  {tmv++;
   if ((f==tmv->f) && (t==tmv->t) && ((tmv->flags & 7)==prom) )
      mv=tmv;
  }

if (mv)
  {processmove(mv);valid=TRUE;tomove=chngcolor(tomove);
   mvenglish(mv,m);strcpy(msg,"You did:");
   strcat(msg,m);
  }

checkstatus();
return(valid);
}

#ifdef BENCHMARK

void gentime(void)
{int x;
 movetype buffer[MAXPLYMVS];
 char s[20];

resetclock();
for (x=0;x < 1000; x++)  genmoves(buffer,tomove);
elaptime();

itoa(1000/ElapClock,msg);
strcat(msg," genmoves/sec");
}

void captime(void)
{int x;
 movetype buffer[MAXPLYMVS];
 char s[20];

resetclock();
for (x=0;x < 1000; x++)  gencaps(buffer,tomove);
elaptime();

itoa(1000/ElapClock,msg);
strcat(msg," gencaps/sec");
}

void evaltime(void)
{int x;
 movetype fakemv;bool check;
 char s[20];
/*
*/
updateweights(0);
level=10;
  /* set search level so full eval, but no updateweights */
resetclock();
for (x=0;x < 1000; x++)
  {
   estscore=evalu8(&fakemv,0,tomove,-BIGNUM,BIGNUM,5,&check,FALSE);
  }
elaptime();

itoa(1000/ElapClock,msg);
strcat(msg," evalu8/sec");
}
#endif
