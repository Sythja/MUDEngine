/**************************************************************************/
// skills.cpp - gain, train, prac etc
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
#include "o_lookup.h"
#include "magic.h"
#include "d2magsys.h"
#include "math.h"
#include "nanny.h"

// command procedures needed
DECLARE_DO_FUN(do_skillgroups);
DECLARE_DO_FUN(do_help		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_spinfo	);
DECLARE_DO_FUN(do_cspinfo	);
DECLARE_DO_FUN(do_cskinfo	);
int morphable_weapon_lookup(const char *weapon_class);

bool	remove_obj	args( (char_data *ch, int iWear, bool fReplace ) );

/**************************************************************************/
// used to get new skills
/*void do_gain(char_data *ch, char *argument)
{
    char arg[MIL];
    char_data *trainer;
    int gn = 0, sn = 0;

    if (IS_NPC(ch))
		return;


	one_argument(argument,arg);

	// gain list no longer requires you to be at the healers
	// - Kal, Feb 06
	if (!IS_NULLSTR(arg) && !str_prefix(arg,"list"))
	{
		int col;

		col = 0;


		ch->printlnf("%-28s %-5s    %-28s %-5s",
			"skillgroup","cost","skillgroup","cost");

		// skillgroups at the top
		for (gn = 0; !IS_NULLSTR(skillgroup_table[gn].name); gn++)
		{
			if (!ch->pcdata->skillgroup_known[gn]
				&&  IS_SKILLGROUP_SELECTABLE_FOR_CHAR(gn, ch))
			{
				ch->printf("%s%-28s %-5d    ",
					skillgroup_table[gn].rating[ch->clss]>ch->train?"`S":"`x",
					skillgroup_table[gn].name,
					skillgroup_table[gn].rating[ch->clss]);
				if (++col % 2 == 0){
					ch->print_blank_lines(1);
				}
			}
		}
		if (col % 2 != 0)
		{
			ch->println("");
		}
		// skillgroups up above

		ch->println("`x");

		// skills below
		col = 0;
		ch->print("`?`#`Yskill `^=============="
			"`Ycost`^==`Ylevel`^=======");
		ch->println("`Yskill`^==============`Ycost"
			"`^==`Ylevel`x");
		for (sn = 0; sn < MAX_SKILL; sn++)
		{
			if (IS_NULLSTR(skill_table[sn].name)){
				break;
			}

			if(IS_SET(skill_table[sn].flags,SKFLAGS_NO_GAIN)){
				continue;
			}

			if( IS_SET(skill_table[sn].flags,SKFLAGS_USE_RACE_RESTRICTIONS)
				&& !IS_SETn(skill_table[sn].race_restrict_n, ch->race))
			{
				continue;
			}

			if (!ch->pcdata->learned[sn]
				&&  skill_table[sn].rating[ch->clss] > 0
				&&  skill_table[sn].skill_level[ch->clss]< LEVEL_IMMORTAL
				&&  skill_table[sn].skill_level[ch->clss]>0
				&&  skill_table[sn].spell_fun == spell_null)
			{
				ch->printf("%s%-18s %3d  %4d         ",
					skill_table[sn].rating[ch->clss]>ch->train?"`S":"`x",
					skill_table[sn].name,
					skill_table[sn].rating[ch->clss],
					skill_table[sn].skill_level[ch->clss]);
				if (++col % 2 == 0)
					ch->print_blank_lines(1);
			}
		}

		if (col % 2 != 0){
			ch->println("`x");
		}else{
			ch->print("`x");
		}

		ch->println("`^-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-`x");
		// display number of trains and pracs, then comments about converting between
		ch->printlnf("You have %d train%s, and %d practice%s.",
			ch->train,
			ch->train==1?"":"s",
			ch->practice,
			ch->practice==1?"":"s");
		ch->println("`Snotes: It is possible to convert 10 pracs into 1 train with 'gain convert'.");
		ch->println("You can also convert 1 train into 10 pracs with 'gain revert' after lvl9.`x");
		ch->println("`^-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-`x");

		return;
		// skills above
	}// gain list


    // find a trainer
    for ( trainer = ch->in_room->people; trainer; trainer = trainer->next_in_room)
	{
		if (IS_NPC(trainer) && IS_SET(trainer->act,ACT_GAIN))
			break;
	}

	if (trainer == NULL || !can_see(ch,trainer))
	{
		ch->println( "You can't do that here." );
		return;
	}


	if(IS_NULLSTR(arg))
	{
		do_say(trainer,"Pardon me?");
		return;
	}


	if ( ch->position < POS_STANDING )
	{
		ch->println( "You need to stand first." );
		return;

	}


	if (!str_prefix(arg,"convert"))
	{
		if (ch->practice < 10)
		{
			act("$N tells you 'You are not yet ready.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		act("$N helps you apply your practice to training",
			ch,NULL,trainer,TO_CHAR);
		ch->practice -= 10;
		ch->train++;
		return;
	}

	if (!str_prefix(arg,"revert"))
	{
		if (ch->train < 1)
		{
			act("$N tells you 'You are not yet ready.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		if(ch->level<10)
		{
			ch->println("You can't revert trains before level 10.");
			return;
		}

		act("$N helps you apply your training to practice",
			ch,NULL,trainer,TO_CHAR);
		ch->practice+=10;
		ch->train--;
		return;
	}

	if (!str_prefix(arg,"points"))
	{
		if (ch->train < 2)
		{
			act("$N tells you 'You are not yet ready.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		if (ch->pcdata->points <= 40)
		{
			act("$N tells you 'There would be no point in that.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		act("$N trains you, and you feel more at ease with your skills.",
			ch,NULL,trainer,TO_CHAR);

		// record how much xp they have beyond their current level
		int beyond=UMAX(0, ch->exp - (exp_per_level(ch,ch->pcdata->points) * ch->level));
		// reduce their creation points
		ch->train -= 2;
		ch->pcdata->points -= 1;
		ch->exp = exp_per_level(ch,ch->pcdata->points) * ch->level;
		// give them back the xp they already had on that given level
		if(beyond){
			gain_exp(ch, beyond);
		}
		return;
	}

	 //else add a group/skill

	gn = skillgroup_lookup(argument);
	if (gn > 0)
	{
		if (ch->pcdata->skillgroup_known[gn])
		{
			act("$N tells you 'You already know that skillgroup!'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		if (skillgroup_table[gn].rating[ch->clss] <= 0)
		{
			act("$N tells you 'That skillgroup is beyond your powers.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		if (ch->train < skillgroup_table[gn].rating[ch->clss])
		{
			act("$N tells you 'You are not yet ready for that skillgroup.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		if (!IS_SKILLGROUP_SELECTABLE_FOR_CHAR(gn, ch)){
			ch->printlnf("%s tells you 'You aren't ready for the skillgroup '%s' at this point in time.'",
				PERS(trainer, ch), skillgroup_table[gn].name);
			return;
		}

		// add the group
		gn_add(ch,gn);
		act("$N trains you in the art of $t",
			ch,skillgroup_table[gn].name,trainer,TO_CHAR);
		ch->train -= skillgroup_table[gn].rating[ch->clss];
		set_char_magic_bits(ch); // fix up their magic bits
		return;
	}

	sn = skill_lookup(argument);
	if (sn > -1)
	{
		if (skill_table[sn].spell_fun && skill_table[sn].spell_fun != spell_null)
		{
			act("$N tells you 'You must learn the full group.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		if( IS_SET(skill_table[sn].flags,SKFLAGS_USE_RACE_RESTRICTIONS)
			&& !IS_SETn(skill_table[sn].race_restrict_n, ch->race))
		{
			act("$N tells you 'Your race can't learn that skill.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		if(IS_SET(skill_table[sn].flags,SKFLAGS_NO_GAIN)){
			act("$N tells you 'That skill can't be gained sorry.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		if (ch->pcdata->learned[sn])
		{
			act("$N tells you 'You already know that skill!'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		if (skill_table[sn].rating[ch->clss] <= 0
			|| skill_table[sn].skill_level[ch->clss] >= LEVEL_IMMORTAL
			|| skill_table[sn].skill_level[ch->clss]<=0)
		{
			act("$N tells you 'That skill is beyond your powers.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		if (ch->train < skill_table[sn].rating[ch->clss]*1)
		{
			act("$N tells you 'You are not yet ready for that skill.'",
				ch,NULL,trainer,TO_CHAR);
			return;
		}

		// add the skill
		ch->pcdata->learned[sn] = 1;
		act("$N trains you in the art of $t",
			ch,skill_table[sn].name,trainer,TO_CHAR);
		ch->train -= skill_table[sn].rating[ch->clss]*1;
		set_char_magic_bits(ch); // fix up their magic bits
		return;
	}

	act("$N tells you 'I do not understand...'",ch,NULL,trainer,TO_CHAR);
}
*/
/* Not within a comment line, wtf! */
/* Not within a comment line, wtf! */
/****************************************************************************/

void do_skills(char_data *ch, char *argument)
{
	char *skill_list[MAX_LEVEL+2];
    unsigned char skill_columns[MAX_LEVEL+2];
	char linebuf[MSL];
    int sn,lev;
    char arg[MIL];
    char_data *victim;
    char buf[MSL];
    BUFFER *output;
	int top_level=0;
    bool found = false;

	if (IS_UNSWITCHED_MOB(ch)){
		return;
	}

    one_argument( argument, arg );

    if (IS_IMMORTAL(ch) && !IS_NULLSTR(arg))
    {
		if ( ( victim = get_char_world( ch, arg ) ) == NULL )
		{
			ch->println( "They aren't here." );
			return;
		}
    }
	else
	{
		victim=ch;
	}

/*    if (IS_NPC(victim))
	{
		ch->println( "Mobs don't have skill percentages yet." );
		return;
	}*/


    output = new_buf();
	{
		if(ch->desc && ch->desc->connected_state!=CON_PLAYING){
			if(ch==victim){
				sprintf(buf,"`=j%s", creation_titlebar("SKILLS"));
			}else{
				sprintf(buf,"`=j%s", creation_titlebar("SKILLS FOR %s", uppercase(victim->name)));
			}
		}else{
			if(ch==victim){
				sprintf(buf,"`=t%s", format_titlebar("SKILLS"));
			}else{
				sprintf(buf,"`=t%s", format_titlebarf("SKILLS FOR %s", uppercase(victim->name)));
			}
		}

		buf[str_len(buf)-2]= '\0';
		strcat(buf,"`x");
	}
	add_buf( output, buf);

	if (ch!=victim)
	{
//		sprintf(buf,"\r\nSkills for %s (Level: %d, Race: %s, Class: %s)",
		sprintf(buf,"\r\nSkills for %s (Level: %d, Race: %s)",
			victim->name,
			victim->level,
			race_table[victim->race]->name
//			class_table[victim->clss].name
			);
		add_buf( output, buf);
	}

    // initialize data
    for (lev = 0; lev < MAX_LEVEL+2; lev++){
        skill_columns[lev] = 0;
        skill_list[lev]= str_dup("");
    }

	if (IS_IMMORTAL(ch)){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}

	bool specially_granted_skills=false;
    for (sn = 0; !IS_NULLSTR(skill_table[sn].name) ; sn++)
    {
		if (skill_table[sn].spell_fun == spell_null
			&& skill_table[sn].sktype==SKTYPE_SKILL
			&&	(
					(victim->pcdata->learned[sn]==101)
				  ||
					(skill_table[sn].skill_level[victim->clss]<=top_level
					&& skill_table[sn].skill_level[victim->clss]>0)
			//		&& victim->pcdata->learned[sn] > 0)
				)
			)
		{
			if(victim->pcdata->learned[sn]==101
				&& skill_table[sn].skill_level[victim->clss]>LEVEL_HERO)
			{
				specially_granted_skills=true;
			}

			found = true;
			lev = skill_table[sn].skill_level[victim->clss];
			if(IS_SET(skill_table[sn].flags,SKFLAGS_NEW_IMPROVE_SYSTEM)){
				if (victim->level < lev){
					sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
				}else{
					sprintf(buf,"%-18s %3d%%(%3d%%,%3d%%)       ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn],
						skill_table[sn].learn_scale_percent[victim->clss],
						(skill_table[sn].learn_scale_percent[victim->clss]==0?
								100:skill_table[sn].learn_scale_percent[ch->clss]));
				}
			}else{
				if (victim->level < lev && victim->pcdata->learned[sn]!=101){
					if(ch->level<lev){
						sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
					}else{
						sprintf(buf,"%-18s  n/a(%3d%%)            ",
							capitalize(skill_table[sn].name),
							victim->pcdata->learned[sn]);
					}
				}else{
					sprintf(buf,"%-18s %3d%%                  ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn]);
				}
			}

			if (IS_NULLSTR(skill_list[lev])){
				sprintf(linebuf,"\r\n Level %2d: %s",lev,buf);
				replace_string(skill_list[lev], linebuf);
			}else{ // append
				if ( ++skill_columns[lev] % 2 == 0){
					sprintf(linebuf,"%s\r\n           ",skill_list[lev]);
					replace_string(skill_list[lev], linebuf);
				}else{
					// trim off the extra spaces
					strcpy(linebuf, skill_list[lev]);
					linebuf[str_len(linebuf)-6]='\0';
					replace_string(skill_list[lev], linebuf);
				}
				sprintf(linebuf,"%s%s",skill_list[lev],buf);
				replace_string(skill_list[lev], linebuf);
			}
		}
    }

	if (IS_IMMORTAL(ch) || specially_granted_skills){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}


    // return results
    if (!found){
		if (ch==victim){
			ch->println( "You know no skills." );
		}else{
			ch->println( "They know no skills." );
		}
    }else{
		for (lev = 1; lev < top_level; lev++){
			if(lev==LEVEL_IMMORTAL){
				if(IS_IMMORTAL(ch)){
					sprintf(buf, "\r\n\r\n%s", format_titlebar("SKILLS DISABLED FOR CHARACTER UNLESS GRANTED BELOW HERE"));
				}else{
					sprintf(buf, "\r\n\r\n%s", format_titlebar("SPECIALLY GRANTED SKILLS"));
				}
				buf[str_len(buf)-2]= '\0';
				strcat(buf,"`x");
				add_buf( output, buf);
			}
			if (!IS_NULLSTR(skill_list[lev])){
				add_buf( output, skill_list[lev] );
			}
		}
		add_buf( output, "\r\n");
		ch->sendpage(buf_string(output));
		free_buf(output);
	}

	// deallocate all memory used
	for (lev = 0; lev < MAX_LEVEL+2; lev++){
        free_string(skill_list[lev]);
    }
	return;
}

/****************************************************************************/
void do_spells(char_data *ch, char *argument)
{
	char *skill_list[MAX_LEVEL+2];
    unsigned char skill_columns[MAX_LEVEL+2];
	char linebuf[MSL];
    int sn,lev;
    char arg[MIL];
    char_data *victim;
    char buf[MSL];
    BUFFER *output;
	int top_level=0;
    bool found = false;

	if (IS_UNSWITCHED_MOB(ch)){
		return;
	}

    one_argument( argument, arg );

    if (IS_IMMORTAL(ch) && !IS_NULLSTR(arg))
    {
		if ( ( victim = get_char_world( ch, arg ) ) == NULL )
		{
			ch->println( "They aren't here." );
			return;
		}
    }
	else
	{
		victim=ch;
	}
/*
    if (IS_NPC(victim))
	{
		ch->println( "Mobs don't have skill percentages as such." );
		return;
	}
*/

    output = new_buf();
	{
		if(ch->desc && ch->desc->connected_state!=CON_PLAYING){
			if(ch==victim){
				sprintf(buf,"`=j%s", creation_titlebar("SPELLS"));
			}else{
				sprintf(buf,"`=j%s", creation_titlebar("SPELLS FOR %s", uppercase(victim->name)));
			}
		}else{
			if(ch==victim){
				sprintf(buf,"`=t%s", format_titlebar("SPELLS"));
			}else{
				sprintf(buf,"`=t%s", format_titlebarf("SPELLS FOR %s", uppercase(victim->name)));
			}
		}

		buf[str_len(buf)-2]= '\0';
		strcat(buf,"`x");
	}
	add_buf( output, buf);

	if (ch!=victim)
	{
//		sprintf(buf,"\r\nSkills for %s (Level: %d, Race: %s, Class: %s)",
		sprintf(buf,"\r\nSkills for %s (Level: %d, Race: %s)",
			victim->name,
			victim->level,
			race_table[victim->race]->name
//			class_table[victim->clss].name
			);
		add_buf( output, buf);
	}

    // initialize data
    for (lev = 0; lev < MAX_LEVEL+2; lev++){
        skill_columns[lev] = 0;
        skill_list[lev]= str_dup("");
    }

	if (IS_IMMORTAL(ch)){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}

	bool specially_granted_skills=false;
    for (sn = 0; !IS_NULLSTR(skill_table[sn].name) ; sn++)
    {
		if (skill_table[sn].spell_fun != spell_null
            &&  skill_table[sn].sktype==SKTYPE_SPELL
			&&	(
					(victim->pcdata->learned[sn]==101)
				  ||
					(skill_table[sn].skill_level[victim->clss]<=top_level
					&& skill_table[sn].skill_level[victim->clss]>0)
			//		&& victim->pcdata->learned[sn] > 0)
				)
			)
		{
			if(victim->pcdata->learned[sn]==101
				&& skill_table[sn].skill_level[victim->clss]>LEVEL_HERO)
			{
				specially_granted_skills=true;
			}

			found = true;
			lev = skill_table[sn].skill_level[victim->clss];
			if(IS_SET(skill_table[sn].flags,SKFLAGS_NEW_IMPROVE_SYSTEM)){
				if (victim->level < lev){
					sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
				}else{
					sprintf(buf,"%-18s %3d%%(%3d%%,%3d%%)       ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn],
						skill_table[sn].learn_scale_percent[victim->clss],
						(skill_table[sn].learn_scale_percent[victim->clss]==0?
								100:skill_table[sn].learn_scale_percent[ch->clss]));
				}
			}else{
				if (victim->level < lev && victim->pcdata->learned[sn]!=101){
					if(ch->level<lev){
						sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
					}else{
						sprintf(buf,"%-18s  n/a(%3d%%)            ",
							capitalize(skill_table[sn].name),
							victim->pcdata->learned[sn]);
					}
				}else{
					sprintf(buf,"%-18s %3d%%                  ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn]);
				}
			}

			if (IS_NULLSTR(skill_list[lev])){
				sprintf(linebuf,"\r\n Level %2d: %s",lev,buf);
				replace_string(skill_list[lev], linebuf);
			}else{ // append
				if ( ++skill_columns[lev] % 2 == 0){
					sprintf(linebuf,"%s\r\n           ",skill_list[lev]);
					replace_string(skill_list[lev], linebuf);
				}else{
					// trim off the extra spaces
					strcpy(linebuf, skill_list[lev]);
					linebuf[str_len(linebuf)-6]='\0';
					replace_string(skill_list[lev], linebuf);
				}
				sprintf(linebuf,"%s%s",skill_list[lev],buf);
				replace_string(skill_list[lev], linebuf);
			}
		}
    }

	if (IS_IMMORTAL(ch) || specially_granted_skills){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}


    // return results
    if (!found){
		if (ch==victim){
			ch->println( "You know no spells." );
		}else{
			ch->println( "They know no spells." );
		}
    }else{
		for (lev = 1; lev < top_level; lev++){
			if(lev==LEVEL_IMMORTAL){
				if(IS_IMMORTAL(ch)){
					sprintf(buf, "\r\n\r\n%s", format_titlebar("SPELLS DISABLED FOR CHARACTER UNLESS GRANTED BELOW HERE"));
				}else{
					sprintf(buf, "\r\n\r\n%s", format_titlebar("SPECIALLY GRANTED SPELLS"));
				}
				buf[str_len(buf)-2]= '\0';
				strcat(buf,"`x");
				add_buf( output, buf);
			}
			if (!IS_NULLSTR(skill_list[lev])){
				add_buf( output, skill_list[lev] );
			}
		}
		add_buf( output, "\r\n");
		ch->sendpage(buf_string(output));
		free_buf(output);
	}

	// deallocate all memory used
	for (lev = 0; lev < MAX_LEVEL+2; lev++){
        free_string(skill_list[lev]);
    }
	return;
}

/*******************************************************************************/
// Kal - Dec 00
void do_cskinfo(char_data *ch, char * argument)
{
	bool found = false;
	char buf[MSL], clss_name[MSL];
	int sn, count, lev;
	BUFFER *output;

	int clss_no;

	if (IS_NULLSTR(argument))
	{
		ch->wrapln( "CSKINFO - Class Skill Info - at what levels "
			"a particular class gets a skill and for how many trains.");
		ch->println( "Syntax: CSKINFO <class>" );
		return;
	}

	argument = one_argument (argument, clss_name);

	for (clss_no = 0; !IS_NULLSTR(class_table[clss_no].name); clss_no++)
	{
		if (!str_cmp(clss_name, class_table[clss_no].short_name) ||
            !str_prefix(clss_name,class_table[clss_no].name))
		{
			found = true;
			break;
		}
	}

	if ( !found )
	{
		ch->printlnf( "No class named '%s' exists. Use the 3-letter WHO names (Mag, Spf etc.)", clss_name);
		do_cskinfo(ch, "");
		return;
	}

	// setup a buffer for info to be displayed
	output	= new_buf();
	count	= 0;

	sprintf(buf,"Class Skill Info - all skills for `B%ss`x\r\n", class_table[clss_no].name);
	add_buf( output, buf);
	add_buf( output, "==Skill==========Level [Trains]==========");
	add_buf( output, "==Skill==========Level [Trains]==\r\n");

	for (lev=1; lev<=LEVEL_HERO; lev++)
	{
		for (sn = 0; !IS_NULLSTR(skill_table[sn].name) ; sn++)
		{
			if ( skill_table[sn].skill_level[clss_no]==lev
				&& skill_table[sn].spell_fun == spell_null )
			{
				count++;
				sprintf(buf, "  %-20.20s %3d [%2d]", capitalize(skill_table[sn].name),
					lev, skill_table[sn].rating[clss_no]);
				add_buf( output, buf);
				if(count%2==0){
					add_buf( output, "\r\n");
				}else{
					add_buf( output, "          ");
				}
			}
		}
	}
	sprintf(buf, "%s  %d skill%s total.",
		count%2==1?"\r\n":"",
		count,
		count==1?"":"s");
	add_buf( output, buf);

	ch->sendpage(buf_string(output));
	free_buf(output);
	return;
}
/**************************************************************************/
// shows skills, groups and costs (only if not bought)
void list_group_costs(char_data *ch)
{
    if (IS_NPC(ch))
        return;

    int gn,sn,col;

    col = 0;
    ch->print(" `g===`MSkillgroup`g==================`xCP`g====");
    ch->println("====`MSkillgroup`g==================`xCP`g====");

    for (gn = 0; !IS_NULLSTR(skillgroup_table[gn].name); gn++)
    {
        if (!ch->gen_data->skillgroup_chosen[gn]
			&&  !ch->pcdata->skillgroup_known[gn]
			&&  IS_SKILLGROUP_SELECTABLE_FOR_CHAR(gn, ch))
        {
			ch->printf("   `S[`M%-28.28s`x%2d`S]`x   ",
				skillgroup_table[gn].name,
				skillgroup_table[gn].rating[ch->clss]);
            if (++col % 2 == 0){
				ch->println("");
			}
        }
    }
    if ( col % 2 != 0 ){
        ch->println( "" );
	}
	ch->println( "" );

    col = 0;

    ch->print(" `g=`YSkill`g===========`xCP`g`BLevel`g=");
    ch->print("`g=`YSkill`g===========`xCP`g`BLevel`g=");
    ch->println("`g=`YSkill`g===========`xCP`g`BLevel`g=");

    for (sn = 0; !IS_NULLSTR(skill_table[sn].name); sn++)
	{

		// race restrictions
		// race restrictions
		if( IS_SET(skill_table[sn].flags,SKFLAGS_USE_RACE_RESTRICTIONS)
				&& !IS_SETn(skill_table[sn].race_restrict_n, ch->race))
		{
			continue;
		}

		// no_gain
		if(IS_SET(skill_table[sn].flags,SKFLAGS_NO_GAIN))
		{
			continue;
		}

        if (!ch->gen_data->skill_chosen[sn]
			&&  ch->pcdata->learned[sn] == 0
			&&  skill_table[sn].spell_fun == spell_null
			&&  skill_table[sn].rating[ch->clss] > 0
			&&  skill_table[sn].skill_level[ch->clss] < LEVEL_IMMORTAL
			&&  skill_table[sn].skill_level[ch->clss]>0)
        {
            ch->printf(" `S[`Y%-16.16s`x%2d`B<%2d>`S]`x",
				skill_table[sn].name,
                skill_table[sn].rating[ch->clss],
                skill_table[sn].skill_level[ch->clss]);
            if (++col % 3 == 0)
                ch->print( "\r\n" );
        }
    }
    if ( col % 3 != 0 )
        ch->print( "\r\n" );

    ch->println( "`x\r\n" );

    ch->printlnf( "Creation points (CP): %3d    Experience per level (XP): %d",
		ch->gen_data->points_chosen, exp_per_level(ch,ch->gen_data->points_chosen));

    return;

}
/**************************************************************************/
void list_group_chosen(char_data *ch)
{
    int gn,sn,col;

    col = 0;
    ch->print(" `g===`MSkillgroup`g==================`xCP`g====");
    ch->println("====`MSkillgroup`g==================`xCP`g====");

    for (gn = 0; !IS_NULLSTR(skillgroup_table[gn].name); gn++)
    {
        if (ch->gen_data->skillgroup_chosen[gn])
        {
			ch->printf("   `S[`M%-28.28s`x%2d`S]`x   ",
				skillgroup_table[gn].name,
				skillgroup_table[gn].rating[ch->clss]);
            if (++col % 2 == 0){
				ch->println("");
			}
        }
    }
    if ( col % 2 != 0 ){
        ch->println( "" );
	}
	ch->println( "" );


	col=0;
    ch->print(" `g=`YSkill`g===========`xCP`g`BLevel`g=");
    ch->print("`g=`YSkill`g===========`xCP`g`BLevel`g=");
    ch->println("`g=`YSkill`g===========`xCP`g`BLevel`g=");

    for (sn = 0; !IS_NULLSTR(skill_table[sn].name); sn++)
    {
        if (ch->gen_data->skill_chosen[sn]
			&&  skill_table[sn].rating[ch->clss] > 0)
        {
            ch->printf(" `S[`Y%-16.16s`x%2d`B<%2d>`S]`x",
				skill_table[sn].name,
                skill_table[sn].rating[ch->clss],
                skill_table[sn].skill_level[ch->clss]);
            if (++col % 3 == 0)
                ch->print( "\r\n" );
        }
    }
    if ( col % 3 != 0 )
        ch->print( "" );

    ch->println( "`x\r\n" );

    ch->printlnf( "Creation points (CP): %3d    Experience per level (XP): %d",
		ch->gen_data->points_chosen, exp_per_level(ch,ch->gen_data->points_chosen));
    return;
}
/**************************************************************************/
int exp_per_level(char_data *ch, int points)
{
    int expl,inc;

    if (IS_NPC(ch)){
		return 1000;
	}

    expl = 2000;
    inc = 500;


	if(race_table[ch->race]->class_exp[ch->clss]<2000){ // safety check
		ch->println("BUG: exp_per_level for your race is below 2000... please report this to the immortals!");
		return 5000;
	}

    if (points < 40){
        return race_table[ch->race]->class_exp[ch->clss];
	}

    // processing
    points -= 40;

    while (points > 9)
    {
		expl += inc;
		points -= 10;
		if (points > 9)
		{
			expl += inc;
			inc *= 2;
			points -= 10;
		}
    }
    expl += points * inc / 10;

    return UMAX(1000, expl * race_table[ch->race]->class_exp[ch->clss]/1000);

}
/**************************************************************************/
// this procedure handles the input parsing for the skill generator
bool parse_gen_groups(char_data *ch,char *argument)
{
    char arg[MIL];
    int gn,sn,i;

    if (IS_NULLSTR(argument)){
		return false;
	}

    argument = one_argument(argument,arg);

    if (!str_prefix(arg,"help"))
    {
		if (argument[0] == '\0')
		{
			do_help(ch,"group help");
			return true;
		}

        do_help(ch,argument);
		return true;
    }

    if (!str_prefix(arg,"add"))
    {
		if (IS_NULLSTR(argument))
		{
			ch->println( "You must provide a skill name." );
			return true;
		}

		// can't take all the skills to over load the size on an int
		if (ch->pcdata->points>150)
		{
			ch->println( "There is a limit of 150 creation points..." );
			ch->println( "you must remove other skills/groups before adding any more." );
			return true;
		}

		gn = skillgroup_lookup(argument);
		if (gn != -1)
		{
			if (ch->gen_data->skillgroup_chosen[gn]
				||  ch->pcdata->skillgroup_known[gn])
			{
				ch->printlnf("You already the skillgroup '%s'.",
					skillgroup_table[gn].name);
				return true;
			}

			if (  !IS_SKILLGROUP_SELECTABLE_FOR_CHAR(gn, ch))
			{
				ch->printlnf( "The skillgroup '%s' is not available to you.",
					skillgroup_table[gn].name );
				return true;
			}

			// can't take all the skills to over load the size on an int
			if (ch->pcdata->points + skillgroup_table[gn].rating[ch->clss]>150)
			{
				ch->println( "There is a limit of 150 creation points..." );
				ch->println( "you must remove other skills/groups before adding this group." );
				return true;
			}

			ch->printlnf( "%s skillgroup added.", skillgroup_table[gn].name);

			ch->gen_data->skillgroup_chosen[gn] = true;
			ch->gen_data->points_chosen += skillgroup_table[gn].rating[ch->clss];
			gn_add(ch,gn);
			ch->pcdata->points += skillgroup_table[gn].rating[ch->clss];
			return true;
		}

		sn = skill_lookup(argument);
		if (sn != -1)
		{
			if (ch->gen_data->skill_chosen[sn]
				||  ch->pcdata->learned[sn] > 0)
			{
				ch->println( "You already know that skill!" );
				return true;
			}

/*			if ( !IS_SKILL_VALID_FOR_CLASS(sn, ch->clss)
				||  IS_SPELL(sn))
			{*/
			if ( IS_SPELL(sn))
			{
				ch->println( "That skill is not available." );
				return true;
			}

			// race restrictions
			if( IS_SET(skill_table[sn].flags,SKFLAGS_USE_RACE_RESTRICTIONS)
				&& !IS_SETn(skill_table[sn].race_restrict_n, ch->race))
			{
				ch->println( "That skill is not available to your race." );
				return true;
			}

			if(IS_SET(skill_table[sn].flags,SKFLAGS_NO_GAIN)){
				ch->println( "That skill is not available." );
				return true;
			}

			// can't take all the skills to over load the size on an int
			if (ch->pcdata->points + skillgroup_table[gn].rating[ch->clss]>150)
			{
				ch->println( "There is a limit of 150 creation points..." );
				ch->println( "you must remove other skills/groups before adding this skill." );
				return true;
			}

			ch->printlnf( "%s skill added.", skill_table[sn].name);
			ch->gen_data->skill_chosen[sn] = true;
			ch->gen_data->points_chosen += skill_table[sn].rating[ch->clss];
			ch->pcdata->learned[sn] = 1;
			ch->pcdata->points += skill_table[sn].rating[ch->clss];
			return true;
		}

		ch->println( "No skills or groups by that name..." );
		return true;
    }

    if (!strcmp(arg,"drop"))
    {
		if (argument[0] == '\0')
		{
			ch->println( "You must provide a skill to drop." );
			return true;
		}

		gn = skillgroup_lookup(argument);
		if (gn != -1 && ch->gen_data->skillgroup_chosen[gn])
		{
			ch->println( "Skillgroup dropped." );
			ch->gen_data->skillgroup_chosen[gn] = false;
			ch->gen_data->points_chosen -= skillgroup_table[gn].rating[ch->clss];
			gn_remove(ch,gn);
			// readd all the groups to make sure we don't lose any skills
			for (i = 0; !IS_NULLSTR(skillgroup_table[i].name); i++)
			{
				if (ch->gen_data->skillgroup_chosen[gn]){
					gn_add(ch,gn);
				}
			}
			ch->pcdata->points -= skillgroup_table[gn].rating[ch->clss];
			return true;
		}

		sn = skill_lookup(argument);
		if (sn != -1 && ch->gen_data->skill_chosen[sn])
		{
			ch->println( "Skill dropped." );
			ch->gen_data->skill_chosen[sn] = false;
			ch->gen_data->points_chosen -= skill_table[sn].rating[ch->clss];
			ch->pcdata->learned[sn] = 0;
			ch->pcdata->points -= skill_table[sn].rating[ch->clss];
			return true;
		}

		ch->println( "You haven't bought any such skill or group." );
		return true;
    }

    if (!str_prefix(arg,"premise") || !str_prefix(arg,"explain"))
    {
		do_help(ch,"premise");
		return true;
    }

    if (!str_prefix(arg,"list"))
    {
		ch->titlebar("LIST OF SKILLS AND SKILLSGROUPS STILL AVAILABLE");
        list_group_costs(ch);
        return true;
    }

    if (!str_prefix(arg,"learned"))
    {
		ch->titlebar("LEARNED SKILLS AND SKILLGROUPS");
        list_group_chosen(ch);
        ch->println( "You already have the following skills:" );
        ch->println( "You have these skills as a mixture, of the default skills you get" );
//        ch->println( "with your race and class, plus the others you have added." );
		ch->println( "with your race, plus the others you have added." );
		do_skills(ch,"");
        return true;
    }

    if (!str_prefix(arg,"skillgroups") || !str_prefix(arg,"info"))
    {
		do_skillgroups(ch,argument);
		return true;
    }

    if (!str_prefix(arg,"spinfo"))
    {
		do_spinfo(ch,argument);
        return true;
    }

    if (!str_prefix(arg,"cspinfo"))
    {
		do_cspinfo(ch,argument);
        return true;
    }

    if (!str_prefix(arg,"cskinfo"))
    {
		do_cskinfo(ch,argument);
        return true;
    }

    return false;
}
/**************************************************************************/
// shows all skillgroups, or the sub-members of a skillgroup
void do_skillgroups(char_data *ch, char *argument)
{
    int gn,sn,col;

    if (IS_NPC(ch))
		return;

    col = 0;

    if (argument[0] == '\0')
    {
		ch->titlebar("SKILLGROUPS YOU HAVE");
		for (gn = 0; !IS_NULLSTR(skillgroup_table[gn].name); gn++)
		{
			if (ch->pcdata->skillgroup_known[gn])
			{
				ch->printf( "%-25s ",skillgroup_table[gn].name);

				if (++col % 3 == 0)
					ch->print( "\r\n" );
			}
        }
        if ( col % 3 != 0 )
            ch->print( "\r\n" );
		ch->titlebar("");
		ch->println("To list all skillgroups type `=Cskillgroups all`x.");
		ch->println("To show the skills in a particular skillgroup type `=Cskillgroups <skgrp name>`x.");
		ch->titlebar("");
		return;
	}

	if (!str_cmp(argument,"all"))    // show all skillgroups
	{
		ch->titlebar("ALL SKILLGROUPS");
	    for (gn = 0; !IS_NULLSTR(skillgroup_table[gn].name); gn++)
	    {
			if(!IS_SET(skillgroup_table[gn].flags, SKILLGROUP_CREATION_SELECTABLE)){
				continue;
			}
			if(skillgroup_table[gn].remort){
				ch->printf( "`S%d`x%-24s ", skillgroup_table[gn].remort, skillgroup_table[gn].name );
			}else{
				ch->printf( "%-25s ", skillgroup_table[gn].name );
			}
			if (++col % 3 == 0)
				ch->print( "\r\n" );
        }
        if ( col % 3 != 0 ){
            ch->print( "\r\n" );
		}
		ch->titlebar("");
		return;
	}


	// show the sub-members of a group
	gn = skillgroup_lookup(argument);
	if (gn == -1)
	{
		ch->printlnf( "No skillgroup of the name '%s' exist.", argument );
		ch->println( "Type 'skillgroups all' for a full listing." );
		return;
	}

	ch->titlebarf("SKILLS WITHIN THE SKILLGROUP '%s'", uppercase(skillgroup_table[gn].name));
	for (sn = 0; skillgroup_table[gn].skills[sn]>-1; sn++)
	{
		ch->printf( "%-25s ", skill_table[skillgroup_table[gn].skills[sn]].name);

		if (++col % 3 == 0)
			ch->print( "\r\n" );
	}
    if ( col % 3 != 0 )
        ch->print( "\r\n" );
	ch->titlebar("");
}

/**************************************************************************/
// new check improve system - Kal
// trial code, not really completed yet.
void new_check_improve( char_data *ch, int sn, bool success, int multiplier)
{
	// standard situations where you can't improve
    if (IS_NPC(ch) || sn==-1 || IS_OOC(ch) || multiplier<1) {
		logf("1start %d\r\n",multiplier);
		return;
	}

	// can't improve by spamming
/*#ifndef WIN32
	if (ch->desc && ch->desc->repeat>5){
		return;
	}
#endif*/

	// store the perthousand improve table
	static short improve_chance_lookup_table[102]; // stored in range 1 ->10000
	static bool initialise_table=true;
	if(initialise_table){ // calculate the improvement chances once, first time
		int i;
		double f;
		for(i=0; i<=100; i++){
//			f=sqrt((100.0-i)/100);
			f=(100.0-i)/100;
			f=pow(100.0,f);
			f*=100; // put in 1->10000 range - increased precision
			improve_chance_lookup_table[i]=(short)f;
			//logf("improve_chance_lookup_table[%d]=%d", i, improve_chance_lookup_table[i]);
		}
		improve_chance_lookup_table[101]=1;
		initialise_table=false;
	}

	int maxlearn=skill_table[sn].learn_scale_percent[ch->clss];
	if(maxlearn<1){
		maxlearn=100;
	}

    if ( (ch->level < skill_table[sn].skill_level[ch->clss])
	  	||  skill_table[sn].skill_level[ch->clss]==0
		||  skill_table[sn].rating[ch->clss] == 0
		||  ch->pcdata->learned[sn] <2 // 2% required - must prac at least once to improve
		||  ch->pcdata->learned[sn] >=maxlearn)
	{
		return;  // skill is not known, or past the point of improvement
	}

	if (number_range(1,4)!=1){ // make things harder
		new_check_improve( ch, sn, success, multiplier-1);
		return;
	}

    // check to see if the character has a chance to learn
    int chance= 8 * ch->modifiers[STAT_ME] + 2*ch->modifiers[STAT_IN];
    chance /= (		multiplier
		*	skill_table[sn].rating[ch->clss]
		*	4);
    chance += ch->level*3/4;

    if (number_range(1,1000) > chance){
		// try again
		new_check_improve( ch, sn, success, multiplier-1);
		return;
	}

    // now that the character has a CHANCE to learn, see if they really have

	chance=improve_chance_lookup_table[ch->pcdata->learned[sn]];
	int max_percent=UMAX(maxlearn,
						skill_table[sn].maxprac_percent[ch->clss]);
	if(max_percent>=0 && max_percent<=100){
		chance=chance* max_percent/100;
		if(chance<1){
			chance=1;
		}
	}

	if (!success){
		chance/=2;
		chance = URANGE(1,chance,2500);
	}
	if (number_range(1,10000)> chance){
		// try again
		new_check_improve( ch, sn, success, multiplier-1);
		return;
	}

    int skillcap, s3n, skillpoints, skillcap2;
    skillcap = ((ch->modifiers[STAT_ME] * 5) + 30) * ((ch->level) + (race_table[ch->race]->stat_modifier[STAT_ME]));
    skillcap = 2 * skillcap;
    skillcap2 = ((3 * 5) + 30) * ((ch->level) + (3));
    skillcap2 = 3 * skillcap2;
    if (skillcap <= skillcap2){
    skillcap = skillcap2;
    }
    skillpoints = 0;
    for ( s3n = 0; s3n < MAX_SKILL; s3n++ )
    {

// skill values
	if ( !IS_NULLSTR(skill_table[s3n].name) && ch->pcdata->learned[s3n] > 0 )
	{
        skillpoints += ch->pcdata->learned[s3n];

        if (skill_table[s3n].sktype==SKTYPE_GEM_ONLY){
        skillpoints -= ch->pcdata->learned[s3n];}

        if (skill_table[s3n].sktype==SKTYPE_MOB_SKILL){
        skillpoints -= ch->pcdata->learned[s3n];}

        if (skill_table[s3n].category!=CAT_LANGUAGE){
		skillpoints -= ch->pcdata->learned[s3n];}
	}


    }
    if (skillpoints>=skillcap){
        return;}

	if (success){
		ch->printlnf("*You have become better at `Y%s`x!",
			skill_table[sn].name);
		ch->pcdata->learned[sn]++;
    }else{
		ch->printlnf("*You learn from your mistakes, and your `Y%s`x improves.",
				skill_table[sn].name);
		ch->pcdata->learned[sn] += number_range(1,3);
		ch->pcdata->learned[sn] = (unsigned char)UMIN(ch->pcdata->learned[sn],skill_table[sn].learn_scale_percent[ch->clss]);
    }
	ch->printlnf("*Your `Y%s`x is now at %d%%!!",
			skill_table[sn].name, ch->pcdata->learned[sn]);

	// calc how far they can prac it
	int maxprac=skill_table[sn].maxprac_percent[ch->clss]?
					skill_table[sn].maxprac_percent[ch->clss]:

					50;

	if(!(ch->level>70 && ch->pcdata->learned[sn]<maxprac)){
		gain_exp(ch,2 * skill_table[sn].rating[ch->clss]);

	}
}
/**************************************************************************/
// checks for skill improvement
void check_improve( char_data *ch, int sn, bool success, int multiplier )
{
	if(IS_SET(skill_table[sn].flags,SKFLAGS_NEW_IMPROVE_SYSTEM)
		|| HAS_CONFIG(ch,CONFIG_PRACSYS_TESTER))
	{
		new_check_improve(ch, sn, success, multiplier);
		return;
	}

    if (IS_NPC(ch))
		return;

	if (sn==-1){
		return;
	}

	// can't improve in ooc rooms
    if (IS_OOC(ch))
		return;

	// can't improve by spamming
	if (ch->desc && ch->desc->repeat>5)
		return;

    if (ch->pcdata->learned[sn] <1	||  ch->pcdata->learned[sn] >99)
		return;  // skill is not known or is mastered

    // check to see if the character has a chance to learn
    int chance= 8 * ch->modifiers[STAT_ME] + 2*ch->modifiers[STAT_IN];
    chance /= (		multiplier
		*	skill_table[sn].rating[ch->clss]

		*	4);
//    chance += ch->level;

    if (number_range(1,1000) > chance)
		return;

    int skillcap, s3n, skillpoints, skillcap2;
    skillcap = ((ch->modifiers[STAT_ME] * 5) + 30) * ((ch->level) + (race_table[ch->race]->stat_modifier[STAT_ME]));
    skillcap = 2 * skillcap;
    skillcap2 = ((3 * 5) + 30) * ((ch->level) + (3));
    skillcap2 = 3 * skillcap2;
    if (skillcap <= skillcap2){
    skillcap = skillcap2;
    }
    skillpoints = 0;
    for ( s3n = 0; s3n < MAX_SKILL; s3n++ )
    {
// skill values
	if ( !IS_NULLSTR(skill_table[s3n].name) && ch->pcdata->learned[s3n] > 0 )
	{
        skillpoints += ch->pcdata->learned[s3n];

        if (skill_table[s3n].sktype==SKTYPE_GEM_ONLY){
        skillpoints -= ch->pcdata->learned[s3n];}

        if (skill_table[s3n].sktype==SKTYPE_MOB_SKILL){
        skillpoints -= ch->pcdata->learned[s3n];}

        if (skill_table[s3n].category==CAT_LANGUAGE){
		skillpoints -= ch->pcdata->learned[s3n];}
	}

    }
    if (skillpoints>=skillcap){
        return;}

    // now that the character has a CHANCE to learn, see if they really have
	if (success){
		chance = URANGE(5,100 - ch->pcdata->learned[sn], 95);
		if (number_percent() < chance)
		{
			ch->printlnf("You have become better at `Y%s`x!", skill_table[sn].name);
			ch->pcdata->learned[sn]++;
			if(!(ch->level>75 && ch->pcdata->learned[sn]<75)){
				gain_exp(ch,2 * skill_table[sn].rating[ch->clss]);

			}
		}
    }else{
		chance = URANGE(5,ch->pcdata->learned[sn]/2,30);
		if (number_percent() < chance)
		{
			ch->printf(
				"You learn from your mistakes, and your `Y%s`x skill improves.\r\n",
				skill_table[sn].name);
			ch->pcdata->learned[sn] += number_range(1,3);
			ch->pcdata->learned[sn] = UMIN(ch->pcdata->learned[sn],100);
			if(!(ch->level>70 && ch->pcdata->learned[sn]<75)){
				gain_exp(ch,2 * skill_table[sn].rating[ch->clss]);

			}
		}
    }
}
/**************************************************************************/
// returns a skillgroup index number given the exact name
int skillgroup_exact_lookup( const char *name )
{
    for( int gn = 0; !IS_NULLSTR(skillgroup_table[gn].name); gn++ )
    {
        if ( !str_cmp( name, skillgroup_table[gn].name ) ){
            return gn;
		}
    }
	return -1;
}
/**************************************************************************/
// returns a skillgroup index number given the name
int skillgroup_lookup( const char *name )
{
    int gn;

	// perform exact match first
	gn=skillgroup_exact_lookup(name);
	if(gn>-1){
		return gn;
	}

	// perform prefix match
    for( gn = 0; !IS_NULLSTR(skillgroup_table[gn].name); gn++ )
    {
        if ( LOWER(name[0]) == LOWER(skillgroup_table[gn].name[0])
			&& !str_prefix( name, skillgroup_table[gn].name ) )
            return gn;
    }

    return -1;
}
/**************************************************************************/
// recursively adds a group given its number -- uses group_add
void gn_add( char_data *ch, int gn)
{
    int i;
	if(IS_NPC(ch)){
		return;
	}

	/*if(!IS_SKILLGROUP_SELECTABLE_FOR_CHAR(gn, ch)){
		return;
	}*/

    ch->pcdata->skillgroup_known[gn] = true;

	for (i = 0; skillgroup_table[gn].skills[i]>-1; i++)
	{
		/*if(IS_SKILL_VALID_FOR_CLASS(skillgroup_table[gn].skills[i], ch->clss)){*/
			group_add(ch,skill_table[skillgroup_table[gn].skills[i]].name,false, skillgroup_table[gn].skillsvalue[i]);
		/*}*/
    }
}
/**************************************************************************/
// recursively removes a group given its number -- uses group_remove
void gn_remove( char_data *ch, int gn)
{
    int i;

    ch->pcdata->skillgroup_known[gn] = false;

	for (i = 0; skillgroup_table[gn].skills[i]>-1; i++)
	{
		/*if(IS_SKILL_VALID_FOR_CLASS(skillgroup_table[gn].skills[i], ch->clss)){
			group_remove(ch,skill_table[skillgroup_table[gn].skills[i]].name);
		}*/
		group_remove(ch,skill_table[skillgroup_table[gn].skills[i]].name);
    }
}
/**************************************************************************/
// use for processing a skill or group for addition
void group_add( char_data *ch, const char *name, bool deduct, int percent)
{
    int sn,gn;

    if (IS_NPC(ch)){ // NPCs do not have skills
		return;
	}

	if(percent<0 || percent>101){ // safety check
		return;
	}

    sn = skill_exact_lookup(name);

    if (sn != -1)
    {
		if (ch->pcdata->learned[sn] < percent){
			ch->pcdata->learned[sn] = percent;
			if (deduct){
				ch->pcdata->points += skill_table[sn].rating[ch->clss];
			}
		}
		return;
    }

	// if there wasn't a skill by the name, check for a group
	gn = skillgroup_lookup(name);

    if (gn != -1)
    {
		if (ch->pcdata->skillgroup_known[gn] == false)
		{
			ch->pcdata->skillgroup_known[gn] = true;
			if (deduct)
				ch->pcdata->points += skillgroup_table[gn].rating[ch->clss];

		}
		gn_add(ch,gn); // make sure all skills in the group are known
    }
}
/**************************************************************************/
// used for processing a skill or group for deletion -- no points back!
void group_remove(char_data *ch, const char *name)
{
    int sn, gn;

	sn = skill_lookup(name);

    if (sn != -1)
    {
		ch->pcdata->learned[sn] = 0;
		return;
    }

    // now check skillgroups

    gn = skillgroup_lookup(name);

    if (gn != -1 && ch->pcdata->skillgroup_known[gn] == true)
    {
		ch->pcdata->skillgroup_known[gn] = false;
		gn_remove(ch,gn);  // be sure to call gn_add on all remaining skillgroups
    }
}

/**************************************************************************/
// Jarren - June 1999
void do_vault( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	char arg[MIL];
	char_data *victim;
	int chance, accuracy;

	one_argument(argument,arg);

	if ( (chance = get_skill(ch,gsn_vault)) == 0
		||      (!IS_NPC(ch)
		&&       ch->level < skill_table[gsn_vault].skill_level[ch->clss]))
	{
		if (!IS_CONTROLLED(ch))
		{
			ch->println( "Vault? What's that?" );
			return;
		}
	}

	if (arg[0] == '\0')
	{
		victim = ch->fighting;
		if (!victim){
			ch->println( "But you aren't fighting anyone!" );
			return;
		}
	}else if ((victim = get_char_room(ch,arg)) == NULL){
		ch->println( "They aren't here." );
		return;
	}

	if (victim->position < POS_FIGHTING)
	{
		act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
		return;
	}

	if (victim == ch){
		ch->println( "You try to pole vault onto yourself but miss horribly." );
		return;
	}

	if (is_safe(ch,victim))
		return;

	if ( !can_initiate_combat( ch, victim, 1 )) return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim){
		act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
		return;
    }

    // modifiers

    // size and weight
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
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE)){
		chance += 10;
    }
	if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE)){
		chance -= 30;
	}

    // level
    chance += (ch->level - victim->level);

    if (IS_CONTROLLED(ch)){
        chance*=3;
    };

    if (!IS_NPC(victim) && chance < get_skill(victim,gsn_dodge) )
		   chance -= 3 * (get_skill(victim,gsn_dodge) - chance);

    // now the attack
    if (number_percent() < chance )
    {

		accuracy = 100 - get_skill(victim,gsn_vault);
		accuracy -= ch->modifiers[STAT_PR]/10;
		accuracy -= ch->modifiers[STAT_AG]/10;
		accuracy += victim->modifiers[STAT_QU]/9;
		if(ch->pcdata){
			accuracy += ch->pcdata->tired/2;
		}

		if(accuracy < number_range(1,8))
		{
			act("$n pole vaults at you delivering a powerfull blow to your head!",ch,NULL,NULL,TO_VICT);
			act("You pole vault at $n delivering a powerfull blow to $m head!",victim,NULL,NULL,TO_CHAR);
			act("$n pole vaults at $N delivering a powerfull blow to $M head.",ch,NULL,victim,TO_NOTVICT);
			accuracy = 2;
		}else{
			act("$n pole vaults at you delivering a crushing blow to your abdomen!",ch,NULL,NULL,TO_VICT);
			act("You pole vault at $n delivering a crushing blow to $m abdomen!",victim,NULL,NULL,TO_CHAR);
			act("$n pole vaults at $N delivering a crushing blow to $M abdomen.",ch,NULL,victim,TO_NOTVICT);
			accuracy = 1;
		}

		check_improve(ch,gsn_vault,true,1);

		DAZE_STATE(victim, 3 * PULSE_VIOLENCE);
		WAIT_STATE(ch,skill_table[gsn_vault].beats);
		victim->position = POS_RESTING;
		damage(ch,victim,number_range(2,2 + accuracy * ch->size + chance/10),
			gsn_vault,	DAM_BASH,false);

    }else{
		damage(ch,victim,0,gsn_vault,DAM_BASH,false);
		act("You try to pole vault at $n but miss and fly right past $m!", victim,NULL,NULL,TO_CHAR);
		act("$n tries to pole vault at $N but flies right past $M.", ch,NULL,victim,TO_NOTVICT);
		act("You evade $n's pole vault, causing $m to fly right past you.", ch,NULL,NULL,TO_VICT);
		check_improve(ch,gsn_vault,false,1);
		WAIT_STATE(ch,skill_table[gsn_vault].beats * 3/2);
    }
}
/**************************************************************************/
/*               Psionic             Begin             Here               */
/**************************************************************************/
/* Self Target First*/
/**************************************************************************/
void levitate( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_LEVITATE)){
	REMOVE_BIT(ch->psionic_affects, PAFF_LEVITATE);
	ch->println("Your levitation ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_levitation ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your levitation attempt fails!");
		check_improve(ch, gsn_levitation, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_LEVITATE);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus allows you to rise off the ground!");
	check_improve(ch, gsn_levitation, true, 1);

}
/**************************************************************************/
void mind_over_body( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_MOVERB)){
	REMOVE_BIT(ch->psionic_affects, PAFF_MOVERB);
	ch->println("Your mental control ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_mind_over_body ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your mind over body attempt fails!");
		check_improve(ch, gsn_mind_over_body, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_MOVERB);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus allows you to control your body's needs!");
	check_improve(ch, gsn_mind_over_body, true, 1);

}
/**************************************************************************/
void psishield( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_PSISHIELD)){
	REMOVE_BIT(ch->psionic_affects, PAFF_PSISHIELD);
	ch->println("Your psionic shielding ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_psi_shield ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your psi shield attempt fails!");
		check_improve(ch, gsn_psi_shield, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_PSISHIELD);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus creates a psionic barrier!");
	check_improve(ch, gsn_psi_shield, true, 1);

}
/**************************************************************************/
void nettles( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_NETTLES)){
	REMOVE_BIT(ch->psionic_affects, PAFF_NETTLES);
	ch->println("Your psionic nettles fade.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_nettles ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your attempt to guard your mind with psionic nettles fails!");
		check_improve(ch, gsn_nettles, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_NETTLES);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus protects your mind with a barrier of nettles!");
	check_improve(ch, gsn_nettles, true, 1);

}
/**************************************************************************/
void spikes( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_SPIKES)){
	REMOVE_BIT(ch->psionic_affects, PAFF_SPIKES);
	ch->println("Your psionic spikes fade.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_spikes ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your attempt to guard your mind with psionic spikes fails!");
		check_improve(ch, gsn_spikes, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_SPIKES);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus protects your mind with a barrier of spikes!");
	check_improve(ch, gsn_spikes, true, 1);

}
/**************************************************************************/
void blur( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_BLUR)){
	REMOVE_BIT(ch->psionic_affects, PAFF_BLUR);
	ch->println("Your blurring ability ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_blur ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your blur attempt fails!");
		check_improve(ch, gsn_blur, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_BLUR);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus allows you to blur your form!");
	check_improve(ch, gsn_blur, true, 1);

}
/**************************************************************************/
void awareness( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_AWARENESS)){
	REMOVE_BIT(ch->psionic_affects, PAFF_AWARENESS);
	ch->println("Your awareness ability ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_psiawareness ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your awareness attempt fails!");
		check_improve(ch, gsn_psiawareness, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_AWARENESS);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus makes you more aware of your sroundings!");
	check_improve(ch, gsn_psiawareness, true, 1);

}
/**************************************************************************/
void tkshield( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_TKSHIELD)){
	REMOVE_BIT(ch->psionic_affects, PAFF_TKSHIELD);
	ch->println("Your tk shield ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_tk_shield ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your tk shield attempt fails!");
		check_improve(ch, gsn_tk_shield, false, 1);
		return;}


	SET_BIT(ch->psionic_affects, PAFF_TKSHIELD);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus provides a protective barrier!");
	check_improve(ch, gsn_tk_shield, true, 1);

}
/**************************************************************************/
void antimagic( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_ANTIMAGIC)){
	REMOVE_BIT(ch->psionic_affects, PAFF_ANTIMAGIC);
	ch->println("Your antimagic barrier ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_antimagic ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your antimagic barrier attempt fails!");
		check_improve(ch, gsn_antimagic, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_ANTIMAGIC);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus shields you from magic!");
	check_improve(ch, gsn_antimagic, true, 1);

}
/**************************************************************************/
void adrenaline( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_ADRENCONTROL)){
	REMOVE_BIT(ch->psionic_affects, PAFF_ADRENCONTROL);
	ch->println("Your adrenaline control ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_adrenaline_control ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your attempt to control your adrenaline fails!");
		check_improve(ch, gsn_adrenaline_control, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_ADRENCONTROL);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus gives you control of your adrenaline!");
	check_improve(ch, gsn_adrenaline_control, true, 1);

}
/**************************************************************************/
void mindscan( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_SCANNING)){
	REMOVE_BIT(ch->psionic_affects, PAFF_SCANNING);
	ch->println("Your mindscan ability ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_mindscan ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your mindscan attempt fails!");
		check_improve(ch, gsn_mindscan, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_SCANNING);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus allows you to scan for thoughts!");
	check_improve(ch, gsn_mindscan, true, 1);

}
/**************************************************************************/
void fastheal( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_FASTHEAL)){
	REMOVE_BIT(ch->psionic_affects, PAFF_FASTHEAL);
	ch->println("Your fast healing ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_psifast_healing ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your fast healing attempt fails!");
		check_improve(ch, gsn_psifast_healing, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_FASTHEAL);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus allows you to heal faster!");
	check_improve(ch, gsn_psifast_healing, true, 1);

}
/**************************************************************************/
void mindshield( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_MINDSHIELD)){
	REMOVE_BIT(ch->psionic_affects, PAFF_MINDSHIELD);
	ch->println("Your mind shield ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_mind_shield ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your mind shield attempt fails!");
		check_improve(ch, gsn_mind_shield, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_MINDSHIELD);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus allows you to shield your mind!");
	check_improve(ch, gsn_mind_shield, true, 1);

}
/**************************************************************************/
void dangersense( char_data *ch)
{
if (IS_SET(ch->psionic_affects, PAFF_DANGERSENSE)){
	REMOVE_BIT(ch->psionic_affects, PAFF_DANGERSENSE);
	ch->println("Your danger sense ceases.");
	return;}

	int chance;
	chance = get_skill( ch, gsn_danger_sense ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	if (ch->psi < (psipower/5)){
		ch->println("Your psionic power is too low for this ability!");
		return;}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/10);
		ch->println("Your danger sense attempt fails!");
		check_improve(ch, gsn_danger_sense, false, 1);
		return;}

	SET_BIT(ch->psionic_affects, PAFF_DANGERSENSE);
	ch->psi -= (psipower/5);
	ch->println("Your mental focus allows you to become aware to danger!");
	check_improve(ch, gsn_danger_sense, true, 1);

}
/**************************************************************************/
void enfuse( char_data *ch, char *argument)
{
	char arg1[MIL];
	char_data *victim;

	argument = one_argument( argument, arg1 );

	if(IS_NULLSTR(arg1)){
		ch->println("Who do you wish to lend your psionic power to?");
		return;
	}
	else if ((victim = get_char_room(ch,arg1)) == NULL){
		ch->printlnf("'%s' is not here.", arg1);
		return;}
	if (victim == ch){
		ch->println("That would be pointless.");
		return;}
	int chance;
	chance = get_skill( ch, gsn_enfuse ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);

//	if (victim != ch && !is_safe(ch,victim)){
	if (!IS_NPC(victim)){
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= (psipower/4);
		ch->println("Your enfusion attempt fails!");
		check_improve(ch, gsn_enfuse, false, 1);
		return;}

	if (IS_SET(victim->psionic_affects, PAFF_NETTLES)){
		ch->printlnf("%s's psychic nettles sting your mind!", victim->name);
		victim->printlnf("%s is stung by your psychic nettles.", ch->name);
		ch->hit -= 2;
		ch->psi -= 1;
		check_improve(ch, gsn_nettles, true, 3);}
	if (IS_SET(victim->psionic_affects, PAFF_SPIKES)){
		ch->printlnf("%s's psychic spikes stab your mind!", victim->name);
		victim->printlnf("%s is stabbed by your psychic spikes.", ch->name);
		ch->hit -= 10;
		ch->psi -= 5;
		check_improve(victim, gsn_spikes, true, 3);}
	if (ch->psi <= (psipower/2)){
		ch->println("Your concentration on the enfusion attempt is broken!");
		return;}
	int enfuse;
	enfuse = (psipower/2);

	if(IS_SET(victim->imm_flags, IMM_MENTAL)){
		ch->printlnf("%s is unnaffected and the power is wasted!.", victim->name);
	if (IS_SET(victim->res_flags, RES_MENTAL)){
		enfuse = (enfuse/2);}
		ch->psi -= enfuse;
		victim->psi += enfuse;
		ch->printlnf("You enfuse %s with psionic power.", victim->name);
		check_improve(ch, gsn_enfuse, true, 1);
		victim->printlnf("%s enfuses you with psionic power!", ch->name);}
	if (victim->psi >= victim->max_psi){
		victim->psi = victim->max_psi;}
	}else{
	ch->println("This can not be used on mobs.");
	return;}
}
/**************************************************************************/
void stun( char_data *ch, char *argument)
{
	char arg1[MIL];
	char_data *victim;

	argument = one_argument( argument, arg1 );

	if(IS_NULLSTR(arg1)){
		victim = ch->fighting;
		if (victim == NULL){
			ch->println("But you aren't fighting anyone!");
			return;}
	}
	else if ((victim = get_char_room(ch,arg1)) == NULL){
		ch->printlnf("'%s' is not here.", arg1);
		return;}

	int chance;
	chance = get_skill( ch, gsn_stun ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}

	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
//	if (victim != ch && !is_safe(ch,victim)){
	if (!is_safe(ch,victim)){
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->println("Your stun blast fails!");
		ch->psi -= (psipower/4);
		check_improve(ch, gsn_stun, false, 1);
		return;}
	if (IS_SET(victim->psionic_affects, PAFF_NETTLES)){
		ch->printlnf("%s's psychic nettles sting your mind!", victim->name);
		victim->printlnf("%s is stung by your psychic nettles.", ch->name);
		ch->hit -= 2;
		ch->psi -= 1;
		check_improve(victim, gsn_nettles, true, 3);}
	if (IS_SET(victim->psionic_affects, PAFF_SPIKES)){
		ch->printlnf("%s's psychic spikes stab your mind!", victim->name);
		victim->printlnf("%s is stabbed by your psychic spikes.", ch->name);
		ch->hit -= 10;
		ch->psi -= 5;
		check_improve(victim, gsn_spikes, true, 3);}
	if (ch->psi <= (psipower/2)){
		ch->println("Your concentration on the psionic stun attempt is broken!");
		return;}

	int psidmg = (psipower/2);

	if(IS_SET(victim->imm_flags, IMM_MENTAL)){
		psidmg=0;}
	if (IS_SET(victim->res_flags, RES_MENTAL)){
		psidmg = (psidmg/2);}
	if (IS_SET(victim->vuln_flags, VULN_MENTAL)){
		psidmg = (psidmg * 2);}
	if (IS_SET(victim->psionic_affects, PAFF_PSISHIELD)){
		psidmg = (psidmg/2);

	DAZE_STATE(victim,UMAX(12,ch->level/4 + psipower/20));
		ch->psi -= (psipower/2);
		victim->psi -= psidmg;
		victim->hit -= psidmg;
		ch->printlnf("You stun %s with a blast of psionic energy.", victim->name);
		check_improve(ch, gsn_stun, true, 1);
		victim->printlnf("%s stuns you with a blast of psionic energy!", ch->name);
		victim->fighting =ch;
		ch->fighting = victim;}
}
}
/**************************************************************************/
void pyrok( char_data *ch, char *argument)
{
	char arg1[MIL];
	char_data *victim;

	argument = one_argument( argument, arg1 );

	if(IS_NULLSTR(arg1)){
		victim = ch->fighting;
		if (victim == NULL){
			ch->println("But you aren't fighting anyone!");
			return;}
	}
	else if ((victim = get_char_room(ch,arg1)) == NULL){
		ch->printlnf("'%s' is not here.", arg1);
		return;}
	int chance;
	chance = get_skill( ch, gsn_pyrokinetics ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}


//	if (victim != ch && !is_safe(ch,victim)){
	if (!is_safe(ch,victim)){
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= 25;
		ch->println("Your pyrokinetic power fails!");
		check_improve(ch, gsn_pyrokinetics, false, 1);
		return;}
	ch->psi -= 50;
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	int pyrodmg = (psipower/2);
	int psidmg = psipower;

	if(IS_SET(victim->imm_flags, IMM_MENTAL)){
		psidmg=0;}
	if (IS_SET(victim->res_flags, RES_MENTAL)){
		psidmg = (psidmg/2);}
	if (IS_SET(victim->vuln_flags, VULN_MENTAL)){
		psidmg = (psidmg * 2);}
	if(IS_SET(victim->imm_flags, IMM_FIRE)){
		pyrodmg=0;}
	if (IS_SET(victim->res_flags, RES_FIRE)){
		pyrodmg = (pyrodmg/2);}
	if (IS_SET(victim->vuln_flags, VULN_FIRE)){
		pyrodmg = (pyrodmg * 2);}
	if (IS_SET(victim->psionic_affects, PAFF_PSISHIELD)){
		psidmg = (psidmg/2);
		pyrodmg = (pyrodmg/2);}

		victim->psi -= psidmg;
		victim->hit -= (psidmg + pyrodmg);
		ch->printlnf("You scorch %s with a pyrokinetic blast.", victim->name);
		check_improve(ch, gsn_pyrokinetics, true, 1);
		victim->printlnf("%s scorches you with a pyrokinetic blast!", ch->name);
		victim->fighting =ch;
		ch->fighting = victim;}

}
/**************************************************************************/
void disruption( char_data *ch, char *argument)
{
	char arg1[MIL];
	char_data *victim;

	argument = one_argument( argument, arg1 );

	if(IS_NULLSTR(arg1)){
		victim = ch->fighting;
		if (victim == NULL){
			ch->println("But you aren't fighting anyone!");
			return;}
	}
	else if ((victim = get_char_room(ch,arg1)) == NULL){
		ch->printlnf("'%s' is not here.", arg1);
		return;}
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	int chance;
	chance = get_skill( ch, gsn_disruption ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}


//	if (victim != ch && !is_safe(ch,victim)){
	if (!is_safe(ch,victim)){
	if (IS_SET(victim->psionic_affects, PAFF_PSISHIELD)){
		chance = (chance/2);}
	if (chance <= 4){
		chance = 5;}

	if (number_percent() >= (chance + 1)){
		ch->println("Your psionic disruption fails!");
		check_improve(ch, gsn_disruption, false, 1);
		ch->psi -= (psipower/4);
			return;}

	if (IS_SET(victim->psionic_affects, PAFF_NETTLES)){
		ch->printlnf("%s's psychic nettles sting your mind!", victim->name);
		victim->printlnf("%s is stung by your psychic nettles.", ch->name);
		ch->hit -= 2;
		ch->psi -= 1;
		check_improve(victim, gsn_nettles, true, 3);}
	if (IS_SET(victim->psionic_affects, PAFF_SPIKES)){
		ch->printlnf("%s's psychic spikes stab your mind!", victim->name);
		victim->printlnf("%s is stabbed by your psychic spikes.", ch->name);
		ch->hit -= 10;
		ch->psi -= 5;
		check_improve(victim, gsn_spikes, true, 3);}
	if (ch->psi <= (psipower/2)){
		ch->println("Your concentration on the disruption attempt is broken!");
		return;}
	if(IS_SET(victim->imm_flags, IMM_MENTAL)){
		ch->printlnf("%s is unnaffected!.", victim->name);
		victim->printlnf("You are unnaffected by %s's disruption attempt!.", ch->name);
		victim->fighting =ch;
		ch->fighting =victim;
		return;}
int sn;
sn = skill_lookup("disruption");
		ch->psi -= (psipower/2);
		ch->printlnf("You disrupt %s's psionic control.", victim->name);
		victim->printlnf("%s disrupts your psionic control!", ch->name);
		check_improve(ch, gsn_disruption, true, 1);
		//SET_BIT(victim->psionic_affects, PAFF_DISRUPTION);
		AFFECT_DATA af;
		af.where     = WHERE_PSIAFF;
		af.type      = sn;
		af.level     = ch->level;
		af.duration  = psipower;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = PAFF_DISRUPTION;
		affect_to_char( victim, &af );

		if (IS_SET(victim->psionic_affects, PAFF_MOVERB)){
			REMOVE_BIT(victim->psionic_affects, PAFF_MOVERB);
			victim->println("Your mental control ceases.");}
		if (IS_SET(victim->psionic_affects, PAFF_PSISHIELD)){
			REMOVE_BIT(victim->psionic_affects, PAFF_PSISHIELD);
			victim->println("Your psionic shielding ceases.");}
		if (IS_SET(victim->psionic_affects, PAFF_ADRENCONTROL)){
			REMOVE_BIT(victim->psionic_affects, PAFF_ADRENCONTROL);
			victim->println("Your adrenaline control ceases.");}
	victim->fighting =ch;
	ch->fighting =victim;}
}
/**************************************************************************/
void psidrain( char_data *ch, char *argument)
{
	char arg1[MIL];
	char_data *victim;

	argument = one_argument( argument, arg1 );

	if(IS_NULLSTR(arg1)){
		victim = ch->fighting;
		if (victim == NULL){
			ch->println("But you aren't fighting anyone!");
			return;}
	}
	else if ((victim = get_char_room(ch,arg1)) == NULL){
		ch->printlnf("'%s' is not here.", arg1);
		return;}
	if (victim == ch){
		ch->println("That would be pointless.");
		return;}
	if (IS_NPC(victim)){
		ch->println("This can not be used on mobs.");
		return;}
	int chance;
	chance = get_skill( ch, gsn_psi_drain ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}


//	if (victim != ch && !is_safe(ch,victim)){
	if (!is_safe(ch,victim)){
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->println("Your psi drain fails!");
		check_improve(ch, gsn_psi_drain, false, 1);
			return;}

	if (IS_SET(victim->psionic_affects, PAFF_NETTLES)){
		ch->printlnf("%s's psychic nettles sting your mind!", victim->name);
		victim->printlnf("%s is stung by your psychic nettles.", ch->name);
		ch->hit -= 2;
		ch->psi -= 1;
		check_improve(victim, gsn_nettles, true, 3);}
	if (IS_SET(victim->psionic_affects, PAFF_SPIKES)){
		ch->printlnf("%s's psychic spikes stab your mind!", victim->name);
		victim->printlnf("%s is stabbed by your psychic spikes.", ch->name);
		ch->hit -= 10;
		ch->psi -= 5;
		check_improve(victim, gsn_spikes, true, 3);}
	int drain;
	drain = (victim->psi/2);
	if(IS_SET(victim->imm_flags, IMM_MENTAL)){
		drain=0;}
	if (IS_SET(victim->res_flags, RES_MENTAL)){
		drain = (drain/2);}
/*	if (IS_SET(victim->vuln_flags, VULN_MENTAL)){
		drain = (drain * 2);}*/
	if (IS_SET(victim->psionic_affects, PAFF_PSISHIELD)){
		drain = (drain/2);}
	if (victim->psi >= 2){
		ch->psi += drain;
		victim->psi -= drain;
		ch->printlnf("You drain psionic power from %s.", victim->name);
		victim->printlnf("%s drains your psionic power!", ch->name);}
	else{
		ch->printlnf("%s hasn't got enough psionic power to drain.", victim->name);}
	if (ch->psi >= ch->max_psi){
	ch->psi = ch->max_psi;}
	check_improve(ch, gsn_psi_drain, true, 1);
	victim->fighting =ch;
	ch->fighting =victim;}
}
/**************************************************************************/
void psibolt( char_data *ch, char *argument)
{
	char arg1[MIL];
	char_data *victim;

	argument = one_argument( argument, arg1 );

	if(IS_NULLSTR(arg1)){
		victim = ch->fighting;
		if (victim == NULL){
			ch->println("But you aren't fighting anyone!");
			return;}
	}
	else if ((victim = get_char_room(ch,arg1)) == NULL){
		ch->printlnf("'%s' is not here.", arg1);
		return;}
	int chance;
	chance = get_skill( ch, gsn_psi_bolt ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}


//	if (victim != ch && !is_safe(ch,victim)){
	if (!is_safe(ch,victim)){
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= 25;
		ch->println("Your psi bolt fails!");
		check_improve(ch, gsn_psi_bolt, false, 1);
			return;}
	ch->psi -= 50;
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	int psidmg = psipower;

	if(IS_SET(victim->imm_flags, IMM_MENTAL)){
		psidmg=0;}
	if (IS_SET(victim->res_flags, RES_MENTAL)){
		psidmg = (psidmg/2);}
	if (IS_SET(victim->vuln_flags, VULN_MENTAL)){
		psidmg = (psidmg * 2);}
	if (IS_SET(victim->psionic_affects, PAFF_PSISHIELD)){
		psidmg = (psidmg/2);

		victim->psi -= psidmg;
		victim->hit -= psidmg;
		ch->printlnf("You strike %s with a bolt of psionic energy.", victim->name);
		victim->printlnf("%s strikes you with a bolt of psionic energy!", ch->name);
		check_improve(ch, gsn_psi_bolt, true, 1);
		victim->fighting =ch;
		ch->fighting = victim;}
}
}
/**************************************************************************/
void cryokin( char_data *ch, char *argument)
{
	char arg1[MIL];
	char_data *victim;

	argument = one_argument( argument, arg1 );

	if(IS_NULLSTR(arg1)){
		victim = ch->fighting;
		if (victim == NULL){
			ch->println("But you aren't fighting anyone!");
			return;}
	}
	else if ((victim = get_char_room(ch,arg1)) == NULL){
		ch->printlnf("'%s' is not here.", arg1);
		return;}

	int chance;
	chance = get_skill( ch, gsn_cryokinetics ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}


//	if (victim != ch && !is_safe(ch,victim)){
	if (!is_safe(ch,victim)){
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= 25;
		ch->println("Your cryokinetic power fails!");
		check_improve(ch, gsn_cryokinetics, false, 1);
			return;}
	ch->psi -= 50;
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	int cryodmg = (psipower/2);
	int psidmg = psipower;

	if(IS_SET(victim->imm_flags, IMM_MENTAL)){
		psidmg=0;}
	if (IS_SET(victim->res_flags, RES_MENTAL)){
		psidmg = (psidmg/2);}
	if (IS_SET(victim->vuln_flags, VULN_MENTAL)){
		psidmg = (psidmg * 2);}
	if(IS_SET(victim->imm_flags, IMM_COLD)){
		cryodmg=0;}
	if (IS_SET(victim->res_flags, RES_COLD)){
		cryodmg = (cryodmg/2);}
	if (IS_SET(victim->vuln_flags, VULN_COLD)){
		cryodmg = (cryodmg * 2);}
	if (IS_SET(victim->psionic_affects, PAFF_PSISHIELD)){
		psidmg = (psidmg/2);
		cryodmg = (cryodmg/2);}

		victim->psi -= psidmg;
		victim->hit -= (psidmg + cryodmg);
		ch->printlnf("You chill %s with a cryokinetic blast.", victim->name);
		victim->printlnf("%s chills you with a cryokinetic blast!", ch->name);
		check_improve(ch, gsn_cryokinetics, true, 1);
		victim->fighting =ch;
		ch->fighting = victim;}

}
/**************************************************************************/
void psishock( char_data *ch, char *argument)
{
	char arg1[MIL];
	char_data *victim;

	argument = one_argument( argument, arg1 );

	if(IS_NULLSTR(arg1)){
		victim = ch->fighting;
		if (victim == NULL){
			ch->println("But you aren't fighting anyone!");
			return;}
	}
	else if ((victim = get_char_room(ch,arg1)) == NULL){
		ch->printlnf("'%s' is not here.", arg1);
		return;}

	int chance;
	chance = get_skill( ch, gsn_psi_shock ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}


//	if (victim != ch && !is_safe(ch,victim)){
	if (!is_safe(ch,victim)){
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi -= 25;
		ch->println("Your psi shock fails to discharge properly!");
		check_improve(ch, gsn_psi_shock, false, 1);
		return;}
	ch->psi -= 50;
	int psipower;
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);
	int shockdmg = (psipower/2);
	int psidmg = psipower;

	if(IS_SET(victim->imm_flags, IMM_MENTAL)){
		psidmg=0;}
	if (IS_SET(victim->res_flags, RES_MENTAL)){
		psidmg = (psidmg/2);}
	if (IS_SET(victim->vuln_flags, VULN_MENTAL)){
		psidmg = (psidmg * 2);}
	if(IS_SET(victim->imm_flags, IMM_LIGHTNING)){
		shockdmg=0;}
	if (IS_SET(victim->res_flags, RES_LIGHTNING)){
		shockdmg = (shockdmg/2);}
	if (IS_SET(victim->vuln_flags, VULN_LIGHTNING)){
		shockdmg = (shockdmg * 2);}
	if (IS_SET(victim->psionic_affects, PAFF_PSISHIELD)){
		psidmg = (psidmg/2);
		shockdmg = (shockdmg/2);}

		victim->psi -= psidmg;
		victim->hit -= (psidmg + shockdmg);
		ch->printlnf("You shock %s with a psielectric surge.", victim->name);
		victim->printlnf("%s shocks you with a psielectric surge!", ch->name);
		check_improve(ch, gsn_psi_shock, true, 1);
		victim->fighting =ch;
		ch->fighting = victim;}
}
/**************************************************************************/
void psiburst( char_data *ch )
{
//	char arg1[MIL];
	char_data *victim;
	int chance;

	chance = get_skill( ch, gsn_psi_burst ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}



	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->psi = (ch->psi/2);
		ch->println("Your burst fails!");
		check_improve(ch, gsn_psi_burst, false, 1);
			return;}
	//argument = one_argument( argument, arg1 );

	int burstdmg;
	burstdmg = ch->psi;
	int burstdmgrestore = burstdmg;
	ch->psi = 0;
	REMOVE_BIT(ch->psionic_affects, PAFF_LEVITATE);
	REMOVE_BIT(ch->psionic_affects, PAFF_MOVERB);
	REMOVE_BIT(ch->psionic_affects, PAFF_ADRENCONTROL);
	REMOVE_BIT(ch->psionic_affects, PAFF_FASTHEAL);
	REMOVE_BIT(ch->psionic_affects, PAFF_SCANNING);
	REMOVE_BIT(ch->psionic_affects, PAFF_PSISHIELD);
	REMOVE_BIT(ch->psionic_affects, PAFF_NETTLES);
	REMOVE_BIT(ch->psionic_affects, PAFF_SPIKES);
	REMOVE_BIT(ch->psionic_affects, PAFF_BLUR);
	REMOVE_BIT(ch->psionic_affects, PAFF_AWARENESS);
	REMOVE_BIT(ch->psionic_affects, PAFF_TKSHIELD);
	REMOVE_BIT(ch->psionic_affects, PAFF_ANTIMAGIC);
	REMOVE_BIT(ch->psionic_affects, PAFF_MINDSHIELD);
	REMOVE_BIT(ch->psionic_affects, PAFF_DANGERSENSE);
	ch->println( "Your psionics cease as you run out of psionic energy!" );
	for ( victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room )
	{
		if (victim != ch && !IS_SET(victim->imm_flags, IMM_MENTAL)
		&& !is_safe_spell(ch,victim,true)){



		burstdmg = burstdmgrestore;
		if(IS_SET(victim->imm_flags, IMM_MENTAL)){
			burstdmg=0;}
		if (IS_SET(victim->res_flags, RES_MENTAL)){
		burstdmg = (burstdmg/2);}
		if (IS_SET(victim->vuln_flags, VULN_MENTAL)){
			burstdmg = (burstdmg * 2);}
		if (IS_SET(victim->psionic_affects, PAFF_PSISHIELD)){
			burstdmg = (burstdmg/2);}
		victim->psi -= burstdmg;
		victim->hit -= burstdmg;
		victim->printlnf("%s's psionic burst rips through you!", ch->name);
		check_improve(ch, gsn_psi_burst, true, 1);
		victim->fighting =ch;}
	}
}
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
void do_psi_use(char_data *ch, char *argument)
{
	char arg1[MIL];
    char arg2[MIL];
	int psipower;

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
//	psipower = (ch->perm_stats[STAT_PR] + ch->perm_stats[STAT_RE] + ch->perm_stats[STAT_IN])/2;
	psipower = (ch->modifiers[STAT_PR] + ch->modifiers[STAT_RE] + ch->modifiers[STAT_IN]);

	argument = one_argument( argument, arg1 );
  	strcpy( arg2, argument );
	if (!str_cmp(arg1,"levitation")){
		if (get_skill(ch,gsn_levitation) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		levitate(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"psi shield")){
		if (get_skill(ch,gsn_psi_shield) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		psishield(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"nettles")){
		if (get_skill(ch,gsn_nettles) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		nettles(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"spikes")){
		if (get_skill(ch,gsn_spikes) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		spikes(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"awareness")){
		if (get_skill(ch,gsn_psiawareness) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		awareness(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"blur")){
		if (get_skill(ch,gsn_blur) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		blur(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"tk shield")){
		if (get_skill(ch,gsn_tk_shield) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		tkshield(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"antimagic")){
		if (get_skill(ch,gsn_antimagic) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		antimagic(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"adrenaline control")){
		if (get_skill(ch,gsn_adrenaline_control) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		adrenaline(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"mindscan")){
		if (get_skill(ch,gsn_mindscan) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		mindscan(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"mind over body")){
		if (get_skill(ch,gsn_mind_over_body) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		mind_over_body(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"fast healing")){
		if (get_skill(ch,gsn_psifast_healing) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		fastheal(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"mind shield")){
		if (get_skill(ch,gsn_mind_shield) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		mindshield(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
	if (!str_cmp(arg1,"danger sense")){
		if (get_skill(ch,gsn_danger_sense) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		dangersense(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
	return;}
	if (!str_cmp(arg1,"psi burst")){
		if (get_skill(ch,gsn_psi_burst) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		if (ch->psi >= 1){
			ch->println("You focus your psionic power into a burst!");
			psiburst(ch);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
			return;}
		else{
		ch->println("You have no psionic power to release!");
		return;}
	}

	//targetted Psionics

	if (!str_cmp(arg1,"enfuse")){
		if (get_skill(ch,gsn_enfuse) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		if (ch->psi >= (psipower/2)){

			enfuse(ch, arg2);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
		else{
		ch->println("You haven't got enough psionic power for this ability!");
		return;}
	}
	if (!str_cmp(arg1,"stun")){
		if (get_skill(ch,gsn_stun) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		if (ch->psi >= (psipower/2)){
			ch->println("You focus your psionic power into a stunning blast!");
			stun(ch, arg2);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
		else{
		ch->println("You haven't got enough psionic power for this ability!");
		return;}
	}
	if (!str_cmp(arg1,"pyrokinetics")){
		if (get_skill(ch,gsn_pyrokinetics) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		if (ch->psi >= 50){
			ch->println("You focus your psionic power into a pyrokinetic blast!");
			pyrok(ch, arg2);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
			return;}
		else{
		ch->println("You haven't got enough psionic power for this ability!");
		return;}
	}
	if (!str_cmp(arg1,"disruption")){
		if (get_skill(ch,gsn_disruption) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		if (ch->psi >= (psipower/2)){
			ch->println("You focus your psionic power into disrupting their mind!");
			disruption(ch, arg2);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
		return;}
		else{
		ch->println("You haven't got enough psionic power for this ability!");
		return;}
	}
	if (!str_cmp(arg1,"psi drain")){
		if (get_skill(ch,gsn_psi_drain) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		if (ch->psi >= 50){
			ch->println("You focus your psionic power into their mind!");
			psidrain(ch, arg2);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
			return;}
		else{
		ch->println("You haven't got enough psionic power for this ability!");
		return;}
	}
	if (!str_cmp(arg1,"psi bolt")){
		if (get_skill(ch,gsn_psi_bolt) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		if (ch->psi >= 50){
			ch->println("You focus your psionic power into a psionic bolt!");
			psibolt(ch, arg2);
			return;}
		else{
		ch->println("You haven't got enough psionic power for this ability!");
		return;}
	}
	if (!str_cmp(arg1,"cryokinetics")){
		if (get_skill(ch,gsn_cryokinetics) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		if (ch->psi >= 50){
			ch->println("You focus your psionic power into a cryokinetic blast!");
			cryokin(ch, arg2);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
			return;}
		else{
		ch->println("You haven't got enough psionic power for this ability!");
		return;}
	}
	if (!str_cmp(arg1,"psi shock")){
		if (get_skill(ch,gsn_psi_shock) < 1){
			ch->println("You have no idea how to do that!");
			return;}
		if (ch->psi >= 50){
			ch->println("You focus your psionic power into a psielectric shock!");
			psishock(ch, arg2);
		if (IS_SET(ch->act2,PLR2_TBC) && is_tbcactive (ch)){
        turn_end (ch->turn, 200 - GET_SPEED (ch));}
			return;}
		else{
		ch->println("You haven't got enough psionic power for this ability!");
		return;}
	}
	ch->println("You have no psionic power that can be used like that!");
}
/**************************************************************************/
void do_telepathy( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    char_data *victim;
	char_data *snoop;
    char target[MIL];
	char message[MIL];
	int chance;
	if (get_skill(ch,gsn_telepathy) < 1){
		ch->println("Huh?");
		return;}
	// don't allow messages that could be defrauding
	if(check_defrauding_argument(ch, argument)){
		return;
	}

/*    // wiznet whispers NO MOBS
	if( !IS_UNSWITCHED_MOB(ch )) {
		wiznet(FORMATF("telep %s: \"%s\"", ch->name, argument),
			ch ,NULL,WIZ_WHISPERS,0,get_trust(ch));
	}*/

    argument = one_argument( argument, target);
	strcpy( message, argument );
    if( IS_NULLSTR(target)){
        ch->println( "Send your thoughts to whom?" );
        return;
    }
	if ( !IS_IMMORTAL( ch )){
	chance = get_skill( ch, gsn_telepathy ) - 10;
	if (IS_SET(ch->psionic_affects, PAFF_DISRUPTION)){
		chance = (chance/2);}
	if (chance <= 4){
		chance = 5;}
	if (number_percent() >= (chance + 1)){
		ch->println("Your telepathy fails!");
		check_improve(ch, gsn_telepathy, false, 1);
			return;}
	}
    // find the person in the world to think to them
    victim = get_char_world( ch, target);
    if(!victim){
        ch->printlnf( "You can't find '%s' to send thoughts to", target);
        return;
    }

    if (victim == ch){
        ch->println( "You hear a familiar voice in your head." );
        return;
    }

	if (IS_SET(victim->psionic_affects, PAFF_MINDSHIELD)){
		ch->println("Their mind is shielded from intrusion.");
		check_improve(victim, gsn_mind_shield, false, 1);
		check_improve(ch, gsn_telepathy, false, 1);
		return;}

	if (IS_SET(victim->psionic_affects, PAFF_NETTLES)){
		ch->printlnf("%s's psychic nettles sting your mind!", victim->name);
		victim->printlnf("%s is stung by your psychic nettles.", ch->name);
		ch->hit -= 2;
		ch->psi -= 1;
		check_improve(victim, gsn_nettles, true, 3);}
	if (IS_SET(victim->psionic_affects, PAFF_SPIKES)){
		ch->printlnf("%s's psychic spikes stab your mind!", victim->name);
		victim->printlnf("%s is stabbed by your psychic spikes.", ch->name);
		ch->hit -= 10;
		ch->psi -= 5;
		check_improve(victim, gsn_spikes, true, 3);}

    argument = ltrim_string(argument);
    if(IS_NULLSTR(argument)){
        ch->printlnf( "What thoughts do you want to send to %s?", PERS(victim, ch));
        return;
    }
	ch->printlnf("`xYou mentally send to %s `m'%s'`x", victim->name, message);
	victim->printlnf("`x%s mentally sends to you `m'%s'`x", ch->name, message);
	check_improve(ch, gsn_telepathy, true, 1);
    // go thru all in the room - whispering
    // to target, and those who over hear
	for ( snoop = ch->in_room->people; snoop != NULL; snoop = snoop->next_in_room )
	{
		if (IS_SET(snoop->psionic_affects, PAFF_SCANNING) && snoop != ch && snoop != victim){
			int chance2 = 45 - ch->perm_stats[STAT_IN]/3; // approx range 45 -> 75
            chance2 -= ch->modifiers[STAT_IN]/3;      // can take it up to > 100
            chance2 += snoop->modifiers[STAT_RE]/4;
            chance2 += snoop->modifiers[STAT_IN]/3;

			// awareness helps you overhear
			if (IS_SET(snoop->psionic_affects, PAFF_AWARENESS)){
				chance2 += get_skill(snoop,gsn_psiawareness)/5;
				check_improve(victim, gsn_psiawareness, true, 3);}
			chance2 += get_skill(snoop,gsn_mindscan)/5;

			chance2-= number_range(1,100);

            if ( chance2 > 0 || IS_IMMORTAL(snoop)){
				snoop->printlnf("`x%s mentally sends to %s `m'%s'`x", ch->name, victim->name, message);
				check_improve(snoop, gsn_mindscan, true, 3);}
			if (IS_SET(victim->psionic_affects, PAFF_NETTLES)){
				snoop->printlnf("%s's psychic nettles sting your mind!", victim->name);
				victim->printlnf("%s is stung by your psychic nettles.", snoop->name);
				snoop->hit -= 2;
				snoop->psi -= 1;
				check_improve(victim, gsn_nettles, true, 3);}
			if (IS_SET(victim->psionic_affects, PAFF_SPIKES)){
				snoop->printlnf("%s's psychic spikes stab your mind!", victim->name);
				victim->printlnf("%s is stabbed by your psychic spikes.", snoop->name);
				snoop->hit -= 10;
				snoop->psi -= 5;
				check_improve(victim, gsn_spikes, true, 3);}
			if (IS_SET(ch->psionic_affects, PAFF_NETTLES)){
				snoop->printlnf("%s's psychic nettles sting your mind!", ch->name);
				ch->printlnf("%s is stung by your psychic nettles.", snoop->name);
				snoop->hit -= 2;
				snoop->psi -= 1;
				check_improve(ch, gsn_nettles, true, 3);}
			if (IS_SET(ch->psionic_affects, PAFF_SPIKES)){
				snoop->printlnf("%s's psychic spikes stab your mind!", ch->name);
				ch->printlnf("%s is stabbed by your psychic spikes.", snoop->name);
				snoop->hit -= 10;
				snoop->psi -= 5;
				check_improve(ch, gsn_spikes, true, 3);}


		}
	}
	if (ch->in_room != victim->in_room){
	for ( snoop = victim->in_room->people; snoop != NULL; snoop = snoop->next_in_room )
	{
		if (IS_SET(snoop->psionic_affects, PAFF_SCANNING) && snoop != ch && snoop != victim){
			int chance2 = 45 - ch->perm_stats[STAT_IN]/3; // approx range 45 -> 75
            chance2 -= ch->modifiers[STAT_IN]/3;      // can take it up to > 100
            chance2 += snoop->modifiers[STAT_RE]/4;
            chance2 += snoop->modifiers[STAT_IN]/3;

			// awareness helps you overhear
			if (IS_SET(snoop->psionic_affects, PAFF_AWARENESS)){
				chance2 += get_skill(snoop,gsn_psiawareness)/5;
				check_improve(victim, gsn_psiawareness, true, 3);}
			chance2 += get_skill(snoop,gsn_mindscan)/5;

			chance2-= number_range(1,100);

            if ( chance2 > 0 || IS_IMMORTAL(snoop)){
				snoop->printlnf("`x%s mentally sends to %s `m'%s'`x", ch->name, victim->name, message);
				check_improve(snoop, gsn_mindscan, true, 3);}
			if (IS_SET(victim->psionic_affects, PAFF_NETTLES)){
				snoop->printlnf("%s's psychic nettles sting your mind!", victim->name);
				victim->printlnf("%s is stung by your psychic nettles.", snoop->name);
				snoop->hit -= 2;
				snoop->psi -= 1;
				check_improve(victim, gsn_nettles, true, 3);}
			if (IS_SET(victim->psionic_affects, PAFF_SPIKES)){
				snoop->printlnf("%s's psychic spikes stab your mind!", victim->name);
				victim->printlnf("%s is stabbed by your psychic spikes.", snoop->name);
				snoop->hit -= 10;
				snoop->psi -= 5;
				check_improve(victim, gsn_spikes, true, 3);}
			if (IS_SET(ch->psionic_affects, PAFF_NETTLES)){
				snoop->printlnf("%s's psychic nettles sting your mind!", ch->name);
				ch->printlnf("%s is stung by your psychic nettles.", snoop->name);
				snoop->hit -= 2;
				snoop->psi -= 1;
				check_improve(ch, gsn_nettles, true, 3);}
			if (IS_SET(ch->psionic_affects, PAFF_SPIKES)){
				snoop->printlnf("%s's psychic spikes stab your mind!", ch->name);
				ch->printlnf("%s is stabbed by your psychic spikes.", snoop->name);
				snoop->hit -= 10;
				snoop->psi -= 5;
				check_improve(ch, gsn_spikes, true, 3);}

			}
		}
	}
    return;
}
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*					CRAFT	STUFF	STARTS	HERE						  */
/**************************************************************************/
void do_orerefine( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	char arg[MIL];
	OBJ_DATA *obj;
	int bpurity;
	int bunits;
	int rpurity;
	int runits;
	int rweight;
	int refinedcost;
	OBJ_DATA *refined;
	one_argument( argument, arg );

	int skill=get_skill(ch, gsn_refine);
	if(skill<1){
		ch->println("What would you know about refining ores?");
		return;}
	ch->println("This feature is currently being reworked.");
		return;
	obj_data *tool;
	bool ftool=false;
    for ( tool = ch->carrying; tool; tool = tool->next_content ){
        if ( tool->item_type == ITEM_TOOL
			 && tool->wear_loc != WEAR_NONE
			 && ( tool->value[1] == gsn_refine
			 ||   tool->value[2] == gsn_refine
			 ||   tool->value[3] == gsn_refine
			 ||   tool->value[4] == gsn_refine )) {
			ftool = true;
            break;}
    }

	if(!ftool){
		ch->println("You need to be holding a refining tool in order to process ores.");
		return;}

	// lets see what they can find
	if (tool->value[0] >= 1){
		skill += tool->value[0];
	}
	if ( IS_NULLSTR(arg)){
		ch->println( "Refine what?" );
		return;}

	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL ){
		ch->println("You do not have that item.");
		return;}

	if (obj->item_type != ITEM_MATERIAL){
		ch->println("That's not a material.");
		return;}

	bunits		= obj->value[1];

	if(bunits <= 9){
		ch->println("You must have at least 10 units.");
		return;}

	if(!str_cmp(obj->material,"metal-adamantium")){
			if(obj->pIndexData->vnum != 1392){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1393));
		ch->println("You refine some adamantium ore.");
	}else if(!str_cmp(obj->material,"metal-copper")){
			if(obj->pIndexData->vnum != 1394){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1395));
		ch->println("You refine some copper ore.");
	}else if(!str_cmp(obj->material,"metal-iron")){
			if(obj->pIndexData->vnum != 1396){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1397));
		ch->println("You refine some iron ore.");
	}else if(!str_cmp(obj->material,"metal-lead")){
			if(obj->pIndexData->vnum != 1398){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1399));
		ch->println("You refine some lead ore.");
	}else if(!str_cmp(obj->material,"metal-platinum")){
			if(obj->pIndexData->vnum != 1400){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1401));
		ch->println("You refine some platinum ore.");
	}else if(!str_cmp(obj->material,"metal-silver")){
			if(obj->pIndexData->vnum != 1402){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1403));
		ch->println("You refine some silver ore.");
	}else if(!str_cmp(obj->material,"metal-gold")){
			if(obj->pIndexData->vnum != 1404){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1405));
		ch->println("You refine some gold ore.");
	}else if(!str_cmp(obj->material,"metal-titanium")){
			if(obj->pIndexData->vnum != 1406){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1407));
		ch->println("You refine some titanium ore.");
	}else if(!str_cmp(obj->material,"metal-indurium")){
			if(obj->pIndexData->vnum != 1408){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1409));
		ch->println("You refine some indurium ore.");
	}else if(!str_cmp(obj->material,"metal-arcadium")){
			if(obj->pIndexData->vnum != 1410){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1411));
		ch->println("You refine some arcadium ore.");
	}else if(!str_cmp(obj->material,"metal-zinc")){
			if(obj->pIndexData->vnum != 1412){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1413));
		ch->println("You refine some zinc ore.");
	}else if(!str_cmp(obj->material,"metal-tin")){
			if(obj->pIndexData->vnum != 1414){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1415));
		ch->println("You refine some tin ore.");
	}else if(!str_cmp(obj->material,"metal-astral driftmetal")){
			if(obj->pIndexData->vnum != 1416){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1417));
		ch->println("You refine some astral driftmetal ore.");
	}else if(!str_cmp(obj->material,"metal-abyssal bloodiron")){
			if(obj->pIndexData->vnum != 1418){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1419));
		ch->println("You refine some abyssal bloodiron ore.");
	}else if(!str_cmp(obj->material,"metal-starmetal")){
			if(obj->pIndexData->vnum != 1420){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1421));
		ch->println("You refine some starmetal ore.");
	}else if(!str_cmp(obj->material,"metal-mithril")){
			if(obj->pIndexData->vnum != 1422){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1423));
		ch->println("You refine some mithril ore.");
	}else if(!str_cmp(obj->material,"metal-mithral")){
			if(obj->pIndexData->vnum != 1424){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1425));
		ch->println("You refine some mithral ore.");
	}else if(!str_cmp(obj->material,"metal-blightiron")){
			if(obj->pIndexData->vnum != 1426){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1427));
		ch->println("You refine some blightiron ore.");
	}else if(!str_cmp(obj->material,"metal-stormbane iron")){
			if(obj->pIndexData->vnum != 1428){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1429));
		ch->println("You refine some stormbane iron ore.");
	}else if(!str_cmp(obj->material,"metal-magnetiron")){
			if(obj->pIndexData->vnum != 1430){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1431));
		ch->println("You refine some magnetiron ore.");
	}else if(!str_cmp(obj->material,"metal-sylvan silver")){
			if(obj->pIndexData->vnum != 1432){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1433));
		ch->println("You refine some sylvan silver ore.");
	}else if(!str_cmp(obj->material,"metal-moonsilver")){
			if(obj->pIndexData->vnum != 1434){
				ch->println("That is not an ore.");
			return;}
		refined = create_object(get_obj_index(1435));
		ch->println("You refine some moonsilver ore.");
	}else{
		ch->println("That is not an ore.");
		return;}

		check_improve(ch, gsn_refine, true, 3);
		bpurity		= obj->value[0];
		runits = bunits/5;
		rpurity = number_range(bpurity,100);
		refinedcost = (refined->pIndexData->cost * rpurity);
		refinedcost = (refinedcost * runits);
		rweight = (refined->pIndexData->weight * runits);
		refined->weight = rweight;
		refined->cost = refinedcost;
		refined->value[1] = runits;
		refined->value[0] = rpurity;
		obj_to_char(refined, ch);
		extract_obj( obj);
		return;
}
/**************************************************************************/
void do_weaponsmith( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	char arg[MIL];
	char arg1 [MIL];
	char arg2 [MIL];
	char arg3 [MIL];
	char arg4 [MIL];
	OBJ_DATA *material;
	OBJ_DATA *weapon;
	int svalue;
	int lvalue;
	AFFECT_DATA *paf;
	int matcost = 0;
	argument = one_argument(argument,arg);
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	strcpy( arg4, argument );
	bool wood_material = false;
	bool stone_material = false;
	bool metal_material = false;
	bool organic_material = false;
	bool textile_material = false;
	bool crystal_material = false;
	bool gem_material = false;

	if ( arg[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
	{
		ch->println( "Syntax:" );
		ch->println( "  weaponsmith type material size level (twohand)" );
		ch->println( "  twohand trait optional, only avaliable on some" );
		ch->println( "  weapon types necessary on others." );
		return;
	}

	int skill=get_skill(ch, gsn_weaponsmith);
	if(skill<1){
		ch->println("What would you know about making weapons?");
		return;}

	obj_data *tool;
	bool ftool=false;
    for ( tool = ch->carrying; tool; tool = tool->next_content ){
        if ( tool->item_type == ITEM_TOOL
			 && tool->wear_loc != WEAR_NONE
			 && ( tool->value[1] == gsn_weaponsmith
			 ||   tool->value[2] == gsn_weaponsmith
			 ||   tool->value[3] == gsn_weaponsmith
			 ||   tool->value[4] == gsn_weaponsmith )) {
			ftool = true;
            break;}
    }

	if(!ftool){
		ch->println("You need to be holding a weaponsmithing tool in order to create weapons.");
		return;}

	// lets see what they can find
	if (tool->value[0] >= 1){
		skill += tool->value[0];
	}

    int mwi=morphable_weapon_lookup(arg);
	if(mwi<0)
    {
        ch->printlnf("Weapon type %s not valid!", arg);
      return;
    }

	char *orecheck = "ore";
	if ( ( material = get_obj_carry( ch, arg1 ) ) == NULL ){
		ch->printlnf("You do not have any %s.",arg);
		return;}
	if (material->item_type != ITEM_MATERIAL){
		ch->printlnf("Your %s is not a material.",material->short_descr);
		return;}
	if (is_name( orecheck, material->name )){
		ch->printlnf("Your %s is not a refined material.",material->short_descr);
		return;}
	if ( !is_number( arg3) ){
       ch->println("Numeric level is required.");
       return;
    }
	lvalue = atoi( arg3);
    if ( lvalue > material->value[0] )
    {
		ch->println("Material purity not high enough.");
		return;
    }
	if (lvalue > skill){
		ch->println("Your skill is not high enough.");
		return;
    }
	if(!strcmp(arg2,"diminutive")){
		svalue = 1;
	}else if(!strcmp(arg2,"tiny")){
		svalue = 2;
	}else if(!strcmp(arg2,"small")){
		svalue = 3;
	}else if(!strcmp(arg2,"medium")){
		svalue = 4;
	}else if(!strcmp(arg2,"large")){
		svalue = 5;
	}else if(!strcmp(arg2,"giant")){
		svalue = 6;
	}else if(!strcmp(arg2,"gargantuan")){
		svalue = 7;
	}else{
		ch->println("Incorrect size value.");
		ch->println("Use: diminutive, tiny, small, medium,");
		ch->println("large, giant, or gargantuan.");
		return;}
	if (svalue > material->value[1]){
		ch->println("Not enough material.");
		return;
	}
		if(!str_cmp(material->material,"metal-adamantium") || !str_cmp(material->material,"metal-copper")
		|| !str_cmp(material->material,"metal-iron") || !str_cmp(material->material,"metal-lead")
		|| !str_cmp(material->material,"metal-platinum") || !str_cmp(material->material,"metal-silver")
		|| !str_cmp(material->material,"metal-gold") || !str_cmp(material->material,"metal-titanium")
		|| !str_cmp(material->material,"metal-indurium") || !str_cmp(material->material,"metal-arcadium")
		|| !str_cmp(material->material,"metal-zinc") || !str_cmp(material->material,"metal-tin")
		|| !str_cmp(material->material,"metal-starmetal") || !str_cmp(material->material,"metal-mithril")
		|| !str_cmp(material->material,"metal-mithral") || !str_cmp(material->material,"metal-magnetiron")
		|| !str_cmp(material->material,"metal-moonsilver") || !str_cmp(material->material,"metal-astral driftmetal")
		|| !str_cmp(material->material,"metal-abyssal bloodiron") || !str_cmp(material->material,"metal-blightiron")
		|| !str_cmp(material->material,"metal-stormbane iron") || !str_cmp(material->material,"metal-sylvan silver")){
		metal_material = true;
	}
	//ALLOYS
	if(!str_cmp(material->material,"alloy-rune bronze") || !str_cmp(material->material,"alloy-froststeel")
		|| !str_cmp(material->material,"alloy-greensteel") || !str_cmp(material->material,"alloy-psisteel")
		|| !str_cmp(material->material,"alloy-brass") || !str_cmp(material->material,"alloy-pewter")
		|| !str_cmp(material->material,"alloy-steel") || !str_cmp(material->material,"alloy-bronze")
		|| !str_cmp(material->material,"alloy-solar truesteel") || !str_cmp(material->material,"alloy-bloodsteel")
		|| !str_cmp(material->material,"alloy-ember arcadium") || !str_cmp(material->material,"alloy-magemetal")
		|| !str_cmp(material->material,"alloy-white gold") || !str_cmp(material->material,"alloy-sterling silver")
		|| !str_cmp(material->material,"alloy-rose gold") || !str_cmp(material->material,"alloy-electrum")
		|| !str_cmp(material->material,"alloy-black mithril") || !str_cmp(material->material,"alloy-cast iron")){
		metal_material = true;

	}
	//GEMS
	if(!str_cmp(material->material,"gem-amethyst") || !str_cmp(material->material,"gem-ruby")
		|| !str_cmp(material->material,"gem-turqoise") || !str_cmp(material->material,"gem-sapphire")
		|| !str_cmp(material->material,"gem-pearl") || !str_cmp(material->material,"gem-opal")
		|| !str_cmp(material->material,"gem-onyx") || !str_cmp(material->material,"gem-emerald")
		|| !str_cmp(material->material,"gem-diamond") || !str_cmp(material->material,"gem-jade")
		|| !str_cmp(material->material,"gem-bloodstone") || !str_cmp(material->material,"gem-amber")
		|| !str_cmp(material->material,"gem-quartz") || !str_cmp(material->material,"gem-agate")
		|| !str_cmp(material->material,"gem-aquamarine") || !str_cmp(material->material,"gem-beryl")
		|| !str_cmp(material->material,"gem-carnelian") || !str_cmp(material->material,"gem-chalcedony")
		|| !str_cmp(material->material,"gem-chrysoberyl") || !str_cmp(material->material,"gem-citrine")
		|| !str_cmp(material->material,"gem-darkstone") || !str_cmp(material->material,"gem-garnet")
		|| !str_cmp(material->material,"gem-golden beryl") || !str_cmp(material->material,"gem-jasper")
		|| !str_cmp(material->material,"gem-lapis lazuli") || !str_cmp(material->material,"gem-malachite")
		|| !str_cmp(material->material,"gem-moonstone") || !str_cmp(material->material,"gem-moss agate")
		|| !str_cmp(material->material,"gem-peridot") || !str_cmp(material->material,"gem-scarlet emerald")
		|| !str_cmp(material->material,"gem-spinel") || !str_cmp(material->material,"gem-sunstone")
		|| !str_cmp(material->material,"gem-tiger's-eye") || !str_cmp(material->material,"gem-topaz")
		|| !str_cmp(material->material,"gem-common") || !str_cmp(material->material,"gem-miscellaneous")){
		gem_material = true;

	}
	//ROCKS
	if(!str_cmp(material->material,"rock-greenstone") || !str_cmp(material->material,"rock-obsidian")
		|| !str_cmp(material->material,"rock-common") || !str_cmp(material->material,"rock-sugar")
		|| !str_cmp(material->material,"rock-basalt") || !str_cmp(material->material,"rock-miscellaneous")
		|| !str_cmp(material->material,"rock-gneiss") || !str_cmp(material->material,"rock-shale")){
		stone_material = true;
	}
	//ORGANICS
	if(!str_cmp(material->material,"organic-scale") || !str_cmp(material->material,"organic-dragonscale")
		|| !str_cmp(material->material,"organic-bone") || !str_cmp(material->material,"organic-coral")
		|| !str_cmp(material->material,"organic-jet") || !str_cmp(material->material,"organic-shell")
		|| !str_cmp(material->material,"organic-bone-ivory") || !str_cmp(material->material,"organic-bone-dragonbone")
		|| !str_cmp(material->material,"organic-ice")){
		organic_material = true;
	}
	//MINERAL
	if(!str_cmp(material->material,"mineral-blended quartz") || !str_cmp(material->material,"mineral-dolomite")
		|| !str_cmp(material->material,"mineral-salt")){
		stone_material = true;
	}
	//CRYSTAL
	if(!str_cmp(material->material,"crystal-glass")){
		crystal_material = true;
	}
	//TEXTILES
	if(!str_cmp(material->material,"textile-hemp") || !str_cmp(material->material,"textile-leather")
		|| !str_cmp(material->material,"textile-seaweed")){
		textile_material = true;
	}

	//HEARTWOOD
	if(!str_cmp(material->material,"heartwood-ashwood") || !str_cmp(material->material,"heartwood-balsa")
		|| !str_cmp(material->material,"heartwood-birch") || !str_cmp(material->material,"heartwood-bronzewood")
		|| !str_cmp(material->material,"heartwood-cedar") || !str_cmp(material->material,"heartwood-cherrywood")
		|| !str_cmp(material->material,"heartwood-cypress") || !str_cmp(material->material,"heartwood-darkwood")
		|| !str_cmp(material->material,"heartwood-ebony") || !str_cmp(material->material,"heartwood-elmwood")
		|| !str_cmp(material->material,"heartwood-eucalyptus") || !str_cmp(material->material,"heartwood-hickory")
		|| !str_cmp(material->material,"heartwood-ironwood") || !str_cmp(material->material,"heartwood-mahogany")
		|| !str_cmp(material->material,"heartwood-hard maple") || !str_cmp(material->material,"heartwood-soft maple")
		|| !str_cmp(material->material,"heartwood-mindwood") || !str_cmp(material->material,"heartwood-red oak")
		|| !str_cmp(material->material,"heartwood-white oak") || !str_cmp(material->material,"heartwood-pine")
		|| !str_cmp(material->material,"heartwood-redwood") || !str_cmp(material->material,"heartwood-rosewood")
		|| !str_cmp(material->material,"heartwood-sandalwood") || !str_cmp(material->material,"heartwood-steelwood")
		|| !str_cmp(material->material,"heartwood-teak") || !str_cmp(material->material,"heartwood-walnut")
		|| !str_cmp(material->material,"heartwood-willowwood") || !str_cmp(material->material,"heartwood-yew")
		|| !str_cmp(material->material,"heartwood-common") || !str_cmp(material->material,"heartwood-miscellaneous")
		|| !str_cmp(material->material,"heartwood-applewood")){
		wood_material = true;
	}
	//SAPWOOD
	if(!str_cmp(material->material,"sapwood-ashwood") || !str_cmp(material->material,"sapwood-balsa")
		|| !str_cmp(material->material,"sapwood-birch") || !str_cmp(material->material,"sapwood-bronzewood")
		|| !str_cmp(material->material,"sapwood-cedar") || !str_cmp(material->material,"sapwood-cherrywood")
		|| !str_cmp(material->material,"sapwood-cypress") || !str_cmp(material->material,"sapwood-darkwood")
		|| !str_cmp(material->material,"sapwood-ebony") || !str_cmp(material->material,"sapwood-elmwood")
		|| !str_cmp(material->material,"sapwood-eucalyptus") || !str_cmp(material->material,"sapwood-hickory")
		|| !str_cmp(material->material,"sapwood-ironwood") || !str_cmp(material->material,"sapwood-mahogany")
		|| !str_cmp(material->material,"sapwood-hard maple") || !str_cmp(material->material,"sapwood-soft maple")
		|| !str_cmp(material->material,"sapwood-mindwood") || !str_cmp(material->material,"sapwood-red oak")
		|| !str_cmp(material->material,"sapwood-white oak") || !str_cmp(material->material,"sapwood-pine")
		|| !str_cmp(material->material,"sapwood-redwood") || !str_cmp(material->material,"sapwood-rosewood")
		|| !str_cmp(material->material,"sapwood-sandalwood") || !str_cmp(material->material,"sapwood-steelwood")
		|| !str_cmp(material->material,"sapwood-teak") || !str_cmp(material->material,"sapwood-walnut")
		|| !str_cmp(material->material,"sapwood-willowwood") || !str_cmp(material->material,"sapwood-yew")
		|| !str_cmp(material->material,"sapwood-common") || !str_cmp(material->material,"sapwood-miscellaneous")
		|| !str_cmp(material->material,"sapwood-applewood")){
		wood_material = true;
	}
	//MONOCOT
	if(!str_cmp(material->material,"monocot-bamboo") || !str_cmp(material->material,"monocot-palm")){
		wood_material = true;
	}
	if (!str_prefix(arg,"lightblade")
		&& (str_cmp(material->material,"metal-abyssal bloodiron")
		&& str_cmp(material->material,"metal-stormbane iron")
		&& str_cmp(material->material,"alloy-rune bronze")
		&& str_cmp(material->material,"alloy-psisteel")
		&& str_cmp(material->material,"alloy-greensteel")
		&& str_cmp(material->material,"alloy-bloodsteel")
		&& str_cmp(material->material,"alloy-froststeel")
		&& str_cmp(material->material,"alloy-solar truesteel"))){
		ch->println("Lightblades can not be made from that material.");
		return;
	}
		if(!wood_material && !metal_material
		&& !stone_material && !organic_material
		&& !textile_material && !crystal_material
		&& !gem_material){
		ch->println("The selected material can not be used for weapons.");
		return;}


	if (wood_material){
		if (!str_prefix(arg,"whip") || !str_prefix(arg,"rifle")
		|| !str_prefix(arg,"pistol")){
		ch->println("The selected material can not be used for that weapon type.");
		return;}
	}

	if (organic_material){
		if (!str_prefix(arg,"rifle") || !str_prefix(arg,"pistol")){
		ch->println("The selected material can not be used for that weapon type.");
		return;}
	}

	if (gem_material){
		if (!str_prefix(arg,"whip") || !str_prefix(arg,"rifle")
		|| !str_prefix(arg,"pistol") || !str_prefix(arg,"shortbow")
		|| !str_prefix(arg,"longbow") || !str_prefix(arg,"crossbow")){
		ch->println("The selected material can not be used for that weapon type.");
		return;}
	}

	if (stone_material){
		if (!str_prefix(arg,"whip") || !str_prefix(arg,"rifle")
		|| !str_prefix(arg,"pistol") || !str_prefix(arg,"shortbow")
		|| !str_prefix(arg,"longbow") || !str_prefix(arg,"crossbow")){
		ch->println("The selected material can not be used for that weapon type.");
		return;}
	}

	if (crystal_material){
		if (!str_prefix(arg,"cestus") || !str_prefix(arg,"whip")
			|| !str_prefix(arg,"rifle") || !str_prefix(arg,"pistol")
			|| !str_prefix(arg,"shortbow") || !str_prefix(arg,"longbow")
			|| !str_prefix(arg,"crossbow")){
		ch->println("The selected material can not be used for that weapon type.");
		return;}
	}
	if (textile_material){
		if (str_prefix(arg,"cestus") && str_prefix(arg,"whip")){
		ch->println("The selected material can not be used for that weapon type.");
		return;}
	}

	if (!str_prefix(arg,"lightblade")){
		weapon = create_object(get_obj_index(574));
	}else if (!str_prefix(arg,"blastpistol")){
		weapon = create_object(get_obj_index(575));
	}else if (!str_prefix(arg,"blastrifle")){
		weapon = create_object(get_obj_index(576));
	}else{
		weapon = create_object(get_obj_index(593));
	}
	affects_from_template_to_obj(weapon);
	weapon->material = str_dup(material->material);
	weapon->relative_size = svalue;

	if (!str_prefix(arg,"lightblade")){
	if(!str_cmp(weapon->material,"alloy-psisteel")){
	weapon->weight = 2 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"alloy-bloodsteel")){
	weapon->weight = 4 * svalue;
	weapon->relative_size = 50;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	SET_BIT(weapon->extra_flags, OBJEXTRA_BURN_PROOF);
	SET_BIT(weapon->extra_flags, OBJEXTRA_ANTI_NEUTRAL);
	SET_BIT(weapon->extra_flags, OBJEXTRA_GLOW);
	SET_BIT(weapon->extra_flags, OBJEXTRA_ANTI_GOOD);
	SET_BIT(weapon->extra2_flags, OBJEXTRA2_NODECAY);
	}else if(!str_cmp(weapon->material,"alloy-solar truesteel")){
	weapon->weight = 0;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	weapon->relative_size = 50;
	SET_BIT(weapon->extra_flags, OBJEXTRA_BURN_PROOF);
	SET_BIT(weapon->extra_flags, OBJEXTRA_ANTI_NEUTRAL);
	SET_BIT(weapon->extra_flags, OBJEXTRA_GLOW);
	SET_BIT(weapon->extra_flags, OBJEXTRA_ANTI_EVIL);
	SET_BIT(weapon->extra2_flags, OBJEXTRA2_NODECAY);
	}else if(!str_cmp(weapon->material,"alloy-rune bronze")){
	weapon->weight = 8 * svalue;
	matcost = 70 * svalue;
//	matcost = matcost * material->value[0];
	SET_BIT(weapon->extra_flags, OBJEXTRA_BURN_PROOF);
	SET_BIT(weapon->extra2_flags, OBJEXTRA2_NODECAY);
	}
	weapon->level = lvalue;
	matcost = matcost * material->value[0];
	if(!metal_material){
	SET_BIT(weapon->extra_flags, OBJEXTRA_NONMETAL);}
	weapon->cost = matcost;
	obj_to_char(weapon, ch);
	ch->printlnf("You successfully craft %s!", weapon->short_descr);
	check_improve(ch, gsn_weaponsmith, true, 1);
	weapon->craftername=strdup(ch->name);
	material->weight -= weapon->weight;
	material->value[1] -= svalue;
	material->cost -= matcost;
	if (material->value[1] <= 0 || material->weight <= -1 || material->cost <= 0){
	ch->println ("Your creation expends the last of the usable material in that pile");
	extract_obj(material);
	}
	return;
	}

	matcost = 0;
	bool hit_found = false, dam_found = false;
	bool mana_found = false, health_found = false;
	bool move_found = false, psi_found = false;
	bool defense_found = false;

	 for (paf=OBJECT_AFFECTS(weapon); paf; paf=paf->next )
    {
        if ( paf->location == APPLY_HITROLL ){
            hit_found = true;
        }
		if (paf->location == APPLY_DAMROLL ){
            dam_found = true;
        }
		if (paf->location == APPLY_MANA ){
            mana_found = true;
        }
		if (paf->location == APPLY_PSI ){
            psi_found = true;
        }
		if (paf->location == APPLY_HIT ){
            health_found = true;
        }
		if (paf->location == APPLY_MOVE ){
            move_found = true;
        }
		if (paf->location == APPLY_AC ){
            defense_found = true;
        }
    }

	if (metal_material){
	if(!str_cmp(weapon->material,"metal-adamantium")){
	weapon->weight = 2 * svalue;
	matcost = 1050 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF | OBJEXTRA_NO_DEGRADE;
	weapon->extra2_flags= OBJEXTRA2_NODECAY;
	}else if(!str_cmp(weapon->material,"metal-iron")){
	weapon->weight = 3 * svalue;
	matcost = 12 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	}else if(!str_cmp(weapon->material,"metal-silver")){
	weapon->weight = 4 * svalue;
	matcost = 10 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	}else if(!str_cmp(weapon->material,"metal-gold")){
	weapon->weight = 7 * svalue;
	matcost = 100 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	}else if(!str_cmp(weapon->material,"metal-titanium")){
	weapon->weight = 2 * svalue;
	matcost = 1100 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	weapon->extra2_flags= OBJEXTRA2_NODECAY;
	}else if(!str_cmp(weapon->material,"metal-indurium")){
	weapon->weight = 2 * svalue;
	matcost = 1200 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF | OBJEXTRA_NO_DEGRADE | OBJEXTRA_HUM;
	weapon->extra2_flags= OBJEXTRA2_NODECAY;
	}else if(!str_cmp(weapon->material,"metal-arcadium")){
	weapon->weight = 0;
	matcost = 1300 * svalue;
//	matcost = matcost * material->value[0];
	weapon->relative_size = 50;
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	}else if(!str_cmp(weapon->material,"metal-tin")){
	weapon->weight = 3 * svalue;
	matcost = 8 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	}else if(!str_cmp(weapon->material,"metal-astral driftmetal")){
	weapon->weight = 1 * svalue;
	matcost = 800 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF | OBJEXTRA_ANTI_EVIL | OBJEXTRA_GLOW;
	}else if(!str_cmp(weapon->material,"metal-abyssal bloodiron")){
	weapon->weight = 3 * svalue;
	matcost = 850 * svalue;
//	matcost = matcost * material->value[0];
	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	SET_BIT(weapon->value[4],WEAPON_VAMPIRIC);
	}
	weapon->extra_flags= OBJEXTRA_BURN_PROOF | OBJEXTRA_ANTI_GOOD;
	}else if(!str_cmp(weapon->material,"metal-starmetal")){
	weapon->weight = 6 * svalue;
	matcost = 9000 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"metal-mithril")){
	weapon->weight = 2 * svalue;
	matcost = 150 * svalue;
//  matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF | OBJEXTRA_NO_DEGRADE;
	weapon->extra2_flags= OBJEXTRA2_NODECAY;
	}else if(!str_cmp(weapon->material,"metal-mithral")){
	weapon->weight = 2 * svalue;
	matcost = 50 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"metal-blightiron")){
	weapon->weight = 4 * svalue;
	matcost = 850 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF | OBJEXTRA_ANTI_EVIL | OBJEXTRA_ANTI_GOOD;
	}else if(!str_cmp(weapon->material,"metal-stormbane iron")){
	weapon->weight = 3 * svalue;
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	//add lightning resist
	paf = new_affect();
	paf->where      = WHERE_RESIST;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = RES_LIGHTNING;
	paf->next       = weapon->affected;
	weapon->affected   = paf;

	}else if(!str_cmp(weapon->material,"metal-magnetiron")){
	weapon->weight = 3 * svalue;
	matcost = 50 * svalue;
//	matcost = matcost * material->value[0];

	}else if(!str_cmp(weapon->material,"metal-sylvan silver")){
	weapon->weight = 3 * svalue;
	matcost = 100 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	SET_BIT(weapon->value[4],WEAPON_SHARP);
	}
	}else if(!str_cmp(weapon->material,"metal-zinc")){
	weapon->weight = 3 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	//add lightning vuln
	paf = new_affect();
	paf->where      = WHERE_VULN;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = VULN_LIGHTNING;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	}
	if(!str_cmp(weapon->material,"metal-moonsilver")){
	weapon->weight = 4 * svalue;
	matcost = 120 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF | OBJEXTRA_GLOW;
	}
	if(!str_cmp(weapon->material,"metal-copper")){
	weapon->weight = 3 * svalue;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	//add lightning vuln
	paf = new_affect();
	paf->where      = WHERE_VULN;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = VULN_LIGHTNING;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	}
	if(!str_cmp(weapon->material,"alloy-brass")){
	weapon->weight = 3 * svalue;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"alloy-bronze")){
	weapon->weight = 3 * svalue;
	matcost = 9 * svalue;
//	matcost = matcost * material->value[0];
	//add fire vuln
	paf = new_affect();
	paf->where      = WHERE_VULN;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = VULN_FIRE;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	//add lightning vuln
	paf = new_affect();
	paf->where      = WHERE_VULN;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = VULN_LIGHTNING;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	}else if(!str_cmp(weapon->material,"alloy-steel")){
	weapon->weight = 3 * svalue;
	matcost = 15 * svalue;
//	matcost = matcost * material->value[0];

	}else if(!str_cmp(weapon->material,"alloy-psisteel")){
	weapon->weight = 2 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	if (psi_found){
				for ( paf = weapon->affected; paf != NULL; paf = paf->next){
				if ( paf->location == APPLY_PSI){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
				paf = new_affect();
				paf->where      = WHERE_OBJEXTRA;
				paf->type       = -1;
				paf->duration   = -1;
				paf->location    = APPLY_PSI;
                		paf->modifier    =  lvalue/2;
				paf->bitvector  = 0;
				paf->next       = weapon->affected;
				weapon->affected   = paf;}
	}
	}else if(!str_cmp(weapon->material,"alloy-bloodsteel")){
	weapon->weight = 4 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF | OBJEXTRA_ANTI_NEUTRAL | OBJEXTRA_GLOW | OBJEXTRA_ANTI_GOOD;

	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	SET_BIT(weapon->value[4],WEAPON_VAMPIRIC);
	}
	}else if(!str_cmp(weapon->material,"alloy-solar truesteel")){
	weapon->weight = 0;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	if (mana_found){
				for ( paf = weapon->affected; paf != NULL; paf = paf->next){
				if ( paf->location == APPLY_MANA){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
				paf = new_affect();
				paf->where      = WHERE_OBJEXTRA;
				paf->type       = -1;
				paf->duration   = -1;
				paf->location    = APPLY_MANA;
                paf->modifier    = lvalue/2;
				paf->bitvector  = 0;
				paf->next       = weapon->affected;
				weapon->affected   = paf;}
	}
	weapon->relative_size = 50;
	weapon->extra_flags= OBJEXTRA_BURN_PROOF | OBJEXTRA_ANTI_NEUTRAL | OBJEXTRA_GLOW | OBJEXTRA_ANTI_EVIL;

	}else if(!str_cmp(weapon->material,"alloy-greensteel")){
	weapon->weight = 4 * svalue;
	matcost = 25 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF | OBJEXTRA_ROT_DEATH;
	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	if (move_found){
				for ( paf = weapon->affected; paf != NULL; paf = paf->next){
				if ( paf->location == APPLY_MOVE){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
				paf = new_affect();
				paf->where      = WHERE_OBJEXTRA;
				paf->type       = -1;
				paf->duration   = -1;
				paf->location    = APPLY_MOVE;
                paf->modifier    = lvalue/2;
				paf->bitvector  = 0;
				paf->next       = weapon->affected;
				weapon->affected   = paf;}
	}
	}else if(!str_cmp(weapon->material,"alloy-froststeel")){
	weapon->weight = 3 * svalue;
	matcost = 23 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;

	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	//add fire vuln
	paf = new_affect();
	paf->where      = WHERE_VULN;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = VULN_FIRE;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	//add cold resist
	paf = new_affect();
	paf->where      = WHERE_RESIST;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = RES_COLD;
	paf->next       = weapon->affected;
	weapon->affected   = paf;

	SET_BIT(weapon->value[4],WEAPON_FROST);
	}
	}else if(!str_cmp(weapon->material,"alloy-rune bronze")){
	weapon->weight = 8 * svalue;
	matcost = 70 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	//add fire vuln
	paf = new_affect();
	paf->where      = WHERE_VULN;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = VULN_FIRE;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	//add lightning vuln
	paf = new_affect();
	paf->where      = WHERE_VULN;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = VULN_LIGHTNING;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
		if (mana_found){
				for ( paf = weapon->affected; paf != NULL; paf = paf->next){
				if ( paf->location == APPLY_MANA){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
				paf = new_affect();
				paf->where      = WHERE_OBJEXTRA;
				paf->type       = -1;
				paf->duration   = -1;
				paf->location    = APPLY_MANA;
                paf->modifier    = lvalue/2;
				paf->bitvector  = 0;
				paf->next       = weapon->affected;
				weapon->affected   = paf;}
	}
	}
	if(!str_cmp(weapon->material,"alloy-black mithril")){
	weapon->weight = 4 * svalue;
	matcost = 1200 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	SET_BIT(weapon->value[4],WEAPON_SHARP);
	}
	}else if(!str_cmp(weapon->material,"alloy-cast iron")){
	weapon->weight = 6 * svalue;
	matcost = 17 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	if (dam_found){
				for ( paf = weapon->affected; paf != NULL; paf = paf->next){
				if ( paf->location == APPLY_DAMROLL){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
				paf = new_affect();
				paf->where      = WHERE_OBJEXTRA;
				paf->type       = -1;
				paf->duration   = -1;
				paf->location    = APPLY_DAMROLL;
                paf->modifier    = lvalue/2;
				paf->bitvector  = 0;
				paf->next       = weapon->affected;
				weapon->affected   = paf;}
	}
	}else if(!str_cmp(weapon->material,"alloy-electrum")){
	weapon->weight = 11 * svalue;
	matcost = 110 * svalue;
//	matcost = matcost * material->value[0];
	//add lightning vuln
	paf = new_affect();
	paf->where      = WHERE_VULN;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = VULN_LIGHTNING;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	}else if(!str_cmp(weapon->material,"alloy-white gold")){
	weapon->weight = 10 * svalue;
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	SET_BIT(weapon->value[4],WEAPON_SHARP);
	}
	}
	}

	if (wood_material){
	weapon->weight = 3 * svalue;
	matcost = 15 * svalue;
//	matcost = matcost * material->value[0];

	if(!str_cmp(weapon->material,"sapwood-bamboo") || !str_cmp(weapon->material,"heartwood-bamboo")){
	//add water resist
	paf = new_affect();
	paf->where      = WHERE_RESIST;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = RES_DROWNING;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	}else if (!str_cmp(weapon->material,"sapwood-bronzewood") || !str_cmp(weapon->material,"heartwood-bronzewood")){
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	//add fire vuln
	paf = new_affect();
	paf->where      = WHERE_VULN;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = VULN_LIGHTNING;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	}else if (!str_cmp(weapon->material,"sapwood-cypress") || !str_cmp(weapon->material,"heartwood-cypress")){
	SET_BIT(weapon->extra2_flags, OBJEXTRA2_NODECAY);
	}else if (!str_cmp(weapon->material,"sapwood-elmwood") || !str_cmp(weapon->material,"heartwood-elmwood")){
	SET_BIT(weapon->extra2_flags, OBJEXTRA2_NODECAY);
	}else if (!str_cmp(weapon->material,"sapwood-ironwood") || !str_cmp(weapon->material,"heartwood-ironwood")){
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	}else if (!str_cmp(weapon->material,"sapwood-mindwood") || !str_cmp(weapon->material,"heartwood-mindwood")){
	if (str_prefix(arg,"blastpistol")
	&& str_prefix(arg,"blastrifle")){
	if (psi_found){
				for ( paf = weapon->affected; paf != NULL; paf = paf->next){
				if ( paf->location == APPLY_PSI){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
				paf = new_affect();
				paf->where      = WHERE_OBJEXTRA;
				paf->type       = -1;
				paf->duration   = -1;
				paf->location    = APPLY_PSI;
                		paf->modifier    =  lvalue/2;
				paf->bitvector  = 0;
				paf->next       = weapon->affected;
				weapon->affected   = paf;}
	}
	}else if (!str_cmp(weapon->material,"sapwood-redwood") || !str_cmp(weapon->material,"heartwood-redwood")){
	//add fire resist
	paf = new_affect();
	paf->where      = WHERE_RESIST;
	paf->type       = -1;
	paf->duration   = -1;
	paf->location    = APPLY_NONE;
	paf->modifier    = 0;
	paf->bitvector  = RES_FIRE;
	paf->next       = weapon->affected;
	weapon->affected   = paf;
	}else if (!str_cmp(weapon->material,"sapwood-steelwood") || !str_cmp(weapon->material,"heartwood-steelwood")){
	weapon->extra_flags= OBJEXTRA_BURN_PROOF;
	}
	}


	if (gem_material){
	weapon->weight = 2 * svalue;
	if(!str_cmp(weapon->material,"gem-agate")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-amethyst")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-aquamarine")){
	matcost = 240 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-beryl")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-bloodstone")){
	matcost = 500 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-carnelian")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-chalcedony")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-chrysoberyl")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-citrine")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-darkstone")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-diamond")){
	matcost = 1000 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-emerald")){
	matcost = 1000 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-garnet")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-golden beryl")){
	matcost = 500 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-jade")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-jasper")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-lapis lazuli")){
	matcost = 800 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-malachite")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-moonstone")){
	matcost = 1000 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-moss agate")){
	matcost = 500 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-onyx")){
	matcost = 600 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-opal")){
	matcost = 700 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-pearl")){
	matcost = 720 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-peridot")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-quartz")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-ruby")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-sapphire")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-scarlet emerald")){
	matcost = 1200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-spinel")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-sunstone")){
	matcost = 2500 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-tiger's-eye")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-topaz")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-turquoise")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-common")){
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"gem-miscellaneous")){
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	}
	}

	if (organic_material){
	if(!str_cmp(weapon->material,"organic-scale")){
	weapon->weight = 10 * svalue;
	matcost = 10 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"organic-dragonscale")){
	weapon->weight = 15 * svalue;
	matcost = 300 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"organic-bone")){
	weapon->weight = 5 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"organic-coral")){
	weapon->weight = 1 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"organic-jet")){
	weapon->weight = 5 * svalue;
	matcost = 10 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"organic-shell")){
	weapon->weight = 10 * svalue;
	matcost = 6 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"organic-bone-ivory")){
	weapon->weight = 1 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"organic-bone-dragonbone")){
	weapon->weight = 10 * svalue;
	matcost = 400 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"organic-ice")){
	weapon->weight = 1 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}
	}

	if (stone_material){
	if(!str_cmp(weapon->material,"rock-common")){
	weapon->weight = 1 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"rock-miscellaneous")){
	weapon->weight = 1 * svalue;
	matcost = 4 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"rock-basalt")){
	weapon->weight = 1 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"rock-greenstone")){
	weapon->weight = 1 * svalue;
	matcost = 25 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"rock-obsidian")){
	weapon->weight = 1 * svalue;
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"rock-sugar")){
	weapon->weight = 1 * svalue;
	matcost = svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"rock-gneiss")){
	weapon->weight = 1 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"rock-shale")){
	weapon->weight = 1 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}
	}

	if (crystal_material){
	//glass is only crystal material usable for weapons
	weapon->weight = 0;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	}

	if (textile_material){
	if(!str_cmp(weapon->material,"textile-hemp")){
	weapon->weight = 0;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"textile-leather")){
	weapon->weight = 0;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(weapon->material,"textile-seaweed")){
	weapon->weight = 0;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	}
	}



	if (!str_prefix(arg,"longbow") || !str_prefix(arg,"shortbow")
		|| !str_prefix(arg,"crossbow") || !str_prefix(arg,"rifle")
		|| !str_prefix(arg,"polearm") || !str_prefix(arg,"spear")){
	SET_BIT(weapon->value[4],WEAPON_TWO_HANDS);
	ch->println("Twohand weapon flag set due to weapon type.");
	}else if ( arg4[0] != '\0'){
		if (!str_cmp(arg4,"twohand")){
		if (!str_prefix(arg,"dagger") || !str_prefix(arg,"whip")
			|| !str_prefix(arg,"flail") || !str_prefix(arg,"kama")
			|| !str_prefix(arg,"siangham") || !str_prefix(arg,"cestus")
			|| !str_prefix(arg,"pistol")){
			ch->println("Twohand weapon flag NOT set due to weapon type.");
			}else{
			SET_BIT(weapon->value[4],WEAPON_TWO_HANDS);
			ch->println("Twohand weapon flag set.");}
	}
	}else{
	REMOVE_BIT(weapon->value[4],WEAPON_TWO_HANDS);
	ch->println("Twohand weapon flag NOT set.");
	}

	int weapon_type_value=flag_value( weapon_class_types,  morph_weapon_table[mwi].weapon_class );
    weapon->value[0] = weapon_type_value;
	int att=attack_lookup_with_error( morph_weapon_table[mwi].attack_type );
		if(att>=0){
        weapon->value[3]=att;
    }
	int numdie, dietype, random, wpnlevel;
	weapon->level = lvalue;
	wpnlevel	   = weapon->level;
	if (weapon->value[0] == WEAPON_DAGGER)
    {
        wpnlevel-=8;
    }
    if (weapon->value[0] == WEAPON_WHIP)
    {
        wpnlevel-=3;
    }
    if (weapon->value[0] == WEAPON_FLAIL)
    {
        wpnlevel-=2;
    }
    if (weapon->value[0] == WEAPON_AXE || weapon->value[0] == WEAPON_STAFF)
    {
        wpnlevel+=3;
    }
    if (weapon->value[0] == WEAPON_POLEARM)
    {
        wpnlevel+=8;
    }
//    if (weapon->value[0] == WEAPON_BLASTRIFLE || weapon->value[0] == WEAPON_BLASTPISTOL
//		|| weapon->value[0] == WEAPON_LIGHTBLADE)
//    {
//        wpnlevel+=6;
//    }
	if (IS_SET(weapon->value[4], WEAPON_FLAMING))
    {
        wpnlevel-=3;
    }
    if (IS_SET(weapon->value[4], WEAPON_FROST))
    {
        wpnlevel-=3;
    }
    if (IS_SET(weapon->value[4], WEAPON_SHOCKING))
    {
        wpnlevel-=3;
    }
    if (IS_SET(weapon->value[4], WEAPON_POISON))
    {
        wpnlevel-=3;
    }
    if (IS_SET(weapon->value[4], WEAPON_HOLY))
    {
        wpnlevel-=3;
    }
    if (IS_SET(weapon->value[4], WEAPON_SHARP))
    {
        wpnlevel-=4;
    }
    if (IS_SET(weapon->value[4], WEAPON_VORPAL))
    {
        wpnlevel-=4;
    }
    if (IS_SET(weapon->value[4], WEAPON_VAMPIRIC))
    {
        wpnlevel-=4;
    }
	if (IS_SET(weapon->value[4], WEAPON_SUCKLE))
    {
        wpnlevel-=4;
    }
	if (IS_SET(weapon->value[4], WEAPON_ENERVATE))
    {
        wpnlevel-=4;
    }
	if (IS_SET(weapon->value[4], WEAPON_ANNEALED))
    {
        wpnlevel-=2;
    }
	if (IS_SET(weapon->value[4], WEAPON_MORPHABLE))
    {
        wpnlevel-=4;
    }
	if (IS_SET(weapon->value[4], WEAPON_TWO_HANDS))
    {
        wpnlevel+=6;
    }
	if (IS_SET(weapon->value[4], WEAPON_ACIDIC))
    {
        wpnlevel-=3;
    }

	for ( paf = weapon->affected; paf; paf = paf->next )
	{
		switch (paf->location)
		{
		case (APPLY_DAMROLL):
		case (APPLY_HITROLL):
							wpnlevel += paf->modifier * -(1/2);
			break;
		case (APPLY_MANA):
			if ( paf->modifier > 0 )	wpnlevel += paf->modifier * -(1/10);
			if ( paf->modifier < 0 )	wpnlevel += paf->modifier * -(1/20);
			break;
		case (APPLY_PSI):
			if ( paf->modifier > 0 )	wpnlevel += paf->modifier * -(1/10);
			if ( paf->modifier < 0 )	wpnlevel += paf->modifier * -(1/20);
		case (APPLY_HIT):
			if ( paf->modifier > 0 )	wpnlevel += paf->modifier * -(1/2);
			if ( paf->modifier < 0 )	wpnlevel += paf->modifier * -(1/4);
			break;
		case (APPLY_AC):
			if ( paf->modifier > 0 )	wpnlevel += paf->modifier * -(2/10);
			if ( paf->modifier < 0 )	wpnlevel += paf->modifier * -(1/10);
			break;
		case (APPLY_SAVES):
			if ( paf->modifier > 0 )	wpnlevel += paf->modifier * -(4/10);
			if ( paf->modifier < 0 )	wpnlevel += paf->modifier * -(2/10);
			break;
		case (APPLY_ST):
		case (APPLY_QU):
		case (APPLY_PR):
		case (APPLY_EM):
		case (APPLY_IN):
		case (APPLY_CO):
		case (APPLY_AG):
		case (APPLY_SD):
		case (APPLY_ME):
		case (APPLY_RE):
							wpnlevel -= paf->modifier;
			break;
		default:
			break;
		}
	}
	/*insert checks for skills in relation to result quality here. subpar -1 wpnlevel -5silver per itemlev, superior +2 wpnlevel +10silver per itemlev*/
	switch (weapon->value[0])
    {
        case(WEAPON_SWORD):

		if (get_skill( ch, gsn_sword_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_sword) >= 90){
			skill += 25;
		}
		break;
        case(WEAPON_DAGGER):
		if (get_skill( ch, gsn_dagger_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_dagger) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_STAFF):
		if (get_skill( ch, gsn_staff_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_staff) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_MACE):
		if (get_skill( ch, gsn_mace_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_mace) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_AXE):
		if (get_skill( ch, gsn_axe_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_axe) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_FLAIL):
		if (get_skill( ch, gsn_flail_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_flail) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_WHIP):
		if (get_skill( ch, gsn_whip_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_whip) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_POLEARM):
		if (get_skill( ch, gsn_polearm_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_polearm) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_SICKLE):
		if (get_skill( ch, gsn_sickle_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_sickle) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_SPEAR):
		if (get_skill( ch, gsn_spear_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_spear) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_LONG_BOW):
		if (get_skill( ch, gsn_bow_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_bow) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_SHORT_BOW):
				if (get_skill( ch, gsn_bow_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_bow) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_CESTUS):
		if (get_skill( ch, gsn_unarmed_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_unarmed) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_KAMA):
		if (get_skill( ch, gsn_unarmed_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_unarmed) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_SIANGHAM):
		if (get_skill( ch, gsn_unarmed_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_unarmed) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_CROSSBOW):
		if (get_skill( ch, gsn_crossbow_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_crossbow) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_PISTOL):
		if (get_skill( ch, gsn_pistol_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_pistol) >= 90){
			skill += 25;
		}
			break;
        case(WEAPON_RIFLE):
		if (get_skill( ch, gsn_rifle_mastery) >= 90){
			skill += 50;
		}else if (get_skill( ch, gsn_rifle) >= 90){
			skill += 25;
		}
			break;
	}
	int subpar, superior;
	subpar =0;
	superior = 0;
	if (number_range(1,250) >= skill){
		int badresult = number_range(1,20);
		if (badresult == 1){
		wpnlevel -=1;
		subpar =1;}
	}else{
	wpnlevel +=2;
	superior =1;
	}

	if (wpnlevel >= 101){
	   wpnlevel = 100;}
	if (wpnlevel <= 1){
	   wpnlevel = 1;}


		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( wpnlevel, random, 0 );
		dietype = weapon_balance_lookup( wpnlevel, random, 1 );

		// damage lookup failed, use polarmost damage class,
		if ( numdie == 0 ||  dietype == 0 ){
		numdie  = weapon_balance_lookup( wpnlevel, 0, 0 );
		dietype = weapon_balance_lookup( wpnlevel, 0, 1 );}
		weapon->value[1]	  = numdie;
		weapon->value[2]	  = dietype;

switch (weapon->value[0])
    {
        case(WEAPON_SWORD):
			if (IS_SET(weapon->value[4], WEAPON_TWO_HANDS)){
			weapon->long_descr = str_dup( "A custom-made claymore has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made claymore" );
			weapon->name		  = str_dup( "claymore custom-made" );
			}else{
			weapon->long_descr = str_dup( "A custom-made sword has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made sword" );
			weapon->name		  = str_dup( "sword custom-made" );
			}
			break;
        case(WEAPON_DAGGER):
			weapon->long_descr = str_dup( "A custom-made dagger has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made dagger" );
			weapon->name		  = str_dup( "dagger custom-made" );
			break;
        case(WEAPON_STAFF):
			if (IS_SET(weapon->value[4], WEAPON_TWO_HANDS)){
			weapon->long_descr = str_dup( "A custom-made longstaff has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made longstaff" );
			weapon->name		  = str_dup( "longstaff custom-made" );
			}else{
			weapon->long_descr = str_dup( "A custom-made quarterstaff has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made quarterstaff" );
			weapon->name		  = str_dup( "quarterstaff custom-made" );
			}
			break;
        case(WEAPON_MACE):
			if (IS_SET(weapon->value[4], WEAPON_TWO_HANDS)){
			weapon->long_descr = str_dup( "A custom-made warmace has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made warmace" );
			weapon->name		  = str_dup( "warmace custom-made" );
			}else{
			weapon->long_descr = str_dup( "A custom-made mace has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made mace" );
			weapon->name		  = str_dup( "mace custom-made" );
			}
			break;
        case(WEAPON_AXE):
			if (IS_SET(weapon->value[4], WEAPON_TWO_HANDS)){
			weapon->long_descr = str_dup( "A custom-made greataxe has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made greataxe" );
			weapon->name		  = str_dup( "greataxe custom-made" );
			}else{
			weapon->long_descr = str_dup( "A custom-made hand axe has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made hand axe" );
			weapon->name		  = str_dup( "hand axe custom-made" );
			}
			break;
        case(WEAPON_FLAIL):
			weapon->long_descr = str_dup( "A custom-made flail has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made flail" );
			weapon->name		  = str_dup( "flail custom-made" );
			break;
        case(WEAPON_WHIP):
			weapon->long_descr = str_dup( "A custom-made whip has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made whip" );
			weapon->name		  = str_dup( "whip custom-made" );
			break;
        case(WEAPON_POLEARM):
			weapon->long_descr = str_dup( "A custom-made polearm has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made polearm" );
			weapon->name		  = str_dup( "polearm custom-made" );
			break;
        case(WEAPON_SICKLE):
			if (IS_SET(weapon->value[4], WEAPON_TWO_HANDS)){
			weapon->long_descr = str_dup( "A custom-made scythe has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made scythe" );
			weapon->name		  = str_dup( "scythe custom-made" );
			}else{
			weapon->long_descr = str_dup( "A custom-made hand sickle has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made hand sickle" );
			weapon->name		  = str_dup( "hand sickle custom-made" );
			}
			break;
        case(WEAPON_SPEAR):
			weapon->long_descr = str_dup( "A custom-made spear has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made spear" );
			weapon->name		  = str_dup( "spear custom-made" );
			break;
        case(WEAPON_LONG_BOW):
			weapon->long_descr = str_dup( "A custom-made longbow has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made longbow" );
			weapon->name		  = str_dup( "longbow custom-made" );
			break;
        case(WEAPON_SHORT_BOW):
			weapon->long_descr = str_dup( "A custom-made shortbow has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made shortbow" );
			weapon->name		  = str_dup( "shortbow custom-made" );
			break;
        case(WEAPON_CESTUS):
			weapon->long_descr = str_dup( "A custom-made combat glove has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made combat glove" );
			weapon->name		  = str_dup( "combat glove custom-made" );
			break;
        case(WEAPON_KAMA):
			weapon->long_descr = str_dup( "A custom-made kama has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made kama" );
			weapon->name		  = str_dup( "kama custom-made" );
			break;
        case(WEAPON_SIANGHAM):
			weapon->long_descr = str_dup( "A custom-made siangham has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made siangham" );
			weapon->name		  = str_dup( "siangham custom-made" );
			break;
        case(WEAPON_CROSSBOW):
			weapon->long_descr = str_dup( "A custom-made crossbow has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made crossbow" );
			weapon->name		  = str_dup( "crossbow custom-made" );
			break;
        case(WEAPON_PISTOL):
			weapon->long_descr = str_dup( "A custom-made pistol has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made pistol" );
			weapon->name		  = str_dup( "pistol custom-made" );
			break;
        case(WEAPON_RIFLE):
			weapon->long_descr = str_dup( "A custom-made rifle has been dropped here." );
			weapon->short_descr = str_dup( "a custom-made rifle" );
			weapon->name		  = str_dup( "rifle custom-made" );
			break;
	}
    matcost = matcost * material->value[0];
	if(!metal_material){
	SET_BIT(weapon->extra_flags, OBJEXTRA_NONMETAL);}
	weapon->cost = matcost;
	if (superior >=1){
	weapon->cost += (10*weapon->level);
	}
	if (subpar >=1){
	weapon->cost -= (5*weapon->level);
	}
	if (weapon->cost <= 5){
	weapon->cost = 5;
	}

	if (!str_prefix(arg,"blastpistol")){
	weapon->value[0]	  = 573;
	weapon->value[1]	  = 575;
	weapon->value[2]	  = 581;
	}
	if (!str_prefix(arg,"blastrifle")){
	weapon->value[0]	  = 573;
	weapon->value[1]	  = 576;
	weapon->value[2]	  = 582;
	}

	obj_to_char(weapon, ch);
	ch->printlnf("You successfully craft %s!", weapon->short_descr);
	check_improve(ch, gsn_weaponsmith, true, 1);
	weapon->craftername=strdup(ch->name);
	material->weight -= weapon->weight;
	material->value[1] -= svalue;
	material->cost -= matcost;
	if (material->value[1] <= 0 || material->weight <= -1 || material->cost <= 0){
	ch->println ("Your creation expends the last of the usable material in that pile");
	extract_obj(material);
	}
	return;
    // echo syntax
    do_weaponsmith(ch,"");
    return;
}

/**************************************************************************/
/**************************************************************************/
void do_armorsmith( char_data *ch, char *argument )
{
	char arg[MIL];
	char arg1 [MIL];
	char arg2 [MIL];
	char arg3 [MIL];
	char arg4 [MIL];
	OBJ_DATA *material;
	OBJ_DATA *armor;
	int svalue;
	int lvalue;
	AFFECT_DATA *paf;
	int matcost = 0;
	argument = one_argument(argument,arg);
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	strcpy( arg4, argument );
	bool wood_material = false;
	bool stone_material = false;
	bool metal_material = false;
	bool organic_material = false;
	bool textile_material = false;
	bool crystal_material = false;
	bool gem_material = false;

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	if ( arg[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
	{
		ch->println( "Syntax:" );
		ch->println( "  armorsmith material location level size" );
		ch->println( "  Armor locations:  Arms, Hands, Head, Feet, Legs, Body, Shield" );
		return;
	}

	int skill=get_skill(ch, gsn_armorsmith);
	if(skill<1){
		ch->println("What would you know about making armor?");
		return;}

	obj_data *tool;
	bool ftool=false;
    for ( tool = ch->carrying; tool; tool = tool->next_content ){
        if ( tool->item_type == ITEM_TOOL
			 && tool->wear_loc != WEAR_NONE
			 && ( tool->value[1] == gsn_armorsmith
			 ||   tool->value[2] == gsn_armorsmith
			 ||   tool->value[3] == gsn_armorsmith
			 ||   tool->value[4] == gsn_armorsmith )) {
			ftool = true;
            break;}
    }

	if(!ftool){
		ch->println("You need to be holding an armorsmithing tool in order to create armor.");
		return;}

	// lets see what they can make
	if (tool->value[0] >= 1){
		skill += tool->value[0];
	}


	if (str_prefix(arg1,"legs") && str_prefix(arg1,"feet")
	    && str_prefix(arg1,"arms") && str_prefix(arg1,"hands")
	    && str_prefix(arg1,"head") && str_prefix(arg1,"shield")
	    && str_prefix(arg1,"body")){
		ch->printlnf("Armor location %s not valid!", arg1);
	return;}

	char *orecheck = "ore";
	if ( ( material = get_obj_carry( ch, arg ) ) == NULL ){
		ch->printlnf("You do not have any %s.",arg);
		return;}
	if (material->item_type != ITEM_MATERIAL){
		ch->printlnf("Your %s is not a material.",material->short_descr);
		return;}
	if (is_name( orecheck, material->name )){
		ch->printlnf("Your %s is not a refined material.",material->short_descr);
		return;}
	if ( !is_number( arg2) ){
       ch->println("Numeric level is required.");
       return;
    }
	lvalue = atoi( arg2);
    if ( lvalue > material->value[0] )
    {
		ch->println("Material purity not high enough.");
		return;
    }
	if (lvalue > skill){
		ch->println("Your skill is not high enough.");
		return;
    }
	if(!strcmp(arg3,"diminutive")){
		svalue = 1;
	}else if(!strcmp(arg3,"tiny")){
		svalue = 2;
	}else if(!strcmp(arg3,"small")){
		svalue = 3;
	}else if(!strcmp(arg3,"medium")){
		svalue = 4;
	}else if(!strcmp(arg3,"large")){
		svalue = 5;
	}else if(!strcmp(arg3,"giant")){
		svalue = 6;
	}else if(!strcmp(arg3,"gargantuan")){
		svalue = 7;
	}else{
		ch->println("Incorrect size value.");
		ch->println("Use: diminutive, tiny, small, medium,");
		ch->println("large, giant, or gargantuan.");
		return;}
	if ((svalue*2) > material->value[1]){
		ch->println("Not enough material.");
		return;
	}
	//METALS
	if(!str_cmp(material->material,"metal-adamantium")
	|| !str_cmp(material->material,"metal-copper")
	|| !str_cmp(material->material,"metal-iron")
	|| !str_cmp(material->material,"metal-lead")
	|| !str_cmp(material->material,"metal-platinum")
	|| !str_cmp(material->material,"metal-silver")
	|| !str_cmp(material->material,"metal-gold")
	|| !str_cmp(material->material,"metal-titanium")
	|| !str_cmp(material->material,"metal-indurium")
	|| !str_cmp(material->material,"metal-arcadium")
	|| !str_cmp(material->material,"metal-zinc")
	|| !str_cmp(material->material,"metal-tin")
	|| !str_cmp(material->material,"metal-starmetal")
	|| !str_cmp(material->material,"metal-mithril")
	|| !str_cmp(material->material,"metal-mithral")
	|| !str_cmp(material->material,"metal-magnetiron")
	|| !str_cmp(material->material,"metal-moonsilver")
	|| !str_cmp(material->material,"metal-astral driftmetal")
	|| !str_cmp(material->material,"metal-abyssal bloodiron")
	|| !str_cmp(material->material,"metal-blightiron")
	|| !str_cmp(material->material,"metal-stormbane iron")
	|| !str_cmp(material->material,"metal-sylvan silver")){
		metal_material = true;
	}
	//ALLOYS
	if(!str_cmp(material->material,"alloy-rune bronze")
	|| !str_cmp(material->material,"alloy-froststeel")
	|| !str_cmp(material->material,"alloy-greensteel")
	|| !str_cmp(material->material,"alloy-psisteel")
	|| !str_cmp(material->material,"alloy-brass")
	|| !str_cmp(material->material,"alloy-pewter")
	|| !str_cmp(material->material,"alloy-steel")
	|| !str_cmp(material->material,"alloy-bronze")
	|| !str_cmp(material->material,"alloy-solar truesteel")
	|| !str_cmp(material->material,"alloy-bloodsteel")
	|| !str_cmp(material->material,"alloy-ember arcadium")
	|| !str_cmp(material->material,"alloy-magemetal")
	|| !str_cmp(material->material,"alloy-white gold")
	|| !str_cmp(material->material,"alloy-sterling silver")
	|| !str_cmp(material->material,"alloy-rose gold")
	|| !str_cmp(material->material,"alloy-electrum")
	|| !str_cmp(material->material,"alloy-black mithril")
	|| !str_cmp(material->material,"alloy-cast iron")){
		metal_material = true;
	}
	//GEMS
	if(!str_cmp(material->material,"gem-amethyst")
	|| !str_cmp(material->material,"gem-ruby")
	|| !str_cmp(material->material,"gem-turqoise")
	|| !str_cmp(material->material,"gem-sapphire")
	|| !str_cmp(material->material,"gem-pearl")
	|| !str_cmp(material->material,"gem-opal")
	|| !str_cmp(material->material,"gem-onyx")
	|| !str_cmp(material->material,"gem-emerald")
	|| !str_cmp(material->material,"gem-diamond")
	|| !str_cmp(material->material,"gem-jade")
	|| !str_cmp(material->material,"gem-bloodstone")
	|| !str_cmp(material->material,"gem-amber")
	|| !str_cmp(material->material,"gem-quartz")
	|| !str_cmp(material->material,"gem-agate")
	|| !str_cmp(material->material,"gem-aquamarine")
	|| !str_cmp(material->material,"gem-beryl")
	|| !str_cmp(material->material,"gem-carnelian")
	|| !str_cmp(material->material,"gem-chalcedony")
	|| !str_cmp(material->material,"gem-chrysoberyl")
	|| !str_cmp(material->material,"gem-citrine")
	|| !str_cmp(material->material,"gem-darkstone")
	|| !str_cmp(material->material,"gem-garnet")
	|| !str_cmp(material->material,"gem-golden beryl")
	|| !str_cmp(material->material,"gem-jasper")
	|| !str_cmp(material->material,"gem-lapis lazuli")
	|| !str_cmp(material->material,"gem-malachite")
	|| !str_cmp(material->material,"gem-moonstone")
	|| !str_cmp(material->material,"gem-moss agate")
	|| !str_cmp(material->material,"gem-peridot")
	|| !str_cmp(material->material,"gem-scarlet emerald")
	|| !str_cmp(material->material,"gem-spinel")
	|| !str_cmp(material->material,"gem-sunstone")
	|| !str_cmp(material->material,"gem-tiger's-eye")
	|| !str_cmp(material->material,"gem-topaz")
	|| !str_cmp(material->material,"gem-common")
	|| !str_cmp(material->material,"gem-miscellaneous")){
		gem_material = true;
	}
	//ROCKS
	if(!str_cmp(material->material,"rock-greenstone")
	|| !str_cmp(material->material,"rock-obsidian")
	|| !str_cmp(material->material,"rock-common")
	|| !str_cmp(material->material,"rock-sugar")
	|| !str_cmp(material->material,"rock-basalt")
	|| !str_cmp(material->material,"rock-miscellaneous")
	|| !str_cmp(material->material,"rock-gneiss")
	|| !str_cmp(material->material,"rock-shale")){
		stone_material = true;
	}
	//ORGANICS
	if(!str_cmp(material->material,"organic-scale")
	|| !str_cmp(material->material,"organic-dragonscale")
	|| !str_cmp(material->material,"organic-bone")
	|| !str_cmp(material->material,"organic-coral")
	|| !str_cmp(material->material,"organic-jet")
	|| !str_cmp(material->material,"organic-shell")
	|| !str_cmp(material->material,"organic-bone-ivory")
	|| !str_cmp(material->material,"organic-bone-dragonbone")
	|| !str_cmp(material->material,"organic-ice")
	|| !str_cmp(material->material,"organic-feather")
	|| !str_cmp(material->material,"organic-feather-dragonfeather")
	|| !str_cmp(material->material,"organic-hide")
	|| !str_cmp(material->material,"organic-hide-dragonhide")
	|| !str_cmp(material->material,"organic-nacre")
	|| !str_cmp(material->material,"organic-skin")
	|| !str_cmp(material->material,"organic-skin-dragonskin")){
		organic_material = true;
	}
	//MINERAL
	if(!str_cmp(material->material,"mineral-blended quartz")
	|| !str_cmp(material->material,"mineral-dolomite")
	|| !str_cmp(material->material,"mineral-salt")){
		stone_material = true;
	}
	//CRYSTAL
	if(!str_cmp(material->material,"crystal-glass")){
		crystal_material = true;
	}
	//TEXTILES
	if(!str_cmp(material->material,"textile-hemp")
	|| !str_cmp(material->material,"textile-leather")
	|| !str_cmp(material->material,"textile-seaweed")
	|| !str_cmp(material->material,"textile-bamboo")
	|| !str_cmp(material->material,"textile-brocade")
	|| !str_cmp(material->material,"textile-bronze wool")
	|| !str_cmp(material->material,"textile-burlap")
	|| !str_cmp(material->material,"textile-goldthread")
	|| !str_cmp(material->material,"textile-mesh")
	|| !str_cmp(material->material,"textile-silverthread")
	|| !str_cmp(material->material,"textile-spider silk")
	|| !str_cmp(material->material,"textile-steel wool")
	|| !str_cmp(material->material,"textile-straw")
	|| !str_cmp(material->material,"textile-sharkskin")){
		textile_material = true;
	}

	//HEARTWOOD
	if(!str_cmp(material->material,"heartwood-ashwood")
	|| !str_cmp(material->material,"heartwood-balsa")
	|| !str_cmp(material->material,"heartwood-birch")
	|| !str_cmp(material->material,"heartwood-bronzewood")
	|| !str_cmp(material->material,"heartwood-cedar")
	|| !str_cmp(material->material,"heartwood-cherrywood")
	|| !str_cmp(material->material,"heartwood-cypress")
	|| !str_cmp(material->material,"heartwood-darkwood")
	|| !str_cmp(material->material,"heartwood-ebony")
	|| !str_cmp(material->material,"heartwood-elmwood")
	|| !str_cmp(material->material,"heartwood-eucalyptus")
	|| !str_cmp(material->material,"heartwood-hickory")
	|| !str_cmp(material->material,"heartwood-ironwood")
	|| !str_cmp(material->material,"heartwood-mahogany")
	|| !str_cmp(material->material,"heartwood-hard maple")
	|| !str_cmp(material->material,"heartwood-soft maple")
	|| !str_cmp(material->material,"heartwood-mindwood")
	|| !str_cmp(material->material,"heartwood-red oak")
	|| !str_cmp(material->material,"heartwood-white oak")
	|| !str_cmp(material->material,"heartwood-pine")
	|| !str_cmp(material->material,"heartwood-redwood")
	|| !str_cmp(material->material,"heartwood-rosewood")
	|| !str_cmp(material->material,"heartwood-sandalwood")
	|| !str_cmp(material->material,"heartwood-steelwood")
	|| !str_cmp(material->material,"heartwood-teak")
	|| !str_cmp(material->material,"heartwood-walnut")
	|| !str_cmp(material->material,"heartwood-willowwood")
	|| !str_cmp(material->material,"heartwood-yew")
	|| !str_cmp(material->material,"heartwood-common")
	|| !str_cmp(material->material,"heartwood-miscellaneous")
	|| !str_cmp(material->material,"heartwood-applewood")){
		wood_material = true;
	}
	//SAPWOOD
	if(!str_cmp(material->material,"sapwood-ashwood")
	|| !str_cmp(material->material,"sapwood-balsa")
	|| !str_cmp(material->material,"sapwood-birch")
	|| !str_cmp(material->material,"sapwood-bronzewood")
	|| !str_cmp(material->material,"sapwood-cedar")
	|| !str_cmp(material->material,"sapwood-cherrywood")
	|| !str_cmp(material->material,"sapwood-cypress")
	|| !str_cmp(material->material,"sapwood-darkwood")
	|| !str_cmp(material->material,"sapwood-ebony")
	|| !str_cmp(material->material,"sapwood-elmwood")
	|| !str_cmp(material->material,"sapwood-eucalyptus")
	|| !str_cmp(material->material,"sapwood-hickory")
	|| !str_cmp(material->material,"sapwood-ironwood")
	|| !str_cmp(material->material,"sapwood-mahogany")
	|| !str_cmp(material->material,"sapwood-hard maple")
	|| !str_cmp(material->material,"sapwood-soft maple")
	|| !str_cmp(material->material,"sapwood-mindwood")
	|| !str_cmp(material->material,"sapwood-red oak")
	|| !str_cmp(material->material,"sapwood-white oak")
	|| !str_cmp(material->material,"sapwood-pine")
	|| !str_cmp(material->material,"sapwood-redwood")
	|| !str_cmp(material->material,"sapwood-rosewood")
	|| !str_cmp(material->material,"sapwood-sandalwood")
	|| !str_cmp(material->material,"sapwood-steelwood")
	|| !str_cmp(material->material,"sapwood-teak")
	|| !str_cmp(material->material,"sapwood-walnut")
	|| !str_cmp(material->material,"sapwood-willowwood")
	|| !str_cmp(material->material,"sapwood-yew")
	|| !str_cmp(material->material,"sapwood-common")
	|| !str_cmp(material->material,"sapwood-miscellaneous")
	|| !str_cmp(material->material,"sapwood-applewood")){
		wood_material = true;
	}
	//MONOCOT
	if(!str_cmp(material->material,"monocot-bamboo")
	|| !str_cmp(material->material,"monocot-palm")){
		wood_material = true;
	}

	if(!wood_material && !metal_material
	   && !stone_material && !organic_material
	   && !textile_material && !crystal_material
	   && !gem_material){
	ch->println("The selected material can not be used for armors.");
	return;}
/*
	if (wood_material){
	}
	if (metal_material){
	}
	if (stone_material){
	}
	if (organic_material){
	}
	if (textile_material){
	}
	if (crystal_material){
	}
	if (gem_material){
	}
*/
	if (!str_prefix(arg1,"legs")){
		armor = create_object(get_obj_index(541));
	}else if (!str_prefix(arg1,"feet")){
		armor = create_object(get_obj_index(540));
	}else if (!str_prefix(arg1,"arms")){
		armor = create_object(get_obj_index(544));
	}else if (!str_prefix(arg1,"hands")){
		armor = create_object(get_obj_index(542));
	}else if (!str_prefix(arg1,"head")){
		armor = create_object(get_obj_index(545));
	}else if (!str_prefix(arg1,"shield")){
		armor = create_object(get_obj_index(535));
	}else{  //body
		armor = create_object(get_obj_index(543));
	}

/* armor piece vnums
[  535] A crafted shield              [  536] Some back armor
[  537] Some wrist armor              [  538] Some face armor
[  539] Some waist armor              [  540] Some foot armor
[  541] Some leg armor                [  542] Some hand armor
[  543] Some body armor               [  544] Some arm armor
[  545] Some head armor
*/
	affects_from_template_to_obj(armor);
	armor->material = str_dup(material->material);
	armor->relative_size = svalue;
	svalue = svalue*2;

	matcost = 0;
	bool hit_found = false, dam_found = false;
	bool mana_found = false, health_found = false;
	bool move_found = false, psi_found = false;
	bool defense_found = false;

	 for (paf=OBJECT_AFFECTS(armor); paf; paf=paf->next )
    {
        if ( paf->location == APPLY_HITROLL ){
            hit_found = true;
        }
		if (paf->location == APPLY_DAMROLL ){
            dam_found = true;
        }
		if (paf->location == APPLY_MANA ){
            mana_found = true;
        }
		if (paf->location == APPLY_PSI ){
            psi_found = true;
        }
		if (paf->location == APPLY_HIT ){
            health_found = true;
        }
		if (paf->location == APPLY_MOVE ){
            move_found = true;
        }
		if (paf->location == APPLY_AC ){
            defense_found = true;
        }
    }

	if (metal_material){
	armor->extra_flags= OBJEXTRA_BURN_PROOF;
	//METALS
	if(!str_cmp(armor->material,"metal-adamantium")){
	armor->weight = 2 * svalue;
	matcost = 1050 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra_flags= OBJEXTRA_NO_DEGRADE;
	armor->extra2_flags= OBJEXTRA2_NODECAY;
	}else if(!str_cmp(armor->material,"metal-iron")){
	armor->weight = 3 * svalue;
	matcost = 12 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"metal-silver")){
	armor->weight = 4 * svalue;
	matcost = 10 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"metal-gold")){
	armor->weight = 7 * svalue;
	matcost = 100 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"metal-titanium")){
	armor->weight = 2 * svalue;
	matcost = 1100 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra2_flags= OBJEXTRA2_NODECAY;
	}else if(!str_cmp(armor->material,"metal-indurium")){
	armor->weight = 2 * svalue;
	matcost = 1200 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra_flags= OBJEXTRA_NO_DEGRADE | OBJEXTRA_HUM;
	armor->extra2_flags= OBJEXTRA2_NODECAY;
	}else if(!str_cmp(armor->material,"metal-arcadium")){
	armor->weight = 0;
	matcost = 1300 * svalue;
//	matcost = matcost * material->value[0];
	armor->relative_size = 50;
	}else if(!str_cmp(armor->material,"metal-tin")){
	armor->weight = 3 * svalue;
	matcost = 8 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"metal-astral driftmetal")){
	armor->weight = 1 * svalue;
	matcost = 800 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra_flags= OBJEXTRA_ANTI_EVIL | OBJEXTRA_GLOW;
	}else if(!str_cmp(armor->material,"metal-abyssal bloodiron")){
	armor->weight = 3 * svalue;
	matcost = 850 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra_flags= OBJEXTRA_ANTI_GOOD;
	}else if(!str_cmp(armor->material,"metal-starmetal")){
	armor->weight = 6 * svalue;
	matcost = 9000 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"metal-mithril")){
	armor->weight = 2 * svalue;
	matcost = 150 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra_flags= OBJEXTRA_NO_DEGRADE;
	armor->extra2_flags= OBJEXTRA2_NODECAY;
	}else if(!str_cmp(armor->material,"metal-mithral")){
	armor->weight = 2 * svalue;
	matcost = 50 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"metal-blightiron")){
	armor->weight = 4 * svalue;
	matcost = 850 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra_flags= OBJEXTRA_ANTI_EVIL | OBJEXTRA_ANTI_GOOD;
	}else if(!str_cmp(armor->material,"metal-stormbane iron")){
	armor->weight = 3 * svalue;
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	//add lightning resist
	paf = new_affect();
	paf->where        = WHERE_RESIST;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = RES_LIGHTNING;
	paf->next         = armor->affected;
	armor->affected   = paf;
	}else if(!str_cmp(armor->material,"metal-magnetiron")){
	armor->weight = 3 * svalue;
	matcost = 50 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"metal-sylvan silver")){
	armor->weight = 3 * svalue;
	matcost = 100 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"metal-zinc")){
	armor->weight = 3 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	//add lightning vuln
	paf = new_affect();
	paf->where        = WHERE_VULN;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = VULN_LIGHTNING;
	paf->next         = armor->affected;
	armor->affected   = paf;
	}
	if(!str_cmp(armor->material,"metal-moonsilver")){
	armor->weight = 4 * svalue;
	matcost = 120 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra_flags= OBJEXTRA_GLOW;
	}
	if(!str_cmp(armor->material,"metal-copper")){
	armor->weight = 3 * svalue;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	//add lightning vuln
	paf = new_affect();
	paf->where        = WHERE_VULN;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = VULN_LIGHTNING;
	paf->next         = armor->affected;
	armor->affected   = paf;
	}
	//ALLOYS
	if(!str_cmp(armor->material,"alloy-brass")){
	armor->weight = 3 * svalue;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"alloy-bronze")){
	armor->weight = 3 * svalue;
	matcost = 9 * svalue;
//	matcost = matcost * material->value[0];
	//add fire vuln
	paf = new_affect();
	paf->where        = WHERE_VULN;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = VULN_FIRE;
	paf->next         = armor->affected;
	armor->affected   = paf;
	//add lightning vuln
	paf = new_affect();
	paf->where        = WHERE_VULN;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = VULN_LIGHTNING;
	paf->next         = armor->affected;
	armor->affected   = paf;
	}else if(!str_cmp(armor->material,"alloy-steel")){
	armor->weight = 3 * svalue;
	matcost = 15 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"alloy-psisteel")){
	armor->weight = 2 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	if (psi_found){
		for ( paf = armor->affected; paf != NULL; paf = paf->next){
			if ( paf->location == APPLY_PSI){
        	        paf->type = -1;
        	        paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
	paf = new_affect();
	paf->where        = WHERE_OBJEXTRA;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_PSI;
        paf->modifier     =  lvalue/2;
	paf->bitvector    = 0;
	paf->next         = armor->affected;
	armor->affected   = paf;}
	}else if(!str_cmp(armor->material,"alloy-bloodsteel")){
	armor->weight = 4 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra_flags= OBJEXTRA_ANTI_NEUTRAL | OBJEXTRA_GLOW | OBJEXTRA_ANTI_GOOD;
	}else if(!str_cmp(armor->material,"alloy-solar truesteel")){
	armor->weight = 0;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	if (mana_found){
		for ( paf = armor->affected; paf != NULL; paf = paf->next){
				if ( paf->location == APPLY_MANA){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
	paf = new_affect();
	paf->where        = WHERE_OBJEXTRA;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_MANA;
        paf->modifier     = lvalue/2;
	paf->bitvector    = 0;
	paf->next         = armor->affected;
	armor->affected   = paf;}
	armor->relative_size = 50;
	armor->extra_flags= OBJEXTRA_ANTI_NEUTRAL | OBJEXTRA_GLOW | OBJEXTRA_ANTI_EVIL;
	}else if(!str_cmp(armor->material,"alloy-greensteel")){
	armor->weight = 4 * svalue;
	matcost = 25 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra_flags= OBJEXTRA_ROT_DEATH;
	if (move_found){
		for ( paf = armor->affected; paf != NULL; paf = paf->next){
				if ( paf->location == APPLY_MOVE){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
	paf = new_affect();
	paf->where        = WHERE_OBJEXTRA;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_MOVE;
        paf->modifier     = lvalue/2;
	paf->bitvector    = 0;
	paf->next         = armor->affected;
	armor->affected   = paf;}
	}else if(!str_cmp(armor->material,"alloy-froststeel")){
	armor->weight = 3 * svalue;
	matcost = 23 * svalue;
//	matcost = matcost * material->value[0];
	//add fire vuln
	paf = new_affect();
	paf->where        = WHERE_VULN;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = VULN_FIRE;
	paf->next         = armor->affected;
	armor->affected   = paf;
	//add cold resist
	paf               = new_affect();
	paf->where        = WHERE_RESIST;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = RES_COLD;
	paf->next         = armor->affected;
	armor->affected   = paf;
	}else if(!str_cmp(armor->material,"alloy-rune bronze")){
	armor->weight = 8 * svalue;
	matcost = 70 * svalue;
//	matcost = matcost * material->value[0];
	//add fire vuln
	paf = new_affect();
	paf->where        = WHERE_VULN;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = VULN_FIRE;
	paf->next         = armor->affected;
	armor->affected   = paf;
	//add lightning vuln
	paf = new_affect();
	paf->where        = WHERE_VULN;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = VULN_LIGHTNING;
	paf->next         = armor->affected;
	armor->affected   = paf;
	if (mana_found){
		for ( paf = armor->affected; paf != NULL; paf = paf->next){
			if ( paf->location == APPLY_MANA){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
	paf = new_affect();
	paf->where        = WHERE_OBJEXTRA;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_MANA;
        paf->modifier     = lvalue/2;
	paf->bitvector    = 0;
	paf->next         = armor->affected;
	armor->affected   = paf;}
	}

	if(!str_cmp(armor->material,"alloy-black mithril")){
	armor->weight = 4 * svalue;
	matcost = 1200 * svalue;
//	matcost = matcost * material->value[0];
	armor->extra_flags= OBJEXTRA_BURN_PROOF;

	}else if(!str_cmp(armor->material,"alloy-cast iron")){
	armor->weight = 6 * svalue;
	matcost = 17 * svalue;
//	matcost = matcost * material->value[0];
	if (dam_found){
		for ( paf = armor->affected; paf != NULL; paf = paf->next){
			if ( paf->location == APPLY_DAMROLL){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
	paf = new_affect();
	paf->where        = WHERE_OBJEXTRA;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_DAMROLL;
        paf->modifier     = lvalue/2;
	paf->bitvector    = 0;
	paf->next         = armor->affected;
	armor->affected   = paf;}
	}else if(!str_cmp(armor->material,"alloy-electrum")){
	armor->weight = 11 * svalue;
	matcost = 110 * svalue;
//	matcost = matcost * material->value[0];
	//add lightning vuln
	paf               = new_affect();
	paf->where        = WHERE_VULN;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = VULN_LIGHTNING;
	paf->next         = armor->affected;
	armor->affected   = paf;
	}else if(!str_cmp(armor->material,"alloy-white gold")){
	armor->weight = 10 * svalue;
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}
	}

	if (wood_material){
	armor->weight = 3 * svalue;
	matcost = 15 * svalue;
//	matcost = matcost * material->value[0];

	if(!str_cmp(armor->material,"sapwood-bamboo")
	|| !str_cmp(armor->material,"heartwood-bamboo")){
	//add water resist
	paf               = new_affect();
	paf->where        = WHERE_RESIST;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = RES_DROWNING;
	paf->next         = armor->affected;
	armor->affected   = paf;
	}else if (!str_cmp(armor->material,"sapwood-bronzewood")
	|| !str_cmp(armor->material,"heartwood-bronzewood")){
	armor->extra_flags= OBJEXTRA_BURN_PROOF;
	//add fire vuln
	paf = new_affect();
	paf->where        = WHERE_VULN;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = VULN_LIGHTNING;
	paf->next         = armor->affected;
	armor->affected   = paf;
	}else if (!str_cmp(armor->material,"sapwood-cypress")
	|| !str_cmp(armor->material,"heartwood-cypress")){
	SET_BIT(armor->extra2_flags, OBJEXTRA2_NODECAY);
	}else if (!str_cmp(armor->material,"sapwood-elmwood")
	|| !str_cmp(armor->material,"heartwood-elmwood")){
	SET_BIT(armor->extra2_flags, OBJEXTRA2_NODECAY);
	}else if (!str_cmp(armor->material,"sapwood-ironwood")
	|| !str_cmp(armor->material,"heartwood-ironwood")){
	armor->extra_flags= OBJEXTRA_BURN_PROOF;
	}else if (!str_cmp(armor->material,"sapwood-mindwood")
	|| !str_cmp(armor->material,"heartwood-mindwood")){
	if (psi_found){
		for ( paf = armor->affected; paf != NULL; paf = paf->next){
			if ( paf->location == APPLY_PSI){
                paf->type = -1;
                paf->modifier += lvalue/2;}
				}
	}else{ // add a new affect
	paf = new_affect();
	paf->where        = WHERE_OBJEXTRA;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_PSI;
        paf->modifier     =  lvalue/2;
	paf->bitvector    = 0;
	paf->next         = armor->affected;
	armor->affected   = paf;}
	}else if (!str_cmp(armor->material,"sapwood-redwood")
	|| !str_cmp(armor->material,"heartwood-redwood")){
	//add fire resist
	paf               = new_affect();
	paf->where        = WHERE_RESIST;
	paf->type         = -1;
	paf->duration     = -1;
	paf->location     = APPLY_NONE;
	paf->modifier     = 0;
	paf->bitvector    = RES_FIRE;
	paf->next         = armor->affected;
	armor->affected   = paf;
	}else if (!str_cmp(armor->material,"sapwood-steelwood")
	|| !str_cmp(armor->material,"heartwood-steelwood")){
	armor->extra_flags= OBJEXTRA_BURN_PROOF;
	}
	}


	if (gem_material){
	armor->weight = 2 * svalue;
	if(!str_cmp(armor->material,"gem-agate")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-amethyst")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-aquamarine")){
	matcost = 240 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-beryl")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-bloodstone")){
	matcost = 500 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-carnelian")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-chalcedony")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-chrysoberyl")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-citrine")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-darkstone")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-diamond")){
	matcost = 1000 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-emerald")){
	matcost = 1000 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-garnet")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-golden beryl")){
	matcost = 500 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-jade")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-jasper")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-lapis lazuli")){
	matcost = 800 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-malachite")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-moonstone")){
	matcost = 1000 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-moss agate")){
	matcost = 500 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-onyx")){
	matcost = 600 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-opal")){
	matcost = 700 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-pearl")){
	matcost = 720 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-peridot")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-quartz")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-ruby")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-sapphire")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-scarlet emerald")){
	matcost = 1200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-spinel")){
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-sunstone")){
	matcost = 2500 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-tiger's-eye")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-topaz")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-turquoise")){
	matcost = 250 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-common")){
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"gem-miscellaneous")){
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	}
	}

	if (organic_material){
	if(!str_cmp(armor->material,"organic-scale")){
	armor->weight = 10 * svalue;
	matcost = 10 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"organic-dragonscale")){
	armor->weight = 15 * svalue;
	matcost = 300 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"organic-bone")){
	armor->weight = 5 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"organic-coral")){
	armor->weight = 1 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"organic-jet")){
	armor->weight = 5 * svalue;
	matcost = 10 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"organic-shell")){
	armor->weight = 10 * svalue;
	matcost = 6 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"organic-bone-ivory")){
	armor->weight = 1 * svalue;
	matcost = 20 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"organic-bone-dragonbone")){
	armor->weight = 10 * svalue;
	matcost = 400 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"organic-ice")){
	armor->weight = 1 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}
	}

	if (stone_material){
	if(!str_cmp(armor->material,"rock-common")){
	armor->weight = 1 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"rock-miscellaneous")){
	armor->weight = 1 * svalue;
	matcost = 4 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"rock-basalt")){
	armor->weight = 1 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"rock-greenstone")){
	armor->weight = 1 * svalue;
	matcost = 25 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"rock-obsidian")){
	armor->weight = 1 * svalue;
	matcost = 200 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"rock-sugar")){
	armor->weight = 1 * svalue;
	matcost = svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"rock-gneiss")){
	armor->weight = 1 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"rock-shale")){
	armor->weight = 1 * svalue;
	matcost = 2 * svalue;
//	matcost = matcost * material->value[0];
	}
	}

	if (crystal_material){
	//glass is only crystal material usable for armors
	armor->weight = 0;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	}

	if (textile_material){
	if(!str_cmp(armor->material,"textile-hemp")){
	armor->weight = 0;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"textile-leather")){
	armor->weight = 0;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	}else if(!str_cmp(armor->material,"textile-seaweed")){
	armor->weight = 0;
	matcost = 5 * svalue;
//	matcost = matcost * material->value[0];
	}
	}





	int /*numdie, dietype, random, */aclevel;
	armor->level = lvalue;
	aclevel	   = armor->level;

	for ( paf = armor->affected; paf; paf = paf->next )
	{
		switch (paf->location)
		{
		case (APPLY_DAMROLL):
		case (APPLY_HITROLL):
							aclevel += paf->modifier * -(1/2);
			break;
		case (APPLY_MANA):
			if ( paf->modifier > 0 )	aclevel += paf->modifier * -(1/10);
			if ( paf->modifier < 0 )	aclevel += paf->modifier * -(1/20);
			break;
		case (APPLY_PSI):
			if ( paf->modifier > 0 )	aclevel += paf->modifier * -(1/10);
			if ( paf->modifier < 0 )	aclevel += paf->modifier * -(1/20);
		case (APPLY_HIT):
			if ( paf->modifier > 0 )	aclevel += paf->modifier * -(1/2);
			if ( paf->modifier < 0 )	aclevel += paf->modifier * -(1/4);
			break;
		case (APPLY_AC):
			if ( paf->modifier > 0 )	aclevel += paf->modifier * -(2/10);
			if ( paf->modifier < 0 )	aclevel += paf->modifier * -(1/10);
			break;
		case (APPLY_SAVES):
			if ( paf->modifier > 0 )	aclevel += paf->modifier * -(4/10);
			if ( paf->modifier < 0 )	aclevel += paf->modifier * -(2/10);
			break;
		case (APPLY_ST):
		case (APPLY_QU):
		case (APPLY_PR):
		case (APPLY_EM):
		case (APPLY_IN):
		case (APPLY_CO):
		case (APPLY_AG):
		case (APPLY_SD):
		case (APPLY_ME):
		case (APPLY_RE):
							aclevel -= paf->modifier;
			break;
		default:
			break;
		}
	}
	/*insert checks for skills in relation to result quality here. subpar -1 wpnlevel -5silver per itemlev, superior +2 wpnlevel +10silver per itemlev*/

	int subpar, superior;
	subpar =0;
	superior = 0;
	if (number_range(1,250) >= skill){
		int badresult = number_range(1,20);
		if (badresult == 1){
		aclevel -=1;
		subpar =1;}
	}else{
	aclevel +=2;
	superior =1;
	}

	if (aclevel >= 101){
	   aclevel = 100;}
	if (aclevel <= 1){
	   aclevel = 1;}

        if ( IS_SET(armor->wear_flags, OBJWEAR_SHIELD)){
		armor->long_descr = str_dup( "A custom-made shield has been dropped here." );
		armor->short_descr = str_dup( "a custom-made shield" );
		armor->name		  = str_dup( "shield custom-made" );
		armor->value[0] = aclevel/3;
		armor->value[1] = aclevel/3;
		armor->value[2] = aclevel/3;
		armor->value[3] = aclevel/4;
		}
        if ( IS_SET(armor->wear_flags, OBJWEAR_ARMS)){
		armor->long_descr = str_dup( "Some custom-made arm armor has been dropped here." );
		armor->short_descr = str_dup( "some custom-made arm armor" );
		armor->name		  = str_dup( "arm armor custom-made" );
		armor->value[0] = aclevel/4;
		armor->value[1] = aclevel/4;
		armor->value[2] = aclevel/4;
		armor->value[3] = aclevel/5;
		}
		if ( IS_SET(armor->wear_flags, OBJWEAR_HANDS)){
		armor->long_descr = str_dup( "Some custom-made hand armor has been dropped here." );
		armor->short_descr = str_dup( "some custom-made hand armor" );
		armor->name		  = str_dup( "hand armor custom-made" );
		armor->value[0] = aclevel/5;
		armor->value[1] = aclevel/5;
		armor->value[2] = aclevel/5;
		armor->value[3] = aclevel/6;
		}
        if ( IS_SET(armor->wear_flags, OBJWEAR_FEET)){
		armor->long_descr = str_dup( "Some custom-made foot armor has been dropped here." );
		armor->short_descr = str_dup( "some custom-made foot armor" );
		armor->name		  = str_dup( "foot armor custom-made" );
		armor->value[0] = aclevel/5;
		armor->value[1] = aclevel/5;
		armor->value[2] = aclevel/5;
		armor->value[3] = aclevel/6;
		}
        if ( IS_SET(armor->wear_flags, OBJWEAR_LEGS)){
		armor->long_descr = str_dup( "Some custom-made leg armor has been dropped here." );
		armor->short_descr = str_dup( "some custom-made leg armor" );
		armor->name		  = str_dup( "leg armor custom-made" );
		armor->value[0] = aclevel/4;
		armor->value[1] = aclevel/4;
		armor->value[2] = aclevel/4;
		armor->value[3] = aclevel/5;
		}
        if ( IS_SET(armor->wear_flags, OBJWEAR_HEAD)){
		armor->long_descr = str_dup( "Some custom-made head armor has been dropped here." );
		armor->short_descr = str_dup( "some custom-made head armor" );
		armor->name		  = str_dup( "head armor custom-made" );
		armor->value[0] = aclevel/4;
		armor->value[1] = aclevel/4;
		armor->value[2] = aclevel/4;
		armor->value[3] = aclevel/5;
		}
        if ( IS_SET(armor->wear_flags, OBJWEAR_TORSO)){
		armor->long_descr = str_dup( "Some custom-made body armor has been dropped here." );
		armor->short_descr = str_dup( "some custom-made body armor" );
		armor->name		  = str_dup( "body armor custom-made" );
		armor->value[0] = aclevel/3;
		armor->value[1] = aclevel/3;
		armor->value[2] = aclevel/3;
		armor->value[3] = aclevel/4;
        }

    matcost = matcost * material->value[0];
	if(!metal_material){
	SET_BIT(armor->extra_flags, OBJEXTRA_NONMETAL);}
	armor->cost = matcost;
	if (superior >=1){
	armor->cost += (10*armor->level);
	}
	if (subpar >=1){
	armor->cost -= (5*armor->level);
	}
	if (armor->cost <= 5){
	armor->cost = 5;
	}
	obj_to_char(armor, ch);
	ch->printlnf("You successfully craft %s!", armor->short_descr);
	check_improve(ch, gsn_armorsmith, true, 1);
	armor->craftername=strdup(ch->name);
	material->weight -= armor->weight;
	material->value[1] -= svalue;
	material->cost -= matcost;
	if (material->value[1] <= 0 || material->weight <= -1 || material->cost <= 0){
	ch->println ("Your creation expends the last of the usable material in that pile");
	extract_obj(material);
	}
	return;
    // echo syntax
    do_armorsmith(ch,"");
    return;
}

/**************************************************************************/
/**************************************************************************/
void do_metalwork( char_data *ch, char *argument )
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    char arg1[MIL];
    char arg2[MIL];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    OBJ_DATA *alloy;

	int bpurity1;
	int bunits1;
	int bweight1;

	int bpurity2;
	int bunits2;
	int bweight2;

	int cpurity;

	int alloycost;
	int alloyunits;
	int alloypurity;
	int alloyweight;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	int skill=get_skill(ch, gsn_metalwork);
	if(skill<1){
		ch->println("What would you know about working metal?");
		return;}
	ch->println("This feature is currently being reworked.");
		return;
	obj_data *tool;
	bool ftool=false;
    for ( tool = ch->carrying; tool; tool = tool->next_content ){
        if ( tool->item_type == ITEM_TOOL
			 && tool->wear_loc != WEAR_NONE
			 && ( tool->value[1] == gsn_metalwork
			 ||   tool->value[2] == gsn_metalwork
			 ||   tool->value[3] == gsn_metalwork
			 ||   tool->value[4] == gsn_metalwork )) {
			ftool = true;
            break;}
    }

	if(!ftool){
		ch->println("You need to be holding a metalworking tool in order to process alloys.");
		return;}

	// lets see what they can find
	if (tool->value[0] >= 1){
		skill += tool->value[0];
	}

    if ( arg1[0] == '\0' || arg2[0] == '\0' ){
		ch->println("Work which two materials?");
		return;}

	if ( ( obj1 = get_obj_carry( ch, arg1 ) ) == NULL ){
		ch->printlnf("You do not have any %s.",arg1);
		return;}

	if ( ( obj2 = get_obj_carry( ch, arg2 ) ) == NULL ){
		ch->printlnf("You do not have any %s.",arg2);
		return;}

	if (obj1->item_type != ITEM_MATERIAL){
		ch->printlnf("Your %s is not a material.",arg1);
		return;}

	if (obj2->item_type != ITEM_MATERIAL){
		ch->printlnf("Your %s is not a material.",arg2);
		return;}

	bunits1 = obj1->value[1];
	bunits2 = obj2->value[1];

	if(bunits1 <= 19){
		ch->println("You must have at least 20 units of the first material.");
		return;}
	if(bunits2 <= 19){
		ch->println("You must have at least 20 units of the second material.");
		return;}
	if(bunits1 != bunits2){
		ch->println("You must have equal units of each material.");
		return;}

	bpurity1 = obj1->value[0];
	bpurity2 = obj2->value[0];
	cpurity = (bpurity1 + bpurity2);
	cpurity = cpurity/2;
	alloypurity = number_range(1,cpurity);
	bweight2 = obj2->weight;
	bweight1 = obj1->weight;
	alloyunits = (bunits1 + bunits2);
	alloyunits = alloyunits/4;

	if((obj1->pIndexData->vnum == 1461 && obj2->pIndexData->vnum == 1397)||
		(obj2->pIndexData->vnum == 1461 && obj1->pIndexData->vnum == 1397)){
		alloy = create_object(get_obj_index(1438));
		ch->println("You make some steel alloy.");}
	else if((obj1->pIndexData->vnum == 1435 && obj2->pIndexData->vnum == 1421)||
		(obj2->pIndexData->vnum == 1435 && obj1->pIndexData->vnum == 1421)){
		alloy = create_object(get_obj_index(1440));
		ch->println("You make some psisteel alloy.");}
	else if((obj1->pIndexData->vnum == 1419 && obj2->pIndexData->vnum == 1411)||
		(obj2->pIndexData->vnum == 1419 && obj1->pIndexData->vnum == 1411)){
		alloy = create_object(get_obj_index(1441));
		ch->println("You make some bloodsteel alloy.");}
	else if((obj1->pIndexData->vnum == 1417 && obj2->pIndexData->vnum == 1411)||
		(obj2->pIndexData->vnum == 1417 && obj1->pIndexData->vnum == 1411)){
		alloy = create_object(get_obj_index(1442));
		ch->println("You make some solar truesteel alloy.");}
	else if((obj1->pIndexData->vnum == 1421 && obj2->pIndexData->vnum == 1468)||
		(obj2->pIndexData->vnum == 1421 && obj1->pIndexData->vnum == 1468)){
		alloy = create_object(get_obj_index(1443));
		ch->println("You make some greensteel alloy.");}
	else if((obj1->pIndexData->vnum == 1427 && obj2->pIndexData->vnum == 1409)||
		(obj2->pIndexData->vnum == 1427 && obj1->pIndexData->vnum == 1409)){
		alloy = create_object(get_obj_index(1444));
		ch->println("You make some froststeel alloy.");}
	else if((obj1->pIndexData->vnum == 1429 && obj2->pIndexData->vnum == 1437)||
		(obj2->pIndexData->vnum == 1429 && obj1->pIndexData->vnum == 1437)){
		alloy = create_object(get_obj_index(1445));
		ch->println("You make some runebronze alloy.");}
	else if((obj1->pIndexData->vnum == 1413 && obj2->pIndexData->vnum == 1395)||
		(obj2->pIndexData->vnum == 1413 && obj1->pIndexData->vnum == 1395)){
		alloy = create_object(get_obj_index(1436));
		ch->println("You make some brass alloy.");}
	else if((obj1->pIndexData->vnum == 1395 && obj2->pIndexData->vnum == 1415)||
		(obj2->pIndexData->vnum == 1395 && obj1->pIndexData->vnum == 1415)){
		alloy = create_object(get_obj_index(1437));
		ch->println("You make some bronze alloy.");}
	else if((obj1->pIndexData->vnum == 1399 && obj2->pIndexData->vnum == 1415)||
		(obj2->pIndexData->vnum == 1399 && obj1->pIndexData->vnum == 1415)){
		alloy = create_object(get_obj_index(1439));
		ch->println("You make some pewter alloy.");}
	else if((obj1->pIndexData->vnum == 1423 && obj2->pIndexData->vnum == 1393)||
		(obj2->pIndexData->vnum == 1423 && obj1->pIndexData->vnum == 1393)){
		alloy = create_object(get_obj_index(1391));
		ch->println("You make some black mithril alloy.");}
	else if((obj1->pIndexData->vnum == 1405 && obj2->pIndexData->vnum == 1415)||
		(obj2->pIndexData->vnum == 1405 && obj1->pIndexData->vnum == 1403)){
		alloy = create_object(get_obj_index(1389));
		ch->println("You make some electrum alloy.");}
	else if((obj1->pIndexData->vnum == 1395 && obj2->pIndexData->vnum == 1405)||
		(obj2->pIndexData->vnum == 1395 && obj1->pIndexData->vnum == 1405)){
		alloy = create_object(get_obj_index(1387));
		ch->println("You make some rose gold alloy.");}
	else if((obj1->pIndexData->vnum == 1433 && obj2->pIndexData->vnum == 1405)||
		(obj2->pIndexData->vnum == 1433 && obj1->pIndexData->vnum == 1405)){
		alloy = create_object(get_obj_index(1386));
		ch->println("You make some white gold alloy.");}
	else if((obj1->pIndexData->vnum == 1401 && obj2->pIndexData->vnum == 1411)||
		(obj2->pIndexData->vnum == 1401 && obj1->pIndexData->vnum == 1411)){
		alloy = create_object(get_obj_index(1388));
		ch->println("You make some magemetal alloy.");}
	else if((obj1->pIndexData->vnum == 1403 && obj2->pIndexData->vnum == 1395)||
		(obj2->pIndexData->vnum == 1403 && obj1->pIndexData->vnum == 1395)){
		alloy = create_object(get_obj_index(1385));
		ch->println("You make some sterling silver alloy.");}
	else if((obj1->pIndexData->vnum == 1411 && obj2->pIndexData->vnum == 1393)||
		(obj2->pIndexData->vnum == 1411 && obj1->pIndexData->vnum == 1393)){
		alloy = create_object(get_obj_index(1390));
		ch->println("You make some ember arcadium alloy.");}
	else if((obj1->pIndexData->vnum == 1395 && obj2->pIndexData->vnum == 1397)||
		(obj2->pIndexData->vnum == 1395 && obj1->pIndexData->vnum == 1397)){
		alloy = create_object(get_obj_index(1384));
		ch->println("You make some ember arcadium alloy.");}
	else{
		ch->println("You fail to make an alloy.");
		return;}

	check_improve(ch, gsn_metalwork, true, 3);
	if (alloyunits <= 0){
		alloyunits = 1;}
	if (alloypurity <= 0){
		alloypurity = 1;}
	alloycost = (alloy->pIndexData->cost * alloypurity);
	alloycost = (alloycost * alloyunits);
	alloyweight = (alloy->pIndexData->weight * alloyunits);
	alloy->weight = alloyweight;
	alloy->value[0] = alloypurity;
	alloy->value[1] = alloyunits;
	alloy->cost		= alloycost;
	obj_to_char(alloy, ch);
	extract_obj( obj1);
	extract_obj( obj2);
	return;
}
/**************************************************************************/
/**************************************************************************/
void do_mine(char_data *ch, char*argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	OBJ_DATA *ore;
	ore = create_object(get_obj_index(1467));
	if(IS_NULLSTR(argument)){
		ch->println("Syntax: mine ore  - to attempt to mine the room for ore.");
		ch->println("Note: In order to have any success, you must have the mining skill and a pickaxe.");
		return;}
    if (!GAMESETTING4(GAMESET4_RESOURCE_GATHERING_ENABLED)){
        ch->println("This feature is currently being reworked.");
		return;}
	if( str_prefix(argument, "ore")){
		ch->println("Only ore can be mined.");
		return;}

	if(IS_NPC(ch) || !ch->in_room){
		ch->println("Can't do that here.");
		return;}

	if( IS_AFFECTED(ch,AFF_BLIND)){
		ch->println("How would you propose to do that while being blind?");
		return;}

	if(ch->in_room->sector_type != SECT_HILLS
		&& ch->in_room->sector_type != SECT_MOUNTAIN
		&& ch->in_room->sector_type != SECT_DESERT
		&& ch->in_room->sector_type != SECT_CAVE
		|| !IS_SET( ch->in_room->room2_flags, ROOM2_RESOURCES )){
		ch->println("Can't do that here.");
		return;}

	// check if they have the skill to mine
	int skill=get_skill(ch, gsn_mining);
	if(skill<1){
		ch->println("What would you know about mining?");
		return;}

	// check they are holding/wielding a pick axe
	obj_data *tool;
	bool ftool=false;
    for ( tool = ch->carrying; tool; tool = tool->next_content ){
        if ( tool->item_type == ITEM_TOOL
			 && tool->wear_loc != WEAR_NONE
			 && ( tool->value[1] == gsn_mining
			 ||   tool->value[2] == gsn_mining
			 ||   tool->value[3] == gsn_mining
			 ||   tool->value[4] == gsn_mining )) {
			ftool = true;
            break;}
    }

	if(!ftool){
		ch->println("You need to be holding a mining tool in order to mine ores.");
		return;}

	// lets see what they can find
	if (tool->value[0] >= 1){
		skill += tool->value[0];
	}
	// apply the lag here, so we don't have to do it for every result
	int lag=10*PULSE_PER_SECOND;// a default amount of lag, if one isn't specified
	if(skill_table[gsn_mining].beats){
		lag=skill_table[gsn_mining].beats;}
	if(IS_IMMORTAL(ch)){// imms get 1 second lag regardless
		lag=PULSE_PER_SECOND;}
	WAIT_STATE(ch, lag);
	lag/=PULSE_PER_SECOND; // get lag into seconds

	act("You start to mine with $p...",ch,tool, NULL,TO_CHAR);
	act("$n starts to mine with $p.",ch,tool, NULL,TO_ROOM);

	// if mine has been used in the last 15 minutes, they get nothing
	if(ch->in_room->last_mined_in_room+600>current_time){
		ch->println(lag,"You attempt to mine for a while with no success, this location appears to have already been mined recently.");
		return;}

	// decide on the type of ore they get, before skill mods on these values
	//nothing	11%	11
	//rarity1	39%	50
	//rarity2	17%	67
	//rarity3	16%	83
	//rarity4	14%	97
	//rarity5	3%	100
	//skill adds up to 10% in the value
	int rarecategory=number_range(-10,90) + URANGE(0, skill/10, 10); // random between 1 and 100

	if(rarecategory<11){ // nothing - they failed, don't mark the room as mined
		ch->println(lag,"You mine for quite some time, but fail to find anything usable.");
		check_improve(ch, gsn_mining, false, 3);
		return;}
	int oretype;
	// mark the room as mined, since they are guaranteed success beyond here
	if(rarecategory<50){
		oretype=1;
	}else if(rarecategory<67){
		oretype=number_range(1,2);
	}else if(rarecategory<83){
		oretype=number_range(1,3);
	}else if(rarecategory<97){
		oretype=number_range(1,4);
	}else{
		oretype=number_range(1,5);}


//	OBJ_DATA *ore;
	int oreresult;
	switch(ch->in_room->sector_type){
	case SECT_HILLS:
		if (oretype == 1){
		oreresult = number_range(1,5);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1467));
		break;
		case 2: ore = create_object(get_obj_index(1465));
		break;
		case 3: ore = create_object(get_obj_index(1463));
		break;
		case 4: ore = create_object(get_obj_index(1464));
		break;
		case 5: ore = create_object(get_obj_index(1455));
		break;}
	}else if (oretype == 2){
		oreresult = number_range(1,3);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1451));
		break;
		case 2: ore = create_object(get_obj_index(1466));
		break;
		case 3: ore = create_object(get_obj_index(1447));
		break;}
	}else if (oretype == 3){
		oreresult = number_range(1,5);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1446));
		break;
		case 2: ore = create_object(get_obj_index(1448));
		break;
		case 3: ore = create_object(get_obj_index(1453));
		break;
		case 4: ore = create_object(get_obj_index(1457));
		break;
		case 5: ore = create_object(get_obj_index(1469));
		break;}
	}else if (oretype == 4){
		oreresult = number_range(1,4);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1450));
		break;
		case 2: ore = create_object(get_obj_index(1452));
		break;
		case 3: ore = create_object(get_obj_index(1454));
		break;
		case 4: ore = create_object(get_obj_index(1456));
		break;}
	}else{
		oreresult = number_range(1,3);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1458));
		break;
		case 2: ore = create_object(get_obj_index(1468));
		break;
		case 3: ore = create_object(get_obj_index(1449));
		break;}
	}
	break;
	case SECT_MOUNTAIN:
	case SECT_CAVE:
	if (oretype == 1){
		oreresult = number_range(1,6);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1394));
		break;
		case 2: ore = create_object(get_obj_index(1396));
		break;
		case 3: ore = create_object(get_obj_index(1398));
		break;
		case 4: ore = create_object(get_obj_index(1412));
		break;
		case 5: ore = create_object(get_obj_index(1414));
		break;
		case 6: ore = create_object(get_obj_index(1461));
		break;}
	}else if (oretype == 2){
		oreresult = number_range(1,3);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1430));
		break;
		case 2: ore = create_object(get_obj_index(1424));
		break;
		case 3: ore = create_object(get_obj_index(1402));
		break;}
	}else if (oretype == 3){
		oreresult = number_range(1,2);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1404));
		break;
		case 2: ore = create_object(get_obj_index(1426));
		break;}
	}else if (oretype == 4){
		oreresult = number_range(1,7);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1422));
		break;
		case 2: ore = create_object(get_obj_index(1432));
		break;
		case 3: ore = create_object(get_obj_index(1420));
		break;
		case 4: ore = create_object(get_obj_index(1418));
		break;
		case 5: ore = create_object(get_obj_index(1416));
		break;
		case 6: ore = create_object(get_obj_index(1406));
		break;
		case 7: ore = create_object(get_obj_index(1400));
		break;}
	}else{
		oreresult = number_range(1,5);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1392));
		break;
		case 2: ore = create_object(get_obj_index(1408));
		break;
		case 3: ore = create_object(get_obj_index(1410));
		break;
		case 4: ore = create_object(get_obj_index(1428));
		break;
		case 5: ore = create_object(get_obj_index(1434));
		break;}
	}
	break;

	case SECT_DESERT:
		if (oretype == 1){
		oreresult = number_range(1,5);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1467));
		break;
		case 2: ore = create_object(get_obj_index(1459));
		break;
		case 3: ore = create_object(get_obj_index(1462));
		break;
		case 4: ore = create_object(get_obj_index(1464));
		break;
		case 5: ore = create_object(get_obj_index(1455));
		break;}
	}else if (oretype == 2){
		oreresult = number_range(1,3);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1451));
		break;
		case 2: ore = create_object(get_obj_index(1466));
		break;
		case 3: ore = create_object(get_obj_index(1447));
		break;}
	}else if (oretype == 3){
		oreresult = number_range(1,5);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1446));
		break;
		case 2: ore = create_object(get_obj_index(1448));
		break;
		case 3: ore = create_object(get_obj_index(1453));
		break;
		case 4: ore = create_object(get_obj_index(1457));
		break;
		case 5: ore = create_object(get_obj_index(1469));
		break;}
	}else if (oretype == 4){
		oreresult = number_range(1,4);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1450));
		break;
		case 2: ore = create_object(get_obj_index(1452));
		break;
		case 3: ore = create_object(get_obj_index(1454));
		break;
		case 4: ore = create_object(get_obj_index(1456));
		break;}
	}else{
		oreresult = number_range(1,2);
		switch(oreresult){
		case 1: ore = create_object(get_obj_index(1468));
		break;
		case 2: ore = create_object(get_obj_index(1449));
		break;}
	}
	break;}

	int purity;
	int units;
	int weight;
	int cost;
	purity = number_range(1,skill);
	purity = purity/2;
	if (purity <= 0){
		purity = 1;}

	units = number_range(1,skill);
	units = units/2;
	units = units/2;
	if (units <= 0){
		units = 1;}

	weight = (ore->pIndexData->weight * units);
	cost = (ore->pIndexData->cost * purity);
	cost = (cost * units);
	ore->weight = weight;
	ore->value[0] = purity;
	ore->value[1] = units;
	ore->cost	= cost;

	SET_BIT(ore->wear_flags, OBJWEAR_TAKE|OBJWEAR_HOLD);
	check_improve(ch, gsn_mining, true, 5);
	ch->printlnf(lag, "You have unearthed %s.", ore->short_descr);
	act("$n has unearthed $p.", ch, ore, NULL, TO_ROOM);

	// record the last time the room was mined
	ch->in_room->last_mined_in_room=current_time;

    if( ch->carry_number + get_obj_number( ore ) > can_carry_n( ch )){
		ch->println(lag, "You are carrying too many items to pick up your unearthed ore.");
		obj_to_room(ore, ch->in_room);
		return;
    }

    if ( !IS_SWITCHED (ch) && get_carry_weight(ch) + get_obj_weight( ore ) > can_carry_w( ch ) )
    {
		ch->println(lag, "You are already carrying too much weight to carry this ore.");
		obj_to_room(ore, ch->in_room);
		return;
    }

	ch->printlnf(lag, "You pick up %s.", ore->short_descr);
	act("$n picks up $p.", ch, ore, NULL, TO_ROOM);
	obj_to_char(ore, ch);
    return;
}
/**************************************************************************/
void do_lumberjack(char_data *ch, char*argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	OBJ_DATA *wood;
	wood = create_object(get_obj_index(1492));
	if(IS_NULLSTR(argument)){
		ch->println("Syntax: lumberjack wood  - to attempt to chop trees for wood.");
		ch->println("Note: In order to have any success, you must have the lumberjack skill and a woodcutting axe.");
		return;}
    if (!GAMESETTING4(GAMESET4_RESOURCE_GATHERING_ENABLED)){
        ch->println("This feature is currently being reworked.");
		return;}
    if( str_prefix(argument, "wood")){
		ch->println("Only wood can be chopped.");
		return;}

	if(IS_NPC(ch) || !ch->in_room){
		ch->println("Can't do that here.");
		return;}

	if( IS_AFFECTED(ch,AFF_BLIND)){
		ch->println("How would you propose to do that while being blind?");
		return;}

	if(ch->in_room->sector_type != SECT_FOREST
		&& ch->in_room->sector_type != SECT_SWAMP
		|| !IS_SET( ch->in_room->room2_flags, ROOM2_RESOURCES )){
		ch->println("Can't do that here.");
		return;}

	// check if they have the skill to cut wood
	int skill=get_skill(ch, gsn_lumberjacking);
	if(skill<1){
		ch->println("What would you know about chopping wood?");
		return;}

	// check they are holding/wielding the right axe
	obj_data *tool;
	bool ftool=false;
    for ( tool = ch->carrying; tool; tool = tool->next_content ){
        if ( tool->item_type == ITEM_TOOL
			 && tool->wear_loc != WEAR_NONE
			 && ( tool->value[1] == gsn_lumberjacking
			 ||   tool->value[2] == gsn_lumberjacking
			 ||   tool->value[3] == gsn_lumberjacking
			 ||   tool->value[4] == gsn_lumberjacking )) {
			ftool = true;
            break;}
    }

	if(!ftool){
		ch->println("You need to be holding a woodcutting axe in order to lumberjack.");
		return;}

	// lets see what they can find
	if (tool->value[0] >= 1){
		skill += tool->value[0];
	}
	// apply the lag here, so we don't have to do it for every result
	int lag=10*PULSE_PER_SECOND;// a default amount of lag, if one isn't specified
	if(skill_table[gsn_lumberjacking].beats){
		lag=skill_table[gsn_lumberjacking].beats;}
	if(IS_IMMORTAL(ch)){// imms get 1 second lag regardless
		lag=PULSE_PER_SECOND;}
	WAIT_STATE(ch, lag);
	lag/=PULSE_PER_SECOND; // get lag into seconds

	act("You start to cut wood with $p...",ch,tool, NULL,TO_CHAR);
	act("$n starts to cut wood with $p.",ch,tool, NULL,TO_ROOM);

	// if lumberjacking has been used in the last 15 minutes, they get nothing
	if(ch->in_room->last_mined_in_room+600>current_time){
		ch->println(lag,"You attempt to cut wood for a while with no success, the wood here appears to need time to regrow.");
		return;}

	// decide on the type of ore they get, before skill mods on these values
	//nothing	11%	11
	//rarity1	39%	50
	//rarity2	17%	67
	//rarity3	16%	83
	//rarity4	14%	97
	//rarity5	3%	100
	//skill adds up to 10% in the value
	int rarecategory=number_range(-10,90) + URANGE(0, skill/10, 10); // random between 1 and 100

	if(rarecategory<11){ // nothing - they failed, don't mark the room as mined
		ch->println(lag,"You cut for quite some time, but fail to find anything usable.");
		check_improve(ch, gsn_lumberjacking, false, 3);
		return;}
	int woodtype;
	// mark the room as mined, since they are guaranteed success beyond here
	if(rarecategory<50){
		woodtype=1;
	}else if(rarecategory<67){
		woodtype=number_range(1,2);
	}else if(rarecategory<83){
		woodtype=number_range(1,3);
	}else if(rarecategory<97){
		woodtype=number_range(1,4);
	}else{
		woodtype=number_range(1,5);}

	int purity;
	int units;
	int weight;
	int cost;
//	OBJ_DATA *ore;
	int woodresult;
	switch(ch->in_room->sector_type){
	case SECT_FOREST:
	if (woodtype == 1){
		woodresult = number_range(1,5);
		switch(woodresult){
		case 1: wood = create_object(get_obj_index(1492));/*ashwood*/
		break;
		case 2: wood = create_object(get_obj_index(1493));/*bamboo*/
		break;
		case 3: wood = create_object(get_obj_index(1494));/*oak*/
		break;
		case 4: wood = create_object(get_obj_index(1495));/*pine*/
		break;
		case 5: wood = create_object(get_obj_index(1496));/*cedar*/
		break;}
	}else if (woodtype == 2){
		wood = create_object(get_obj_index(1500));/*darkwood*/
	}else if (woodtype == 3){
		wood = create_object(get_obj_index(1499));/*bronzewood*/
	}else if (woodtype == 4){
		wood = create_object(get_obj_index(1497));/*steelwood*/
	}else{
		wood = create_object(get_obj_index(1498));/*mindwood*/
	break;}

	break;
	case SECT_SWAMP:
		if (woodtype == 1){
		woodresult = number_range(1,5);
		switch(woodresult){
		case 1: wood = create_object(get_obj_index(1492));/*ashwood*/
		break;
		case 2: wood = create_object(get_obj_index(1493));/*bamboo*/
		break;
		case 3: wood = create_object(get_obj_index(1494));/*oak*/
		break;
		case 4: wood = create_object(get_obj_index(1495));/*pine*/
		break;
		case 5: wood = create_object(get_obj_index(1496));/*cedar*/
		break;}
	}else if (woodtype == 2){
		wood = create_object(get_obj_index(1500));/*darkwood*/
	}else if (woodtype == 3){
		wood = create_object(get_obj_index(1499));/*bronzewood*/
	}else if (woodtype == 4){
		wood = create_object(get_obj_index(1497));/*steelwood*/
	}else{
		wood = create_object(get_obj_index(1498));/*mindwood*/
	break;}

	break;}

	purity = number_range(1,skill);
	purity = purity/2;
	if (purity <= 0){
		purity = 1;}

	units = number_range(1,skill);
	units = units/2;
	units = units/2;
	if (units <= 0){
		units = 1;}

	weight = (wood->pIndexData->weight * units);
	cost = (wood->pIndexData->cost * purity);
	cost = (cost * units);
	wood->weight = weight;
	wood->value[0] = purity;
	wood->value[1] = units;
	wood->cost	= cost;

	SET_BIT(wood->wear_flags, OBJWEAR_TAKE|OBJWEAR_HOLD);
	check_improve(ch, gsn_lumberjacking, true, 5);
	ch->printlnf(lag, "You have chopped %s.", wood->short_descr);
	act("$n has chopped $p.", ch, wood, NULL, TO_ROOM);

	// record the last time the room was lumberjacked
	ch->in_room->last_mined_in_room=current_time;

    if( ch->carry_number + get_obj_number( wood ) > can_carry_n( ch )){
		ch->println(lag, "You are carrying too many items to pick up your chopped wood.");
		obj_to_room(wood, ch->in_room);
		return;
    }

    if ( !IS_SWITCHED (ch) && get_carry_weight(ch) + get_obj_weight( wood ) > can_carry_w( ch ) )
    {
		ch->println(lag, "You are already carrying too much weight to carry this wood.");
		obj_to_room(wood, ch->in_room);
		return;
    }

	ch->printlnf(lag, "You pick up %s.", wood->short_descr);
	act("$n picks up $p.", ch, wood, NULL, TO_ROOM);
	obj_to_char(wood, ch);
	return;
}
/**************************************************************************/
/**************************************************************************/
void do_combinematerial( char_data *ch, char *argument )
{
    char arg1[MIL];
    char arg2[MIL];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
	OBJ_DATA *newo;
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	if ( arg1[0] == '\0' || arg2[0] == '\0' ){
		ch->println("Combine which two stacks?");
		return;}

	if ( ( obj1 = get_obj_carry( ch, arg1 ) ) == NULL ){
		ch->printlnf("You do not have any %s.",arg1);
		return;}

	if ( ( obj2 = get_obj_carry( ch, arg2 ) ) == NULL ){
		ch->printlnf("You do not have any %s.",arg2);
		return;}

	if (obj1->item_type != ITEM_MATERIAL){
		ch->printlnf("Your %s is not a material.",arg1);
		return;}
	if (obj2->item_type != ITEM_MATERIAL){
		ch->printlnf("Your %s is not a material.",arg2);
		return;}
	if(obj1->pIndexData->vnum == obj2->pIndexData->vnum){

		newo = create_object(get_obj_index(obj1->pIndexData->vnum));
		ch->println("You combine your material.");}
	else{
		ch->println("You must use stacks of the same material.");
		return;}

	int bpurity1;
	int bunits1;
	int bweight1;
	int bpurity2;
	int bunits2;
	int bweight2;

	int newcost;
	int newunits;
	int newpurity;
	int newweight;

	bunits1 = obj1->value[1];
	bunits2 = obj2->value[1];
	bpurity1 = obj1->value[0];
	bpurity2 = obj2->value[0];
	bweight2 = obj2->weight;
	bweight1 = obj1->weight;
	newpurity = number_range(bpurity1,bpurity2);
	newunits = (bunits1 + bunits2);


	if (newunits <= 0){
		newunits = 1;}
	if (newpurity <= 0){
		newpurity = 1;}
	newcost = (obj1->pIndexData->cost * newpurity);
	newcost = (newcost * newunits);
	newweight = (bweight1 + bweight2);
	newo->weight = newweight;
	newo->value[0] = newpurity;
	newo->value[1] = newunits;
	newo->cost	= newcost;
	obj_to_char(newo, ch);
	extract_obj( obj1);
	extract_obj( obj2);
	return;
}
/**************************************************************************/
/**************************************************************************/
void do_blacksmith( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_butchering( char_data *ch, char * argument)
{
    if (!GAMESETTING4(GAMESET4_RESOURCE_GATHERING_ENABLED)){
    ch->println("This feature is currently being reworked.");
	return;}
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_distilling( char_data *ch, char * argument)
{
	ch->println( "Huh?" );
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_farming( char_data *ch, char * argument)
{
	if (!GAMESETTING4(GAMESET4_RESOURCE_GATHERING_ENABLED)){
    ch->println("This feature is currently being reworked.");
	return;}

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_gemcraft( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_glassblowing( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_keycraft( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_leatherworking( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_luthier( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_magecraft( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_pottery( char_data *ch, char * argument)
{
	ch->println( "Huh?" );
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_stoneworking( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_tailoring( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_tanning( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_waxcrafting( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_weaving( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/**************************************************************************/
void do_woodcraft( char_data *ch, char * argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }
	ch->println( "Huh?" );
    return;
}
/**************************************************************************/
/************************** END OF CRAFTS *********************************/
/**************************************************************************/
void do_powers(char_data *ch, char *argument)
{
	char *skill_list[MAX_LEVEL+2];
    unsigned char skill_columns[MAX_LEVEL+2];
	char linebuf[MSL];
    int sn,lev;
    char arg[MIL];
    char_data *victim;
    char buf[MSL];
    BUFFER *output;
	int top_level=0;
    bool found = false;

	if (IS_UNSWITCHED_MOB(ch)){
		return;
	}

    one_argument( argument, arg );

    if (IS_IMMORTAL(ch) && !IS_NULLSTR(arg))
    {
		if ( ( victim = get_char_world( ch, arg ) ) == NULL )
		{
			ch->println( "They aren't here." );
			return;
		}
    }
	else
	{
		victim=ch;
	}
/*
    if (IS_NPC(victim))
	{
		ch->println( "Mobs don't have percentages YET." );
		return;
	}
*/

    output = new_buf();
	{
		if(ch->desc && ch->desc->connected_state!=CON_PLAYING){
			if(ch==victim){
				sprintf(buf,"`=j%s", creation_titlebar("POWERS"));
			}else{
				sprintf(buf,"`=j%s", creation_titlebar("POWERS FOR %s", uppercase(victim->name)));
			}
		}else{
			if(ch==victim){
				sprintf(buf,"`=t%s", format_titlebar("POWERS"));
			}else{
				sprintf(buf,"`=t%s", format_titlebarf("POWERS FOR %s", uppercase(victim->name)));
			}
		}

		buf[str_len(buf)-2]= '\0';
		strcat(buf,"`x");
	}
	add_buf( output, buf);

	if (ch!=victim)
	{
//		sprintf(buf,"\r\nPowers for %s (Level: %d, Race: %s, Class: %s)",
		sprintf(buf,"\r\nPowers for %s (Level: %d, Race: %s)",
			victim->name,
			victim->level,
			race_table[victim->race]->name
//			class_table[victim->clss].name
			);
		add_buf( output, buf);
	}

    // initialize data
    for (lev = 0; lev < MAX_LEVEL+2; lev++){
        skill_columns[lev] = 0;
        skill_list[lev]= str_dup("");
    }

	if (IS_IMMORTAL(ch)){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}

	bool specially_granted_skills=false;
    for (sn = 0; !IS_NULLSTR(skill_table[sn].name) ; sn++)
    {
		if (skill_table[sn].spell_fun == spell_null
			&& skill_table[sn].sktype==SKTYPE_PSIONIC
			&&	(
					(victim->pcdata->learned[sn]==101)
				  ||
					(skill_table[sn].skill_level[victim->clss]<=top_level
					&& skill_table[sn].skill_level[victim->clss]>0)
			//		&& victim->pcdata->learned[sn] > 0)
				)
			)
		{
			if(victim->pcdata->learned[sn]==101
				&& skill_table[sn].skill_level[victim->clss]>LEVEL_HERO)
			{
				specially_granted_skills=true;
			}

			found = true;
			lev = skill_table[sn].skill_level[victim->clss];
			if(IS_SET(skill_table[sn].flags,SKFLAGS_NEW_IMPROVE_SYSTEM)){
				if (victim->level < lev){
					sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
				}else{
					sprintf(buf,"%-18s %3d%%(%3d%%,%3d%%)       ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn],
						skill_table[sn].learn_scale_percent[victim->clss],
						(skill_table[sn].learn_scale_percent[victim->clss]==0?
								100:skill_table[sn].learn_scale_percent[ch->clss]));
				}
			}else{
				if (victim->level < lev && victim->pcdata->learned[sn]!=101){
					if(ch->level<lev){
						sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
					}else{
						sprintf(buf,"%-18s  n/a(%3d%%)            ",
							capitalize(skill_table[sn].name),
							victim->pcdata->learned[sn]);
					}
				}else{
					sprintf(buf,"%-18s %3d%%                  ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn]);
				}
			}

			if (IS_NULLSTR(skill_list[lev])){
				sprintf(linebuf,"\r\n Level %2d: %s",lev,buf);
				replace_string(skill_list[lev], linebuf);
			}else{ // append
				if ( ++skill_columns[lev] % 2 == 0){
					sprintf(linebuf,"%s\r\n           ",skill_list[lev]);
					replace_string(skill_list[lev], linebuf);
				}else{
					// trim off the extra spaces
					strcpy(linebuf, skill_list[lev]);
					linebuf[str_len(linebuf)-6]='\0';
					replace_string(skill_list[lev], linebuf);
				}
				sprintf(linebuf,"%s%s",skill_list[lev],buf);
				replace_string(skill_list[lev], linebuf);
			}
		}
    }

	if (IS_IMMORTAL(ch) || specially_granted_skills){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}


    // return results
    if (!found){
		if (ch==victim){
			ch->println( "You know no powers." );
		}else{
			ch->println( "They know no powers." );
		}
    }else{
		for (lev = 1; lev < top_level; lev++){
			if(lev==LEVEL_IMMORTAL){
				if(IS_IMMORTAL(ch)){
					sprintf(buf, "\r\n\r\n%s", format_titlebar("POWERS DISABLED FOR CHARACTER UNLESS GRANTED BELOW HERE"));
				}else{
					sprintf(buf, "\r\n\r\n%s", format_titlebar("SPECIALLY GRANTED POWERS"));
				}
				buf[str_len(buf)-2]= '\0';
				strcat(buf,"`x");
				add_buf( output, buf);
			}
			if (!IS_NULLSTR(skill_list[lev])){
				add_buf( output, skill_list[lev] );
			}
		}
		add_buf( output, "\r\n");
		ch->sendpage(buf_string(output));
		free_buf(output);
	}

	// deallocate all memory used
	for (lev = 0; lev < MAX_LEVEL+2; lev++){
        free_string(skill_list[lev]);
    }
	return;
}

/**************************************************************************/
void do_ctechs(char_data *ch, char *argument)
{
	char *skill_list[MAX_LEVEL+2];
    unsigned char skill_columns[MAX_LEVEL+2];
	char linebuf[MSL];
    int sn,lev;
    char arg[MIL];
    char_data *victim;
    char buf[MSL];
    BUFFER *output;
	int top_level=0;
    bool found = false;

	if (IS_UNSWITCHED_MOB(ch)){
		return;
	}

    one_argument( argument, arg );

    if (IS_IMMORTAL(ch) && !IS_NULLSTR(arg))
    {
		if ( ( victim = get_char_world( ch, arg ) ) == NULL )
		{
			ch->println( "They aren't here." );
			return;
		}
    }
	else
	{
		victim=ch;
	}
/*
    if (IS_NPC(victim))
	{
		ch->println( "Mobs don't have percentages YET." );
		return;
	}
*/

    output = new_buf();
	{
		if(ch->desc && ch->desc->connected_state!=CON_PLAYING){
			if(ch==victim){
				sprintf(buf,"`=j%s", creation_titlebar("CRYSTAL TECH KNOWLEDGE"));
			}else{
				sprintf(buf,"`=j%s", creation_titlebar("CRYSTAL TECH KNOWLEDGE FOR %s", uppercase(victim->name)));
			}
		}else{
			if(ch==victim){
				sprintf(buf,"`=t%s", format_titlebar("CRYSTAL TECH KNOWLEDGE"));
			}else{
				sprintf(buf,"`=t%s", format_titlebarf("CRYSTAL TECH KNOWLEDGE FOR %s", uppercase(victim->name)));
			}
		}

		buf[str_len(buf)-2]= '\0';
		strcat(buf,"`x");
	}
	add_buf( output, buf);

	if (ch!=victim)
	{
//		sprintf(buf,"\r\nPowers for %s (Level: %d, Race: %s, Class: %s)",
		sprintf(buf,"\r\nCrystal Tech Knowledge for %s (Level: %d, Race: %s)",
			victim->name,
			victim->level,
			race_table[victim->race]->name
//			class_table[victim->clss].name
			);
		add_buf( output, buf);
	}

    // initialize data
    for (lev = 0; lev < MAX_LEVEL+2; lev++){
        skill_columns[lev] = 0;
        skill_list[lev]= str_dup("");
    }

	if (IS_IMMORTAL(ch)){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}

	bool specially_granted_skills=false;
    for (sn = 0; !IS_NULLSTR(skill_table[sn].name) ; sn++)
    {
		if (skill_table[sn].spell_fun == spell_null
			&& skill_table[sn].sktype==SKTYPE_CTECH
			&&	(
					(victim->pcdata->learned[sn]==101)
				  ||
					(skill_table[sn].skill_level[victim->clss]<=top_level
					&& skill_table[sn].skill_level[victim->clss]>0)
			//		&& victim->pcdata->learned[sn] > 0)
				)
			)
		{
			if(victim->pcdata->learned[sn]==101
				&& skill_table[sn].skill_level[victim->clss]>LEVEL_HERO)
			{
				specially_granted_skills=true;
			}

			found = true;
			lev = skill_table[sn].skill_level[victim->clss];
			if(IS_SET(skill_table[sn].flags,SKFLAGS_NEW_IMPROVE_SYSTEM)){
				if (victim->level < lev){
					sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
				}else{
					sprintf(buf,"%-18s %3d%%(%3d%%,%3d%%)       ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn],
						skill_table[sn].learn_scale_percent[victim->clss],
						(skill_table[sn].learn_scale_percent[victim->clss]==0?
								100:skill_table[sn].learn_scale_percent[ch->clss]));
				}
			}else{
				if (victim->level < lev && victim->pcdata->learned[sn]!=101){
					if(ch->level<lev){
						sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
					}else{
						sprintf(buf,"%-18s  n/a(%3d%%)            ",
							capitalize(skill_table[sn].name),
							victim->pcdata->learned[sn]);
					}
				}else{
					sprintf(buf,"%-18s %3d%%                  ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn]);
				}
			}

			if (IS_NULLSTR(skill_list[lev])){
				sprintf(linebuf,"\r\n Level %2d: %s",lev,buf);
				replace_string(skill_list[lev], linebuf);
			}else{ // append
				if ( ++skill_columns[lev] % 2 == 0){
					sprintf(linebuf,"%s\r\n           ",skill_list[lev]);
					replace_string(skill_list[lev], linebuf);
				}else{
					// trim off the extra spaces
					strcpy(linebuf, skill_list[lev]);
					linebuf[str_len(linebuf)-6]='\0';
					replace_string(skill_list[lev], linebuf);
				}
				sprintf(linebuf,"%s%s",skill_list[lev],buf);
				replace_string(skill_list[lev], linebuf);
			}
		}
    }

	if (IS_IMMORTAL(ch) || specially_granted_skills){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}


    // return results
    if (!found){
		if (ch==victim){
			ch->println( "You know nothing of Crystal Tech." );
		}else{
			ch->println( "They know nothing of Crystal Tech." );
		}
    }else{
		for (lev = 1; lev < top_level; lev++){
			if(lev==LEVEL_IMMORTAL){
				if(IS_IMMORTAL(ch)){
					sprintf(buf, "\r\n\r\n%s", format_titlebar("CRYSTAL TECH KNOWLEDGE DISABLED FOR CHARACTER UNLESS GRANTED BELOW HERE"));
				}else{
					sprintf(buf, "\r\n\r\n%s", format_titlebar("SPECIALLY GRANTED CRYSTAL TECH KNOWLEDGE"));
				}
				buf[str_len(buf)-2]= '\0';
				strcat(buf,"`x");
				add_buf( output, buf);
			}
			if (!IS_NULLSTR(skill_list[lev])){
				add_buf( output, skill_list[lev] );
			}
		}
		add_buf( output, "\r\n");
		ch->sendpage(buf_string(output));
		free_buf(output);
	}

	// deallocate all memory used
	for (lev = 0; lev < MAX_LEVEL+2; lev++){
        free_string(skill_list[lev]);
    }
	return;
}
/**************************************************************************/
void do_songs(char_data *ch, char *argument)
{
	char *skill_list[MAX_LEVEL+2];
    unsigned char skill_columns[MAX_LEVEL+2];
	char linebuf[MSL];
    int sn,lev;
    char arg[MIL];
    char_data *victim;
    char buf[MSL];
    BUFFER *output;
	int top_level=0;
    bool found = false;

	if (IS_UNSWITCHED_MOB(ch)){
		return;
	}

    one_argument( argument, arg );

    if (IS_IMMORTAL(ch) && !IS_NULLSTR(arg))
    {
		if ( ( victim = get_char_world( ch, arg ) ) == NULL )
		{
			ch->println( "They aren't here." );
			return;
		}
    }
	else
	{
		victim=ch;
	}
/*
    if (IS_NPC(victim))
	{
		ch->println( "Mobs don't have percentages YET." );
		return;
	}
*/

    output = new_buf();
	{
		if(ch->desc && ch->desc->connected_state!=CON_PLAYING){
			if(ch==victim){
				sprintf(buf,"`=j%s", creation_titlebar("BARDIC SONG KNOWLEDGE"));
			}else{
				sprintf(buf,"`=j%s", creation_titlebar("BARDIC SONG KNOWLEDGE FOR %s", uppercase(victim->name)));
			}
		}else{
			if(ch==victim){
				sprintf(buf,"`=t%s", format_titlebar("BARDIC SONG KNOWLEDGE"));
			}else{
				sprintf(buf,"`=t%s", format_titlebarf("BARDIC SONG KNOWLEDGE FOR %s", uppercase(victim->name)));
			}
		}

		buf[str_len(buf)-2]= '\0';
		strcat(buf,"`x");
	}
	add_buf( output, buf);

	if (ch!=victim)
	{
//		sprintf(buf,"\r\nPowers for %s (Level: %d, Race: %s, Class: %s)",
		sprintf(buf,"\r\nBardic Song Knowledge for %s (Level: %d, Race: %s)",
			victim->name,
			victim->level,
			race_table[victim->race]->name
//			class_table[victim->clss].name
			);
		add_buf( output, buf);
	}

    // initialize data
    for (lev = 0; lev < MAX_LEVEL+2; lev++){
        skill_columns[lev] = 0;
        skill_list[lev]= str_dup("");
    }

	if (IS_IMMORTAL(ch)){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}

	bool specially_granted_skills=false;
    for (sn = 0; !IS_NULLSTR(skill_table[sn].name) ; sn++)
    {
		if (skill_table[sn].spell_fun != spell_null
			&& skill_table[sn].sktype==SKTYPE_SONG
			&&	(
					(victim->pcdata->learned[sn]==101)
				  ||
					(skill_table[sn].skill_level[victim->clss]<=top_level
					&& skill_table[sn].skill_level[victim->clss]>0)
			//		&& victim->pcdata->learned[sn] > 0)
				)
			)
		{
			if(victim->pcdata->learned[sn]==101
				&& skill_table[sn].skill_level[victim->clss]>LEVEL_HERO)
			{
				specially_granted_skills=true;
			}

			found = true;
			lev = skill_table[sn].skill_level[victim->clss];
			if(IS_SET(skill_table[sn].flags,SKFLAGS_NEW_IMPROVE_SYSTEM)){
				if (victim->level < lev){
					sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
				}else{
					sprintf(buf,"%-18s %3d%%(%3d%%,%3d%%)       ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn],
						skill_table[sn].learn_scale_percent[victim->clss],
						(skill_table[sn].learn_scale_percent[victim->clss]==0?
								100:skill_table[sn].learn_scale_percent[ch->clss]));
				}
			}else{
				if (victim->level < lev && victim->pcdata->learned[sn]!=101){
					if(ch->level<lev){
						sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
					}else{
						sprintf(buf,"%-18s  n/a(%3d%%)            ",
							capitalize(skill_table[sn].name),
							victim->pcdata->learned[sn]);
					}
				}else{
					sprintf(buf,"%-18s %3d%%                  ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn]);
				}
			}

			if (IS_NULLSTR(skill_list[lev])){
				sprintf(linebuf,"\r\n Level %2d: %s",lev,buf);
				replace_string(skill_list[lev], linebuf);
			}else{ // append
				if ( ++skill_columns[lev] % 2 == 0){
					sprintf(linebuf,"%s\r\n           ",skill_list[lev]);
					replace_string(skill_list[lev], linebuf);
				}else{
					// trim off the extra spaces
					strcpy(linebuf, skill_list[lev]);
					linebuf[str_len(linebuf)-6]='\0';
					replace_string(skill_list[lev], linebuf);
				}
				sprintf(linebuf,"%s%s",skill_list[lev],buf);
				replace_string(skill_list[lev], linebuf);
			}
		}
    }

	if (IS_IMMORTAL(ch) || specially_granted_skills){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}


    // return results
    if (!found){
		if (ch==victim){
			ch->println( "You know no songs." );
		}else{
			ch->println( "They know no songs." );
		}
    }else{
		for (lev = 1; lev < top_level; lev++){
			if(lev==LEVEL_IMMORTAL){
				if(IS_IMMORTAL(ch)){
					sprintf(buf, "\r\n\r\n%s", format_titlebar("BARDIC SONG KNOWLEDGE DISABLED FOR CHARACTER UNLESS GRANTED BELOW HERE"));
				}else{
					sprintf(buf, "\r\n\r\n%s", format_titlebar("SPECIALLY GRANTED BARDIC SONG KNOWLEDGE"));
				}
				buf[str_len(buf)-2]= '\0';
				strcat(buf,"`x");
				add_buf( output, buf);
			}
			if (!IS_NULLSTR(skill_list[lev])){
				add_buf( output, skill_list[lev] );
			}
		}
		add_buf( output, "\r\n");
		ch->sendpage(buf_string(output));
		free_buf(output);
	}

	// deallocate all memory used
	for (lev = 0; lev < MAX_LEVEL+2; lev++){
        free_string(skill_list[lev]);
    }
	return;
}
/**************************************************************************/

void do_performances(char_data *ch, char *argument)
{
	char *skill_list[MAX_LEVEL+2];
    unsigned char skill_columns[MAX_LEVEL+2];
	char linebuf[MSL];
    int sn,lev;
    char arg[MIL];
    char_data *victim;
    char buf[MSL];
    BUFFER *output;
	int top_level=0;
    bool found = false;

	if (IS_UNSWITCHED_MOB(ch)){
		return;
	}

    one_argument( argument, arg );

    if (IS_IMMORTAL(ch) && !IS_NULLSTR(arg))
    {
		if ( ( victim = get_char_world( ch, arg ) ) == NULL )
		{
			ch->println( "They aren't here." );
			return;
		}
    }
	else
	{
		victim=ch;
	}

/*    if (IS_NPC(victim))
	{
		ch->println( "Mobs don't have skill percentages yet." );
		return;
	}*/


    output = new_buf();
	{
		if(ch->desc && ch->desc->connected_state!=CON_PLAYING){
			if(ch==victim){
				sprintf(buf,"`=j%s", creation_titlebar("PERFORMANCES"));
			}else{
				sprintf(buf,"`=j%s", creation_titlebar("PERFORMANCES FOR %s", uppercase(victim->name)));
			}
		}else{
			if(ch==victim){
				sprintf(buf,"`=t%s", format_titlebar("PERFORMANCES"));
			}else{
				sprintf(buf,"`=t%s", format_titlebarf("PERFORMANCES FOR %s", uppercase(victim->name)));
			}
		}

		buf[str_len(buf)-2]= '\0';
		strcat(buf,"`x");
	}
	add_buf( output, buf);

	if (ch!=victim)
	{
//		sprintf(buf,"\r\nSkills for %s (Level: %d, Race: %s, Class: %s)",
		sprintf(buf,"\r\nPerformances for %s (Level: %d, Race: %s)",
			victim->name,
			victim->level,
			race_table[victim->race]->name
//			class_table[victim->clss].name
			);
		add_buf( output, buf);
	}

    // initialize data
    for (lev = 0; lev < MAX_LEVEL+2; lev++){
        skill_columns[lev] = 0;
        skill_list[lev]= str_dup("");
    }

	if (IS_IMMORTAL(ch)){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}

	bool specially_granted_skills=false;
    for (sn = 0; !IS_NULLSTR(skill_table[sn].name) ; sn++)
    {
		if (skill_table[sn].spell_fun == spell_null
			&& skill_table[sn].sktype==SKTYPE_PERFORM
			&&	(
					(victim->pcdata->learned[sn]==101)
				  ||
					(skill_table[sn].skill_level[victim->clss]<=top_level
					&& skill_table[sn].skill_level[victim->clss]>0)
			//		&& victim->pcdata->learned[sn] > 0)
				)
			)
		{
			if(victim->pcdata->learned[sn]==101
				&& skill_table[sn].skill_level[victim->clss]>LEVEL_HERO)
			{
				specially_granted_skills=true;
			}

			found = true;
			lev = skill_table[sn].skill_level[victim->clss];
			if(IS_SET(skill_table[sn].flags,SKFLAGS_NEW_IMPROVE_SYSTEM)){
				if (victim->level < lev){
					sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
				}else{
					sprintf(buf,"%-18s %3d%%(%3d%%,%3d%%)       ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn],
						skill_table[sn].learn_scale_percent[victim->clss],
						(skill_table[sn].learn_scale_percent[victim->clss]==0?
								100:skill_table[sn].learn_scale_percent[ch->clss]));
				}
			}else{
				if (victim->level < lev && victim->pcdata->learned[sn]!=101){
					if(ch->level<lev){
						sprintf(buf,"%-18s  n/a                  ", capitalize(skill_table[sn].name));
					}else{
						sprintf(buf,"%-18s  n/a(%3d%%)            ",
							capitalize(skill_table[sn].name),
							victim->pcdata->learned[sn]);
					}
				}else{
					sprintf(buf,"%-18s %3d%%                  ",capitalize(skill_table[sn].name),
						victim->pcdata->learned[sn]);
				}
			}

			if (IS_NULLSTR(skill_list[lev])){
				sprintf(linebuf,"\r\n Level %2d: %s",lev,buf);
				replace_string(skill_list[lev], linebuf);
			}else{ // append
				if ( ++skill_columns[lev] % 2 == 0){
					sprintf(linebuf,"%s\r\n           ",skill_list[lev]);
					replace_string(skill_list[lev], linebuf);
				}else{
					// trim off the extra spaces
					strcpy(linebuf, skill_list[lev]);
					linebuf[str_len(linebuf)-6]='\0';
					replace_string(skill_list[lev], linebuf);
				}
				sprintf(linebuf,"%s%s",skill_list[lev],buf);
				replace_string(skill_list[lev], linebuf);
			}
		}
    }

	if (IS_IMMORTAL(ch) || specially_granted_skills){
		top_level= ABSOLUTE_MAX_LEVEL;
	}else{
		top_level= LEVEL_HERO;
	}


    // return results
    if (!found){
		if (ch==victim){
			ch->println( "You know no skills." );
		}else{
			ch->println( "They know no skills." );
		}
    }else{
		for (lev = 1; lev < top_level; lev++){
			if(lev==LEVEL_IMMORTAL){
				if(IS_IMMORTAL(ch)){
					sprintf(buf, "\r\n\r\n%s", format_titlebar("SKILLS DISABLED FOR CHARACTER UNLESS GRANTED BELOW HERE"));
				}else{
					sprintf(buf, "\r\n\r\n%s", format_titlebar("SPECIALLY GRANTED SKILLS"));
				}
				buf[str_len(buf)-2]= '\0';
				strcat(buf,"`x");
				add_buf( output, buf);
			}
			if (!IS_NULLSTR(skill_list[lev])){
				add_buf( output, skill_list[lev] );
			}
		}
		add_buf( output, "\r\n");
		ch->sendpage(buf_string(output));
		free_buf(output);
	}

	// deallocate all memory used
	for (lev = 0; lev < MAX_LEVEL+2; lev++){
        free_string(skill_list[lev]);
    }
	return;
}

/**************************************************************************/
/*                    Aegis Transformations Version 1.0                   */
/*                               By Sythja                                */
/* [X] Active Form    [X] Power Form    [X] Proto Form     [X] Omega Form */
/**************************************************************************/
void do_proto_form( char_data * ch, char *argument ) /* Proto Form */
{

  int pl_mult = 0;
  int arg;

  arg = atoi( argument );

  if (!IS_CSTRUCT(ch)){
    ch->println("You are unable to use this ability");
    return;
  }

  if (IS_CSTRUCTAFF(ch, CONSTRUCT_OMEGA_FORM)
      || IS_CSTRUCTAFF(ch, CONSTRUCT_ACTIVE_FORM)
      || IS_CSTRUCTAFF(ch, CONSTRUCT_POWER_FORM)){
    ch->println( "You must power down first.\n\r" );
    return;}

	if (IS_CSTRUCTAFF(ch, CONSTRUCT_PROTO_FORM) && arg == 0)
  {
    ch->println( "You reroute your power settings to their default status and return to normal.\n\r" );
   	act("`C$n's armor phase-shifts back to normal color, and you notice $e has less speed and power, but looks a bit less fragile.",ch,NULL,NULL,TO_NOTVICT);
			affect_strip ( ch, gsn_proto_form );
    return;
  }
  else if( !IS_CSTRUCTAFF(ch, CONSTRUCT_PROTO_FORM) && arg == 0 )
  {
    ch->println( "Syntax: 'protoform' [2-10]\n\r" );
    return;
  }

  pl_mult = 10;

  if( arg > pl_mult )
    pl_mult = ( int )pl_mult;
  else if( arg < 1 )
    pl_mult = 1;
  else
    pl_mult = arg;

    if (IS_CSTRUCTAFF(ch, CONSTRUCT_SIGMA_VIRUS)){
        pl_mult	= pl_mult*1.5;
    }

  if( ch->mana < skill_table[gsn_proto_form].min_mana * pl_mult )
  {
    ch->println( "`RWARNING:`Y Insufficient Energy to reroute.`D\n\r" );
    return;
  }
  if( arg < 2 || arg > 10 )
  {
    ch->println( "Syntax: 'protoform' [2-10]\n\r" );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_proto_form].beats );

  if ( get_skill(ch,gsn_proto_form) > number_percent())
  {
  		act("`CYour power is suddenly re-proportioned throughout your body, as your armor phase-shifts to `sjet black`C, and you notice power and speed boosts, but your armor has become a little less resistant to damage..",ch,NULL,NULL,TO_CHAR);
		act("`C$n's armor phase-shifts to `sjet black`C, and you notice $e has more speed and power, but looks a bit more fragile.",ch,NULL,NULL,TO_NOTVICT);
		check_improve(ch,gsn_proto_form,true,1);
    AFFECT_DATA affStr;
    AFFECT_DATA affSpd;
    AFFECT_DATA affInt;
    AFFECT_DATA affCon;
	affect_strip ( ch, gsn_omega_form );
	affect_strip ( ch, gsn_power_form );
	affect_strip ( ch, gsn_active_form );
	affect_strip ( ch, gsn_proto_form );

    affStr.where     = WHERE_CONSTRUCT;
	affStr.type = gsn_proto_form;
    affStr.duration = -1;
    affStr.level     = ch->level;
    affStr.location = APPLY_ST;
    affStr.modifier = pl_mult;
	affStr.bitvector = CONSTRUCT_PROTO_FORM;
    affect_to_char( ch, &affStr );

    affSpd.where     = WHERE_CONSTRUCT;
	affSpd.type = gsn_proto_form;
    affSpd.duration = -1;
    affSpd.level     = ch->level;
    affSpd.location = APPLY_AG;
    affSpd.modifier = pl_mult;
	affSpd.bitvector = CONSTRUCT_PROTO_FORM;
    affect_to_char( ch, &affSpd );

    affInt.where     = WHERE_CONSTRUCT;
	affInt.type = gsn_proto_form;
    affInt.duration = -1;
    affInt.level     = ch->level;
    affInt.location = APPLY_IN;
    affInt.modifier = pl_mult * (pl_mult/2);
	affInt.bitvector = CONSTRUCT_PROTO_FORM;
    affect_to_char( ch, &affInt );

    affCon.where = WHERE_CONSTRUCT;
    affCon.type = gsn_proto_form;
    affCon.duration = -1;
    affCon.level     = ch->level;
    affCon.location = APPLY_CO;
    affCon.modifier = pl_mult * -1;
    affCon.bitvector = CONSTRUCT_PROTO_FORM;
    affect_to_char( ch, &affCon );

  }
  else
  {
    act("`CYour body fails to re-proportion your power right to unlock your Proto Form.",
         ch, NULL, NULL, TO_CHAR );
    act("`C$n's armor phase-shifts slightly but returns to normal as $n fails to unlock $s Form.",
         ch, NULL, NULL, TO_NOTVICT );
		check_improve(ch,gsn_proto_form,false,1);
  }
  ch->mana -= skill_table[gsn_proto_form].min_mana * pl_mult;
  return;
}
/**************************************************************************/
void do_active_form( char_data * ch, char *argument ) /* Active Form */
{
  int pl_mult = 0;
  int arg;
  arg = atoi( argument );

  if (!IS_CSTRUCT(ch)){
    ch->println("You are unable to use this ability");
    return;
  }
  if (IS_CSTRUCTAFF(ch, CONSTRUCT_OMEGA_FORM)
      || IS_CSTRUCTAFF(ch, CONSTRUCT_PROTO_FORM)
      || IS_CSTRUCTAFF(ch, CONSTRUCT_POWER_FORM)){
    ch->println( "You must power down first.\n\r" );
    return;}
	if (IS_CSTRUCTAFF(ch, CONSTRUCT_ACTIVE_FORM) && arg == 0)
  {
    ch->println( "You reroute your power settings to their default status and return to normal.\n\r" );
    act("`R$n's armor phase-shifts back to its normal color, and you notice significant speed decreases in $s movement.",ch,NULL,NULL,TO_NOTVICT);
   		affect_strip ( ch, gsn_active_form );
    return;
  }
  else if( !IS_CSTRUCTAFF(ch, CONSTRUCT_ACTIVE_FORM) && arg == 0 )
  {
    ch->println( "Syntax: 'activeform' [12-18]\n\r" );
    return;
  }

  pl_mult = 18;

  if( arg > pl_mult )
    pl_mult = ( int )pl_mult;
  else if( arg < 1 )
    pl_mult = 1;
  else
    pl_mult = arg;

    if (IS_CSTRUCTAFF(ch, CONSTRUCT_SIGMA_VIRUS)){
        pl_mult	= pl_mult*1.5;
    }


  if( ch->mana < skill_table[gsn_active_form].min_mana * pl_mult )
  {
    ch->println( "`RWARNING:`Y Insufficient Energy to reroute.`D\n\r" );
    return;
  }
  if( arg < 12 || arg > 18 )
  {
    ch->println( "Syntax: 'activeform' [12-18]\n\r" );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_active_form].beats );

  if ( get_skill(ch,gsn_active_form) > number_percent())
  {
  		act("`RYour power is suddenly re-proportioned throughout your body, as your armor phase-shifts to a bright `zorange`R color, and you notice significant speed increases, as well as small amount of strength loss..",ch,NULL,NULL,TO_CHAR);
		act("`R$n's armor phase-shifts to a bright `zorange`R color, and you notice significant speed increases in $s movement.",ch,NULL,NULL,TO_NOTVICT);
		check_improve(ch,gsn_active_form,true,1);
    AFFECT_DATA affStr;
    AFFECT_DATA affSpd;
    AFFECT_DATA affInt;
    AFFECT_DATA affCon;
	affect_strip ( ch, gsn_omega_form );
	affect_strip ( ch, gsn_power_form );
	affect_strip ( ch, gsn_active_form );
	affect_strip ( ch, gsn_proto_form );

    affStr.where     = WHERE_CONSTRUCT;
	affStr.type = gsn_active_form;
    affStr.duration = -1;
    affStr.level     = ch->level;
    affStr.location = APPLY_ST;
    affStr.modifier = pl_mult * -1;
	affStr.bitvector = CONSTRUCT_ACTIVE_FORM;
    affect_to_char( ch, &affStr );

    affSpd.where     = WHERE_CONSTRUCT;
	affSpd.type = gsn_active_form;
    affSpd.duration = -1;
    affSpd.level     = ch->level;
    affSpd.location = APPLY_AG;
    affSpd.modifier = pl_mult * (pl_mult+(pl_mult/2));
	affSpd.bitvector = CONSTRUCT_ACTIVE_FORM;
    affect_to_char( ch, &affSpd );

    affInt.where     = WHERE_CONSTRUCT;
	affInt.type = gsn_active_form;
    affInt.duration = -1;
    affInt.level     = ch->level;
    affInt.location = APPLY_IN;
    affInt.modifier = pl_mult;
	affInt.bitvector = CONSTRUCT_ACTIVE_FORM;
    affect_to_char( ch, &affInt );

    affCon.where = WHERE_CONSTRUCT;
    affCon.type = gsn_active_form;
    affCon.duration = -1;
    affCon.level     = ch->level;
    affCon.location = APPLY_CO;
    affCon.modifier = pl_mult * (pl_mult/2);
    affCon.bitvector = CONSTRUCT_ACTIVE_FORM;
    affect_to_char( ch, &affCon );
  }
  else
  {
    act("`RYour body fails to re-proportion your power right to unlock your Active Form.",
         ch, NULL, NULL, TO_CHAR );
    act("`R$n's armor phase-shifts slightly but returns to normal as $n fails to unlock $s Form.",
         ch, NULL, NULL, TO_NOTVICT );
		check_improve(ch,gsn_active_form,false,1);
  }
  ch->mana -= skill_table[gsn_active_form].min_mana * pl_mult;
  return;
}
/**************************************************************************/
void do_power_form( char_data * ch, char *argument ) /* Power Form */
{
  int pl_mult = 0;
  int arg;

  arg = atoi( argument );

  if (!IS_CSTRUCT(ch)){
    ch->println("You are unable to use this ability");
    return;
  }

  if (IS_CSTRUCTAFF(ch, CONSTRUCT_OMEGA_FORM)
      || IS_CSTRUCTAFF(ch, CONSTRUCT_PROTO_FORM)
      || IS_CSTRUCTAFF(ch, CONSTRUCT_ACTIVE_FORM)){
    ch->println( "You must power down first.\n\r" );
    return;}

	if (IS_CSTRUCTAFF(ch, CONSTRUCT_POWER_FORM) && arg == 0)
  {
    ch->println( "You reroute your power settings to their default status and return to normal.\n\r" );
    act("`G$n's armor phase-shifts back to its normal color, and you notice $e has significantly less power.",ch,NULL,NULL,TO_NOTVICT);
   		affect_strip ( ch, gsn_power_form );
    return;
  }
  else if( !IS_CSTRUCTAFF(ch, CONSTRUCT_POWER_FORM) && arg == 0 )
  {
    ch->println( "Syntax: 'powerform' [22-28]\n\r" );
    return;
  }

  pl_mult = 28;

  if( arg > pl_mult )
    pl_mult = ( int )pl_mult;
  else if( arg < 1 )
    pl_mult = 1;
  else
    pl_mult = arg;

    if (IS_CSTRUCTAFF(ch, CONSTRUCT_SIGMA_VIRUS)){
        pl_mult	= pl_mult*1.5;
    }

  if( ch->mana < skill_table[gsn_power_form].min_mana * pl_mult )
  {
    ch->println( "`RWARNING:`Y Insufficient Energy to reroute.`D\n\r" );
    return;
  }
  if( arg < 22 || arg > 28 )
  {
    ch->println( "Syntax: 'powerform' [22-28]\n\r" );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_power_form].beats );

  if ( get_skill(ch,gsn_power_form) > number_percent())
  {
  		act("`GYour power is suddenly re-proportioned throughout your body, as your armor phase-shifts to a deep `Mpurple`G color, and you notice significant strength increases, as well as small amounts of speed loss..",ch,NULL,NULL,TO_CHAR);
		act("`G$n's armor phase-shifts to a deep `Mpurple`G color, and you notice $e has significantly more power.",ch,NULL,NULL,TO_NOTVICT);
		check_improve(ch,gsn_power_form,true,1);
    AFFECT_DATA affStr;
    AFFECT_DATA affSpd;
    AFFECT_DATA affInt;
    AFFECT_DATA affCon;
	affect_strip ( ch, gsn_omega_form );
	affect_strip ( ch, gsn_power_form );
	affect_strip ( ch, gsn_active_form );
	affect_strip ( ch, gsn_proto_form );

    affStr.where     = WHERE_CONSTRUCT;
	affStr.type = gsn_power_form;
    affStr.duration = -1;
    affStr.level     = ch->level;
    affStr.location = APPLY_ST;
    affStr.modifier = pl_mult * (pl_mult+(pl_mult/2));
	affStr.bitvector = CONSTRUCT_POWER_FORM;
    affect_to_char( ch, &affStr );

    affSpd.where     = WHERE_CONSTRUCT;
	affSpd.type = gsn_power_form;
    affSpd.duration = -1;
    affSpd.level     = ch->level;
    affSpd.location = APPLY_AG;
    affSpd.modifier = pl_mult * -1;
	affSpd.bitvector = CONSTRUCT_POWER_FORM;
    affect_to_char( ch, &affSpd );

    affInt.where     = WHERE_CONSTRUCT;
	affInt.type = gsn_power_form;
    affInt.duration = -1;
    affInt.level     = ch->level;
    affInt.location = APPLY_IN;
    affInt.modifier = pl_mult * (pl_mult/2);
	affInt.bitvector = CONSTRUCT_POWER_FORM;
    affect_to_char( ch, &affInt );

    affCon.where = WHERE_CONSTRUCT;
    affCon.type = gsn_power_form;
    affCon.duration = -1;
    affCon.level     = ch->level;
    affCon.location = APPLY_CO;
    affCon.modifier = pl_mult;
    affCon.bitvector = CONSTRUCT_POWER_FORM;
    affect_to_char( ch, &affCon );
  }
  else
  {
    act("`GYour body fails to re-proportion your power right to unlock your Power Form.",
         ch, NULL, NULL, TO_CHAR );
    act("`G$n's armor phase-shifts slightly but returns to normal as $n fails to unlock $s Form.",
         ch, NULL, NULL, TO_NOTVICT );
		check_improve(ch,gsn_power_form,false,1);
  }
  ch->mana -= skill_table[gsn_power_form].min_mana * pl_mult;
  return;
}
/**************************************************************************/
void do_omega_form( char_data * ch, char *argument ) /* Omega Form */
{
  int pl_mult = 0;
  int arg;

  arg = atoi( argument );

  if (!IS_CSTRUCT(ch)){
    ch->println("You are unable to use this ability");
    return;
  }

  if (IS_CSTRUCTAFF(ch, CONSTRUCT_PROTO_FORM)
      || IS_CSTRUCTAFF(ch, CONSTRUCT_ACTIVE_FORM)
      || IS_CSTRUCTAFF(ch, CONSTRUCT_POWER_FORM)){
    ch->println( "You must power down first.\n\r" );
    return;}

	if (IS_CSTRUCTAFF(ch, CONSTRUCT_OMEGA_FORM) && arg == 0)
  {
    ch->println( "The boost of power you were experiencing has ended.\n\r" );
    act("`s$n's armor phase-shifts back to normal and the omnious aura fades from about $m.",ch,NULL,NULL,TO_NOTVICT);
   		affect_strip ( ch, gsn_omega_form );
    return;
  }
  else if( !IS_CSTRUCTAFF(ch, CONSTRUCT_OMEGA_FORM) && arg == 0 )
  {
    ch->println( "Syntax: 'omegaform' [2-40]\n\r" );
    return;
  }

  pl_mult = 40;

  if( arg > pl_mult )
    pl_mult = ( int )pl_mult;
  else if( arg < 1 )
    pl_mult = 1;
  else
    pl_mult = arg;
    if (IS_CSTRUCTAFF(ch, CONSTRUCT_SIGMA_VIRUS)){
        pl_mult	= pl_mult*1.5;
    }

  if( ch->mana < skill_table[gsn_omega_form].min_mana * pl_mult )
  {
    ch->println( "`RWARNING:`Y Insufficient Energy to reroute.`D\n\r" );
    return;
  }
  if( arg < 2 || arg > 40 )
  {
    ch->println( "Syntax: 'omegaform' [2-40]\n\r" );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_omega_form].beats );

  if ( get_skill(ch,gsn_omega_form) > number_percent())
  {

  		act("`sYour suddenly receive an extreme energy boost throughout your body, as your armor phase-shifts to `rcrimson red`s, and you notice all primairy functions are working at above-average quality..",ch,NULL,NULL,TO_CHAR);
		act("`s$n's armor phase-shifts to `rcrimson red`s and an omnious aura appears about $m.",ch,NULL,NULL,TO_NOTVICT);
		check_improve(ch,gsn_omega_form,true,1);
    AFFECT_DATA affStr;
    AFFECT_DATA affSpd;
    AFFECT_DATA affInt;
    AFFECT_DATA affCon;
    AFFECT_DATA affQu;
    AFFECT_DATA affDR;
    AFFECT_DATA affHR;


	affect_strip ( ch, gsn_omega_form );
	affect_strip ( ch, gsn_power_form );
	affect_strip ( ch, gsn_active_form );
	affect_strip ( ch, gsn_proto_form );

    affStr.where     = WHERE_CONSTRUCT;
	affStr.type = gsn_omega_form;
    affStr.duration = -1;
    affStr.level     = ch->level;
    affStr.location = APPLY_ST;
//    affStr.modifier = pl_mult * 3;
    affStr.modifier = pl_mult * (pl_mult/2);

	affStr.bitvector = CONSTRUCT_OMEGA_FORM;
    affect_to_char( ch, &affStr );

    affSpd.where     = WHERE_CONSTRUCT;
	affSpd.type = gsn_omega_form;
    affSpd.duration = -1;
    affSpd.level     = ch->level;
    affSpd.location = APPLY_AG;
    affSpd.modifier = pl_mult * (pl_mult/2);
	affSpd.bitvector = CONSTRUCT_OMEGA_FORM;
    affect_to_char( ch, &affSpd );

    affInt.where     = WHERE_CONSTRUCT;
	affInt.type = gsn_omega_form;
    affInt.duration = -1;
    affInt.level     = ch->level;
    affInt.location = APPLY_IN;
//    affInt.modifier = pl_mult;
    affInt.modifier = pl_mult * (pl_mult/2);
	affInt.bitvector = CONSTRUCT_OMEGA_FORM;
    affect_to_char( ch, &affInt );

    affCon.where = WHERE_CONSTRUCT;
    affCon.type = gsn_omega_form;
    affCon.duration = -1;
    affCon.level     = ch->level;
    affCon.location = APPLY_CO;
    affCon.modifier = pl_mult * (pl_mult/2);
//    affCon.modifier = pl_mult * 2;
    affCon.bitvector = CONSTRUCT_OMEGA_FORM;
    affect_to_char( ch, &affCon );

    affQu.where = WHERE_CONSTRUCT;
    affQu.type = gsn_omega_form;
    affQu.duration = -1;
    affQu.level     = ch->level;
    affQu.location = APPLY_QU;
    affQu.modifier = pl_mult * (pl_mult/2);
//    affCon.modifier = pl_mult * 2;
    affQu.bitvector = CONSTRUCT_OMEGA_FORM;
    affect_to_char( ch, &affQu );

    affDR.where = WHERE_CONSTRUCT;
    affDR.type = gsn_omega_form;
    affDR.duration = -1;
    affDR.level     = ch->level;
    affDR.location = APPLY_DAMROLL;
    affDR.modifier = pl_mult * (pl_mult/2);
//    affCon.modifier = pl_mult * 2;
    affDR.bitvector = CONSTRUCT_OMEGA_FORM;
    affect_to_char( ch, &affDR );

    affHR.where = WHERE_CONSTRUCT;
    affHR.type = gsn_omega_form;
    affHR.duration = -1;
    affHR.level     = ch->level;
    affHR.location = APPLY_HITROLL;
    affHR.modifier = pl_mult * (pl_mult/2);
//    affCon.modifier = pl_mult * 2;
    affHR.bitvector = CONSTRUCT_OMEGA_FORM;
    affect_to_char( ch, &affHR );
  }
  else
  {
    act("`sYour body fails to re-proportion your power right to unlock your Omega Form.",
         ch, NULL, NULL, TO_CHAR );
    act("`s$n's armor phase-shifts slightly but returns to normal as $n fails to unlock $s Form.",
         ch, NULL, NULL, TO_NOTVICT );
		check_improve(ch,gsn_omega_form,false,1);
  }
  ch->mana -= skill_table[gsn_omega_form].min_mana * pl_mult;
  return;
}
/**************************************************************************/
void do_xbuster(char_data *ch, char *argument ) /* X Buster */
{
    char_data *victim;
    int dam = 0;

    if ( IS_NPC(ch) && IS_AFFECTED( ch, AFF_CHARM ) ){
		ch->println( "You can't concentrate enough for that.\n\r" );
		return;}

	if (!IS_CSTRUCT(ch)){
		ch->println("You are unable to use this ability");
		return;}

	if (!IS_CSTRUCTAFF(ch, CONSTRUCT_ACTIVE_BUSTER)){
		ch->println( "Aegisblaster not active!!\n\r" );
//		SET_BIT(ch->construct_flags, CONSTRUCT_ACTIVE_BUSTER);
		return;}

	OBJ_DATA *weapon;
	weapon = get_eq_char(ch,WEAR_WIELD);
	if (weapon == NULL
        || weapon->pIndexData->vnum != 102){
		ch->println("Something has gone wrong, Aegisblaster Deactivated!");
		REMOVE_BIT(ch->construct_flags, CONSTRUCT_ACTIVE_BUSTER);
	return;}

	victim = ch->fighting;
	if (victim == NULL){
		ch->println( "You aren't fighting anyone.\n\r" );
	return;}

	if ( ch->mana < skill_table[gsn_xbuster].min_mana ){
		ch->println( "You don't have the Energy you fire your Charge Shot.\n\r" );
    return;}

    WAIT_STATE( ch, skill_table[gsn_xbuster].beats );

	if ( get_skill(ch,gsn_xbuster) > number_percent()){
        dam = weapon->value[1] * weapon->value[2];
	int droll=GET_DAMROLL(ch);
    if (ch->symbiotetype != 0){
        droll += ch->symbiotedr;}
        dam = dam * droll;
    if (IS_CSTRUCTAFF(ch, CONSTRUCT_SIGMA_VIRUS)){
        dam	= dam * 10;
    }
	    act( "`CYou fire your Charge Shot at $N.",ch, NULL, victim, TO_CHAR);
        act( "`C$n fires at you with their Charge Shot. ", ch, NULL, victim, TO_VICT);
        act( "`C$n fires at $N with $s Charge Shot.", ch, NULL, victim, TO_NOTVICT );
		check_improve(ch,gsn_xbuster,true,1);
		damage(ch, victim, dam, gsn_xbuster, DAM_LIGHT, true);
    }
    else
    {
        act( "`CYou missed $N with your Charge Shot.", ch, NULL, victim, TO_CHAR );
        act( "`C$n misses you with $s Charge Shot.", ch, NULL, victim, TO_VICT );
        act( "`C$n missed $N with $s Charge Shot.", ch, NULL, victim, TO_NOTVICT );
		check_improve(ch,gsn_xbuster,false,1);
		damage(ch, victim, 0, gsn_xbuster, DAM_LIGHT, true);
	}
	if (!IS_CSTRUCT(ch)){
      ch->mana -= skill_table[gsn_xbuster].min_mana;
	}
    return;
}
/**************************************************************************/
void do_aegissaber( char_data *ch, char * ) /* Saber Activation */
{
	OBJ_INDEX_DATA *pObjIndex;
	pObjIndex = get_obj_index(101);

	if (!IS_CSTRUCT(ch)){
		ch->println("You are unable to use this ability");
	return;}

	if (IS_CSTRUCTAFF(ch, CONSTRUCT_ACTIVE_BUSTER)){
		ch->println( "You must deactivate your Blaster first.\n\r" );
    return;}

	if (IS_CSTRUCTAFF(ch, CONSTRUCT_ACTIVE_SABER) ){
		ch->println( "Reconfiguring Saber Arm.\n\r" );
   		affect_strip ( ch, gsn_aegissaber );
	OBJ_DATA *aegisweapon;
		aegisweapon = get_eq_char(ch,WEAR_WIELD);
//		remove_obj( ch, WEAR_WIELD, true ); // remove their current object
		extract_obj(aegisweapon);
		WAIT_STATE( ch, skill_table[gsn_aegisblaster].beats );
		ch->println("Aegissaber deactivated");
    return;}

//	else if( !IS_CSTRUCTAFF(ch, CONSTRUCT_OMEGA_FORM) ){
//		ch->println( "Syntax: 'gsn_aegisblaster' [2-40]\n\r" );
//	return;}


	if( ch->mana < skill_table[gsn_aegissaber].min_mana ){
		ch->println( "`RWARNING:`Y Insufficient Energy to activate Saber.\n\r" );
	return;}

	OBJ_DATA *weaponcheck;
	weaponcheck = get_eq_char(ch,WEAR_WIELD);
	if (weaponcheck != NULL){
		ch->println( "You must remove your weapon to activate your Saber.\n\r" );
	return;}

	if ( get_skill(ch,gsn_aegissaber) > number_percent()){
	WAIT_STATE( ch, skill_table[gsn_aegissaber].beats );

  		act("`sYour forearm's plating shifts as your Aegissaber is activated.",ch,NULL,NULL,TO_CHAR);
		act("`s$n's forearm plating shifts as $m Aegissaber activates!",ch,NULL,NULL,TO_NOTVICT);
		check_improve(ch,gsn_aegissaber,true,1);
    AFFECT_DATA af;

    OBJ_DATA *saber;
	int numdie, dietype, random, vlevel;

    saber= create_object(pObjIndex);

	saber->level = ch->level;


	vlevel	   = saber->level + number_range( 0, (( 100 - ch->level ) / 10 ));
	if (IS_CSTRUCTAFF(ch, CONSTRUCT_SIGMA_VIRUS)){
    vlevel	   = vlevel + 5;
	}
	// first try to get a random damage based on level
	random  = number_range( 0, 7 );
	numdie  = weapon_balance_lookup( vlevel, random, 0 );
	dietype = weapon_balance_lookup( vlevel, random, 1 );

	// damage lookup failed, use polarmost damage class,
	if ( numdie == 0 ||  dietype == 0 )
	{
		numdie  = weapon_balance_lookup( vlevel, 0, 0 );
		dietype = weapon_balance_lookup( vlevel, 0, 1 );
	}

	saber->attune_flags = ATTUNE_NEED_TO_USE|ATTUNE_HARD|ATTUNE_INFURIATING|ATTUNE_PREVIOUS;
	saber->attune_id    = ch->player_id;
	saber->value[1]	  = numdie;
	saber->value[2]	  = dietype;

	obj_to_char(saber, ch);

	int tempwizi;

	// put the object in characters hand (use wizi to hide some of it)
	tempwizi = INVIS_LEVEL(ch);
	INVIS_LEVEL(ch)= LEVEL_IMMORTAL;
//	remove_obj( ch, WEAR_WIELD, true ); // remove their current object
	wear_obj( ch, saber, true, false );	// wield the object
	INVIS_LEVEL(ch)= tempwizi;

	affect_strip ( ch, gsn_aegissaber );

    af.where     = WHERE_CONSTRUCT;
	af.type = gsn_aegissaber;
    af.duration = -1;
    af.level     = ch->level;
    af.location = APPLY_NONE;
    af.modifier = 0;
	af.bitvector = CONSTRUCT_ACTIVE_SABER;
    affect_to_char( ch, &af );

  }
  else
  {
    act("`sYour forearm's plating starts to shift, then suddenly snaps back into place.",
         ch, NULL, NULL, TO_CHAR );
    act("`s$n's forearm plating starts to shift, then suddenly snaps back into place!",
         ch, NULL, NULL, TO_NOTVICT );
		check_improve(ch,gsn_aegissaber,false,1);
  }
  ch->mana -= skill_table[gsn_aegissaber].min_mana;
  return;
}
/**************************************************************************/
void do_aegisblaster( char_data *ch, char * ) /* Blaster Activation */
{
	OBJ_INDEX_DATA *pObjIndex;
	pObjIndex = get_obj_index(102);

	if (!IS_CSTRUCT(ch)){
		ch->println("You are unable to use this ability");
	return;}

	if (IS_CSTRUCTAFF(ch, CONSTRUCT_ACTIVE_SABER)){
		ch->println( "You must deactivate your Saber first.\n\r" );
    return;}

	if (IS_CSTRUCTAFF(ch, CONSTRUCT_ACTIVE_BUSTER) ){
		ch->println( "Reconfiguring Blaster Arm.\n\r" );
   		affect_strip ( ch, gsn_aegisblaster );
	OBJ_DATA *aegisweapon;
		aegisweapon = get_eq_char(ch,WEAR_WIELD);
//		remove_obj( ch, WEAR_WIELD, true ); // remove their current object
		extract_obj(aegisweapon);
		WAIT_STATE( ch, skill_table[gsn_aegisblaster].beats );
		ch->println("Aegisblaster deactivated");
    return;}

//	else if( !IS_CSTRUCTAFF(ch, CONSTRUCT_OMEGA_FORM) ){
//		ch->println( "Syntax: 'gsn_aegisblaster' [2-40]\n\r" );
//	return;}

	if( ch->mana < skill_table[gsn_aegisblaster].min_mana ){
		ch->println( "`RWARNING:`Y Insufficient Energy to activate Blaster.`D\n\r" );
	return;}

	OBJ_DATA *weaponcheck;
	weaponcheck = get_eq_char(ch,WEAR_WIELD);
	if (weaponcheck != NULL){
		ch->println( "You must remove your weapon to activate your Blaster.`D\n\r" );
	return;}

	if ( get_skill(ch,gsn_aegisblaster) > number_percent()){
	WAIT_STATE( ch, skill_table[gsn_aegisblaster].beats );

  		act("`sYour forearm's plating shifts as your Aegisblaster is activated.",ch,NULL,NULL,TO_CHAR);
		act("`s$n's forearm plating shifts as $m Aegisblaster activates!",ch,NULL,NULL,TO_NOTVICT);
		check_improve(ch,gsn_aegisblaster,true,1);
    AFFECT_DATA af;

    OBJ_DATA *blaster;
	int numdie, dietype, random, vlevel;

    blaster= create_object(pObjIndex);

	blaster->level = ch->level;

	vlevel	   = blaster->level + number_range( 0, (( 100 - ch->level ) / 10 ));

	if (IS_CSTRUCTAFF(ch, CONSTRUCT_SIGMA_VIRUS)){
    vlevel	   = vlevel + 5;
	}

	// first try to get a random damage based on level
	random  = number_range( 0, 7 );
	numdie  = weapon_balance_lookup( vlevel, random, 0 );
	dietype = weapon_balance_lookup( vlevel, random, 1 );

	// damage lookup failed, use polarmost damage class,
	if ( numdie == 0 ||  dietype == 0 )
	{
		numdie  = weapon_balance_lookup( vlevel, 0, 0 );
		dietype = weapon_balance_lookup( vlevel, 0, 1 );
	}

	blaster->attune_flags = ATTUNE_NEED_TO_USE|ATTUNE_HARD|ATTUNE_INFURIATING|ATTUNE_PREVIOUS;
	blaster->attune_id    = ch->player_id;
	blaster->value[1]	  = numdie;
	blaster->value[2]	  = dietype;

	obj_to_char(blaster, ch);

	int tempwizi;

	// put the object in characters hand (use wizi to hide some of it)
	tempwizi = INVIS_LEVEL(ch);
	INVIS_LEVEL(ch)= LEVEL_IMMORTAL;
//	remove_obj( ch, WEAR_WIELD, true ); // remove their current object
	wear_obj( ch, blaster, true, false );	// wield the object
	INVIS_LEVEL(ch)= tempwizi;

	affect_strip ( ch, gsn_aegisblaster );

    af.where     = WHERE_CONSTRUCT;
	af.type = gsn_aegisblaster;
    af.duration = -1;
    af.level     = ch->level;
    af.location = APPLY_NONE;
    af.modifier = 0;
	af.bitvector = CONSTRUCT_ACTIVE_BUSTER;
    affect_to_char( ch, &af );

  }
  else
  {
    act("`sYour forearm's plating starts to shift, then suddenly snaps back into place.",
         ch, NULL, NULL, TO_CHAR );
    act("`s$n's forearm plating starts to shift, then suddenly snaps back into place!",
         ch, NULL, NULL, TO_NOTVICT );
		check_improve(ch,gsn_aegisblaster,false,1);
  }
  ch->mana -= skill_table[gsn_aegisblaster].min_mana;
  return;
}
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

