/**************************************************************************/
// fight.cpp -
/***************************************************************************
 * The Dawn of Time v1.69s.beta6 (c)1997-2010 Kalahn                       *
 * >> A number of people have contributed to the Dawn codebase, with the   *
 *    majority of code written by Kalahn - www.dawnoftime.org              *
 * >> To use this source code, you must fully comply with all the licenses *
 *    in licenses.txt... In particular, you may not remove this copyright  *
 *    notice.                                                              *
 ***************************************************************************
 * >> Original Diku Mud copyright (c)1990, 1991 by Sebastian Hammer,       *
 *    Michael Seifert, Hans Henrik Staerfeldt, Tom Madsen, & Katja Nyboe.  *
 * >> Merc Diku Mud improvements copyright (C) 1992, 1993 by Michael       *
 *    Chastain, Michael Quan, and Mitchell Tse.                            *
 * >> ROM 2.4 is copyright 1993-1995 Russ Taylor and has been brought to   *
 *    you by the ROM consortium: Russ Taylor(rtaylor@pacinfo.com),         *
 *    Gabrielle Taylor(gtaylor@pacinfo.com) & Brian Moore(rom@rom.efn.org) *
 * >> Oblivion 1.2 is copyright 1996 Wes Wagner                            *
 **************************************************************************/

#include "include.h" // dawn standard includes
#include "clan.h"
#include "duel.h"
#include "msp.h"
#include "d2magsys.h"
#include "recycle.h"
#define MAX_DAMAGE_MESSAGE 44
DECLARE_DO_FUN( do_fight		);//TBC

/* command procedures needed */
DECLARE_DO_FUN(do_backstab      );
DECLARE_DO_FUN(do_pbackstab		);
DECLARE_DO_FUN(do_cardform      );
DECLARE_DO_FUN(do_circle		);
DECLARE_DO_FUN(do_emote         );
DECLARE_DO_FUN(do_berserk       );
DECLARE_DO_FUN(do_bash          );
DECLARE_DO_FUN(do_trip          );
DECLARE_DO_FUN(do_dirt          );
DECLARE_DO_FUN(do_flee          );
DECLARE_DO_FUN(do_kick          );
DECLARE_DO_FUN(do_kill          );
DECLARE_DO_FUN(do_disarm        );
DECLARE_DO_FUN(do_get           );
//DECLARE_DO_FUN( do_petwhistle);
DECLARE_DO_FUN(do_recall        );
DECLARE_DO_FUN(do_yell          );
DECLARE_DO_FUN(do_gore			); // Kerenos
DECLARE_DO_FUN(do_disarm_trap	);
DECLARE_DO_FUN(do_visible		);
DECLARE_SPEC_FUN( spec_cast_mage );
DECLARE_SPEC_FUN( spec_cast_cleric );
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_omega_form          );
DECLARE_DO_FUN(do_proto_form          );
DECLARE_DO_FUN(do_power_form          );
DECLARE_DO_FUN(do_active_form          );
DECLARE_DO_FUN(do_xbuster          );
DECLARE_DO_FUN(do_aegisblaster          );
DECLARE_DO_FUN(do_aegisaber          );

char *get_weapontype(OBJ_DATA *obj);
char *get_materialtype(OBJ_DATA *obj);
void landchar( char_data *ch);
int group_max_level_attacking_victim(char_data *killer, char_data *victim );
void group_bypass_killer_penatly(char_data *killer, char_data *victim, bool death);
void pkill_note_required_message(char_data *ch, char_data *victim, bool bypass_duel_used);
bool can_join_combat( char_data *attacker, char_data *victim);
void do_speedcast2( char_data *ch, char *argument, int cast_level, CLASS_CAST_TYPE type );
void do_newcast( char_data *ch, char *argument, int cast_level, char *target_name );
char *  const   death_cry_dir_name[]=
{
    "the north", "the east", "the south", "the west", "above", "below",
	"the northeast", "the southeast", "the southwest", "the northwest"
};

/*
 * Local functions.
 */
void	pkill_autonote		args(( char_data *ch, char_data *victim ));
void    check_assist		args(( char_data *ch, char_data *victim ));
bool    check_dodge			args(( char_data *ch, char_data *victim ));
bool    check_parry			args(( char_data *ch, char_data *victim ));
bool    check_fade			args(( char_data *ch, char_data *victim ));
bool    check_shield_block	args(( char_data *ch, char_data *victim ));
void    dam_message			args(( char_data *ch, char_data *victim, int dam,
									int dt, bool immune ));
void    death_cry			args(( char_data *ch, char_data *killer ));
void    group_gain			args(( char_data *ch, char_data *victim ));
int     xp_compute			args(( char_data *gch, char_data *victim,
									int total_levels));
bool    is_safe				args(( char_data *ch, char_data *victim ));
void    make_corpse			args(( char_data *ch, char *killer ));
void    one_hit				args(( char_data *ch, char_data *victim, int dt, bool second ));
void    mob_hit				args(( char_data *ch, char_data *victim, int dt ));
void	set_fighting		args(( char_data *ch, char_data *victim ));
void    disarm			    args(( char_data *ch, char_data *victim ));
void    entangle		    args(( char_data *ch, char_data *victim ));
void    mount				args(( char_data *, char_data * ));
void    dismount			args(( char_data * ));
void	show_list_to_char	args(( OBJ_DATA *list, char_data *ch, char * filter,
									bool fShort, bool fShowNothing ));

void    do_tension          args((char_data * ch, char * argument));
/**************************************************************************/
// lame way to do this, but it was in the oblivion code and I
// can't be bothered changing it cause the code is only called
// when a pkill happens - it was originally put in update_handler() with
// ran everytime violence_update() was called!!!
void check_death_update(void)
{
	char_data *victim;
	char_data *next_player;
    for ( victim = player_list; victim; victim = next_player )
    {
		next_player=victim->next_player;

		if(victim->pcdata->karns<0)
		{
			flush_char_outbuffer(victim);
			victim->titlebar("`RPERMDEATH");
			victim->wrapln(
				"You have been pkilled when you had no karns.  The system is if you "
				"are pkilled you lose a karn, if you are pkilled when you have no karns "
				"the death is considered permanent.");
			victim->println("");
			victim->wrapln("Please keep in mind that it is bad RP to discuss this unless you think "
				"it was a OOC pkill.  If you consider it ooc please write a note to admin using a "
				"different character explaining the situation and we will look into it.");
			victim->titlebar("`RYOU WILL NOW BE DISCONNECTED SINCE YOUR CHARACTER IS PERMANENTLY DEAD");
			flush_char_outbuffer(victim);

			logf( "PERMDEATH: Closing link to %s.", victim->name);

			// calculate the filenames relating the victim before we kick them off
			char filenamebuf[MIL];
			strcpy(filenamebuf, pfilename(victim->name, victim->pcdata->pfiletype));
			char deadpfilename[MSL];
			sprintf(deadpfilename, "%s%s", DEAD_DIR, pfile_filename(victim->name));

			// do the autonote
			char subject[MIL];
			sprintf(subject,"`RPERMDEATH:`W %s!`x", victim->name);
/*			autonote(NOTE_INOTE, "Your friendly neighbourhood pkill system",
				subject, "imm", FORMATF("Pfile moved from '%s' to '%s'.", filenamebuf, deadpfilename), true);
			autonote(NOTE_PKNOTE, "Your friendly neighbourhood pkill system",
				subject, "imm", FORMATF("Pfile moved from '%s' to '%s'.", filenamebuf, deadpfilename), true);*/

			// disconnect them
			extract_char(victim,true);
			connection_close(victim->desc);

			// move them to the dead directory
			if(rename(filenamebuf,deadpfilename)!=0){
				char errbuf[MSL];
				sprintf(errbuf,"check_death_update(): error moving pfile '%s' to the dead directory (%s), error %d (%s)",
					filenamebuf,
					deadpfilename,
					errno,
					strerror(errno));
				log_string(errbuf);
				bug(errbuf);
				wiznet(errbuf,NULL,NULL,WIZ_SECURE,0,0);
//				autonote(NOTE_SNOTE, "check_death_update()", "error moving pfile to the dead directory!", "admin", errbuf, true);
			}
		}
	}
}
/**************************************************************************/
/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
    char_data *ch;
    char_data *ch_next;
    char_data *victim;

    for ( ch = char_list; ch; ch = ch_next )
    {
		ch_next = ch->next;
		/*start of random battle procedure*/
		if (!IS_NPC(ch) && !ch->fighting && !IS_SET(ch->act2,PLR2_TBC)
            && !IS_SET(ch->in_room->room_flags, ROOM_SAFE)){
            int rnd_battle = 0;
            rnd_battle = number_range(1,100);
            if (rnd_battle >90){
        if ( IS_SET(ch->in_room->room2_flags, ROOM2_RND_BATTLE_ZONE )
        || IS_SET(ch->in_room->area->area_flags, AREA_RND_BATTLE_ZONE)
         ){
        int rngcount =0;
        int i =0;
        int minrndvnum = ch->in_room->area->min_rndbtlvnum;
        int maxrndvnum = ch->in_room->area->max_rndbtlvnum;
        for (i = minrndvnum; i <= maxrndvnum; i++) {
            if (get_mob_index(i) != NULL) {
            rngcount++;}
        }
        ROOM_INDEX_DATA *ambushroom;
        ambushroom = ch->in_room;
        int mobteamsize =0;
        mobteamsize = number_range(0,4);
        char_data *ambusher;
        if (mobteamsize != 0){
        ch->println("`R!!!!!!!!!!!AMBUSH!!!!!!!!!!!!!`x");
        int mobteammember;
        mobteammember = 0;
        while (mobteamsize >= 1){
        mobteammember = minrndvnum += number_range(0,rngcount);
        if (mobteammember >= maxrndvnum){
        mobteammember = maxrndvnum;
        }
        ambusher = create_mobile(get_mob_index(mobteammember), 0);
        char_to_room(ambusher, ambushroom);
        multi_hit( ambusher, ch, TYPE_UNDEFINED );
        --mobteamsize;}
}
}
}
}/*end of random battle procedure*/


		if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
			continue;

		// do a room check
		if( ch->in_room!=ch->fighting->in_room){
		//	bug("violence_update(): ch->in_room != ch->fighting->in_room");
			stop_fighting(ch,false);
			continue;
		}


		if(ch->bleeding)
		{
			damage( ch, ch, ch->bleeding, TYPE_UNDEFINED, DAM_NONE,false);
			act("$n's deteriorating condition is very apparent.",
				ch,NULL,NULL,TO_ROOM);
			ch->println("You lose even more blood.");
		}

		if(ch->is_stunned)
		{
			ch->is_stunned-=1;
			if(ch->is_stunned<=0)
			{
				ch->position=POS_RESTING;
				ch->is_stunned=0;
				ch->println("You are no longer stunned.");
			}
		}

		if(ch->will_die)
		{
			ch->will_die-=20;
			if(ch->will_die<=0)
			{
				ch->will_die=0;
				// we know victim exists and is in the same room
				kill_char(ch, victim);
				continue;
			}
		}



        if (IS_AWAKE (ch) && ch->in_room == victim->in_room)
		{
			//Using 'tension'...
			if (ch->tension > 0)
			{
				AFFECT_DATA af;

				//Tweak these to your liking.
				sh_int tension_data[5][3] =
				{
					{100,  0,  0},
					{100,  5,  0},
					{ 90, 10,  5},
					{ 80, 15, 10},
					{ 40, 20, 15},
				};

				//Super High Tension has lower chance in even battles.
				sh_int chance = tension_data[ch->tension][0];
				if (ch->tension == 4 && ch->fighting->level - ch->level < 5)
					chance -= 10;

				//Tension increased.
				if (number_percent () < chance)
				{
					//Boost hitroll and damroll the easy way.

					af.where = WHERE_AFFECTS;
					af.type = gsn_tension;
					af.duration = -1;
					af.bitvector = 0;
					af.level = 0;

					af.location = APPLY_DAMROLL;
					af.modifier = tension_data[ch->tension][1];
					affect_to_char (ch, &af);

					af.location = APPLY_HITROLL;
					af.modifier = tension_data[ch->tension][2];
					affect_to_char (ch, &af);

					++ch->tension;

					//Send the proper message.
					if (ch->tension == 4)
					{
						act ("You reach a state of high tension!",
								ch, NULL, NULL, TO_CHAR);
						act ("$n reaches a state of high tension!",
								ch, NULL, NULL, TO_ROOM);
					}
					else if (ch->tension == 5)
					{
						act ("You reach a state of super high tension!",
								ch, NULL, NULL, TO_CHAR);
						act ("$n reaches a state of super high tension!",
								ch, NULL, NULL, TO_ROOM);
					}
					else
					{
						act ("Your tension increases!", ch, NULL, NULL, TO_CHAR);
						act ("$n's tension increases!", ch, NULL, NULL, TO_ROOM);
					}

					//Automatically blow it at Super High.
					if (ch->tension > 4)
						do_tension (ch, "");
				}
				else
				{
					//Failed Super High in an even match, use it up.
					if (ch->tension == 4 && ch->fighting->level - ch->level < 5)
						do_tension (ch, "");
					//Otherwise, let them keep trying.
					else
					{
						act ("You try to psyche yourself up, but nothing happens.",
								ch, NULL, NULL, TO_CHAR);
						act ("$n tries to increase $s tension, but nothing happens.",
								ch, NULL, NULL, TO_ROOM);
					}
				}
			}
			//Not using 'tension', act normally.
			else
	            multi_hit (ch, victim, TYPE_UNDEFINED);
		}
        else{
            stop_fighting (ch, false);
            }
/*		if ( IS_AWAKE(ch) && ch->in_room == victim->in_room ){
			multi_hit( ch, victim, TYPE_UNDEFINED );
		}else{
			stop_fighting( ch, false ); // Violence_update - not awake/diff room
		}
*/





		if (( victim = ch->fighting ) == NULL )
			continue;

		// Fun for the whole family!
		if (!IS_SET(victim->act2,PLR2_TBC)
		&& !IS_SET(ch->act2,PLR2_TBC)){
		check_assist(ch,victim);
        }

		if ( IS_NPC( ch ) )	{
			if ( HAS_TRIGGER( ch, MTRIG_FIGHT ) )
				mp_percent_trigger( ch, victim, NULL, NULL, MTRIG_FIGHT );
			if ( HAS_TRIGGER( ch, MTRIG_HPCNT ) )
				mp_hprct_trigger( ch, victim );
		}
	}
	return;
}
/**************************************************************************/
int get_sublevels_for_level(int level);
/**************************************************************************/
// Called when a player/mob victim is killed:
// - victim position should be POS_DEAD when called
// - handles the loss of karns, and resetting of karn countdowns
//   loss of xp when dieing
// - records mobkills, pkills, pkdefeat, pkools
// - does NOT handle pknorecall, pksafe
// - autolooting
// - autonoting pkills
// - mob death trigger
// After this function, the following can be considered true
// - if the victim is a mob, it no longer valid
// - no one will no longer be fighting the victim
void kill_char(char_data *victim, char_data *ch)
{
	if(victim->position != POS_DEAD ){
		bug("kill_char(): victim->position != POS_DEAD ");
		return;
	}

	if(!IS_VALID(ch)){
		bugf("kill_char(): ch (%d) is not valid!", ch->vnum());
	}

	if(!IS_VALID(victim)){
		bugf("kill_char(): victim (%d) is not valid!", victim->vnum());
	}

	ROOM_INDEX_DATA *deathroom;
	if(ch->in_room){
		deathroom=ch->in_room;
	}else if(victim->in_room){
		deathroom=victim->in_room;
		bug("kill_char(): ch->in_room==NULL, setting it to victim->in_room");
		ch->in_room=victim->in_room;
	}else{
		bugf("kill_char(): Couldn't find DEATHROOM!!!... setting it to limbo.");
		deathroom=get_room_index(ROOM_VNUM_LIMBO);
		assertp(deathroom);
	}

	group_gain( ch, victim );

	// record mob kill values
	if (!IS_NPC(ch) && IS_NPC(victim)){
		ch->pcdata->mkills++;
       if (ch->symbiotetype != 0){
        ch->symbiotemkills += 1;
        if (ch->symbiotemkills >= 100){
        ch->symbiotemkills = 0;
        if (ch->symbiotelvl < 3){
        ch->symbiotelvl++;
        ch->println("`RYour Symbiote has grown!");
        if (ch->symbiotelvl == 3){
        ch->println("`GYour Symbiote has matured!!");
        }
        }else{
        ch->symbiotepoints++;
        ch->println("`GYour Symbiote gains a point.");
        }
    }
    }

	}else if (IS_NPC(ch) && !IS_NPC(victim)){
		victim->pcdata->mdefeats++;
	}

	if ( !IS_NPC(victim) )
	{

		OBJ_DATA *token;
	    if ( ( ( token = get_obj_token( victim, "702")) != NULL)
                    && !IS_SET(ch->in_room->area->area_flags, AREA_ARENA)
                    && !IS_SET(ch->in_room->area->area_flags, AREA_VIRTUAL))
	    {
		   obj_from_char (token);
		   extract_obj (token);
		   victim->println("You avoid your own death and suddenly vanish!");
		   ch->printlnf("%s avoids death at your hands and vanishes!", victim->name);
		   victim->position = POS_RESTING;
	 	   char_from_room(victim);
		   char_to_room(victim, get_room_index(1));
		   victim->hit = victim->max_hit;
		   do_look(victim, "auto");
		   return;
	    }


        if(IS_SET(ch->in_room->area->area_flags, AREA_ARENA)){

        victim->position = POS_RESTING;
	 	char_from_room(victim);
		char_to_room(victim, get_room_index(ch->in_room->area->min_vnum));
		victim->hit = victim->max_hit;
        victim->mana = victim->max_mana;
        victim->psi = victim->max_psi;
        do_look(victim, "auto");
        return;}



		if (IS_NPC(ch)){ // mob killed player
			sprintf( log_buf, "%s mkilled by %s at %d (kill_char())",
				victim->name,
				ch->short_descr,
				deathroom->vnum);

			// send the info
			info_broadcast(victim, "%s has been toasted by %s.",	victim->name, ch->short_descr);

			// Log mobkills
			{
				char mklogbuf[MSL];
				sprintf( mklogbuf, "%s mkilled by %s at %d (kill_char())",
					victim->name,
					ch->short_descr,
					deathroom->vnum);
				append_datetime_ch_to_file( ch, MKILL_LOGFILE, mklogbuf);
			}

		}else{ // player killed player
			sprintf( log_buf, "%s pkilled by %s at %d (kill_char())",
				victim->name,
				ch->name,
				deathroom->vnum);

			// send the info
			info_broadcast(victim, "%s has been toasted by %s!", victim->name, ch->name);

		}

		log_string( log_buf );

		if(ch->pkool>0 && victim->level<5){
		// ch didn't start the fight so they don't get the pkills on them
		// reducing their max karns
			ch->pkkills--;
		}

		if(victim->level<11 && !IS_LETGAINED(victim) && GAMESETTING_LETGAINING_IS_REQUIRED){
			if(!GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
				ch->pkool+=4000;
			}
		}

		// pk system
		if(ch!=victim && !IS_NPC(ch) && !IS_NPC(victim))
		{
			ch->pkkills+=1;
			victim->pkdefeats+=1;
			if(!GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
				// new pk karn system
				if(group_max_level_attacking_victim(ch, victim)<=victim->level+10){
					victim->pcdata->karns--;
					if(GAMESETTING(GAMESET_NOPERMDEATH) && victim->pcdata->karns<0){
						victim->pcdata->karns=0;
					}
					if(victim->pcdata->karns) // if they actually lost a karn
					{
						victim->println(
							"From within your body you feel a surge of energy,\r\n"
							"a ball of light departs from your body and disappears!");
					}

				}
				group_bypass_killer_penatly(ch, victim, true);


				if(GAMESETTING3(GAMESET3_KILLER_SYSTEM_ENABLED)
					&& !IS_SET(victim->dyn, DYN_STARTED_FIGHT))
				{
					// mark the attackers as killers if the victim didn't start the fight
					char_data *gch;
					for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
					{
						if ( !IS_NPC(gch) && gch->fighting==victim && is_same_group( gch, ch))
						{
							if(!IS_KILLER(gch)){
								ch->println( "*** You are now tagged as a KILLER!! ***");
							}
							gch->pcdata->killer_until=current_time+ (60*game_settings->killer_system_tagged_duration);
							save_char_obj( gch );
						}
					}
				}

				// reduce the victims killer timer
				if(IS_KILLER(victim)){
					victim->pcdata->killer_until-=
						(60*game_settings->killer_system_death_reduction_duration);
					if(!IS_KILLER(victim)){
						victim->pcdata->killer_until=0;
					}
				}

				// update duels to protect victim
				duel_protect_victim(victim);
				// karn checking is done at the end of this function
			}

			if(	ch->pknorecall==0){
				ch->pknorecall=5;
			}
			ch->pknoquit=UMAX(ch->pknorecall,ch->pknoquit);
			if(GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
				ch->pknoquit=UMIN(ch->pknoquit,2);
			}

			victim->pcdata->next_karn_countdown=
				UMIN(victim->pcdata->next_karn_countdown,GET_NEW_KARN_COUNTER(victim));

			// autonote the pkill
			pkill_autonote(ch, victim);
			if(victim->alliance < -349){
			ch->alliance += 25;
			}else if(victim->alliance > 350){
			ch->alliance -= 25;
			}

			if(ch->alliance > 1000){
			ch->alliance = 1000;
			}else if(ch->alliance < -1000){
			ch->alliance = -1000;
			}

			// deactivate autopkassist
			if (IS_SET(victim->dyn,DYN_AUTOPKASSIST))
			{
				victim->println("`YYour autopkassist setting has been deactivated.`x");
				REMOVE_BIT(victim->dyn,DYN_AUTOPKASSIST);
			}

			// remove thief status on death
			if(IS_THIEF(victim)){
				victim->pcdata->theft = 0;
				victim->println("You are no longer marked as a thief");
				victim->pcdata->thief_until=0;
			}

		}

		// Dying penalty: 2/3 way back to previous level for those level 20 or lower
		{
			if(victim->level<21)
			{
				if ( victim->exp > exp_per_level(victim,victim->pcdata->points)
					* victim->level )
				{

					gain_exp( victim, (2 * (exp_per_level(victim,victim->pcdata->points)
						* victim->level - victim->exp)/3) + 50 );
				}
			}
			else
			{
				int xp_amount;
				if(victim->level>50){
					xp_amount=500+(get_sublevels_for_level( victim->level) *150);
				}else{
					xp_amount=500;
				}

				if(IS_HERO(victim)){
					if(GAMESETTING5(GAMESET5_HEROS_DONT_LOSE_XP_FOR_DYING)){
						victim->printlnf("You would have lost %d xp if you weren't a hero.", xp_amount);
					}else{
						victim->printlnf("You have lost %d xp.", xp_amount);
						gain_exp( victim, -xp_amount);
					}
				}else{
					victim->printlnf("You have lost %d xp.", xp_amount);
					gain_exp( victim, -xp_amount);
				}

				if ( IS_HERO( victim )){
					do_heroxp( victim, -xp_amount);
				}

				if(victim->exp<exp_per_level(victim,victim->pcdata->points)* victim->level )
				{
//					drop_level(victim);
//					ch->exp = exp_per_level(ch,ch->pcdata->points)* ch->level;
//					check_perm_damage(victim);
				}else{
					if(number_range(1,get_sublevels_for_level( victim->level+5))<2){
//						check_perm_damage(victim);
					}else{
						victim->println("Luckily you have not suffered serious injury.");
					}
				}
			}
		}
	}

	sprintf( log_buf, "%s [lvl %d] got toasted by %s at %s [lvl %d] [room %d] (%s)",
		(IS_NPC(victim) ? victim->short_descr : victim->name),
		victim->level,
		(IS_NPC(ch) ? ch->short_descr : ch->name),
		deathroom->name, ch->level, deathroom->vnum,
		deathroom->area->name);

	if (IS_NPC(victim)){
		wiznet(log_buf,NULL,NULL,WIZ_MOBDEATHS,0,0);
	}else{
		wiznet(log_buf,NULL,NULL,WIZ_DEATHS,0,0);
	}

	// Death trigger mudprog trigger
	if ( IS_NPC( victim ) && HAS_TRIGGER( victim, MTRIG_DEATH) )
	{
		victim->position = POS_STANDING;
		mp_percent_trigger( victim, ch, NULL, NULL, MTRIG_DEATH );
	}


	ch->mobmemory = NULL;
	victim->mobmemory = NULL;

	raw_kill( victim, ch );

	// handle players autolooting and autogold mob corpses
	if ( !IS_NPC(ch) && IS_NPC(victim) )
	{
		OBJ_DATA *corpse;

		corpse = get_obj_list( ch, "corpse", deathroom->contents );

		if(corpse && corpse->contains) // a corpse exists and isnt empty
		{
			if (IS_SET(ch->act, PLR_AUTOLOOT))
			{
				do_get( ch, "all corpse" );
			}
			else
			{
				if(IS_SET(ch->act,PLR_AUTOGOLD) )
				{
					OBJ_DATA *coins=get_obj_list(ch,"gcash",corpse->contains);
					if (coins)
					{
						do_get(ch, "all.gcash corpse");
					}
				}
			}
		}
		if ( corpse
		&&   HAS_CONFIG(ch, CONFIG_AUTOEXAMINE)
		&&  !IS_SET( ch->act,PLR_AUTOLOOT ))
		{
			act( "$p holds:", ch, corpse, NULL, TO_CHAR );
			show_list_to_char( corpse->contains, ch, "", true, true );
		}
	}
	if(!IS_NPC(victim)){
		check_death_update();
	}
}

/**************************************************************************/
void do_eshield( char_data *ch, char *);
/**************************************************************************/
// Calculate how much damage will be inflicted
// sets DYN_DOING_DAMAGE, for other functions to unset unless result <0
// return values:  <0 if damage_display_infliction_message() and
//                    damage_outcome() should NOT be returned
//                 otherwise the amount of damage done
// the input dam must be >= 0
int damage_calculate_infliction(char_data *ch, char_data *victim,
								int dam, int dt, int dam_type)
{
	// catch any requests for negative damages... shouldn't be possible
	if(dam<0){
		bugf("damage_calculate_infliction(): input dam value=%d (less than 0), dt=%d, dam_type=%d... aborting.",
			dam, dt,dam_type);
		do_abort();
	}

    if ( !IS_VALID(victim) || victim->position == POS_DEAD ){
		return -1;
	}

    // doing damage system used to determine who starts fights
	if(    dt!=gsn_fire_shield
		&& dt!=gsn_chill_shield
	//	&& dt!=gsn_icy_armor
	/*	&& dt!=gsn_fiery_armor*/ )
	{
//	if (!IS_SET(ch->act2,PLR2_TBC) && !IS_SET(victim->act2,PLR2_TBC)){
		SET_BIT(ch->dyn,DYN_DOING_DAMAGE);
//		}
	}

	//	Stop up any residual loopholes.
	if ( dam > 999 && dt >= TYPE_HIT
		&&!IS_IMMORTAL(ch)
		&&!IS_HERO(ch)
		&& !IS_NPC(ch))
    {
		bugf( "Damage: %d: more than 999 points! by %s.", dam, PERS(ch, NULL));
		dam = 999;
//		if (!IS_IMMORTAL(ch))
//		{
//			OBJ_DATA *obj;
//			obj = get_eq_char( ch, WEAR_WIELD );
//			ch->println("You really shouldn't cheat.");
//			if (obj != NULL)
//				extract_obj(obj);
//
//		}

	}
if(IS_CSTRUCTAFF(victim,CONSTRUCT_ESHIELD)){
    int eshield = victim->mana;
    int dam2 = dam - eshield;
    int dte = eshield - dam;
     if (dte >= 1){
     //eshieldmessage block
     act( "Your energy shield blocks $n's attack.", ch, NULL, victim, TO_VICT );
     act( "%N's energy shield blocks $n's attack.",  ch, NULL, victim, TO_NOTVICT );
     act( "$N's energy shield blocks your attack.",  ch, NULL, victim, TO_CHAR );
     	REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
     	victim->mana -= dam;
     	return -1;
	 }else if (dte == 0){
	 //eshieldmessage block and break
	 act( "Your energy shield shatters as it blocks $n's attack.", ch, NULL, victim, TO_VICT );
     act( "%N's energy shield shatters as it blocks $n's attack.",  ch, NULL, victim, TO_NOTVICT );
     act( "$N's energy shield shatters as it blocks your attack.",  ch, NULL, victim, TO_CHAR );
     do_eshield(victim,"");
     REMOVE_CSTRUCTAFF(victim,CONSTRUCT_ESHIELD);
        REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
        victim->mana = 0;
        return -1;
     }else{
     dam = dam2;
     //eshieldmessage break and overcome
     act( "Your energy shield shatters under $n's attack.", ch, NULL, victim, TO_VICT );
     act( "%N's energy shield shatters under $n's attack.",  ch, NULL, victim, TO_NOTVICT );
     act( "$N's energy shield shatters under your attack.",  ch, NULL, victim, TO_CHAR );
     do_eshield(victim,"");
     REMOVE_CSTRUCTAFF(victim,CONSTRUCT_ESHIELD);
     victim->mana = 0;
     }
   }

	// scale the damage
	if(!GAMESETTING3(GAMESET3_EXPERIMENTAL_DAMAGE_SCALING)){
		// old damage scaling system originating from oblivion
		// too complicated for its own good.
		dam=dam*3/2;
		switch(UMIN(number_range(1,20),number_range(1,20)))
		{
		default :
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:  dam = dam / (victim->level*2/5+1) ;  break ;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
			if(ch->level>victim->level){
				dam=dam * (ch->level-victim->level+5)/5;
			}
			dam = dam / (victim->level*2/5+1);
			break;
		case 14:
		case 15:
		case 16:
		case 17:
			if(ch->level*11/10>victim->level){
				dam=dam * (ch->level*11/10-victim->level+5)/5;
			}
			dam = dam / (victim->level*2/5+1);
			break;
		case 18:
		case 19:
			if(ch->level*12/10>victim->level){
				dam=dam * (ch->level*12/10-victim->level+5)/5;
			}
			dam = dam / (victim->level*2/5+1);
			break;
		case 20:
			if(ch->level*15/10>victim->level){
				dam=dam * (ch->level*15/10-victim->level+5)/5;
			}
			dam = dam / (victim->level*2/5+1);
			break;
		}
	}else{
		// increase/decrease the damage_scale_value
		// we will use by upto 50%
		int scale=game_settings->damage_scale_value;
		if(number_range(1,2)==1){
			scale+=number_range(1,number_range(1,scale/2));
		}else{
			scale-=number_range(1,number_range(1,scale/2));
		}
		scale=URANGE(100, scale, 4000);
		// apply the damage scale value
		scale=(victim->level * scale / 10000);
		if(scale){
			dam/=scale;
		}
	}

    if ( victim != ch )
    {
		//	Certain attacks are forbidden.
		//	Most other attacks are returned - set victim to fighting
		if ( is_safe( ch, victim ) ){
			REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
			return -1;
		}

		if ( victim->position > POS_STUNNED )
		{
			if ( ch->fighting == NULL ){
				set_fighting( ch, victim );
			}

			if ( victim->fighting == NULL ){ // moved from above
				set_fighting( victim, ch );
				if ( IS_NPC( victim ) && HAS_TRIGGER( victim, MTRIG_KILL )){
					mp_percent_trigger( victim, ch, NULL, NULL, MTRIG_KILL );
					if(!IS_VALID(victim)){  // if the mob purged itself
						REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
						return -1;
					}
				}
			}
			if (victim->timer <= 4 && (!IS_SET(victim->act2,PLR2_TBC) && !IS_SET(ch->act2,PLR2_TBC))){
				victim->position = POS_FIGHTING;
			}


			if ( IS_NPC(ch)
				&&   IS_NPC(victim)
				&&   IS_AFFECTED(victim, AFF_CHARM)
				&&   victim->master != NULL
				&&   victim->master->in_room == ch->in_room
				&&	 number_range(0, 7 ) == 0 )
			{
				stop_fighting( ch, false ); // Damage
				multi_hit( ch, victim->master, TYPE_UNDEFINED );
				REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
				return -1;
			}
		}


		// More charm stuff.
		if ( victim->master == ch ){
			stop_follower( victim );
		}
    }


	// Inviso attacks ... not.

	if ( IS_SET( ch->affected_by2, AFF2_TREEFORM ))
	{
		REMOVE_BIT( ch->affected_by2, AFF2_TREEFORM );
		affect_parentspellfunc_strip( ch, gsn_treeform );
		act( "A tree suddenly transforms into $n.", ch, NULL, NULL, TO_ROOM );
		ch->println( "You assume your normal form." );
	}

	if ( IS_SET( ch->affected_by2, AFF2_VANISH ))
	{
		REMOVE_BIT( ch->affected_by2, AFF2_VANISH );
		affect_parentspellfunc_strip( ch, gsn_vanish );
		act( "A swirl of dust reveals $n.", ch, NULL, NULL, TO_ROOM );
		ch->println( "You shake the `#`Bfaerie-dust`^ from your body." );
	}

	if ( IS_AFFECTED(ch, AFF_INVISIBLE) )
	{
		affect_parentspellfunc_strip( ch, gsn_invisibility );
		affect_parentspellfunc_strip( ch, gsn_mass_invis );
		REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
		act( "$n fades into existence.", ch, NULL, NULL, TO_ROOM );
	}

	if (    IS_SET( ch->affected_by, AFF_HIDE)
		|| IS_SET( ch->affected_by, AFF_SNEAK))
	{
		REMOVE_BIT( ch->affected_by, AFF_HIDE      );
		REMOVE_BIT( ch->affected_by, AFF_SNEAK     );
		act( "$n is revealed from the shadows.", ch, NULL, NULL, TO_ROOM );
	}

	// Set Mob Remember
	/*
	if ( victim->mobmemory ) {
	victim->mobmemory = ch;
	}
	else
	mobRememberSet( victim, ch );
	*/
	// Damage modifiers.

	if ( dam > 1 && !IS_NPC(victim)
		&&   victim->pcdata->condition[COND_DRUNK]  > 10 ){
		dam = 9 * dam / 10;
	}

	if ( dam > 1 && IS_AFFECTED(victim, AFF_SANCTUARY) ){
		dam /= 2;
	}

	// subdue modifications - make damage 60-90% of what it would have been
    if (IS_SET(ch->dyn,DYN_CURRENT_SUBDUE))
    {
		dam = dam * number_range(60,90) /100;
    }

	// protect_(good)|(evil) can reduce the damage to 3 quarters
    if ( dam > 1 && ((IS_AFFECTED(victim, AFF_PROTECT_EVIL) && IS_EVIL(ch) )
		||               (IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) )))
		dam -= dam / 4;

	if(num_enemies(victim)>num_enemies(ch))
		dam = dam + dam * (num_enemies(victim) - num_enemies(ch))/2;

	// Check for parry, dodge and fade
    if ( dt >= TYPE_HIT && ch != victim)
    {
		if ( check_parry( ch, victim ) ){
			REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
			return -1;
		}
		if ( check_dodge( ch, victim ) ){
			REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
			return -1;
		}
		if ( check_fade( ch, victim ) ){
			REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
			return -1;
		}
		if ( check_shield_block(ch,victim)){
			REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
			return -1;
		}
	}

	// apply immunities, resistance and vulnerabilities
	switch(check_immune(victim,dam_type))
	{
	case(IS_IMMUNE):
		dam = 0;
		break;
	case(IS_RESISTANT):
		dam -= dam/2;
		break;
	case(IS_VULNERABLE):
		dam += dam;
		break;
	}
    if (!IS_IMMORTAL(ch)){
	if ( dam > 999 && ch->level <= 92 && !IS_HERO(ch) && !IS_NPC(ch)){
	dam = 999;}
    }

	// dam now has the amount of damage which will be inflicted, it should be positive or 0
	assert(dam>=0);

	return dam;
}
/**************************************************************************/
void damage_display_infliction_message(char_data *ch, char_data *victim,int dam,int dt,int dam_type)
{
	assert(IS_VALID(victim) && victim->position != POS_DEAD );

	bool immune=false;
	if(dam==0 && check_immune(victim,dam_type)==IS_IMMUNE){
		immune=true;
	}
	dam_message( ch, victim, dam, dt, immune );
}
/**************************************************************************/

// - damage_inflict applies dam calculated by damage_calculate_infliction()
// *** SHOULD NEVER be called with any value other than what is output
//     from damage_calculate_infliction()
// - removes DYN_DOING_DAMAGE
// - typically called after damage_display_infliction_message()... unless
//   a custom message is used.
void damage_inflict(char_data *ch,char_data *victim,int dam,int dt,int dam_type)
{
	int			i;
	char_data	*rch;
	char_data	*rch_next = NULL;

	assert(IS_VALID(victim) && victim->position != POS_DEAD );

    if(dam == 0){
		REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
		return;
	}

	int vlev = victim->level;
	int alev = ch->level;
	if ((vlev -= alev) >= 11){
		dam = 0;
	}

	if ( dam > 999 && ch->level <= 92 && !IS_HERO(ch) && !IS_NPC(ch))
    {
	dam = 999;
    }

	//	hurt the victim.
	victim->hit -= dam;
	if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch))
	{
	if (victim->hit <= 0){
//	victim->hit=0;
//	if (victim->position!=POS_STANDING){
//	victim->position=POS_STANDING;}
	kill_unit (victim);
//	ch->println("Death!");
//    return;
    }
    }

	// if we did damage, record if the level of the attacker is higher than
	// the previously highest recorded attacker.  This information is used
	// when allocating group xp, the amount of XP awarded reduces based on
	// if you are more than 8 levels below the highest attacker's level
	// (the lower you are from that level, the less you get)
	if(dam>0){
		victim->highest_level_to_do_damage=
			UMAX(victim->highest_level_to_do_damage, ch->level);
	}

	//	inform the victim of their new state.
    update_pos( victim );

	// if the victim is riding a mount, check if they are knocked off it
	if(victim->mounted_on){
		long percent_of_max_hits;
		percent_of_max_hits=dam*100;
		percent_of_max_hits/=victim->max_hit;

		if(percent_of_max_hits>=10){
			if(!IS_NPC(victim)) // player
			{
				if ((number_range(1, 100)) <
					(105-get_skill(victim, gsn_riding))	*
					(percent_of_max_hits-9))
				{
					act("You are knocked from your mount!\r\n", victim, NULL, NULL, TO_CHAR);
					act("$n is knocked from $s mount!\r\n", victim, NULL, NULL, TO_ROOM);
					dismount(victim);
					check_improve(victim, gsn_riding, false, 1);
				}
			}else{ // mob
				if (number_range(1, 100)+((percent_of_max_hits-10)*3)>80){
					act("You are knocked from your mount!\r\n", victim, NULL, NULL, TO_CHAR);
					act("$n is knocked from %s mount!\r\n", victim, NULL, NULL, TO_ROOM);
					dismount(victim);
				}
			}
		}
	}

	if(dam>1)
	{
		if(is_affected(victim,gsn_fire_shield) && dt!=gsn_fire_shield){
			damage(victim,ch,UMIN(ch->max_hit,dam/2),gsn_fire_shield,DAM_FIRE,true);
		}
		if(is_affected(victim,gsn_chill_shield) && dt!=gsn_chill_shield){
			damage(victim,ch,UMIN(ch->max_hit,dam/2),gsn_chill_shield,DAM_COLD,true);
		}
//		if(is_affected(victim,gsn_fiery_armor) && dt!=gsn_fiery_armor){
//			damage(victim,ch,UMIN(ch->max_hit,dam/2),gsn_fiery_armor,DAM_FIRE,true);
//		}
//		if(is_affected(victim,gsn_icy_armor) && dt!=gsn_icy_armor){
//			damage(victim,ch,UMIN(ch->max_hit,dam/2),gsn_icy_armor,DAM_COLD,true);
//		}
	}

	if(100 * dam / victim->max_hit > (i = number_percent())){
		bash_eq(victim,i);
	}

    update_pos(ch);

	// NO ADDITIONAL DAMAGE SHOULD BE DONE BELOW HERE,
	// AS CODE BELOW HANDLES DEATH ETC

	// Subdue System - Kalahn June 97, updated October 98
    if (IS_SET(ch->dyn,DYN_CURRENT_SUBDUE) && !IS_AWAKE(victim)
		&& (  IS_IMMORTAL(ch)
		|| (number_percent()< UMAX(15,101-(GET_HITROLL(ch)/2)))
		)
		)
    {
		act( "$n appears to be subdued.", victim, NULL, NULL, TO_ROOM );
		victim->println("You have been subdued.");

		if(!IS_NPC(victim)){
			group_bypass_killer_penatly(ch, victim, false);

			// update duels to protect victim
			duel_protect_victim(victim);
		}

		stop_fighting( victim, true); // Damage - subdued

		group_gain( ch, victim );

		victim->subdued= true;
		victim->subdued_timer = number_range(10,20);
		victim->is_stunned = ch->level;
		victim->position=POS_STUNNED;

		if (IS_NPC(victim))
		{
			victim->no_xp = true;
			victim->pIndexData->count--;
			// no more exchanging money with a weak money changer and then
			// subduing the mob and getting your silver back.
			if (IS_SET(victim->act,ACT_IS_CHANGER)){
				victim->gold=	number_range(1,UMIN(3,victim->gold));
				victim->silver= number_range(1,UMIN(350,victim->silver));
			}
		}

		sprintf( log_buf, "%s subdued by %s at %d",
			victim->name,
			(IS_NPC(ch) ? ch->short_descr : ch->name),
			ch->in_room->vnum );
		log_string( log_buf );

		wiznet(log_buf,NULL,NULL,WIZ_BETA,0,0);

		if(GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
			pkill_broadcast("%s", log_buf);
		}else{
			if (!IS_NPC(victim))
			{
				sprintf( log_buf, "%s[%d] subdued by %s[%d] at %d",
					victim->name, victim->level,
					ch->name, ch->level,
					ch->in_room->vnum );
				wiznet(log_buf,NULL,NULL,WIZ_DEATHS,0,0);
			}
		}
    }
    else
    {
		switch( victim->position )
		{
		case POS_MORTAL:
			act( "$n is mortally wounded, and will die soon, if not aided.",
				victim, NULL, NULL, TO_ROOM );
			victim->println("You are mortally wounded, and will die soon, if not aided.");
			break;

		case POS_INCAP:
			act( "$n is incapacitated and will slowly die, if not aided.",
				victim, NULL, NULL, TO_ROOM );
			victim->println("You are incapacitated and will slowly die, if not aided.");
			break;

		case POS_STUNNED:
			act( "$n is stunned, but will probably recover.",
				victim, NULL, NULL, TO_ROOM );
			victim->println("You are stunned, but will probably recover.");
			break;

		case POS_DEAD:
			// Death trigger mudprog trigger
			act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
			victim->println("You have been `rKILLED!!`x");
			if(IS_AFFECTED3(victim, AFF3_PREGNANT)){
			victim->println("You have lost your child due to the death!!`x");
					REMOVE_BIT(victim->affected_by3, AFF3_PREGNANT);}
			break;

		default:
			if ( dam > victim->max_hit / 4 )
			{
				victim->println("`YThat really did `rHURT!`x");
			}
			if ( victim->hit < victim->max_hit / 4 )
			{
				victim->println("`YYou sure are `rBLEEDING!`x");
			}
			break;
		}

		// Sleep spells and extremely wounded folks.
		if ( !IS_AWAKE(victim) ){
			stop_fighting( victim, false ); // Damage - not awake victim
		}

		// Payoff for killing things.
		if ( victim->position == POS_DEAD )
		{
			int number_in_room=0;
			// ** get rch the safe way
			// This code is necessary because a mob (ch) can trigger on
			// on position acts ("is DEAD!!" etc)... and purge itself
			rch= NULL;
			if(IS_VALID(ch) && ch->in_room){
				rch = ch->in_room->people;
				number_in_room=ch->in_room->number_in_room;
			}else if(IS_VALID(victim) && victim->in_room){
				rch = victim->in_room->people;
				number_in_room=victim->in_room->number_in_room;
			}
			if(!rch){
				logf("Not testing for roomdeath trigger cause victim room and ch room couldn't be found.");
			}

			// ** Check for the roomdeath triggers
			// note: number_in_room is used to prevent a mudprog on two mobs in
			// a room creating an endless loop by removing themselves from
			// the room and putting themselves back in the room - Kal, June 01
			for ( ; rch && --number_in_room>=0; rch = rch_next )
			{
				rch_next = rch->next_in_room;

				if ( IS_NPC( rch ))
				{
					if ( HAS_TRIGGER( rch, MTRIG_ROOMDEATH )
						&&	 can_see( rch, ch )
						&& rch!=victim)
					{
						logf("Running roomdeath trigger prog on mob '%s' %d",
							PERS(rch, NULL), rch->pIndexData?rch->pIndexData->vnum:0);
						mp_percent_trigger( rch, ch, NULL, NULL, MTRIG_ROOMDEATH );
					}
				}
			}
			kill_char(victim, ch);
			REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
			return;
		}

		if ( victim == ch ){
			REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
			return;
		}

		// Take care of link dead people.
		if ( !IS_NPC(victim) && victim->desc == NULL )
		{
			if ( number_range( 0, victim->wait ) == 0
				&& HAS_CONFIG(victim, CONFIG_AUTORECALL))
			{
				do_recall( victim, "" );
				REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
				return;
			}
		}

		// Wimp out?
		if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
		{
			if ( ( IS_SET(victim->act, ACT_WIMPY) && number_range(0, 3 ) == 0
				&&   victim->hit < victim->max_hit / 5)
				||   ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
				&&     victim->master->in_room != victim->in_room ) )
				do_flee( victim, "" );
		}

		if ( !IS_NPC(victim)
			&&   victim->hit > 0
			&&   victim->hit <= victim->wimpy
			&&   victim->wait < PULSE_VIOLENCE / 2 )
			do_flee( victim, "" );

		if ( !IS_NPC(victim)
			&&   victim->hit > 0
			&&   victim->hit <= victim->pcdata->panic
			&&   victim->wait < PULSE_VIOLENCE / 2 )
		{
			victim->println("P A N I C ! ! !");
			do_recall( victim, "" );
		}
    } // end of subdue else

	REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
}
/**************************************************************************/
int damage(char_data *ch,char_data *victim,int dam,int dt,int dam_type,bool show)
{
	int damage_to_inflict=damage_calculate_infliction(ch,victim,dam,dt,dam_type);

	if(damage_to_inflict>=0){
		if(show){
			damage_display_infliction_message(ch, victim,damage_to_inflict,dt,dam_type);
		}
		damage_inflict(ch, victim, damage_to_inflict, dt, dam_type);
	}
	if (IS_SET(victim->psionic_affects, PAFF_TKSHIELD)){
		dam = dam/2;
		check_improve(victim, gsn_tk_shield, true, 1);}
	return damage_to_inflict;
}

/**************************************************************************/
// spells call this to inflict damage
// (dt = sn)
int damage_spell( char_data *ch, char_data *victim, int dam, int dt,
		int dam_type, bool show )
{
//	char buf[MSL];

	if( get_skill(ch, gsn_magic_mastery ) )
	{
		int chance = get_skill(ch,gsn_magic_mastery);
		if( number_percent() < chance / 3 )
		{
			dam += number_range(chance / 3,chance / 2);
			check_improve(ch,gsn_magic_mastery,true,2);
		}
	}
	if (get_skill (ch, skill_lookup ("damage plus")) > 0 &&
		is_linked_skill (ch, dt, skill_lookup ("damage plus"))){
		dam = dam * 3 / 2;
	}
	if (IS_SET(victim->psionic_affects, PAFF_ANTIMAGIC)){
		dam = dam/2;
		check_improve(victim, gsn_antimagic, true, 1);}
	int damount=damage( ch, victim, dam, dt, dam_type, show );

	// code below in for monitoring reasons to fix up the AC system
/*	sprintf(buf,"%3d, %3d, %3d, \"%s\", \"%s\", %3d, \"%s\", %3d, %d",
		dt, dam, dam_type, skill_table[dt].name, ch->name, ch->level,
		victim->name, victim->level, damount);
	append_string_to_file( SPELL_DAMAGE_LOG,buf);
*/

	return (damount);
}
/**************************************************************************/
// for auto assisting
// victim is being attacked by ch... will rch join in
void check_assist(char_data *ch,char_data *victim)
{
	char_data *rch, *rch_next;

	for (rch = ch->in_room->people; rch != NULL; rch = rch_next)
	{
		rch_next = rch->next_in_room;

		if (IS_AWAKE(rch) && rch->fighting == NULL)
		{
			// quick check for ASSIST_PLAYER
			if (!IS_NPC(ch) && IS_NPC(rch)
				&& IS_SET(rch->off_flags,ASSIST_PLAYERS)
				&&  rch->level + 6 > victim->level)
			{
				do_emote(rch,"screams and attacks!");
				multi_hit(rch,victim,TYPE_UNDEFINED);
				continue;
			}

			// PCs next
			if (!IS_NPC(ch) || IS_AFFECTED(ch,AFF_CHARM))
			{
				if(	!is_same_group(ch,rch)
					|| !can_join_combat(rch, victim)
					|| is_safe(rch, victim)){
					continue;
				}

				if(IS_NPC(victim)){
					if(!IS_AFFECTED(rch,AFF_CHARM)
						&& !(!IS_NPC(rch) && IS_SET(rch->act,PLR_AUTOASSIST))){
						continue;
					}
				}else{
					if(IS_NPC(rch)){
						if(!IS_AFFECTED(rch,AFF_CHARM)){
							continue;
						}
					}else{
						if(!IS_SET(rch->dyn,DYN_AUTOPKASSIST)){
							rch->println("`YYou don't automatically assist because you don't have autopkassist is off.`x");
							continue;
						}
					}
				}
				// player attacking another player
				if (!IS_NPC(rch) && !IS_NPC(victim))
				{
					logf( "%s attacking %s at %d (check_assist()(charm) might start pkill)",
						rch->name,
						victim->name,
						rch->in_room->vnum );
				}
				multi_hit (rch,victim,TYPE_UNDEFINED);
				continue;
			}

			//for horse attacking characters
			if (victim->fighting && victim->fighting==rch->mounted_on)
			{
				// player attacking another player
				if (!IS_NPC(rch) && !IS_NPC(victim))
				{
					logf( "%s attacking %s at %d (check_assist() might start pkill)",
						rch->name,
						victim->name,
						rch->in_room->vnum );
				}

				multi_hit(rch, victim, TYPE_UNDEFINED);
				continue;
			}

			// now check the NPC cases
			if (IS_NPC(ch) && !IS_AFFECTED(ch,AFF_CHARM))
			{
				if ( (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALL))
					||   (IS_NPC(rch) && rch->helpgroup && rch->helpgroup == ch->group)
					||   (IS_NPC(rch) && rch->race == ch->race
					&& IS_SET(rch->off_flags,ASSIST_RACE))
					||   (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALIGN)
					&&   ((IS_GOOD(rch)    && IS_GOOD(ch))
					||  (IS_EVIL(rch)    && IS_EVIL(ch))
					||  (IS_NEUTRAL(rch) && IS_NEUTRAL(ch))))
					||   (rch->pIndexData == ch->pIndexData
					&& IS_SET(rch->off_flags,ASSIST_VNUM)))

				{
					char_data *vch;
					char_data *target;
					int number;

					if (number_range(0,1) == 0)
						continue;

					target = NULL;
					number = 0;
					for (vch = ch->in_room->people; vch; vch = vch->next_in_room)
					{
						if (can_see(rch,vch)
							&&  is_same_group(vch,victim)
							&&  number_range(0,number) == 0)
						{
							target = vch;
							number++;
						}
					}

					if (target != NULL)
					{
						do_emote(rch,"screams and attacks!");
						multi_hit(rch,target,TYPE_UNDEFINED);
					}
				}
			}
		}
	}
}


/**************************************************************************/
// Do one group of attacks.
void multi_hit( char_data *ch, char_data *victim, int dt )
{
	int chance;

	if(IS_MOUNTED(ch))
	{
		ch->println("Cant do that.");
		return;
	}

	if(!IS_NPC(ch) && !IS_NPC(victim) && ch!=victim)
	{

		// transfer those to the void that are attacked
		// while linkdead and don't have a pkill timer
		if ( victim->pknorecall ==0
			&& IS_LINKDEAD(victim)
			&& victim->was_in_room == NULL
			&& victim->in_room != NULL )
		{
			victim->was_in_room = victim->in_room;
			if ( victim->fighting != NULL ){
				stop_fighting( victim, true ); // Multi_hit linkdead transfer to void
			}
			act( "$n disappears into the void.", victim, NULL, NULL, TO_ROOM );
			victim->println("You disappear into the void.");
			if (victim->level > 1)
				save_char_obj( victim);
			char_from_room( victim);
			char_to_room( victim, get_room_index( ROOM_VNUM_LIMBO ) );
			return;
		}

		ch->pknorecall=15;
		victim->pknorecall=15;

		if(GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
			ch->pknoquit	=2;
			victim->pknoquit=2;
		}else{
			ch->pknoquit	=20;
			victim->pknoquit=20;
		}
	}
	// decrement the wait
	if (ch->desc == NULL)
		ch->wait = UMAX(0,ch->wait - PULSE_VIOLENCE);

	if (ch->desc == NULL)
		ch->daze = UMAX(0,ch->daze - PULSE_VIOLENCE);


	// no attacks for stunnies -- just a check
	if (ch->position == POS_STUNNED
		|| ch->position == POS_SLEEPING
		|| ch->position == POS_INCAP
		|| ch->position == POS_MORTAL
		|| ch->position == POS_DEAD){
		ch->println("Too stunned for that!");
		return;}

	if (IS_NPC(ch))
	{
		mob_hit(ch,victim,dt);
		return;
	}

	one_hit( ch, victim, dt, false );

	if (ch->fighting != victim)
		return;

	if (IS_AFFECTED(ch,AFF_HASTE))
		one_hit(ch,victim,dt,false);

	// extra attack if defending your mount
	if (victim->fighting==ch->mounted_on)
		one_hit(ch,victim,dt,false);

	if (get_eq_char (ch, WEAR_SECONDARY))
	{
		if(GAMESETTING2(GAMESET2_NO_SECOND_SKILL_REQUIRED)){
			if(number_range(ch->get_skill(gsn_second), 100)>90){
				check_improve(ch,gsn_second,true,5);
				one_hit( ch, victim, dt, true );
				if ( ch->fighting != victim ){
					return;
				}
			}else{
				check_improve(ch,gsn_second,false,1);
			}
		}else{
			if(number_range(ch->get_skill(gsn_second), 100)>90){
				check_improve(ch,gsn_second,true,5);
				one_hit( ch, victim, dt, true );
				if ( ch->fighting != victim ){
					return;
				}
			}else{
				check_improve(ch,gsn_second,false,1);
			}
		}
	}

	if ( ch->fighting != victim || dt == gsn_backstab )
		return;

    chance = get_skill(ch,gsn_second_attack)/2;

    if (IS_AFFECTED(ch,AFF_SLOW))
		chance /= 2;

	if ( number_percent( ) < chance )
	{
		one_hit( ch, victim, dt, false );
		check_improve(ch,gsn_second_attack,true,5);
		if ( ch->fighting != victim )
			return;
	}

	chance = get_skill(ch,gsn_third_attack)/3;

	if (IS_AFFECTED(ch,AFF_SLOW))
		chance = 0;

	if ( number_percent( ) < chance )
	{
		one_hit( ch, victim, dt, false );
		check_improve(ch,gsn_third_attack,true,6);
		if ( ch->fighting != victim )
			return;
	}

	chance = get_skill(ch,gsn_quad_attack)/2;

	if (IS_AFFECTED(ch,AFF_SLOW))
		chance = 0;

	if ( number_percent( ) < chance )
	{
		one_hit( ch, victim, dt, false );
		check_improve(ch,gsn_quad_attack,true,7);
		if ( ch->fighting != victim )
			return;
	}
	return;
}

/**************************************************************************/
// procedure for all mobile attacks
void mob_hit (char_data *ch, char_data *victim, int dt)
{
    int chance,number;
    char_data *vch, *vch_next;

	one_hit(ch,victim,dt,false);

    if (ch->fighting != victim)
		return;

	// Area attack -- BALLS nasty!

	// mobs get the extra attack too if defending mount
	if (victim->fighting==ch->mounted_on)
		one_hit(ch,victim,dt,false);

    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK))
    {
		for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
		{
			vch_next = vch->next_in_room;
			if ((vch != victim && vch->fighting == ch))
				one_hit(ch,vch,dt,false);
		}
	}

	if (IS_AFFECTED(ch,AFF_HASTE)
		||  (IS_SET(ch->off_flags,OFF_FAST) && !IS_AFFECTED(ch,AFF_SLOW)))
		one_hit(ch,victim,dt,false);

    if (ch->fighting != victim || dt == gsn_backstab)
		return;

	chance = get_skill(ch,gsn_second_attack)/2;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))
		chance /= 2;

    if (number_percent() < chance)
	{
		one_hit(ch,victim,dt,false);
		if (ch->fighting != victim)
			return;
    }

	chance = get_skill(ch,gsn_third_attack)/4;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))
		chance = 0;

    if (number_percent() < chance)
    {
		one_hit(ch,victim,dt,false);
		if (ch->fighting != victim)
			return;
	}

	// oh boy!  Fun stuff!
    if (ch->wait > 0)
		return;

    number = number_range(0,2);

    if (number == 1 && IS_SET(ch->act,ACT_MAGE)){
		spec_cast_mage(ch);
		return;
    }

    if (number == 2 && IS_SET(ch->act,ACT_CLERIC)){
		spec_cast_cleric(ch);
		return;
    }

    // now for the skills
	number = number_range(0,9);

    switch(number)
    {
    case (0) :
		if (IS_SET(ch->off_flags,OFF_BASH))
			do_bash(ch,"");
		break;

	case (1) :
		if (IS_SET(ch->off_flags,OFF_BERSERK) && !IS_AFFECTED(ch,AFF_BERSERK))
			do_berserk(ch,"");
		break;


    case (2) :
		if (IS_SET(ch->off_flags,OFF_DISARM)
			|| (get_weapon_sn(ch) != gsn_hand_to_hand
			&& (IS_SET(ch->act,ACT_WARRIOR)
			||  IS_SET(ch->act,ACT_THIEF))))
			do_disarm(ch,"");
		break;

	case (3) :
		if (IS_SET(ch->off_flags,OFF_KICK))
			do_kick(ch,"");
		break;

    case (4) :
		if (IS_SET(ch->off_flags,OFF_KICK_DIRT))
			do_dirt(ch,"");
		break;

	case (5) :
		if (IS_SET(ch->off_flags,OFF_CIRCLE))
		{
			 do_circle(ch,"");
		}
		break;

    case (6) :
		if (IS_SET(ch->off_flags,OFF_TRIP))
			do_trip(ch,"");
		break;

    case (7) :
		if (IS_SET(ch->off_flags,OFF_CRUSH))
		{
			/* do_crush(ch,"") */ ;
		}
		break;
    case (8) :
		if (IS_SET(ch->off_flags,OFF_BACKSTAB))
		{
			do_pbackstab(ch,"");
		}
    case (9) :
        if ( IS_SET( ch->off_flags, OFF_GORE ))
			do_gore( ch, "" );
        break;
    }
}


/**************************************************************************/
// Hit one guy once.
void one_hit( char_data *ch, char_data *victim, int dt, bool second )
{
	OBJ_DATA *wield;
	int victim_ac;
	int thac0;
	int thac0_00;
	int thac0_32;
	int dam;
	int diceroll;
	int sn,skill;
	int dam_type;
	int result;

	sn = -1;

	// just in case
	if (victim == ch
		|| ch == NULL		|| !IS_VALID(ch)
		|| victim == NULL	|| !IS_VALID(victim))
	{
		return;
	}

	// Can't beat a dead char! and guard against weird room-leavings.
	if ( victim->position == POS_DEAD || ch->in_room != victim->in_room )
		return;

	OBJ_DATA *primary_weapon= get_eq_char( ch, WEAR_WIELD );
	OBJ_DATA *secondary_weapon= get_eq_char( ch, WEAR_SECONDARY);

	// Figure out the type of damage message, first find the weapon
	wield = second?secondary_weapon:primary_weapon;

	if ( dt == TYPE_UNDEFINED )
	{
		dt = TYPE_HIT;
		if ( wield != NULL && wield->item_type == ITEM_WEAPON ){
			dt += wield->value[3];
		}else{
			dt += ch->dam_type;
		}
	}

	if (dt < TYPE_HIT){
		if (wield != NULL){
			dam_type = attack_table[wield->value[3]].damage;
		}else{
			dam_type = attack_table[ch->dam_type].damage;
		}
	}else{
		dam_type = attack_table[dt - TYPE_HIT].damage;
	}

	if (dam_type == -1){
		dam_type = DAM_BASH;
	}

	// get the weapon skill
	if(second){ // swap over the secondary and primary to get sn
		if(primary_weapon){
			primary_weapon->wear_loc = WEAR_SECONDARY;
		}
		secondary_weapon->wear_loc = WEAR_WIELD;
		sn = get_weapon_sn(ch);
		if(primary_weapon){
			primary_weapon->wear_loc = WEAR_WIELD;
		}
		secondary_weapon->wear_loc = WEAR_SECONDARY;
	}else{
		sn = get_weapon_sn(ch);
	}
	if( ch->clss == class_lookup("citizen")){
		skill = 65;
	}else{
		skill = 20 + get_weapon_skill(ch,sn);
	}

	// bonus skill percentage for those with relevant mastery
	int arms_mastery_bonus=get_skill( ch, gsn_arms_mastery )/ 5;
	int mastery_bonus=0;
	if(sn == gsn_sword
		&& get_skill( ch, gsn_sword_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_sword_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_dagger
		&& get_skill( ch, gsn_dagger_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_dagger_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_flail
		&& get_skill( ch, gsn_flail_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_flail_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_mace
		&& get_skill( ch, gsn_mace_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_mace_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_polearm
		&& get_skill( ch, gsn_polearm_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_polearm_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_staff
		&& get_skill( ch, gsn_staff_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_staff_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_whip
		&& get_skill( ch, gsn_whip_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_whip_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_spear
		&& get_skill( ch, gsn_spear_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_spear_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_sickle
		&& get_skill( ch, gsn_sickle_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_sickle_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_lightblade
		&& get_skill( ch, gsn_lightblade_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_lightblade_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_bow
		&& get_skill( ch, gsn_bow_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_bow_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_crossbow
		&& get_skill( ch, gsn_crossbow_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_crossbow_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_unarmed
		&& get_skill( ch, gsn_unarmed_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_unarmed_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_pistol
		&& get_skill( ch, gsn_pistol_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_pistol_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_rifle
		&& get_skill( ch, gsn_rifle_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_rifle_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_blastpistol
		&& get_skill( ch, gsn_blastpistol_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_blastpistol_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_blastrifle
		&& get_skill( ch, gsn_blastrifle_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_blastrifle_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}
	skill+=arms_mastery_bonus;

	if(second){
		if(!GAMESETTING2(GAMESET2_NO_SECOND_SKILL_REQUIRED)){
			skill*=(ch->get_skill(gsn_second)/100);
		}
	}

	if ( get_eq_char( ch, WEAR_LODGED_ARM )){
		skill -= 20;
	}

	if(HAS_CONFIG(ch,CONFIG_PRACSYS_TESTER)){
		skill+=20;
	}

	/*Calculate to-hit-armor-clss-0 versus armor.*/
	if ( IS_NPC(ch) )
	{
		thac0_00 = 20;
		thac0_32 = -4;   // as good as a thief
		if (IS_SET(ch->act,ACT_WARRIOR))
			thac0_32 = -10;
		else if (IS_SET(ch->act,ACT_THIEF))
			thac0_32 = -4;
		else if (IS_SET(ch->act,ACT_CLERIC))
			thac0_32 = 2;
		else if (IS_SET(ch->act,ACT_MAGE))
			thac0_32 = 6;
	}
	else
	{
		thac0_00 = class_table[ch->clss].thac0_00;
		thac0_32 = class_table[ch->clss].thac0_32;
	}
	thac0  = interpolate( ch->level, thac0_00, thac0_32 );

	if (thac0 < 0)
		thac0 = thac0/2;

	if (thac0 < -5)
		thac0 = -5 + (thac0 + 5) / 2;

    int hroll= GET_HITROLL(ch);
    if (ch->symbiotetype != 0){
        hroll += ch->symbiotehr;}

	thac0 -= hroll * skill/100;
//	thac0 -= GET_HITROLL(ch) * skill/100;
	if(!IS_NPC(ch)){
		// increase affect to hitroll for low pc newbies
		if(ch->level<6){
			thac0-= (6-ch->level)*2;
		}
	}else{
		// decrease hitroll on low mobs when fighting newbies
		if(ch->level<6 && victim->level<6){
			thac0+= (6-victim->level)*2;
		}
	}

	// make faerie fire do something
	if(count_affected_by_base_spell( victim, gsn_faerie_fire )>0){
		thac0-=number_range(1,3);
	}

	// make the likelyhood of impact less if they are lacking in skill
	thac0 += 5 * (100 - skill) / 100;

	switch(dam_type)
	{
	case(DAM_PIERCE):victim_ac = GET_AC(victim,AC_PIERCE)/10;       break;
	case(DAM_BASH):  victim_ac = GET_AC(victim,AC_BASH)/10;         break;
	case(DAM_SLASH): victim_ac = GET_AC(victim,AC_SLASH)/10;        break;
	default:         victim_ac = GET_AC(victim,AC_EXOTIC)/10;       break;
	};

	if (victim_ac < -15)
		victim_ac = (victim_ac + 15) / 5 - 15;

	if(victim->position==POS_FIGHTING)
		victim_ac -= victim->modifiers[STAT_QU]/5;

	if ( !can_see( ch, victim ) )
		victim_ac -= 4;

	if ( victim->position < POS_FIGHTING)
		victim_ac += 4;

	if (victim->position < POS_RESTING)
		victim_ac += 6;

    if (victim->symbiotetype != 0)
        victim_ac -= ch->symbioteac;


	// The moment of excitement!
	diceroll=number_range(0,19);

/*	{
	// some debugging code to get an understanding of what is actually happening
		int val=thac0 - victim_ac + num_enemies(ch) - num_enemies(victim);
ch->printlnf    ("ATTACKER=== thac0=%d, victim_ac = %d, num_enemies(ch)=%d, num_enemies(victim)=%d, dice=%d, val=%d",
			 thac0, victim_ac, num_enemies(ch), num_enemies(victim), diceroll, val);
victim->printlnf("VICTIM  === thac0=%d, victim_ac = %d, num_enemies(ch)=%d, num_enemies(victim)=%d, dice=%d, val=%d",
			 thac0, victim_ac, num_enemies(ch), num_enemies(victim), diceroll, val);
	}
*/
	if(diceroll){
		// the above check guarantees a hit (1 in 20) - when diceroll==0
		if ( diceroll == 19 // guarantee miss (1 in 20)
			||
			diceroll < thac0 - victim_ac + num_enemies(ch) - num_enemies(victim))
		{
			// Miss
			damage( ch, victim, 0, dt, dam_type, true );
			tail_chain( );
			return;
		}
	}

	// Hit.
	// Calc damage.
	if ( IS_NPC(ch) && wield == NULL)
	{
		dam = dice(ch->damage[DICE_NUMBER],ch->damage[DICE_TYPE]);
	}
	else
	{
		if (sn != -1){
			check_improve(ch,sn,true,5);
		}

	if( get_skill( ch, gsn_arms_mastery ) )
		check_improve(ch,gsn_arms_mastery,true,3);
	else if( get_skill(ch, gsn_sword_mastery )
		&& wield->value[0] == WEAPON_SWORD )
		check_improve(ch,gsn_sword_mastery,true,3);
	else if( get_skill(ch, gsn_dagger_mastery )
		&& wield->value[0] == WEAPON_DAGGER )
		check_improve(ch,gsn_dagger_mastery,true,3);
	else if( get_skill(ch, gsn_flail_mastery )
		&& wield->value[0] == WEAPON_FLAIL )
		check_improve(ch,gsn_flail_mastery,true,3);
	else if( get_skill(ch, gsn_mace_mastery )
		&& wield->value[0] == WEAPON_MACE )
		check_improve(ch,gsn_mace_mastery,true,3);
	else if( get_skill(ch, gsn_polearm_mastery )
		&& wield->value[0] == WEAPON_POLEARM )
		check_improve(ch,gsn_polearm_mastery,true,3);
	else if( get_skill(ch, gsn_staff_mastery )
		&& wield->value[0] == WEAPON_STAFF )
		check_improve(ch,gsn_staff_mastery,true,3);
	else if( get_skill(ch, gsn_whip_mastery )
		&& wield->value[0] == WEAPON_WHIP )
		check_improve(ch,gsn_whip_mastery,true,3);
	else if( get_skill(ch, gsn_spear_mastery )
		&& wield->value[0] == WEAPON_SPEAR )
		check_improve(ch,gsn_spear_mastery,true,3);
	else if( get_skill(ch, gsn_sickle_mastery )
		&& wield->value[0] == WEAPON_SICKLE )
		check_improve(ch,gsn_sickle_mastery,true,3);
	else if( get_skill(ch, gsn_lightblade_mastery )
		&& wield->value[0] == WEAPON_LIGHTBLADE )
		check_improve(ch,gsn_lightblade_mastery,true,3);
	else if( get_skill(ch, gsn_crossbow_mastery )
		&& wield->value[0] == WEAPON_CROSSBOW )
		check_improve(ch,gsn_crossbow_mastery,true,3);
	else if( get_skill(ch, gsn_pistol_mastery )
		&& wield->value[0] == WEAPON_PISTOL )
		check_improve(ch,gsn_pistol_mastery,true,3);
	else if( get_skill(ch, gsn_rifle_mastery )
		&& wield->value[0] == WEAPON_RIFLE )
		check_improve(ch,gsn_rifle_mastery,true,3);
	else if( get_skill(ch, gsn_blastpistol_mastery )
		&& wield->value[0] == WEAPON_BLASTPISTOL )
		check_improve(ch,gsn_blastpistol_mastery,true,3);
	else if( get_skill(ch, gsn_blastrifle_mastery )
		&& wield->value[0] == WEAPON_BLASTRIFLE )
		check_improve(ch,gsn_blastrifle_mastery,true,3);
	else if( get_skill(ch, gsn_bow_mastery )
		&& wield->value[0] == WEAPON_LONG_BOW )
		check_improve(ch,gsn_bow_mastery,true,3);
	else if( get_skill(ch, gsn_bow_mastery )
		&& wield->value[0] == WEAPON_SHORT_BOW )
		check_improve(ch,gsn_bow_mastery,true,3);
	else if( get_skill(ch, gsn_unarmed_mastery )
		&& wield->value[0] == WEAPON_CESTUS )
		check_improve(ch,gsn_unarmed_mastery,true,3);
	else if( get_skill(ch, gsn_unarmed_mastery )
		&& wield->value[0] == WEAPON_SIANGHAM )
		check_improve(ch,gsn_unarmed_mastery,true,3);
	else if( get_skill(ch, gsn_unarmed_mastery )
		&& wield->value[0] == WEAPON_KAMA )
		check_improve(ch,gsn_unarmed_mastery,true,3);

		if ( wield != NULL )
		{
			dam = dice(wield->value[1] ,wield->value[2]) * skill/100;
			// Send MSP sound message
			{
				char weapon_sound[MIL];
				sprintf(weapon_sound,"%s6.wav", get_weapontype( wield ));
				msp_to_room(MSPT_COMBAT, weapon_sound,
					0,
					ch,
					false,
					true);
			}

			// if weapon is above their level reduce the damage accordingly
			if (wield->level>ch->level)
			{
				dam=dam * ch->level / wield->level;
			}

			// if defending a mount get bunch of extra damage
			if (victim->fighting==ch->mounted_on)
			{
				dam=dam * 11/7;
			}

			// If weapon is blessed & enemy is undead/ghost/etc
			if( IS_SET(wield->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD ))
			{
				dam = dam * 3 / 2;
			}


			// add damage bonus/penalty for mountedness:
			// - if riding <80 you get from 1 to 100 percent of original damage
			// - if riding >=80 you get 100 to 120 percent
			// - based on riding skill and random factors
			// - if defending your mount, these bonuses/penalties do not apply
			if ((ch->mounted_on!=NULL)&&(!IS_NPC(ch))
				&&(victim->fighting!=ch->mounted_on))
			{
				if (get_skill(ch, gsn_riding)<80)
				{
					dam *=(((get_skill(ch, gsn_riding)+20) * number_range(10,100))
						/10000);
				}
				else
				{
					dam+=dam*((number_range(1,100))*(get_skill(ch, gsn_riding)-80))/10000;
					check_improve(ch, gsn_riding, true, 10);
				}
			}

			// no shield or second weapon = more damage
			if (!get_eq_char(ch, WEAR_SECONDARY)
				&& !get_eq_char(ch,WEAR_SHIELD))
				dam = dam * 6/5;

			// sharpness!
			if (IS_WEAPON_STAT(wield,WEAPON_SHARP))
			{
				int percent;

				if ((percent = number_percent()) <= (skill / 8))
					dam = 2 * dam + (dam * 2 * percent / 100);
			}

			if (IS_WEAPON_STAT(wield,WEAPON_VORPAL))
			{
				int percent;
				if((percent=number_percent()) <=(skill/33))
					dam = 6 * dam + (dam*4*percent/100);
			}

			if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(wield,WEAPON_HOLY)
				&& IS_OBJ_STAT(wield,OBJEXTRA_ANTI_GOOD))
			/*	&& HAS_CLASSFLAG(ch, CLASSFLAG_DAMMODS_WITH_HOLYWEAPONS))*/
			{
				dam = dam + (victim->alliance/3)* (dam*5/4);
			}

			if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(wield,WEAPON_HOLY)
				&& IS_OBJ_STAT(wield,OBJEXTRA_ANTI_EVIL) )
			/*	&& HAS_CLASSFLAG(ch, CLASSFLAG_DAMMODS_WITH_HOLYWEAPONS))*/
			{
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);
			}

		}else{
			if( get_skill(ch, gsn_unarmed_mastery )){
				check_improve(ch,gsn_unarmed_mastery,true,3);}
			dam = number_range( 1 + 4 * skill/100, 2 * ch->level/3 * skill/100);
		}
	}

	// Bonuses
	if ( get_skill(ch,gsn_enhanced_damage) > 0 )
	{
		diceroll = number_percent();
		if (diceroll <= get_skill(ch,gsn_enhanced_damage))
		{
			check_improve(ch,gsn_enhanced_damage,true,6);
			dam += 2 * ( dam * diceroll/300);
		}
	}

    if (IS_AFFECTED2(ch, AFF2_POSSESSION))
    {
        diceroll = number_percent();
		if (diceroll <= 50){
			dam += 2 * (dam * diceroll / 300);
		}
    }

	if ( get_skill(ch,gsn_ultra_damage) > 0 )
	{
		diceroll = number_percent();
		if (diceroll <= get_skill(ch,gsn_ultra_damage))
		{
			check_improve(ch,gsn_ultra_damage,true,7);
			dam += 3/2 * ( dam * diceroll/300);
		}
	}

	if ( !IS_AWAKE(victim) ){
		dam *= 2;
	}else{
		if (victim->position < POS_FIGHTING){
			dam = dam * 3 / 2;
		}
	}

	if ( dt == gsn_backstab && wield != NULL){
		if ( wield->value[0] != 2 ){
			dam *= 2 + (ch->level / 10);
		}else {
			dam *= 2 + (ch->level / 8);
		}
	}

    if ( dt == gsn_circle && wield != NULL){
		if ( wield->value[0] != 2 ){
			dam *= 2 + (ch->level / 16);
		}else{
			dam *= 2 + (ch->level / 11);
		}
	}

    int droll=GET_DAMROLL(ch);
    if (ch->symbiotetype != 0){
        droll += ch->symbiotedr;}


    dam += droll * UMIN(100,skill) /100;
//	dam += GET_DAMROLL(ch) * UMIN(100,skill) /100;

	if ( dam <= 0 )
		dam = 1;
    diceroll=number_range(0,19);
    if ( diceroll == 19 )
    {
    ch->println("`r!!!CRITICAL STRIKE!!!`x");
	dam = dam*2;
    }


	result = damage( ch, victim, dam, dt, dam_type, true );

	// but do we have a funky weapon?
	if (result && wield)
	{
		int dam;
		int damage_to_inflict; // damage result

		if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_POISON))
		{
			int level;
			AFFECT_DATA *poison, af;

			poison = affect_find(wield->affected,gsn_poison);
			if(poison){
				level = poison->level;
			}else{
				level = wield->level;
			}

			if (!saves_spell(level / 2,victim,DAM_POISON)
				&& !HAS_CLASSFLAG(victim, CLASSFLAG_POISON_IMMUNITY))
			{
				victim->println("You feel poison coursing through your veins.");
				act("$n is poisoned by the venom on $p.",
					victim,wield,NULL,TO_ROOM);

				af.where     = WHERE_AFFECTS;
				af.type      = gsn_poison;
				af.level     = level * 3/4;
				af.duration  = level / 2;
				af.location  = APPLY_ST;
				af.modifier  = -1;
				af.bitvector = AFF_POISON;
				affect_join( victim, &af );
			}

			// weaken the poison if it's temporary
			if(poison){
				poison->level = UMAX(0,poison->level - 2);
				poison->duration = UMAX(0,poison->duration - 1);

				if (IS_ALIVE(victim) && (poison->level == 0 || poison->duration == 0)){
					act("The poison on $p has worn off.",ch,wield,NULL,TO_CHAR);
				}
			}
		}

		// vampiric weapon effect *
		if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_VAMPIRIC))
		{
			dam = number_range(1, wield->level / 10 + 1);

			damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_NEGATIVE);
			if(damage_to_inflict>=1){
				// *** patch up the damage_to_inflict value
				// limit the damage to the amount required to put the victim
				// into -11 hp... in order to limit the benefits of the weapon
				damage_to_inflict=UMIN(victim->hit+11,damage_to_inflict);
				damage_to_inflict=UMAX(0,damage_to_inflict);
				// display the custom damage messages
				act("$p draws life from $N.",ch,wield,victim,TO_NOTVICT);
				act_with_autodam_to_char("You feel $p drawing your life away.",victim,wield,NULL,damage_to_inflict);
				act_with_autodam_to_char("$p draws life from $N.",ch,wield,victim,damage_to_inflict/2);
				// inflict the custom effect before the standard damage
				ch->hit += damage_to_inflict/2;
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_NEGATIVE);
			}
		}

		// flaming weapon effect *
		if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FLAMING))
		{
			dam = number_range(1,wield->level / 4 + 1);

			damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_FIRE);
			if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is schorched by $p.",ch,wield,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is scorched by $p.",ch,wield,victim,damage_to_inflict);
				act_with_autodam_to_char("$p sears your flesh.",victim,wield,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				fire_effect( (void *) victim,wield->level/2,dam,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_FIRE);
			}
		}

		// acidic weapon effect *
		if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_ACIDIC))
		{
			dam = number_range(1,wield->level / 4 + 1);

			damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_ACID);
			if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is burned by $p's acid.",ch,wield,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is burned by $p's acid.",ch,wield,victim,damage_to_inflict);
				act_with_autodam_to_char("$p melts your flesh.",victim,wield,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				acid_effect( (void *) victim,wield->level/2,dam,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_ACID);
			}
		}

		// frost weapon effect *
		if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FROST))
		{
			dam = number_range(1,wield->level / 6 + 2);

			damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_COLD);
			if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$p freezes $N.",ch,wield,victim,TO_NOTVICT);
				act_with_autodam_to_char("$p freezes $N.",ch,wield,victim,damage_to_inflict);
				act_with_autodam_to_char("The cold touch of $p surrounds you with ice.",
					victim,wield,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_COLD);
			}
		}

		// suckle weapon effect - takes mana
		if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_SUCKLE))
        {
            dam = number_range(1, wield->level / 5 + 1);
			if(dam>=1){
            act("$p draws mystical energy from $n.",victim,wield,NULL,TO_ROOM);
            act("You feel $p drawing your mystical energy away.",
                victim,wield,NULL,TO_CHAR);
            victim->mana -= dam;
            ch->mana += dam/2;
			}
        }

		// enervated weapon effect - takes movements
        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_ENERVATE))
        {
			if (!IS_SET(victim->form, FORM_UNDEAD )){
		    dam = number_range(1, wield->level / 5 + 1);
            if(dam>=1){
			act("$p saps the strength from $n.",victim,wield,NULL,TO_ROOM);
            act("You feel $p sapping your strength away.",
                victim,wield,NULL,TO_CHAR);
            victim->move-= dam;
            ch->move += dam/2;
			}
			}
        }

		// annealed weapon effect *
		if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_ANNEALED))
		{
			dam = number_range(1,wield->level/5 + 2);
			if (!saves_spell(wield->level/4 + dam/20,victim,DAM_BASH)){
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_BASH);
				if(damage_to_inflict>=1){
					// display the custom damage messages
					victim->printlnf("You see stars as your head `Crings`x!%s",autodamtext(victim, damage_to_inflict));
					ch->printlnf("Your opponent looks `Bstunned`x!%s",autodamtext(ch, damage_to_inflict));
					// inflict the custom effect before the standard damage
					DAZE_STATE(victim,UMAX(12,wield->level/4 + dam/20));
					// inflict the actual damage
					damage_inflict(ch, victim, damage_to_inflict, 0, DAM_BASH);
				}
			}
		}

		// shocking weapon effect *
		if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_SHOCKING))
		{
			dam = number_range(1,wield->level/5 + 2);
			damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_LIGHTNING);
			if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is struck by lightning from $p.",ch,wield,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is struck by lightning from $p.",ch,wield,victim,damage_to_inflict);
				act_with_autodam_to_char("You are shocked by $p",victim,wield,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				shock_effect(victim,wield->level/2,dam,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0,DAM_LIGHTNING);
			}
		}
	}
	tail_chain( );
	return;
}

/**************************************************************************/
bool is_safe(char_data *ch, char_data *victim)
{
    char log_buf[MSL];

	if (victim->in_room == NULL || ch->in_room == NULL)
		return true;

	if (victim->fighting == ch || victim == ch)
		return false;

	if (IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL)
		return false;


	if(!IS_NPC(ch) && !IS_NPC(victim) && GAMESETTING(GAMESET_PEACEFUL_MUD)){
		ch->println("This is a peaceful mud... pkilling is not permitted.");
		return true;
	}

	// unseen mobs
	if ( !IS_IMMORTAL(ch) &&
		IS_NPC(victim) && IS_SET(victim->act, ACT_IS_UNSEEN))
	{
		return true;
	}

	// safe rooms
	if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
	{
		ch->println("Not in this room.");
		return true;
	}

	if( IS_NPC(victim) && IS_SET(victim->act2,ACT2_AVOIDS_ALL_ATTACKS) ){
		ch->printlnf("%s avoids your attack.", PERS(victim, ch));
		return true;
	}

	if( IS_KEEPER(victim) && !GAMESETTING2(GAMESET2_CAN_ATTACK_SHOPKEEPERS)){
		ch->printlnf("%s avoids your attack.", PERS(victim, ch));
		return true;
	}


	// ooc rooms
	if (IS_SET(victim->in_room->room_flags,ROOM_OOC))
	{
		ch->println("Not in an OOC room.");
		return true;
	}


    // killing mobiles
    if (IS_NPC(victim))
    {
		if( ch->level<6 || (!IS_LETGAINED(ch) && GAMESETTING_LETGAINING_IS_REQUIRED) ){
			if ( IS_SET(victim->act, ACT_PET)
					&& victim->leader
					&& !IS_NPC(victim->leader)
					&& (victim->leader->pet == victim)
					&& IS_SET(victim->affected_by, AFF_CHARM))
			{
				ch->wrapln("That is a players pet, which could start a pkill fight, "
					"since you are either below level 6 or not letgained you can't "
					"start pkill combat.");
				return true;
			}
		}

		if (HAS_CONFIG(ch, CONFIG_DISALLOWED_PKILL))
		{
			if ( IS_SET(victim->act, ACT_PET)
					&& victim->leader
					&& !IS_NPC(victim->leader)
					&& (victim->leader->pet == victim)
					&& IS_SET(victim->affected_by, AFF_CHARM))
			{
				ch->wrapln("That is a players pet, which could start a pkill fight, "
					"since you are pkill restricted you can't start pkill combat.");
				return true;
			}
		}



		if (victim->pIndexData->pShop != NULL)
		{
		     ch->println("The shopkeeper wouldn't like that.");
		     return true;
		}





		 if (IS_SET(victim->act,ACT_TRAIN)
		 ||  IS_SET(victim->act,ACT_PRACTICE)
		 ||  IS_SET(victim->act,ACT_IS_HEALER)
		 ||  IS_SET(victim->act,ACT_IS_CHANGER)
		 ||  IS_SET(victim->act2,ACT2_REPAIRMAN))
		  {
		     ch->println("I don't think the Gods would approve.");
		     return true;
		  }


		/* checks that players can't kill certain types of mobs */
		if (!IS_NPC(ch))
		{
			/* no pets */
			/*    if (IS_SET(victim->act,ACT_PET))
			*   {
			*  act("But $N looks so cute and cuddly...",
			*       ch,NULL,victim,TO_CHAR);
			*  return true;
			*   }
			*/
			/* no charmed creatures unless owner */

			/*			if (IS_AFFECTED(victim,AFF_CHARM) && ch != victim->master)
			{
			ch->println("You don't own that monster.");
			return true;
			}
			*/
		}
    }
    else  // killing players
    {
		// being ordered to, and the master isn't letgained
		if(IS_SET( ch->dyn, DYN_IS_BEING_ORDERED ) && ch->master)
		{
			if((ch->master->level<6) || (!IS_LETGAINED(ch->master) && GAMESETTING_LETGAINING_IS_REQUIRED) ){
				// tell charmie
				ch->println("Your master isn't letgained or above level 6.");
				// tell master
				ch->master->println("You can not order a pet to pkill, steal from players etc,");
				ch->master->println("till level 6 and you have been letgained.");
				ch->master->println("(See HELP DEATH and HELP LETGAIN)");
				return true;
			}
		}

		// NPC doing the killing
		if (IS_NPC(ch))
		{
			// safe room check
			if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
			{
				ch->println("Not in this room.");
				return true;
			}

			// charmed mobs and pets cannot attack players while owned
			if (IS_AFFECTED(ch,AFF_CHARM) && ch->master != NULL
				&&  ch->master->fighting != victim)
			{
				ch->println("Players are your friends!");
				return true;
			}
			if (IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL
				&& ch->master->level<ch->level )
			{
				ch->master->println( "Your pet realizes it is more powerfull than you and leaves." );
				act( "$n stops following you.", ch, NULL, ch->master, TO_VICT );
				act( "You stop following $N.",	ch, NULL, ch->master, TO_CHAR );

				REMOVE_BIT(ch->master->pet->act, ACT_PET);
				stop_fighting(ch,false);
				stop_follower(ch->master->pet);
				return true;
			}
		}
		else  // player killing another player
		{
			if (IS_PEACEFUL(ch)){
				ch->println("You can not player kill, steal from players etc,");
				ch->println("till are set to ACTIVE PK status.");
				return true;
			}

			if (IS_ACTIVE(ch) && IS_PEACEFUL(victim)){
				ch->println("They are not set to ACTIVE PK status!");
				return true;
			}

			if (ch->level != victim->level){
			int clev;
			int vlev;
			clev = ch->level;
			vlev = victim->level;
			if ((clev -= 6) >= vlev && !IS_ICIMMORTAL(victim)){
				ch->println("They are out of range!");
				return true;}

			/*int ldiff;
			ldiff = vlev - clev;
			if (ldiff >= 6  && !IS_ICIMMORTAL(victim)){
				ch->println("You suck!");
				return true;}*/

			}

			if(HAS_CONFIG(ch, CONFIG_DISALLOWED_PKILL))
			{
				ch->println("You can not player kill, steal from players etc,");
				ch->println("due to a pkill restriction sorry.");
				return true;
			}

			if(HAS_CONFIG2(ch,CONFIG2_NOPKILL)){
				ch->println("You can't pkill others");
				return true;
			}
			if(HAS_CONFIG2(victim,CONFIG2_NOPKILL)){
				ch->println("A mysterious force prevents you from doing that.");
				return true;
			}

			if(((victim->level<11))&&(ch->pkool>0))
			{
				ch->println("You cannot kill the weak so soon after doing so.");
				return true;
			}
			if(victim->pksafe>0)
			{
				ch->println("He has died too recently, leave him alone.");
				return true;
			}

			if (!IS_NPC(ch) && !IS_NPC(victim))
			{
				sprintf( log_buf, "is_safe: %s might be starting a pkill fight with %s at %d!!!",
					ch->name,
					victim->name,
					ch->in_room->vnum );
				log_string( log_buf );
				wiznet(log_buf,NULL,NULL,WIZ_BETA,0,0);
			}


		}
    }
    return false;
}

/**************************************************************************/
bool is_safe_spell(char_data *ch, char_data *victim, bool area )
{
	if (victim->in_room == NULL || ch->in_room == NULL)
		return true;

	if (victim == ch && area){
		return true;
	}

	if ( !IS_ICIMMORTAL(ch) &&
		IS_NPC(victim) && IS_SET(victim->act, ACT_IS_UNSEEN))
	{
		return true;
	}

	if (victim->fighting == ch || victim == ch){
		return false;
	}

	if(area)
	{
		if (ch->fighting!=victim && victim->fighting!=ch
			&& !can_initiate_combat( ch, victim, CIT_GENERAL | CIT_SILENT))
		{
			return true;
		}
	}else{
		if (ch->fighting!=victim && victim->fighting!=ch
			&& !can_initiate_combat( ch, victim, CIT_GENERAL))
		{
			return true;
		}
	}

    if (IS_ICIMMORTAL(ch) && !area){
		return false;
	}
	if (IS_ICIMMORTAL(ch)){
		return false;}

	// safe room?
	if (IS_SET(victim->in_room->room_flags,ROOM_SAFE)){
		return true;
	}

	if (HAS_CONFIG(ch, CONFIG_DISALLOWED_PKILL))
	{
		return true;
	}

	// safe room?
	if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
		return true;

	// killing mobiles
	if (IS_NPC(victim))
	{
		if (victim->pIndexData->pShop != NULL)
			return true;

		// no killing healers, trainers, etc
		if (IS_SET(victim->act,ACT_TRAIN)
			||  IS_SET(victim->act,ACT_PRACTICE)
			||  IS_SET(victim->act,ACT_IS_HEALER)
			||  IS_SET(victim->act2,ACT2_REPAIRMAN)
			||  IS_SET(victim->act,ACT_IS_CHANGER))
			return true;

		if (!IS_NPC(ch))
		{
			// no pets
			if (IS_SET(victim->act,ACT_PET))
				return true;

			// no charmed creatures unless owner
			if (IS_AFFECTED(victim,AFF_CHARM) && (area || ch != victim->master))
				return true;

			// legal kill? -- cannot hit mob fighting non-group member
			if (victim->fighting != NULL && !is_same_group(ch,victim->fighting))
				return true;
		}
		else
		{
			// area effect spells do not hit other mobs
			if (area && !is_same_group(victim,ch->fighting))
				return true;
		}
	}
	// killing players
	else
	{
		if (area && IS_ICIMMORTAL(victim)){
			return true;
		}

		if(HAS_CONFIG2(ch,CONFIG2_NOPKILL)){
			return true;
		}
		if(HAS_CONFIG2(victim,CONFIG2_NOPKILL)){
			return true;
		}

		// NPC doing the killing
		if (IS_NPC(ch))
		{
			// charmed mobs and pets cannot attack players while owned
			if (IS_AFFECTED(ch,AFF_CHARM) && ch->master != NULL
				&&  ch->master->fighting != victim)
				return true;

			// safe room?
			if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
				return true;

			// legal kill? -- mobs only hit players grouped with opponent
			if (ch->fighting != NULL && !is_same_group(ch->fighting,victim))
				return true;
		}
		// player doing the killing
		else
		{
			if(IS_PEACEFUL(ch)){
				return true;
			}
			if(IS_PEACEFUL(victim)){
				return true;
			}
			if (IS_ACTIVE(ch) && IS_ACTIVE(victim)){
			if ((ch->level -= 6) >= victim->level){
				//ch->println("They are out of range");
				if(ch->fighting == victim){
					ch->fighting = NULL;}
				return true;}

			if ((ch->level += 6) <= victim->level){
				//ch->println("They are out of range");
				if(ch->fighting == victim){
					ch->fighting = NULL;}
				return true;}
			}

			if(victim->pksafe>0)
				return true;

			if ((victim->level<11)&&(ch->pkool>0))
				return true;
		}


	}

    if (!IS_NPC(ch) && !IS_NPC(victim))
    {
		sprintf( log_buf, "is_safe_spell: %s might be starting a pkill fight with %s at %d!!!",
			ch->name,
			victim->name,
			ch->in_room->vnum );
		log_string( log_buf );
		wiznet(log_buf,NULL,NULL,WIZ_BETA,0,0);
    }
	return false;
}


/**************************************************************************/
// Check for parry.
bool check_parry( char_data *ch, char_data *victim )
{
	int chance;

	if ( !IS_AWAKE(victim) ){
		return false;
	}

	if ( IS_NPC(victim) && !IS_SET(victim->off_flags, OFF_PARRY) ){
		return false;
	}

	chance = get_skill(victim,gsn_parry) / 2;
	if(chance < 1){
		return false;
	}

	if ( get_eq_char( victim, WEAR_WIELD ) == NULL )
	{
		if (IS_NPC(victim))
			chance /= 2;
		else
			return false;
	}

	if (!can_see(ch,victim))
		chance /= 2;
/*
    if (ch->tension != 0){
		chance = chance * 2 / 3;
    }
*/
	if ( number_percent( ) >= chance + victim->level - ch->level )
		return false;

	act( "`bYou parry $n's attack.`x", ch, NULL, victim, TO_VICT  );
	act( "`b$n parries your attack.`x", victim, NULL, ch, TO_VICT );
	check_improve(victim,gsn_parry,true,6);
	return true;
}

/**************************************************************************/
// Check for shield block.
bool check_shield_block( char_data *ch, char_data *victim )
{
	int chance;

	if ( !IS_AWAKE(victim) )
		return false;

	chance = get_skill(victim,gsn_shield_block) / 5 + 3;


	if ( get_eq_char( victim, WEAR_SHIELD ) == NULL )
		return false;

    if (ch->tension != 0){
		chance = chance * 2 / 3;
    }

	if ( number_percent( ) >= chance + victim->level - ch->level )
		return false;

	act( "`bYou block $n's attack with your shield.`x",
		ch, NULL, victim, TO_VICT    );
	act( "`b$n blocks your attack with a shield.`x",
		victim, NULL, ch, TO_VICT    );

	check_improve(victim,gsn_shield_block,true,6);
	return true;
}


/**************************************************************************/
// Check for dodge and advanced dodge.
bool check_dodge( char_data *ch, char_data *victim )
{
	int chance;
	int sn;
	if ( !IS_AWAKE(victim) ){
		return false;
	}
	if ( IS_NPC(victim) && !IS_SET(victim->off_flags, OFF_DODGE) ){
        return false;
	}
	chance = get_skill(victim,gsn_advanced_dodge);
//	if(chance < 1){
//      chance = 1;
//	}
	if(chance){
		sn = gsn_advanced_dodge;
		chance -= 40;

		// factor in your skill at standard dodge as well (increase your chance by 0 to 20%)
		chance += get_skill(victim,gsn_dodge) / 5;

		if(get_skill(victim,gsn_dodge)>0 && number_percent()>get_skill(victim,gsn_dodge)){
			// primarily improve in dodge until that is mastered before improving in advanced dodge
			sn=gsn_dodge;
		}
	}else{
		sn = gsn_dodge;
		chance = get_skill(victim,gsn_dodge) / 2;
	}
	if (!can_see(victim,ch)){
		chance /= 2;
	}

	if (ch->tension != 0){
		chance = chance * 2 / 3;
    }

	if ( number_percent( ) >= chance + victim->level - ch->level ){
		return false;
	}
	act( "`bYou dodge $n's attack.`x", ch, NULL, victim, TO_VICT);
	act( "`b$n dodges your attack.`x", victim, NULL, ch, TO_VICT);
	check_improve(victim,sn,true,6);
	if (IS_SET(victim->psionic_affects, PAFF_BLUR)){
		chance += 3 * (get_skill(victim,gsn_blur) - chance);
		check_improve(victim, gsn_blur, true, 1);}
	return true;
}

/**************************************************************************/
// Set position of a victim based on their current hp
void update_pos( char_data *victim )
{
    if ( !IS_NPC(victim)
		&& victim->level >= LEVEL_IMMORTAL
		&& victim->hit < 1 )
	{
		victim->hit = 1;
	}

	// GAMESET2_SHOPKEEPERS_CAN_BE_KILLED flag turned off
	if(IS_KEEPER(victim)
		&& !GAMESETTING2(GAMESET2_SHOPKEEPERS_CAN_BE_KILLED)
		&& victim->hit < 1 )
	{
		victim->hit=20;
		victim->max_hit+=20;
		victim->level+=number_range(2,20);
		victim->highest_level_to_do_damage+=10;
	}

	if ( victim->hit > 0 )
	{
		if ( victim->position <= POS_STUNNED )
			victim->position = POS_STANDING;
		return;
	}

	if ( IS_NPC(victim) && victim->hit < 1 )
	{
		victim->position = POS_DEAD;
		return;
    }

    if ( victim->hit <= -11 )
    {
		victim->position = POS_DEAD;
		return;
    }

	if ( victim->hit <= -6 ) victim->position = POS_MORTAL;
    else if ( victim->hit <= -3 ) victim->position = POS_INCAP;
    else                          victim->position = POS_STUNNED;

    return;
}

/**************************************************************************/
// Start fights.
void set_fighting( char_data *ch, char_data *victim )
{
	char		buf[MSL];
	OBJ_DATA	*obj;
 	if (IS_SET(ch->act2,PLR2_TBC)/* && is_fighting (ch)*/){
    //     turn_end (ch->turn, 200 - GET_SPEED (ch));
         return;
        }
    if (IS_SET(victim->act2,PLR2_TBC)/* && is_fighting (victim)*/){
    //     turn_end (victim->turn, 200 - GET_SPEED (victim));
         return;
        }

    if(ch->fighting)
    {
		bug("Set_fighting: ch already fighting");
		return;
    }

	if(!IS_NPC(ch))
	{
		// wizi imms wont start fighting on area attacks by PC's
		if (!IS_SET(ch->dyn,DYN_DOING_DAMAGE) && INVIS_LEVEL(victim)> ch->level)
		{
			victim->println("You don't return the attack, your wizi above them.");
			return;
		}
		// unseen mobs wont attack the ch
		if (IS_NPC(victim) 	&& !IS_SET(ch->dyn,DYN_DOING_DAMAGE)
			&& IS_SET(victim->act, ACT_IS_UNSEEN)
			&& !HAS_HOLYLIGHT(ch))
		{
			victim->println("You don't return the attack, your UNSEEN to them.");
			return;
		}
	}

	if( is_affected( ch, gsn_sneak )){
		affect_strip( ch, gsn_sneak );
	}

	if( is_affected( victim, gsn_sneak )){
		affect_strip( victim, gsn_sneak );
	}

    if( IS_AFFECTED(ch, AFF_SLEEP) ){
		affect_parentspellfunc_strip( ch, gsn_sleep );
		REMOVE_BIT( ch->affected_by, AFF_SLEEP);
	}
    if (!IS_SET(ch->act2,PLR2_TBC) && !IS_SET(ch->act2,PLR2_TBC)){
    ch->fighting = victim;
    ch->position = POS_FIGHTING;}

	// Auto draw weapons from sheathed to wield
	if ( get_eq_char( ch, WEAR_WIELD ) == NULL
	&&   get_eq_char( ch, WEAR_SHEATHED ) != NULL )
	{
		obj = get_eq_char( ch, WEAR_SHEATHED );
		equip_char( ch, obj, WEAR_WIELD );
		act( "You hastily draw $p.", ch, obj, NULL, TO_CHAR );
		act( "$n hastily draws $p.", ch, obj, NULL, TO_ROOM );
		WAIT_STATE( ch, PULSE_VIOLENCE );
	}
	if ( get_eq_char( victim, WEAR_WIELD ) == NULL
	&&   get_eq_char( victim, WEAR_SHEATHED ) != NULL )
	{
		obj = get_eq_char( victim, WEAR_SHEATHED );
		equip_char( victim, obj, WEAR_WIELD );
		act( "You hastily draw $p.", victim, obj, NULL, TO_CHAR );
		act( "$n hastily draws $p.", victim, obj, NULL, TO_ROOM );
		WAIT_STATE( victim, PULSE_VIOLENCE );
	}


    if (true_CH(ch)->pcdata && IS_SET(true_CH(ch)->act,PLR_AUTOSUBDUE)){
		SET_BIT(ch->dyn,DYN_CURRENT_SUBDUE);
		ch->println("You are subduing in this fight.");
	}else{
		REMOVE_BIT(ch->dyn,DYN_CURRENT_SUBDUE);
		ch->println("You are fighting to kill in this fight.");
    }

	// who started the fight and pksafeness stuff
	if(IS_SET(ch->dyn,DYN_DOING_DAMAGE)
		==IS_SET(victim->dyn,DYN_DOING_DAMAGE))
	{
		// check for mudprogs that trigger when someone starts combat
		// with a spell, and the mudprog triggers a spell in return
		// (this is only in the first round of combat)
		if(IS_SET(ch->dyn,DYN_RUNNING_MUDPROG_CMD)){
			// victim started the fight, ch is mudprog in return
			logf("set_fighting(): ch->dyn,DYN_RUNNING_MUDPROG_CMD is true.");
			logf("set_fighting(): victim (%s) started fight with %s, ch=%s",
				victim->name,
				victim->fighting?victim->fighting->name:"victim->fighting==NULL",
				ch->name);
			victim->pksafe=0;
		}else if(IS_SET(victim->dyn,DYN_RUNNING_MUDPROG_CMD)){
			// ch started the fight, victim is mudprog in return
			logf("set_fighting(): victim->dyn,DYN_RUNNING_MUDPROG_CMD is true.");
			logf("set_fighting(): ch (%s) started fight with %s, vict=%s",
				ch->name,
				ch->fighting?ch->fighting->name:"ch->fighting==NULL",
				victim->name);
			ch->pksafe=0;
		}else{
			bugf("set_fighting(): ch->dyn,DYN_DOING_DAMAGE "
				"equals victim->dyn,DYN_DOING_DAMAGE)!");
			if(!IS_NPC(ch) && !IS_NPC(victim)){
			victim->pksafe=0;
			ch->pksafe=0;
	//			do_abort();
			}
		}
	}else if(IS_SET(ch->dyn,DYN_DOING_DAMAGE)){
		// ch started the fight
		if(!IS_NPC(victim) || !IS_NPC(ch->fighting)){
			logf("set_fighting(): ch (%s) started fight with %s, vict=%s",
				ch->name, ch->fighting->name, victim->name);
			SET_BIT(ch->dyn, DYN_STARTED_FIGHT);
			REMOVE_BIT(victim->dyn, DYN_STARTED_FIGHT);
		}
		ch->pksafe=0;
	}else{
		// victim started the fight
		if(!IS_NPC(ch) || (victim->fighting && !IS_NPC(victim->fighting))){
			logf("set_fighting(): victim (%s) started fight with %s, ch=%s",
				victim->name,
				victim->fighting?victim->fighting->name:"victim->fighting==NULL",
				ch->name);
			SET_BIT(victim->dyn, DYN_STARTED_FIGHT);
			REMOVE_BIT(ch->dyn, DYN_STARTED_FIGHT);
		}
		victim->pksafe=0;
	}


	// faults with mobs caused by bad use of olc checks only below here
	// - WILL BE MOVED INTO ASAVE LATER PROBABLY
	// check the level on both parties
	if (victim->level==0)
	{
		sprintf( buf, "`1Victim = [%5d] %s (%s) `1Room = [%5d] %s"
			"`1Attacker = %s`1"
			"Note - victims level has been set to 90 after the fight",
			IS_NPC(victim) ? victim->pIndexData->vnum : 0,
			victim->short_descr, victim->name,
			victim->in_room->vnum,
			victim->in_room->name, ch->name);

		victim->level=1;
		if (IS_NPC(victim) && victim->pIndexData->level==0)
		{
			victim->pIndexData->level=90;
			SET_BIT( victim->pIndexData->area->olc_flags, OLCAREA_CHANGED );
		}

		autonote(NOTE_SNOTE, "set_fighting()", "victim->level==0", "olc"	, buf, true);

	}
	if (ch->level==0)
	{
		sprintf( buf, "`1ch = [%5d] %s (%s) `1Room = [%5d] %s"
			"`1Attacker = %s`1"
			"Note - ch level has been set to 90 after the fight",
			IS_NPC(ch) ? ch->pIndexData->vnum : 0,
			ch->short_descr, ch->name,
			ch->in_room->vnum,
			ch->in_room->name, victim->name);

		ch->level=1;
		if (IS_NPC(ch) && ch->pIndexData->level==0)
		{
			ch->pIndexData->level=90;
			SET_BIT( ch->pIndexData->area->olc_flags, OLCAREA_CHANGED );
		}

		autonote(NOTE_SNOTE, "set_fighting()", "ch->level==0", "olc"	, buf, true);
	}

	// check the max hp on both parties - note olc if there is a problem
	if (victim->max_hit==0)
	{
		sprintf( buf, "`1Victim = [%5d] %s (%s) `1Room = [%5d] %s"
			"`1Attacker = %s`1"
			"Note - victims hitdice has been set to 1+1+(level*4) after the fight",
			IS_NPC(victim) ? victim->pIndexData->vnum : 0,
			victim->short_descr, victim->name,
			victim->in_room->vnum,
			victim->in_room->name, ch->name);

		victim->max_hit=1;

		if(IS_NPC(victim))
		{
			victim->pIndexData->hit[DICE_NUMBER] = 1;
			victim->pIndexData->hit[DICE_TYPE]   = 1;
			victim->pIndexData->hit[DICE_BONUS]  = victim->level*4;
			SET_BIT( victim->pIndexData->area->olc_flags, OLCAREA_CHANGED );
		}
		autonote(NOTE_SNOTE, "set_fighting()", "victim->max_hit==0", "olc"	, buf, true);
	}
	if (ch->max_hit==0)
	{
		sprintf( buf, "`1Ch= [%5d] %s (%s) `1Room = [%5d] %s"
			"`1Attacker = %s`1"
			"Note - ch's hitdice has been set to 1+1+(level*4) after the fight",
			IS_NPC(ch) ? ch->pIndexData->vnum : 0,
			ch->short_descr, ch->name,
			ch->in_room->vnum,
			ch->in_room->name, victim->name);

		ch->max_hit=1;

		if(IS_NPC(ch))
		{
			ch->pIndexData->hit[DICE_NUMBER] = 1;
			ch->pIndexData->hit[DICE_TYPE]   = 1;
			ch->pIndexData->hit[DICE_BONUS]  = ch->level*4;
			SET_BIT( ch->pIndexData->area->olc_flags, OLCAREA_CHANGED );
		}

		autonote(NOTE_SNOTE, "set_fighting()", "ch->max_hit==0", "olc"	, buf, true);
	}
    return;
}


/**************************************************************************/
// Stop fights.
void stop_fighting( char_data *ch, bool fBoth )
{
    char_data *fch;

    for ( fch = char_list; fch; fch = fch->next )
    {
		if ( fch == ch || ( fBoth && fch->fighting == ch ) )
		{
			fch->fighting   = NULL;
			//REMOVE_BIT(fch->dyn,DYN_CURRENT_SUBDUE);
			REMOVE_BIT(fch->dyn,DYN_IS_CUTTING_OFF);
			fch->position   = IS_NPC(fch) ? fch->default_pos : POS_STANDING;
			update_pos( fch );
			strip_tension( fch );
		}
    }

    return;
}

/**************************************************************************/
// Instantly decay a corpse - eg wraith called from make_corpse
void instant_decay_corpse(char_data *ch)
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    act( "`w$n `Ydisappears`w into thin air right before your eyes!", ch, NULL, NULL, TO_ROOM );

    if ( IS_NPC(ch) )
    {
	if ( ch->gold > 0 )
	{
	    obj_to_room( create_money( ch->gold, ch->silver ), ch->in_room );
	    act( "A pile of coins falls to the ground from the space\r\nonce occupied by $n.", ch, NULL, NULL, TO_ROOM );
	    ch->gold = 0;
	    ch->silver = 0;
	}
    }
    else /* a player corpse */
    {
	if (ch->gold > 1 || ch->silver > 1)
	{
	    obj_to_room( create_money( ch->gold/2, ch->silver/2 ), ch->in_room );
	    ch->gold -= ch->gold/2;
	    ch->silver -= ch->silver/2;
	}
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	bool floating = false;

	obj_next = obj->next_content;

	if (obj->wear_loc == WEAR_FLOAT)
	     floating = true;

	obj_from_char( obj );

	if (obj->item_type == ITEM_POTION)
	     obj->timer = number_range(500,1000);
	if (obj->item_type == ITEM_SCROLL)
	     obj->timer = number_range(1000,2500);

	if (IS_SET(obj->extra_flags,OBJEXTRA_ROT_DEATH) && !floating)
	{
	    obj->timer = number_range(5,10);
	    REMOVE_BIT(obj->extra_flags,OBJEXTRA_ROT_DEATH);
	}
	REMOVE_BIT(obj->extra_flags,OBJEXTRA_VIS_DEATH);

	if ( IS_SET( obj->extra_flags, OBJEXTRA_INVENTORY ) )
	     extract_obj( obj );
	else if (floating)
	{
	    if (IS_OBJ_STAT(obj,OBJEXTRA_ROT_DEATH)) /* get rid of it! */
	    {
		if (obj->contains != NULL)
		{
		    OBJ_DATA *in, *in_next;

		    act("$p evaporates,scattering its contents.",
		    ch,obj,NULL,TO_ROOM);
		    for (in = obj->contains; in != NULL; in = in_next)
		    {
			in_next = in->next_content;
			obj_from_obj(in);
			obj_to_room(in,ch->in_room);
		    }
		 }
		 else
		    act("$p evaporates.", ch,obj,NULL,TO_ROOM);

		 extract_obj(obj);
	    }
	    else
	    {
		act("$p falls to the floor.",ch,obj,NULL,TO_ROOM);
		obj_to_room(obj,ch->in_room);
	    }
	}
	else
	{
	    act("$p falls to the floor from the space\r\nonce occupied by $n.",ch,obj,NULL,TO_ROOM);
	    obj_to_room( obj, ch->in_room );
	}
    }

    return;

}
/**************************************************************************/
// find the players morgue vnum in order of priority
// 1st - a manually set vnum *
// 2nd - clan morgue *
// 3rd - class morgue point*
// 4th - default racial morgue*
// 5th - gamewide default morgue
// * not yet implemented
int get_morguevnum(char_data *ch)
{
	int vnum=game_settings->roomvnum_morgue;

	if(get_room_index(class_table[ch->clss].morgue)){
		vnum=class_table[ch->clss].morgue;
	} else if(get_room_index(race_table[ch->race]->morgue)){
		vnum=race_table[ch->race]->morgue;
	}
	return vnum;
};
/**************************************************************************/
void do_save_corpses(char_data *ch, char *);
/**************************************************************************/
// Make a corpse out of a character.
void make_corpse( char_data *ch, char_data *killer )
{
	char *killed_by;
	if ( ch == killer ){
		killed_by = "someone";
	}else{
		killed_by = killer->short_descr;
	}

    char buf[MSL];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char *name;
	bool keeper;

	ROOM_INDEX_DATA *target_room=NULL;

	if(GAMESETTING(GAMESET_MORGUE_ENABLED) && !IS_NPC(ch)
		&& (!killer || (killer!=ch && IS_NPC(killer) && !killer->master)) )
	{
		target_room=get_room_index(get_morguevnum(ch));
		if(!target_room){
			// if you don't specify the morgue room,
			// the corpse goes to the room they appear in after death.
			target_room=get_room_index(race_table[ch->race]->death_room);
		}
	}
	if(!target_room){
		target_room=ch->in_room;
	}

    // reduce money on shopkeeper corpses to stop
    // people killing the shopkeepers for money
	keeper = IS_KEEPER(ch);
    if (keeper)
    {
		if (ch->gold>5 || ch->silver>500 )
		{
			if (ch->gold>5)
			{
				ch->gold= number_range(1,5);
			}
			else
			{
				ch->gold= number_range(1,ch->gold);
			}

			if (ch->silver>500)
			{
				ch->silver=number_range(1,500);
			}
			else
			{
				ch->silver=number_range(1,ch->silver);
			}
		}
    }

    if (IS_SET(ch->form, FORM_INSTANT_DECAY))
    {
		instant_decay_corpse(ch);
		return;
    }

    if ( IS_NPC(ch) )
    {
		name        = ch->short_descr;
		corpse      = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC));
		corpse->timer   = number_range( 3, 6 );
		if ( ch->gold > 0 || ch->silver > 0 )
		{
			obj_to_obj( create_money( ch->gold, ch->silver ), corpse );
			ch->gold = 0;
			ch->silver = 0;
		}
		//corpse->cost = 0;
		corpse->cost = ch->level * number_range(3,4);
		replace_string(corpse->killer, killed_by );

		if (IS_SET( ch->form, FORM_SENTIENT )){
			int deckchance = 2;

			if (number_range(1,10000) <= deckchance){
				OBJ_DATA *deck;
				deck = create_object(get_obj_index(701));
				obj_to_obj(deck , corpse);
			}
		}


    }
    else // a player corpse
    {
		name        = ch->short_descr;
		if (ch->master){
			stop_follower(ch);
		}
		corpse      = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC));
		corpse->timer   = number_range( 35, 50 );
		corpse->killer = str_dup( killed_by );

		if (!is_clan(ch)){
			replace_string(corpse->owner, ch->name);
		}else{
			corpse->owner = NULL;
			if (ch->gold > 1 || ch->silver > 1){
				obj_to_obj(create_money(ch->gold / 2, ch->silver/2), corpse);
				ch->gold -= ch->gold/2;
				ch->silver -= ch->silver/2;
			}
		}
		corpse->cost = 0;
	}

	corpse->level = ch->level;
	corpse->corpserace = race_table[ch->race]->name;

	sprintf( buf, corpse->short_descr, name );
	replace_string( corpse->short_descr, buf );

	sprintf( buf, corpse->long_descr, name );
	replace_string( corpse->long_descr, buf);

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
		bool floating = false;

		obj_next = obj->next_content;

		// don't take tokens out of the players inventory
		// extract_char() handles the destruction of
		// dropdeath tokens (obj->value[0], TOKEN_DROPDEATH)
		if (obj->item_type == ITEM_TOKEN || obj->item_type == ITEM_MAGNUS_CARD){
			continue;
		}
		if (IS_SET(obj->extra2_flags, OBJEXTRA2_PERMANENT) || IS_SET(obj->extra2_flags, OBJEXTRA2_KEY_ITEM)){
		continue;
		}

		// pendants which are currently being worn stay with
		// a player after death
		if( GAMESETTINGMF1( GAMESETMF1_PENDANTS_ENABLED ) ){
			if (obj->wear_loc==WEAR_PENDANT && obj->item_type == ITEM_PENDANT){
				continue;
			}
		}


		if (obj->wear_loc == WEAR_FLOAT){
			 floating = true;
		}

		obj_from_char( obj );

		if (obj->item_type == ITEM_POTION){
			 obj->timer = number_range(500,1000);
		}
		if (obj->item_type == ITEM_SCROLL){
			 obj->timer = number_range(1000,2500);
		}

		if (IS_SET(obj->extra_flags,OBJEXTRA_ROT_DEATH) && !floating){
			obj->timer = number_range(5,10);
			REMOVE_BIT(obj->extra_flags,OBJEXTRA_ROT_DEATH);
		}
		REMOVE_BIT(obj->extra_flags,OBJEXTRA_VIS_DEATH);

		// shop keepers
		if (keeper){
			if (number_range(1,10)<2){
				obj->timer = number_range(2,5);
			}else{
				extract_obj(obj);
				continue;
			}
		}

		if( IS_SET( obj->extra_flags, OBJEXTRA_INVENTORY )){
			 extract_obj( obj );
		}else{
			if (floating)
			{
				if (IS_OBJ_STAT(obj,OBJEXTRA_ROT_DEATH)) // get rid of it!
				{
					if (obj->contains != NULL)
					{
						OBJ_DATA *in, *in_next;
						act("$p evaporates,scattering its contents.", ch,obj,NULL,TO_ROOM);
						for (in = obj->contains; in != NULL; in = in_next)
						{
							in_next = in->next_content;
							obj_from_obj(in);
							obj_to_room(in,target_room);
						}
					}
					else
					{
						act("$p evaporates.", ch,obj,NULL,TO_ROOM);
					}
					extract_obj(obj);
				}
				else
				{
					act("$p falls to the floor.",ch,obj,NULL,TO_ROOM);
					obj_to_room(obj,target_room);
				}
			}
			else
			{
				obj_to_obj( obj, corpse );
			}
		}
	}
    obj_to_room( corpse, target_room );

	// if it is a player, resave the corpses
	if(!IS_NPC(ch)){
		do_save_corpses(NULL, "");
	}
	return;
}


/**************************************************************************/
// Improved Death_cry contributed by Diavolo.
void death_cry( char_data *ch, char_data *killer )
{
	ROOM_INDEX_DATA *was_in_room;
    char *msg;
    int door;
    int vnum;

    // no death cry at this stage for a vanishing body
    if (IS_SET(ch->form, FORM_INSTANT_DECAY))
    {
		return;
    }

	vnum = 0;
	msg = "You hear $n's death cry.";

    switch ( number_range(0, 15))
    {
    case  0: msg  = "$n hits the ground ... DEAD.";
		break;
    case  1:
		if (ch->material == 0)
		{
			msg  = "$n splatters blood on your armor.";
			break;
		}
    case  2:
		if (IS_SET(ch->parts,PART_GUTS))
		{
			msg = "$n spills $s guts all over the floor.";
			vnum = OBJ_VNUM_GUTS;
		}
		break;
    case  3:
		if (IS_SET(ch->parts,PART_HEAD))
		{
			msg  = "$n's severed head plops on the ground.";
			vnum = OBJ_VNUM_SEVERED_HEAD;
		}
		break;
    case  4:
		if (IS_SET(ch->parts,PART_HEART))
		{
			msg  = "$n's heart is torn from $s chest.";
			vnum = OBJ_VNUM_TORN_HEART;
		}
		break;
    case  5:
		if (IS_SET(ch->parts,PART_ARMS))
		{
			msg  = "$n's arm is sliced from $s dead body.";
			vnum = OBJ_VNUM_SLICED_ARM;
		}
		break;
	case  6:
		if (IS_SET(ch->parts,PART_LEGS))
		{
			msg  = "$n's leg is sliced from $s dead body.";
			vnum = OBJ_VNUM_SLICED_LEG;
		}
		break;
	case 7:
		if (IS_SET(ch->parts,PART_BRAINS))
		{
			msg = "$n's head is shattered, and $s brains splash all over you.";
			vnum = OBJ_VNUM_BRAINS;
		}
    }

    if ( vnum != 0 )
    {
		char buf[MSL];
		OBJ_DATA *obj;
		char *name;

		name            = ch->short_descr;
		obj             = create_object( get_obj_index( vnum ));
		obj->timer      = number_range( 4, 7 );

		sprintf( buf, obj->short_descr, name );
		free_string( obj->short_descr );
		obj->short_descr = str_dup( buf );

		sprintf( buf, obj->long_descr, name );
		free_string( obj->long_descr );
		obj->long_descr = str_dup( buf );

		if (obj->item_type == ITEM_FOOD)
		{
			if (IS_SET(ch->form,FORM_POISON))
				obj->value[3] = 1;
			else if (!IS_SET(ch->form,FORM_EDIBLE))
				obj->item_type = ITEM_TRASH;
		}

		obj_to_room( obj, ch->in_room );
    }

	if ( number_range(1,100) <= get_skill(killer, gsn_stifle ))
	{
		act( "You managed to kill $N before they could let out a final cry.", killer, NULL, ch, TO_CHAR );
		check_improve(ch,gsn_stifle,true,3);
		return;
	}

    act( msg, ch, NULL, NULL, TO_ROOM );


	// if ( IS_NPC(ch) )
	if ( IS_SET( ch->form, FORM_SENTIENT )
		|| !IS_NPC(ch))
		msg = "You hear someone's death cry";
	else
		msg = "You hear something's death cry";


    was_in_room = ch->in_room;

	if (was_in_room==NULL){
		return;
	}

    for ( door = 0; door < MAX_DIR; door++ )
    {
		EXIT_DATA *pexit;
		char tempmsg[MIL];

		sprintf( tempmsg, "%s from %s.", msg, death_cry_dir_name[rev_dir[door]]);
		if ( ( pexit = was_in_room->exit[door] ) != NULL
			&&   pexit->u1.to_room != NULL
			&&   pexit->u1.to_room != was_in_room )
		{
			ch->in_room = pexit->u1.to_room;
			act( tempmsg, ch, NULL, NULL, TO_ROOM );
		}
	}
    ch->in_room = was_in_room;

	return;
}


/**************************************************************************/
void affect_copy_to_char( char_data *ch, AFFECT_DATA *paf, int objlevel );
void affect_modify( char_data *ch, AFFECT_DATA *paf, bool fAdd );
/**************************************************************************/
// called by kill_char()
void raw_kill( char_data *victim, char_data *killer )
{
    int		i;

    stop_fighting( victim, true ); //Raw_kill

    death_cry( victim, killer );

    if (victim->mounted_on){
		dismount(victim);
	}

    if (victim->ridden_by){
		victim->ridden_by->println("Your mount has been killed, you fall to the ground!");
		dismount(victim->ridden_by);
    }

    make_corpse( victim, killer );
    victim->pksafe=30;

	if(GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
		victim->pksafe=3;
		if(victim->gold<400){
			victim->gold+=number_range(40,400);
			victim->println("`GYOUR GOLD HAS BEEN BOOSTED UP.`x");
		}
		victim->pknoquit=2;
		victim->println("`YYOU ARE PK SAFE FOR 3 MINUTES!!!. PKNOQUIT AT 2MINS :)`x");
	}else{
		victim->pknoquit=0;
	}
    victim->pknorecall=0;

    if ( IS_NPC(victim) )
    {
		victim->pIndexData->killed++;
		kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
//	if (victim->master){
//		return;}
		extract_char( victim, true );
		return;
    }

    extract_char( victim, false );
	while ( victim->affected ){
		affect_remove( victim, victim->affected );
	}
    victim->affected_by = race_table[victim->race]->aff;
	victim->subdued = false;
    for (i = 0; i < 4; i++){
		victim->armor[i]= 100;
	}
    victim->position    = POS_RESTING;
    victim->hit         = UMAX( 1, victim->hit  );
    victim->mana        = UMAX( 1, victim->mana );
	victim->move		= UMAX( 1, victim->move );

	reset_char(victim); // easiest way to fix problems with death bugs
	landchar(victim);	// make sure they die on the ground
	OBJ_DATA *graft;
	AFFECT_DATA *paf;
	for ( graft = victim->carrying; graft != NULL; graft = graft->next_content ){
            if ( graft->wear_loc != WEAR_NONE
				&& graft->item_type == ITEM_AUGMENT ){
				for(paf=OBJECT_AFFECTS(graft); paf; paf = paf->next )
				{
				if( paf->where==WHERE_OBJECTSPELL)	{
				affect_copy_to_char( victim, paf, graft->level);
				}else{
					if(paf->level<=victim->level){
					affect_modify( victim, paf, true );
					}
				}
			}
		}
	}
	save_char_obj(victim);
    return;
}
/**************************************************************************/
void group_bypass_killer_penatly(char_data *killer, char_data *victim, bool death)
{
	assert(!IS_NPC(victim));
	bool bypass_duel_used=false;

	char_data *gch;

	if(GAMESETTING2(GAMESET2_BYPASSDUEL_REDUCES_KARNS)){
		for ( gch = killer->in_room->people; gch; gch = gch->next_in_room )
		{
			if ( !IS_NPC(gch) && gch->fighting==victim && is_same_group( gch, killer))
			{

				if(gch->duels){
					if(gch->duels->is_bypassingduel(victim)){
						bool lose_karn=false;
						bypass_duel_used=true;

						if(death){ // lose a karn
							lose_karn=true;
						}else{ // lose a karn some of the time
							gch->duel_subdues_before_karn_loss--;
							if(gch->duel_subdues_before_karn_loss<0){
								lose_karn=true;
								gch->duel_subdues_before_karn_loss=4;
							}
						}

						if(lose_karn){
							gch->pcdata->karns--;
							if(GAMESETTING(GAMESET_NOPERMDEATH) && victim->pcdata->karns<0){
								victim->pcdata->karns=0;
							}
							if(gch->pcdata->karns<0) // if they had a karn to lose
							{
								gch->println(
									"`xFrom within your body you feel a `Rburning sensation`x from within,\r\n"
									"a ball of light departs from your body and disappears!");
							}else{
								gch->println(
									"`xFrom within your body you feel a `Rburning sensation`x from within,\r\n"
									"you collapse to the ground, and pass out.");

							}
							save_char_obj(gch);
						}
					}
				}

			}
		}
	}

	// tell all those who are required to write a pknote to do so
	pkill_note_required_message(victim, victim, bypass_duel_used);
	for ( gch = killer->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( !IS_NPC(gch) && gch->fighting==victim && is_same_group( gch, killer))
		{
			pkill_note_required_message(gch, victim, bypass_duel_used);
		}
	}
}
/**************************************************************************/
// calculate the maximum level in a group attacking a victim
int group_max_level_attacking_victim(char_data *killer, char_data *victim )
{
	int max_level = 0;
	char_data *gch;
	for ( gch = killer->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( is_same_group( gch, killer)
			  && gch->fighting==victim
			  && max_level<gch->level)
		{
			max_level=gch->level;

		}
	}
	return max_level;
}
/**************************************************************************/
void group_gain( char_data *ch, char_data *victim )
{
	char_data *gch;
	char_data *lch;
	int xp;
	int members;
	int group_levels;
	int max_group_level;

	/*
	* Monsters don't get kill xp's or alignment changes.
	* P-killing doesn't help either.
	* Dying of mortal wounds or poison doesn't give xp to anyone!
	*/
	if ( victim == ch )
		return;

	// no xp gained for pkilling
	if(!IS_NPC(victim)){
		return;
	}

	if ( !ch || !ch->in_room || !victim)
	{
		if (!ch){
			bug("group_gain(): ch == NULL!");
		}else{
			if (!ch->in_room){
				bug("group_gain(): ch->in_room == NULL!");
			}else{
				bug("group_gain(): victim == NULL!");
			}
		}
		return;
	}
	if (IS_NPC(victim)){
       if (ch->questmob == victim->pIndexData->vnum){
		ch->questmobamount -= 1;
		if (ch->questmobamount >= 1){
			ch->printlnf("You have %d more to go!", ch->questmobamount);}
		else{
		ch->println("`#`YYou have almost completed your QUEST!\n\r");
        ch->println("`YReturn to the questmaster before your time runs out!`^\n\r");}
        }
	}

	members = 0;
	group_levels = 0;
	max_group_level= 0; // max level in killer group
	for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( is_same_group( gch, ch ) )
		{
			if( victim->level<11 && !IS_NPC(ch) && !IS_NPC(victim)
				&& !IS_LETGAINED(victim) && GAMESETTING_LETGAINING_IS_REQUIRED )
			{
				if(!GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
					gch->pkool+=4000;
				}
			}

			// if it isn't a mount being ridden then include it
			if(!IS_MOUNTED(gch)){
				members++;
				group_levels += IS_NPC(gch) ? gch->level / 2 : gch->level;
				max_group_level=UMAX(max_group_level, gch->level);
			}
		}
	}

	if ( members == 0 ){
		bug("Group_gain: members=0.");
		members=1;
		group_levels=ch->level;
		max_group_level=ch->level;
	}

	lch = (ch->leader != NULL) ? ch->leader : ch;

	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		OBJ_DATA *obj;
		OBJ_DATA *obj_next;

		if ( !is_same_group( gch, ch ) || IS_NPC(gch) ){

		if (gch->master){
//		gch->master->println("Pet level");
        if (gch->level <= gch->master->level){
		gch->exp = gch->exp+number_range(1,300);
		}
		}

			continue;
		}

		/*      Taken out, add it back if you want it
			if ( gch->level - lch->level >= 5 )
			{
			gch->println("You are too high for this group.");
			continue;
			}

			  if ( gch->level - lch->level <= -5 )
			  {
			  gch->println("You are too low for this group.");
			  continue;
			  }
		*/

		xp = xp_compute( gch, victim, group_levels);

		// Kal Jan 2004, based on Ixliam's code
		if(GAMESETTING5(GAMESET5_ADDICT_XP_BONUS) && !IS_NPC(gch)){
			char_data *tempch, *addict=NULL;
     		for ( tempch= player_list; tempch; tempch= tempch->next_player)
			{
				if ( !IS_IMMORTAL(tempch)
					&& tempch->level>10
					&& !IS_OOC(tempch)
					&& tempch->idle<2)
				{
					addict=tempch;
				}
			}
			if(xp && addict && gch && addict== gch && dice(1,4)==1)
			{
//				gch->println("`YADDICT EXP BONUS!!!`x");
				// give xp in range (xp+1) and (xp+1)*150%
				// to the player who has been on the longest
				xp = number_range(xp+1, (xp+1)*3/2);
				if (xp >=301){
					xp = 300;
				}
			}
		}

		if(IS_SET(gch->comm, COMM_BUILDING)){
			gch->printlnf("`=\xa6If you weren't in building mode "
				"you would receive %d xp.`x", xp );
		}else{
		    if (gch->level<=90 || IS_SET(gch->act,PLR_IS_HERO) ) {
			gch->printlnf("`=\xa6You receive %d experience points.`x", xp );
		}
			gain_exp( gch, xp );
		}

		for ( obj = ch->carrying; obj != NULL; obj = obj_next )
			{
				obj_next = obj->next_content;
				if ( obj->wear_loc == WEAR_NONE )
					continue;
		if ( IS_SET( obj->extra2_flags, OBJEXTRA2_RELIC) && obj->xp_tolevel > 0
			&& obj->wear_loc != WEAR_SHEATHED
			&& obj->wear_loc != WEAR_CONCEALED
			/*&& obj->plevel <= 9*/){
       gain_object_exp( ch, obj, xp );
            }
		}
		if (!IS_IMMORTAL( ch )) {

			for ( obj = ch->carrying; obj != NULL; obj = obj_next )
			{
				obj_next = obj->next_content;
				if ( obj->wear_loc == WEAR_NONE )
					continue;

				if ( ( IS_OBJ_STAT(obj, OBJEXTRA_ANTI_EVIL)  && IS_EVIL(ch) )
					||   ( IS_OBJ_STAT(obj, OBJEXTRA_ANTI_GOOD)    && IS_GOOD(ch) )
					||   ( IS_OBJ_STAT(obj, OBJEXTRA_ANTI_NEUTRAL) && IS_NEUTRAL(ch))
//					|| ( IS_OBJ2_STAT(obj, OBJEXTRA2_ANTI_CHAOS)   && IS_TEND_CHAOTIC(ch))
//					|| ( IS_OBJ2_STAT(obj, OBJEXTRA2_ANTI_LAW)     && IS_TEND_LAWFUL(ch))
//					|| ( IS_OBJ2_STAT(obj, OBJEXTRA2_ANTI_BALANCE) && IS_TEND_NEUTRAL(ch)))
                    )
				{

					act( "You are zapped by $p.", ch, obj, NULL, TO_CHAR );
					act( "$n is zapped by $p.",   ch, obj, NULL, TO_ROOM );
                        if ( IS_OBJ_STAT(obj, OBJEXTRA_NODROP) || IS_OBJ_STAT(obj, OBJEXTRA_NOREMOVE)){
                        ch->hit = ch->hit -1;
                        }else{
                        obj_from_char( obj );
                        obj_to_room( obj, ch->in_room );
                    }
				}
			}
		}
	}
	return;
}

/**************************************************************************/
/*
 * Compute xp for a kill.
 * Also adjust alignment of killer.
 * Edit this function to change xp computations.
 */
int xp_compute( char_data *gch, char_data *victim, int total_levels)
{
	char buf[MSL];
	int xp,base_exp;
	int level_range;
	int level;
	int lower_xp_level;

	//align mod
	if(victim->alliance < -349){
		gch->alliance += 25;
	}else if(victim->alliance > 350){
		gch->alliance -= 25;
	}

	if(gch->alliance > 1000){
		gch->alliance = 1000;
	}else if(gch->alliance < -1000){
		gch->alliance = -1000;
	}

	// abort the whole process if we can
	if(	victim->no_xp	// mob previously subdued
		|| IS_NPC(gch)  // mobs don't get xp
		|| gch->pcdata->xp_penalty>0  // player still has an outstanding xp penalty
		|| IS_AFFECTED(gch, AFF_CHARM)){ // charmed player
		return 0;
	}

//   if(	IS_NPC(gch) && gch->master!=NULL ){ // unowned mobs don't get xp
//       gch->master->println( "Got exp." );
//		return 10;
//	}

	level=gch->level;
	lower_xp_level=victim->highest_level_to_do_damage-8;
	if(level<lower_xp_level){
		level= lower_xp_level + (lower_xp_level - level);
	}

	level_range = victim->level - level;

	// compute the base exp
	switch (level_range)
	{
	default :	base_exp =  0;	break;
	case -9 :	base_exp =  1;	break;
	case -8 :	base_exp =  2;	break;
	case -7 :	base_exp =  5;	break;
	case -6 :	base_exp = 20;	break;
	case -5 :	base_exp = 22;	break;
	case -4 :	base_exp = 23;	break;
	case -3 :	base_exp = 25;	break;
	case -2 :	base_exp = 27;	break;
	case -1 :	base_exp = 29;	break;
	case  0 :	base_exp = 40;	break;
	case  1 :	base_exp = 45;	break;
	case  2 :	base_exp = 105;	break;
	case  3 :	base_exp = 145;	break;
	case  4 :	base_exp = 160;	break;
	}

	if (level_range > 4){
		base_exp = 160 + 7 * (level_range - 4);
	}

	xp = base_exp;

	// calculate exp multiplier
	if(IS_NPC(victim)){
		xp = xp * victim->pIndexData->xp_mod / 100;
	}


	// do alignment computations
	if (IS_GOOD(gch)){
		if (IS_EVIL(victim))
			xp*=14/10;
		else if (IS_NEUTRAL(victim))
			xp*=12/10;
	}else if (IS_NEUTRAL(gch)){
		if (IS_EVIL(victim))
			xp*=12/10;
		else if (IS_GOOD(victim))
			xp*=12/10;
	}else if (IS_EVIL(gch)){
		if (IS_NEUTRAL(victim))
			xp*=115/100;
		else
			if (IS_GOOD(victim))
				xp*=13/10;
	}

	// more exp at the low levels
	if (level < 6){
		xp = 5 * xp / (level + 2);
	}
	// reduced xp percent system
	if (!IS_NPC(gch) && gch->pcdata->reduce_xp_percent>0)
	{
		xp= (xp *gch->pcdata->reduce_xp_percent)/100;
	}

	// global xp scaling system
	if(game_settings->global_xp_scale_value!=100){
		xp=xp*game_settings->global_xp_scale_value/100;
	}

	// randomize the rewards
	xp = number_range(xp * 3/4, xp);
	// adjust for grouping
	xp = ( 6 * xp * level/( UMAX(1,total_levels -1) ) ) / 5;

	// different xp scales for letgained and non-letgained
	if(IS_LETGAINED(gch) || GAMESETTING(GAMESET_NO_LETGAINING_REQUIRED)){
		if(level<5)
			xp=xp*45/10;
		else if(level<10)
			xp=xp*275/100;
		else if(level<15)
			xp=xp*2;
		else if(level<20)
			xp=xp*14/10;
		else if(level<25)
			xp=xp*12/10;
	}else{
		if(level<5)
			xp=xp*275/100;
		else if(level<10)
			xp=xp*2;
		else if(level<15)
			xp=xp*14/10;
		else if(level<20)
			xp=xp*12/10;
	}

	// less XP the higher the level over 45
//	if (level > 45 ){
//		xp =  25 * xp / (level - 20);
//	}

	// reduce xp for 40mins after a level is gained by upto 40%
	if(!IS_NPC(gch))
	{
		if(level>15)
		{
			int playedtime=GET_SECONDS_PLAYED(gch);
			int playval=playedtime-gch->pcdata->last_level;
			if(playval<2400) // 40 Minutes
			{
				playedtime=2400-playval; // number of seconds before 40mins after last level
				float less_percent=	1- ((float)playedtime/6000);
				float newxp=(float)xp*less_percent;

				if(newxp>(float)xp)
				{
					bugf("xp_compute: newxp=%f, xp=%f, lvl=%d, pv=%d, pt=%d",
						newxp, (float)xp, level, playval, playedtime);
				}
				else
				{
					xp=(int)newxp;
				}
			}
		}
	}
	if (xp >= 301){
		xp = 300;
	}
	// big XP notification
/*	if((xp>150-level) && (level>10) && (victim->level>12))
	{
		sprintf(buf, "BIG EXP: %s <%d> gained %d xp by killing %s [vnum: %d, lvl: %d]",
			gch->name,
			level,
			xp,
			victim->short_descr,
			(victim->pIndexData) ? victim->pIndexData->vnum : 0,
			victim->level);
		log_string(buf);
		wiznet(buf,gch,NULL,WIZ_SECURE,0,get_trust(gch));

//		if((victim->pIndexData != NULL) && (victim->pIndexData->level>9) )
//			victim->pIndexData->level-=2;
	}
*/
	// log their xp
	if (!IS_NPC(gch) && xp>0 && IS_SET(gch->act, PLR_LOG))
	{
		sprintf(buf, "->>>>>>>>>EXP LOG: %s gained %d xp by killing %s [vnum: %d]",
			gch->name,
			xp,
			victim->short_descr,
			(victim->pIndexData) ? victim->pIndexData->vnum : 0);
		append_playerlog( gch, buf);
	}
	return xp;
}

/**************************************************************************/
void dam_message( char_data *ch, char_data *victim,int dam,int dt,bool immune )
{
	char buf1[256], buf2[256], buf3[256];
	char bf[MSL];
	const char *vs;
	const char *vp;
	const char *attack="unset_attack_value_in_dam_message";
	char punct;
	OBJ_DATA *obj;
		obj = get_eq_char( ch, WEAR_WIELD );


	int nullatk;
	nullatk = 0;

	if (ch == NULL || victim == NULL)
		return;

	int vlev = victim->level;
	int alev = ch->level;
	if ((vlev -= alev) >= 11){
		dam = 0;
	}

	if ( dam > 999 && ch->level <= 92 && !IS_HERO(ch) && !IS_NPC(ch))
    {
	dam = 999;
    }

    if(GAMESETTING3(GAMESET3_STORM_DAMAGE_MESSAGES)){
        if( dam > victim->max_hit ) { vs="`SSLAYS`x"; vp="`SSLAYS`x";}
        else if( dam == 0 ) { vs="`Gmiss"; vp="`Gmisses";}
        else if( dam <= 1 ) { vs="`Bscratch"; vp="`Bscratches";}
        else if( dam <= 2 ) { vs="`Bbruise"; vp="`Bbruises";}
        else if( dam <= 3 ) { vs="`Bgraze"; vp="`Bgrazes";}
        else if( dam <= 4 ) { vs="`rhit"; vp="`rhits"; }
        else if( dam <= 5 ) { vs="`rinjure"; vp="`rinjures";}
        else if( dam <= 6 ) { vs="`rwound"; vp="`rwounds";}
        else if( dam <= 7 ) { vs="`rscar"; vp="`rscars";}
        else if( dam <= 8 ) { vs="`rmangle"; vp="`rmangles";}
        else if( dam <= 9 ) { vs="`Rmaul"; vp="`Rmauls";}
        else if( dam <= 10 ) { vs="`RDECIMATE"; vp="`RDECIMATES";}
        else if( dam <= 12 ) { vs="`RDEVASTATE"; vp="`RDEVASTATES";}
        else if( dam <= 14 ) { vs="`RMAIM"; vp="`RMAIMS";}
        else if( dam <= 16 ) { vs="`RCRIPPLE"; vp="`RCRIPPLES";}
        else if( dam <= 18 ) { vs="`RMUTILATE"; vp="`RMUTILATES";}
        else if( dam <= 20 ) { vs="`RDISMEMBER"; vp="`RDISMEMBERS";}
        else if( dam <= 22 ) { vs="`RDISEMBOWEL"; vp="`RDISEMBOWELS";}
        else if( dam <= 24 ) { vs="`RMASSACRE"; vp="`RMASSACRES";}
        else if( dam <= 26 ) { vs="`RVICTIMIZE"; vp="`RVICTIMIZES";}
        else if( dam <= 28 ) { vs="`RTEAR INTO"; vp="`RTEARS INTO";}
        else if( dam <= 30 ) { vs="`ROBLITERATE"; vp="`ROBLITERATES";}
        else if( dam <= 32 ) { vs="`YBLAST"; vp="`YBLASTS";}
        else if( dam <= 34 ) { vs="`R-= BUTCHER =-"; vp="`R-= BUTCHERS =-";}
        else if( dam <= 36 ) { vs="`W-=* OVERWHELM *=-"; vp="`W -=* OVERWHELMS *=-";}
        else if( dam <= 38 ) { vs="`r***`RDEMOLISH`r***"; vp="`r***`RDEMOLISHES`r***";}
        else if( dam <= 40 ) { vs="`r****`R SHRED `r****"; vp="`r**** `RSHREDS `r****";}
        else if( dam <= 43 ) { vs="`c-=`C*`c=- `CDESTROY `c-=`C*`c=-"; vp="`c-=`C*`c=- `CDESTROYS `c-=`C*`c=-";}
        else if( dam <= 47 ) { vs="`R*** `BPULVERIZE `R***"; vp="`R*** `BPULVERIZES `R***";}
        else if( dam <= 50 ) { vs="`c-== `WVAPORIZE `c==-"; vp="`c-== `WVAPORIZES `c==-";}
        else if( dam <= 53 ) { vs="`c-=`W*`c= `RSMITE `c=`W*`c=-"; vp="`c-=`W*`c= SMITES `c=`W*`c=-";}
        else if( dam <= 57 ) { vs="`R>>> RIP APART <<<"; vp="`R>>> RIPS APART <<<";}
        else if( dam <= 60 ) { vs="`R<`r-`R=`r-`R> `WTORMENT `R<`r-`R=`r-`R>"; vp="`R<`r-`R=`r-`R> `WTORMENTS `R<`r-`R=`r-`R>";}
        else if( dam <= 65 ) { vs="`R<`r-`R=`r-`R> `BRAVAGE `R<`r-`R=`r-`R>"; vp="`R<`r-`R=`r-`R> `BRAVAGES `R<`r-`R=`r-`R>";}
        else if( dam <= 70 ) { vs="`G<> `RDISFIGURE `G<>"; vp="`G<> `RDISFIGURES `G<>";}
        else if( dam <= 75 ) { vs="`R<`c*`R> `rFISSURE `R<`c*`R>"; vp="`R<`c*`R> `rFISSURES `R<`c*`R>";}
        else if( dam <= 80 ) { vs="`b<><><> `YSUNDER `b<><><>"; vp="`b<><><> `YSUNDERS `b<><><>";}
        else if( dam <= 85 ) { vs="`R<*><*> `GWASTE `R<*><*>"; vp="`R<*><*> `GWASTES `R<*><*>";}
        else if( dam <= 90 ) { vs="`RA`rN`RN`rI`RH`rI`RL`rA`RT`rE"; vp="`RA`rN`RN`rI`RH`rI`RL`rA`RT`rE`RS";}
        else if( dam <= 95 ) { vs="`WA`sSSASSINATE"; vp="`WA`SSSASSINATES";}
        else if( dam <= 100) { vs="`R</\\> EVISCERATE </\\>"; vp="`R</\\> EVISCERATES </\\>";}
        else if( dam <= 110) { vs="`B</^\\> `REXTERMINATE `B</^\\>"; vp="`B</^\\> `REXTERMINATES `B</^\\>";}
        else if( dam <= 120) { vs="`B<=-=> `cERADICATE `B<=-=>"; vp="`B<=-=> `cERADICATES `B<=-=>";}
        else if( dam <= 130) { vs="`R+`r.`R+`r.`R+ `RC`rR`RU`rC`RI`rF`RY `R+`r.`R+`r.`R+"; vp="`R+`r.`R+`r.`R+ `RC`rR`RU`rC`RI`rF`RI`rE`RS `R+`r.`R+`r.`R+";}
        else if( dam <= 140) { vs="`WSLAUGHTER"; vp="`WSLAUGHTERS";}
        else if( dam <= 150) { vs="`YNEUTRALIZE"; vp="`YNEUTRALIZES";}
        else if( dam <= 160) { vs="`RNULLIFY"; vp="`RNULLIFIES";}
        else if( dam <= 170) { vs="`BENGULF"; vp="`BENGULFS";}
        else if( dam <= 180) { vs="`RL`rA`RY `RW`rA`RS`rT`RE `rT`RO"; vp="`RL`rA`RY`rS `RW`rA`RS`rT`RE `RT`rO";}
        else if( dam <= 190) { vs="`BW`bR`BE`bA`BK `bH`BA`bV`BO`bC `BU`bP`BO`bN"; vp="`BW`bR`BE`bA`BK`bS `BH`bA`BV`BO`BC `BU`bP`BO`bN";}
        else if( dam <= 200) { vs="`CC`cO`wM`WP`CL`cE`wT`WE`CL`cY `wR`WU`CI`cN"; vp="`CC`cO`wM`WP`CL`cE`wT`WE`CL`cY `wR`WU`CI`cN`wS";}
        else if( dam <= 210) { vs="`WD`wO `WU`wN`WS`wP`WE`wA`WK`wA`WB`wL`WE `WT`wH`WI`wN`WG`wS `WT`wO"; vp="`WD`wO`WE`wS `WU`wN`WS`wP`WE`wA`WK`wA`WB`wL`WE `WT`wH`WI`wN`WG`wS `WT`wO";}
        else  { vs="`WD`wO `RILLEGAL `WT`wH`WI`wN`WG`wS `WT`wO"; vp="`WD`wO`WE`wS `RILLEGAL `WT`wH`WI`wN`WG`wS `WT`wO";}
	}else{
		if ( dam > victim->max_hit ) { vs = "SLAY"; vp = "SLAYS";			}
		else if ( dam == 0  ) { vs = "miss";		vp = "misses";			}
		else if ( dam <= 2  ) { vs = "scratch";		vp = "scratches";		}
		else if ( dam <= 4  ) { vs = "graze";		vp = "grazes";			}
		else if ( dam <= 6  ) { vs = "hit";			vp = "hits";			}
		else if ( dam <= 8  ) { vs = "injure";		vp = "injures"; 		}
		else if ( dam <= 10 ) { vs = "wound";		vp = "wounds";			}
		else if ( dam <= 12 ) { vs = "maul";		vp = "mauls";			}
		else if ( dam <= 14 ) { vs = "decimate";	vp = "decimates";		}
		else if ( dam <= 16 ) { vs = "devastate";	vp = "devastates";		}
		else if ( dam <= 20 ) { vs = "maim";		vp = "maims";			}
		else if ( dam <= 25 ) { vs = "MUTILATE";	vp = "MUTILATES";		}
		else if ( dam <= 30 ) { vs = "DISEMBOWEL";	vp = "DISEMBOWELS"; 	}
		else if ( dam <= 40 ) { vs = "DISMEMBER";	vp = "DISMEMBERS";		}
		else if ( dam <= 50 ) { vs = "MASSACRE";	vp = "MASSACRES";		}
		else if ( dam <= 60 ) { vs = "MANGLE";		vp = "MANGLES"; 		}
		else if ( dam <= 70 ) { vs = "*** DEMOLISH ***";
					  vp = "*** DEMOLISHES ***";							}
		else if ( dam <=  80 ) { vs = "*** DEVASTATE ***";
					 vp = "*** DEVASTATES ***";		 						}
		else if ( dam <= 100)  { vs = "=== OBLITERATE ===";
					 vp = "=== OBLITERATES ===";							}
		else if ( dam <= 125)	{ vs = ">>> ANNIHILATE <<<";
					 vp = ">>> ANNIHILATES <<<";							}
		else if ( dam <= 150)  { vs = "<<< ERADICATE >>>";
					 vp = "<<< ERADICATES >>>"; 							}
		else				   { vs = "do UNSPEAKABLE things to";
					 vp = "does UNSPEAKABLE things to"; 					}
	}

	char vsbuf[MSL], vpbuf[MSL];
	sprintf(vsbuf, "`#%s`^", vs);
	sprintf(vpbuf, "`#%s`^", vp);
	vs=vsbuf;
	vp=vpbuf;

    punct   = (dam <= 24) ? '.' : '!';

    if ( dt == TYPE_HIT )
    {
		if (ch  == victim)
		{
			sprintf( buf1, "$n %s $melf%c",vp,punct);
			sprintf( buf2, "You %s yourself%c",vs,punct);
		}
		else
		{
			 sprintf( buf1, "$n %s $N%c",  vp, punct );
			 sprintf( buf2, "You %s $N%c", vs, punct );
			 sprintf( buf3, "$n %s you%c", vp, punct );
		}
	}
	else
	{
		if ( dt >= 0 && dt < MAX_SKILL )
		{
			attack = skill_table[dt].noun_damage;
		}
		else
		{
			if ( dt >= TYPE_HIT
				&& dt <= TYPE_HIT + MAX_DAMAGE_MESSAGE)
			{
				attack = attack_table[dt - TYPE_HIT].noun;
			}else{
				 bugf( "Dam_message: bad dt %d.", dt );
				 dt  = TYPE_HIT;
				 attack  = attack_table[0].name;
			}
		}
		if (obj != NULL){
		if (obj->value[0]==WEAPON_LONG_BOW ||
				obj->value[0]==WEAPON_SHORT_BOW ||
				obj->value[0]==WEAPON_CROSSBOW ||
				obj->value[0]==WEAPON_RIFLE ||
				obj->value[0]==WEAPON_BLASTRIFLE ||
				obj->value[0]==WEAPON_BLASTPISTOL ||
				obj->value[0]==WEAPON_PISTOL)
			{
			if ((ch->in_room) != (victim->in_room)){
				nullatk = 3;
				attack = attack_table[obj->value[3]].noun;
			}
		}
		}
		if(attack==NULL)
		{

			if (!IS_SET( victim->form, FORM_UNDEAD))
			{
				nullatk = 1;

			}else
			{
				nullatk = 2;
				bugf("dam_message(): NULL attack value... dt=%d, TYPE_HIT=%d, ch=%s, victim=%s",
				dt, TYPE_HIT, PERS(ch, NULL), PERS(victim, NULL));

			}
		}

		/*if(attack==NULL){
			attack = attack_table[obj->value[3]].noun;
		}*/


		if (immune)
		{
			if (ch == victim)
			{
				sprintf(buf1,"$n is unaffected by $s own %s.",attack);
				sprintf(buf2,"Luckily, you are immune to that.");
			}
			else
			{
				sprintf(buf1,"$N is unaffected by $n's %s!",attack);
				sprintf(buf2,"$N is unaffected by your %s!",attack);
				sprintf(buf3,"$n's %s is powerless against you.",attack);
			}
		}
		else
		{
			if (ch == victim)
			{
				sprintf( buf1, "$n's %s %s $m%c",attack,vp,punct);
				sprintf( buf2, "Your %s %s you%c",attack,vp,punct);
			}
			else{
			if (nullatk != 1){
				sprintf( buf1, "$n's %s %s $N%c",  attack, vp, punct );
				sprintf( buf2, "Your %s %s $N%c",  attack, vp, punct );
				sprintf( buf3, "$n's %s %s you%c", attack, vp, punct );
			}

			else
			{
				sprintf( buf3, "$n's positive energy %s you!", vp );
				sprintf( buf2, "Your positive energy %s $N !", vp);
				sprintf( buf1, "$n %s $N with positive energy!", vp );
			}
			}
		}
	}

	// do autodamage for the attacker
	if(dam && !GAMESETTING2(GAMESET2_NO_AUTODAMAGE_COMMAND)
		&& HAS_CONFIG2(ch, CONFIG2_AUTODAMAGE))
	{
		strcat(buf2, FORMATF(" [%d]", dam));
	}

	// display the damage message
    if (ch == victim)
    {
		act(buf1,ch,NULL,NULL,TO_ROOM);

		act(buf2,ch,NULL,NULL,TO_CHAR);
    }
    else
    {
		act( buf1, ch, NULL, victim, TO_NOTVICT );
		sprintf(bf,"`g%s`x", buf2);
		act( bf, ch, NULL, victim, TO_CHAR );

		// do autodamage for the victim
		if(dam && !GAMESETTING2(GAMESET2_NO_AUTODAMAGE_COMMAND)
			&& HAS_CONFIG2(victim, CONFIG2_AUTODAMAGE))
		{
			strcat(buf3, FORMATF(" [%d]", dam));
		}

		sprintf(bf,"`R%s`x",buf3);
		act( bf, ch, NULL, victim, TO_VICT );
    }
    return;
}

/**************************************************************************/
/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( char_data *ch, char_data *victim )
{
	OBJ_DATA *obj;

	if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
	if ( ( obj = get_eq_char( victim, WEAR_SECONDARY) ) == NULL )

	{
		if ( ( obj = get_eq_char( victim, WEAR_SECONDARY) ) == NULL ){

			return;
		}
	}

	if ( IS_OBJ_STAT(obj,OBJEXTRA_NOREMOVE))
	{
		act("$S weapon won't budge!",ch,NULL,victim,TO_CHAR);
		act("$n tries to disarm you, but your weapon won't budge!",
			ch,NULL,victim,TO_VICT);
		act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);
		return;
	}

    act( "$n DISARMS you and sends your weapon flying!",
		ch, NULL, victim, TO_VICT    );
    act( "You disarm $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n disarms $N!",  ch, NULL, victim, TO_NOTVICT );

    obj_from_char( obj );
    if ( IS_OBJ_STAT(obj,OBJEXTRA_NODROP) || IS_OBJ_STAT(obj,OBJEXTRA_INVENTORY) )
		obj_to_char( obj, victim );
    else
    {
		obj_to_room( obj, victim->in_room );
		if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
			get_obj(victim,obj,NULL);
    }

    return;
}

/**************************************************************************/
void entangle( char_data *ch, char_data *victim )
{
	OBJ_DATA *obj;

	if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
	if ( ( obj = get_eq_char( victim, WEAR_SECONDARY) ) == NULL )
	{
		if ( ( obj = get_eq_char( victim, WEAR_SECONDARY) ) == NULL ){
			return;
		}
	}

	if ( IS_OBJ_STAT(obj,OBJEXTRA_NOREMOVE))
	{
		act("$S weapon won't budge!",ch,NULL,victim,TO_CHAR);
		act("$n tries to entangle your weapon but your weapon won't budge!",ch,NULL,victim,TO_VICT);
		act("$n tries to entangle $N's weapon, but fails.",ch,NULL,victim,TO_NOTVICT);
		return;
	}

    act( "$n `Bentangles`x $p and grabs it!!",	ch, obj, victim, TO_VICT    );
    act( "You `Bentangle`x $N's weapon!",  ch, NULL, victim, TO_CHAR    );
    act( "$n `Bentangles`x $N's weapon!",  ch, NULL, victim, TO_NOTVICT );

    obj_from_char( obj );
    if ( IS_OBJ_STAT(obj,OBJEXTRA_NODROP) || IS_OBJ_STAT(obj,OBJEXTRA_INVENTORY) )
		obj_to_char( obj, victim );
    else
    {
		obj_to_char( obj, ch );
    }
    return;
}

/**************************************************************************/
void do_berserk( char_data *ch, char *)
{
	int chance, hp_percent;

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	if (IS_SET(ch->psionic_affects, PAFF_ADRENCONTROL)){
		ch->printf( "You are controlling your adrenaline.\r\n" );
		check_improve(ch, gsn_adrenaline_control, true, 3);
	return;}

	if ((chance = get_skill(ch,gsn_berserk)) == 0
		||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BERSERK))
		||  (!IS_NPC(ch)
		&&   ch->level < skill_table[gsn_berserk].skill_level[ch->clss]))
	{
		ch->println("You turn red in the face, but nothing happens.");
		return;
	}

	if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk)
		||  is_affected(ch,gsn_frenzy))
	{
		ch->println("You get a little madder.");
		return;
	}

	if (IS_AFFECTED(ch,AFF_CALM))
	{
		ch->println("You're feeling to mellow to berserk.");
		return;
	}

    if (ch->mana < 50)
    {
		ch->println("You can't get up enough energy.");
		return;
    }

    /* modifiers */

    /* fighting */
    if (ch->position == POS_FIGHTING)
		chance += 10;

    /* damage -- below 50% of hp helps, above hurts */
    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
		AFFECT_DATA af;

		WAIT_STATE(ch,PULSE_VIOLENCE);
		ch->mana -= 50;
		if (ch->race != race_lookup("liche"))
		ch->move /= 2;

		/* heal a little damage */
		ch->hit += ch->level * 2;
		ch->hit = UMIN(ch->hit,ch->max_hit);

		ch->println("Your pulse races as you are consumed by rage!");
		act("$n gets a wild look in $s eyes.",ch,NULL,NULL,TO_ROOM);
		check_improve(ch,gsn_berserk,true,2);

		af.where        = WHERE_AFFECTS;
		af.type         = gsn_berserk;
		af.level        = ch->level;
		af.duration     = number_fuzzy(ch->level / 8);
		af.modifier     = UMAX(1,ch->level/5);
		af.bitvector    = AFF_BERSERK;

		af.location     = APPLY_HITROLL;
		affect_to_char(ch,&af);

		af.location     = APPLY_DAMROLL;
		affect_to_char(ch,&af);

		af.modifier     = UMAX(10,10 * (ch->level/5));
		af.location     = APPLY_AC;
		affect_to_char(ch,&af);
    }

    else
    {
		WAIT_STATE(ch,3 * PULSE_VIOLENCE);
		ch->mana -= 25;
		if (ch->race != race_lookup("liche"))
		ch->move /= 2;

		ch->println("Your pulse speeds up, but nothing happens.");
		check_improve(ch,gsn_berserk,false,2);
	}

	if (ch->position == POS_FIGHTING){
		strip_tension (ch);
    }

}

/**************************************************************************/
void do_bash( char_data *ch, char *argument )
{
	char arg[MIL];
	char_data *victim;
	int chance;
	int dr; // damage result

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	one_argument(argument,arg);

	if ( (chance = get_skill(ch,gsn_bash)) == 0
		||      (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH))
		||      (!IS_NPC(ch)
		&&       ch->level < skill_table[gsn_bash].skill_level[ch->clss]))
	{
		if (!IS_CONTROLLED(ch))
		{
			ch->println("Bashing? What's that?");
			return;
		}
	}

	if (arg[0] == '\0')
	{
		victim = ch->fighting;
		if (victim == NULL)
		{
			ch->println("But you aren't fighting anyone!");
			return;
		}
	}

	else if ((victim = get_char_room(ch,arg)) == NULL)
	{
		ch->println("They aren't here.");
		return;
	}

	if (victim->position < POS_FIGHTING)
	{
		act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
		return;
	}

	if (victim == ch)
	{
		ch->println("You try to bash your brains out, but fail.");
		return;
	}

	if ((IS_AFFECTED(victim, AFF_FLYING) || IS_SET(victim->psionic_affects, PAFF_LEVITATE))
		&& (!IS_AFFECTED(ch, AFF_FLYING) ||!IS_SET(victim->psionic_affects, PAFF_LEVITATE)))
	{
		ch->println("You can't reach them since they are flying and you are not.");
		return;
	}


	if (is_safe(ch,victim))
		return;

	if ( !can_initiate_combat( ch, victim, CIT_GENERAL )) return;

	if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
		act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
		return;
    }

    // modifiers

    // size  and weight
    chance += ch->carry_weight / 250;
    chance -= victim->carry_weight / 200;

    if (ch->size < victim->size){
		chance += (ch->size - victim->size) * 15;
    }else{
		chance += (ch->size - victim->size) * 10;
	}


    // stats
    chance += ch->modifiers[STAT_ST];
    chance -= victim->modifiers[STAT_QU];
    chance -= GET_AC(victim,AC_BASH) /25;
    // speed
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
		chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
		chance -= 30;

    // level
    chance += (ch->level - victim->level);

    if (IS_CONTROLLED(ch)){
        chance*=3;
    };
	if (IS_SET(victim->psionic_affects, PAFF_BLUR)){
		chance -= 3 * (get_skill(victim,gsn_blur) - chance);
		check_improve(victim, gsn_blur, true, 1);}
    if (!IS_NPC(victim)
		&& chance < get_skill(victim,gsn_dodge) )
		{   /*
			act("$n tries to bash you, but you dodge it.",ch,NULL,victim,TO_VICT);
			act("$N dodges your bash, you fall flat on your face.",ch,NULL,victim,TO_CHAR);
			WAIT_STATE(ch,skill_table[gsn_bash].beats);
		return;*/
		chance -= 3 * (get_skill(victim,gsn_dodge) - chance);
    }

    // now the attack
    if (number_percent() < chance )
    {
		DAZE_STATE(victim, 3 * PULSE_VIOLENCE);
		WAIT_STATE(ch,skill_table[gsn_bash].beats);
		victim->position = POS_RESTING;
		dr=damage(ch,victim,number_range(2,2 + 2 * ch->size + chance/20),gsn_bash,
			DAM_BASH,false);

		if(dr){
			act("$n sends you sprawling with a powerful bash!$t",
				ch, autodamtext(victim, dr) ,victim,TO_VICT);
			act("You powerfully slam into $N, and send $M flying!$t",
				ch, autodamtext(ch, dr) ,victim,TO_CHAR);
			act("$n sends $N sprawling with a powerful bash.",
				ch,NULL,victim,TO_NOTVICT);
		}else{
			act("$n sends you sprawling with a bash!$t",
				ch, autodamtext(victim, dr),victim,TO_VICT);
			act("You slam into $N, and send $M flying!$t",
				ch, autodamtext(ch, dr) ,victim,TO_CHAR);
			act("$n sends $N sprawling with a bash.",
				ch,NULL,victim,TO_NOTVICT);
		}
		check_improve(ch,gsn_bash,true,1);
    }
	else
	{
		damage(ch,victim,0,gsn_bash,DAM_BASH,false);
		act("You fall flat on your face!", ch,NULL,victim,TO_CHAR);
		act("$n falls flat on $s face.", ch,NULL,victim,TO_NOTVICT);
		act("You evade $n's bash, causing $m to fall flat on $s face.",
			ch,NULL,victim,TO_VICT);
		check_improve(ch,gsn_bash,false,1);
		ch->position = POS_RESTING;
		WAIT_STATE(ch,skill_table[gsn_bash].beats * 3/2);
    }
}

/**************************************************************************/
void do_dirt( char_data *ch, char *argument )
{
    char arg[MIL];
    char_data *victim;
    int chance;
    int size_mod;

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_dirt_kicking)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK_DIRT))
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_dirt_kicking].skill_level[ch->clss]))
    {
		ch->println("You get your feet dirty.");
		return;
    }

    if (arg[0] == '\0')
    {
		victim = ch->fighting;
		if (victim == NULL)
		{
		    ch->println("But you aren't in combat!");
		    return;
		}
    }
	else if ((victim = get_char_room(ch,arg)) == NULL)
	{
		ch->println("They aren't here.");
		return;
    }

	if (IS_AFFECTED(victim,AFF_BLIND))
	{
		act("$E's already been blinded.",ch,NULL,victim,TO_CHAR);
		return;
	}

	if (IS_AFFECTED( ch, AFF_FLYING))
	{
		ch->println("You can't dirt kick while you fly.");
		return;
	}

	if (victim == ch)
    {
		ch->println("Very funny.");
		return;
    }

    if (is_safe(ch,victim))
		return;

	if ( !can_initiate_combat( ch, victim, CIT_GENERAL )) return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
		act("But $N is such a good friend!",ch,NULL,victim,TO_CHAR);
		return;
    }

    /* modifiers */

    /* dexterity */
    chance += ch->modifiers[STAT_QU];
    chance -= 2 * victim->modifiers[STAT_QU];

    /* mount size*/
	if (victim->mounted_on)
	{
		size_mod = (victim->mounted_on->size)-(ch->size)+1;
		if (size_mod<=0)
			size_mod=1;
		chance/=size_mod;
	}

    /* speed  */
	if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
		chance += 10;
	if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
		chance -= 25;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* sloppy hack to prevent false zeroes */
	if (chance % 5 == 0)
		chance += 1;

    /* terrain */

	switch(ch->in_room->sector_type)
	{
		case(SECT_INSIDE):              chance -= 20;   break;
		case(SECT_CITY):                chance -= 10;   break;
		case(SECT_FIELD):               chance +=  5;   break;
		case(SECT_FOREST):                              break;
		case(SECT_HILLS):                               break;
		case(SECT_MOUNTAIN):            chance -= 10;   break;
		case(SECT_DESERT):              chance += 10;   break;
		case(SECT_CAVE):				chance +=  5;	break;
		case(SECT_LAVA):				chance -= 90;   break;
		case(SECT_RUBBLE):              chance +=  5;   break;
		default:						chance	=  0;	break;
    }

	if ( chance == 0 )
    {
		ch->println("There isn't any dirt to kick.");
		return;
    }

    /* now the attack */
	if (number_percent() < chance)
	{
		AFFECT_DATA af;
		act("$n is blinded by the dirt in $s eyes!",victim,NULL,NULL,TO_ROOM);
		act("$n kicks dirt in your eyes!",ch,NULL,victim,TO_VICT);

		if (!IS_SET(ch->imm_flags,(IMM_BASH & IMM_WEAPON))){
			damage(ch,victim,number_range(2,5),gsn_dirt_kicking,DAM_NONE,false);
		}

		victim->println("You can't see a thing!");
		check_improve(ch,gsn_dirt_kicking,true,2);
		WAIT_STATE(ch,skill_table[gsn_dirt_kicking].beats);

		if (!IS_AFFECTED(victim,AFF_BLIND)) {

			af.where        = WHERE_AFFECTS;
			af.type         = gsn_dirt_kicking;
			af.level        = ch->level;
			af.duration     = 0;
			af.location     = APPLY_HITROLL;
			af.modifier     = -4;
			af.bitvector    = AFF_BLIND;

			affect_to_char(victim,&af);
		}
		else
		{
			act("$E's already been blinded.",ch,NULL,victim,TO_CHAR);
		}
    }
    else
	{
		act("$n is totally missed by the dirt!",victim,NULL,NULL,TO_ROOM);
		act("$n fails to kick dirt properly!",ch,NULL,victim,TO_VICT);
		damage(ch,victim,0,gsn_dirt_kicking,DAM_NONE,false);
		WAIT_STATE(ch,skill_table[gsn_dirt_kicking].beats*2);
    }
    return;
}

/**************************************************************************/
void do_trip( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    char arg[MIL];
    char_data *victim;
    int chance;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_trip)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TRIP))
    ||   (!IS_NPC(ch)
	  && ch->level < skill_table[gsn_trip].skill_level[ch->clss]))
    {
		ch->println("Tripping?  What's that?");
		return;
    }

// standard no mount checks etc
	if(IS_RIDING(ch))
	{
		ch->println("You can't trip others while riding a creature.");
		return;
	}


    if (arg[0] == '\0')
    {
		victim = ch->fighting;
		if (victim == NULL)
		{
			ch->println("But you aren't fighting anyone!");
			return;
		}
    }else if ((victim = get_char_room(ch,arg)) == NULL)
    {
		ch->println("They aren't here.");
		return;
    }

    if (is_safe(ch,victim))
	return;

    if ((IS_AFFECTED(victim,AFF_FLYING) || IS_SET(victim->psionic_affects, PAFF_LEVITATE))
       || (ch->mounted_on))
    {
		act("$S feet aren't on the ground.",ch,NULL,victim,TO_CHAR);
		return;
    }

    if (victim->position < POS_FIGHTING)
    {
		act("$N is already down.",ch,NULL,victim,TO_CHAR);
		return;
    }

    if (victim == ch)
    {
		ch->println("You fall flat on your face!");
		WAIT_STATE(ch,2 * skill_table[gsn_trip].beats);
		act("$n trips over $s own feet!",ch,NULL,NULL,TO_ROOM);
		return;
    }

	if ( !can_initiate_combat( ch, victim, CIT_GENERAL )) return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
		act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);
		return;
    }

    /* modifiers */

    /* size */
    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 10;  /* bigger = harder to trip */

    /* dex */
    chance += ch->modifiers[STAT_QU];
    chance -= victim->modifiers[STAT_QU] * 3 / 2;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance -= 20;

    // factor in level to chance of success
    chance += (ch->level - victim->level) * 2;

    // now the attack
    if (number_percent() < chance)
    {
		act("$n trips you and you go down!",ch,NULL,victim,TO_VICT);
		act("You trip $N and $N goes down!",ch,NULL,victim,TO_CHAR);
		act("$n trips $N, sending $M to the ground.",ch,NULL,victim,TO_NOTVICT);
		check_improve(ch,gsn_trip,true,1);

		msp_to_room(MSPT_COMBAT, MSP_SOUND_TRIP, 0,
					ch,
					true,
					false);

		msp_to_room(MSPT_COMBAT, MSP_SOUND_TRIP, 0,
					victim,
					true,
					false);

		DAZE_STATE(victim,2 * PULSE_VIOLENCE);
		WAIT_STATE(ch,skill_table[gsn_trip].beats);
		victim->position = POS_RESTING;
		damage(ch,victim,number_range(2, 2 +  2 * victim->size),gsn_trip,
			DAM_BASH,true);
    }
    else
    {
		damage(ch,victim,0,gsn_trip,DAM_BASH,true);
		WAIT_STATE(ch,skill_table[gsn_trip].beats*2/3);
		check_improve(ch,gsn_trip,false,1);
    }
}

/**************************************************************************/
void do_kill( char_data *ch, char *argument )
{
    char arg[MIL];
    char_data *victim;
	char buf[MSL];

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	sprintf (buf,"%s", argument);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
		ch->println("Kill whom?");
		return;
    }
	victim = get_char_room( ch, arg );
	OBJ_DATA 	*weapon = NULL;
	weapon = get_eq_char( ch, WEAR_WIELD );


    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		ch->println("They aren't here.");
		return;

    }
	if (weapon != NULL){
	if (weapon->value[0]==WEAPON_LONG_BOW ||
				weapon->value[0]==WEAPON_SHORT_BOW ||
				weapon->value[0]==WEAPON_CROSSBOW ||
				weapon->value[0]==WEAPON_RIFLE ||
				weapon->value[0]==WEAPON_BLASTRIFLE)
			{
			ch->println("Don't you think that shooting from a bit further away would be wise?");
			return;}
	}
	// checks for preventing the start of a pkill fight
    if ( !IS_NPC(victim) ) // directly attacking the player
    {
		if(GAMESETTING(GAMESET_PEACEFUL_MUD)){
			ch->wrapln("That is a player you are trying to attack. "
				"This is a peaceful mud where no pkilling is allowed.");
		}else{
			ch->wraplnf("That is a player you are trying to kill, "
				"type pkill %s if you really want to do that.", buf );
		}
		return;
    }else{
		if ( IS_SET(victim->act, ACT_PET)
				&& victim->leader
				&& !IS_NPC(victim->leader)
				&& (victim->leader->pet == victim)
				&& IS_SET(victim->affected_by, AFF_CHARM))
		{
			if(GAMESETTING(GAMESET_PEACEFUL_MUD)){
				ch->wrapln("That is a players pet you are trying to attack. "
					"This is a peaceful mud where no pkilling is allowed.");
			}else{
				ch->wraplnf("That is a players pet, which could start a pkill fight, "
					"type pkill %s if you really want to do that.", buf );
			}
			return;
		}
	}

    if ( victim == ch )
    {
		ch->println("You hit yourself.  Ouch!");
		multi_hit( ch, ch, TYPE_UNDEFINED );
		return;
    }
	if ( is_safe( ch, victim ) )
		return;
	if ( !can_initiate_combat( ch, victim, CIT_GENERAL )) return;
    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
		act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
		return;
    }

    if ( ch->position == POS_FIGHTING )
    {
		ch->println("You do the best you can!");
		return;
    }
    if (IS_SET(ch->act2,PLR2_TBC)/*&&!IS_NPC(ch)*/){
        do_fight (ch,argument);
        return;}

    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}

/**************************************************************************/
void do_pkill( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    char arg[MIL];
    char oarg[MIL];
    char_data *victim;

	if(!IS_NPC(ch) && GAMESETTING(GAMESET_PEACEFUL_MUD)){
		ch->println("This is a peaceful mud... pkilling is not permitted.");
		return;
	}

	sprintf(oarg, "%s", argument);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
		ch->println("Pkill whom?");
		return;
    }
	victim = get_char_room( ch, arg );
	OBJ_DATA 	*weapon = NULL;
	weapon = get_eq_char( ch, WEAR_WIELD );
	if (weapon != NULL){
	}
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		ch->println("They aren't here.");
		// lag those who spam it
		if (ch->desc && ch->desc->repeat>3){
			WAIT_STATE( ch, 3 * PULSE_VIOLENCE );
		}
		return;
    }
	if (ch->level <= 9){
		ch->println("Pkills can only be made after you reach level 10.");
			return;}
	if (victim->level <= 9){
		ch->println("Pkills can only be made after they reach level 10.");
			return;}
	if (weapon != NULL){
	if (weapon->value[0]==WEAPON_LONG_BOW ||
				weapon->value[0]==WEAPON_SHORT_BOW ||
				weapon->value[0]==WEAPON_CROSSBOW||
				weapon->value[0]==WEAPON_RIFLE ||
				weapon->value[0]==WEAPON_BLASTRIFLE)
			{
			ch->println("Don't you think that shooting from a bit further away would be wise?");
			return;}
	}
	// log it if is a pkill attack
    if ( !IS_NPC(victim) )
    {
		char log_buf[MSL];

        sprintf( log_buf, "Log %s: pkill %s - (found %s)",
			PERS(ch,NULL), oarg, PERS(victim,NULL));

        log_string( log_buf );
        wiznet(log_buf,ch,NULL,WIZ_SECURE,0,get_trust(ch));
    }

    if ( victim == ch )
    {
		ch->println("You hit yourself.  Ouch!");
		multi_hit( ch, ch, TYPE_UNDEFINED );
		return;
    }

    if ( is_safe( ch, victim ) )
		return;

	if ( !can_initiate_combat( ch, victim, CIT_GENERAL )) return;

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
		act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
		return;
    }

    if ( ch->position == POS_FIGHTING )
    {
		ch->println("You do the best you can!");
		return;
    }

    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}

/**************************************************************************/
void do_pbackstab( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    char arg[MIL];
    char oarg[MIL];
    char_data *victim;
    OBJ_DATA *obj, *osec;

	if(!IS_NPC(ch) && GAMESETTING(GAMESET_PEACEFUL_MUD)){
		ch->println("This is a peaceful mud... pkilling is not permitted.");
		return;
	}

	if(IS_MOUNTED(ch)){
		ch->println("You can't backstab cause you are being ridden.");
		return;
	}

	if(IS_RIDING(ch)){
		ch->println("You can't backstab while riding a creature.");
		return;
	}

	sprintf(oarg, "%s", argument);
    one_argument( argument, arg );

    if (arg[0] == '\0')
    {
		ch->println("Pbackstab whom?");
		return;
    }

    if (ch->fighting != NULL)
    {
		ch->println("You're facing the wrong end.");
		return;
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
		ch->println("They aren't here.");
		return;
    }

    if ( victim == ch )
    {
		ch->println("How can you sneak up on yourself?");
		return;
    }

	if ( !can_initiate_combat( ch, victim, CIT_GENERAL )) return;

	// log it if is a pkill attack
    if ( !IS_NPC(victim) ){
		char log_buf[MSL];

        sprintf( log_buf, "Log %s: pbackstab %s - (found %s)",
			PERS(ch,NULL), oarg, PERS(victim,NULL));

        log_string( log_buf );
        wiznet(log_buf,ch,NULL,WIZ_SECURE,0,get_trust(ch));
    }

    if ( (!IS_AFFECTED(ch, AFF_FLYING) && !IS_SET(ch->psionic_affects, PAFF_LEVITATE))
		&& IS_RIDING(victim))
	{
		ch->println("You can't backstab someone on a mount unless you are flying.");
		return;
	}

    if ( is_safe( ch, victim ) )
		return;

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)
    {
		ch->println("You need to wield a dagger to backstab.");
		return;
    }

	// must use a dagger to backstab
	if (obj->item_type != ITEM_WEAPON
		|| obj->value[0]!=WEAPON_DAGGER)
	{
		ch->println("You need to wield a dagger to backstab.");
		return;
	}

	osec=get_eq_char (ch, WEAR_SECONDARY);
	if (osec)
	{
		if (osec->item_type != ITEM_WEAPON
			|| osec->value[0]!=WEAPON_DAGGER)
		{
			ch->println("Your secondary weapon must be a dagger as well to backstab.");
			return;
		}
	}

    if ( victim->hit < victim->max_hit / 2)
    {
		act( "$N is hurt and suspicious ... you can't sneak up.",
			ch, NULL, victim, TO_CHAR );
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_backstab].beats );

	if ( (IS_AFFECTED(victim, AFF_FLYING) ||IS_SET(victim->psionic_affects, PAFF_LEVITATE))
		&& (!IS_AFFECTED(ch, AFF_FLYING) && !IS_SET(ch->psionic_affects, PAFF_LEVITATE))
		&& number_range(1,3)==1)
    {
		ch->println("You attempt to backstab them, but they fly out of the way and you miss.");
		check_improve(ch,gsn_backstab,false,1);
		damage( ch, victim, 0, gsn_backstab,DAM_NONE,true);
    }else{
		if ( number_percent( ) < get_skill(ch,gsn_backstab)
			|| ( get_skill(ch,gsn_backstab) >= 2 && !IS_AWAKE(victim) ) )
		{
			check_improve(ch,gsn_backstab,true,1);
			multi_hit( ch, victim, gsn_backstab );

			msp_to_room(MSPT_COMBAT, MSP_SOUND_BACKSTAB,
					0,
					ch,
					false,
					true);

			msp_to_room(MSPT_COMBAT, MSP_SOUND_BACKSTAB,
					0,
					victim,
					true,
					false);

		}
		else
		{
			check_improve(ch,gsn_backstab,false,1);
			damage( ch, victim, 0, gsn_backstab,DAM_NONE,true);
		}
	}

    return;
}

/**************************************************************************/
void do_circle( char_data *ch, char *argument )
{
    char_data * victim;
    OBJ_DATA  * obj;
    int         chance;

	if ((chance = get_skill(ch,gsn_circle)) == 0)
	{
		ch->println("What would you know about circling people?");
		return;
	}

    obj=get_eq_char( ch, WEAR_WIELD);
    if(!obj || obj->value[0]!=WEAPON_DAGGER){
		ch->println("You must wield a dagger to circle.");
        return;
    }

	victim = ch->fighting;
    if(!victim){
		ch->println("You aren't fighting anyone.");
		return;
    }

	if (IS_AFFECTED(ch,AFF_BLIND)){
		ch->println("You are blinded and can't find a path to circle them.");
		WAIT_STATE( ch, skill_table[gsn_circle].beats );
		return;
	}
	if (!IS_NPC(ch) && victim->fighting == ch){
    int chance = ch->modifiers[STAT_AG] + ch->modifiers[STAT_QU] + get_skill( ch,gsn_circle );
    if (!IS_NPC(victim)){
    int dodge = victim->modifiers[STAT_AG] + victim->modifiers[STAT_QU] + get_skill( victim,gsn_dodge );
	if (dodge > chance){
	act( "$N turns with you, not offering you a clear shot.", ch, NULL, victim, TO_CHAR );
	return;
	}
	}else{
	if ( chance < number_range(10,220)){
		act( "$N turns with you, not offering you a clear shot.", ch, NULL, victim, TO_CHAR );
		return;}

	}
	}

//	if(!GAMESETTING3(GAMESET3_CIRCLE_ALLOWED_WITHOUT_CLEAR_SHOT) && !IS_NPC(ch)){
//		if ( victim->fighting == ch ){
//			act( "$N turns with you, not offering you a clear shot.", ch, NULL, victim, TO_CHAR );
//			return;
//		}
//	}

    if ( get_skill( ch,gsn_circle ) > number_range(10,125) ) {

//		one_hit( ch, victim, gsn_circle, true );
		multi_hit( ch, victim, gsn_circle );
		check_improve( ch, gsn_circle, true, 1 );
	}else{
		damage(ch, victim, 0, gsn_circle, (obj->value[3]), true );
        check_improve( ch, gsn_circle, false, 1 );
    }
    WAIT_STATE( ch, skill_table[gsn_circle].beats );
}

/**************************************************************************/
void do_backstab( char_data *ch, char *argument )
{
    char arg[MIL];
    char_data *victim;
    OBJ_DATA *obj, *osec;
	char buf[MSL];

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	// standard no mount checks etc
	if(IS_MOUNTED(ch))
	{
		ch->println("You can't backstab cause you are being ridden.");
		return;
	}
	if(IS_RIDING(ch))
	{
		ch->println("You can't backstab while riding a creature.");
		return;
	}

	strcpy(buf,argument);
    one_argument( argument, arg );

    if (arg[0] == '\0'){
		ch->println("Backstab whom?");
		return;
    }

    if (ch->fighting != NULL)
    {
		ch->println("You're facing the wrong end.");
		return;
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
		ch->println("They aren't here.");
		if (ch->desc && ch->desc->repeat>3){
			WAIT_STATE( ch, 3 * PULSE_VIOLENCE );
		}
		return;
    }

    if ( victim == ch )
    {
		ch->println("How can you sneak up on yourself?");
		return;
    }


	if ( victim->fighting == ch )
    {
		ch->println("They already appear to be fighting you!");
		return;
    }

	// checks for preventing the start of a pkill fight
    if ( !IS_NPC(victim) ) // directly attacking the player
    {
		if(GAMESETTING(GAMESET_PEACEFUL_MUD)){
			ch->wrapln("That is a player you are trying to backstab. "
				"This is a peaceful mud where no pkilling is allowed.");
		}else{
			ch->wraplnf("That is a player your a trying to backstab, "
				"type pbs %s if you really want to do that.", buf );
		}
		return;
    }else if ( IS_SET(victim->act, ACT_PET)
		&& victim->leader
		&& !IS_NPC(victim->leader)
		&& (victim->leader->pet == victim)
		&& IS_SET(victim->affected_by, AFF_CHARM))
	{
		if(GAMESETTING(GAMESET_PEACEFUL_MUD)){
			ch->wrapln("That is a players pet you are trying to backstab. "
				"This is a peaceful mud where no pkilling is allowed.");
		}else{
			ch->wraplnf("That is a players pet, which could start a pkill fight, "
				"type pbs %s if you really want to do that.", buf );
		}
		return;
	}

	if ( !can_initiate_combat( ch, victim, CIT_GENERAL )) return;

    if ( (!IS_AFFECTED(ch, AFF_FLYING) && !IS_SET(ch->psionic_affects, PAFF_LEVITATE))
		&& IS_RIDING(victim))
	{
		ch->println("You can't backstab someone on a mount unless you are flying.");
		return;
	}

    if ( is_safe( ch, victim ) )
		return;

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)
    {
		ch->println("You need to wield a dagger to backstab.");
		return;
    }

	// must use a dagger to backstab
	if (obj->item_type != ITEM_WEAPON
		|| obj->value[0]!=WEAPON_DAGGER)
	{
		ch->println("You need to wield a dagger to backstab.");
		return;
	}

	osec=get_eq_char (ch, WEAR_SECONDARY);
	if (osec)
	{
		if (osec->item_type != ITEM_WEAPON
			|| osec->value[0]!=WEAPON_DAGGER)
		{
			ch->println("Your secondary weapon must be a dagger as well to backstab.");
			return;
		}
	}


    if ( victim->hit < victim->max_hit / 3)
    {
		act( "$N is hurt and suspicious ... you can't sneak up.",
			ch, NULL, victim, TO_CHAR );
		return;
    }

    if (  victim->cautious_about_backstab>0)
    {
		act( "$N is too suspicious of your attack... you can't sneak up.",
			ch, NULL, victim, TO_CHAR );
		return;
    }


    WAIT_STATE( ch, skill_table[gsn_backstab].beats );
	if ( (IS_AFFECTED(victim, AFF_FLYING) ||IS_SET(victim->psionic_affects, PAFF_LEVITATE))
		&& (!IS_AFFECTED(ch, AFF_FLYING) && !IS_SET(ch->psionic_affects, PAFF_LEVITATE))
		&& number_range(1,3)==1)
    {
		// missed due to the victim flying
		ch->println("You attempt to backstab them, but they fly out of the way and you miss.");
		check_improve(ch,gsn_backstab,false,1);
		damage( ch, victim, 0, gsn_backstab,DAM_NONE,true);
    }else{
		if ( number_percent( ) < get_skill(ch,gsn_backstab)
			|| ( get_skill(ch,gsn_backstab) >= 2 && !IS_AWAKE(victim) ) )
		{
			// a hit
			msp_to_room(MSPT_COMBAT, MSP_SOUND_BACKSTAB,
					0,
					ch,
					false,
					true);

			msp_to_room(MSPT_COMBAT, MSP_SOUND_BACKSTAB,
					0,
					victim,
					true,
					false);

			check_improve(ch,gsn_backstab,true,1);
			multi_hit( ch, victim, gsn_backstab );
		}
		else
		{
			// a miss
			check_improve(ch,gsn_backstab,false,1);
			damage( ch, victim, 0, gsn_backstab,DAM_NONE,true);
		}
	}

    return;
}


/**************************************************************************/
void do_flee( char_data *ch, char *)
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    char_data *victim;
	OBJ_DATA *polearm;
    int attempt;

    if ( ( victim = ch->fighting ) == NULL )
    {
		if ( ch->position == POS_FIGHTING )
			ch->position = POS_STANDING;
		ch->println("You aren't fighting anyone.");
		return;
    }

	if( IS_AFFECTED(ch,AFF_SLOW)
		&& number_range(1,4) < 4
		&& ch->mounted_on!=NULL)
	{
		ch->println("You feel too lethargic to run.");
		return;
	}

	// 65% chance those that are berserked/frenzied can't flee
	if ((IS_AFFECTED(ch,AFF_BERSERK)
		|| is_affected(ch,gsn_frenzy)
		|| is_affected(ch,gsn_berserk))
		&& number_range(1,100)>35)
	{
		ch->println("Your RAGE within is so strong you can't draw yourself away from the fight!");
		return;
	}

	// Cuttoff code here

	if ( IS_NPC( victim )
	||   IS_SET( victim->dyn, DYN_IS_CUTTING_OFF ))
	{
		polearm = get_eq_char( victim, WEAR_WIELD );

		if ( polearm )
		{
			if ( polearm->value[0] == WEAPON_POLEARM )
			{
				int		chance = number_percent();
				bool	successful = false;

				if ( IS_NPC( victim )
				&& IS_SET( victim->act, ACT_WARRIOR )
				&& chance < ( victim->level/3 ))
					successful = true;
				else if ( chance + 50 < get_skill( victim, gsn_cutoff ))
					successful = true;

				if ( successful )
				{
					act( "$N blocks your escape with $p.", ch, polearm, victim, TO_CHAR );
					act( "$n tries to flee, but you cut $m off.", ch, NULL, victim, TO_VICT );
					act( "$n tries to flee, but $N makes escape impossible.", ch, NULL, victim, TO_NOTVICT );
					check_improve( ch, gsn_cutoff, true, 1 );
					return;
				}
			}
		}
	}
	// End Cuttoff addition

    was_in = ch->in_room;
    for ( attempt = 0; attempt < 10; attempt++ )
    {
		EXIT_DATA *pexit;
		int door;

		door = number_door( );

		if ( ( pexit = was_in->exit[door] ) == 0
			||   pexit->u1.to_room == NULL
			||   IS_SET(pexit->exit_info, EX_CLOSED)
			||   number_range(0,ch->daze) != 0
			|| ( IS_NPC(ch)
			&&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
			continue;


		move_char( ch, door, false );
		if (ch->mounted_on!=NULL)
		{
			char_from_room(ch->mounted_on);
			char_to_room(ch->mounted_on, ch->in_room);
		}

		if ( ( now_in = ch->in_room ) == was_in )
			continue;

		ch->in_room = was_in;
		act( "$n has fled!", ch, NULL, NULL, TO_ROOM );
		ch->in_room = now_in;

		if ( !IS_NPC(ch) )
		{
			msp_to_room(MSPT_ACTION, MSP_SOUND_FLEE, 0,	ch, true, false);

			ch->println("You flee from combat!");
			if( HAS_CLASSFLAG(ch, CLASSFLAG_SAFE_FLEE_FROM_COMBAT)
				&& (number_percent() < 3*(ch->level/2) ) )
			{
				ch->println("You snuck away safely.");
			}else{
				ch->printlnf("You lost %d exp.", game_settings->xp_loss_for_fleeing);
				gain_exp( ch, game_settings->xp_loss_for_fleeing*-1 );
				if ( IS_HERO( ch )){
					do_heroxp( ch, game_settings->xp_loss_for_fleeing*-1 );
				}
			}
		}else{		//  IS NPC
			mobRememberClear( ch );
		}

		stop_fighting( ch, true ); // Do_Flee
		return;
    }

    ch->println("PANIC! You couldn't escape!");
    return;
}


/**************************************************************************/
// Kerenos - July 98
void do_retreat( char_data *ch, char *argument )
{
    ROOM_INDEX_DATA	*was_in;
    ROOM_INDEX_DATA	*now_in;
	OBJ_DATA		*polearm;
    char_data		*victim;
    EXIT_DATA		*pexit;
    int 		chance, door;
    char		arg[MIL];

    if (( victim = ch->fighting ) == NULL ) {
		if ( ch->position == POS_FIGHTING ){
			ch->position = POS_STANDING;
		}
		ch->println("You aren't fighting anyone.");
		return;
    }

    if (( chance = get_skill( ch, gsn_retreat )) == 0)
	{
		ch->println("You do not know how to retreat, try fleeing.");
		return;
    }

    if ( IS_AFFECTED( ch, AFF_SLOW )
		&& number_range( 1, 4 ) < 4
		&& ch->mounted_on != NULL ) {
		ch->println("You feel too lethargic to run.");
		return;
    }

	// 85% chance those that are berserked/frenzied can't flee
	if ((IS_AFFECTED(ch,AFF_BERSERK)
		|| is_affected(ch,gsn_frenzy)
		|| is_affected(ch,gsn_berserk))
		&& number_range(1,100)>85)
	{
		ch->println("Your RAGE within is so strong you don't want to retreat!");
		return;
	}

	// Cuttoff code here

	if ( IS_NPC( victim )
	||   IS_SET( victim->dyn, DYN_IS_CUTTING_OFF ))
	{
		polearm = get_eq_char( victim, WEAR_WIELD );

		if ( polearm )
		{
			if ( polearm->value[0] == WEAPON_POLEARM )
			{
				int		chance = number_percent();
				bool	successful = false;

				if ( IS_NPC( victim )
				&& IS_SET( victim->act, ACT_WARRIOR )
				&& chance < ( victim->level/5 ))
					successful = true;
				else if ( chance + 67 < get_skill( victim, gsn_cutoff ))
					successful = true;

				if ( successful )
				{
					act( "$N blocks your escape with $p.", ch, polearm, victim, TO_CHAR );
					act( "$n tries to flee, but you cut $m off.", ch, NULL, victim, TO_VICT );
					act( "$n tries to flee, but $N makes escape impossible.", ch, NULL, victim, TO_NOTVICT );
					check_improve( ch, gsn_cutoff, true, 1 );
					return;
				}
			}
		}
	}
	// End Cuttoff addition


	one_argument( argument, arg );

    if ( IS_NULLSTR( arg )) {
		ch->println("You must provide a direction in which to retreat.");
		return;
    }

	door = dir_lookup( arg );

    if ( door == -1 )
	{
		ch->println("Bad direction. Which way?");
		return;
    }


    was_in = ch->in_room;

    if (( pexit = was_in->exit[door] ) == 0
		||   pexit->u1.to_room == NULL
		||   IS_SET( pexit->exit_info, EX_CLOSED )
		||   number_range( 0, ch->daze ) != 0
		|| ( IS_NPC( ch )
		&&   IS_SET( pexit->u1.to_room->room_flags, ROOM_NO_MOB ))) {
		ch->println("You can't retreat in that direction.");
		return;
    }

	//  Do some stat checking
    if ( IS_SET( ch->off_flags, OFF_FAST ) || IS_AFFECTED( ch, AFF_HASTE ))
		chance += 10;
    if ( IS_SET( victim->off_flags, OFF_FAST ) || IS_AFFECTED( victim, AFF_HASTE ))
		chance -= 20;
    chance     += ch->modifiers[STAT_QU];
    chance     += ch->modifiers[STAT_SD];   // Character's resolve
    chance     -= victim->modifiers[STAT_QU];
    chance     -= victim->modifiers[STAT_AG];

    if ( number_percent() < chance )
    {
		move_char( ch, door, false );
		if ( ch->mounted_on != NULL ) {
			char_from_room( ch->mounted_on );
			char_to_room( ch->mounted_on, ch->in_room );
		}

        if (( now_in = ch->in_room ) == was_in ) {
			ch->println("That got you nowhere.");
			return;
		}  //  Loopy maze room check

		ch->in_room = was_in;
        act("$n has retreated.", ch, NULL, NULL, TO_ROOM );
        ch->in_room = now_in;

		if ( !IS_NPC( ch )) {
			msp_to_room(MSPT_ACTION, MSP_SOUND_FLEE, 0,	ch, true, false);
			ch->println("You have successfully retreated from the fight.");
			if (ch->move)
			{
				if (ch->race != race_lookup("liche"))
				ch->move= (ch->move*8/10)+1; //lose just under 20% of your moves
			}

			ch->println("You lost 5 exp.");
			gain_exp( ch, -5 );
			if ( IS_HERO( ch ))
				do_heroxp( ch, -10 );
		}
		check_improve( ch, gsn_retreat, true, 1);
        WAIT_STATE(ch,skill_table[gsn_trip].beats);
		stop_fighting( ch, true ); // Do_Retreat
		return;
    }
    else {
		ch->println("You couldn't get away.");
        WAIT_STATE( ch, skill_table[gsn_retreat].beats*2/3);
        check_improve( ch, gsn_retreat, false, 1 );
    }
    return;
}

/**************************************************************************/
void do_rescue( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    char arg[MIL];
    char_data *victim;
    char_data *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
		ch->println("Rescue whom?");
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		ch->println("They aren't here.");
		return;
    }

    if ( victim == ch )
    {
		ch->println("What about fleeing instead?");
		return;
    }

	// players can't rescue mobs
    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
		ch->println("Doesn't need your help!");
		return;
    }

    if ( ch->fighting == victim )
    {
		ch->println("Too late.");
		return;
    }

    if ( ( fch = victim->fighting ) == NULL )
    {
		ch->println("That person is not fighting right now.");
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( number_percent( ) > get_skill(ch,gsn_rescue))
    {
		ch->println("You fail the rescue.");
		check_improve(ch,gsn_rescue,false,1);
		return;
    }

	// The victim ($N) is being rescued
    act( "You rescue $N!",  ch, NULL, victim, TO_CHAR	 );
    act( "$n rescues you!", ch, NULL, victim, TO_VICT    );
    act( "$n rescues $N!",  ch, NULL, victim, TO_NOTVICT );
    check_improve(ch,gsn_rescue,true,1);

    stop_fighting( fch, false );   // Do_Rescue
    stop_fighting( victim, false );// Do_Rescue

	// stop cheaters backstabbing straight away
	fch->cautious_about_backstab =
		UMAX(fch->cautious_about_backstab, number_range(1,3));

	SET_BIT(ch->dyn,DYN_DOING_DAMAGE);
	if (!ch->fighting){
		set_fighting( ch, fch );
	}
	if (!fch->fighting){
		set_fighting( fch, ch );
	}
	REMOVE_BIT(ch->dyn,DYN_DOING_DAMAGE);
    return;
}


/**************************************************************************/
void do_kick( char_data *ch, char *argument)
{
    char_data *victim;

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    if ( !IS_NPC(ch)
		&&   ch->level < skill_table[gsn_kick].skill_level[ch->clss] )
	{
		ch->println("You better leave the martial arts to fighters.");
		return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK)){
		return;
	}

	if(IS_NULLSTR(argument))
	{
		victim = ch->fighting;
		if (victim == NULL)
		{
			ch->println("But you aren't fighting anyone!");
			return;
		}
	}
	else if ((victim = get_char_room(ch,argument)) == NULL)
	{
		ch->printlnf("'%s' is not here.", argument);
		return;
	}

	if (victim == ch)
	{
		ch->println("You try to kick yourself but fail.");
		return;
	}

	if (is_safe(ch,victim))
		return;

	if ( !can_initiate_combat( ch, victim, CIT_GENERAL )) return;

    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ( get_skill(ch,gsn_kick) > number_percent())
    {
		damage(ch,victim,number_range( 1, ch->level ), gsn_kick,DAM_BASH,true);
		check_improve(ch,gsn_kick,true,1);
    }
    else
    {
		damage( ch, victim, 0, gsn_kick,DAM_BASH,true);
		check_improve(ch,gsn_kick,false,1);
    }
    return;
}


/**************************************************************************/
void do_disarm( char_data *ch, char *argument )
{
	char arg[MIL];
    char_data *victim;
    OBJ_DATA *obj;
    int chance,hth,ch_weapon,vict_weapon,ch_vict_weapon;

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	one_argument( argument,arg );

	if ( !IS_NULLSTR( arg ) && !ch->fighting )
	{
		// disarm with an argument tries to disarm a trap
		// if the player is fighting, then it will continue
		do_disarm_trap( ch, arg );
		return;
	}

    hth = 0;

    if ((chance = get_skill(ch,gsn_disarm)) == 0)
    {
		ch->println("You don't know how to disarm opponents.");
		return;
    }

    if ( get_eq_char( ch, WEAR_WIELD ) == NULL
    &&   ((hth = get_skill(ch,gsn_hand_to_hand)) == 0
    ||    (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DISARM))))
    {
		ch->println("You must wield a weapon to disarm.");
		return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
		ch->println("You aren't fighting anyone.");
		return;
    }

    if (( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
	if (( obj = get_eq_char( victim, WEAR_SECONDARY) ) == NULL )
    {
		ch->println("Your opponent is not wielding a weapon.");
		return;
    }

	// No disarm when the victim has vicegrip
    if (IS_AFFECTED2(victim, AFF2_VICEGRIP)){
		act("Your opponent has an iron grip on their weapon! You have no chance!",ch,NULL,victim,TO_CHAR);
		check_improve(ch,gsn_disarm,false,1);
	    return;
	}

    // find weapon skills
    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch));
    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim));
    ch_vict_weapon = get_weapon_skill(ch,get_weapon_sn(victim));

    // modifiers

    // skill
    if ( get_eq_char(ch,WEAR_WIELD) == NULL)
		chance = chance * hth/150;
    else
		chance = chance * ch_weapon/100;

    chance += (ch_vict_weapon/2 - vict_weapon) / 2;

    // quickness/agility  vs. strength
    chance += (ch->modifiers[STAT_QU] + ch->modifiers[STAT_AG])/2;
    chance -= 2 * victim->modifiers[STAT_ST];

	if(IS_NPC(ch)){
		chance = chance * 4/5;
	}

    // level
    chance += (ch->level - victim->level) * 2;

    // and now the attack
    if (number_percent() < chance)
    {
		WAIT_STATE( ch, skill_table[gsn_disarm].beats );
		disarm( ch, victim );
		check_improve(ch,gsn_disarm,true,1);
    }
    else
    {
		WAIT_STATE(ch,skill_table[gsn_disarm].beats);
		act("You fail to disarm $N.",ch,NULL,victim,TO_CHAR);
		act("$n tries to disarm you, but fails.",ch,NULL,victim,TO_VICT);
		act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);
		check_improve(ch,gsn_disarm,false,1);
    }
    return;
}

/**************************************************************************/
void do_surrender( char_data *ch, char * )
{
    char_data *mob;

	mob = ch->fighting;
    if (!mob)
	{
		ch->println("But you're not fighting !");
		return;
    }

	if (!IS_NPC(mob)){
		ch->println("You can't surrender in pkill fights!");
		return;
	}

	// stop cheaters backstabbing straight away
	mob->cautious_about_backstab =
		UMAX(mob->cautious_about_backstab, number_range(2,5));

    act( "You surrender to $N!", ch, NULL, mob, TO_CHAR );
    act( "$n surrenders to you!", ch, NULL, mob, TO_VICT );
    act( "$n tries to surrender to $N!", ch, NULL, mob, TO_NOTVICT );
    ch->casttimer1 =0;
    ch->casting1name = NULL;
    ch->casting1 = -1;
    ch->casttimer2 =0;
    ch->casting2name = NULL;
    ch->casting2 = -1;
    stop_fighting( ch, true ); // Do_surrender

    if ( !IS_NPC( ch ) && IS_NPC( mob )
    &&   ( !HAS_TRIGGER( mob, MTRIG_SURR )
	|| !mp_percent_trigger( mob, ch, NULL, NULL, MTRIG_SURR ) ) )
    {
		act( "$N seems to ignore your cowardly act!", ch, NULL, mob, TO_CHAR );
		multi_hit( mob, ch, TYPE_UNDEFINED );
	}
}


/**************************************************************************/
void do_sla( char_data *ch, char *)
{
    ch->println("If you want to SLAY, spell it out.");
    return;
}

/**************************************************************************/

void do_tail( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    char arg[MIL];
    char_data *victim;
    int chance;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_tail)) == 0
	||	!IS_SET(ch->parts, PART_TAIL)
	||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TAIL))
    ||  (!IS_NPC(ch)
	&&   ch->level < skill_table[gsn_tail].skill_level[ch->clss]))
    {
		ch->println("Do you even have a tail?");
		return;
    }

	if (arg[0] == '\0')
	{
		victim = ch->fighting;
		if (victim == NULL)
	    {
		    ch->println("But you aren't fighting anyone!");
		    return;
	    }
		else if ((victim = get_char_room(ch,arg)) == NULL)
		{
		    ch->println("They aren't here.");
		    return;
		}
	}

	if ( ( victim = ch->fighting ) == NULL )
    {
		ch->println("You aren't fighting anyone.");
		return;
    }

	if (victim->position < POS_FIGHTING)
	{
		act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
	    return;
    }

    if (victim == ch)
    {
	    ch->println("Do you really want to hit yourself?");
		return;
    }

	if ( IS_AFFECTED( victim, AFF_FLYING ) ||
		IS_SET(victim->psionic_affects, PAFF_LEVITATE))
	{
		ch->println("Their feet are not on the ground.");
		return;
	}

    if (is_safe(ch,victim))
		return;

    if ( IS_NPC(victim) &&
		victim->fighting != NULL &&
		!is_same_group(ch,victim->fighting))
	    {
			ch->println("Kill stealing is not permitted.");
			return;
	    }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
		act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
		return;
    }

    chance += ch->carry_weight / 250;
    chance -= victim->carry_weight / 200;

    if (ch->size < victim->size)
		chance += (ch->size - victim->size) * 15;
    else
		chance += (ch->size - victim->size) * 10;

    chance += ch->modifiers[STAT_ST];
    chance -= victim->modifiers[STAT_QU];
    chance -= GET_AC(victim,AC_BASH) /25;

    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
		chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
		chance -= 30;

    chance += (ch->level - victim->level);
	if (IS_SET(victim->psionic_affects, PAFF_BLUR)){
		chance -= 3 * (get_skill(victim,gsn_blur) - chance);
		check_improve(victim, gsn_blur, true, 1);}
    if (!IS_NPC(victim)
	&& chance < get_skill( victim, gsn_dodge ))
    {
		act("$n tries to smash you with a tail.",ch,NULL,victim,TO_VICT);
		act("$N dodges your tail, you falter off balance.",ch,NULL,victim,TO_CHAR);
		WAIT_STATE(ch,skill_table[gsn_tail].beats);
//		return;
    }

    if (number_range(1, 100) < chance )
    {
		act("$n sweeps your feet from under you with $s tail!",	ch,NULL,victim,TO_VICT);
	    act("You slam a tail into $N, and send $M flying!",ch,NULL,victim,TO_CHAR);
	    act("$n sends $N sprawling with a powerful smash from a tail.",	ch,NULL,victim,TO_NOTVICT);
	    check_improve(ch,gsn_tail,true,1);

		DAZE_STATE(victim, 3 * PULSE_VIOLENCE);
	    WAIT_STATE(ch,skill_table[gsn_tail].beats);
		victim->position = POS_RESTING;
//		damage(ch,victim,number_range(2,2 + 2 * ch->size + chance/20),gsn_tail,
//	    DAM_BASH,false);

    }
    else
    {
//		damage(ch,victim,0,gsn_tail,DAM_BASH,false);
		act("You fall flat on your face!", ch,NULL,victim,TO_CHAR);
		act("$n falls flat on $s face.", ch,NULL,victim,TO_NOTVICT);
	    act("You evade $n's tail, causing $m to falter and lose balance.", ch,NULL,victim,TO_VICT);
	    check_improve(ch,gsn_tail,false,1);
	    ch->position = POS_STANDING;
	    WAIT_STATE(ch,skill_table[gsn_tail].beats * 3/2);
    }
}


/**************************************************************************/
void do_claw( char_data *ch, char *)
{
    char_data *victim;
	int damage_to_inflict;
	int dam;

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    if ( !IS_NPC(ch)
		&&   /*(ch->level < skill_table[gsn_claw].skill_level[ch->clss]
		|| */get_skill(ch,gsn_claw)==0/*)*/ )
    {
		ch->println("You do not know how to claw.");
		return;
    }

	if (!IS_SET( ch->parts, PART_CLAWS )
		&& !IS_VAMPAFF(ch, VAMPYRE_CLAWS)
		&& !IS_WEREAFF(ch, WAFF_CLAWS)){
		ch->println("You don't have claws");
		return;}

    if ( ( victim = ch->fighting ) == NULL )
    {
		ch->println("You aren't fighting anyone.");
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_claw].beats );
    if ( get_skill(ch,gsn_claw) > number_percent())
    {
		damage(ch,victim,number_range( 1, ch->level+ch->level/2 ), gsn_claw,DAM_SLASH,true);
			if (ch->race == race_lookup("vampyre")){
		if (ch->fighting == victim && IS_VAMPAFF(ch, VAMPYRE_CLAWS)){
			dam = number_range(1, ch->level / 2);

			damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_NEGATIVE);
			if(damage_to_inflict>=0){
				// *** patch up the damage_to_inflict value
				// limit the damage to the amount required to put the victim
				// into -11 hp... in order to limit the benefits of the weapon
				damage_to_inflict=UMIN(victim->hit+11,damage_to_inflict);
				damage_to_inflict=UMAX(0,damage_to_inflict);
				// display the custom damage messages
				act("$n's claws draw life from $N.",ch,ch,victim,TO_NOTVICT);
				act_with_autodam_to_char("You feel $n's claws drawing your life away.",victim,ch,NULL,damage_to_inflict);
				act_with_autodam_to_char("Your claws draw life from $N.",ch,ch,victim,damage_to_inflict);
				// inflict the custom effect before the standard damage
				ch->hit += damage_to_inflict;
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_NEGATIVE);
			if (ch->hit >= ch->max_hit){
				ch->hit = ch->max_hit;}
			}
		}
	}
		check_improve(ch,gsn_claw,true,1);
    }
    else
    {
		damage( ch, victim,0, gsn_claw,DAM_SLASH,true);
		check_improve(ch,gsn_claw,false,1);
    }
    return;
}

/**************************************************************************/
// Written originally by Rathern, fixed poison by Kal July 99
void do_gouge( char_data *ch, char *)
{
    char_data *victim;

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    if ( !IS_NPC(ch)
		&&   (ch->level < skill_table[gsn_gouge].skill_level[ch->clss]
		|| get_skill(ch,gsn_gouge)==0) )
	{
		ch->println("You do not know how to gouge.");
		return;
    }

    /*
    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_gouge))
	return;
    */

    if ( ( victim = ch->fighting ) == NULL )
    {
		ch->println("You aren't fighting anyone.");
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_gouge].beats );
    if ( get_skill(ch,gsn_gouge) > number_percent())
    {
		damage(ch,victim,number_range( 1, ch->level+ch->level/2 ), gsn_gouge,DAM_PIERCE,true);

		// chance of poisoning
		if ( saves_spell( ch->level*3/2, victim, DAM_POISON)
//			|| HAS_CLASSFLAG(victim, CLASSFLAG_POISON_IMMUNITY))
			)
		{
			act("$n turns slightly green, but it passes.",victim,NULL,NULL,TO_ROOM);
			victim->println("You feel momentarily ill, but it passes.");
			check_improve(ch,gsn_gouge,false,1);
		}else{
			AFFECT_DATA af;

			victim->println("You feel poison coursing through your veins.");
			act("$n looks very ill.",victim,NULL,NULL,TO_ROOM);

			af.where     = WHERE_AFFECTS;
			af.type      = gsn_poison;
			af.level     = ch->level;
			af.duration  = ch->level/2;
			af.location  = APPLY_ST;
			af.modifier  = -1;
			af.bitvector = AFF_POISON;
			affect_join( victim, &af );
			check_improve(ch,gsn_gouge,true,1);
		}

    }
    else
    {
		damage( ch, victim,0, gsn_gouge,DAM_PIERCE,true);
		check_improve(ch,gsn_gouge,false,1);
    }
}


/**************************************************************************/
// written by Kerenos - July 98
void do_gore( char_data *ch, char *argument )
{
    char_data * victim;
    OBJ_DATA  * obj;
    char        arg[MIL];
    int         chance;

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    one_argument( argument, arg );

    obj = ( get_eq_char( ch, WEAR_HEAD ));

    if ( !obj && !IS_SET( ch->parts, PART_HORNS )) {
        ch->println("You have no horns to gore with.");
        return;
    }

	if ( obj ) {
        if ( !IS_OBJ_STAT( obj, OBJEXTRA_HORNED )) {
            act ( "There are no horns on $p!", ch, obj, NULL, TO_CHAR );
            return;
        }
    }

// standard no mount checks etc
	if(IS_MOUNTED(ch))
	{
		ch->println("You can't gore cause you are being ridden.");
		return;
	}
	if(IS_RIDING(ch))
	{
		ch->println("You can't gore others while riding a creature.");
		return;
	}

    if (( chance = get_skill( ch, gsn_gore )) == 0
        || ( IS_NPC( ch ) && !IS_SET( ch->off_flags, OFF_GORE))
        || ( !IS_NPC( ch )
			&& ch->level < skill_table[gsn_gore].skill_level[ch->clss])) {

		ch->println("You are not proficient enough in that skill.");
        return;
    }

    if ( IS_NULLSTR(arg) ) {
        if (( victim = ch->fighting ) == NULL ) {
            ch->println("But you aren't fighting anyone!");
            return;
        }
    }
    else if (( victim = get_char_room( ch, arg )) == NULL ) {
        ch->printlnf("You can't seem to find '%s' here.", arg);
        return;
    }


// Inserted possible cheat deterrents

	if ( is_safe( ch, victim ))
		return;

	if ( !can_initiate_combat( ch, victim, CIT_GENERAL )) return;

    if ( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim ) {
		act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_gore].beats*2);

    if ( get_skill( ch,gsn_gore ) > number_percent() ) {
        ch->println("You set your head down and charge!");
        if (damage( ch,victim,number_range( ch->level / 10, ch->level / 2 ),
			gsn_gore, DAM_PIERCE, true))
		{
			// successful gore
			victim->position = POS_RESTING;
	        WAIT_STATE( victim, skill_table[gsn_gore].beats*2 );
			check_improve( ch, gsn_gore, true, 1 );
		}
		else // unsuccessful gore
		{
			check_improve( ch, gsn_gore, false, 1 );
		}
        if ( number_percent() <= 50 ) {
            ch->println("You charge so wildly that you knock yourself down too.");
            act( "$n knocks himself down after charging you.",
                  ch, NULL, victim, TO_VICT );
            act( "With a wild flourish, $n goes down in a heap.",
                  ch, NULL, victim, TO_NOTVICT );
            ch->position = POS_RESTING;
        }
    }else{
        damage( ch, victim, 0, gsn_gore, DAM_PIERCE, false );
        check_improve( ch, gsn_gore, false, 1 );
		ch->println("You blindly charge past your mark, missing by a mile!");
        act("$n tries to gore $N but fails miserably.",
            ch, NULL, victim, TO_NOTVICT);
        act("$n runs right past you and trips a step later.",
            ch, NULL, victim, TO_VICT );
        check_improve( ch, gsn_gore, false, 1 );
        ch->position = POS_RESTING;
    }
    return;
}

/**************************************************************************/
void do_subdue(char_data *ch, char *)
{
    if (!ch->fighting){
        ch->println("You aren't currently in combat, use autosubdue");
        return;
    }

    if (IS_SET(ch->dyn,DYN_CURRENT_SUBDUE))
    {
        ch->println("You will now fight this fight to the death.");
        REMOVE_BIT(ch->dyn,DYN_CURRENT_SUBDUE);
    }
    else
    {
        ch->println("You will attempt to subdue your current opponent.");
        SET_BIT(ch->dyn,DYN_CURRENT_SUBDUE);
    }
}

/**************************************************************************/
// tell a ch to write a pknote for their involvement in the death of victim
void pkill_note_required_message(char_data *ch, char_data *victim, bool bypass_duel_used)
{
	char subject[MSL], buf[MSL], buf2[MSL];
	if (IS_NPC(ch) || IS_NPC(victim))
	{
		if(IS_NPC(ch)){
			bug("pkill_note_required_message: ch==NPC!");
		}
		if(IS_NPC(victim)){
			bug("pkill_note_required_message: victim==NPC!");
		}
		make_corefile();
	}

	// no pkill notes ever on a dedicated pkill style mud :)
	if(GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
		return;
	}

	if(ch==victim){
		sprintf( subject, "`RPkill note required from %s - was pkilled/subdued`x",ch->name);
	}else{
		sprintf( subject, "`RPkill note required from %s (involved in pk/subdue of %s)`x",
			ch->name, victim->name);
	}

	// pkstats to imm and noble
	sprintf( buf2, "`1`?`#--===== `YPk status on players after the fight `^=====-`x"
		"`1  %-12s - karns = %d, pkills = %d, pkdefeats = %d, pkool = %d, %s"
		"`1  %-12s - karns = %d, pkills = %d, pkdefeats = %d, pkool = %d, %s"
		"`1  bypass duel used = %s, %s linkdead=%s",
		ch->name, ch->pcdata->karns, ch->pkkills, ch->pkdefeats, ch->pkool, IS_ACTIVE(ch)?"active":"peaceful",
		victim->name, victim->pcdata->karns, victim->pkkills,
			victim->pkdefeats, victim->pkool,
			IS_ACTIVE(victim)?"active":"peaceful",
			bypass_duel_used?"true":"false",
			ch->name, ch->desc?"false":"true");

	autonote(NOTE_PKNOTE, "p_anote()", subject, "immpkill noblepkill", buf2, true);

	/*if(ch==victim){
		if(!codehelp(ch, "pknote_required_from_victim", CODEHELP_ALL_BUT_PLAYERS)){
			ch->wrapln(
			"`WYou have just been involved in a fight that has ended in either your death or subduel. "
			"Player killing is allowed here so long as it is in done within a roleplaying "
			"context.  To ensure all pkills have sufficient IC basis we require you (and all others "
			"involved to write a note to `Ypknote to PKILL`W within the next 2 hours "
			"explaining the IC background of the pkill.  `RPLEASE WRITE AT LEAST 5 LINES!`x`1`1"
			"If you have to chased for a pkill note you will be mooted negatively.`1`1"
			"If you don't consider this kill was totally IC, or someone may claim it wasnt "
			"try to save a log of the fight and events leading up to this in support of your opinion.\r\n");
		}
	}else{
		if(!codehelp(ch, "pknote_required_from_attacker", CODEHELP_ALL_BUT_PLAYERS)){
			ch->wrapln(
			"`WYou have just been involved in a fight that has ended another player being killed or subdued. "
			"Player killing is allowed here so long as it is in done within a roleplaying "
			"context.  To ensure all pkills have sufficient IC basis we require you (and all others "
			"involved to write a note to `=Cpknote to PKILL`W within the next 2 hours "
			"If you have to chased for a pkill note you will be mooted negatively.`1`1"
			"explaining the IC background of the pkill.  `RPLEASE WRITE AT LEAST 5 LINES!`x`1`1"
			"If you don't consider this kill was totally IC, or someone may claim it wasnt "
			"try to save a log of the fight and events leading up to this in support of your opinion.\r\n");
		}


	}*/

	if(GAMESETTING2(GAMESET2_AUTONOTE_IMMPKILLS_TO_ADMIN)
		&& (IS_IMMORTAL(ch) || !IS_NULLSTR(ch->pcdata->immtalk_name))){
		if(!IS_NULLSTR(ch->pcdata->immtalk_name)){
			sprintf(buf2,"%s (%s) involved in pkill/subduel of %s",
				ch->name, ch->pcdata->immtalk_name, victim->name);
		}else{
			sprintf(buf2,"%s involved in pkill/subduel of %s", ch->name, victim->name);
		}
		autonote(NOTE_ANOTE, "p_anote()", "Pkill/subdue by immortal/mort with immtalk", "admin", buf2, true);
	}

	if(!GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
		if(!IS_IMMORTAL(ch) && !IS_SET(true_CH(ch)->act, PLR_LOG)){
			SET_BIT(true_CH(ch)->act, PLR_LOG);
			sprintf( buf, "Player log turned ON by doing a pkill");
			append_playerlog( true_CH(ch), buf);
			logf("LOG set on %s by doing a pkill.", true_CH(ch)->name);
		}
		if(!IS_IMMORTAL(victim) && !IS_SET(true_CH(victim)->act, PLR_LOG)){
			SET_BIT(true_CH(victim)->act, PLR_LOG);
			sprintf( buf, "Player log turned ON by doing a pkill");
			append_playerlog( true_CH(victim), buf);
			logf("LOG set on %s by doing a pkill.", true_CH(victim)->name);
		}
	}
}
/**************************************************************************/
// note a pkill and remind the victim and killer to write a note
void pkill_autonote(char_data *ch, char_data *victim)
{
    if(victim->alliance < -349){
		ch->alliance += 25;
	}else if(victim->alliance > 350){
		ch->alliance -= 25;
	}

	if(ch->alliance > 1000){
		ch->alliance = 1000;
	}else if(ch->alliance < -1000){
		ch->alliance = -1000;
	}

	char subject[MSL], buf[MSL], buf2[MSL];
	if (IS_NPC(ch) || IS_NPC(victim))
	{
		bug("pkill_autonote: not both players");
		make_corefile();
	}

	sprintf( subject, "`RPkill: %s pkilled %s`x",	ch->name, victim->name);

	// pkstats to imm and noble
	sprintf( buf2, "`1`?`#--===== `YPk status on players after the fight `^=====-`x"
		"`1  %-12s - karns = %d, pkills = %d, pkdefeats = %d, pkool = %d, %s"
		"`1  %-12s - karns = %d, pkills = %d, pkdefeats = %d, pkool = %d, %s",
		ch->name, ch->pcdata->karns, ch->pkkills, ch->pkdefeats, ch->pkool, IS_ACTIVE(ch)?"active":"peaceful",
		victim->name, victim->pcdata->karns, victim->pkkills,
			victim->pkdefeats, victim->pkool, IS_ACTIVE(victim)?"active":"peaceful");
	autonote(NOTE_PKNOTE, "p_anote()",subject, "immpkill noblepkill", buf2, true);

	// pkinfo to imm only
	sprintf( buf, "`Y%s`x (%s) lvl %d   started_fight=%s`1"
				  "logon how long ago: %s`1"
		          "clan: %s`1`Rpkilled`x"
		"`1`Y%s`x (%s) lvl %d    started_fight=%s`1"
		"logon how long ago: %s`1"
		"clan: %s`x`1Room: %d (%s in %s)`1"
		"`1"
		"autosub,currentsub  %s=%s,%s   %s=%s,%s`1",
		ch->name,		ch->short_descr,	ch->level,
		(IS_SET(ch->dyn, DYN_STARTED_FIGHT)?"Yes":"No"),
		short_timediff(current_time, ch->logon),

					ch->clan->cname(),
					victim->name,
					victim->short_descr,
					victim->level,
		(IS_SET(victim->dyn, DYN_STARTED_FIGHT)?"Yes":"No"),
		short_timediff(current_time, victim->logon),
					victim->clan->cname(),
					ch->in_room->vnum,
					ch->in_room->name,
					ch->in_room->area->name,
					ch->name,
					IS_SET(ch->act,PLR_AUTOSUBDUE)?"ON":"OFF",
					IS_SET(ch->dyn,DYN_CURRENT_SUBDUE)?"ON":"OFF",
					victim->name,
					IS_SET(victim->act,PLR_AUTOSUBDUE)?"ON":"OFF",
					IS_SET(victim->dyn,DYN_CURRENT_SUBDUE)?"ON":"OFF"
		);
	autonote(NOTE_PKNOTE, "p_anote()","pkstatus on last note", "immpkilldetails", buf, true);

	if(GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
		true_CH(ch)->pcdata->p9999kills++;
		true_CH(victim)->pcdata->p9999defeats++;
		if(!IS_IMMORTAL(ch)){
			pkill_broadcast(
                "%s [Pk=%d.Pd=%d.L=%d] `RTOASTED`W %s [Pk=%d.Pd=%d.L=%d] at %s (in %s)",
				true_CH(ch)->name,
				true_CH(ch)->pcdata->p9999kills,
				true_CH(ch)->pcdata->p9999defeats,
                true_CH(ch)->level,
				true_CH(victim)->name,
				true_CH(victim)->pcdata->p9999kills,
				true_CH(victim)->pcdata->p9999defeats,
                true_CH(victim)->level,
				true_CH(ch)->in_room->name,
				true_CH(ch)->in_room->area->name);
		}

		// record max pkill
		if (p9999maxpk<true_CH(ch)->pcdata->p9999kills){
			if (IS_IMMORTAL(ch)){
				pkill_broadcast("Immortal pkill record of %s not saved!",ch->name);
			}else{
				if (str_cmp(p9999maxpkname,true_CH(ch)->name)){
					pkill_broadcast("PKILL RECORD BROKEN BY %s!",ch->name);
                    strcpy(p9999maxpkname,true_CH(ch)->name);
                    p9999maxpklevel=true_CH(ch)->level;
				}
				p9999maxpk=true_CH(ch)->pcdata->p9999kills;
			}
		}
		// record max defeat
		if (p9999maxpd<true_CH(victim)->pcdata->p9999defeats){
			if (str_cmp(p9999maxpdname,true_CH(victim)->name)){
                pkill_broadcast("PKDEFEAT RECORD BROKEN BY %s, congratulations %s!",
					victim->name, ch->name);
				strcpy(p9999maxpdname,true_CH(victim)->name);
			}
            p9999maxpdlevel=true_CH(victim)->level;
			p9999maxpd=true_CH(victim)->pcdata->p9999defeats;
		}
	}

}

// MOB MEMORY STUFF

bool mobRememberCH( char_data *ch, char_data *victim )
{
	if ( !ch->mobmemory || ch->mobmemory != victim )
		return false;
	return true;
}

void mobRememberClear( char_data *ch )
{
	ch->mobmemory = NULL;

	return;
}

void mobRememberSet( char_data *ch, char_data *victim )
{
	if ( !IS_NEWBIE( ch ))
		ch->mobmemory = victim;

	return;
}
/**************************************************************************/
void do_becomeactive(char_data *ch, char *argument)
{
    if(IS_NPC(ch))
		return;

	// Check if they're being ordered to do this
	if ( IS_SET( ch->dyn, DYN_IS_BEING_ORDERED ))
	{
		if ( ch->master ){
			ch->master->println( "Not going to happen.");
		}
		return;
	}

	if(!GAMESETTING(GAMESET_NO_LETGAINING_REQUIRED) && !IS_LETGAINED(ch)){
		ch->println("You can't use this command unless letgained.");
		return;
	}

	if( IS_ACTIVE(ch)){
		ch->println("You are already an active player.");
		return;
	};

	if(GAMESETTING(GAMESET_PEACEFUL_MUD)){
		ch->println("The game is locked in peaceful mode, as a result you can't becomeactive.");
		return;
	}

	/*if(GAMESETTING2(GAMESET2_NO_DUEL_REQUIRED)){
		ch->println("The duel system is not enabled on this mud, so becoming active is pointless.");
		return;
	}*/

	if(HAS_CONFIG2(ch,CONFIG2_NOPKILL))
	{
		ch->println( "You are not allowed to become active." );
		return;
	}

	if ( str_cmp("confirm", argument)) {
		if(!codehelp(ch, "do_becomeactive_noargument", CODEHELP_ALL_BUT_PLAYERS)){
			ch->println("If you want to enable your active status type:");
			ch->println("  `=Cbecomeactive confirm`x.");
			ch->println("`RBE WARNED: `xThe active status once on is not able to be removed.");
		};
		return;
	}

	SET_CONFIG(ch, CONFIG_ACTIVE);
	if(!codehelp(ch, "do_becomeactive_enabled", CODEHELP_ALL_BUT_PLAYERS)){
		ch->println("Your active status has been activated.  This can not be turned off.");
	}

/*	if(ch->level<=10){
		SET_BIT(ch->act, PLR_NOREDUCING_MAXKARN);
		ch->wrapln("Because you have changed from peaceful to active before level 10, the maximum "
			"number of karns you can attain will always be 5 regardless of the number of "
			"pkills you have been involved in.");
	}*/

	save_char_obj(ch);
}
/**************************************************************************/
bool mp_prekill_trigger( char_data *mob, char_data *attacker);
/**************************************************************************/
// doesn't display any message to anyone
bool can_initiate_combat_with_player( char_data *attacker, char_data *victim, int type)
{
	assert(!IS_NPC(victim));
	// prevent ordering starting pk combat
	if( !IS_NPC(victim) && IS_SET( attacker->dyn, DYN_IS_BEING_ORDERED ))
	{
		if(attacker->master && !IS_NPC(attacker->master)){
			if(!can_initiate_combat(attacker->master, victim, type)){
//				attacker->println("You can't be ordered to initiate PK combat unless you master can attack them.");
//				attacker->master->println("You can't ordered people or mobs to initiate PK combat unless you can attack the target!" );
				return false;
			}
		}else{
//			attacker->println("You can't be ordered to initiate PK combat.");
			return false;
		}
	}

	if(IS_NPC(attacker)){ // nonordered mobs shouldn't get hindered by the code below
		return true;
	}

	if(GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
		return true;
	}

	// PREVENT KILL STEALING
	if(IS_SET(type,CIT_GENERAL)){
		if ( IS_NPC(victim) &&
			victim->fighting != NULL &&
			!is_same_group(attacker,victim->fighting))
		{
//			attacker->println("Kill stealing is not permitted.");
			return false;
		}
	};

	if(IS_NPC(victim)){ // mobs don't require duelling
		return true;
	}

	if(victim==attacker){ // casting slow on yourself etc
		return true;
	}

	if(GAMESETTING2(GAMESET2_NO_DUEL_REQUIRED)){
		return true;
	}

	if(IS_SET(type,(CIT_GENERAL | CIT_AGRESSIVE_ACTION | CIT_CASTING_SPELL))){

		// pk duelsystem checks - only apply to morts
		if(    !IS_ICIMMORTAL(attacker))
		{
			// bypass duel system (spells can't make use of it)
			if(attacker->duels
				&& !IS_SET(type,CIT_CASTING_SPELL)
				&& !IS_SET(type,CIT_NO_BYPASSDUEL)
				&& attacker->duels->is_bypassingduel(victim))
			{
				return true;
			}

			if(IS_ACTIVE(victim)){
				// check for peaceful attempting to attack an active player
				if(IS_PEACEFUL(attacker)){
					// peaceful must either bypassduel to attack a pk (or duel if they have been stealing)
					if(!attacker->duels || !attacker->duels->is_known(victim)){
						return false;
					}
					if(!attacker->duels->is_dueling(attacker, victim)){
						return false;
					}
					return true;
				}
			}else{
				// The victim is peaceful, duel is required regardless of the
				// pk status of the attacker
				if(!attacker->duels || !attacker->duels->is_known(victim)){
					return false;
				}
				if(!attacker->duels->is_dueling(attacker, victim)){
					return false;
				}
				return true;
			}
		}
	}
	return true;
}
/**************************************************************************/
bool can_initiate_combat( char_data *attacker, char_data *victim, int type)
{
	// prekill trigger
	if ( IS_NPC(victim) &&  HAS_TRIGGER( victim, MTRIG_PREKILL ) ){
		if(mp_prekill_trigger( victim, attacker )){
			return false;
		};
	}

	// nonordered mobs shouldn't get hindered by the code below
	if(IS_NPC(attacker) && !IS_SET( attacker->dyn, DYN_IS_BEING_ORDERED )){
		return true;
	}

	if(GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
		return true;
	}

	// attacking players pet checks
    if ( IS_NPC(victim)
		&& IS_SET(victim->act, ACT_PET)
		&& victim->leader
		&& !IS_NPC(victim->leader)
		&& (victim->leader->pet == victim)
		&& IS_SET(victim->affected_by, AFF_CHARM))
	{
		if(!can_initiate_combat_with_player( attacker, victim->leader, (type | CIT_NO_BYPASSDUEL))){
		// can't attack, inform of situation
			// prevent ordering starting pk combat
			if(!IS_NPC(victim) && IS_SET( attacker->dyn, DYN_IS_BEING_ORDERED ))
			{
				if(attacker->master && !IS_NPC(attacker->master)){
					if(!can_initiate_combat(attacker->master, victim, type)){
						if(!IS_SET(type,CIT_SILENT)){
							attacker->println("You can't be ordered to initiate PK combat with another players pet\r\n"
								"unless your master can attack the master of the pet you are attacking." );
							attacker->master->println("You can't ordered people or mobs to initiate combat with another players pet\r\n"
								"unless you can attack the owner of the pet yourself!" );
						}
						return false;
					}
				}else{
					if(!IS_SET(type,CIT_SILENT)){
						attacker->println("You can't be ordered to initiate PK combat with another players pet.");
					}
					return false;
				}
			}
			if(!IS_SET(type,CIT_SILENT)){
				attacker->master->println("You can't initiate combat with another players pet unless you can\r\n"
					"attack the owner of the pet yourself (without using bypass duel)!" );
			}
			return false;
		}
	}

	// prevent ordering starting pk combat
	if(!IS_NPC(victim) && IS_SET( attacker->dyn, DYN_IS_BEING_ORDERED ))
	{
		if(attacker->master && !IS_NPC(attacker->master)){
			if(!can_initiate_combat_with_player(attacker->master, victim, type)){
				if(!IS_SET(type,CIT_SILENT)){
					attacker->println("You can't be ordered to initiate PK combat unless your master can attack them.");
					attacker->master->println("You can't ordered people or mobs to initiate PK combat unless you can attack the target!" );
				}
				return false;
			}
		}else{
			if(!IS_SET(type,CIT_SILENT)){
				attacker->println("You can't be ordered to initiate PK combat.");
			}
			return false;
		}
	}

	if(IS_SET(type,CIT_GENERAL)){
		// kill stealing check
		if ( IS_NPC(victim) &&
			victim->fighting != NULL &&
			!is_same_group(attacker,victim->fighting))
		{
			if(!IS_SET(type,CIT_SILENT)){
				attacker->println("Kill stealing is not permitted.");
			}
			return false;
		}
	};

	if(IS_NPC(victim)){ // mobs don't require duelling
		return true;
	}

	if(victim==attacker){ // casting slow on yourself etc
		return true;
	}



	if(!GAMESETTING2(GAMESET2_NO_DUEL_REQUIRED)
		&& IS_SET(type,(CIT_GENERAL | CIT_AGRESSIVE_ACTION | CIT_CASTING_SPELL))){

		// pksystem checks - only apply to morts
		if( !IS_ICIMMORTAL(attacker))
		{
			// bypass duel system (spells can't make use of it)
			if(attacker->duels
				&& !IS_SET(type,CIT_NO_BYPASSDUEL)
				&& !IS_SET(type,CIT_CASTING_SPELL)
				&& attacker->duels->is_bypassingduel(victim))
			{
				return true;
			}

			if(IS_ACTIVE(victim)){
				// check for peaceful attempting to attack an active player
				if(IS_PEACEFUL(attacker)){
					// peaceful must bypassduel (or duel a thief) to attack an active player
					if(!attacker->duels || !attacker->duels->is_known(victim)){
						if(!IS_SET(type,CIT_SILENT)){
							attacker->printlnf("%s is an active player, because you are a peaceful character, your options\r\n"
								"to initiate conflict with them are one of the following:\r\n"
								"  1. Become an active player yourself.\r\n"
								"  2. Bypassduel the duelling system.\r\n"
								"  3. If they have been stealing in the last 20 minutes, you may challenge them to a duel.",
								CPERS(victim, attacker));
						}
						return false;
					}
					if(!attacker->duels->is_dueling(attacker, victim)){
						if(!IS_SET(type,CIT_SILENT)){
							attacker->printlnf("You can't initiate conflict with %s at this point in time.",
								PERS(victim, attacker));
						}
						return false;
					}
					return true;
				}
			}else{
				// The victim is peaceful, duel is required regardless of the
				// pk status of the attacker
				if(!attacker->duels || !attacker->duels->is_known(victim)){
					if(!IS_SET(type,CIT_SILENT)){
						attacker->println("You must duel them or bypass the dueling system before you attempt to initiate conflict!");
					}
					return false;
				}
				if(!attacker->duels->is_dueling(attacker, victim)){
					if(!IS_SET(type,CIT_SILENT)){
						attacker->printlnf("You can't initiate conflict with %s at this point in time.",
							PERS(victim, attacker));
					}
					return false;
				}
				return true;
			}
		}
	}
	return true;
}
/**************************************************************************/
// return true if the attacker can join the current combat (autoassist)
// - handles all the duel/bypass duel rules etc
bool can_join_combat( char_data *attacker, char_data *victim)
{
	// can always join in when fighting a mob
	// pets can always join
	// can always join in on a dedicated pkill mud
	if(	   IS_NPC(attacker)
		|| IS_NPC(victim)
		|| GAMESETTING5(GAMESET5_DEDICATED_PKILL_STYLE_MUD)){
		return true;
	}

	if(can_initiate_combat_with_player( attacker, victim, CIT_GENERAL)){
		return true;
	};

// Rules for group combat
//===attacker===victim=======condition===
//	* active	active		- can always
//	* active	peaceful	- normal + join in if victim->fighting is duelling victim && same group as victim->fighting
//	* peaceful	active		- normal + join in if victim->fighting is duelling victim && same group as victim->fighting
//	* peaceful	peaceful	- normal + join in if victim->fighting is duelling victim && same group as victim->fighting

	if(	victim->fighting
		&& victim->fighting->fighting==victim
		&& is_same_group(victim->fighting, attacker)
		&& victim->fighting->duels
		&& victim->fighting->duels->is_dueling(victim->fighting, victim))
	{
		return true;
	}

	attacker->println("You need to duel or `=Cbypassduel`x to join in.");
	return false;

}
/**************************************************************************/
// able to avoid combat by fading
bool check_fade( char_data* ch, char_data* victim )
{
    int chance;

    if ( IS_NPC(victim) && !IS_SET(victim->off_flags, OFF_FADE) ){
        return false;
	}

    if ( !IS_NPC(victim) && !IS_AFFECTED2(victim, AFF2_FADE) ){
		return false;
	}

	if (victim->is_stunned){
		return false;
	}

    if ( !IS_AWAKE(victim) ){
        return false;
	}

    chance = get_skill(victim, gsn_fade) / 3;
	if(chance < 1){
		return false;
	}
    if ( number_percent( ) >= chance + victim->level - ch->level){
        return false;
	}

    if (victim->is_stunned){
		return false;
	}

    act( "`bYou fade around $n's attack.`x", ch, NULL, victim, TO_VICT );
    act( "`b$n fades around your attack.`x", victim, NULL, ch, TO_VICT );
    check_improve(victim, gsn_fade, true, 6);
    return true;
}
/**************************************************************************/
void do_fire( char_data *ch, char *argument )
{
//	char		arg[MIL];
	char_data	*victim = NULL;
	char arg1[MIL], arg2[MIL];
    //	OBJ_DATA 	*arrow = NULL;
    	OBJ_DATA 	*bow = NULL;
	int		dam, door, chance;
    	ROOM_INDEX_DATA *was_in_room;
		ROOM_INDEX_DATA	*in_room, *to_room;
    	EXIT_DATA 	*pexit;
		int dam2;
		int damage_to_inflict;
	bow = get_eq_char( ch, WEAR_WIELD );
	argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	if ( !bow ){
		ch->println("You must be wielding a missile weapon.");
		return;}

    /*if (IS_OOC(ch))
    {
	do_huh(ch,"");
	return;
    }
*/
	if ( bow->value[0]!=WEAPON_LONG_BOW &&
	     bow->value[0]!=WEAPON_SHORT_BOW &&
		 bow->value[0]!=WEAPON_CROSSBOW){
		ch->println("You must be wielding a missile weapon.");
		return;}

	if ( bow->value[0] == WEAPON_SHORT_BOW ||
		 bow->value[0] == WEAPON_LONG_BOW){
		chance = get_skill( ch, gsn_bow );
		if ( chance < 1){
			do_huh(ch,"");
			return;}
	}
	if ( bow->value[0] == WEAPON_CROSSBOW){
		chance = get_skill( ch, gsn_crossbow );
		if ( chance < 1){
			do_huh(ch,"");
			return;}
	}

	int dt;
	dt = bow->value[3];
	if ( dt == TYPE_UNDEFINED ){
		dt = TYPE_HIT;
		if ( bow != NULL && bow->item_type == ITEM_WEAPON ){
			dt += bow->value[3];
		}else{
			dt += ch->dam_type;}
	}
   /* if ( bow->value[0] == WEAPON_BLOWGUN)
    {
	chance = get_skill( ch, gsn_blowgun );
	if ( chance < 1)
	{
		do_huh(ch,"");
		return;
	}
    }

    if ( bow->value[0] == WEAPON_SLING)
    {
	chance = get_skill( ch, gsn_sling );
	if ( chance < 1)
	{
		do_huh(ch,"");
		return;
	}
    }
*/
   /* if( ( arrow = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
        ch->println( "You are not holding a projectile!");
        return;
    }

    if( arrow->item_type != ITEM_PROJECTILE )
    {
        ch->println( "You are not holding a projectile!");
        return;
    }

    if ( !can_drop_obj( ch, arrow ))
    {
		ch->println("You can't let go of that.");
		return;
    }*/

//	one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
        if ( ch->fighting )
        {
            victim = ch->fighting;
        }
        else
        {
            ch->println( "Fire at whom or what?");
            return;
        }
    }

	door = dir_lookup( arg2 );

    if ( door == -1 )
	{
		ch->printlnf( "'%s' is an invalid direction.", arg2 );
		ch->println("Syntax: fire <target> <direction>");
		return;
    }

	in_room = ch->in_room;
	to_room = NULL;

	if ( (( pexit   = in_room->exit[door] ) == NULL
	||  ( to_room = pexit->u1.to_room   ) == NULL
	||  !can_see_room( ch, pexit->u1.to_room ))
	&&  strcmp( arg2, "down" ))
	{
		ch->println("You cannot shoot in that direction.");
		return;
    }

	/* See if who you are trying to shoot at is nearby... */
          if ( ( victim = get_char_room ( ch, arg1 ) ) == NULL)
        {
            was_in_room = ch->in_room;

                if ( (  pexit = was_in_room->exit[door] ) != NULL
                   &&   pexit->u1.to_room != NULL
                   &&   pexit->u1.to_room != was_in_room
                   &&   !strcmp( pexit->u1.to_room->area->name ,
                                was_in_room->area->name ) )
                   {
                     ch->in_room = pexit->u1.to_room;
                     victim = get_char_room ( ch, arg1 );
                    }


            ch->in_room = was_in_room;

            if(victim==NULL)
              {
               ch->println( "You can't seem to locate your target.");
               return;
              }
            else
              {  if(IS_SET(pexit->exit_info,EX_CLOSED))
                    { ch->println("You can't fire through a door.");
                      return;
                     }
             }
        }
   	if((ch->in_room) == (victim->in_room))
    	{
		ch->println("Don't you think that standing a bit further away would be wise?");
		return;
    	}

 	if(ch == victim)
 	{
     	   ch->println("Yeah, right.");
      	   return;
    	}

	if ( is_safe( ch, victim ))
	{
     	   	ch->println("You cannot seem to target them for some reason.");
		return;
	}

	if(IS_NPC(victim) && (IS_SET(victim->act, ACT_NO_HUNT) ||
	   IS_SET(victim->act, ACT_DONT_WANDER)) )
	{
     	   	ch->println("You cannot seem to target them for some reason.");
		return;
	}

    	if ( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim )
	{
		act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
		return;

    }



      /*switch( arrow->value[0] )
      {
        case PROJ_BOLT:
		if(bow->value[0]!=WEAPON_HEAVY_CROSSBOW &&
	       bow->value[0]!=WEAPON_LIGHT_CROSSBOW &&
           bow->value[0]!=WEAPON_HAND_CROSSBOW)
		{
			ch->println("You need to be holding a bolt to use with that.");
			return;
		}
		break;
        case PROJ_ARROW:
		if(bow->value[0]!=WEAPON_COMPOSITE_LONG_BOW &&
		   bow->value[0]!=WEAPON_COMPOSITE_SHORT_BOW &&
		   bow->value[0]!=WEAPON_LONG_BOW &&
		   bow->value[0]!=WEAPON_SHORT_BOW)
		{
			ch->println("You need to be holding an arrow to use with that.");
			return;
		}
		break;
        case PROJ_DART:
		if(bow->value[0] != WEAPON_BLOWGUN)
		{
			ch->println("You need to be holding a dart to use with that.");
			return;
		}
		break;
        case PROJ_STONE:
		if(bow->value[0] != WEAPON_SLING)
		{
			ch->println("You need to be holding a stone to use with that.");
			return;
		}
		break;

      }*/


    /* Lag the bowman... */
	if (!IS_IMMORTAL(ch)){
    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
	}
     /* Fire the weapon */
    /*if(arrow->value[0] == PROJ_ARROW )
    {
	act( "$n fire $p at $N.",  ch, arrow, victim, TO_NOTVICT );
	act( "You fire $p at $N.",  ch, arrow, victim, TO_CHAR );
	act( "$n fires $p at you.", ch, arrow, victim, TO_VICT );*/

	/* Fire the weapon with unlimitted ammo */

	if(bow->value[0] == WEAPON_SHORT_BOW ||
	   bow->value[0] == WEAPON_LONG_BOW){

	act( "$n fires $p at $N.",  ch, bow, victim, TO_NOTVICT );
	act( "You fire $p at $N.",  ch, bow, victim, TO_CHAR );
//	act( "$n fires $p at you.", ch, bow, victim, TO_VICT );

	/*if (( ch->level + 5 ) < arrow->level
	||  number_percent() >= 10 + get_skill( ch, gsn_bow ) * 4/5 )
	{
		act( "$p misses $N and lands on the ground.", ch, arrow, victim, TO_CHAR );
		act( "$n fires $p but it misses and lands on the ground.", ch, arrow, NULL, TO_ROOM );
		act( "$p flies into the room and lands on the ground.", victim, arrow, NULL, TO_ROOM );
		obj_from_char( arrow );
		obj_to_room( arrow, ch->in_room );
		check_improve( ch, gsn_bow, false, 1 );
	    if ( IS_OBJ_STAT( arrow, OBJEXTRA_MELT_DROP ))
	    {
			act( "$p dissolves into smoke.", ch, arrow, NULL, TO_ROOM );
			extract_obj( arrow );
		}
	}*/
	if (( ch->level + 5 ) < bow->level
	||  number_percent() >= 10 + get_skill( ch, gsn_bow ) * 4/5 )
	{
		act( "$p misses $N !", ch, bow, victim, TO_CHAR );
		act( "$n fires $p but it misses!", ch, bow, NULL, TO_ROOM );
		act( "$n fires $p at you but misses.", ch, bow, victim, TO_VICT );
		check_improve( ch, gsn_bow, false, 1 );
	}
	else

	{


		chance = number_range(1,10);
		switch (chance)
		{
			case 1:

				break;
			case 2:

				break;
			case 3:

				break;
			case 4:

				break;
			case 5:
				act( "$n shoots you with $p .", ch, bow, victim, TO_VICT );
				if (IS_WEAPON_STAT(bow,WEAPON_POISON))
				{
				int level;
				AFFECT_DATA *poison, af;
				poison = affect_find(bow->affected,gsn_poison);
				if(poison){
				level = poison->level;
				}else{
				level = bow->level;}
				if (!saves_spell(level / 2,victim,DAM_POISON)
				&& !HAS_CLASSFLAG(victim, CLASSFLAG_POISON_IMMUNITY))
				{
				victim->println("You feel poison coursing through your veins.");
				act("$n is poisoned by the venom on $p.",
					victim,bow,NULL,TO_ROOM);
				af.where     = WHERE_AFFECTS;
				af.type      = gsn_poison;
				af.level     = level * 3/4;
				af.duration  = level / 2;
				af.location  = APPLY_ST;
				af.modifier  = -1;
				af.bitvector = AFF_POISON;
				affect_join( victim, &af );
				}
				// weaken the poison if it's temporary
				if(poison){
				poison->level = UMAX(0,poison->level - 2);
				poison->duration = UMAX(0,poison->duration - 1);
				if (IS_ALIVE(victim) && (poison->level == 0 || poison->duration == 0)){
				act("The poison on $p has worn off.",ch,bow,NULL,TO_CHAR);}
				}
				}
				if (IS_WEAPON_STAT(bow,WEAPON_FLAMING))
				{
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_FIRE);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is schorched by $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is scorched by $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p sears your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				fire_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_FIRE);}
				}
				// acidic weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ACIDIC))
				{
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_ACID);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is burned by $p's acid.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is burned by $p's acid.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p melts your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				acid_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_ACID);}
				}
				// frost weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_FROST))
				{
				dam2 = number_range(1,bow->level / 6 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_COLD);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$p freezes $N.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$p freezes $N.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("The cold touch of $p surrounds you with ice.",
				victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				cold_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_COLD);}
				}
				// shocking weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_SHOCKING))
				{
				dam2 = number_range(1,bow->level/5 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_LIGHTNING);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is struck by lightning from $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is struck by lightning from $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("You are shocked by $p",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				shock_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0,DAM_LIGHTNING);}
				}
				// annealed weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ANNEALED))
				{
				dam2 = number_range(1,bow->level/5 + 2);
				if (!saves_spell(bow->level/4 + dam2/20,victim,DAM_BASH)){
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_BASH);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				victim->printlnf("You see stars as your head `Crings`x!%s",autodamtext(victim, damage_to_inflict));
				ch->printlnf("Your opponent looks `Bstunned`x!%s",autodamtext(ch, damage_to_inflict));
				// inflict the custom effect before the standard damage
				DAZE_STATE(victim,UMAX(12,bow->level/4 + dam2/20));
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0, DAM_BASH);}
				}
				}
				dam = dice( bow->value[1], bow->value[2] );
				// if weapon is above their level reduce the damage accordingly
				if (bow->level>ch->level){
				dam=dam * ch->level / bow->level;}
				// If weapon is blessed & enemy is undead
				if( IS_SET(bow->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD )){
				dam = dam * 3 / 2;}
				if (IS_WEAPON_STAT(bow,WEAPON_SHARP)){
				int percent;
				if ((percent = number_percent()) <= (gsn_bow / 8))
				dam = 2 * dam + (dam * 2 * percent / 100);}
				if (IS_WEAPON_STAT(bow,WEAPON_VORPAL)){
				int percent;
				if((percent=number_percent()) <=(gsn_bow/33))
				dam = 6 * dam + (dam*4*percent/100);}
				if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_GOOD)){
				dam = dam + (victim->alliance/3)* (dam*5/4);}
				if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_EVIL) ){
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);}
				if ( damage( ch, victim, dam, gsn_bow, dt, true ))
//		    	act( "An arrow flies into the room and hits $n.", victim, TO_ROOM );
//				act( "$p hits $N !", ch, bow, victim, TO_CHAR );

				check_improve( ch, gsn_bow, true, 1 );
				break;
			case 6:
			case 7:
			case 8:
				act( "$n shoots you with $p .", ch, bow, victim, TO_VICT );
				if (IS_WEAPON_STAT(bow,WEAPON_POISON))
				{
				int level;
				AFFECT_DATA *poison, af;
				poison = affect_find(bow->affected,gsn_poison);
				if(poison){
				level = poison->level;
				}else{
				level = bow->level;}
				if (!saves_spell(level / 2,victim,DAM_POISON)
				&& !HAS_CLASSFLAG(victim, CLASSFLAG_POISON_IMMUNITY))
				{
				victim->println("You feel poison coursing through your veins.");
				act("$n is poisoned by the venom on $p.",
					victim,bow,NULL,TO_ROOM);
				af.where     = WHERE_AFFECTS;
				af.type      = gsn_poison;
				af.level     = level * 3/4;
				af.duration  = level / 2;
				af.location  = APPLY_ST;
				af.modifier  = -1;
				af.bitvector = AFF_POISON;
				affect_join( victim, &af );
				}
				// weaken the poison if it's temporary
				if(poison){
				poison->level = UMAX(0,poison->level - 2);
				poison->duration = UMAX(0,poison->duration - 1);
				if (IS_ALIVE(victim) && (poison->level == 0 || poison->duration == 0)){
				act("The poison on $p has worn off.",ch,bow,NULL,TO_CHAR);}
				}
				}
				if (IS_WEAPON_STAT(bow,WEAPON_FLAMING))
				{
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_FIRE);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is schorched by $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is scorched by $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p sears your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				fire_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_FIRE);}
				}
				// acidic weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ACIDIC))
				{
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_ACID);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is burned by $p's acid.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is burned by $p's acid.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p melts your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				acid_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_ACID);}
				}
				// frost weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_FROST))
				{
				dam2 = number_range(1,bow->level / 6 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_COLD);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$p freezes $N.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$p freezes $N.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("The cold touch of $p surrounds you with ice.",
				victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				cold_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_COLD);}
				}
				// shocking weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_SHOCKING))
				{
				dam2 = number_range(1,bow->level/5 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_LIGHTNING);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is struck by lightning from $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is struck by lightning from $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("You are shocked by $p",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				shock_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0,DAM_LIGHTNING);}
				}
				// annealed weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ANNEALED))
				{
				dam2 = number_range(1,bow->level/5 + 2);
				if (!saves_spell(bow->level/4 + dam2/20,victim,DAM_BASH)){
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_BASH);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				victim->printlnf("You see stars as your head `Crings`x!%s",autodamtext(victim, damage_to_inflict));
				ch->printlnf("Your opponent looks `Bstunned`x!%s",autodamtext(ch, damage_to_inflict));
				// inflict the custom effect before the standard damage
				DAZE_STATE(victim,UMAX(12,bow->level/4 + dam2/20));
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0, DAM_BASH);}
				}
				}
				dam = 3 * ( dice( bow->value[1], bow->value[2] ));
				// if weapon is above their level reduce the damage accordingly
				if (bow->level>ch->level){
				dam=dam * ch->level / bow->level;}
				// If weapon is blessed & enemy is undead
				if( IS_SET(bow->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD )){
				dam = dam * 3 / 2;}
				if (IS_WEAPON_STAT(bow,WEAPON_SHARP)){
				int percent;
				if ((percent = number_percent()) <= (gsn_bow / 8))
				dam = 2 * dam + (dam * 2 * percent / 100);}
				if (IS_WEAPON_STAT(bow,WEAPON_VORPAL)){
				int percent;
				if((percent=number_percent()) <=(gsn_bow/33))
				dam = 6 * dam + (dam*4*percent/100);}
				if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_GOOD)){
				dam = dam + (victim->alliance/3)* (dam*5/4);}
				if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_EVIL) ){
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);}
                if ( damage( ch, victim, dam, gsn_bow, dt, true ))
//		    	act( "An arrow flies into the room and hits $n.", victim, TO_ROOM );
//				act( "$p hits $N !", ch, bow, victim, TO_CHAR );

	//			one_hit( ch, victim, dt, false );
				check_improve( ch, gsn_bow, true, 1 );

				break;
			case 9:
			case 10:
				act( "$n shoots you with $p .", ch, bow, victim, TO_VICT );
				if (IS_WEAPON_STAT(bow,WEAPON_POISON)){
				int level;
				AFFECT_DATA *poison, af;
				poison = affect_find(bow->affected,gsn_poison);
				if(poison){
				level = poison->level;
				}else{
				level = bow->level;}
				if (!saves_spell(level / 2,victim,DAM_POISON)
				&& !HAS_CLASSFLAG(victim, CLASSFLAG_POISON_IMMUNITY)){
				victim->println("You feel poison coursing through your veins.");
				act("$n is poisoned by the venom on $p.",
					victim,bow,NULL,TO_ROOM);
				af.where     = WHERE_AFFECTS;
				af.type      = gsn_poison;
				af.level     = level * 3/4;
				af.duration  = level / 2;
				af.location  = APPLY_ST;
				af.modifier  = -1;
				af.bitvector = AFF_POISON;
				affect_join( victim, &af );}
				// weaken the poison if it's temporary
				if(poison){
				poison->level = UMAX(0,poison->level - 2);
				poison->duration = UMAX(0,poison->duration - 1);
				if (IS_ALIVE(victim) && (poison->level == 0 || poison->duration == 0)){
				act("The poison on $p has worn off.",ch,bow,NULL,TO_CHAR);}
				}
				}
				if (IS_WEAPON_STAT(bow,WEAPON_FLAMING)){
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_FIRE);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is schorched by $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is scorched by $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p sears your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				fire_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_FIRE);}
				}
				// acidic weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ACIDIC))
				{
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_ACID);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is burned by $p's acid.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is burned by $p's acid.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p melts your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				acid_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_ACID);}
				}
				// frost weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_FROST))
				{
				dam2 = number_range(1,bow->level / 6 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_COLD);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$p freezes $N.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$p freezes $N.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("The cold touch of $p surrounds you with ice.",
				victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				cold_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_COLD);}
				}
				// shocking weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_SHOCKING))
				{
				dam2 = number_range(1,bow->level/5 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_LIGHTNING);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is struck by lightning from $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is struck by lightning from $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("You are shocked by $p",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				shock_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0,DAM_LIGHTNING);}
				}
				// annealed weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ANNEALED))
				{
				dam2 = number_range(1,bow->level/5 + 2);
				if (!saves_spell(bow->level/4 + dam2/20,victim,DAM_BASH)){
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_BASH);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				victim->printlnf("You see stars as your head `Crings`x!%s",autodamtext(victim, damage_to_inflict));
				ch->printlnf("Your opponent looks `Bstunned`x!%s",autodamtext(ch, damage_to_inflict));
				// inflict the custom effect before the standard damage
				DAZE_STATE(victim,UMAX(12,bow->level/4 + dam2/20));
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0, DAM_BASH);}
				}
				}
				dam = 2 * ( dice( bow->value[1], bow->value[2] ));
				// if weapon is above their level reduce the damage accordingly
				if (bow->level>ch->level){
				dam=dam * ch->level / bow->level;}
				// If weapon is blessed & enemy is undead
				if( IS_SET(bow->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD )){
				dam = dam * 3 / 2;}
				if (IS_WEAPON_STAT(bow,WEAPON_SHARP)){
				int percent;
				if ((percent = number_percent()) <= (gsn_bow / 8))
				dam = 2 * dam + (dam * 2 * percent / 100);}
				if (IS_WEAPON_STAT(bow,WEAPON_VORPAL)){
				int percent;
				if((percent=number_percent()) <=(gsn_bow/33))
				dam = 6 * dam + (dam*4*percent/100);}
				if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_GOOD)){
				dam = dam + (victim->alliance/3)* (dam*5/4);}
				if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_EVIL) ){
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);}
				if ( damage( ch, victim, dam, gsn_bow, dt, true ))
//		    	act( "An arrow flies into the room and hits $n.", victim, TO_ROOM );
//				act( "$p hits $N !", ch, bow, victim, TO_CHAR );
				check_improve( ch, gsn_bow, true, 1 );
				break;
		}
	}

	return;
    }



    if(bow->value[0] == WEAPON_CROSSBOW ){

	act( "$n fires $p at $N.",  ch, bow, victim, TO_NOTVICT );
	act( "You fire $p at $N.",  ch, bow, victim, TO_CHAR );
//	act( "$n fires $p at you.", ch, bow, victim, TO_VICT );

	if (( ch->level + 5 ) < bow->level
	||  number_percent() >= 10 + get_skill( ch, gsn_crossbow ) * 4/5 ){
		act( "$p misses $N.", ch, bow, victim, TO_CHAR );
		act( "$n fires $p but it misses!", ch, bow, NULL, TO_ROOM );
		act( "Something zips by your head!", victim, bow, NULL, TO_ROOM );
		check_improve( ch, gsn_crossbow, false, 1 );}
	else{

		chance = number_range(1,10);
		switch (chance){
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				act( "$p hits $N !", ch, bow, victim, TO_CHAR );
				act( "$n shoots you with $p .", ch, bow, victim, TO_VICT );
				if (IS_WEAPON_STAT(bow,WEAPON_POISON)){
				int level;
				AFFECT_DATA *poison, af;
				poison = affect_find(bow->affected,gsn_poison);
				if(poison){
				level = poison->level;
				}else{
				level = bow->level;}
				if (!saves_spell(level / 2,victim,DAM_POISON)
				&& !HAS_CLASSFLAG(victim, CLASSFLAG_POISON_IMMUNITY)){
				victim->println("You feel poison coursing through your veins.");
				act("$n is poisoned by the venom on $p.",
					victim,bow,NULL,TO_ROOM);
				af.where     = WHERE_AFFECTS;
				af.type      = gsn_poison;
				af.level     = level * 3/4;
				af.duration  = level / 2;
				af.location  = APPLY_ST;
				af.modifier  = -1;
				af.bitvector = AFF_POISON;
				affect_join( victim, &af );}
				// weaken the poison if it's temporary
				if(poison){
				poison->level = UMAX(0,poison->level - 2);
				poison->duration = UMAX(0,poison->duration - 1);
				if (IS_ALIVE(victim) && (poison->level == 0 || poison->duration == 0)){
				act("The poison on $p has worn off.",ch,bow,NULL,TO_CHAR);}
				}
				}
				if (IS_WEAPON_STAT(bow,WEAPON_FLAMING)){
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_FIRE);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is schorched by $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is scorched by $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p sears your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				fire_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_FIRE);}
				}
				// acidic weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ACIDIC))
				{
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_ACID);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is burned by $p's acid.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is burned by $p's acid.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p melts your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				acid_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_ACID);}
				}
				// frost weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_FROST))
				{
				dam2 = number_range(1,bow->level / 6 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_COLD);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$p freezes $N.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$p freezes $N.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("The cold touch of $p surrounds you with ice.",
				victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				cold_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_COLD);}
				}
				// shocking weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_SHOCKING))
				{
				dam2 = number_range(1,bow->level/5 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_LIGHTNING);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is struck by lightning from $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is struck by lightning from $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("You are shocked by $p",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				shock_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0,DAM_LIGHTNING);}
				}
				// annealed weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ANNEALED))
				{
				dam2 = number_range(1,bow->level/5 + 2);
				if (!saves_spell(bow->level/4 + dam2/20,victim,DAM_BASH)){
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_BASH);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				victim->printlnf("You see stars as your head `Crings`x!%s",autodamtext(victim, damage_to_inflict));
				ch->printlnf("Your opponent looks `Bstunned`x!%s",autodamtext(ch, damage_to_inflict));
				// inflict the custom effect before the standard damage
				DAZE_STATE(victim,UMAX(12,bow->level/4 + dam2/20));
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0, DAM_BASH);}
				}
				}
				dam = dice( bow->value[1], bow->value[2] );
				// if weapon is above their level reduce the damage accordingly
				if (bow->level>ch->level){
				dam=dam * ch->level / bow->level;}
				// If weapon is blessed & enemy is undead
				if( IS_SET(bow->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD )){
				dam = dam * 3 / 2;}
				if (IS_WEAPON_STAT(bow,WEAPON_SHARP)){
				int percent;
				if ((percent = number_percent()) <= (gsn_crossbow / 8))
				dam = 2 * dam + (dam * 2 * percent / 100);}
				if (IS_WEAPON_STAT(bow,WEAPON_VORPAL)){
				int percent;
				if((percent=number_percent()) <=(gsn_crossbow/33))
				dam = 6 * dam + (dam*4*percent/100);}
				if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_GOOD)){
				dam = dam + (victim->alliance/3)* (dam*5/4);}
				if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_EVIL) ){
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);}
				if ( damage( ch, victim, dam, gsn_crossbow, dt, true ))
//				act( "A bolt flies into the room and hits $n.", victim, NULL, TO_ROOM );
				check_improve( ch, gsn_crossbow, true, 1 );
				break;
			case 6:
			case 7:
			case 8:
				act( "$p hits $N !", ch, bow, victim, TO_CHAR );
				act( "$n shoots you with $p .", ch, bow, victim, TO_VICT );
				if (IS_WEAPON_STAT(bow,WEAPON_POISON)){
				int level;
				AFFECT_DATA *poison, af;
				poison = affect_find(bow->affected,gsn_poison);
				if(poison){
				level = poison->level;
				}else{
				level = bow->level;}
				if (!saves_spell(level / 2,victim,DAM_POISON)
				&& !HAS_CLASSFLAG(victim, CLASSFLAG_POISON_IMMUNITY)){
				victim->println("You feel poison coursing through your veins.");
				act("$n is poisoned by the venom on $p.",
					victim,bow,NULL,TO_ROOM);
				af.where     = WHERE_AFFECTS;
				af.type      = gsn_poison;
				af.level     = level * 3/4;
				af.duration  = level / 2;
				af.location  = APPLY_ST;
				af.modifier  = -1;
				af.bitvector = AFF_POISON;
				affect_join( victim, &af );}
				// weaken the poison if it's temporary
				if(poison){
				poison->level = UMAX(0,poison->level - 2);
				poison->duration = UMAX(0,poison->duration - 1);
				if (IS_ALIVE(victim) && (poison->level == 0 || poison->duration == 0)){
				act("The poison on $p has worn off.",ch,bow,NULL,TO_CHAR);}
				}
				}
				if (IS_WEAPON_STAT(bow,WEAPON_FLAMING)){
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_FIRE);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is schorched by $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is scorched by $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p sears your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				fire_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_FIRE);}
				}
				// acidic weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ACIDIC))
				{
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_ACID);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is burned by $p's acid.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is burned by $p's acid.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p melts your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				acid_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_ACID);}
				}
				// frost weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_FROST)){
				dam2 = number_range(1,bow->level / 6 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_COLD);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$p freezes $N.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$p freezes $N.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("The cold touch of $p surrounds you with ice.",
				victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				cold_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_COLD);}
				}
				// shocking weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_SHOCKING)){
				dam2 = number_range(1,bow->level/5 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_LIGHTNING);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is struck by lightning from $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is struck by lightning from $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("You are shocked by $p",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				shock_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0,DAM_LIGHTNING);}
				}
				// annealed weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ANNEALED)){
				dam2 = number_range(1,bow->level/5 + 2);
				if (!saves_spell(bow->level/4 + dam2/20,victim,DAM_BASH)){
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_BASH);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				victim->printlnf("You see stars as your head `Crings`x!%s",autodamtext(victim, damage_to_inflict));
				ch->printlnf("Your opponent looks `Bstunned`x!%s",autodamtext(ch, damage_to_inflict));
				// inflict the custom effect before the standard damage
				DAZE_STATE(victim,UMAX(12,bow->level/4 + dam2/20));
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0, DAM_BASH);}
				}
				}
				dam = 3 * ( dice( bow->value[1], bow->value[2] )/2);
				// if weapon is above their level reduce the damage accordingly
				if (bow->level>ch->level){
				dam=dam * ch->level / bow->level;}
				// If weapon is blessed & enemy is undead
				if( IS_SET(bow->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD )){
				dam = dam * 3 / 2;}
				if (IS_WEAPON_STAT(bow,WEAPON_SHARP)){
				int percent;
				if ((percent = number_percent()) <= (gsn_crossbow / 8))
				dam = 2 * dam + (dam * 2 * percent / 100);}
				if (IS_WEAPON_STAT(bow,WEAPON_VORPAL)){
				int percent;
				if((percent=number_percent()) <=(gsn_crossbow/33))
				dam = 6 * dam + (dam*4*percent/100);}
				if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_GOOD)){
				dam = dam + (victim->alliance/3)* (dam*5/4);}
				if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_EVIL) ){
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);}
				if ( damage( ch, victim, dam, gsn_crossbow, dt, true ))
//				act( "A bolt flies into the room and hits $n.", victim, NULL, TO_ROOM );
				check_improve( ch, gsn_crossbow, true, 1 );
				break;
			case 9:
			case 10:
				act( "$p hits $N !", ch, bow, victim, TO_CHAR );
				act( "$n shoots you with $p .", ch, bow, victim, TO_VICT );
				if (IS_WEAPON_STAT(bow,WEAPON_POISON)){
				int level;
				AFFECT_DATA *poison, af;
				poison = affect_find(bow->affected,gsn_poison);
				if(poison){
				level = poison->level;
				}else{
				level = bow->level;}
				if (!saves_spell(level / 2,victim,DAM_POISON)
				&& !HAS_CLASSFLAG(victim, CLASSFLAG_POISON_IMMUNITY)){
				victim->println("You feel poison coursing through your veins.");
				act("$n is poisoned by the venom on $p.",
					victim,bow,NULL,TO_ROOM);
				af.where     = WHERE_AFFECTS;
				af.type      = gsn_poison;
				af.level     = level * 3/4;
				af.duration  = level / 2;
				af.location  = APPLY_ST;
				af.modifier  = -1;
				af.bitvector = AFF_POISON;
				affect_join( victim, &af );}
				// weaken the poison if it's temporary
				if(poison){
				poison->level = UMAX(0,poison->level - 2);
				poison->duration = UMAX(0,poison->duration - 1);
				if (IS_ALIVE(victim) && (poison->level == 0 || poison->duration == 0)){
				act("The poison on $p has worn off.",ch,bow,NULL,TO_CHAR);}
				}
				}
				if (IS_WEAPON_STAT(bow,WEAPON_FLAMING)){
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_FIRE);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is schorched by $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is scorched by $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p sears your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				fire_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_FIRE);}
				}
				// acidic weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ACIDIC)){
				dam2 = number_range(1,bow->level / 4 + 1);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_ACID);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is burned by $p's acid.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is burned by $p's acid.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("$p melts your flesh.",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				acid_effect( (void *) victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_ACID);}
				}
				// frost weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_FROST)){
				dam2 = number_range(1,bow->level / 6 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_COLD);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$p freezes $N.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$p freezes $N.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("The cold touch of $p surrounds you with ice.",
				victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				cold_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_COLD);}
				}
				// shocking weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_SHOCKING)){
				dam2 = number_range(1,bow->level/5 + 2);
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_LIGHTNING);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is struck by lightning from $p.",ch,bow,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is struck by lightning from $p.",ch,bow,victim,damage_to_inflict);
				act_with_autodam_to_char("You are shocked by $p",victim,bow,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				shock_effect(victim,bow->level/2,dam2,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0,DAM_LIGHTNING);}
				}
				// annealed weapon effect *
				if (IS_WEAPON_STAT(bow,WEAPON_ANNEALED)){
				dam2 = number_range(1,bow->level/5 + 2);
				if (!saves_spell(bow->level/4 + dam2/20,victim,DAM_BASH)){
				damage_to_inflict=damage_calculate_infliction(ch,victim,dam2,0,DAM_BASH);
				if(damage_to_inflict>=1){
				// display the custom damage messages
				victim->printlnf("You see stars as your head `Crings`x!%s",autodamtext(victim, damage_to_inflict));
				ch->printlnf("Your opponent looks `Bstunned`x!%s",autodamtext(ch, damage_to_inflict));
				// inflict the custom effect before the standard damage
				DAZE_STATE(victim,UMAX(12,bow->level/4 + dam2/20));
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0, DAM_BASH);}
				}
				}
				dam = 2 * ( dice( bow->value[1], bow->value[2] ));
				// if weapon is above their level reduce the damage accordingly
				if (bow->level>ch->level){
				dam=dam * ch->level / bow->level;}
				// If weapon is blessed & enemy is undead
				if( IS_SET(bow->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD )){
				dam = dam * 3 / 2;}
				if (IS_WEAPON_STAT(bow,WEAPON_SHARP)){
				int percent;
				if ((percent = number_percent()) <= (gsn_crossbow / 8))
				dam = 2 * dam + (dam * 2 * percent / 100);}
				if (IS_WEAPON_STAT(bow,WEAPON_VORPAL)){
				int percent;
				if((percent=number_percent()) <=(gsn_crossbow/33))
				dam = 6 * dam + (dam*4*percent/100);}
				if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_GOOD)){
				dam = dam + (victim->alliance/3)* (dam*5/4);}
				if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(bow,WEAPON_HOLY)
				&& IS_OBJ_STAT(bow,OBJEXTRA_ANTI_EVIL) ){
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);}
				if ( damage( ch, victim, dam, gsn_crossbow, dt, true ))
//				act( "A bolt flies into the room and hits $n.", victim, NULL, TO_ROOM );
				check_improve( ch, gsn_crossbow, true, 1 );
				break;}
	}

	return;}

}

/************************************************************************/
void do_bite( char_data *ch, char *argument )
{
	int damage_to_inflict;
	int dam;
	char_data *victim;

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	if(IS_NULLSTR(argument)){
		victim = ch->fighting;
		if (victim == NULL){
			ch->println("But you aren't fighting anyone!");
			return;}
	}
	else if ((victim = get_char_room(ch,argument)) == NULL){
		ch->printlnf("'%s' is not here.", argument);
		return;}

	if (victim == ch){
		ch->println("You try to bite yourself but fail.");
		return;}

	if (is_safe(ch,victim))
		return;

	if (ch->race == race_lookup("vampyre")){
		if (ch->fighting == victim && (IS_AFFECTED2(ch, AFF2_VAMP_BITE)||
			IS_VAMPAFF(ch, VAMPYRE_FANGS))){
			dam = number_range(1, ch->level);

			damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_NEGATIVE);
			if(damage_to_inflict>=0){
				// *** patch up the damage_to_inflict value
				// limit the damage to the amount required to put the victim
				// into -11 hp... in order to limit the benefits of the weapon
				damage_to_inflict=UMIN(victim->hit+11,damage_to_inflict);
				damage_to_inflict=UMAX(0,damage_to_inflict);
				// display the custom damage messages
				act("$n's bite draws life from $N.",ch,ch,victim,TO_NOTVICT);
				act_with_autodam_to_char("You feel $n's bite drawing your life away.",victim,ch,NULL,damage_to_inflict);
				act_with_autodam_to_char("Your bite draws life from $N.",ch,ch,victim,damage_to_inflict);
				// inflict the custom effect before the standard damage
				ch->hit += damage_to_inflict;
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_NEGATIVE);
			if (ch->hit >= ch->max_hit){
				ch->hit = ch->max_hit;}
			return;}
		}
	}
	if (ch->race == race_lookup("dhamphir")){
		if (ch->fighting == victim && (IS_AFFECTED2(ch, AFF2_VAMP_BITE)||
			IS_VAMPAFF(ch, VAMPYRE_FANGS))){
			dam = number_range(1, ch->level / 2);

			damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_NEGATIVE);
			if(damage_to_inflict>=0){
				// *** patch up the damage_to_inflict value
				// limit the damage to the amount required to put the victim
				// into -11 hp... in order to limit the benefits of the weapon
				damage_to_inflict=UMIN(victim->hit+11,damage_to_inflict);
				damage_to_inflict=UMAX(0,damage_to_inflict);
				// display the custom damage messages
				act("$n's bite draws life from $N.",ch,ch,victim,TO_NOTVICT);
				act_with_autodam_to_char("You feel $n's bite drawing your life away.",victim,ch,NULL,damage_to_inflict);
				act_with_autodam_to_char("Your bite draws life from $N.",ch,ch,victim,damage_to_inflict);
				// inflict the custom effect before the standard damage
				ch->hit += damage_to_inflict;
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_NEGATIVE);
			if (ch->hit >= ch->max_hit){
				ch->hit = ch->max_hit;}
			return;}
		}
	}
	if (ch->fighting == victim && (IS_AFFECTED2(ch, AFF2_VAMP_BITE)||
			IS_VAMPAFF(ch, VAMPYRE_FANGS))){
			dam = number_range(1, ch->level / 4);

			damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_NEGATIVE);
			if(damage_to_inflict>=0){
				// *** patch up the damage_to_inflict value
				// limit the damage to the amount required to put the victim
				// into -11 hp... in order to limit the benefits of the weapon
				damage_to_inflict=UMIN(victim->hit+11,damage_to_inflict);
				damage_to_inflict=UMAX(0,damage_to_inflict);
				// display the custom damage messages
				act("$n's bite draws life from $N.",ch,ch,victim,TO_NOTVICT);
				act_with_autodam_to_char("You feel $n's bite drawing your life away.",victim,ch,NULL,damage_to_inflict);
				act_with_autodam_to_char("Your bite draws life from $N.",ch,ch,victim,damage_to_inflict);
				// inflict the custom effect before the standard damage
				ch->hit += damage_to_inflict;
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_NEGATIVE);
			if (ch->hit >= ch->max_hit){
				ch->hit = ch->max_hit;}
		return;}
	}
	if (ch->fighting == victim && IS_SET( ch->parts, PART_FANGS )){
			dam = number_range(1, ch->level / 2);

			damage_to_inflict=damage_calculate_infliction(ch,victim,dam,0,DAM_PIERCE);
			if(damage_to_inflict>=0){
				// *** patch up the damage_to_inflict value
				// limit the damage to the amount required to put the victim
				// into -11 hp... in order to limit the benefits of the weapon
				damage_to_inflict=UMIN(victim->hit+11,damage_to_inflict);
				damage_to_inflict=UMAX(0,damage_to_inflict);
				//display the custom damage messages
				act("$n's fangs pierce $N.",ch,ch,victim,TO_NOTVICT);
				act_with_autodam_to_char("You feel $n's fangs pierce you.",victim,ch,NULL,damage_to_inflict);
				act_with_autodam_to_char("Your fangs pierce $N.",ch,ch,victim,damage_to_inflict);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_PIERCE);
		return;}
	}
	if (ch->fighting == NULL){
		ch->println("You'll have to get closer.");
	return;
	}
	ch->println("You have no fangs!");
	return;
}
/**************************************************************************/
void do_snipe( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	char arg1[MIL], arg2[MIL];
	char_data	*victim = NULL;
    ROOM_INDEX_DATA	*in_room, *to_room;
    	OBJ_DATA 	*gun = NULL;
	int		dam, door, chance;
    	ROOM_INDEX_DATA *was_in_room;
    	EXIT_DATA 	*pexit;
//		int dam2;
//		int damage_to_inflict;
	gun = get_eq_char( ch, WEAR_WIELD );
	argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
	if ( !gun ){
		ch->println("You must be wielding a gun.");
		return;}


	if ( gun->value[0]!=WEAPON_RIFLE &&
	     gun->value[0]!=WEAPON_BLASTRIFLE &&
		gun->value[0]!=WEAPON_PISTOL &&
		gun->value[0]!=WEAPON_BLASTPISTOL){
		ch->println("You must be wielding a gun.");
		return;}

	if ( gun->value[0] == WEAPON_RIFLE){
		chance = get_skill( ch, gsn_rifle );
		if ( chance < 1){
			do_huh(ch,"");
			return;}
	}

	if ( gun->value[0] == WEAPON_BLASTRIFLE){
		chance = get_skill( ch, gsn_blastrifle );
		if ( chance < 1){
			do_huh(ch,"");
			return;}
	}

	if ( gun->value[0] == WEAPON_BLASTPISTOL){
		chance = get_skill( ch, gsn_blastpistol );
		if ( chance < 1){
			do_huh(ch,"");
			return;}
	}

	if ( gun->value[0] == WEAPON_PISTOL){
		chance = get_skill( ch, gsn_pistol );
		if ( chance < 1){
			do_huh(ch,"");
			return;}
	}


	int dt;
	dt = gun->value[3];
	if ( dt == TYPE_UNDEFINED ){
		dt = TYPE_HIT;
		if ( gun != NULL && gun->item_type == ITEM_WEAPON ){
			dt += gun->value[3];
		}else{
			dt += ch->dam_type;}
	}

//	one_argument( argument, arg );

    if ( arg1 == '\0' )
    {
        if ( ch->fighting )
        {
            victim = ch->fighting;
        }
        else
        {
            ch->println( "Fire at whom or what?");
            return;
        }
    }

    door = dir_lookup( arg2 );

    if ( door == -1 )
	{
		ch->printlnf( "'%s' is an invalid direction.", arg2 );
		ch->println("Syntax: snipe <target> <direction>");
		return;
    }

	in_room = ch->in_room;
	to_room = NULL;

	if ( (( pexit   = in_room->exit[door] ) == NULL
	||  ( to_room = pexit->u1.to_room   ) == NULL
	||  !can_see_room( ch, pexit->u1.to_room ))
	&&  strcmp( arg2, "down" ))
	{
		ch->println("You cannot shoot in that direction.");
		return;
    }
	/* See if who you are trying to shoot at is nearby... */
        if ( ( victim = get_char_room ( ch, arg1 ) ) == NULL)
        {
            was_in_room = ch->in_room;

/*         for( door=0 ; door < MAX_DIR && victim==NULL ; door++ )
             { */
                if ( (  pexit = was_in_room->exit[door] ) != NULL
                   &&   pexit->u1.to_room != NULL
                   &&   pexit->u1.to_room != was_in_room
                   &&   !strcmp( pexit->u1.to_room->area->name ,
                                was_in_room->area->name ) )
                   {
                     ch->in_room = pexit->u1.to_room;
                     victim = get_char_room ( ch, arg1 );
                    }

//           }

            ch->in_room = was_in_room;

            if(victim==NULL)
              {
               ch->println( "You can't seem to locate your target.");
               return;
              }
            else
              {  if(IS_SET(pexit->exit_info,EX_CLOSED))
                    { ch->println("You can't fire through a door.");
                      return;
                     }
             }
        }
   	if((ch->in_room) == (victim->in_room))
    	{
		ch->println("Don't you think that standing a bit further away would be wise?");
		return;
    	}

 	if(ch == victim)
 	{
     	   ch->println("Yeah, right.");
      	   return;
    	}

	if ( is_safe( ch, victim ))
	{
     	   	ch->println("You cannot seem to target them for some reason.");
		return;
	}

	if(IS_NPC(victim) && (IS_SET(victim->act, ACT_NO_HUNT) ||
	   IS_SET(victim->act, ACT_DONT_WANDER)) )
	{
     	   	ch->println("You cannot seem to target them for some reason.");
		return;
	}

    	if ( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim )
	{
		act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
		return;

    }


    /* Lag the sniper... */
	if (!IS_IMMORTAL(ch)){
    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
	}
     /* Fire the weapon */

	/* Fire the weapon with unlimitted ammo */


	act( "$n shoots $p at $N.",  ch, gun, victim, TO_NOTVICT );
	act( "You shoot $p at $N.",  ch, gun, victim, TO_CHAR );

	int sn;
	sn = -1;
	sn = get_weapon_sn(ch);

	{

		chance = number_range(1,10);
		switch (chance)
		{
			case 1:

				break;
			case 2:

				break;
			case 3:

				break;
			case 4:

				break;
			case 5:
				act( "$n shoots you with $p .", ch, gun, victim, TO_VICT );


				dam = dice( gun->value[1], gun->value[2] );
				// If weapon is blessed & enemy is undead
				if( IS_SET(gun->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD )){
				dam = dam * 3 / 2;}
				if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(gun,WEAPON_HOLY)
				&& IS_OBJ_STAT(gun,OBJEXTRA_ANTI_GOOD)){
				dam = dam + (victim->alliance/3)* (dam*5/4);}
				if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(gun,WEAPON_HOLY)
				&& IS_OBJ_STAT(gun,OBJEXTRA_ANTI_EVIL) ){
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);}

				if ( damage( ch, victim, dam, sn, dt, true )){
				check_improve( ch, sn, true, 1 );}
				break;
			case 6:
			case 7:
			case 8:
				act( "$n shoots you with $p .", ch, gun, victim, TO_VICT );

				dam = 3 * ( dice( gun->value[1], gun->value[2] ));
				// If weapon is blessed & enemy is undead
				if( IS_SET(gun->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD )){
				dam = dam * 3 / 2;}
				if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(gun,WEAPON_HOLY)
				&& IS_OBJ_STAT(gun,OBJEXTRA_ANTI_GOOD)){
				dam = dam + (victim->alliance/3)* (dam*5/4);}
				if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(gun,WEAPON_HOLY)
				&& IS_OBJ_STAT(gun,OBJEXTRA_ANTI_EVIL) ){
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);}

				if ( damage( ch, victim, dam, sn, dt, true )){
				check_improve( ch, sn, true, 1 );}

				break;
			case 9:
			case 10:
				act( "$n shoots you with $p .", ch, gun, victim, TO_VICT );

				dam = 2 * ( dice( gun->value[1], gun->value[2] ));

				// If weapon is blessed & enemy is undead
				if( IS_SET(gun->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD )){
				dam = dam * 3 / 2;}

				if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(gun,WEAPON_HOLY)
				&& IS_OBJ_STAT(gun,OBJEXTRA_ANTI_GOOD)){
				dam = dam + (victim->alliance/3)* (dam*5/4);}
				if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(gun,WEAPON_HOLY)
				&& IS_OBJ_STAT(gun,OBJEXTRA_ANTI_EVIL) ){
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);}

				if ( damage( ch, victim, dam, sn, dt, true )){
				check_improve( ch, sn, true, 1 );}

				break;
		}


	return;
    }

}
/**************************************************************************/
/**************************************************************************/
/*
 * Control the magic shit going on.
 * Called periodically by update_handler.
 */
void magic_update( void )
{
    char_data *ch;
    char_data *ch_next;
//    char_data *victim;

    for ( ch = char_list; ch; ch = ch_next )
    {
		ch_next = ch->next;

	if (!ch->elements && !IS_NPC(ch)){
	int rndelement;
	rndelement = number_range(1,56);
	switch(rndelement) {
		default:
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			ch->elements = ELEMENT_AIR;
			ch->pcdata->elements = ELEMENT_AIR;
			break;
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			ch->elements = ELEMENT_WATER;
			ch->pcdata->elements = ELEMENT_WATER;
			break;
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			ch->elements = ELEMENT_EARTH;
			ch->pcdata->elements = ELEMENT_EARTH;
			break;
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
			ch->elements = ELEMENT_FIRE;
			ch->pcdata->elements = ELEMENT_FIRE;
			break;
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
			ch->elements = ELEMENT_LIGHT;
			ch->pcdata->elements = ELEMENT_LIGHT;
			break;
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
			ch->elements = ELEMENT_DARK;
			ch->pcdata->elements = ELEMENT_DARK;
			break;
		case 31:
		case 32:
		case 33:
		case 34:
			ch->elements = ELEMENT_PLANT;
			ch->pcdata->elements = ELEMENT_PLANT;
			break;
		case 35:
		case 36:
		case 37:
		case 38:
			ch->elements = ELEMENT_ANIMAL;
			ch->pcdata->elements = ELEMENT_ANIMAL;
			break;
		case 39:
		case 40:
		case 41:
		case 42:
			ch->elements = ELEMENT_LAND;
			ch->pcdata->elements = ELEMENT_LAND;
			break;
		case 43:
		case 44:
		case 45:
			ch->elements = ELEMENT_ICE;
			ch->pcdata->elements = ELEMENT_ICE;
			break;
		case 46:
		case 47:
		case 48:
			ch->elements = ELEMENT_LIGHTNING;
			ch->pcdata->elements = ELEMENT_LIGHTNING;
			break;
		case 49:
		case 50:
			ch->elements = ELEMENT_CELESTIAL;
			ch->pcdata->elements = ELEMENT_CELESTIAL;
			break;
		case 51:
		case 52:
			ch->elements = ELEMENT_MOON;
			ch->pcdata->elements = ELEMENT_MOON;
			break;
		case 53:
		case 54:
			ch->elements = ELEMENT_SUN;
			ch->pcdata->elements = ELEMENT_SUN;
			break;
		case 55:
		case 56:
			ch->elements = ELEMENT_DEATH;
			ch->pcdata->elements = ELEMENT_DEATH;
			break;
		case 57:
		case 58:
			ch->elements = ELEMENT_LIFE;
			ch->pcdata->elements = ELEMENT_LIFE;
			break;
		case 59:
			ch->elements = ELEMENT_TIME;
			ch->pcdata->elements = ELEMENT_TIME;
			break;
		case 60:
			ch->elements = ELEMENT_GRAVITY;
			ch->pcdata->elements = ELEMENT_GRAVITY;
			break;
		}
	}

if (ch->casttimer1 >= 1){

      if (ch->casttimer1 == 1){
         --ch->casttimer1;
                if (!IS_NPC(ch)){
                  ch->printlnf("You finish casting '%s'!",skill_table[ch->casting1].name);
		}
         do_newcast( ch, ch->casting1name, 0, ch->castingtarget1);
//                do_speedcast2( ch, ch->casting1name, 0, CCT_MAGE);
         ch->casting1 = -1;
         ch->casting1name = NULL;
         ch->castingtarget1 = NULL;
                if (!IS_NULLSTR(ch->casting2name) && ch->casting2name!=NULL){
                	if (!IS_NPC(ch)){
	                	ch->printlnf("You resume casting '%s'!",skill_table[ch->casting2].name);
			}
		        ch->casting1 = ch->casting2;
        		ch->casting2 = -1;
        		ch->casting1name = ch->casting2name;
        		ch->casting2name = NULL;
        		ch->casttimer1 = ch->casttimer2 +=2;
        		ch->casttimer2 = 0;
        		ch->castingtarget1 = ch->castingtarget2;
        		ch->castingtarget2 = NULL;
		}
	}
        --ch->casttimer1;
        if (!IS_NPC(ch)){
            if (ch->casttimer1 == 4
                || ch->casttimer1 ==6
                || ch->casttimer1 ==8
                || ch->casttimer1 ==10
                || ch->casttimer1 ==12){
                ch->printlnf("You continue to focus your power on '%s'..",skill_table[ch->casting1].name);
            }
            if (ch->casttimer1 == 2){
                ch->println("Your spell is nearly complete..");
            }
        }
}

    }

	return;
}
/**************************************************************************/
//TBC
/*
 * Updater for what few autonomous functions used in this combat system.
 * Mostly just triggers mobile's turns and updates battlefield conditions.
 * Also controls the automatic turn skip when someone tries to stall.
 */
void combat_update (void)
{
	COMBAT_DATA * battle, * battle_next;
	char_data * ch;

	for (battle = battle_list; battle != NULL; battle = battle_next)
	{
		battle_next = battle->next;

		if (battle->turn_list->roundtime >= 2000000000)
			nuke_roundtime (battle);

		ch = battle->turn_list->unit;

		if (ch == NULL)
		{
			bug ("combat_update (): Turn with NULL character!");
			clean_combat (battle);
		}
		else if (IS_NPC (ch))
			mobile_turn (ch);
		else if (ch->desc == NULL)
			linkdead_turn (ch);
		else
		{
			battle->turn_list->timer++;

			if (battle->turn_list->timer == 70)
				turn_end (battle->turn_list, 300);
		}
	}
}
/****************************************************************************/
/*
 * Begins a fight!
 */
void do_fight (char_data * ch, char * argument)
{
	char_data * vch;

	if (is_tbcfighting (ch))
	{
		ch->println ("You're already fighting!\n\r");
		return;
	}

	if (argument[0] == '\0')
	{
		ch->println ("Fight whom?\n\r");
		return;
	}
	if ((vch = get_char_room (ch, argument)) == NULL)
	{
		ch->println ("They aren't here!\n\r");
		return;
	}
	if (vch == ch)
	{
		ch->println ("You won!\n\r");
		return;
	}

	//Might want to add kill steal/illegal player kill protection to this.
	if (is_tbcfighting (vch))
	{
		char_to_combat (ch, vch->in_battle);
		act ("You rush headlong into the fray!", ch, NULL, vch, TO_CHAR);
		act ("$n rushes headlong into the fray!", ch, NULL, vch, TO_ROOM);
		return;
	}

	initiate_combat (ch, vch);
	act ("You initiate combat against $N.", ch, NULL, vch, TO_CHAR);
	act ("$n initiates combat against you.", ch, NULL, vch, TO_VICT);
	act ("$n initiates combat against $N.", ch, NULL, vch, TO_NOTVICT);
	return;
}


/*
 * This is a simple replacement to the original damage, here solely to make
 * the snippet run clean in stockier MUDs.
 */
bool damage_new (char_data * ch, char_data * vch, long dam, int sn, char * damverb,
				 bool hide, bool fKill)
{
	/* Make sure the damage is valid. */
	if (ch == NULL || vch == NULL)
	{
		bug ("damage_new(): missing character");
		return false;
	}

	/* Apply damage caps. */
	if (!fKill)
		dam = UMIN (dam, vch->hit - 1);

	dam = UMIN (dam, 1200);

	/* If you add damtype(s), this'd be where to check them. */
    int dt = TYPE_UNDEFINED;
   	int victim_ac;
	int thac0;
	int thac0_00;
	int thac0_32;
    int dam_type;
    int skill=0;
    int diceroll=0;
    char_data * victim=vch;
    char *attack="attack";
    OBJ_DATA *primary_weapon= get_eq_char( ch, WEAR_WIELD );
    if ( dt == TYPE_UNDEFINED )
	{
		dt = TYPE_HIT;
		if ( primary_weapon != NULL && primary_weapon->item_type == ITEM_WEAPON ){
			dt += primary_weapon->value[3];
			attack = attack_table[primary_weapon->value[3]].noun;
			damverb= attack;
		}else{
			dt += ch->dam_type;
		}
	}

    if (dt < TYPE_HIT){
		if (primary_weapon != NULL){
			dam_type = attack_table[primary_weapon->value[3]].damage;
		}else{
			dam_type = attack_table[ch->dam_type].damage;
		}
	}else{
		dam_type = attack_table[dt - TYPE_HIT].damage;
	}

	if (dam_type == -1){
		dam_type = DAM_BASH;
	}

	// get the weapon skill
	/*if(second){ // swap over the secondary and primary to get sn
		if(primary_weapon){
			primary_weapon->wear_loc = WEAR_SECONDARY;
		}
		secondary_weapon->wear_loc = WEAR_WIELD;
		sn = get_weapon_sn(ch);
		if(primary_weapon){
			primary_weapon->wear_loc = WEAR_WIELD;
		}
		secondary_weapon->wear_loc = WEAR_SECONDARY;
	}else{*/
		sn = get_weapon_sn(ch);
//	}
	if( ch->clss == class_lookup("citizen")){
		skill = 65;
	}else{
		skill = 20 + get_weapon_skill(ch,sn);
	}

	// bonus skill percentage for those with relevant mastery
	int arms_mastery_bonus=get_skill( ch, gsn_arms_mastery )/ 5;
	int mastery_bonus=0;
	if(sn == gsn_sword
		&& get_skill( ch, gsn_sword_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_sword_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_dagger
		&& get_skill( ch, gsn_dagger_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_dagger_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_flail
		&& get_skill( ch, gsn_flail_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_flail_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_mace
		&& get_skill( ch, gsn_mace_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_mace_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_polearm
		&& get_skill( ch, gsn_polearm_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_polearm_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_staff
		&& get_skill( ch, gsn_staff_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_staff_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_whip
		&& get_skill( ch, gsn_whip_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_whip_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_spear
		&& get_skill( ch, gsn_spear_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_spear_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_sickle
		&& get_skill( ch, gsn_sickle_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_sickle_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_lightblade
		&& get_skill( ch, gsn_lightblade_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_lightblade_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_bow
		&& get_skill( ch, gsn_bow_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_bow_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_crossbow
		&& get_skill( ch, gsn_crossbow_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_crossbow_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_unarmed
		&& get_skill( ch, gsn_unarmed_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_unarmed_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_pistol
		&& get_skill( ch, gsn_pistol_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_pistol_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_rifle
		&& get_skill( ch, gsn_rifle_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_rifle_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_blastpistol
		&& get_skill( ch, gsn_blastpistol_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_blastpistol_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}else if(sn == gsn_blastrifle
		&& get_skill( ch, gsn_blastrifle_mastery ) >= 5 ){
		mastery_bonus += get_skill( ch, gsn_blastrifle_mastery)/ 5;
		if (mastery_bonus <= 0){
		mastery_bonus = 0;}
		skill += mastery_bonus;
	}
	skill+=arms_mastery_bonus;
/*
	if(second){
		if(!GAMESETTING2(GAMESET2_NO_SECOND_SKILL_REQUIRED)){
			skill*=(ch->get_skill(gsn_second)/100);
		}
	}
*/
	if ( get_eq_char( ch, WEAR_LODGED_ARM )){
		skill -= 20;
	}

	if(HAS_CONFIG(ch,CONFIG_PRACSYS_TESTER)){
		skill+=20;
	}

	/*Calculate to-hit-armor-clss-0 versus armor.*/
	if ( IS_NPC(ch) )
	{
		thac0_00 = 20;
		thac0_32 = -4;   // as good as a thief
		if (IS_SET(ch->act,ACT_WARRIOR))
			thac0_32 = -10;
		else if (IS_SET(ch->act,ACT_THIEF))
			thac0_32 = -4;
		else if (IS_SET(ch->act,ACT_CLERIC))
			thac0_32 = 2;
		else if (IS_SET(ch->act,ACT_MAGE))
			thac0_32 = 6;
	}
	else
	{
		thac0_00 = class_table[ch->clss].thac0_00;
		thac0_32 = class_table[ch->clss].thac0_32;
	}
	thac0  = interpolate( ch->level, thac0_00, thac0_32 );

	if (thac0 < 0)
		thac0 = thac0/2;

	if (thac0 < -5)
		thac0 = -5 + (thac0 + 5) / 2;

    int hroll= GET_HITROLL(ch);
    if (ch->symbiotetype != 0){
        hroll += ch->symbiotehr;}

	thac0 -= hroll * skill/100;
//	thac0 -= GET_HITROLL(ch) * skill/100;
	if(!IS_NPC(ch)){
		// increase affect to hitroll for low pc newbies
		if(ch->level<6){
			thac0-= (6-ch->level)*2;
		}
	}else{
		// decrease hitroll on low mobs when fighting newbies
		if(ch->level<6 && victim->level<6){
			thac0+= (6-victim->level)*2;
		}
	}

	// make faerie fire do something
	if(count_affected_by_base_spell( victim, gsn_faerie_fire )>0){
		thac0-=number_range(1,3);
	}

	// make the likelyhood of impact less if they are lacking in skill
	thac0 += 5 * (100 - skill) / 100;

	switch(dam_type)
	{
	case(DAM_PIERCE):victim_ac = GET_AC(victim,AC_PIERCE)/10;       break;
	case(DAM_BASH):  victim_ac = GET_AC(victim,AC_BASH)/10;         break;
	case(DAM_SLASH): victim_ac = GET_AC(victim,AC_SLASH)/10;        break;
	default:         victim_ac = GET_AC(victim,AC_EXOTIC)/10;       break;
	};

	if (victim_ac < -15)
		victim_ac = (victim_ac + 15) / 5 - 15;

	if(victim->position==POS_FIGHTING)
		victim_ac -= victim->modifiers[STAT_QU]/5;

	if ( !can_see( ch, victim ) )
		victim_ac -= 4;

	if ( victim->position < POS_FIGHTING)
		victim_ac += 4;

	if (victim->position < POS_RESTING)
		victim_ac += 6;

    if (victim->symbiotetype != 0)
        victim_ac -= ch->symbioteac;



    if ( IS_NPC(ch) && primary_weapon == NULL)
	{
		dam = dice(ch->damage[DICE_NUMBER],ch->damage[DICE_TYPE]);
	}
	else
	{
		if (sn != -1){
			check_improve(ch,sn,true,5);
		}

	if( get_skill( ch, gsn_arms_mastery ) )
		check_improve(ch,gsn_arms_mastery,true,3);
	else if( get_skill(ch, gsn_sword_mastery )
		&& primary_weapon->value[0] == WEAPON_SWORD )
		check_improve(ch,gsn_sword_mastery,true,3);
	else if( get_skill(ch, gsn_dagger_mastery )
		&& primary_weapon->value[0] == WEAPON_DAGGER )
		check_improve(ch,gsn_dagger_mastery,true,3);
	else if( get_skill(ch, gsn_flail_mastery )
		&& primary_weapon->value[0] == WEAPON_FLAIL )
		check_improve(ch,gsn_flail_mastery,true,3);
	else if( get_skill(ch, gsn_mace_mastery )
		&& primary_weapon->value[0] == WEAPON_MACE )
		check_improve(ch,gsn_mace_mastery,true,3);
	else if( get_skill(ch, gsn_polearm_mastery )
		&& primary_weapon->value[0] == WEAPON_POLEARM )
		check_improve(ch,gsn_polearm_mastery,true,3);
	else if( get_skill(ch, gsn_staff_mastery )
		&& primary_weapon->value[0] == WEAPON_STAFF )
		check_improve(ch,gsn_staff_mastery,true,3);
	else if( get_skill(ch, gsn_whip_mastery )
		&& primary_weapon->value[0] == WEAPON_WHIP )
		check_improve(ch,gsn_whip_mastery,true,3);
	else if( get_skill(ch, gsn_spear_mastery )
		&& primary_weapon->value[0] == WEAPON_SPEAR )
		check_improve(ch,gsn_spear_mastery,true,3);
	else if( get_skill(ch, gsn_sickle_mastery )
		&& primary_weapon->value[0] == WEAPON_SICKLE )
		check_improve(ch,gsn_sickle_mastery,true,3);
	else if( get_skill(ch, gsn_lightblade_mastery )
		&& primary_weapon->value[0] == WEAPON_LIGHTBLADE )
		check_improve(ch,gsn_lightblade_mastery,true,3);
	else if( get_skill(ch, gsn_crossbow_mastery )
		&& primary_weapon->value[0] == WEAPON_CROSSBOW )
		check_improve(ch,gsn_crossbow_mastery,true,3);
	else if( get_skill(ch, gsn_pistol_mastery )
		&& primary_weapon->value[0] == WEAPON_PISTOL )
		check_improve(ch,gsn_pistol_mastery,true,3);
	else if( get_skill(ch, gsn_rifle_mastery )
		&& primary_weapon->value[0] == WEAPON_RIFLE )
		check_improve(ch,gsn_rifle_mastery,true,3);
	else if( get_skill(ch, gsn_blastpistol_mastery )
		&& primary_weapon->value[0] == WEAPON_BLASTPISTOL )
		check_improve(ch,gsn_blastpistol_mastery,true,3);
	else if( get_skill(ch, gsn_blastrifle_mastery )
		&& primary_weapon->value[0] == WEAPON_BLASTRIFLE )
		check_improve(ch,gsn_blastrifle_mastery,true,3);
	else if( get_skill(ch, gsn_bow_mastery )
		&& primary_weapon->value[0] == WEAPON_LONG_BOW )
		check_improve(ch,gsn_bow_mastery,true,3);
	else if( get_skill(ch, gsn_bow_mastery )
		&& primary_weapon->value[0] == WEAPON_SHORT_BOW )
		check_improve(ch,gsn_bow_mastery,true,3);
	else if( get_skill(ch, gsn_unarmed_mastery )
		&& primary_weapon->value[0] == WEAPON_CESTUS )
		check_improve(ch,gsn_unarmed_mastery,true,3);
	else if( get_skill(ch, gsn_unarmed_mastery )
		&& primary_weapon->value[0] == WEAPON_SIANGHAM )
		check_improve(ch,gsn_unarmed_mastery,true,3);
	else if( get_skill(ch, gsn_unarmed_mastery )
		&& primary_weapon->value[0] == WEAPON_KAMA )
		check_improve(ch,gsn_unarmed_mastery,true,3);

		if ( primary_weapon != NULL )
		{
			dam = dice(primary_weapon->value[1] ,primary_weapon->value[2]) * skill/100;
			// Send MSP sound message
			{
				char weapon_sound[MIL];
				sprintf(weapon_sound,"%s6.wav", get_weapontype( primary_weapon ));
				msp_to_room(MSPT_COMBAT, weapon_sound,
					0,
					ch,
					false,
					true);
			}

			// if weapon is above their level reduce the damage accordingly
			if (primary_weapon->level>ch->level)
			{
				dam=dam * ch->level / primary_weapon->level;
			}

			// if defending a mount get bunch of extra damage
			if (victim->fighting==ch->mounted_on)
			{
				dam=dam * 11/7;
			}

			// If weapon is blessed & enemy is undead/ghost/etc
			if( IS_SET(primary_weapon->extra_flags, OBJEXTRA_BLESS)
				&& IS_SET( victim->form, FORM_UNDEAD ))
			{
				dam = dam * 3 / 2;
			}


			// add damage bonus/penalty for mountedness:
			// - if riding <80 you get from 1 to 100 percent of original damage
			// - if riding >=80 you get 100 to 120 percent
			// - based on riding skill and random factors
			// - if defending your mount, these bonuses/penalties do not apply
			if ((ch->mounted_on!=NULL)&&(!IS_NPC(ch))
				&&(victim->fighting!=ch->mounted_on))
			{
				if (get_skill(ch, gsn_riding)<80)
				{
					dam *=(((get_skill(ch, gsn_riding)+20) * number_range(10,100))
						/10000);
				}
				else
				{
					dam+=dam*((number_range(1,100))*(get_skill(ch, gsn_riding)-80))/10000;
					check_improve(ch, gsn_riding, true, 10);
				}
			}

			// no shield or second weapon = more damage
			if (!get_eq_char(ch, WEAR_SECONDARY)
				&& !get_eq_char(ch,WEAR_SHIELD))
				dam = dam * 6/5;

			// sharpness!
			if (IS_WEAPON_STAT(primary_weapon,WEAPON_SHARP))
			{
				int percent;

				if ((percent = number_percent()) <= (skill / 8))
					dam = 2 * dam + (dam * 2 * percent / 100);
			}

			if (IS_WEAPON_STAT(primary_weapon,WEAPON_VORPAL))
			{
				int percent;
				if((percent=number_percent()) <=(skill/33))
					dam = 6 * dam + (dam*4*percent/100);
			}

			if ( IS_GOOD(victim)
				&& IS_WEAPON_STAT(primary_weapon,WEAPON_HOLY)
				&& IS_OBJ_STAT(primary_weapon,OBJEXTRA_ANTI_GOOD))
			/*	&& HAS_CLASSFLAG(ch, CLASSFLAG_DAMMODS_WITH_HOLYWEAPONS))*/
			{
				dam = dam + (victim->alliance/3)* (dam*5/4);
			}

			if ( IS_EVIL(victim)
				&& IS_WEAPON_STAT(primary_weapon,WEAPON_HOLY)
				&& IS_OBJ_STAT(primary_weapon,OBJEXTRA_ANTI_EVIL) )
			/*	&& HAS_CLASSFLAG(ch, CLASSFLAG_DAMMODS_WITH_HOLYWEAPONS))*/
			{
				dam = (sh_int) dam + (abs(victim->alliance)/3)*(dam*5/4);
			}

		}else{
			if( get_skill(ch, gsn_unarmed_mastery )){
				check_improve(ch,gsn_unarmed_mastery,true,3);}
			dam = number_range( 1 + 4 * skill/100, 2 * ch->level/3 * skill/100);
		}
	}

	// Bonuses
	if ( get_skill(ch,gsn_enhanced_damage) > 0 )
	{
		diceroll = number_percent();
		if (diceroll <= get_skill(ch,gsn_enhanced_damage))
		{
			check_improve(ch,gsn_enhanced_damage,true,6);
			dam += 2 * ( dam * diceroll/300);
		}
	}

    if (IS_AFFECTED2(ch, AFF2_POSSESSION))
    {
        diceroll = number_percent();
		if (diceroll <= 50){
			dam += 2 * (dam * diceroll / 300);
		}
    }

	if ( get_skill(ch,gsn_ultra_damage) > 0 )
	{
		diceroll = number_percent();
		if (diceroll <= get_skill(ch,gsn_ultra_damage))
		{
			check_improve(ch,gsn_ultra_damage,true,7);
			dam += 3/2 * ( dam * diceroll/300);
		}
	}

	if ( !IS_AWAKE(victim) ){
		dam *= 2;
	}else{
		if (victim->position < POS_FIGHTING){
			dam = dam * 3 / 2;
		}
	}

	if ( dt == gsn_backstab && primary_weapon != NULL){
		if ( primary_weapon->value[0] != 2 ){
			dam *= 2 + (ch->level / 10);
		}else {
			dam *= 2 + (ch->level / 8);
		}
	}

    if ( dt == gsn_circle && primary_weapon != NULL){
		if ( primary_weapon->value[0] != 2 ){
			dam *= 2 + (ch->level / 16);
		}else{
			dam *= 2 + (ch->level / 11);
		}
	}

    int droll=GET_DAMROLL(ch);
    if (ch->symbiotetype != 0){
        droll += ch->symbiotedr;}


    dam += droll * UMIN(100,skill) /100;

//	dam += GET_DAMROLL(ch) * UMIN(100,skill) /100;

	if ( dam <= 0 )
		dam = 1;




	// The moment of excitement!
	diceroll=number_range(0,19);

/*	{
	// some debugging code to get an understanding of what is actually happening
		int val=thac0 - victim_ac + num_enemies(ch) - num_enemies(victim);
ch->printlnf    ("ATTACKER=== thac0=%d, victim_ac = %d, num_enemies(ch)=%d, num_enemies(victim)=%d, dice=%d, val=%d",
			 thac0, victim_ac, num_enemies(ch), num_enemies(victim), diceroll, val);
victim->printlnf("VICTIM  === thac0=%d, victim_ac = %d, num_enemies(ch)=%d, num_enemies(victim)=%d, dice=%d, val=%d",
			 thac0, victim_ac, num_enemies(ch), num_enemies(victim), diceroll, val);
	}
*/
	if(diceroll){
		// the above check guarantees a hit (1 in 20) - when diceroll==0
		if ( diceroll == 19 // guarantee miss (1 in 20)
			||
			diceroll < thac0 - victim_ac + num_enemies(ch) - num_enemies(victim))
		{
			// Miss
			dam=0;
			tail_chain( );
        }
	}

	/*result = */damage( ch, victim, dam, dt, dam_type, true );
    if (dam<+1){
    diceroll=number_range(0,19);
    if ( diceroll == 19 )
    {
    ch->println("`r!!!CRITICAL STRIKE!!!`x");
	dam = dam*2;
    }
    }
	/* Dish out the damage. */
	vch->hit = UMAX (0, vch->hit - dam);

	/* Grab an sn-based damverb if one isn't present. */
	if (damverb[0] == '\0')
	{
        if (sn >= 0 && sn < MAX_SKILL && skill_table[sn].noun_damage != NULL)
            damverb = skill_table[sn].noun_damage;
		else
			damverb = "attack";
	}

	/* Dispatch messaging, if hide is false. */
	if (!hide)
	{
	//	char buf[200];

		if (dam > 0)
		{
/*			sprintf (buf, "`CYour $t hits $N for %ld damage!`x\n\r", dam);
			act (buf, ch, damverb, vch, TO_CHAR);

			sprintf (buf, "`R$n's $t hits you for %ld damage!`x\n\r", dam);
			act (buf, ch, damverb, vch, TO_VICT);

			sprintf (buf, "`D$n's $t hits $N for %ld damage!`x\n\r", dam);
			act (buf, ch, damverb, vch, TO_NOTVICT);*/
		}
		else
		{
			act ("`CYour $t misses $N!`x\n\r", ch, damverb, vch, TO_CHAR);
			act ("`R$n's $t misses you!`x\n\r", ch, damverb, vch, TO_VICT);
			act ("`D$n's $t misses $N!`x\n\r", ch, damverb, vch, TO_NOTVICT);
		}
	}
	// funky weapon?
	if ((dam>=1) && primary_weapon)
	{
		int edam=0;
		int damage_to_inflict; // damage result

		if (/*ch->fighting == victim*/(vch->hit>=1) && IS_WEAPON_STAT(primary_weapon,WEAPON_POISON))
		{
			int elevel;
			AFFECT_DATA *poison, af;

			poison = affect_find(primary_weapon->affected,gsn_poison);
			if(poison){
				elevel = poison->level;
			}else{
				elevel = primary_weapon->level;
			}

			if (!saves_spell(elevel / 2,victim,DAM_POISON)
				&& !HAS_CLASSFLAG(victim, CLASSFLAG_POISON_IMMUNITY))
			{
				victim->println("You feel poison coursing through your veins.");
				act("$n is poisoned by the venom on $p.",
					victim,primary_weapon,NULL,TO_ROOM);

				af.where     = WHERE_AFFECTS;
				af.type      = gsn_poison;
				af.level     = elevel * 3/4;
				af.duration  = elevel / 2;
				af.location  = APPLY_ST;
				af.modifier  = -1;
				af.bitvector = AFF_POISON;
				affect_join( victim, &af );
			}

			// weaken the poison if it's temporary
			if(poison){
				poison->level = UMAX(0,poison->level - 2);
				poison->duration = UMAX(0,poison->duration - 1);

				if (IS_ALIVE(victim) && (poison->level == 0 || poison->duration == 0)){
					act("The poison on $p has worn off.",ch,primary_weapon,NULL,TO_CHAR);
				}
			}
		}

		// vampiric weapon effect *
		if (/*ch->fighting == victim*/(vch->hit>=1) && IS_WEAPON_STAT(primary_weapon,WEAPON_VAMPIRIC))
		{
			edam = number_range(1, primary_weapon->level / 10 + 1);

			damage_to_inflict=damage_calculate_infliction(ch,victim,edam,0,DAM_NEGATIVE);
			if(damage_to_inflict>=1){
				// *** patch up the damage_to_inflict value
				// limit the damage to the amount required to put the victim
				// into -11 hp... in order to limit the benefits of the weapon
				damage_to_inflict=UMIN(victim->hit+11,damage_to_inflict);
				damage_to_inflict=UMAX(0,damage_to_inflict);
				// display the custom damage messages
				act("$p draws life from $N.",ch,primary_weapon,victim,TO_NOTVICT);
				act_with_autodam_to_char("You feel $p drawing your life away.",victim,primary_weapon,NULL,damage_to_inflict);
				act_with_autodam_to_char("$p draws life from $N.",ch,primary_weapon,victim,damage_to_inflict/2);
				// inflict the custom effect before the standard damage
				ch->hit += damage_to_inflict/2;
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_NEGATIVE);
			}
		}

		// flaming weapon effect *
		if (/*ch->fighting == victim*/(vch->hit>=1) && IS_WEAPON_STAT(primary_weapon,WEAPON_FLAMING))
		{
			edam = number_range(1,primary_weapon->level / 4 + 1);

			damage_to_inflict=damage_calculate_infliction(ch,victim,edam,0,DAM_FIRE);
			if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is schorched by $p.",ch,primary_weapon,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is scorched by $p.",ch,primary_weapon,victim,damage_to_inflict);
				act_with_autodam_to_char("$p sears your flesh.",victim,primary_weapon,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				fire_effect( (void *) victim,primary_weapon->level/2,edam,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_FIRE);
			}
		}

		// acidic weapon effect *
		if (/*ch->fighting == victim*/(vch->hit>=1) && IS_WEAPON_STAT(primary_weapon,WEAPON_ACIDIC))
		{
			edam = number_range(1,primary_weapon->level / 4 + 1);

			damage_to_inflict=damage_calculate_infliction(ch,victim,edam,0,DAM_ACID);
			if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is burned by $p's acid.",ch,primary_weapon,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is burned by $p's acid.",ch,primary_weapon,victim,damage_to_inflict);
				act_with_autodam_to_char("$p melts your flesh.",victim,primary_weapon,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				acid_effect( (void *) victim,primary_weapon->level/2,edam,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_ACID);
			}
		}

		// frost weapon effect *
		if (/*ch->fighting == victim*/(vch->hit>=1) && IS_WEAPON_STAT(primary_weapon,WEAPON_FROST))
		{
			edam = number_range(1,primary_weapon->level / 6 + 2);

			damage_to_inflict=damage_calculate_infliction(ch,victim,edam,0,DAM_COLD);
			if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$p freezes $N.",ch,primary_weapon,victim,TO_NOTVICT);
				act_with_autodam_to_char("$p freezes $N.",ch,primary_weapon,victim,damage_to_inflict);
				act_with_autodam_to_char("The cold touch of $p surrounds you with ice.",
					victim,primary_weapon,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				cold_effect(victim,primary_weapon->level/2,edam,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict,0,DAM_COLD);
			}
		}

		// suckle weapon effect - takes mana
		if (/*ch->fighting == victim*/(vch->hit>=1) && IS_WEAPON_STAT(primary_weapon,WEAPON_SUCKLE))
        {
            edam = number_range(1, primary_weapon->level / 5 + 1);
			if(edam>=1){
            act("$p draws mystical energy from $n.",victim,primary_weapon,NULL,TO_ROOM);
            act("You feel $p drawing your mystical energy away.",
                victim,primary_weapon,NULL,TO_CHAR);
            victim->mana -= edam;
            ch->mana += edam/2;
			}
        }

		// enervated weapon effect - takes movements
        if (/*ch->fighting == victim*/(vch->hit>=1) && IS_WEAPON_STAT(primary_weapon,WEAPON_ENERVATE))
        {
			if (!IS_SET(victim->form, FORM_UNDEAD )){
		    edam = number_range(1, primary_weapon->level / 5 + 1);
            if(dam>=1){
			act("$p saps the strength from $n.",victim,primary_weapon,NULL,TO_ROOM);
            act("You feel $p sapping your strength away.",
                victim,primary_weapon,NULL,TO_CHAR);
            victim->move-= edam;
            ch->move += edam/2;
			}
			}
        }

		// annealed weapon effect *
		if (/*ch->fighting == victim*/(vch->hit>=1) && IS_WEAPON_STAT(primary_weapon,WEAPON_ANNEALED))
		{
			edam = number_range(1,primary_weapon->level/5 + 2);
			if (!saves_spell(primary_weapon->level/4 + edam/20,victim,DAM_BASH)){
				damage_to_inflict=damage_calculate_infliction(ch,victim,edam,0,DAM_BASH);
				if(damage_to_inflict>=1){
					// display the custom damage messages
					victim->printlnf("You see stars as your head `Crings`x!%s",autodamtext(victim, damage_to_inflict));
					ch->printlnf("Your opponent looks `Bstunned`x!%s",autodamtext(ch, damage_to_inflict));
					// inflict the custom effect before the standard damage
					DAZE_STATE(victim,UMAX(12,primary_weapon->level/4 + edam/20));
					// inflict the actual damage
					damage_inflict(ch, victim, damage_to_inflict, 0, DAM_BASH);
				}
			}
		}

		// shocking weapon effect *
		if (/*ch->fighting == victim*/(vch->hit>=1) && IS_WEAPON_STAT(primary_weapon,WEAPON_SHOCKING))
		{
			edam = number_range(1,primary_weapon->level/5 + 2);
			damage_to_inflict=damage_calculate_infliction(ch,victim,edam,0,DAM_LIGHTNING);
			if(damage_to_inflict>=1){
				// display the custom damage messages
				act("$N is struck by lightning from $p.",ch,primary_weapon,victim,TO_NOTVICT);
				act_with_autodam_to_char("$N is struck by lightning from $p.",ch,primary_weapon,victim,damage_to_inflict);
				act_with_autodam_to_char("You are shocked by $p",victim,primary_weapon,NULL,damage_to_inflict);
				// inflict the custom effect before the standard damage
				shock_effect(victim,primary_weapon->level/2,edam,TARGET_CHAR);
				// inflict the actual damage
				damage_inflict(ch, victim, damage_to_inflict, 0,DAM_LIGHTNING);
			}
		}
	}

	/* Trigger death. */
	if (vch->hit <= 0)
		kill_unit (vch);

	return true;
}
/*
 * Basic attack.  This includes damroll and crude level based attack bonus and
 * defense application.
 */
void do_attack (char_data * ch, char * argument)
{
	char_data * vch;
//if (!IS_NPC(ch)){
//    ch->printlnf ("%s",ch->castingtarget1);}
	if (!is_tbcfighting (ch))
	{
		ch->println ("You're not fighting!\n\r");
		return;
	}

	if (argument[0] == '\0')
	{
		ch->println ("Attack whom?\n\r");
		return;
	}
	if ((vch = get_char_room (ch, argument)) == NULL)
	{
		ch->println ("They aren't here!\n\r");
		return;
	}
	if (!is_tbcfighting (vch) || is_same_group (ch, vch))
	{
		ch->println ("They aren't fighting you!\n\r");
		return;
	}

	if (!is_tbcactive (ch))
	{
		ch->println ("Just wait your turn.\n\r");
		return;
	}


    //one_hit( ch, vch, -1, true ); //crashed

	act ("You execute a physical attack against $N.", ch, NULL, vch, TO_CHAR);
	act ("$n executes a physical attack against you.", ch, NULL, vch, TO_VICT);
	act ("$n executes a physical attack against $N.", ch, NULL, vch, TO_NOTVICT);
	damage_new (ch, vch, (GET_DAMROLL(ch) + ch->level) - vch->level, 0, "attack",
				false, DAM_OTHER);
 //   if (!IS_NPC(ch)){
 //   ch->printlnf ("%s",ch->castingtarget1);}
	if (is_tbcfighting (ch))
		turn_end (ch->turn, 200 - GET_SPEED (ch));

	return;
}

/*
 * Displays the active turn list.
 * Goes 20 turns deep, assuming everyone uses "Attack".
 */
void do_turns (char_data * ch, char * argument)
{
	char buf[100];
	TURN_DATA * turn, * list;
	int round = 1, count = 0;
	int atime = 200, btime = 0;

	if (!is_tbcfighting (ch))
	{
		ch->println ("You aren't part of a battle.\n\r");
		return;
	}

	for (list = ch->in_battle->turn_list; list != NULL; list = list->next)
		count++;

	if (count < 1)
	{
		ch->println ("You aren't part of a battle, yet.\n\r");
		return;
	}

	list = ch->in_battle->turn_list;
	turn = ch->in_battle->turn_list;
	while (round < 21)
	{
		if (turn->roundtime + (atime - GET_SPEED (turn->unit)) >
			list->roundtime + (btime - GET_SPEED (turn->unit)))
		{
			if (list->next == NULL)
			{
				turn = list;
				list = ch->in_battle->turn_list;
				btime += 200;
			}
			else
			{
				turn = list;
				list = list->next;
			}
		}
		atime += 200;

		if (turn->unit != ch)
		{
			sprintf (buf, "`D[`C%2d`D]`R $N", round);
			act (buf, ch, NULL, turn->unit, TO_CHAR);
		}
		else
		{
			sprintf (buf, "`D[`C%2d`D]`w You`x\n\r", round);
			ch->println (buf);
		}
		round++;
	}
	return;
}

/*
 * Displays the unit list for a battle.
 */
void do_units (char_data * ch, char * argument)
{
	char buf[100];
	char_data * unit;
	int count = 0;

	if (!is_tbcfighting (ch))
	{
		ch->println ("You aren't part of a battle.\n\r");
		return;
	}

	for (unit = ch->in_battle->unit_list; unit != NULL; unit = unit->next_in_battle)
	{
		count++;

		if (unit != ch)
		{
			if (is_same_group (unit, ch))
				sprintf (buf, "`D[`M%2d`D]`C $N", count);
			else
				sprintf (buf, "`D[`M%2d`D]`R $N", count);
			act (buf, ch, NULL, unit, TO_CHAR);
		}
		else
		{
			sprintf (buf, "`D[`M%2d`D]`w You`x\n\r", count);
			ch->println (buf);
		}
	}
	return;
}

/*
 * Skips this turn, letting the next character go first.
 */
void do_wait (char_data * ch, char * argument)
{
	if (!is_tbcfighting (ch) || !is_tbcactive (ch))
	{
		act ("You wait patiently.", ch, NULL, NULL, TO_CHAR);
		act ("$n waits patiently.", ch, NULL, NULL, TO_ROOM);
		return;
	}

	act ("You bide your time to see what $N will do.", ch, NULL,
			ch->in_battle->turn_list->next->unit, TO_CHAR);
	act ("$n bides $s time to see what you will do.", ch, NULL,
			ch->in_battle->turn_list->next->unit, TO_VICT);
	act ("$n bides $s time to see what $N will do.", ch, NULL,
			ch->in_battle->turn_list->next->unit, TO_NOTVICT);
	skip_turn (ch->turn, true);
	return;
}

/*
 * Attempts to flee from combat.
 */
void do_escape (char_data * ch, char * argument)
{
	COMBAT_DATA * battle;

	if (!is_tbcfighting (ch))
	{
		ch->println ("You try frantically to escape from your own stupidity.\n\r");
		return;
	}
	if (!is_tbcactive (ch))
	{
		ch->println ("Just wait your turn to run away screaming!\n\r");
		return;
	}

	if (number_percent () > 40)
	{
		act ("You failed to escape!", ch, NULL, NULL, TO_CHAR);
		act ("$n tries desperately to escape, but fails!", ch, NULL, NULL, TO_ROOM);
		turn_end (ch->turn, 160 - GET_SPEED (ch));
		return;
	}

	battle = ch->in_battle;

	act ("You execute a daring escape to stage left!", ch, NULL, NULL, TO_CHAR);
	act ("$n executes a daring escape to stage left!", ch, NULL, NULL, TO_ROOM);
	char_from_combat (ch);
	check_victory (battle);
	return;
}
/****************************************************************************/
/*
 * Forces a mobile to take its turn.
 */
void mobile_turn (char_data * ch)
{
	char_data * vch, * list;

	vch = ch;

	for (list = ch->in_battle->unit_list; list != NULL; list = list->next_in_battle)
	{
		if (!is_same_group (ch, list))
		{
			if (vch == ch)
				vch = list;
			else if (vch != ch && number_percent () < 45)
				vch = list;
			else continue;
		}
	}

	/*
	 * I'm leaving this at just attack for obvious reasons.
	 * Be creative, will ya?
	 */
	if (vch == ch)
		clean_combat (ch->in_battle);
	else
		do_attack (ch, vch->name);
}

/*
 * PC version of mobile turn, for linkdead characters.
 */
void linkdead_turn (char_data * ch)
{
	char_data * vch, * list;

	if (number_percent () < 40)
	{
		do_escape (ch, "");
		return;
	}

	vch = ch;

	for (list = ch->in_battle->unit_list; list != NULL; list = list->next_in_battle)
	{
		if (!is_same_group (ch, list))
		{
			if (vch == ch)
				vch = list;
			else if (vch != ch && number_percent () < 45)
				vch = list;
			else continue;
		}
	}

	if (vch == ch)
		clean_combat (ch->in_battle);
	else
		do_attack (ch, vch->name);
}


/*
 * Dispatches the KO affect to a unit and removes its turn from the list.
 * Will not remove the unit from combat.
 */
void kill_unit (char_data * ch)
{
	if (!is_tbcfighting (ch))
		return;


	SET_BIT(ch->affected_by2, AFF2_KNOCKOUT);
	remove_turn (ch->turn);

//	act ("`DYou feel the sweet embrace of sleep eternal as your body "
//	     "fades out of existence.`x\n\r",
//			ch, NULL, NULL, TO_CHAR);
	act ("$n has been defeated.\n\r", ch, NULL, NULL, TO_ROOM);

	check_victory (ch->in_battle);
}

/* Removes the KO affect from a unit and adds its turn back to the list.
 * Will not remove the unit from combat.

void revive_unit (char_data * ch)
{
	if (!is_fighting (ch))
		return;

	REMOVE_BIT(ch->affected_by2, AFF2_KNOCKOUT);

	ch->turn->roundtime = (ch->in_battle->turn_list->roundtime + 500);
	insert_turn (ch->in_battle, ch->turn);
}
*/
/*
 * This will end combat. tally up the experience
 */
void check_victory (COMBAT_DATA * battle)
{
	char_data * leader;
	char_data * unit;
	bool victory = true;
	bool found = false;


	//First look for PCs.  Since mobs don't gain, if only mobs are left, we abort.
	for (unit = battle->unit_list; unit != NULL; unit = unit->next_in_battle)
		if (!IS_NPC (unit))
			found = true;

	if (!found)
	{
        clean_combat (battle);
		return;
	}

	//PCs exist!  Grab a guy.
	for (leader = battle->unit_list; leader != NULL; leader = leader->next_in_battle)
	{
		if (IS_AFFECTED2(leader, AFF2_KNOCKOUT))
			continue;

		//Once we find the first guy that isn't dead, we look for another guy that
		//isn't dead, and isn't on his team.  If one is found, battle's over.
		for (unit = battle->unit_list; unit != NULL; unit = unit->next_in_battle)
		{
			if ((IS_AFFECTED2 (unit, AFF2_KNOCKOUT) && !is_same_group (leader, unit))
				|| is_same_group (leader, unit) || leader == unit)
				continue;
			else
			{
				victory = false;
				break;
			}
		}

		if (!victory)
			continue;
		else
		{
			for (unit = battle->unit_list; unit != NULL; unit = unit->next_in_battle)
			{
				if (is_same_group (leader, unit))
					act ("Your party has won the battle!\n\r", unit, NULL, NULL, TO_CHAR);

              	else
					act ("Your party has lost the battle...\n\r", unit, NULL, NULL, TO_CHAR);
			}
			clean_combat (battle);
			return;
		}

	}
}

/*
 * True if ch is part of any combat.
 */
bool is_tbcfighting (char_data * ch)
{
	if (ch->in_battle != NULL)
		return true;

	return false;
}

/*
 * True if ch may take a turn right now.
 */
bool is_tbcactive (char_data * ch)
{
	if (!is_tbcfighting (ch))
		return false;

	if (ch->turn == ch->in_battle->turn_list)
		return true;

	return false;
}

/*
 * Inserts a character into combat and adds their turn to the list.
 */
void char_to_combat (char_data * ch, COMBAT_DATA * combat)
{
	TURN_DATA * turn;

	if (combat->unit_list == NULL)
		combat->unit_list = ch;
	else
	{
		ch->next_in_battle = combat->unit_list;
		combat->unit_list  = ch;
	}
	ch->in_battle = combat;

	turn = new_turn ();
	turn->unit = ch;
	turn->in_battle = combat;

	//So people don't end up getting a dozen turns by joining in.
	if (combat->turn_count > 0)
		turn->roundtime = (combat->turn_list->roundtime + 200) - GET_SPEED (ch);
	else
		turn->roundtime = 500 - GET_SPEED (ch);

	ch->turn = turn;
	insert_turn (combat, turn);
}
/*
 * Removes a ch and their turn entry from combat.
 */
void char_from_combat (char_data * ch)
{
	char_data * prev;

	if (ch == NULL)
	{
		bug ("cfc () -- null ch!?");
		return;
	}

	if (!is_tbcfighting (ch))
		return;

	if (ch->in_battle->unit_list == ch)
	{
		ch->in_battle->unit_list = ch->next_in_battle;
		ch->next_in_battle = NULL;
	}
	else
	{
		for (prev = ch->in_battle->unit_list; prev != NULL; prev = prev->next_in_battle)
		{
			if (prev->next_in_battle == ch)
			{
				prev->next_in_battle = ch->next_in_battle;
				ch->next_in_battle = NULL;
				break;
			}
		}

		if (prev == NULL)
		{
			bug ("cfc () -- ch not found.");
			return;
		}
	}
	remove_turn (ch->turn);
//	ch->turn = NULL;
	ch->in_battle = NULL;

	/*
	 * Here I've chosen to revive characters as they leave combat, but
	 * you'll want to enter PCs into your death handler at this point,
	 * and extract mobiles.
	 */

	if (IS_AFFECTED2 (ch, AFF2_KNOCKOUT))
	{
		REMOVE_BIT(ch->affected_by2, AFF2_KNOCKOUT);
//		ch->hit = UMAX(1, ch->hit);
	}
}

/*
 * Returns the data for ch's next/current turn.
 */
TURN_DATA * get_turn_char (char_data * ch)
{
	TURN_DATA * turn;

	if (!is_tbcfighting (ch))
		return NULL;

	for (turn = ch->in_battle->turn_list; turn != NULL; turn = turn->next)
		if (turn->unit == ch)
			return turn;

	return NULL;
}

/*
 * Inserts a turn into its proper position in the specified list.
 */
void insert_turn (COMBAT_DATA * combat, TURN_DATA * nTurn)
{

	if (combat->turn_list == NULL)
	{
		combat->turn_list = nTurn;
		nTurn->in_battle = combat;
	}
	else
	{
		TURN_DATA * prev;

		if (combat->turn_list->roundtime > nTurn->roundtime)
		{
			nTurn->next = combat->turn_list;
			combat->turn_list = nTurn;
			nTurn->in_battle = combat;
		}
		else
		{
			for (prev = combat->turn_list; prev != NULL; prev = prev->next)
			{
				if (prev->next != NULL && prev->next->roundtime > nTurn->roundtime)
				{
					nTurn->next = prev->next;
					prev->next  = nTurn;
					nTurn->in_battle = combat;
					break;
				}
				else if (prev->next == NULL)
				{
					prev->next  = nTurn;
					nTurn->in_battle = combat;
					break;
				}
			}
		}
	}
}

/*
 * Removes a turn from the list without freeing it.
 * It isn't freed here because you have to use this every time you move a turn
 * to a new position in the list.
 */
void remove_turn (TURN_DATA * turn)
{
	TURN_DATA * prev;

	if (turn == NULL)
	{
		bug ("remove_turn () -- null turn!?");
		return;
	}

	if (turn == turn->in_battle->turn_list)
	{
		turn->in_battle->turn_list = turn->next;
		turn->next = NULL;
	}
	else
	{
		for (prev = turn->in_battle->turn_list; prev != NULL; prev = prev->next)
		{
			if (prev->next == turn)
			{
				prev->next = turn->next;
				turn->next = NULL;
				break;
			}
		}
	}
}


/*
 * Similar to roundtime (), but used specifically at the end of each action.
 */
void turn_end (TURN_DATA * turn, int roundtime)
{
	turn->timer = 0;
	turn->roundtime += roundtime;
//    char_data *ch;
	//code here to update affects each turn.
//    ch->println("TURN END");
	remove_turn (turn);
	insert_turn (turn->in_battle, turn);
	turn->in_battle->turn_count++;
}

/*
 * Moves the turn down the list a bit.
 */
void skip_turn (TURN_DATA * turn, bool fWilling)
{
	turn->roundtime = turn->next->roundtime + number_range (45, 75);
	remove_turn (turn);
	insert_turn (turn->in_battle, turn);
	if (fWilling)
		turn->in_battle->turn_count++;
}

/*
 * Adds to the roundtime of a turn, then replaces it in the order.
 */
void roundtime (TURN_DATA * turn, int roundtime)
{
	turn->roundtime += roundtime;
	remove_turn (turn);
	insert_turn (turn->in_battle, turn);
}

/*
 * Swaps the placement of two turns, by switching roundtimes and reordering.
 */
void swap_turn (TURN_DATA * aTurn, TURN_DATA * bTurn)
{
	COMBAT_DATA * battle = aTurn->in_battle;
	int rt = aTurn->roundtime;

	aTurn->roundtime = bTurn->roundtime;
	bTurn->roundtime = rt;

	remove_turn (aTurn);
	remove_turn (bTurn);
	insert_turn (battle, aTurn);
	insert_turn (battle, bTurn);
}

/*
 * Just in case some newb with a trigger leaves a battle against a training
 * dummy running for weeks on end with trigs, or something.  You can never
 * have too much protection, y'know.
 */
void nuke_roundtime (COMBAT_DATA * battle)
{
	TURN_DATA * list;

	for (list = battle->turn_list; list != NULL; list = list->next)
		list->roundtime -= 2000000000;
}

/*
 * Creates a new battle between two parties.
 * Recycles the data if there somehow aren't enough people.
 */
void initiate_combat (char_data * ch, char_data * vch)
{
	char_data * och;
	TURN_DATA * turn;
	COMBAT_DATA * battle;
	int acount = 0, bcount = 0;

	battle = new_combat ();

	if (battle_list == NULL)
		battle_list = battle;
	else
	{
		battle->next = battle_list;
		battle_list = battle;
	}


	for (och = ch->in_room->people; och != NULL; och = och->next_in_room)
	{
		if (is_same_group (ch, och))
			char_to_combat (och, battle);
		if (is_same_group (vch, och))
			char_to_combat (och, battle);
	}

	for (turn = battle->turn_list; turn != NULL; turn = turn->next)
	{
		if (is_same_group (ch, turn->unit))
			acount++;
		else
			bcount++;
	}
	if (acount < 1 || bcount < 1)
		clean_combat (battle);
}

/*
 * Frees combat data at the end of battle.
 */
void clean_combat (COMBAT_DATA * battle)
{
	COMBAT_DATA * list;
	char_data * unit, *unit_next;

	for (unit = battle->unit_list; unit != NULL; unit = unit_next)
	{
		unit_next = unit->next_in_battle;
		char_from_combat (unit);
		//Perhaps call raw_kill() here on KOd mobiles?
	}

	if (battle == battle_list)
		battle_list = battle->next;
	else
	for (list = battle_list; list != NULL; list = list->next)
	{
		if (list->next == battle)
			list->next = battle->next;
	}

	battle->next = NULL;


	free_combat (battle);
}

/*
 * Standard recycling functions for turns and battles.
 */
TURN_DATA *turn_free;

TURN_DATA *new_turn (void)
{
    static TURN_DATA turn_zero;
    TURN_DATA *turn;

    if (turn_free == NULL)
        turn = (TURN_DATA *)alloc_perm (sizeof (*turn));
    else
    {
        turn = turn_free;
        turn_free = turn_free->next;
    }

    *turn = turn_zero;
    VALIDATE (turn);
    return turn;
}

void free_turn (TURN_DATA * turn)
{
    if (!IS_VALID (turn))
        return;

    INVALIDATE (turn);

    turn->next = turn_free;
    turn_free = turn;
}

COMBAT_DATA *combat_free;

COMBAT_DATA *new_combat (void)
{
    static COMBAT_DATA combat_zero;
    COMBAT_DATA *combat;

    if (combat_free == NULL)
        combat = (COMBAT_DATA*)alloc_perm (sizeof (*combat));
    else
    {
        combat = combat_free;
        combat_free = combat_free->next;
    }

    *combat = combat_zero;
    VALIDATE (combat);
    return combat;
}

void free_combat (COMBAT_DATA * combat)
{
    if (!IS_VALID (combat))
        return;

    INVALIDATE (combat);

    combat->next = combat_free;
    combat_free = combat;
}
/**************************************************************************/
 // Strips the effects of tension.

void strip_tension (char_data * ch)
{
	if (ch->tension == 0)
		return;

	act ("Your tension level returns to normal.", ch, NULL, NULL, TO_CHAR);
	act ("$n's tension level returns to normal.", ch, NULL, NULL, TO_ROOM);
	affect_strip (ch, gsn_tension);
	ch->tension = 0;
}

void do_tension (char_data * ch, char * argument)
{

	// Make sure they're fighting.

	if (ch->position != POS_FIGHTING)
	{
		ch->println("But you're not fighting!\n\r");
		return;
	}


	 // Start psyching up.

	if (ch->tension < 1)
	{
		act ("You begin to build your tension!", ch, NULL, NULL, TO_CHAR);
		act ("$n begins to build their tension!", ch, NULL, NULL, TO_ROOM);
		ch->tension = 1;
	}

	// Unleash the tension!

	else
	{
		//Grab the victim.
		char_data * vch = ch->fighting;

		//They're not fighting anymore?
		if (ch->fighting == NULL)
		{
		ch->println("You try to unleash your tension, but can't find any poor sap to do it on.\n\r");
		strip_tension (ch);
		return;
		}

		//Unleash ch's Squirrely Wr-- er, Tension.
		act ("You unleash all your tension on $N!", ch, NULL, vch, TO_CHAR);
		act ("$n unleashes all $s tension on $N!", ch, NULL, vch, TO_NOTVICT);
		act ("$n unleashes all $s tension on you!", ch, NULL, vch, TO_VICT);
		tension_hit (ch, vch);
		strip_tension (ch);
	}
	return;
}


// Group of tension attacks, hacked off of multi_hit.
// Second and third attack are much easier with tension.
// Also, tension attacks are unaffected by haste or slow.

void tension_hit (char_data * ch, char_data * victim)
{
    int chance, mod;

	switch (ch->tension)
	{
		//They have no real tension, cancel.
		default:
			act ("You stop building your tension.", ch, NULL, NULL, TO_CHAR);
			act ("$n stops building their tension.", ch, NULL, NULL, TO_ROOM);
			ch->tension = 0;
			return;
		case 2:
			mod = 50;
		case 3:
			mod = 25;
		case 4:
			mod = 5;
		case 5:
			mod = 0;
	}

	//First hit.
    one_hit (ch, victim, TYPE_UNDEFINED, false);

	//Make sure the enemy is still there.
    if (ch->fighting != victim)
        return;

	//Second attack check.
    chance = UMAX (0, get_skill (ch, gsn_second_attack) / 2 + mod);

    if (number_percent () < chance)
    {
        one_hit (ch, victim, TYPE_UNDEFINED, false);

        if (ch->fighting != victim)
            return;
    }

	//Third attack check.
    chance = UMAX (0, get_skill (ch, gsn_third_attack) / 4 + mod);

    if (number_percent () < chance)
    {
        one_hit (ch, victim, TYPE_UNDEFINED, false);

        if (ch->fighting != victim)
            return;
    }
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_cardform( char_data *ch, char *argument)
{
    char_data *victim;
    int percent;


    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    if ( !IS_NPC(ch)
		&&   ch->level < skill_table[gsn_cardform].skill_level[ch->clss] )
	{
		ch->println("You have no clue how to do that.");
		return;
    }

    if (IS_NPC(ch)){
		return;
	}

	if(IS_NULLSTR(argument))
	{
		victim = ch->fighting;
		if (victim == NULL)
		{
			ch->println("But you aren't fighting anyone!");
			return;
		}
	}
	else if ((victim = get_char_room(ch,argument)) == NULL)
	{
		ch->printlnf("'%s' is not here.", argument);
		return;
	}

	if (victim == ch)
	{
		ch->println("You can't do that.");
		return;
	}

	if (is_safe(ch,victim))
		return;

 /*   if(IS_NULLSTR(victim->card_form)){
    victim->card_form = str_dup("NONE");
    }*/

    if (IS_SET( victim->form, FORM_SENTIENT )
        || !IS_NPC(victim)){
        ch->println("You can't use that on sentients or players.");
		return;
	}

	if ( !can_initiate_combat( ch, victim, CIT_GENERAL )) return;

	if ( victim->max_hit > 0 )
        percent = ( 100 * victim->hit ) / victim->max_hit;
	else
        percent = -1;

    WAIT_STATE( ch, skill_table[gsn_cardform].beats );


    if (!str_cmp( victim->card_form, "NONE" )
    || IS_NULLSTR(victim->card_form)){
    ch->print("You failed!!");
    return;
    }

    MAGNUS_DATA * magcardcheck;
    magcardcheck = magnus_lookup(victim->card_form);
    if (!magcardcheck){
    ch->print("You failed!!");
    return;
    }

    if ( get_skill(ch,gsn_cardform) > number_percent()
	&& percent <=  50)
    {
		ch->printlnf("With a swirl of lights %s is transformed.", victim->short_descr);
		OBJ_DATA * magnus;
        magnus = create_object(get_obj_index(1380));
        MAGNUS_DATA * magcard;
        magcard = magnus_lookup(victim->card_form);

        magnus->level       = magcard->level;
        if (victim->level >= magcard->level){
        magnus->level       = victim->level;}
        magnus->description = magcard->description;
        magnus->short_descr = magcard->short_d;
        magnus->long_descr  = magcard->long_d;
        magnus->value[0]    = magcard->element;
        magnus->value[1]    = magcard->refine;
        magnus->value[2]    = magcard->refinecost;
        magnus->value[3]    = magcard->magnus_number;
		ch->printlnf("You recieve %s!", magnus->short_descr);
        obj_to_char(magnus , ch);
	//	v0 element
	//  v1 refineobj
	//  v2 refineamount
		extract_char(victim,true);
		check_improve(ch,gsn_cardform,true,1);
    }
    else
    {
		ch->println("You fail miserably.");
		check_improve(ch,gsn_kick,false,1);
    }
    return;
}
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

