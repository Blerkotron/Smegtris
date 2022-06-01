/* ==================================== */
/* |       SMEGTRIS version 1.00      | */
/* | (C)1994, Cheese Freak Industries | */
/* |    Written in Microsoft C 7.0    | */
/* |   This source is public domain   | */
/* |    but must *NOT* be altered!    | */
/* ==================================== */

/* Future Improvements :
    sound effects?
    Two player serial link
    GAME B (as on Gameboy version) */

/* ========================================================================= */
/* | The #included files go here.......don't forget to link GRAPHICS.LIB!  | */
/* ========================================================================= */
#include <stdio.h>
#include <time.h>
#include <graph.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>

#include "smegtris.h"


/* ========================================================================= */
/* | Oh no! Global Variables! Still, not many, eh chums?                   | */
/* ========================================================================= */
unsigned char left=LEFT,
              right=RIGHT,
              rot=TAB,
              drop=SPACE;

unsigned char leftkey[10]="CURSLEFT",
              rightkey[10]="CURSRGHT",
              rotkey[10]="TAB",
              dropkey[10]="SPACE";

KEY speckeys[15]={9,"TAB",8,"BSPACE",13,"ENTER",164,"INSERT",166,"DELETE",
                  142,"HOME",158,"END",146,"PGUP",162,"PGDN",144,"UP",
                  160,"DOWN",150,"LEFT",154,"RIGHT",27,"ESCAPE",32,"SPACE"};


/* ========================================================================= */
/* | Oh crikey, it's the main program!                                     | */
/* ========================================================================= */

void main(void)
{
    long int hisc[20]=START_SCORES;
    int level;
    long int score;
    unsigned char temp_ch;
    unsigned char hiname[20][21]=START_NAMES;

    loadscores(hisc,hiname);
    init();

    while(TRUE)
    {
        temp_ch=0;
        menu();
        while(!temp_ch)
        {
            while(!_kbhit());
            temp_ch=_getch();
            temp_ch=tolower(temp_ch);
            if (temp_ch!=13 && temp_ch!=32 && temp_ch!=27 && temp_ch!='c' && temp_ch!='r') temp_ch=0;
        };
        switch(temp_ch)
        {
            case ENTER:
                display_hi(hisc,hiname);
                break;

            case SPACE:
                level=get_lev();
                score=game(level);
                game_over(score);
                if (score>hisc[19]) new_high(score,hisc,hiname);
                display_hi(hisc,hiname);
                break;

            case 'c':
                display_credits();
                break;

            case 'r':
                redefkeys();
                break;

            case ESCAPE:
                _setvideomode(_DEFAULTMODE);
                restrate();
                printf("Thanks for playing Smegtris!\n(C)1994 Cheese Freak Software\n\n");
                exit(0);
        };
    };
}


/* ========================================================================= */
/* | And here are all the little functions.......                          | */
/* ========================================================================= */

void savescores(long int hisc[20],unsigned char hiname[20][21])
{
    unsigned char buffer[1500],tempsc[30];
    short int count,count2;
    int buf=0;
    FILE *fp;

    for(count=0;count<20;count++)
    {
        for(count2=0;count2<21;count2++)
        {
            buffer[buf]=hiname[count][count2]+50;
            buf++;
        };

        sprintf(tempsc,"%ld",hisc[count]);
        for(count2=0;count2<30;count2++)
        {
            buffer[buf]=tempsc[count2]+50;
            buf++;
        };
    };

    fp=fopen("smegtris.dat","wb");
    if(fp!=NULL)
    {
        fwrite(buffer,1020,1,fp);
        fclose(fp);
    };
}


void loadscores(long int hisc[20],unsigned char hiname[20][21])
{
    unsigned char buffer[1500],tempsc[30];
    short int count,count2;
    int buf=0;
    FILE *fp;

    fp=fopen("smegtris.dat","rb");
    if(fp!=NULL)
    {
        fread(buffer,1020,1,fp);
        fclose(fp);

        for(count=0;count<20;count++)
        {
            for(count2=0;count2<21;count2++)
            {
                hiname[count][count2]=buffer[buf]-50;
                buf++;
            };

            for(count2=0;count2<30;count2++)
            {
                tempsc[count2]=buffer[buf]-50;
                buf++;
            };
            hisc[count]=atol(tempsc);
        };
    }
}


void clrbuf(void)
{
    while(_kbhit())
        _getch();
}


void setrate(void)
{
    union _REGS rg;

    rg.h.ah = 0x03;
    rg.h.al = 0x05;
    rg.h.bh = 0x00;
    rg.h.bl = 0x00;

    _int86(0x16, &rg, &rg);
} 


void restrate(void)
{
    union _REGS rg;

    rg.h.ah = 0x03;
    rg.h.al = 0x05;
    rg.h.bh = 0x02;
    rg.h.bl = 0x00;

    _int86(0x16, &rg, &rg);
} 


void redefkeys(void)
{
    unsigned char temp;
    short int count,check;
    unsigned char string[10];
    short int OKAY;

    _clearscreen(_GCLEARSCREEN);
    centre(11,7,"REDEFINE KEYS");
    centre(14,9,"Press the key you want for :");

    for(count=0;count<4;count++)
    {
        OKAY=FALSE;
        switch(count)
        {
            case 0:
                strcpy(string,"LEFT");
                break;

            case 1:
                strcpy(string,"RIGHT");
                break;

            case 2:
                strcpy(string,"ROTATE");
                break;

            case 3:
                strcpy(string,"DROP");
                break;
        };
        _settextcolor(12);
        _settextposition((12+count),13);
        _outtext(string);

        while(!OKAY)
        {
            temp=inkey();
            temp=toupper(temp);
            if(temp>32 && temp<128)
            {
                OKAY=TRUE;
                string[0]=temp;
                string[1]=0;
            }
            else
            {
                for(check=0;check<15;check++)
                {
                    if(speckeys[check].val==temp && temp!=ESCAPE)
                    {
                        OKAY=TRUE;
                        strcpy(string,speckeys[check].name);
                    };
                };
            };
        };

        _settextcolor(15);
        _settextposition((12+count),(28-strlen(string)));
        _outtext(string);

        switch(count)
        {
            case 0:
                left=temp;
                strcpy(leftkey,string);
                break;

            case 1:
                right=temp;
                strcpy(rightkey,string);
                break;

            case 2:
                rot=temp;
                strcpy(rotkey,string);
                break;

            case 3:
                drop=temp;
                strcpy(dropkey,string);
                break;
        };
    };
    pause(300);
}


void pause(unsigned int wait)
{
    clock_t goal;

    goal=((time_t) wait)+clock();
    while(goal>clock());
};


void centre(int colour,int x,unsigned char *text)
{
    int y,oldcol;

    oldcol=_settextcolor(colour);
    y=(42-(strlen(text)))/2;
    _settextposition(x,y);
    _outtext(text);
    _settextcolor(oldcol);
}


void menu(void)
{
    unsigned char temp[40];

    _clearscreen(_GCLEARSCREEN);
    centre(12,2,"SMEGTRIS");
    centre(11,4,"(C)1994 CHEESE FREAK INDUSTRIES");
    centre(12,5,"Version 1.00, 20th April 1994");
    centre(10,7,"INSTRUCTIONS");
    centre(7,8,"This is a blatant TETRIS clone, so");
    centre(7,9,"you already know how to play!");
    centre(10,10,"PLAYING KEYS (R to REDEFINE)");
    strcpy(temp,"LEFT: ");
    strcat(temp,leftkey);
    strcat(temp,"    RIGHT: ");
    strcat(temp,rightkey);
    centre(7,11,temp);
    strcpy(temp,"ROTATE: ");
    strcat(temp,rotkey);
    strcat(temp,"    DROP: ");
    strcat(temp,dropkey);
    centre(7,12,temp);
    centre(14,14,"This game is Public Domain, so do what");
    centre(14,15,"you like with it, but don't charge for");
    centre(14,16,"it or change it in any way. Ta muchly.");
    centre(9,18,"By the way - it's called SMEGTRIS 'cos");
    centre(9,19,"it's so basic and generally crappy");
    centre(9,20,"(in a funky skillo sort of way)...!");
    centre(15,22,"SPACE to play, RETURN for High Scores,");
    centre(15,23,"C for Credits, or ESC to quit to DOS");
}


void init(void)
{
    _displaycursor(_GCURSOROFF);
    _setvideomoderows(_TEXTC40,25);
    _clearscreen(_GCLEARSCREEN);
    srand((unsigned)time(NULL));
    setrate();
}


int rnd(int x)
{
    int r,ans;

    r=rand();
    ans=(r-(x*(r/x)));
    return ans;
}


int get_lev(void)
{
    int level;
    unsigned char read_key=' ';

    _clearscreen(_GCLEARSCREEN);
    centre(13,8,"Which Level of Play?");
    centre(15,10,"(0 - Slow, 9 - Fast)");
    while(read_key==' ')
    {
        while(!_kbhit());
        read_key=_getch();
        if (read_key>'9' || read_key<'0') read_key=' ';
    };
    level=read_key-48;
    return level;
}


void display_hi(long int hisc[20],unsigned char hiname[20][21])
{
    int count,length;
    unsigned char temp[21],temp_ch;

    _clearscreen(_GCLEARSCREEN);
    centre(12,2,"TODAYS SMEGGIEST PLAYERS");
    for (count=0; count<20; count++)
    {
        sprintf(temp,"%ld",hisc[count]);
        length=strlen(temp);
        _settextcolor(2);
        _settextposition((count+4),7);
        _outtext(hiname[count]);
        _settextcolor(14);
        _settextposition((count+4),(34-length));
        _outtext(temp);
    };
    _settextcolor(7);
    while(!_kbhit());
    temp_ch=_getch();
}


void game_over(long int score)
{
    unsigned char line[38]="Final Score: ";
    unsigned char temp_ch,temp[21];
    short int repeat;

    _settextcolor(15);

    for(repeat=10;repeat>0;repeat--)
    {
        _settextposition(2,5);
        _outtext("€€€€€€€€€€");
        pause(100);
        _settextposition(2,5);
        _outtext("          ");
        pause(100);
    };

    smiley();
    sprintf(temp,"%ld",score);
    strcat(line,temp);
    centre(31,8,"GAME OVER, SUCKER!");
    centre(14,10,line);
    clrbuf();
    while(!_kbhit());
    temp_ch=_getch();
}


void smiley(void)
{
    short int count,ex,wy,col;
    unsigned char face[2]={2,0};
    unsigned char key=0;

    count=0;
    clrbuf();
    while (count<2000 && key==0)
    {
        ex=(rnd(24))+1;
        wy=(rnd(40))+1;
        col=(rnd(15))+1;
        _settextposition(ex,wy);
        _settextcolor(col);
        _outtext(face);
        count++;
        if (_kbhit()) key=_getch();
    };
}


void new_high(long int score,long int hisc[20],unsigned char hiname[20][21])
{
    unsigned char newname[21];
    int newpos,count;

    _clearscreen(_GCLEARSCREEN);
    centre(30,8,"WELL DONE!");
    centre(11,9,"You've made the HIGH SCORE TABLE!");
    centre(15,11,"Enter your name!");
    get_name(newname);
    newpos=hipos(score,hisc);
    switch(newpos)
    {
        case 19:
            strcpy(hiname[19],newname);
            hisc[19]=score;
            break;

        default:
            for (count=18; count>=newpos; count--)
            {
                strcpy(hiname[(count+1)],hiname[count]);
                hisc[(count+1)]=hisc[count];
            };
            strcpy(hiname[newpos],newname);
            hisc[newpos]=score;
            break;
    };
    savescores(hisc,hiname);
}


void get_name(unsigned char newname[21])
{
    int count=0;
    int fin;
    unsigned char temp_ch;

    fin=FALSE;
    strcpy(newname,"____________________");
    while(!fin)
    {
        centre(12,12,newname);
        while(!_kbhit());
        temp_ch=_getch();
        if (temp_ch==13)
        {
            newname[count]=0;
            fin=TRUE;
        };
        if (temp_ch==8 && count>0)
        {
            count--;
            newname[count]='_';
        };
        if (temp_ch<'~' && temp_ch>=' ' && count<20)
        {
            temp_ch=(toupper(temp_ch));
            newname[count]=temp_ch;
            count++;
        };
    };
    for (count=0;count<20;count++) if (newname[count]=='_') newname[count]=' ';
    if ((strlen(newname))==0) strcpy(newname,"SMEGHEAD");
}


int hipos(long int score,long int hisc[20])
{
    int count=19;

    while(score>hisc[count] && count>=0) count--;
    count++;
    return count;
}


void display_credits(void)
{
    unsigned char temp_ch;

    _clearscreen(_GCLEARSCREEN);
    centre(12,2,"CREDITS");
    centre(9,4,"or WHO TO BLAME WHEN IT GOES WRONG");
    centre(10,7,"All code, graphics and everything -");
    centre(12,8,"The infamous BLOOD!");
    centre(10,10,"Coded entirely in -");
    centre(12,11,"Microsoft C 7.0");
    centre(10,13,"All code is copyright of -");
    centre(12,14,"CHEESE FREAK INDUSTRIES");
    centre(14,17,"A FEW HELLOS GO TO -");
    centre(11,18,"Greg, Sea, Andy, Matt, and the");
    centre(11,19,"SHU drunken smegheads, see ya soon!");
    centre(11,20,"Jackie, Wij, Mark, Payney, Pete,");
    centre(11,21,"and everyone back in boring town.");
    centre(11,22,"Finally, Boyd & all my so called");
    centre(11,23,"pals at work for constant abuse.");
    while(!_kbhit());
    temp_ch=_getch();
}


void backdrop(void)
{
    int count;

    _clearscreen(_GCLEARSCREEN);
    _settextcolor(6);
    _settextposition(2,4);
    _outtext("±±±±±±±±±±±±");
    for (count=3;count<23;count++)
    {
        _settextposition(count,4);
        _outtext("±");
        _settextposition(count,15);
        _outtext("±");
    };
    _settextposition(23,4);
    _outtext("±±±±±±±±±±±±");

    _settextcolor(14);
    _settextposition(2,18);
    _outtext("…ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕª");
    for (count=3;count<9;count++)
    {
        _settextposition(count,18);
        _outtext("∫");
        _settextposition(count,37);
        _outtext("∫");
    };
    _settextposition(9,18);
    _outtext("»ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº");
    _settextcolor(12);
    _settextposition(4,24);
    _outtext("SMEGTRIS");
    _settextcolor(11);
    _settextposition(6,21);
    _outtext("(C)1994 CHEESE");
    _settextposition(7,20);
    _outtext("FREAK INDUSTRIES");

    _settextcolor(14);
    _settextposition(17,21);
    _outtext("…ÕÕÕÕÕÕÕÕÕÕÕÕª");
    for (count=18;count<22;count++)
    {
        _settextposition(count,21);
        _outtext("∫");
        _settextposition(count,34);
        _outtext("∫");
    };
    _settextposition(22,21);
    _outtext("»ÕÕÕÕÕÕÕÕÕÕÕÕº");
    _settextcolor(10);
    _settextposition(19,29);
    _outtext("NEXT");
    _settextposition(20,28);
    _outtext("SHAPE");

    _settextcolor(12);
    _settextposition(11,23);
    _outtext("ØØSCORE!ÆÆ");
    _settextposition(14,19);
    _outtext("LINES       LEV");
    _settextcolor(15);
    _settextposition(12,23);
    _outtext("0000000000");
    _settextposition(14,25);
    _outtext("0000");
    _settextposition(14,35);
    _outtext("00");
}


void clr_board(short int board[22][12])
{
    int count1,count2;

    for (count1=0;count1<22;count1++)
    {
        if (count1==0 || count1==21)
        {
            for (count2=0;count2<12;count2++) board[count1][count2]=31;
        }
        else
        {
            board[count1][0]=31;
            for (count2=1;count2<11;count2++) board[count1][count2]=0;
            board[count1][11]=31;
        };
    };
}


void piece(short int x,short int y,short int col,short int shape)
{
    _settextcolor(col);
    switch(shape)
    {
        case 1:
            _settextposition(x,y);
            _outtext("€€");
            _settextposition(x+1,y);
            _outtext("€€");
            break;

        case 2:
            _settextposition(x,y+1);
            _outtext("€€");
            _settextposition(x+1,y);
            _outtext("€€");
            break;

        case 3:
            _settextposition(x,y);
            _outtext("€");
            _settextposition(x+1,y);
            _outtext("€€");
            _settextposition(x+2,y+1);
            _outtext("€");
            break;

        case 4:
            _settextposition(x,y);
            _outtext("€€");
            _settextposition(x+1,y+1);
            _outtext("€€");
            break;

        case 5:
            _settextposition(x,y+1);
            _outtext("€");
            _settextposition(x+1,y);
            _outtext("€€");
            _settextposition(x+2,y);
            _outtext("€");
            break;

        case 6:
            _settextposition(x,y-1);
            _outtext("€€€");
            _settextposition(x+1,y);
            _outtext("€");
            break;

        case 7:
            _settextposition(x,y);
            _outtext("€");
            _settextposition(x+1,y);
            _outtext("€€");
            _settextposition(x+2,y);
            _outtext("€");
            break;

        case 8:
            _settextposition(x,y);
            _outtext("€");
            _settextposition(x+1,y-1);
            _outtext("€€€");
            break;

        case 9:
            _settextposition(x,y);
            _outtext("€");
            _settextposition(x+1,y-1);
            _outtext("€€");
            _settextposition(x+2,y);
            _outtext("€");
            break;

        case 10:
            _settextposition(x,y-1);
            _outtext("€€€");
            _settextposition(x+1,y-1);
            _outtext("€");
            break;

        case 11:
            _settextposition(x,y);
            _outtext("€");
            _settextposition(x+1,y);
            _outtext("€");
            _settextposition(x+2,y);
            _outtext("€€");
            break;

        case 12:
            _settextposition(x,y+1);
            _outtext("€");
            _settextposition(x+1,y-1);
            _outtext("€€€");
            break;

        case 13:
            _settextposition(x,y);
            _outtext("€€");
            _settextposition(x+1,y+1);
            _outtext("€");
            _settextposition(x+2,y+1);
            _outtext("€");
            break;

        case 14:
            _settextposition(x,y-1);
            _outtext("€€€");
            _settextposition(x+1,y+1);
            _outtext("€");
            break;

        case 15:
            _settextposition(x,y);
            _outtext("€€");
            _settextposition(x+1,y);
            _outtext("€");
            _settextposition(x+2,y);
            _outtext("€");
            break;

        case 16:
            _settextposition(x,y-1);
            _outtext("€");
            _settextposition(x+1,y-1);
            _outtext("€€€");
            break;

        case 17:
            _settextposition(x,y+1);
            _outtext("€");
            _settextposition(x+1,y+1);
            _outtext("€");
            _settextposition(x+2,y);
            _outtext("€€");
            break;

        case 18:
            _settextposition(x,y-1);
            _outtext("€€€€");
            break;

        case 19:
            _settextposition(x-1,y);
            _outtext("€");
            _settextposition(x,y);
            _outtext("€");
            _settextposition(x+1,y);
            _outtext("€");
            _settextposition(x+2,y);
            _outtext("€");
            break;
    };
}


void land_piece(short int x,short int y,short int col,short int shape,short int board[22][12])
{
    x=x-2;
    y=y-4;
    switch(shape)
    {
        case 1:
            board[x][y]=col;
            board[x][y+1]=col;
            board[x+1][y]=col;
            board[x+1][y+1]=col;
            break;

        case 2:
            board[x][y+1]=col;
            board[x][y+2]=col;
            board[x+1][y]=col;
            board[x+1][y+1]=col;
            break;

        case 3:
            board[x][y]=col;
            board[x+1][y]=col;
            board[x+1][y+1]=col;
            board[x+2][y+1]=col;
            break;

        case 4:
            board[x][y]=col;
            board[x][y+1]=col;
            board[x+1][y+1]=col;
            board[x+1][y+2]=col;
            break;

        case 5:
            board[x][y+1]=col;
            board[x+1][y]=col;
            board[x+1][y+1]=col;
            board[x+2][y]=col;
            break;

        case 6:
            board[x][y-1]=col;
            board[x][y]=col;
            board[x][y+1]=col;
            board[x+1][y]=col;
            break;

        case 7:
            board[x][y]=col;
            board[x+1][y]=col;
            board[x+1][y+1]=col;
            board[x+2][y]=col;
            break;

        case 8:
            board[x][y]=col;
            board[x+1][y-1]=col;
            board[x+1][y]=col;
            board[x+1][y+1]=col;
            break;

        case 9:
            board[x][y]=col;
            board[x+1][y-1]=col;
            board[x+1][y]=col;
            board[x+2][y]=col;
            break;

        case 10:
            board[x][y-1]=col;
            board[x][y]=col;
            board[x][y+1]=col;
            board[x+1][y-1]=col;
            break;

        case 11:
            board[x][y]=col;
            board[x+1][y]=col;
            board[x+2][y]=col;
            board[x+2][y+1]=col;
            break;

        case 12:
            board[x][y+1]=col;
            board[x+1][y-1]=col;
            board[x+1][y]=col;
            board[x+1][y+1]=col;
            break;

        case 13:
            board[x][y]=col;
            board[x][y+1]=col;
            board[x+1][y+1]=col;
            board[x+2][y+1]=col;
            break;

        case 14:
            board[x][y-1]=col;
            board[x][y]=col;
            board[x][y+1]=col;
            board[x+1][y+1]=col;
            break;

        case 15:
            board[x][y]=col;
            board[x][y+1]=col;
            board[x+1][y]=col;
            board[x+2][y]=col;
            break;

        case 16:
            board[x][y-1]=col;
            board[x+1][y-1]=col;
            board[x+1][y]=col;
            board[x+1][y+1]=col;
            break;

        case 17:
            board[x][y+1]=col;
            board[x+1][y+1]=col;
            board[x+2][y]=col;
            board[x+2][y+1]=col;
            break;

        case 18:
            board[x][y-1]=col;
            board[x][y]=col;
            board[x][y+1]=col;
            board[x][y+2]=col;
            break;

        case 19:
            board[x-1][y]=col;
            board[x][y]=col;
            board[x+1][y]=col;
            board[x+2][y]=col;
            break;
    };
}


short int random_shape(void)
{
    short int temp;

    temp=rnd(7)+1;
    switch(temp)
    {
        case 1:
            break;

        case 2:
            break;

        case 3:
            temp=4;
            break;

        case 4:
            temp=6;
            break;

        case 5:
            temp=10;
            break;

        case 6:
            temp=14;
            break;

        case 7:
            temp=18;
            break;
    };
    return temp;
}


short int rotate(short int shape)
{
    short int oldpos;

    oldpos=shape;
    shape++;
    switch(oldpos)
    {
        case 1:
            shape=1;
            break;

        case 3:
            shape=2;
            break;

        case 5:
            shape=4;
            break;

        case 9:
            shape=6;
            break;

        case 13:
            shape=10;
            break;

        case 17:
            shape=14;
            break;

        case 19:
            shape=18;
            break;
    };
    return shape;
}


void update_score(long int score,int lines,int level)
{
    unsigned char temp[21];
    short int length;

    _settextcolor(15);
    sprintf(temp,"%ld",score);
    length=strlen(temp);
    _settextposition(12,(33-length));
    _outtext(temp);
    sprintf(temp,"%d",lines);
    length=strlen(temp);
    _settextposition(14,(29-length));
    _outtext(temp);
    sprintf(temp,"%d",level);
    length=strlen(temp);
    _settextposition(14,(37-length));
    _outtext(temp);
}


unsigned char inkey(void)
{
    unsigned char temp;

    if (!_kbhit()) return 0;
    else
    {
        temp=_getch();
        if (temp==0) temp=(_getch())<<1;
        return toupper(temp);
    };
}


void set_next(short int shape,short int col)
{
    short int y=24;

    _settextposition(19,23);
    _outtext("    ");
    _settextposition(20,23);
    _outtext("    ");
    switch(shape)
    {
        case 2:
            y=23;
            break;

        case 4:
            y=23;
            break;
    };
    piece(19,y,col,shape);
}


short int collide(short int x,short int y,short int shape,short int board[22][12])
{
    x=x-2;
    y=y-4;
    switch(shape)
    {
        case 1:
            if(board[x][y]) return FALSE;
            if(board[x][y+1]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            break;

        case 2:
            if(board[x][y+1]) return FALSE;
            if(board[x][y+2]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            break;

        case 3:
            if(board[x][y]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            if(board[x+2][y+1]) return FALSE;
            break;

        case 4:
            if(board[x][y]) return FALSE;
            if(board[x][y+1]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            if(board[x+1][y+2]) return FALSE;
            break;

        case 5:
            if(board[x][y+1]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            if(board[x+2][y]) return FALSE;
            break;

        case 6:
            if(board[x][y-1]) return FALSE;
            if(board[x][y]) return FALSE;
            if(board[x][y+1]) return FALSE;
            if(board[x+1][y]) return FALSE;
            break;

        case 7:
            if(board[x][y]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            if(board[x+2][y]) return FALSE;
            break;

        case 8:
            if(board[x][y]) return FALSE;
            if(board[x+1][y-1]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            break;

        case 9:
            if(board[x][y]) return FALSE;
            if(board[x+1][y-1]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+2][y]) return FALSE;
            break;

        case 10:
            if(board[x][y-1]) return FALSE;
            if(board[x][y]) return FALSE;
            if(board[x][y+1]) return FALSE;
            if(board[x+1][y-1]) return FALSE;
            break;

        case 11:
            if(board[x][y]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+2][y]) return FALSE;
            if(board[x+2][y+1]) return FALSE;
            break;

        case 12:
            if(board[x][y+1]) return FALSE;
            if(board[x+1][y-1]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            break;

        case 13:
            if(board[x][y]) return FALSE;
            if(board[x][y+1]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            if(board[x+2][y+1]) return FALSE;
            break;

        case 14:
            if(board[x][y-1]) return FALSE;
            if(board[x][y]) return FALSE;
            if(board[x][y+1]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            break;

        case 15:
            if(board[x][y]) return FALSE;
            if(board[x][y+1]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+2][y]) return FALSE;
            break;

        case 16:
            if(board[x][y-1]) return FALSE;
            if(board[x+1][y-1]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            break;

        case 17:
            if(board[x][y+1]) return FALSE;
            if(board[x+1][y+1]) return FALSE;
            if(board[x+2][y]) return FALSE;
            if(board[x+2][y+1]) return FALSE;
            break;

        case 18:
            if(board[x][y-1]) return FALSE;
            if(board[x][y]) return FALSE;
            if(board[x][y+1]) return FALSE;
            if(board[x][y+2]) return FALSE;
            break;

        case 19:
            if(board[x-1][y]) return FALSE;
            if(board[x][y]) return FALSE;
            if(board[x+1][y]) return FALSE;
            if(board[x+2][y]) return FALSE;
            break;
    };
    return TRUE;
}


long int calc_score(int level,short int zapped)
{
    long int temp;

    level++;
    switch(zapped)
    {
        case 1:
            temp=40;
            break;

        case 2:
            temp=100;
            break;

        case 3:
            temp=300;
            break;

        case 4:
            temp=1200;
            break;
    };
    temp=temp*level;
    return temp;
}


short int low_line(short int x,short int shape)
{
    if(shape==18) return x;
    x++;
    if(((shape/2)*2)!=shape && shape!=1) x++;
    return x;
}


short int full_line(short int line_num,short int board[22][12])
{
    short int count;

    if(line_num<=0) return FALSE;
    for (count=1;count<11;count++)
    {
        if(board[line_num][count]==0) return FALSE;
    };
    return TRUE;
}


void flash_lines(short int dead_line[4],short int zappo)
{
    short int count,repeat;

    _settextcolor(15);

    for(repeat=3;repeat>0;repeat--)
    {
        for(count=0;count<zappo;count++)
        {
            _settextposition(dead_line[count]+2,5);
            _outtext("€€€€€€€€€€");
        };
        pause(100);
        for(count=0;count<zappo;count++)
        {
            _settextposition(dead_line[count]+2,5);
            _outtext("          ");
        };
        pause(100);
    };

    /* DON'T FORGET TO ADD 2 TO EACH NUMBER TO GET SCREEN VERSION */
}


void redraw_board(short int lowest,short int board[22][12])
{
    short int line,count;
    short int x=3,y=5,ex=1,wy=1;

    for(line=lowest;line>=1;line--)
    {
        for(count=1;count<11;count++)
        {
            _settextposition(x,y);
            _settextcolor(board[ex][wy]);
            _outtext("€");
            y++;
            wy++;
        };
        wy=1;
        y=5;
        ex++;
        x++;
    };
}


void update_board(short int lowest,short int zappo,short int dead_line[4],short int board[22][12])
{
    short int count,shuffle,repeat;

    for(count=(zappo-1);count>=0;count--)
    {
        for(shuffle=dead_line[count];shuffle>1;shuffle--)
        {
            for(repeat=1;repeat<11;repeat++)
            {
                board[shuffle][repeat]=board[shuffle-1][repeat];
            };
        };
        for(repeat=1;repeat<11;repeat++) board[1][repeat]=0;
    };

    redraw_board(lowest,board);
}


short int zapped_lines(short int x,short int shape,short int board[22][12])
{
    short int lowest,count,zappo;
    short int dead_line[4]={0,0,0,0};

    x=x-2;

    lowest=low_line(x,shape);
    zappo=0;
    for (count=0;count<4;count++)
    {
        if(full_line((lowest-count),board))
        {
            dead_line[zappo]=(lowest-count);
            zappo++;
        }
    };
    if(zappo)
    {
        flash_lines(dead_line,zappo);
        update_board(lowest,zappo,dead_line,board);
    };
    return zappo;
}


unsigned int levpause(int level)
{
    unsigned int values[15]={600,550,500,450,400,350,300,250,200,150,100,50,25,10};

    if(level<14) return values[level];
    else return 5;
}


long int game(int level)
{
    long int score=0;
    unsigned int dropwait;
    clock_t goal;
    int lines=0;
    short int board[22][12];
    short int next_shape,now_shape,old_shape;
    short int next_col,now_col;
    short int x,y,zap;
    int next_lev;
    short int dead=FALSE;
    short int landed=FALSE;
    unsigned char hitkey;
    short int dropnow;

    next_lev=10*(level+1);
    next_col=rnd(15)+1;
    next_shape=random_shape();

    _clearscreen(_GCLEARSCREEN);
    clr_board(board);
    backdrop();
    dropwait=levpause(level);
    
    while (!dead)
    {
        update_score(score,lines,level);
        now_col=next_col;
        now_shape=next_shape;
        next_col=rnd(15)+1;
        next_shape=random_shape();
        set_next(next_shape,next_col);
        x=3;
        y=9;
        zap=0;
        hitkey=0;
        piece(x,y,now_col,now_shape);
        landed=FALSE;
        dropnow=FALSE;

        while(!landed)
        {
            hitkey=0;
            goal=((time_t) dropwait)+clock();
            while(goal>clock() && !dropnow)
            {
                hitkey=inkey();
                if(hitkey)
                {
                    if(hitkey==left)
                    {
                        y--;
                        if(collide(x,y,now_shape,board))
                        {
                            piece(x,(y+1),0,now_shape);
                            piece(x,y,now_col,now_shape);
                        }
                        else y++;
                    }
                    else
                    if(hitkey==right)
                    {
                        y++;
                        if(collide(x,y,now_shape,board))
                        {
                            piece(x,(y-1),0,now_shape);
                            piece(x,y,now_col,now_shape);
                        }
                        else y--;
                    }
                    else
                    if(hitkey==rot)
                    {
                        old_shape=now_shape;
                        now_shape=rotate(now_shape);
                        if(collide(x,y,now_shape,board))
                        {
                            piece(x,y,0,old_shape);
                            piece(x,y,now_col,now_shape);
                        }
                        else now_shape=old_shape;
                    }
                    else
                    if(hitkey==drop)
                    {
                        score++;
                        dropnow=TRUE;
                    }
                    else
                    if(hitkey==ESCAPE)
                        return score;
                    else
                        hitkey=0;
                };
            };
            x++;
            dropnow=FALSE;

            if(!collide(x,y,now_shape,board))
            {
                x--;
                landed=TRUE;
                score=score+2;
                land_piece(x,y,now_col,now_shape,board);
                if(x<=3)
                {
                    dead=TRUE;
                };
                if(!dead)
                {
                    zap=zapped_lines(x,now_shape,board);   /* DO COMPLETE LINE CHECK */
                    lines=lines+zap;
                    if(zap) score=score+calc_score(level,zap);
                    if(lines>=next_lev)
                    {
                        score=score+(level*100);
                        level++;
                        dropwait=levpause(level);
                        next_lev=next_lev+10;
                    };
                };
            };

            if (!landed)
            {
                piece((x-1),y,0,now_shape);
                piece(x,y,now_col,now_shape);
            };
            hitkey=0;
        };
    };

    return score;
}
