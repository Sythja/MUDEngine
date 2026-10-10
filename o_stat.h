/**************************************************************************/
// o_stat.h - written by Daos (weapon/armor autobalance)
/**************************************************************************/
/****************************************************************************
 * The Darkening Sun ?2006 Marc Garrett                              *
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
#ifndef O_STAT_H
#define O_STAT_H

#define DICE_ROLLS (0)
#define DICE_SIDES (1)

#define O_STAT_ABOVE_MAX_LEVEL 50

struct obj_diceset_type
{
   int dice[3];
};

class o_stat
{
   public:
      void load_o_stat(const char *filename);
      void save_o_stat(const char *filename);

      // return false if there is an error
      bool apply_objstat(int level, int *dice_rolls, int *dice_sides);
      bool apply_objstat(int level, int *fixed_value);
      bool apply_objstat(int level, short *dice_rolls, short *dice_sides);
      bool apply_objstat(int level, short *fixed_value);
   private:
      bool bounds_error(int level, bool fixed);
      obj_diceset_type lev[MAX_LEVEL+O_STAT_ABOVE_MAX_LEVEL];
      bool valid;
};

#endif // O_STAT_H
/**************************************************************************/

