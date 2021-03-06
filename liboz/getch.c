#include <oz.h>
#include <ctype.h>

static byte mapreturn=1;
static byte enterisreturn=1;

void ozenterisreturn(byte map)
{
    enterisreturn=map;
}

void ozkeymapcrtolf(byte map)
{
	mapreturn=map;
}

static unsigned ozkeyread(byte read)
{
	static byte isshifted=0;
	static byte is2nded=0;
	static byte iscapslock=0;
	static byte isctrl=0;
	static byte iscat=0;
	static byte shiftheld=0;
	static byte catheld=0;
	static byte secondheld=0;
	static byte ctrlheld=0;
	static unsigned int c;
	static unsigned int origc;
/*	  termputch(isshifted?'+':'-'); */
	if(!read && !ozkeyhit()) return NO_KEY;
	origc=ozgetch();
	c=origc&0xF0FF;
	switch(c)
	{
		case KEY_CATEGORY:
           iscat^=1;
		   return NO_KEY;
		case KEY_LEFT_SHIFT & 0xF0FF:
		   if(is2nded)
		   {
              iscapslock^=1;
			  iscat=is2nded=0;
			  return NO_KEY;
		   }
		case KEY_RIGHT_SHIFT:
/*			 termputch('!'); */
           isshifted^=1;
		   return NO_KEY;
		case KEY_2ND:
           is2nded^=1;
		   return NO_KEY;
		case KEY_NEW:
           isctrl^=1;
		   return NO_KEY;
		case KEY_DEL:
		   c=8;
		   break;
		case KEY_LOWER_ESC:
		   c=27;
		   break;
		case KEY_LOWER_ENTER:
		case KEY_UPPER_ENTER:
		case '\r':
           if(c=='\r' || enterisreturn)
           {
              if(mapreturn) c='\n'; else c='\r';
           }
		default:
		   if(!read) break;
		   if(isshifted || origc&MASKSHIFT) shiftheld=1; else shiftheld=0;
		   if(isctrl || origc&MASKCTRL) ctrlheld=1; else ctrlheld=0;
		   if(iscat || origc&MASKCATEGORY) catheld=1; else catheld=0;
		   if(is2nded || origc&MASK2ND) secondheld=1; else secondheld=0;
		   c&=0xF0FF;
		   if(catheld && !ctrlheld)
		   {
			   switch(c)
			   {
				 case 'o': c='['; break;
				 case 'p': c=']'; break;
				 case '9': c='{'; break;
				 case '0': c='}'; break;
				 case 'r': c='\\'; break;
				 case 'w': c='`'; break;
                 case 'l':
				 case 'q': c='~'; break;
				 case '1': c='|'; break;
				 case ',': c='<'; break;
				 case '.': c='>'; break;
				 default: c|=MASKCATEGORY; break;
				}
		   }
           else if(ctrlheld && !catheld && c<128 && isalpha(c))
			   c-='a'-1;
		   else if(ctrlheld && c==' ')
			   c=0;
		   else if(ctrlheld && catheld)
		   {
			   switch(c)
			   {
				 case 'o': c=27; break;
				 case 'p': c=29; break;
				 case 'r': c=28; break;
				 case 'q': c=30; break;
			   }
		   }
		   else if(ctrlheld && secondheld && c=='i')
				  c=31;
		   else if(ctrlheld && c=='6')
				  c=0x1E;
		   else if(ctrlheld && c=='-')
				  c=0x1F;
           else if((shiftheld || iscapslock) && c<128 && isalpha(c) && !ctrlheld
			   && !catheld && !secondheld)
               {
                    if(shiftheld ^ iscapslock) c+='A'-'a';
               }
           else if((shiftheld /* || secondheld */) && c=='-')
			   c='_';
           else if(shiftheld && c<128 && isdigit(c))
			   c=")!@#$%^&*("[c-'0'];
           else if((/* secondheld || */ shiftheld) && c=='.')
			   c=':';
           else if((/* secondheld || */ shiftheld) && c==',')
			  c='\'';
		   else if(secondheld)
		   {
			  switch(c)
			  {
				case 'e': c='*'; break;
				case 'i': c='?'; break;
				case 'o': c='"'; break;
				case 'p': c=';'; break;
				case 'q': c='+'; break;
				case 'r': c='/'; break;
				case 't': c='='; break;
				case 'w': c='-'; break;
                default: c|=MASK2ND; break;
			  }
              if(ctrlheld) c|=MASKCTRL;
              if(shiftheld) c|=MASKSHIFT;
		   }
           else if(ctrlheld || shiftheld)
           {
               if(ctrlheld) c|=MASKCTRL;
               if(shiftheld) c|=MASKSHIFT;
           }
	 }
	if(!read)
		ozungetch(origc);
	else
		isshifted=is2nded=isctrl=iscat=0;
	return c;
}

int kbhit(void)
{
	static int k;
	static int l;
	while((k=ozkeyhit()) && (l=(NO_KEY==ozkeyread(0))));
	return k && !l;
}

unsigned getch(void)
{
	static unsigned c;
    while(NO_KEY==(c=ozkeyread(1))) ozintwait();
	return c;
}

/*
main()
{
	int x,y,c;
	x=0;
	y=0;
	while((c=getch())!=27)
	{
		x=ozputch(x,y,c);
		if(x>230)
		{
			y+=10;
			x=0;
		}
	}
}
*/
