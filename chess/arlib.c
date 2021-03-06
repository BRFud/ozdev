/*
  This module has misc. general purpose routines that are used throughout
  the entire chess program.
*/


#define HEADER
#include "arvar.c"

void findpieces(void)
/* find location of the king, the number of pawns, and the number of
   pawns on each column */
{ register int x;
pawncnt[WHITE]=pawncnt[BLACK]=pawncnt[NONE]=0;
kingloc[WHITE]=kingloc[BLACK]=kingloc[NONE]=-1;
for (x=7;x>=0;x--) pawncol[WHITE][x]=pawncol[BLACK][x]=pawncol[NONE][x]=0;

for (x=63;x>=0;x--) {
  if (board[x]==KING) kingloc[brdcolor[x]]=x;
  if (board[x]==PAWN)
    { pawncnt[brdcolor[x]]++;
      pawncol[brdcolor[x]][(x & 7)]++;
    }
  }
mtl[WHITE]=mtl[BLACK]=mtl[NONE]=0;
for (x=63;x>=0;x--) mtl[brdcolor[x]]+=pieceval[board[x]];
}

void mvenglish(movetype *mv,char *st)
/*translate the move into english*/
{static char s[10];static int f,t;

f=mv->f;t=mv->t;
if (mv->flags & CASTLE)
  {if (t>f) strcpy(st,"O-O");
   else     strcpy(st,"O-O-O");
   return;
  }
s[0]=('A'+getcol(f));s[1]=('1'+getrow(f));
s[2]='-';if (mv->flags & CAPTURE) s[2]='x';
s[3]=('A'+getcol(t));s[4]=('1'+getrow(t));
s[5]=name[mv->flags & 7];s[6]=0;
if (mv->flags & ENPASSANT) {s[5]='e';s[6]='p';}
s[7]=0;
strcpy(st,s);
}

void checkstatus(void)
/* this calls findpiece and then checks to make sure both kings exist.
   if not, then the global flag mate is set */
/* This needs some work.  It should also check the number of legal
   moves the side to move can make (in case of stalemate).  */
{ static int side;

mate=FALSE;findpieces();status[0]=0;
if ((kingloc[WHITE]<0) || (kingloc[BLACK]<0)) mate=TRUE;
if (mate)
  {side=WHITE;if (kingloc[WHITE]<0) side=BLACK;
   if (human==BLACK) side=chngcolor(side);
   if (side==WHITE) strcpy(status,"White mates");
   else strcpy(status,"Black mates");
   bothsides=FALSE;
  }

if (repetition()>2) draw=TRUE;

if (draw)
   {strcpy(status,"Drawn game");
    bothsides=FALSE;
   }
}

bool mveq(movetype *mv1,movetype *mv2)
{
if (mv1->f != mv2->f) return(FALSE);
if (mv1->t != mv2->t) return(FALSE);
if ((mv1->flags & 0xff) != (mv2->flags & 0xff)) return(FALSE);
return(TRUE);
}

#if 0
int distance(int x,int y)
/* The distance between two squares.  Only the greatest of the distances
   of the rows or the columns is the 'distance'.  */
{ static int a,b;
  a=abs(getcol(x)-getcol(y));
  b=abs(getrow(x)-getrow(y));
  if (a > b) return(a); else return(b);
}
#endif
