/**************************************************************************/
// o_stat.cpp - written by Daos (weapon/armor autobalance)
/**************************************************************************/
/****************************************************************************
 * The Darkening Sun ©2006 Marc Garrett                              *
 * >> The Darkening Sun is a derivative from the Dawn parent codebase      *
 * written by Michael Garratt.  This is one of the many source files that   *
 * has been included into the Dawn/DS master source.  This file may not be  *
 * used by any other MUD unless it is part of an official Dawn public       *
 * release by Michael Garratt.  This copyright notice may not be removed.   *
 ***************************************************************************/
/***************************************************************************
 * The Dawn of Time v1.69r (c)1997-2004 Michael Garratt                    *
 * >> A number of people have contributed to the Dawn codebase, with the   *
 *    majority of code written by Michael Garratt - www.dawnoftime.org     *
 * >> To use this source code, you must fully comply with the dawn license *
 *    in licenses.txt... In particular, you may not remove this copyright  *
 *    notice.                                                              *
 **************************************************************************/

#include "include.h"
#include "o_stat.h"

/****************************************
* File format:                     *   
* -1 = EOF marker                  *
* -2 = Comment to EOL               *
****************************************/

/****************************************
* File format:                     *
* Level X Y                      *
* Level = X d Y                     *
* Level X -1 0                     *
* = fixed at X for level            *
****************************************/

/**************************************************************************/
void o_stat::load_o_stat(const char *filename)
{
   FILE *fp=fopen( filename,"r");
   if(!fp)
   {
      valid=false;
      bugf("o_stat::load_o_stat(): Couldn't open file '%s' to read in o_stat from!",
         filename);
      return;
   }

   logf("o_stat::load_o_stat(): Loading in o_stat from %s...",
      filename);
   
   memset(&lev[0], 0, sizeof(obj_diceset_type)* (MAX_LEVEL+O_STAT_ABOVE_MAX_LEVEL));

   int level=0;

   while(!feof(fp))
   {
      level=fread_number(fp);

      if(level==-1)
      {
         break;
      }

      if(level==-2)
      {
         fread_to_eol(fp);
         continue;
      }

      if(level<0 || level>MAX_LEVEL+O_STAT_ABOVE_MAX_LEVEL)
      {
         bugf("o_stat::load_o_stat(): value %d read in from file "
            "- not within level valid range... aborting mud!",
            level);
         do_abort();
      }

      lev[level].dice[DICE_ROLLS]=fread_number(fp);
      lev[level].dice[DICE_SIDES]=fread_number(fp);
      fread_to_eol(fp);
   }
   if(feof(fp) && level!=-1)
   {
      logf("o_stat::load_o_stat(): load incomplete, aborting mud!");
      do_abort();
   }

   valid=true;

   logf("o_stat::load_o_stat(): Load complete.");
   fclose(fp);
}
/**************************************************************************/
void o_stat::save_o_stat(const char *filename)
{
   if(!valid)
   {
      bugf("o_stat::save_o_stat('%s'): current o_stat set is NULL!",
         filename);
      do_abort();
      return;
   }

   logf("o_stat::save_o_stat(): Saving in o_stat to %s...",
      filename);

   FILE *fp=fopen( filename,"w");

   if(!fp)
   {
      bugf("o_stat::save_o_stat(): Couldn't open file '%s' to save o_stat to!",
         filename);
      return;
   }

   fprintf(fp,"-2 Level  Dice_rolls  Dice_sides\n");

   for(int level=0; level<MAX_LEVEL+O_STAT_ABOVE_MAX_LEVEL; level++)
   {
      if(lev[level].dice[DICE_ROLLS]
         || lev[level].dice[DICE_SIDES]==-1)
      {
         fprintf(fp,"%d %d %d\n",
            level,
            lev[level].dice[DICE_ROLLS],
            lev[level].dice[DICE_SIDES]);
      }
   }
   fprintf(fp,"-1\n");

   fclose(fp);

   logf("o_stat::save_o_stat(): Saving complete.");

}
/**************************************************************************/
bool o_stat::bounds_error(int level, bool fixed)
{
   if(!valid)
   {
      bug("o_stat::bounds_error(): invalid node!");
      return true;
   }

   if(level<0 || level>=MAX_LEVEL+O_STAT_ABOVE_MAX_LEVEL)
   {
      bugf("o_stat::bounds_error(): level %d out of range!",
         level);
      return true;
   }

   if(lev[level].dice[DICE_SIDES]==-1)
   {
      if(!fixed)
      {
         bugf("o_stat::bounds_error(): fixed value requested,"
            "not fixed entry type (level %d)",
            level);
         return true;
      }
   }
   else
   {
      if(fixed)
      {
         bugf("o_stat::bounds_error(): dice value requested,"
            "not dice entry type (level %d)",
            level);
         return true;
      }
   }

   if(lev[level].dice[DICE_ROLLS]==0 && lev[level].dice[DICE_SIDES]==0)
   {
      bugf("o_stat::bounds_error(): no o_stat value for level %d.",
         level);
      return true;
   }

   return false;
};
/**************************************************************************/
bool o_stat::apply_objstat(int level, int *dice_rolls, int *dice_sides)
{
   if(bounds_error(level, false))
   {
      return false;
   }

   *dice_rolls=lev[level].dice[DICE_ROLLS];
   *dice_sides=lev[level].dice[DICE_SIDES];
   return true;
};
/**************************************************************************/
bool o_stat::apply_objstat(int level, int *fixed_value)
{
   if(bounds_error(level, true))
   {
      return false;
   }

   *fixed_value=lev[level].dice[DICE_ROLLS];
   return true;
};

/**************************************************************************/
bool o_stat::apply_objstat(int level, short *dice_rolls, short *dice_sides)
{
   if(bounds_error(level, false))
   {
      return false;
   }

   *dice_rolls=(short)lev[level].dice[DICE_ROLLS];
   *dice_sides=(short)lev[level].dice[DICE_SIDES];
   return true;
};
/**************************************************************************/
bool o_stat::apply_objstat(int level, short *fixed_value)
{
   if(bounds_error(level, true))
   {
      return false;
   }

   *fixed_value=(short)lev[level].dice[DICE_ROLLS];
   return true;
};
/**************************************************************************/
/**************************************************************************/	
