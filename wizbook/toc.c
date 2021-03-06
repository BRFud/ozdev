#include <oz.h>
#include "wizbook.h"

static unsigned line;

static void highlight(void)
{
    box239(line*font_height,font_height,XOR);
}

#define load(_pos) \
{ \
    static int n; \
    myseek(trim(_pos)); \
    n=read1(filebuf,MAX_CLINE); \
    if(n<=0) errread(); \
    filebuf[n]=0; \
    dcompline(filebuf); \
}

void reload(unsigned len)
{
    myseek(buffer_offset);
    if(read1(filebuf,len)<0) errread();
    reseek=1;
}

unsigned toc(getfun get,delfun del,unsigned _startpos,unsigned _num)
/* table of contents viewer */
{
    static unsigned pos,p0;
    static unsigned i,num,nlines;
    static unsigned current;
    byte y;
    static unsigned k;
    static unsigned startpos;
    byte redraw=1;
    startpos=_startpos;
    num=_num;
    nlines=80/font_height;
    line=startpos%nlines;
    pos=startpos-line;
    if(!num) return startpos;
    ozcls();
    for(;;)
    {
        if(!num) goto GETOUT;
        if(redraw)
        {
            for(y=i=0;i<nlines && (p0=pos+i)<num;i++,y+=font_height)
            {
                load(get(p0));
                setmyfont();
                ozputsf(y);
            }
            clearrest(y);
            if(pos) uparrow();
            if(pos+nlines<num) downarrow();
            redraw=0;
        }
        current=pos+line;
        highlight();
        k=mygetch();
        highlight();
        switch(k)
        {
            case 'c':
                if(del!=NULL)
                {
                    clearbookmarks();
                    if(!numbookmarks) return 0;
                }
                break;
            case KEY_DELETE:
                if(del!=NULL)
                {
                    del(current);
                    num--;
                    redraw=1;
                    if(current<startpos) startpos--;
                    if(current==num) goto UP;
                }
                break;
            case KEY_UP:
                UP:
                if(!line)
                {
                    if(!pos) break;
                    pos-=nlines;
                    line=nlines-1;
                    redraw=1;
                }
                else line--;
                break;
            case KEY_DOWN:
                if(pos+line<num-1)
                {
                    line++;
                    if(line==nlines)
                    {
                        pos+=nlines;
                        line=0;
                        redraw=1;
                    }
                }
                break;
            case KEY_PAGEUP:
                if(pos)
                {
                    pos-=nlines;
                    redraw=1;
                }
                else
                    line=0;
                break;
            case KEY_PAGEDOWN:
                if(pos+nlines<num)
                {
                    pos+=nlines;
                    redraw=1;
                }
                else
                    line=num-pos-1;
                break;
            case KEY_RETURN:
                reload(MAX_CLINE+1);
                goto_pos(get(current));
                return current;
            case KEY_LOWER_ESC:
            GETOUT:
                reload(MAX_CLINE+1);
                return startpos;
        }
    }
}

