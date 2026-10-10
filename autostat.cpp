/**************************************************************************/
// autostat.cpp - auto stats setting system - Kal, May 2000
//            - additional weapon/armor autostat code by Daos, Nov. 2003
/***************************************************************************
 * The Dawn of Time v1.69r (c)1997-2004 Michael Garratt                    *
 * >> A number of people have contributed to the Dawn codebase, with the   *
 *    majority of code written by Michael Garratt - www.dawnoftime.org     *
 * >> To use this source code, you must fully comply with the dawn license *
 *    in licenses.txt... In particular, you may not remove this copyright  *
 *    notice.                                                              *
 **************************************************************************/
#include "include.h"
#include "olc.h"
#include "statset.h"
#include "o_stat.h"

statset mob_hitpoints;
statset mob_damroll;
statset mob_ac;
statset mob_mana;
statset mob_psi;
o_stat obj_dam;
/**************************************************************************/
// Save statsets to disk,
// - just used for reformatting the files nicely at this stage
void save_autostat_files()
{
   mob_hitpoints.save_statset(AUTOSTAT_DIR"mob_hitp.sav");
   mob_damroll.save_statset(AUTOSTAT_DIR"mob_damr.sav");
   mob_mana.save_statset(AUTOSTAT_DIR"mob_mana.sav");
   mob_psi.save_statset(AUTOSTAT_DIR"mob_psi.sav");
   mob_ac.save_statset(AUTOSTAT_DIR"mob_ac.sav");
   obj_dam.save_o_stat(AUTOSTAT_DIR"obj_dam.sav");
}

/**************************************************************************/
void do_save_autostat_files(char_data*ch, char*)
{
   save_autostat_files();
   ch->printf("Stat files saved to *.sav");

}
/**************************************************************************/
void load_autostat_files()
{
   logf("Starting: load_autostat_files()...");
   mob_hitpoints.load_statset(AUTOSTAT_DIR"mob_hitp.txt");
   mob_damroll.load_statset(AUTOSTAT_DIR"mob_damr.txt");
   mob_mana.load_statset(AUTOSTAT_DIR"mob_mana.txt");
   mob_psi.load_statset(AUTOSTAT_DIR"mob_psi.txt");
   mob_ac.load_statset(AUTOSTAT_DIR"mob_ac.txt");
   obj_dam.load_o_stat(AUTOSTAT_DIR"obj_dam.txt");
   logf("Finished: load_autostat_files().");
}
/**************************************************************************/
void do_load_autostat_files(char_data*ch, char*)
{
   load_autostat_files();
   ch->printf("AutoStat files reloaded from *.txt");
}
/**************************************************************************/
// return false for bad input.
// syntax <number> - straight number
// syntax +<number> - base number + amount
// syntax -<number> - base number - amount
bool find_flexivalue(char_data *ch, char*argument, int *result,
                int base_value, int min_value, int max_value)
{
   char arg[MIL];
   one_argument(argument, arg);
   int possible_result;

   if(IS_NULLSTR(arg)){
      // no input, just use base value
      *result=base_value;
      return true;
   }

   if(!is_number(arg)){
      ch->println("flexivalue(): Input must be numeric");
      return false;
   }

   possible_result=base_value;

   if(arg[0]=='+'){
      // addition modifier
      possible_result+=atoi(arg+1);
   }else if(arg[0]=='-'){
      // subtraction modifier
      possible_result-=atoi(arg+1);
   }else{
      possible_result=atoi(arg);
   }

   // bounds check the result
   if(possible_result<min_value || possible_result>max_value ){
      ch->printlnf("flexivalue(): The input value must be between %d and %d, %d is not.",
         min_value, max_value, possible_result);
      return false;
   }

   *result=possible_result;
   return true;
}
/**************************************************************************/
// this way we dont need a descriptor on a character
/*
*Thief mobiles should read their hp, ac, and damage at one level lower.
*Mage mobiles read hp and ac at one level lower, and damage three levels lower.
*Cleric mobiles read damage at two levels lower.
*Warrior mobiles read hit points one level higher.
*Armor class vs. magical attacks should be 80-90% of the other AC's.

An average of 100 + 10xlevel for the mana value of mobs.
This is highly susceptive to change in accordance to the type of mob.
Mage mobs to have the above recommended values.
Warrior types to have 50%.
Barbarian types (hatred to magic type mobs) to have as low as even 10%.
*/
bool autostat_mindex(char_data* ch, char* argument, mob_index_data *mi)
{
   char buf[MSL];
   int level;

   if(!IS_NULLSTR(argument) && argument[0]=='?'){
      ch->println("medit_autostat -  syntax: autostat <flexivalue_level>");
      ch->printlnf("Flexivalue is the term %s uses for is a value that can be absolute or relevative.",
         MUD_NAME);
      ch->println("autostat      - sets the stats for the mobs current level.");
      ch->println("autostat 10   - sets the stats as if the mob was level 10.");
      ch->println("autostat +10  - sets the stats as if the mob was 10 levels higher than its current level.");
      ch->println("autostat -10  - sets the stats as if the mob was 10 levels lower than its current level.");
      return false;
   }
   
   if(!find_flexivalue(ch, argument, &level, mi->level, 0, MAX_LEVEL+STATSET_ABOVE_MAX_LEVEL-1)){
      return false;
   };
   
   if(level==mi->level){
      ch->printlnf("\r\nAuto setting stats for mob[%d] level %d", mi->vnum, level);
   }else{
      ch->printlnf("\r\nAuto setting stats for %slevel %d`x, (mob[%d] level is %d).",
         level>mi->level?"`G":"`R",
         level,
         mi->vnum,
         mi->level);
   }

   // hitpoints
   sprintf(buf,"%dd%d+%d",
      mi->hit[DICE_ROLLS],
      mi->hit[DICE_SIDES],
      mi->hit[DICE_BONUS]);

   int hlev=level;
   if(IS_SET(mi->act, ACT_WARRIOR)){
      ch->println("Warrior Act Flag - Hitpoints Level +1");
      hlev+=1;
   }
   if(IS_SET(mi->act, ACT_THIEF)){
      ch->println("Thief Act Flag - Hitpoints Level -1");
      hlev-=1;
   }
   if(IS_SET(mi->act, ACT_MAGE)){
      ch->println("Mage Act Flag - Hitpoints Level -1");
      hlev-=1;
   }
   
   if(mob_hitpoints.apply_autostat(hlev, &mi->hit[DICE_ROLLS],
      &mi->hit[DICE_SIDES], &mi->hit[DICE_BONUS]))
   {
      ch->printlnf("Hitpoints changed from %s to %dd%d+%d",
         buf,
         mi->hit[DICE_ROLLS],
         mi->hit[DICE_SIDES],
         mi->hit[DICE_BONUS]);
   }else{
      ch->printlnf("`RHitpoints unchanged from %s - most likely due to no hitpoints settings for level %d?`x",
         buf, hlev);
   }


   // mana
   sprintf(buf,"%dd%d+%d",
      mi->mana[DICE_ROLLS],
      mi->mana[DICE_SIDES],
      mi->mana[DICE_BONUS]);


   int mlev=level;
   if(IS_SET(mi->act, ACT_WARRIOR)){
      ch->println("Warrior Act Flag - Mana Level Quartered");
      mlev/=4;
   }
   if(IS_SET(mi->act, ACT_THIEF)){
      ch->println("Thief Act Flag - Mana Level Halved");
      mlev/=2;
   }
   if(IS_SET(mi->act, ACT_MAGE)){
      ch->println("Mage Act Flag - Mana Level Boosted by 10");
      mlev+=10;
   }
   if(mob_mana.apply_autostat(mlev, &mi->mana[DICE_ROLLS],
      &mi->mana[DICE_SIDES], &mi->mana[DICE_BONUS]))
   {
      ch->printlnf("Mana dice changed from %s to %dd%d+%d",
         buf,
         mi->mana[DICE_ROLLS],
         mi->mana[DICE_SIDES],
         mi->mana[DICE_BONUS]);
   }else{
      ch->printlnf("`RManapoints unchanged from %s - most likely due to no manapoints settings for level %d?`x",
         buf, mlev);
   }

	// psi
   sprintf(buf,"%dd%d+%d",
      mi->psi[DICE_ROLLS],
      mi->psi[DICE_SIDES],
      mi->psi[DICE_BONUS]);


   int plev=level;
   if(IS_SET(mi->act, ACT_WARRIOR)){
      ch->println("Warrior Act Flag - Psi Level Quartered");
      plev/=4;
   }
   if(IS_SET(mi->act, ACT_THIEF)){
      ch->println("Thief Act Flag - Psi Level Halved");
      plev/=2;
   }
   if(IS_SET(mi->act2, ACT2_PSION)){
      ch->println("Psion Act Flag - Psi Level Boosted by 10");
      plev+=10;
   }
   if(mob_psi.apply_autostat(plev, &mi->psi[DICE_ROLLS],
      &mi->psi[DICE_SIDES], &mi->psi[DICE_BONUS]))
   {
      ch->printlnf("Psi dice changed from %s to %dd%d+%d",
         buf,
         mi->psi[DICE_ROLLS],
         mi->psi[DICE_SIDES],
         mi->psi[DICE_BONUS]);
   }else{
      ch->printlnf("`RPsipoints unchanged from %s - most likely due to no psipoints settings for level %d?`x",
         buf, mlev);
   }

   // damroll
   int dlev=level;
   if(IS_SET(mi->act, ACT_THIEF)){
      ch->println("Thief Act Flag - Damage Dice Level -1");
      dlev-=1;
   }
   if(IS_SET(mi->act, ACT_MAGE)){
      ch->println("Mage Act Flag - Hitpoints Level -3");
      dlev-=3;
   }
   if(IS_SET(mi->act, ACT_CLERIC)){
      dlev-=1;
      ch->println("Cleric Act Flag - Damage Dice Level -1");
   }
   sprintf(buf,"%dd%d+%d",
      mi->damage[DICE_ROLLS],
      mi->damage[DICE_SIDES],
      mi->damage[DICE_BONUS]);
   if(mob_damroll.apply_autostat(dlev, &mi->damage[DICE_ROLLS],
      &mi->damage[DICE_SIDES], &mi->damage[DICE_BONUS]))
   {
      ch->printlnf("Damroll changed from %s to %dd%d+%d",
         buf,
         mi->damage[DICE_ROLLS],
         mi->damage[DICE_SIDES],
         mi->damage[DICE_BONUS]);
   }else{
      ch->printlnf("`RDamroll unchanged from %s - most likely due to no damroll settings for level %d?`x",
         buf, dlev);
   }

   // ac
   int alev=level;
   if(IS_SET(mi->act, ACT_THIEF)){
      ch->println("Thief Act Flag - AC Level -1");
      alev-=1;
   }
   if(IS_SET(mi->act, ACT_MAGE)){
      ch->println("Mage Act Flag - AC Level -1");
      alev-=1;
   }
   int ac=500;
   if(mob_ac.apply_autostat(alev, &ac)){
      if(    ac==mi->ac[AC_PIERCE]
         && ac==mi->ac[AC_BASH]
         && ac==mi->ac[AC_SLASH]
         && (ac*0.9)>=mi->ac[AC_EXOTIC]
         && (ac*0.8)<=mi->ac[AC_EXOTIC])
      {
         ch->printlnf("`gAC unchanged from: %d %d %d %d - in balance for level %d`x",
            mi->ac[AC_PIERCE],
            mi->ac[AC_BASH   ],
            mi->ac[AC_SLASH],
            mi->ac[AC_EXOTIC],
            alev);
      }else{
         int new_ac_exotic=ac*number_range(80,90)/100;
         ch->printlnf("AC changed from: %d %d %d %d to %d %d %d %d",
            mi->ac[AC_PIERCE],
            mi->ac[AC_BASH   ],
            mi->ac[AC_SLASH],
            mi->ac[AC_EXOTIC],
            ac, ac, ac, new_ac_exotic);
         mi->ac[AC_PIERCE]=mi->ac[AC_BASH]=mi->ac[AC_SLASH]=ac;
         mi->ac[AC_EXOTIC]=new_ac_exotic;
      }
   }else{
      ch->printlnf("`RAC unchanged from: %d %d %d %d - most likely due to no ac settings for level %d?`x",
         mi->ac[AC_PIERCE],
         mi->ac[AC_BASH   ],
         mi->ac[AC_SLASH],
         mi->ac[AC_EXOTIC],
         alev);
   }

   if(IS_NULLSTR(argument)){
      ch->println("`xType '`=Cautostat ?`x' for help on some of the features of autostat.");
   }
   return true;
}
/**************************************************************************/
bool medit_autostat(char_data* ch, char* argument)
{
   mob_index_data *mi;

   EDIT_MOB(ch, mi);

   return autostat_mindex(ch, argument, mi);
}
/**************************************************************************/
void do_autobalance(char_data* ch, char* argument)
{
   if (!HAS_SECURITY(ch,9))
   {
      ch->println("You must have a security of 9 to use autobalance.");
      return;
   }

   char lower[MIL];
   char upper[MIL];
   char level[MIL];
   argument=one_argument(argument, lower);
   argument=one_argument(argument, upper);
   argument=one_argument(argument, level);

   if(IS_NULLSTR(level)){
      ch->println("syntax: autobalance <lowervnum> <uppervnum> <level_flexivalue>");
      ch->println("Use '+0' for flexivalue if you want it as the mobs level");
      return;
   }

   if(!is_number(lower)){
      ch->printlnf("Lower vnum must be a numeric value and '%s' is not numeric.", lower);
      return;
   }
   if(!is_number(upper)){
      ch->printlnf("Upper vnum must be a numeric value and '%s' is not numeric.", upper);
      return;
   }
   if(!is_number(level)){
      ch->println("Level_flexivalue must be a numeric expression, digits and +/- are allowed.");
      return;
   }
   
   int l=atoi(lower);
   int u=atoi(upper);

   if(l>u){
      ch->printlnf("Lower vnum (%d) is greater than upper vnum (%d)!", l, u);
      return;
   }

   ch->printlnf("Autobalance on any mobs %d thru %d... using autostat setting of '%s'",
      l, u, level);


   MOB_INDEX_DATA *mi;
   for(int i=l; i<=u; i++){
      mi=get_mob_index( i);
      if(mi){
         autostat_mindex(ch, level, mi);
      }
   }
}
/**************************************************************************/
// Written by Daos - Nov. 2003
bool autostat_oindex(char_data* ch, char* argument, obj_index_data *oi)
{
   char buf[MSL];
   int level;

   if(!IS_NULLSTR(argument) && argument[0]=='?')
   {
      ch->println("oedit_autostat -  syntax: autostat <flexivalue_level>");
      ch->printlnf("Flexivalue is the term %s uses for is a value that can be absolute or relevative.",
         MUD_NAME);
      ch->println("autostat      - sets the stats for the objects current level.");
      ch->println("autostat 10   - sets the stats as if the object was level 10.");
      ch->println("autostat +10  - sets the stats as if the object was 10 levels higher than its current level.");
      ch->println("autostat -10  - sets the stats as if the object was 10 levels lower than its current level.");
      return false;
   }
   
   if(!find_flexivalue(ch, argument, &level, oi->level, 0, MAX_LEVEL+O_STAT_ABOVE_MAX_LEVEL-1))
   {
      return false;
   };
   
   if(level==oi->level)
   {
      ch->printlnf("\r\nAuto setting stats for object[%d] level %d",
         oi->vnum,
         level);
   }
   else
   {
      ch->printlnf("\r\nAuto setting stats for %slevel %d`x, (object[%d] level is %d).",
         level>oi->level?"`G":"`R",
         level,
         oi->vnum,
         oi->level);
   }

   /***********************************************
   * These are necessary weapon-type checks so    *
   * the code initiates the proper weapon-damage  *
   * settings.  The code checks for the most      *
   * common weapon-types.  Feel free to add any   *
   * additional weapon-checks to suit your MUD.   *
   ***********************************************/

   switch (oi->item_type) {
      default:
         break;
      case ITEM_WEAPON: {
         int olev=level; 
   

      /*************************************
      * Below is code to add intelligence  *
      * for the autostat to consider v4    *
      * weapon values for the autobalance. *
      * Added Feb. 2004 - Daos             *
      *************************************/

      if (IS_SET(oi->value[4], WEAPON_FLAMING))
      {
         olev-=3;
         ch->printlnf("Flaming Weapon = -3 levels.");
      }

      if (IS_SET(oi->value[4], WEAPON_FROST))
      {
         olev-=3;
         ch->printlnf("Frost Weapon = -3 levels.");
      }

      if (IS_SET(oi->value[4], WEAPON_SHOCKING))
      {
         olev-=3;
         ch->printlnf("Shocking Weapon = -3 levels.");
      }

      if (IS_SET(oi->value[4], WEAPON_POISON))
      {
         olev-=3;
         ch->printlnf("Poison Weapon = -3 levels.");
      }

      if (IS_SET(oi->value[4], WEAPON_HOLY))
      {
         olev-=3;
         ch->printlnf("Holy/Unholy Weapon = -3 levels.");
      }

      if (IS_SET(oi->value[4], WEAPON_SHARP))
      {
         olev-=4;
         ch->printlnf("Sharp Weapon = -4 levels.");
      }

      if (IS_SET(oi->value[4], WEAPON_VORPAL))
      {
         olev-=4;
         ch->printlnf("Vorpal Weapon = -4 levels.");
      }

      if (IS_SET(oi->value[4], WEAPON_VAMPIRIC))
      {
         olev-=4;
         ch->printlnf("Vampiric Weapon = -4 levels.");
      }

	  if (IS_SET(oi->value[4], WEAPON_SUCKLE))
      {
         olev-=4;
         ch->printlnf("Mana Drain Weapon = -4 levels.");
      }

	  if (IS_SET(oi->value[4], WEAPON_ENERVATE))
      {
         olev-=4;
         ch->printlnf("Move Drain Weapon = -4 levels.");
      }

	  if (IS_SET(oi->value[4], WEAPON_ANNEALED))
      {
         olev-=2;
         ch->printlnf("Stunning Weapon = -2 levels.");
      }

	  if (IS_SET(oi->value[4], WEAPON_MORPHABLE))
      {
         olev-=4;
         ch->printlnf("Morphing Weapon = -6 levels.");
      }

	  if ( IS_SET( oi->extra2_flags, OBJEXTRA2_RELIC))
      {
         olev-=4;
         ch->printlnf("Relic Weapon = -6 levels.");
      }

	  if (IS_SET(oi->value[4], WEAPON_TWO_HANDS))
      {
         olev+=6;
         ch->printlnf("Two Handed Weapon = +6 levels.");
      }

	  if (IS_SET(oi->value[4], WEAPON_ACIDIC))
      {
         olev-=3;
         ch->printlnf("Acidic Weapon = -3 levels.");
      }
	
      /*********************************
      * Autobalance code below to      *
      * determine proper balance level *
      * based on weapon type.          *
      * Added Nov. 2003 - Daos         *
      *********************************/

      if (oi->value[0] == WEAPON_DAGGER)
      {
         ch->printlnf("Weapon Type - Dagger = -8 levels.");
         olev-=8;
      }

      if (oi->value[0] == WEAPON_WHIP)
      {
         ch->printlnf("Weapon Type - Whip = -3 levels.");
         olev-=3;
      }

      if (oi->value[0] == WEAPON_FLAIL)
      {
         ch->printlnf("Weapon Type - Flail = -2 levels.");
         olev-=2;
      }

      if (oi->value[0] == WEAPON_AXE)
      {
         ch->printlnf("Weapon Type - Axe = +3 levels.");
         olev+=3;
      }

      if (oi->value[0] == WEAPON_POLEARM)
      {
         ch->printlnf("Weapon Type - Polearm = +8 levels.");
         olev+=8;
      }

      if (oi->value[0] == WEAPON_STAFF)
      {
         ch->printlnf("Weapon Type - Staff = +3 levels.");
         olev+=3;
      }


      if (oi->value[0] == WEAPON_LIGHTBLADE)
      {
         ch->printlnf("Weapon Type - Lightblade = +6 levels.");
         olev+=6;
      }

	  
      if (oi->value[0] == WEAPON_BLASTRIFLE)
      {
         ch->printlnf("Weapon Type - Blastrifle = +6 levels.");
         olev+=6;
      }

	  if (oi->value[0] == WEAPON_BLASTPISTOL)
      {
         ch->printlnf("Weapon Type - Blastpistol = +6 levels.");
         olev+=6;
      }

      sprintf(buf,"%d+%d",
         oi->value[1],
         oi->value[2]);

      if(obj_dam.apply_objstat(olev, &oi->value[1],
         &oi->value[2]))
      {
         ch->printlnf("Weapon dice changed from %s to %d+%d",
            buf,
            oi->value[1],
            oi->value[2]);
      }else{
         ch->printlnf("`RWeapon dice unchanged from %s - most likely due to no dice settings for level %d?`x",
            buf,
            olev);
      }
   }break;

   /******************************************************
   * This is the autobalance for ACvalue code.  In this  *
   * commented section is the stock balance from Dawn in *
   * which this code is based upon.  Feel free to modify *
   * it if it doesn't apply to the balance standards of  *
   * your MUD. - Daos   Nov. 2003                    *
   * --------------------------------------------------- *
   * Wear-Location     ACpierce  ACslash  ACbash  ACmagic  *
   *                                         *
   * Face         level/12  level/12 level/12  level/15 *
   * Finger      level/14  level/14 level/14  level/15 *
   * Neck         level/6   level/6  level/6   level/8  *
   * Torso         level/3   level/3  level/3   level/4  *
   * Head          level/4     level/4  level/4   level/5  *
   * Legs         level/4   level/4  level/4   level/5  *
   * Feet         level/5   level/5  level/5   level/6  *
   * Hands         level/5   level/5  level/5   level/6  *
   * About Waist   level/5     level/5  level/5   level/7  *
   * Arm         level/4   level/4  level/4   level/5  *
   * Wrist         level/7     level/7  level/7   level/9  *
   * Ankle         level/7     level/7  level/7   level/9  *
   * Shield        level/3   level/3  level/3   level/4  *
   ******************************************************/

   case ITEM_ARMOR: 
   {
      int aclev = level;

      if ( IS_SET(oi->wear_flags, OBJWEAR_FACE))
      {
         oi->value[0] = aclev/12;
         oi->value[1] = aclev/12;
         oi->value[2] = aclev/12;
         oi->value[3] = aclev/15;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_FINGER )
		  || IS_SET(oi->wear_flags, OBJWEAR_EAR )
		  || IS_SET(oi->wear_flags, OBJWEAR_EYES ))
      {
         oi->value[0] = aclev/14;
         oi->value[1] = aclev/14;
         oi->value[2] = aclev/14;
         oi->value[3] = aclev/15;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_NECK ))
      {
         oi->value[0] = aclev/6;
         oi->value[1] = aclev/6;
         oi->value[2] = aclev/6;
         oi->value[3] = aclev/8;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_TORSO ))
      {   
         oi->value[0] = aclev/3;
         oi->value[1] = aclev/3;
         oi->value[2] = aclev/3;
         oi->value[3] = aclev/4;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_HEAD ))
      {
         oi->value[0] = aclev/4;
         oi->value[1] = aclev/4;
         oi->value[2] = aclev/4;
         oi->value[3] = aclev/5;
      }
   
      if ( IS_SET(oi->wear_flags, OBJWEAR_LEGS ))
      {
         oi->value[0] = aclev/4;
         oi->value[1] = aclev/4;
         oi->value[2] = aclev/4;
         oi->value[3] = aclev/5;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_FEET ))
      {
         oi->value[0] = aclev/5;
         oi->value[1] = aclev/5;
         oi->value[2] = aclev/5;
         oi->value[3] = aclev/6;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_HANDS ))
      {
         oi->value[0] = aclev/5;
         oi->value[1] = aclev/5;
         oi->value[2] = aclev/5;
         oi->value[3] = aclev/6;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_ABOUT ))
      {
         oi->value[0] = aclev/5;
         oi->value[1] = aclev/5;
         oi->value[2] = aclev/5;
         oi->value[3] = aclev/7;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_WAIST ))
      {
         oi->value[0] = aclev/5;
         oi->value[1] = aclev/5;
         oi->value[2] = aclev/5;
         oi->value[3] = aclev/7;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_ARMS ))
      {
         oi->value[0] = aclev/4;
         oi->value[1] = aclev/4;
         oi->value[2] = aclev/4;
         oi->value[3] = aclev/5;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_WRIST ))
      {
         oi->value[0] = aclev/7;
         oi->value[1] = aclev/7;
         oi->value[2] = aclev/7;
         oi->value[3] = aclev/9;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_ANKLE ))
      {
         oi->value[0] = aclev/7;
         oi->value[1] = aclev/7;
         oi->value[2] = aclev/7;
         oi->value[3] = aclev/9;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_SHIELD ))
      {
         oi->value[0] = aclev/3;
         oi->value[1] = aclev/3;
         oi->value[2] = aclev/3;
         oi->value[3] = aclev/4;
      }

      if ( IS_SET(oi->wear_flags, OBJWEAR_BACK ))
      {
         oi->value[0] = aclev/6;
         oi->value[1] = aclev/6;
         oi->value[2] = aclev/6;
         oi->value[3] = aclev/7;
      }

      ch->printlnf("AC value settings changed to ACpierce[%d] ACbash[%d] ACslash[%d] ACother[%d]",
         oi->value[0],
         oi->value[1],
         oi->value[2],
         oi->value[3]);
      }break;
	}

   if(IS_NULLSTR(argument))
   {   
      ch->println("`xType '`=Cautostat ?`x' for help on some of the features of autostat.");
   }
   return true;
}
/**************************************************************************/
// Written by Daos - Nov. 2003
bool oedit_autostat(char_data* ch, char* argument)
{
   obj_index_data *oi;

   EDIT_OBJ(ch, oi);

   return autostat_oindex(ch, argument, oi);
}
/***************************************************************************/
