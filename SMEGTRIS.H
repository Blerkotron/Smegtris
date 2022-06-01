/* Header File for Smegtris version 1.0 */
/* Written by the infamous BLOOD */
/* (C)1994 Cheese Freak Software */
/* This is public domain! */


/* ========================================================================= */
/* | Lots of weird definitions.......                                      | */
/* ========================================================================= */
#define START_SCORES {100000,95000,90000,85000,80000,75000,70000,65000,60000,55000,50000,45000,40000,35000,30000,25000,20000,15000,10000,5000}
#define START_NAMES {"BLOOD","CHEESE","LISTER","KRYTEN","CAT","RIMMER","HOLLY","FAITH NO MORE","RAGE AGAINST","THE MACHINE","RED HOT","CHILI PEPPERS","CARTER USM","NIRVANA","LEVELLERS","THERAPY?","MINISTRY","MORDRED","PEARL JAM","ALICE IN CHAINS"}
#define TRUE 1
#define FALSE 0

#define LEFT 150
#define RIGHT 154
#define TAB 9
#define ENTER 13
#define ESCAPE 27
#define SPACE 32


/* ========================================================================= */
/* | Yikes alordy, if it isn't a bloody structure declaration...           | */
/* ========================================================================= */
typedef struct {
    unsigned char val;
    unsigned char name[10];
} KEY;


/* ========================================================================= */
/* | The function prototypes (this is ANSI C, after all!).......           | */
/* ========================================================================= */
void savescores(long int hisc[20],unsigned char hiname[20][21]);
void loadscores(long int hisc[20],unsigned char hiname[20][21]);
void clrbuf(void);
void setrate(void);
void restrate(void);
void redefkeys(void);
void pause(unsigned int wait);
void centre(int colour,int x,unsigned char *text);
void menu(void);
void init(void);
int rnd(int x);
int get_lev(void);
void display_hi(long int hisc[20],unsigned char hiname[20][21]);
void game_over(long int score);
void smiley(void);
void new_high(long int score,long int hisc[20],unsigned char hiname[20][21]);
void display_credits(void);
void get_name(unsigned char newname[21]);
int hipos(long int score,long int hisc[20]);
void backdrop(void);
void clr_board(short int board[22][12]);
void piece(short int x,short int y,short int col,short int shape);
void land_piece(short int x,short int y,short int col,short int shape,short int board[22][12]);
short int random_shape(void);
short int rotate(short int shape);
void update_score(long int score,int lines,int level);
unsigned char inkey(void);
void set_next(short int shape,short int col);
short int collide(short int x,short int y,short int shape,short int board[22][12]);
long int calc_score(int level,short int zapped);
short int low_line(short int x,short int shape);
short int full_line(short int line_num,short int board[22][12]);
void flash_lines(short int dead_line[4],short int zappo);
void redraw_board(short int lowest,short int board[22][12]);
void update_board(short int lowest,short int zappo,short int dead_line[4],short int board[22][12]);
short int zapped_lines(short int x,short int shape,short int board[22][12]);
long int game(int level);

