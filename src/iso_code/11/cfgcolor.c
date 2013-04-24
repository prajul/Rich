


#include <bios.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tdecfg.h"
#include "cfgcolor.h"



COLORS colour = {
   "$colors",
   { { HERC_REVERSE, HERC_NORMAL, HERC_UNDER, HERC_REVERSE, HERC_REVERSE,
     HERC_HIGH, HERC_NORMAL, HERC_NORMAL, HERC_HIGH, HERC_HIGH, HERC_HIGH,
     HERC_REVERSE, HERC_REVERSE, HERC_NORMAL },
   { COLOR_HEAD, COLOR_TEXT, COLOR_DIRTY, COLOR_MODE, COLOR_BLOCK,
     COLOR_MESSAGE, COLOR_HELP, COLOR_WRAP, COLOR_EOF, COLOR_CURL, COLOR_RULER,
     COLOR_POINTER, COLOR_TEXT, COLOR_OVRS } }
};


extern struct vcfg cfg;         
extern FILE *tde_exe;           
extern long color_offset;

COLORS temp_colours;            
static int index;               



void tdecolor( void )
{
   initialize_color( );
   show_init_sample( );
   change_colors( );
   restore_overscan( cfg.overscan );
}



void initialize_color( void )
{

   fseek( tde_exe, color_offset, SEEK_SET );
   fread( (void *)&temp_colours, sizeof( COLORS ), 1, tde_exe );

   if (cfg.color == FALSE)
      index = 0;
   else
      index = 1;

   fields[0].color = temp_colours.clr[index][HELP];
   fields[1].color = temp_colours.clr[index][HEAD];
   fields[2].color = temp_colours.clr[index][TEXT];
   fields[3].color = temp_colours.clr[index][CURL];
   fields[4].color = temp_colours.clr[index][DIRTY];
   fields[5].color = temp_colours.clr[index][WARNING];
   fields[6].color = temp_colours.clr[index][MODE];
   fields[7].color = temp_colours.clr[index][WRAP];
   fields[8].color = temp_colours.clr[index][CEOF];
   fields[9].color = temp_colours.clr[index][OVERSCAN];
   fields[10].color = temp_colours.clr[index][RULER];
   fields[11].color = temp_colours.clr[index][RULER_PTR];
   fields[12].color = temp_colours.clr[index][BLOCK];
   fields[13].color = temp_colours.clr[index][HILITED_FILE];

   fields[0].show_me = show_help_color;
   fields[1].show_me = show_fileheader_color;
   fields[2].show_me = show_text_color;
   fields[3].show_me = show_curl_color;
   fields[4].show_me = show_dirty_color;
   fields[5].show_me = show_warning_color;
   fields[6].show_me = show_mode_color;
   fields[7].show_me = show_wrapped_color;
   fields[8].show_me = show_eof_color;
   fields[9].show_me = show_overscan_color;
   fields[10].show_me = show_ruler_color;
   fields[11].show_me = show_rulerptr_color;
   fields[12].show_me = show_block_color;
   fields[13].show_me = show_hilitedfile_color;
}



void restore_overscan( int overscan )
{
   ASSEMBLE {
        mov     ah, 0x0b                
        mov     bl, BYTE PTR overscan   
        xor     bh, bh
        push    bp
        int     VIDEO_INT               
        pop     bp
   }
}


void show_init_sample( void )
{
char *sample;
int  line;
int  i;
int  j;
int  k;
int  l;
char temp[6];
char far *p;

   xygoto( -1, -1 );
   sample = sample_screen[0];
   for (line=0; sample != NULL; ) {
      s_output( (char far *)sample, line, 0, 7 );
      sample = sample_screen[++line];
   }
   for (i=0; i<NUM_COLORS; i++)
      (*fields[i].show_me)();
   sample = field_screen[0];
   for (line=12, i=1; sample != NULL; line++,i++) {
      s_output( (char far *)sample, line, 0, 7 );
      sample = field_screen[i];
   }
   p = (char far *)temp;
   for (i=0,k=0,line=17; i<8; i++, line++) {
      for (j=0,l=0; j<16; j++, k++,l+=5) {
         color_number( temp, k );
         s_output( p, line, l, k );
      }
   }
   for (i=0; i<NUM_COLORS; i++) {
      color_number( temp, fields[i].color );
      s_output( p, fields[i].line, fields[i].col, fields[i].color );
   }
}



void color_number( char *dest, int num )
{
int i, j, k;
char temp[6];

   strcpy( dest, "[   ]" );
   itoa( num, temp, 10 );
   i = strlen( temp );
   j = 4 - i;
   for (k=0; i > 0; i--,j++, k++)
      dest[j] = temp[k];
}



void current_color_number( char *dest, int num )
{
int i, j, k;
char temp[6];

   strcpy( dest, "*   *" );
   itoa( num, temp, 10 );
   i = strlen( temp );
   j = 4 - i;
   for (k=0; i > 0; i--,j++, k++)
      dest[j] = temp[k];
}



void show_help_color( void )
{
int color;
int line;

   color = fields[0].color;
   for (line=1; line <10; line++)
      hlight_line( 1, line, 37, color );
   hlight_line( 1,  10, 13, color );
   hlight_line( 25, 10, 13, color );
}



void show_fileheader_color( void )
{
   hlight_line( 41, 1, 38, fields[1].color );
}



void show_text_color( void )
{
int color;

   color = fields[2].color;
   hlight_line( 41, 3, 38, color );

}



void show_curl_color( void )
{
   hlight_line( 41, 4, 38, fields[3].color );
}



void show_dirty_color( void )
{
   hlight_line( 41, 5, 38, fields[4].color );
}


void show_warning_color( void )
{
   hlight_line( 41, 9, 38, fields[5].color );
}



void show_mode_color( void )
{
   hlight_line( 41, 10, 26, fields[6].color );
}



void show_wrapped_color( void )
{
   hlight_line( 67, 10, 12, fields[7].color );
}



void show_eof_color( void )
{
   hlight_line( 41, 8, 38, fields[8].color );
}



void show_overscan_color( void )
{
int overscan;

   overscan = fields[9].color;
   restore_overscan( overscan );
}



void show_ruler_color( void )
{
   hlight_line( 41, 2, 21, fields[10].color );
   hlight_line( 63, 2, 16, fields[10].color );
}



void show_rulerptr_color( void )
{
   hlight_line( 62, 2, 1, fields[11].color );
}



void show_block_color( void )
{
int color;
int line;

   color = fields[12].color;
   for (line=6; line <8; line++)
      hlight_line( 41, line, 38, color );
}



void show_hilitedfile_color( void )
{
   hlight_line( 14, 10, 11, fields[13].color );
}



void change_colors( void )
{
int  c;
int  area;
int  new_color;
int  i;
char temp[6];
char far *p;

   p = (char far *)temp;
   area = 0;
   current_color_number( temp, fields[area].color );
   s_output( p, fields[area].line, fields[area].col, fields[area].color );
   xygoto( fields[area].col+3, fields[area].line );
   for (c=0; c != F3  &&  c != F10  &&  c != ESC;) {
      new_color = FALSE;
      c = getkey( );
      switch (c) {
         case RTURN :
         case DOWN  :
            color_number( temp, fields[area].color );
            s_output( p, fields[area].line, fields[area].col, fields[area].color );
            ++area;
            if (area > 13)
               area = 0;
            current_color_number( temp, fields[area].color );
            s_output( p, fields[area].line, fields[area].col, fields[area].color );
            xygoto( fields[area].col+3, fields[area].line );
            break;
         case UP    :
            color_number( temp, fields[area].color );
            s_output( p, fields[area].line, fields[area].col, fields[area].color );
            --area;
            if (area < 0)
               area = 13;
            current_color_number( temp, fields[area].color );
            s_output( p, fields[area].line, fields[area].col, fields[area].color );
            xygoto( fields[area].col+3, fields[area].line );
            break;
         case LEFT :
            --fields[area].color;
            if (area == 9) {
               if (fields[area].color < 0)
                  fields[area].color = 15;
            } else {
               if (fields[area].color < 0)
                  fields[area].color = 127;
            }
            new_color = TRUE;
            break;
         case RIGHT :
            ++fields[area].color;
            if (area == 9) {
               if (fields[area].color > 15)
                  fields[area].color = 0;
            } else {
               if (fields[area].color > 127)
                  fields[area].color = 0;
            }
            new_color = TRUE;
            break;
         case PGUP :
            if (area == 9)
               break;
            fields[area].color -= 16;
            if (fields[area].color < 0)
               fields[area].color = (fields[area].color & 0x000f) + 0x70;
            new_color = TRUE;
            break;
         case PGDN :
            if (area == 9)
               break;
            fields[area].color += 16;
            if (fields[area].color > 127)
               fields[area].color = fields[area].color & 0x000f;
            new_color = TRUE;
            break;
         case F2 :

           
            fields[0].color = colour.clr[index][HELP];
            fields[1].color = colour.clr[index][HEAD];
            fields[2].color = colour.clr[index][TEXT];
            fields[3].color = colour.clr[index][CURL];
            fields[4].color = colour.clr[index][DIRTY];
            fields[5].color = colour.clr[index][WARNING];
            fields[6].color = colour.clr[index][MODE];
            fields[7].color = colour.clr[index][WRAP];
            fields[8].color = colour.clr[index][CEOF];
            fields[9].color = colour.clr[index][OVERSCAN];
            fields[10].color = colour.clr[index][RULER];
            fields[11].color = colour.clr[index][RULER_PTR];
            fields[12].color = colour.clr[index][BLOCK];
            fields[13].color = colour.clr[index][HILITED_FILE];
            for (i=0; i<NUM_COLORS; i++) {
               color_number( temp, fields[i].color );
               s_output( p, fields[i].line, fields[i].col, fields[i].color );
               (*fields[i].show_me)();
            }
            current_color_number( temp, fields[area].color );
            s_output( p, fields[area].line, fields[area].col, fields[area].color );
            break;
      }
      if (new_color) {
         current_color_number( temp, fields[area].color );
         s_output( p, fields[area].line, fields[area].col, fields[area].color );
         (*fields[area].show_me)();
      }
   }

   if (c == F10) {
      temp_colours.clr[index][HELP]         = fields[0].color;
      temp_colours.clr[index][HEAD]         = fields[1].color;
      temp_colours.clr[index][TEXT]         = fields[2].color;
      temp_colours.clr[index][CURL]         = fields[3].color;
      temp_colours.clr[index][DIRTY]        = fields[4].color;
      temp_colours.clr[index][WARNING]      = fields[5].color;
      temp_colours.clr[index][MODE]         = fields[6].color;
      temp_colours.clr[index][WRAP]         = fields[7].color;
      temp_colours.clr[index][CEOF]         = fields[8].color;
      temp_colours.clr[index][OVERSCAN]     = fields[9].color;
      temp_colours.clr[index][RULER]        = fields[10].color;
      temp_colours.clr[index][RULER_PTR]    = fields[11].color;
      temp_colours.clr[index][BLOCK]        = fields[12].color;
      temp_colours.clr[index][HILITED_FILE] = fields[13].color;
      fseek( tde_exe, color_offset, SEEK_SET );
      fwrite( (void *)&temp_colours, sizeof( COLORS ), 1, tde_exe );
   }
   cls( );
}
