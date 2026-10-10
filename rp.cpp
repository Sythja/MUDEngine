/**************************************************************************/
// rp.cpp - code dealing with rp and rp support
/***************************************************************************
 * The Dawn of Time v1.69s.beta6 (c)1997-2010 Kalahn                       *
 * >> A number of people have contributed to the Dawn codebase, with the   *
 *    majority of code written by Kalahn - www.dawnoftime.org              *
 * >> To use this source code, you must fully comply with the dawn license *
 *    in licenses.txt... In particular, you may not remove this copyright  *
 *    notice.                                                              *
 **************************************************************************/
#include "include.h"
#include "security.h"
#include "interp.h"
#include "olc.h"
#include "ictime.h"

// Locals
void show_rpsheet( char_data *ch, char_data *v);

/**************************************************************************/
void do_metric( char_data *ch, char * )
{
	if ( HAS_CONFIG( ch, CONFIG_METRIC )) {
		ch->println("You will now see your height and weight in Imperial.");
		REMOVE_CONFIG( ch, CONFIG_METRIC );
	} else {
		SET_CONFIG( ch, CONFIG_METRIC );
		ch->println("You will now see your height and weight in Metric.");
	}
}

/**************************************************************************/
void do_rpsupport( char_data *ch, char *argument )
{
	char arg[MIL];
	char_data *victim;

    if (!IS_ADMIN(ch)
        && !IS_SET(true_CH(ch)->pcdata->council, COUNCIL_HEADRP))
	{
        ch->println("RP support can only be used by admin or headrp.");
        return;
	}

    one_argument( argument, arg );

    if( IS_NULLSTR(arg)){
        ch->println("Set as/turn off rpsupport on whom?");
        return;
    }
    if (( victim = get_whovis_player_world( ch, arg )) == NULL ){
        ch->printlnf("Can't find any player '%s' in the game.", arg);
        return;
    }

    if ( HAS_CONFIG(victim, CONFIG_RP_SUPPORT ))
    {
        REMOVE_CONFIG(victim, CONFIG_RP_SUPPORT);
        ch->printlnf("RP support turned off for %s.",
            PERS(victim, ch));
	}else{
        SET_CONFIG(victim, CONFIG_RP_SUPPORT);
        ch->printlnf("RP support turned on for %s.",
            PERS(victim, ch));
    }

    save_char_obj( victim );
    return;
}

/**************************************************************************/
void do_rp_obj_load( char_data *ch, char *argument )
{
	OBJ_INDEX_DATA	*pObjIndex;
	OBJ_DATA		*pObj;
	int				wear;

	if ( !IS_IMMORTAL(ch) && !HAS_CONFIG(ch, CONFIG_RP_SUPPORT))
	{
		do_huh(ch,"");
		return;
	}

	if (( pObjIndex = get_obj_index( OBJ_VNUM_RP_ITEM )) == NULL ) {
		ch->println("RP item non-existant, please report this with a note to admin.");
		return;
	}

	pObj = create_object( pObjIndex);
	ch->println("rpobjload: You've created an RP item and it's in your inventory.");

	if ( !IS_NULLSTR( argument ))
	{
		wear = flag_value( wear_flags, argument );
		if ( wear == NO_FLAG )
		{
			ch->printlnf( "'%s' is not a valid wear location. Item reverted to standard take & hold.", argument );
			pObj->wear_flags = OBJWEAR_TAKE|OBJWEAR_HOLD;
		}
		else
		{
			pObj->wear_flags = OBJWEAR_TAKE|wear;
			ch->printlnf( "RP item's wear loc are now: %s.", flag_string( wear_flags, pObj->wear_flags ));
		}
	}
	else
	{
		pObj->wear_flags = OBJWEAR_TAKE|OBJWEAR_HOLD;
	}

	obj_to_char( pObj, ch );
}

/**************************************************************************/
void do_surname( char_data *ch, char *argument )
{
	char arg1[MIL];
	char_data	*victim;

	if ( IS_NPC( ch ))
	{
		ch->println("Players only.");
		return;
	}

	if(c_str_len(argument)==-1)
		return;

	if ((IS_IMMORTAL(ch) || IS_RP_SUPPORT(ch)))
	{
		argument = one_argument( argument, arg1 );

		if ( IS_NULLSTR( arg1 ) || IS_NULLSTR( argument ))
		{
			ch->println("Syntax: rpsheet surname <player> <name>");
			return;
		}

		if (( victim = get_char_world( ch, arg1 )) == NULL )
		{
			ch->println("They aren't here.");
			return;
		}
		if ( IS_NPC( victim ))
		{
			ch->println("Players only.");
			return;
		}

		if ( str_len(argument) > 20)
		{
			ch->println("Surname must be 20 characters or less.");
			return;
		}


		if (!str_cmp( argument, "none" ))
		{
	        free_string( victim->pcdata->surname );
			ch->printlnf("%s's surname has been cleared.", victim->name );
			victim->println("Your surname has been cleared.");
			return;
		}

		free_string( victim->pcdata->surname );
        victim->pcdata->surname = str_dup( argument );
		ch->printlnf("%s's surname has been changed to %s.",
			victim->name, victim->pcdata->surname );
		victim->printlnf("Your surname has been changed to %s.",
			victim->pcdata->surname );
		return;
	}

/*	if ( IS_LETGAINED( ch ))
	{
		ch->println("You cannot set your surname once you have been letgained.");
		ch->println(" Send a note to rpsupport if you really need it changed.");
		ch->println(" Please send the note only if you have a really good reason");
		ch->println(" to have it changed, ie marriage, typo, etc...");
		return;
	}
*/
	if ( IS_NULLSTR( argument ))
	{
		ch->println("Syntax: rpsheet surname <name>");
		return;
	}

	if ( str_len(argument) > 20)
	{
		ch->println("Surname must be 20 characters or less.");
		return;
	}

	free_string( ch->pcdata->surname );
	ch->pcdata->surname = str_dup( argument );
	ch->printlnf("Your surname is now %s.\r\n",
		ch->pcdata->surname );
	return;
}

/**************************************************************************/
void do_crest( char_data *ch, char *argument )
{
	char arg1[MIL];
	char_data	*victim;

	if ( IS_NPC( ch ))
	{
		ch->println("Players only.");
		return;
	}

	if(c_str_len(argument)==-1)
		return;

	if ((IS_IMMORTAL(ch) || IS_RP_SUPPORT(ch)))
	{
		argument = one_argument( argument, arg1 );

		if ( IS_NULLSTR( arg1 ) || IS_NULLSTR( argument ))
		{
			ch->println("Syntax: rpsheet crest <player> <crest>");
			return;
		}

		if (( victim = get_char_world( ch, arg1 )) == NULL )
		{
			ch->println("They aren't here.");
			return;
		}
		if ( IS_NPC( victim ))
		{
			ch->println("Players only.");
			return;
		}

		if ( str_len(argument) > 40)
		{
			ch->println("Crest must be 40 characters or less.");
			return;
		}


		if (!str_cmp( argument, "none" ))
		{
	        free_string( victim->pcdata->crest );
			ch->printlnf("%s's crest has been cleared.", victim->name );
			victim->println("Your crest has been cleared.");
			return;
		}

		free_string( victim->pcdata->crest );
        victim->pcdata->crest = str_dup( argument );
		ch->printlnf("%s's crest has been changed to %s.",
			victim->name, victim->pcdata->crest );
		victim->printlnf("Your crest has been changed to %s.",
			victim->pcdata->crest );
		return;
	}

/*	if ( IS_LETGAINED( ch ))
	{
		ch->println("You cannot set your crest once you have been letgained.");
		ch->println(" Send a note to rpsupport if you really need it changed.");
		ch->println(" Please send the note only if you have a really good reason");
		ch->println(" to have it changed, ie marriage, typo, etc...");
		return;
	}
*/
	if ( IS_NULLSTR( argument ))
	{
		ch->println("Syntax: rpsheet crest <symbol>");
		return;
	}

	if ( str_len(argument) > 40)
	{
		ch->println("Crest must be 40 characters or less.");
		return;
	}

	free_string( ch->pcdata->crest );
	ch->pcdata->crest = str_dup( argument );
	ch->printlnf("Your crest is now %s.",
		ch->pcdata->crest );
	return;
}


/**************************************************************************/
void do_birthplace( char_data *ch, char *argument )
{
	char arg1[MIL];
	char_data	*victim;

	if ( IS_NPC( ch ))
	{
		ch->println("Players only.");
		return;
	}

	if(c_str_len(argument)==-1)
		return;

	if (IS_IMMORTAL(ch))
	{
		argument = one_argument( argument, arg1 );

		if ( IS_NULLSTR( arg1 ) || IS_NULLSTR( argument ))
		{
			ch->println("Syntax: rpsheet birthplace <player> <name>");
			return;
		}

		if (( victim = get_char_world( ch, arg1 )) == NULL )
		{
			ch->println("They aren't here.");
			return;
		}
		if ( IS_NPC( victim ))
		{
			ch->println("Players only.");
			return;
		}

		if ( str_len(argument) > 20)
		{
			ch->println("Birthplace must be 20 characters or less.");
			return;
		}

		free_string( victim->pcdata->birthplace );
        victim->pcdata->birthplace = str_dup( argument );
		ch->printlnf("%s's birthplace is now %s.",
			victim->name, argument );
		victim->printlnf("Your birthplace is now %s.",
			argument );
		return;
	}

/*	if ( IS_LETGAINED( ch ))
	{
		ch->println("You cannot set your birthplace once you have been letgained.");
		ch->println(" Send a note to imm if you really need it changed.  This is");
		ch->println(" something we don't change unless it's an extreme case since");
		ch->println(" where you are born can never really change.");
		return;
	}
*/
	if ( IS_NULLSTR( argument ))
	{
		ch->println("Syntax: rpsheet birthplace <place>");
		return;
	}

	if ( str_len( argument ) > 20 )
	{
		ch->println("Birthplace must be 20 characters or less.");
		return;
	}



	free_string( ch->pcdata->birthplace );
	ch->pcdata->birthplace = str_dup( argument );
	ch->printlnf("Your birthplace is now %s.\r\n",
		argument );
	return;
}

/**************************************************************************/
void do_haircolor( char_data *ch, char *argument )
{
	char arg1[MIL];
	char_data	*victim;

	if ( IS_NPC( ch ))
	{
		ch->println("Players only.");
		return;
	}

	if(c_str_len(argument)==-1)
		return;

	if (IS_IMMORTAL(ch))
	{
		argument = one_argument( argument, arg1 );

		if ( IS_NULLSTR( arg1 ) || IS_NULLSTR( argument ))
		{
			ch->println("Syntax: rpsheet hair <player> <color>");
			return;
		}

		if (( victim = get_char_world( ch, arg1 )) == NULL )
		{
			ch->println("They aren't here.");
			return;
		}
		if ( IS_NPC( victim ))
		{
			ch->println("Players only.");
			return;
		}

		/*if ( str_len(argument) > 12)
		{
			ch->println("Colour must be 12 characters or less.");
			return;
		}*/

		free_string( victim->pcdata->haircolor );
        victim->pcdata->haircolor = str_dup( argument );
		ch->printlnf("%s's hair color has been changed to %s.",
			victim->name, argument );
		victim->printlnf("Your hair color has been changed to %s.",
			argument );
		return;
	}

/*	if ( IS_LETGAINED( ch ))
	{
		ch->println("You cannot set your hair colour once you have been letgained.");
		ch->println(" Send a note to rp and support if you really need it changed.");
		return;
	}
*/
	if ( IS_NULLSTR( argument ))
	{
		ch->println("Syntax: rpsheet hair <color>");
		return;
	}

/*	if ( str_len(argument) > 12)
	{
		ch->println("Colour must be 12 characters or less.");
		return;
	}*/

	free_string( ch->pcdata->haircolor );
	ch->pcdata->haircolor = str_dup( argument );
	ch->printlnf("Your hair color is now %s.\r\n",
		argument );
	return;
}

/**************************************************************************/
void do_eyecolor( char_data *ch, char *argument )
{
	char arg1[MIL];
	char_data	*victim;

	if ( IS_NPC( ch ))
	{
		ch->println("Players only.");
		return;
	}

	if(c_str_len(argument)==-1)
		return;

	if (IS_IMMORTAL(ch))
	{
		argument = one_argument( argument, arg1 );

		if ( IS_NULLSTR( arg1 ) || IS_NULLSTR( argument ))
		{
			ch->println("Syntax: rpsheet eyes <player> <color>");
			return;
		}

		if (( victim = get_char_world( ch, arg1 )) == NULL )
		{
			ch->println("They aren't here.");
			return;
		}
		if ( IS_NPC( victim ))
		{
			ch->println("Players only.");
			return;
		}

	/*	if ( str_len(argument) > 12)
		{
			ch->println("Colour must be 12 characters or less.");
			return;
		}*/

		free_string( victim->pcdata->eyecolor );
        victim->pcdata->eyecolor = str_dup( argument );
		ch->printlnf("%s's eye color has been changed to %s.",
			victim->name, argument );
		victim->printlnf("Your eye color has been changed to %s.",
			argument );
		return;
	}

/*	if ( IS_LETGAINED( ch ))
	{
		ch->println("You cannot set your eye colour once you have been letgained.");
		ch->println(" Send a note to rp and support if you really need it changed.");
		ch->println(" This is unlikely to be changed unless there is a spectacular");
		ch->println(" and cataclysmic character rp event.");
		return;
	}
*/
	if ( IS_NULLSTR( argument ))
	{
		ch->println("Syntax: rpsheet eyes <color>");
		return;
	}

/*	if ( str_len(argument) > 12)
	{
		ch->println("Colour must be 12 characters or less.");
		return;
	}*/

	free_string( ch->pcdata->eyecolor );
	ch->pcdata->eyecolor = str_dup( argument );
	ch->printlnf("Your eye color is now %s.\r\n",
		argument );
	return;
}

/**************************************************************************/
void do_height( char_data *ch, char *argument )
{
	char		arg1[MIL];
	char_data	*victim;
	int			value;

	argument = one_argument( argument, arg1 );

	if ( IS_NPC( ch ))
	{
		ch->println("Players only.");
		return;
	}

	if (IS_IMMORTAL(ch))
	{
		if ( IS_NULLSTR( arg1 ) || IS_NULLSTR( argument ))
		{
			ch->println("Syntax: rpsheet height <player> <inches>");
			ch->println("  Imms can go beyond the regular racial limitations,");
			ch->println("  but make sure it is realistic, we don't want to go,");
			ch->println("  back and change it once it's set.");
			ch->println("  `=Chelp RACIAL-WEIGHT/HEIGHT`x");
			return;
		}

		if (( victim = get_char_world( ch, arg1 )) == NULL )
		{
			ch->println("They aren't here.");
			return;
		}
		if ( IS_NPC( victim ))
		{
			ch->println("Players only.");
			return;
		}

		value = atoi(argument);

        victim->pcdata->height = value;
		ch->printlnf("%s's height been changed to %d inches.",
			victim->name, value );
		victim->printlnf("Your height has been changed to %d inches.",
			value );
		return;
	}

/*	if ( IS_LETGAINED( ch ))
	{
		ch->println("You cannot set your height once you have been letgained. Send");
		ch->println(" a note to rp and support if you really need it changed.");
		ch->println(" Your reason must have a strong IC basis.");
		return;
	}
*/
	if ( IS_NULLSTR( arg1 ))
	{
		ch->println("Syntax: rpsheet height <inches>");
		return;
	}

	value = atoi( arg1 );

	if ( value < race_table[ch->race]->min_height
	||   value > race_table[ch->race]->max_height )
	{
		ch->printlnf("Invalid height value.  Choose a number between %d and %d.",
			race_table[ch->race]->min_height,
			race_table[ch->race]->max_height);
		ch->println(" If you feel your character should be outside of these values,");
		ch->println(" send a note to support and to rp explaining your reasons.");
		return;
	}

	ch->pcdata->height = value;
	ch->printlnf("Your height is now %d inches.\r\n",
		value );
	return;
}

/**************************************************************************/
void do_weight( char_data *ch, char *argument )
{
	char		arg1[MIL];
	char_data	*victim;
	int			value;

	argument = one_argument( argument, arg1 );

	if ( IS_NPC( ch ))
	{
		ch->println("Players only.");
		return;
	}


	if (IS_IMMORTAL(ch))
	{
		if ( IS_NULLSTR( arg1 ) || IS_NULLSTR( argument ))
		{
			ch->println("Syntax: rpsheet weight <player> <pounds>");
			ch->println("  Imms can go beyond the regular racial limitations,");
			ch->println("  but make sure it is realistic, we don't want to go,");
			ch->println("  back and change it once it's set.");
			ch->println("  `=Chelp RACIAL-WEIGHT/HEIGHT`x");
			return;
		}

		if (( victim = get_char_world( ch, arg1 )) == NULL )
		{
			ch->println("They aren't here.");
			return;
		}
		if ( IS_NPC( victim ))
		{
			ch->println("Players only.");
			return;
		}

		value = atoi(argument);

        victim->pcdata->weight = value;
		ch->printlnf("%s's weight been changed to %d pounds.",
			victim->name, value );
		victim->printlnf("Your weight has been changed to %d pounds.",
			value );
		return;
	}

/*	if ( IS_LETGAINED( ch ))
	{
		ch->println("You cannot set your weight once you have been letgained. Send");
		ch->println(" a note to rp and support if you really need it changed.");
		ch->println(" Your reason must have a strong IC basis.");
		return;
	}
*/
	if ( IS_NULLSTR( arg1 ))
	{
		ch->println("Syntax: rpsheet weight <pounds>");
		return;
	}

	value = atoi( arg1 );

	if ( value < race_table[ch->race]->min_weight
	||   value > race_table[ch->race]->max_weight )
	{
		ch->printlnf("Invalid weight value.  Choose a number between %d and %d.",
			race_table[ch->race]->min_weight,
			race_table[ch->race]->max_weight);
		ch->println(" If you feel your character should be outside of these values,");
		ch->println(" send a note to support and to rp explaining your reasons.");
		return;
	}

	ch->pcdata->weight = value;
	ch->printlnf("Your weight is now %d pounds.\r\n",
		value );
	return;
}

/**************************************************************************/
void do_traits( char_data *ch, char *argument )
{
	char		arg1[MIL];
	char_data	*victim;
	int			value;

	if(c_str_len(argument)==-1)
		return;

	argument = one_argument( argument, arg1 );

	if ( IS_NPC( ch ))
	{
		ch->println("Players only.");
		return;
	}

	if ((IS_IMMORTAL(ch) || IS_RP_SUPPORT(ch)))
	{
		char		arg2[MIL];

		if ( IS_NULLSTR( arg1 ) || IS_NULLSTR( argument ))
		{
			ch->println("Syntax: rpsheet trait <player> <1-9> <trait>");
			return;
		}

		argument = one_argument( argument, arg2 );
		value = atoi( arg2 );

		if (( victim = get_char_world( ch, arg1 )) == NULL )
		{
			ch->println("They aren't here.");
			return;
		}
		if ( IS_NPC( victim ))
		{
			ch->println("Players only.");
			return;
		}

		if ( value < 1 || value > 9 )
		{
			ch->println("Syntax: `=Crpsheet trait <player> <1-9> <trait>`x");
			return;
		}

		if ( str_len(argument) > 60 )
		{
			ch->println("Please keep the trait under 60 characters.");
			return;
		}

		free_string( victim->pcdata->trait[value-1] );
        victim->pcdata->trait[value-1] = str_dup( argument );
		ch->printlnf("%s's trait #%d is `y%s`x.",
			victim->name, value, argument );
		victim->printlnf("Your trait #%d is now `y%s`x.",
			value, argument );
		return;
	}

/*	if ( IS_LETGAINED( ch ))
	{
		ch->println("You cannot set your traits once you have been letgained.");
		ch->println(" Send a note to imm and rpsupport if you really need it changed.");
		ch->println(" Please include some good RP reasons to reflect this change.");
		return;
	}
*/

	value = atoi( arg1 );

	if ( value < 1 || value > 9 )
	{
		ch->println("Syntax: `=Crpsheet trait <1-9> <trait>`x");
		return;
	}


	if ( str_len( argument ) > 60 )
	{
		ch->print("Please keep your trait description under 60 characters.r\n");
		return;
	}


	free_string( ch->pcdata->trait[value-1] );
	ch->pcdata->trait[value-1] = str_dup( argument );
	ch->printlnf("Your trait #%d now `y%s`x.\r\n",
		value, argument );
	return;
}

/**************************************************************************/
void extract_char_from_char_list(char_data *ch);
/**************************************************************************/
void do_rpsheet( char_data *ch, char *argument )
{
	char arg[MIL];
	char arg1[MIL];
	char buf[MIL];
	char_data *victim;

	if(c_str_len(argument)==-1){
		ch->println("no newlines color codes can be used with the rpsheet command.");
		return;
	}

	argument = one_argument( argument, arg1 );
	sprintf( buf, "%s", argument );
	argument = one_argument( argument, arg );

	if(!IS_NULLSTR(arg1)){
		if(!str_prefix( arg1, "view" ))
		{
			// immortal version of rpsheet - can view offline players
			if(IS_IMMORTAL(ch))
			{
				if(IS_NULLSTR(arg)){
					show_rpsheet(ch, ch);
					return;
				}

				victim=get_whovis_player_world( ch, arg );
				if(victim){
					// redirect them to the online player
					show_rpsheet(ch, victim);
					return;
				}

				connection_data d;
				memset(&d, 0, sizeof(d));
				d.make_connected_socket_invalid();

				// attempt to load the pfile
				bool pfile_loaded_okay= load_char_obj( &d, arg);

				if(!pfile_loaded_okay || !d.character){
					ch->printlnf("Couldn't load pfile '%s'.", arg);
					return;
				}

				// pfile loaded, clean up the pointer relationship
				victim=d.character;
				victim->desc=NULL;
				if(victim->pet){
					// remove pets from the char_list
					extract_char_from_char_list(victim->pet);
				}
				if(victim->cardpet){
					// remove pets from the char_list
					extract_char_from_char_list(victim->cardpet);
				}

				if(str_cmp(ch->name, victim->name)){
					ch->println("Player is offline, displaying offline rpsheet:");
				}

				// display the rpsheet
				show_rpsheet(ch, victim);

				// deallocate the loaded character data
				if(victim->pet){
					free_char( victim->pet );
				}
				if(victim->cardpet){
					free_char( victim->cardpet );
				}
				free_char( victim);
				return;
			}

			// rp support can view any online players
			if(IS_RP_SUPPORT(ch) && !IS_NULLSTR(arg))
			{
				if (( victim = get_char_world( ch, arg )) == NULL )
				{
					ch->println("They aren't here.");
					return;
				}
				if ( IS_NPC( victim ))
				{
					ch->println("Players only!");
					return;
				}
			}else{
				victim=ch;
			}
   			show_rpsheet(ch, victim);
			return;
		}

		if(!str_prefix( arg1, "crest" ))
		{
			do_crest( ch, buf );
			return;
		}

		if (!str_prefix( arg1, "surname" ))
		{
			do_surname( ch, buf );
			return;
		}

		if (!str_prefix( arg1, "hair" ))
		{
			do_haircolor( ch, buf );
			return;
		}

		if (!str_prefix( arg1, "eyes" ))
		{
			do_eyecolor( ch, buf );
			return;
		}

		if (!str_prefix( arg1, "height" ))
		{
			do_height( ch, buf );
			return;
		}

		if (!str_prefix( arg1, "weight" ))
		{
			do_weight( ch, buf );
			return;
		}

		if (!str_prefix( arg1, "birthplace" ))
		{
			do_birthplace( ch, buf );
			return;
		}

		if (!str_prefix( arg1, "trait" ))
		{
			do_traits( ch, buf );
			return;
		}
	}

	if (IS_IMMORTAL(ch))
	{
		ch->println("`YRPSHEET COMMANDS\r\n");
		ch->println("  `Bview        `x<player>   (note: player can be offline)");
		ch->println("  `Beyes        `x<player> <color>");
		ch->println("  `Bhair        `x<player> <color>");
		ch->println("  `Bheight      `x<player> <inches>");
		ch->println("  `Bweight      `x<player> <pounds>");
		ch->println("  `Bbirthplace  `x<player> <location>");
		ch->println("  `Bsurname     `x<player> <name>");
		ch->println("  `Btrait       `x<player> <1-9> <trait>");
		ch->println("  `Bcrest       `x<player> <symbol>");
		return;
	}

	if ( IS_RP_SUPPORT( ch ))
	{
		ch->println("`YRPSHEET COMMANDS\r\n");
		ch->println("  `Bview    `x<player>");
		ch->println("  `Bsurname `x<player>");
		ch->println("  `Bcrest   `x<player> <symbol>");
		ch->println("  `Btrait   `x<player> <1-9> <trait>");
		return;
	}

	ch->println("`YRPSHEET COMMANDS\r\n");
	ch->println("  `Bview       `x- displays your rpsheet");
	ch->println("  `Beyes       `x- sets the color of your eyes");
	ch->println("  `Bhair       `x- sets the color of your hair");
	ch->println("  `Bheight     `x- sets your height in inches");
	ch->println("  `Bweight     `x- sets your weight in pounds");
	ch->println("  `Bbirthplace `x- sets the place of your birth");
	ch->println("  `Bsurname    `x- sets your last name");
	ch->println("  `Btrait      `x- sets your physical and personality traits");
	ch->println("  `Bcrest      `x- sets your character's family symbol");
	ch->println("  `B           `x- once crest is set, it cannot be changed! get it right");
	ch->println("  `B           `x- send a note to rpsupport and/or imm to get it changed");
}
/**************************************************************************/
void show_rpsheet( char_data *ch, char_data *v)
{
	int dsec, icyear, i;
	int feet, inches;
    double cm, kg;

	dsec	= abs((int)(v->pcdata->birthdate - current_time)); // number of IRL seconds between the 2 times
	icyear	= (dsec/ICTIME_IRLSECS_PER_YEAR)+v->pcdata->birthyear_modifier;

	if ( !v->pcdata->height )
		v->pcdata->height = race_table[v->race]->min_height;

	if ( !v->pcdata->weight )
		v->pcdata->weight = race_table[v->race]->min_weight;


	inches	= v->pcdata->height % 12;
	feet	= v->pcdata->height / 12;
	cm		= v->pcdata->height * 2.54;
	kg		= v->pcdata->weight * .454;

	ch->println("`b _____________________________________________________________________________");
	ch->println("`b|\\`B___________________________________________________________________________`b/|");
	ch->println("`b|`B|                                                                           `B|`b|");
	ch->printlnf("`b|`B|`x  Name: `W%-60s       `B|`b|",capitalize(v->name));

	ch->printlnf("`b|`B|`x  Surname: `W%-20s          `xBirthplace: `r%-20s  `B|`b|",
		true_CH(v)->pcdata->surname ? true_CH(v)->pcdata->surname : "(none)" ,
		true_CH(v)->pcdata->birthplace ? true_CH(v)->pcdata->birthplace : "(unset)" );
	if(v->pcdata->birthdate){
		ch->printlnf("`b|`B|`x  Born:  `y%s `x  Age:  `y%-44s  `B|`b|",
			str_width( get_shorticdate_from_time(v->pcdata->birthdate,
						"`#%d`S/`^%d`S/`&%d", v->pcdata->birthyear_modifier),11,false),
			get_ictimediff(v->pcdata->birthdate, current_time, -v->pcdata->birthyear_modifier));

	}else{
		ch->printlnf("`b|`B|`x  Born:  `y%s `x  Age:  `y%-44s  `B|`b|",
			str_width(get_shorticdate_from_time(
				v->player_id - (17*ICTIME_IRLSECS_PER_YEAR),
					"`#%d`S/`^%d`S/`&%d", 0),11,false),
		get_ictimediff(v->player_id-(17*ICTIME_IRLSECS_PER_YEAR),
			current_time, 0));
	}
	ch->println("`b|`B|                                                                           `B|`b|");

	if ( HAS_CONFIG( ch, CONFIG_METRIC ))
	{
		ch->printlnf("`b|`B|`x  Hair: `W%-12s`x   Eyes: `W%-12s`xHeight: `W%d cm    `xWeight: `W%-4dkg  `B|`b|",
			true_CH(v)->pcdata->haircolor ? capitalize(strip_color(true_CH(v)->pcdata->haircolor )) : "(unset)",
			true_CH(v)->pcdata->eyecolor ? capitalize(strip_color(true_CH(v)->pcdata->eyecolor )) : "(unset)",
			(int)cm,
			(int)kg );
	}
	else
	{
		ch->printlnf("`b|`B|`x  Hair: `W%-12s`x   Eyes: `W%-12s`xHeight: `W%d'%d\"    `xWeight: `W%-4d lbs  `B|`b|",
			true_CH(v)->pcdata->haircolor ? capitalize(strip_color(true_CH(v)->pcdata->haircolor) ) : "(unset)",
			true_CH(v)->pcdata->eyecolor ? capitalize(strip_color(true_CH(v)->pcdata->eyecolor )) : "(unset)",
			feet,
			inches,
			v->pcdata->weight );
	}
	ch->printlnf("`b|`B|`x  Family Crest: `W%-40s                   `B|`b|",
		true_CH(v)->pcdata->crest ? capitalize(strip_color(true_CH(v)->pcdata->crest)) : "(none)" );
	ch->println("`b|`B|___________________________________________________________________________`B|`b|");
	ch->println("`b|`B|                                                                           `B|`b|");
	ch->println("`b|`B| `CTraits                                                                    `B|`b|");
	ch->println("`b|`B|                                                                           `B|`b|");
	for ( i=0; i < 6; i++ )
	{
		ch->printlnf("`b|`B| `C%d. `W%-60s           `B|`b|",
		i+1,
		v->pcdata->trait[i] ? capitalize( v->pcdata->trait[i] ) : "" );
	}
/*	ch->println("`b|`B|                                                                           `B|`b|");
	ch->println("`b|`B|___________________________________________________________________________`B|`b|");
	ch->println("`b|`B|                                                                           `B|`b|");
	ch->println("`b|`B| `CPhysical Traits                                                           `B|`b|");
	ch->println("`b|`B|                                                                           `B|`b|");*/
	for ( i=6; i < 9; i++ )
	{
		ch->printlnf("`b|`B| `C%d. `W%-60s           `B|`b|",
		i+1,
		v->pcdata->trait[i] ? capitalize( v->pcdata->trait[i] ) : "" );
	}
	ch->println("`b|`B|___________________________________________________________________________`B|`b|");
	ch->println("`b|/___________________________________________________________________________`b\\|");
	return;


}

/**************************************************************************/
void do_rptell( char_data *ch, char *argument )
{
	char arg[MIL], buf[MSL];
    char_data *nch, *victim, *nvictim;

    // unswitched mobs can't send rptells
    if (IS_UNSWITCHED_MOB(ch))
    {
		return;
    }

	if (( !IS_IMMORTAL(ch) || !IS_RP_SUPPORT(ch)))
	{
		do_huh(ch,"");
		return;
	}

	// Check if they're being ordered to do this
	if ( IS_SET( ch->dyn, DYN_IS_BEING_ORDERED ))
	{
		if ( ch->master )
			ch->master->println( "Not going to happen." );
		return;
	}

    argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		ch->println("RP tell whom what?");
		return;
	}

	if ( ( victim = get_whovis_player_world( ch, arg ) ) == NULL
		||  IS_NPC(victim) )
	{
		ch->println( "They aren't here." );
		return;
	}
	//Put in these lines from do_tell in because it made some sense. <shrug>
	if (victim->controlling && victim->controlling->desc)
	{
		victim=victim->controlling;
	}

	if(ch==victim)
	{
		ch->println( "No point in sending an rptell to yourself." );
		return;
	};

	//Also from do_tell, looked good to put.
	if ( victim->desc == NULL)
	{
		act("$N seems to have misplaced $S link...try again later.", ch,NULL,victim,TO_CHAR);
		sprintf(buf,"An RP support member tells you '%s'\r\n`x", argument);
		buf[0] = UPPER(buf[0]);
		add_buf(victim->pcdata->buffer,buf);
		return;
	}

	if (IS_SET(victim->comm,COMM_AFK))
	{
		if (IS_NPC(victim))
		{
			act("$E is AFK, and not receiving tells.",ch,NULL,victim,TO_CHAR);
			return;
		}

		act("$E is AFK, but your tell will go through when $E returns.", ch,NULL,victim,TO_CHAR);

		sprintf(buf,"An RP support member tells you '%s'\r\n", argument );
		buf[0] = UPPER(buf[0]);
		add_buf(victim->pcdata->buffer,buf);

		victim->printlnf( "An RP support member just tried to tell you '%s' (you are marked as afk)`x", argument);
		return;
	}

	if(!IS_NPC(ch))
		ch->pcdata->did_ooc=true;

	//Send the tell to the player.
	victim->printlnf("`y%s tells you '%s'"
		"(you can use rpreply to talk back if you need to)`x\r\n",
		IS_IMMORTAL(victim)?PERS(ch,victim):"An RP support member", argument );

	//Start the broadcast of the rptell to all rpsupport
	for ( nch = player_list; nch; nch = nch->next_player )
	{
		if (true_CH(ch)==nch)
		{
			ch->printlnf( "`MYou tell %s '%s`M'", victim->name, argument);
			continue;
		}

		// don't see both the rp broadcast and the personal message
		if(	nch==victim)
		{
			continue;
		};

//		nvictim=nch;
		nvictim=nch;
		// rptalk going thru switch
		if (nvictim->controlling && nvictim->controlling->desc)
			nvictim=nvictim->controlling;

		if (IS_RP_SUPPORT(nch))
		{
			if (IS_IMMORTAL(nch))
			{
				nvictim->printlnf( "`M<%s rptells %s>: '%s`M'`x",
					true_CH(ch)->name, victim->name, argument);
			}else{
				nvictim->printlnf( "`M<An RP support member tells %s>: '%s`M'`x", victim->name, argument);
			}
		}
	}
	return;
}
/**************************************************************************/
void do_rpreply( char_data *ch, char *argument )
{
	char_data *nch, *victim;

    // unswitched mobs can't send nreplies
    if (IS_UNSWITCHED_MOB(ch))
    {
		return;
    }

    if ( IS_NULLSTR(argument) )
    {
        ch->println("RPreply what?");
        return;
    }

	//Start the broadcast of the rpreply to all rpsupport
    for ( nch = player_list; nch; nch = nch->next_player )
    {
		if (true_CH(ch)==nch)
		{
			ch->printlnf( "`MYou rpreply '%s`M'", argument);
			continue;
		}

		victim=nch;

		// rpreply going thru switch
		if (victim->controlling && victim->controlling->desc)
			victim=victim->controlling;

		if ( IS_RP_SUPPORT(nch))
		{
			victim->printlnf( "`M<%s rpreplies> '%s`M'`x",
					true_CH(ch)->name, argument);
		}
	}
	return;
}

/**************************************************************************/
void do_flip( char_data *ch, char *argument )
{
	int				face = number_range( 0, 1);
	bool			spam = false;
	OBJ_INDEX_DATA	*coinIndex;
	OBJ_DATA		*coin;

	if ( IS_NULLSTR( argument )) {
		ch->println("What do you want to flip, a silver or a gold coin?");
		return;
	}

	if ( ch->desc && ch->desc->repeat>5 )
		spam = true;

	if ( is_name( argument, "silver" ))
	{
		if ( ch->silver < 1 ) {
			ch->println("You have no silver coins to flip.");
			return;
		}

		coinIndex	= get_obj_index( OBJ_VNUM_SILVER_ONE );
		coin		= create_object( coinIndex);
		ch->silver -= 1;
		free_string( coin->long_descr );

		if ( face ) {
			act( "$n flips a coin high into the air and lands showing heads.", ch, NULL, NULL, TO_ROOM );
			ch->println("You flip a coin high into the air.  It lands showing heads.");
			coin->long_descr = str_dup( "You see a silver coin showing heads." );
		} else {
			act( "$n flips a coin high into the air and lands showing tails.", ch, NULL, NULL, TO_ROOM );
			ch->println("You flip a coin high into the air.  It lands showing tails.");
			coin->long_descr = str_dup( "You see a silver coin showing tails." );
		}
		if ( !IS_OOC(ch) ) { obj_to_room( coin, ch->in_room ); }
	}
	else if ( is_name( argument, "gold" ))
	{
		if ( ch->gold < 1 ) {
			ch->println("You have no gold coins to flip.");
			return;
		}

		coinIndex	= get_obj_index( OBJ_VNUM_GOLD_ONE );
		coin		= create_object( coinIndex);
		ch->gold   -= 1;
		free_string( coin->long_descr );

		if ( face ) {
			act( "$n flips a coin high into the air and lands showing heads.", ch, NULL, NULL, TO_ROOM );
			ch->println("You flip a coin high into the air.  It lands showing heads.");
			coin->long_descr = str_dup( "You see a gold coin showing heads." );
		} else {
			act( "$n flips a coin high into the air and lands showing tails.", ch, NULL, NULL, TO_ROOM );
			ch->println("You flip a coin high into the air.  It lands showing tails.");
			coin->long_descr = str_dup( "You see a gold coin showing tails." );
		}
		if ( !IS_OOC(ch)) { obj_to_room( coin, ch->in_room ); }
	}
	else
	{
		ch->println("What do you want to flip, a silver or a gold coin?");
	}
	if ( spam )
		WAIT_STATE( ch, PULSE_PER_SECOND * 10 );
}
/**************************************************************************/
void do_pregnant(char_data *ch, char *argument)

{
	char arg[MIL], buf[MSL];
	char_data *victim;
	int chance;
	one_argument( argument, arg );
	victim = ch;
	if(IS_AFFECTED3(victim, AFF3_PREGNANT)){
		return;}
	chance = 0;





	victim->println("`#`CYou suddenly realize you are pregnant. `YCongratulations!!`^");

	SET_BIT(victim->affected_by3, AFF3_PREGNANT);

    if (!str_cmp( victim->name, "Myseri" ) || !str_cmp( victim->name, "myseri" )){
    victim->pcdata->pregnant = current_time+ (3 * ICTIME_IRLSECS_PER_MONTH);
    }else{
	victim->pcdata->pregnant = current_time+ (9 * ICTIME_IRLSECS_PER_MONTH);
    }
	victim->printlnf("You will give birth on %s (%.24s)",
		get_shorticdate_from_time(victim->pcdata->pregnant, "", 0),
		ctime(&victim->pcdata->pregnant));
	return;
}
/**************************************************************************/
/**************************************************************************/
void dream_kill(char_data *ch)
{
	ch->dreamp = 0;
	ch->dreamn = 0;
	save_char_obj(ch);
	return;
}
/**************************************************************************/
void dream_reset(char_data *ch)
{
	ch->dreamn = 0;
	ch->dreamp = 0;
	ch->dreamn = number_range(0,23);
	save_char_obj(ch);
	return;
}
/**************************************************************************/
/**************************************************************************/
void dream_update(void)
{
     char_data *ch;

     for ( ch = char_list; ch; ch = ch->next )
     {
	if ( ch->desc == NULL || ch->desc->connected_state != CON_PLAYING )
	   continue;

        if(IS_NPC(ch))
                return;

	// If the character is asleep.
	if(!IS_AWAKE(ch))
	{
		if(ch->dreamn == 0)
		{
			ch->print("`#`SYou dream:`c ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou're walking on a dead, grey path in the dark woods...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cNothing looks familiar to you, yet this image is clear.`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cWherever you are, you must get out. They're watching you....`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cThey know who you are, and know where you've come from.`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cYou hear a voice, its telling you to be careful and prepare.`^");
				act( "$n passes into a deeper sleep...", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 1)
		{
			ch->print("`#`SYou dream:`c ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cWhoa! you thought you were sleeping, but now you're standing...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cIn the center of a stage! The lights shine down on you...`^");
				ch->dreamp++;
				break;
			    case 2:
    				ch->println("`#`cThe crowd cheers as you are standing there dressed in your armor...`^");
				ch->dreamp++;
				break;
			    case 3:
    				ch->println("`#`cThe crowd settles..awaiting your next move.. you don't know what to do!`^");
				ch->dreamp++;
				break;
			    case 4:
    				ch->println("`#`cAn overwhelming effect of stage fright washes over you! You panic...`^");
				ch->dreamp++;
				break;
			    case 5:
    				ch->println("`#`cRisking everyone laughing at you, you start to sing a song..`^");
				ch->dreamp++;
				break;
			    case 6:
    				ch->println("`#`cThe crowd starts booing at your performance! You run off the stage!`^");
				act( "Mumbling strange words, $n passes into a deeper sleep.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		// Nightmare Dream
		else if(ch->dreamn == 2)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cMasses are moving in a void. They are flowing though it...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cThey are closing in on you. You look closer. You start maneuvering the masses...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cYou clench your fist, but it feels smaller than usual. You open it..`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cAnd reclench it, but it still feels small.. and hard, it feels like a rock...`^");
				ch->print("`#`SYou dream:`^ ");
				ch->println("`#`cYou maneuver the masses with the power of your mind. `^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cIt's a game. The masses are closing in. You look in closer...`^");
				ch->print("`#`SYou dream:`^ ");
				ch->println("`#`cThey're getting closer...you can't win, you can't escape!`^");
				ch->println("`#`RYou wake up in a `Ccold sweat`R, fearing for your life.`x");
				do_wake(ch, "");
				dream_kill(ch);
	//			do_yell(ch,"AAAAAGGGGHHHH! The HORROR!!!");
				break;
			}
		}
		else if(ch->dreamn == 3)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou are walking in a forest, and you don't know where it is..`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cYou've never been here before, but yet it seems so familiar to you...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cAs you're walking, you come across a pond. You see your reflection...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cOr do you?.. It doesn't look like you.. but it has to be...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cYou somehow feel better about knowing your true self.`^");
				act( "$n passes into a deeper sleep...", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		// Nightmare Dream
		else if(ch->dreamn == 4)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou think about vampires and one appears before you...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cIt reaches for you and you scream in terror...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cA swirl of eternal darkness flows around you...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cThe fangs sink into your neck, draining your blood...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`RYou wake up in a `Ccold sweat`R, fearing for your life.`x");
				do_wake(ch, "");
				dream_kill(ch);
//				do_yell(ch,"AAAAAGGGGHHHH! The HORROR!!!");
				break;
			}
		}
		else if(ch->dreamn == 5)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou shudder from the cold as you see a scene of a vast frozen wasteland...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cYou begin to stumble around in the cold snow drifts and frozen ponds...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cThe cold begins to reach your fingers and toes as you begin to lose feeling...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cAll of a sudden a man dressed in black appears before you...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cIn one hand he is holding a dagger and in the other, `YYOUR HEAD!!`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cAs you begin to scream it slowly fades away as you feel cold...`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cThe frost from his cold blade is cutting through you...`^");
				ch->println("`#`RYou wake up in a `Ccold sweat`R, fearing for your life.`x");
				do_wake(ch, "");
				dream_kill(ch);
//				do_yell(ch,"AAAAAGGGGHHHH! The HORROR!!!");
				break;
			}
		}
		else if(ch->dreamn == 6)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cA warm incredible feeling of happiness washes over you....`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cIt feels like you are floating on a bed of air...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cThousands of butterflies are fluttering over you...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cA far off harp peacefully fills the air with soul-stirring music...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cAs you open your eyes, you realize the sweet aroma filling the air..`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cAre from the hundreds of flowers on and around you, with more floating in the air...`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cYou close your eyes, hoping to retain these feelings forever.`^");
	    			act( "The air is filled with sweetness as hundreds of flower petals float down from the sky to cover $n as $e lies sleeping.", ch, NULL, NULL, TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 7)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cCreak...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cSomeone... or something is approaching...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cYou struggle to awaken as you hear the whisper of a sword being drawn...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cRed eyes glare evilly at you as you frantically roll out of bed reaching...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cIn vain for your weapon...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cSwish...`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cDarkness...`^");
				act( "$n passes into a deeper sleep...", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 8)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou are walking down the street on a bright and sunny morning...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cPeople are laughing and smiling happily as you walk by and you smile back...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cIt sure is nice to see people happy for once...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cYou continue your walk down the street and stop to look in a store window...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cYou look at the beautiful dishes. Looking more closely in the window you gasp in horror..`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cYOU FORGOT TO PUT YOUR CLOTHES ON THIS MORNING!!!!`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cYou turn deep red from embarassment and hide behind a bush...`^");
				act( "$n turns beet red and mumbles in their sleep.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 9)		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cAhhhh...  Asleep at last, it's been a long time since you've had a good night's sleep...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cAll of a sudden you feel a burst of energy run through your body. You break out in a sweat...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cYou feel your body being pulled down, down, down.. deep into the earth...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cThen a deep voice speaks, 'I have been awaiting your arrival.' it booms...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cLooking around frantically you see a dark figure with horns dressed in a blood-stained cloak...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`c'You have been very bad...' it continues 'for this I will punish you!'`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cYou are suddenly thrown into a giant flaming pit, and screaming fills your ears...`^");
				act( "$n passes into a deeper sleep...", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 10)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou hear flutes in the distance. As the fog clears you realize that...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cYou are standing in a quaint village filled with small happy faeries. A small male approaches...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cHe smiles, puts a pipe to his lips, and begins to play a tune...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cHe walks away - and you begin to follow. At first you are enchanted...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`c...but you slowly come to the realization you can't stop! You struggle against the sound...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cYou try throwing your hands over your ears...but it solves nothing!`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cHe leads you away from the village into a dark, ominous forest....`^");
				ch->dreamp++;
				break;
			    case 7:
				ch->println("`#`cThe sound begins to drill into your eardrums...then your brain...`^");
				act( "$n passes into a deeper sleep...", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 11)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou are being led down a corridor. Your arms are shackled and held by zombies...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cThey force you into a large dark throne room. Before you sits Lord Jadus...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cHe glares at you, as the other immortals around him whisper your secrets into his ear...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cJadus extends his arm, makes a fist, and points his thumb down to the floor...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cExcruciating pain fills your entire being, and you look down at your body...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cYour skin dissolves into dust. You open your mouth, and a scream of hopelessness escapes...`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cEverything goes dark......`^");
				act( "$n passes into a deeper sleep...", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 12)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou're resting peacefully, dreaming about beautiful green fields, full of colorful flowers...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cThe sun shines brightly overhead, when an odd sensation awakens you...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cYou wake up to feel an odd burning sensation on your left hand, and look down...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cYou find your hand on fire! Bright flames and greasy orange smoke rise from the palm of your hand...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cYou sit helplessly and watch the skin drip and crackle, flesh peeling away from the bones...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cYou begin to scream in agony...`^");
				act( "$n moans as if in pain while they sleep.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 13)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou hear them coming...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cThey're coming, there's too many of them. What will you do ?`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cSuddenly, they crest the ridge, and you are bowled over by them...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`c2000 cute puppies who want to lick your face...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cThey're all over you. Well, at least they're housebroken...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cAren't they?`^");
				act( "$n passes into a deeper sleep...", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 14)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou walk towards the top of a hill, searching for something...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cYou see something in the distance at the hilltop, you are almost there...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cA smell like no other assaults you as you reach the summit of the hill...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`c'YES!!' you exclaim, you have finally found it...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cAfter years of searching, you have found the Eternal Tree of Everlasting Cupcakes...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cYou reach into the tree, selecting a delicious cupcake from among the branches...`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cNo matter how hard you try, you can't seem to get the cupcake into your mouth...`^");
				act( "$n passes into a deeper sleep...", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 15)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou walk along the edge of a beautiful seashore...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cBarefoot, the sand squishes between your toes...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cYou walk through the water as it crashes upon the shore...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cLooking just up ahead, you notice a large but strange rock...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cApproaching it, you ponder to yourself about it...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cThe rock transforms before you. It's not a rock...`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cIT'S A ROCK LOBSTER!!!`^");
				act( "$n mutters 'rock lobster' in their sleep.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 16) // Strift
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou are surrounded by green fields, fresh water and lush trees...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cYou begin walking until you spot a brook. You jump in the water and begin swimming...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cSuddenly the trees begin to burn, the grass turns to piles of corpses and bones...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cThe water turns to blood before your eyes...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cYou find yourself surrounded with no way of escape...`^");
				act( "$n tosses about in their sleep.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 17)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cWhile traveling you spot an entrance to a cave and tell your friends to hold up while you go explore....`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cAs you reach inside it is dark and dank, the walls feel like slime...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cThe floor has so many bugs you can hear the crunching of their bones and shells...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cYou begin to hear screams of pain and horror as you go deeper into the cave...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cWhen an impulse drives you to run screaming from the cave..`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cOnly to find your friends murdered and thier blood on your hands!`^");
				act( "$n mutters something in their sleep.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 18)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou're walking through a forest blanketed by a heavy-mist...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cA chill is sent up your spine as there's a crunch of twigs behind you...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cYou panic and started running, swerving and dodging trees and low branches...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cYou trip on the path and scream as what feels like fingers start touching you...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cSomething bushy touches your nose and you open your eyes...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cYou see your foe before you... a squirrel.`^");
				act( "$n wiggles their nose in their sleep.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 19)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou're floating on a rainbow cloud high in the sky...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cThe stars come out to greet you, smiling, laughing and waving as they twinkle...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cThe cloud swerves around and takes you to an even bigger star, wearing a golden crown...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`c'I am the Northern Star,' the star says. 'Welcome to my domain.  Here is a gift...'`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cThe Northern Star gives you a pouch, and you open it...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cStar dust goes everywhere, and you begin to sneeze...`^");
				act( "$n sneezed in their sleep.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 20)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou open your eyes and find yourself underwater...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cSharks swim around you in circles like vultures...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cYou scream out, losing the air you hold in your lungs and swallow water...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cUsing all the strength you can muster you swim for the surface...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cThe light from the surface just gets further away...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cYour body grows weak, everything fades to darkness...`^");
				act( "$n begins to gasp for breath.", ch, NULL, NULL,TO_ROOM);
				do_wake(ch, "");
				dream_kill(ch);
				act( "You awake gasping for breath.", ch, NULL, NULL,TO_CHAR);
//				do_yell(ch,"AAAAAGGGGHHHH! The HORROR!!!");
				break;
			}
		}
		else if(ch->dreamn == 21)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou're standing on a floating chessboard in a void of nothing...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cYou stare over to the other side to see a giant tyrannosaurus rex...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cThe dinosaur's high-pitched roar echoes as it charges at you...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cIn your hand is a club... which is not a good weapon against it, so you run...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cSuddenly the squares fall from the board... you jump to remaining squares...`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cEnding up on the dinosaurs square, it's jaws close in around you...`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cCRUNCH......`^");
				act( "$n curls up into a tiny ball from a bad dream.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 22)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou look around and find yourself in a cemetery. It is very dark...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cYou hear a noise. Was it behind you? In front? Where did it come from ?`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cYou panic and start to run. You trip over a tombstone...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cYou land hard on the ground. A sound starts to come from the ground...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cA hand pops out of the ground and grabs your ankle!`^");
				ch->dreamp++;
				break;
			    case 5:
				ch->println("`#`cIt starts to pull you into the grave, then you notice the tombstone...`^");
				ch->dreamp++;
				break;
			    case 6:
				ch->println("`#`cInscribed on the tombstone, is YOUR OWN NAME!!!`^");
				act( "$n curls up into a tiny ball from a bad dream.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}
		else if(ch->dreamn == 23)
		{
			ch->print("`#`SYou dream:`^ ");
			switch(ch->dreamp)
			{
			    case 0:
				ch->println("`#`cYou seem to be walking down a serene tree lined path...`^");
				ch->dreamp++;
				break;
			    case 1:
				ch->println("`#`cYou come to a clearing full of flowers and wildlife, seemingly a paradise...`^");
				ch->dreamp++;
				break;
			    case 2:
				ch->println("`#`cSuddenly, a dragon with scales that gleam brighter than rubies seems to swoop down from above, setting flame to the meadow...`^");
				ch->dreamp++;
				break;
			    case 3:
				ch->println("`#`cYou look down at your hands, and all you see are charred stumps, and then you realize you?re suddenly shorter and standing on a pile of ashes...`^");
				ch->dreamp++;
				break;
			    case 4:
				ch->println("`#`cA scream tries to issue from your mouth, but all that comes out is more ash, clouding your vision and filling it with a pitch dark haze...`^");
				act( "$n curls up into a tiny ball from a bad dream.", ch, NULL, NULL,TO_ROOM);
				dream_reset(ch);
				break;
			}
		}

	} // end awake
    } // end for loop
    return;
}
/**************************************************************************
Dead Grey Path
On Stage
The Masses
The Reflection
Vampires
Frozen Wasteland
Flowers & Butterflies
Creak/Swish
No Clothes On
Going To Hell
Pied Piper
Jadus Rules
Burning Hand
Puppies
Cupcakes
Rock Lobster
Water to Blood
The Cave
The Squirrel
Star Dust
Underwater
Chessboard Dinosaur
Tombstone
Dragonfire*/
/**************************************************************************/
/**************************************************************************/
void do_sterilize(char_data *ch, char *argument)
{
	char arg1[MIL];
//  connection_data *d;
    char_data *victim;

    argument = one_argument( argument, arg1 );

    if ( IS_NULLSTR(arg1) )
    {
        ch->println( "Sterilize whom?" );
        return;
    }


	// search first in the world for players before searching for mobs
	victim=get_whovis_player_world( ch, arg1 );
	if ( !victim)
	{
	    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
		{
			ch->println( "They aren't here." );
			return;
		}
	}
	if(IS_AFFECTED3(victim, AFF3_STERILE)){
		REMOVE_BIT(victim->affected_by3, AFF3_STERILE);
        victim->println("`#`CYou have been unsterilized!`^");
        ch->println("`#`CYou have unsterilized them!`^");
        return;
			}
	if(IS_AFFECTED3(victim, AFF3_PREGNANT)){
	victim->println("You have lost your child due to sterilization!!`x");
					REMOVE_BIT(victim->affected_by3, AFF3_PREGNANT);}
	SET_BIT(victim->affected_by3, AFF3_STERILE);
	victim->println("`#`CYou have been sterilized!`^");
	ch->println("`#`CYou have sterilized them!`^");
	return;
}
/**************************************************************************/
/**************************************************************************/
void do_pullfate(char_data *ch, char *argument)
{
//  char_data *mob;
    char arg[MIL];
    int card, item;

	// exception in here so players can buy heals while badly hurt
	// but can't while sleeping
	if (ch->position == POS_SLEEPING)
	{
		ch->println("You can't do that while sleeping.");
        return;
	}
//	mob = get_mob_index( 64 );
//	mob = create_mobile( get_mob_index(91), 0 );
	// check for healer
/*  for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( IS_NPC(mob) && IS_SET(mob->act2, ACT2_DECK_OF_FATE) )
            break;
    }

    if ( mob == NULL )
    {
        ch->println("You can't do that here.");
        return;
    }*/

	obj_data *deck;
	bool fdeck=false;
    for ( deck = ch->carrying; deck; deck = deck->next_content ){
        if ( deck->pIndexData->vnum == 701
			 && deck->wear_loc != WEAR_NONE) {
			fdeck = true;
            break;}
    }

	if(!fdeck){
		ch->println("You need to be holding a deck in order to use this command.");
		return;}

	if (IS_NPC(ch))
	{
		ch->println("Not while transformed.");
		return;
	}

	if ( ch->fighting ) {
		ch->println("You seem to be too busy do anything.");
		return;
	}

	if ((ch->level<19) || (!IS_SET(ch->act,PLR_CAN_ADVANCE)))
	{
		ch->println("Sorry but you are not experienced enough or letgained.");
		return;
	}

	if( IS_AFFECTED2(ch, AFF2_BAN_DECK))
	{
		ch->println("Sorry but you are not allowed to use the deck anymore.");
		return;
	}


    argument=one_argument(argument,arg);

    if (arg[0] == '\0')
    {
        // display information
		act("$N says 'The deck is a mysterious thing.'",ch,NULL,NULL,TO_CHAR);
		act("$N says 'Some things are good, some are bad.'",ch,NULL,NULL,TO_CHAR);
		act("$N says 'All effects are permanent, good or bad.'",ch,NULL,NULL,TO_CHAR);
		act("$N says 'The immortals will not reimburse for a bad card.'",ch,NULL,NULL,TO_CHAR);
		act("$N says 'So do not complain to them if you get a bad card.'",ch,NULL,NULL,TO_CHAR);
		act("$N says 'Type DRAW CARD to draw a card from the deck of fate.'",ch,NULL,NULL,TO_CHAR);
		return;
    }

    if (!str_prefix(arg,"fate"))
    {
	act("You draw a card from the deck of fate.", ch,NULL,NULL,TO_CHAR);
	act( "`#$n pulls a card from the `YDeck of Fate`^.", ch, NULL, NULL, TO_ROOM );
	WAIT_STATE(ch,PULSE_VIOLENCE);
	card = number_range(1,22);
//	card=14;
	unequip_char( ch, deck );
	obj_from_char( deck );
	extract_obj(deck);
	if (card == 1)
	{
		act("`#You draw the `YJester`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YJester`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You gain `Y1500 experience`^.", ch,NULL,NULL,TO_CHAR);
		gain_exp(ch, 1500);
		return;
	}
	else
	if (card == 2)//worked
	{
		act("`#You draw the `YHarpy`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YHarpy`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You lose `Y1500 experience`^.", ch,NULL,NULL,TO_CHAR);
		if(IS_IMMORTAL(ch)){
			ch->println("Your immortality saves you from harm.");
			return;
		}
		gain_exp(ch, -1500);
		return;
	}
	else
	if (card == 3)//worked
	{
		act("`#You draw the `YHeart`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YHeart`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You feel your `YCONSTITUTION`^ increase.", ch,NULL,NULL,TO_CHAR);
		ch->perm_stats[STAT_CO] += 3;
		ch->potential_stats[STAT_CO] += 3;
		if (ch->perm_stats[STAT_CO] > 101)
			ch->perm_stats[STAT_CO] = 101;
		if (ch->potential_stats[STAT_CO] > 101)
			ch->potential_stats[STAT_CO] = 101;
		return;
	}
	else
	if (card == 4)//worked
	{
		act("`#You draw the `YIdiot`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YIdiot`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You suddenly feel stupid as you lose 5 points `YINTUITION`^.", ch,NULL,NULL,TO_CHAR);
		if(IS_IMMORTAL(ch)){
			ch->println("Your immortality saves you from harm.");
			return;
		}
		ch->perm_stats[STAT_IN] -= 5;
		ch->potential_stats[STAT_IN] -= 5;
		if (ch->perm_stats[STAT_IN] <= 5){
			ch->perm_stats[STAT_IN] = 5;}
		if (ch->potential_stats[STAT_IN] <= 5){
			ch->potential_stats[STAT_IN] = 5;}

		return;
	}
	else
	if (card == 5)//worked
	{
		act("`#You draw the `YGem`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YGem`^.", ch, NULL, NULL, TO_ROOM );
		act("`#Your pockets feel heavier as you gain `Y5,000 gold`^!", ch,NULL,NULL,TO_CHAR);
		ch->gold += 5000;
		return;
	}
	else
	if (card == 6)//worked
	{
		act("`#You draw the `YRuin`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YRuin`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You lose all of your wealth and riches. You are `YPOOR!`^", ch,NULL,NULL,TO_CHAR);
		if(IS_IMMORTAL(ch)){
			ch->println("Your immortality saves you from harm.");
			return;
		}
		ch->gold = 0;
		ch->silver = 0;
		ch->bank = 0;
		return;
	}
	else
	if (card == 7)//worked
	{
		act("`#You draw the `YJudge`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YJudge`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You are granted a free `YPARDON`^ from prison card.", ch,NULL,NULL,TO_CHAR);
		obj_data *o;
		o=create_object(get_obj_index(491)); // Pardon card
	        obj_to_char( o, ch );
		return;
	}
	else
	if (card == 8)//worked
	{
		act("`#You draw the `YTalon`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YTalon`^.", ch, NULL, NULL, TO_ROOM );
		act("`#Your `Ymagical items`^ are torn from you.", ch,NULL,NULL,TO_CHAR);
		if(IS_IMMORTAL(ch)){
			ch->println("Your immortality saves you from harm.");
			return;
		}
		obj_data *obj;
		obj_data *obj_next;
	  	for (obj = ch->carrying; obj != NULL; obj = obj_next)
	    	{
		    obj_next = obj->next_content;
		    if(IS_SET(obj->extra_flags, OBJEXTRA_MAGIC))
		    {
		            unequip_char( ch, obj );
		            obj_from_char( obj );
			    extract_obj(obj);
			    act( "$p is torn from $n!", ch, obj, NULL, TO_ROOM );
			    act( "$p is torn from your grasp!", ch, obj, NULL, TO_CHAR );
		    }
		}
		save_char_obj(ch);;
		return;
	}
	else
	if (card == 9)//worked
	{
		act("`#You draw the `YSerpent`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YSerpent`^.", ch, NULL, NULL, TO_ROOM );
		act("`#Your `YPRESENCE`^ is now 101.", ch,NULL,NULL,TO_CHAR);
		ch->perm_stats[STAT_PR] = 101;
		ch->potential_stats[STAT_PR] = 101;
		return;
	}
	else
	if (card == 10)//worked
	{
		act("`#You draw the `YWeakling`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YWeakling`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You suddenly feel weak as you lose 5 points `YSTRENGTH`^.", ch,NULL,NULL,TO_CHAR);
		if(IS_IMMORTAL(ch)){
			ch->println("Your immortality saves you from harm.");
			return;
		}
		ch->perm_stats[STAT_ST] -= 5;
		ch->potential_stats[STAT_ST] -= 5;
		if (ch->perm_stats[STAT_ST] <= 5){
			ch->perm_stats[STAT_ST] = 5;}
		if (ch->potential_stats[STAT_ST] <= 5){
			ch->potential_stats[STAT_ST] = 5;}

    		return;
	}
	else
	if (card == 11)//worked
	{
		act("`#You draw the `YKey`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YKey`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You gain a `Ymagic item`^!", ch,NULL,NULL,TO_CHAR);
		item = number_range(1,5);
		obj_data *o;
		int numdie, dietype, random, lblevel;
		lblevel	   = ch->level + 6;
		if (lblevel >= 101){
		  lblevel = 100;}
		if(item == 1)
		{
			o=create_object(get_obj_index(497)); // Celestial Dagger
			o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );

		// damage lookup failed, use polarmost damage class,
		if ( numdie == 0 ||  dietype == 0 ){
		numdie  = weapon_balance_lookup( lblevel, 0, 0 );
		dietype = weapon_balance_lookup( lblevel, 0, 1 );}
		o->value[1]	  = numdie;
		o->value[2]	  = dietype;
		        obj_to_char( o, ch );
		}
		else if(item == 2)
		{
			o=create_object(get_obj_index(496)); // Whip of Anihilation
			o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );

		// damage lookup failed, use polarmost damage class,
		if ( numdie == 0 ||  dietype == 0 ){
		numdie  = weapon_balance_lookup( lblevel, 0, 0 );
		dietype = weapon_balance_lookup( lblevel, 0, 1 );}
		o->value[1]	  = numdie;
		o->value[2]	  = dietype;
		        obj_to_char( o, ch );
		}
		else if(item == 3)
		{
			o=create_object(get_obj_index(495)); // Axe of the Rager
			o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );

		// damage lookup failed, use polarmost damage class,
		if ( numdie == 0 ||  dietype == 0 ){
		numdie  = weapon_balance_lookup( lblevel, 0, 0 );
		dietype = weapon_balance_lookup( lblevel, 0, 1 );}
		o->value[1]	  = numdie;
		o->value[2]	  = dietype;
		        obj_to_char( o, ch );
		}
		else if(item == 4)
		{
			o=create_object(get_obj_index(494)); // Mace of Death
			o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );

		// damage lookup failed, use polarmost damage class,
		if ( numdie == 0 ||  dietype == 0 ){
		numdie  = weapon_balance_lookup( lblevel, 0, 0 );
		dietype = weapon_balance_lookup( lblevel, 0, 1 );}
		o->value[1]	  = numdie;
		o->value[2]	  = dietype;
		        obj_to_char( o, ch );
		}
		else if(item == 5)
		{
			o=create_object(get_obj_index(493)); // Berserker Sword
			o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );

		// damage lookup failed, use polarmost damage class,
		if ( numdie == 0 ||  dietype == 0 ){
		numdie  = weapon_balance_lookup( lblevel, 0, 0 );
		dietype = weapon_balance_lookup( lblevel, 0, 1 );}
		o->value[1]	  = numdie;
		o->value[2]	  = dietype;
		        obj_to_char( o, ch );
		}
		act("You suddenly notice something in your inventory.", ch,NULL,NULL,TO_CHAR);
		return;
	}
	else
	if (card == 12)//worked
	{
		act("`#You draw the `YGear`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YGear`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You are sent to `RThe Clock Room!`^.", ch,NULL,NULL,TO_CHAR);
   		act( "$n has been sent to `#`RThe Clock Room!`^", ch, NULL, NULL, TO_CHAR );
		char_from_room(ch);
		char_to_room(ch, get_room_index(3611));
		do_look(ch, "auto");
		return;
	}
	else
	if (card == 13)//worked
	{
		act("`#You draw the `YFates`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YFates`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You will avoid your next `YDEATH`^.", ch,NULL,NULL,TO_CHAR);
		obj_data *o;
		o=create_object(get_obj_index(702)); // Avoid Death Token
	        obj_to_char( o, ch );
		return;
	}
	else
	if (card == 14)//worked
	{
		OBJ_DATA  *statue;
		char buf[MSL];
		char *name, *mobname;
		name        = ch->short_descr;
		mobname = "`#`gMedusa`^";
		act("`#You draw `YMedusa`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws `YMedusa`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You have been turned to `YSTONE!!`^", ch,NULL,NULL,TO_CHAR);
                statue = create_object(get_obj_index(492));
                statue->level = 0;
				statue->cost = (ch->level * 100);
				statue->weight = (ch->pcdata->weight * 6);
				sprintf( buf, statue->short_descr, name );
				replace_string( statue->short_descr, buf );
				sprintf( buf, statue->long_descr, name );
				replace_string( statue->long_descr, buf);
				statue->description = ch->description;
    		obj_to_room( statue, ch->in_room );

		if(IS_IMMORTAL(ch)){
			ch->println("Your immortality saves you from death.");
			return;
		}
            raw_kill( ch, ch );
			info_broadcast(ch, "%s has been toasted by %s.",	ch->name, mobname);
			do_look(ch, "auto");
		return;
	}
	else
	if (card == 15)//worked
	{
		act("`#You draw the `YTrainer`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YTrainer`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You gain `Y10 practices & 5 trains`^!", ch,NULL,NULL,TO_CHAR);
		ch->practice += 10;
		ch->train += 5;
		return;
	}
	else
	if (card == 16)//worked
	{
		act("`#You draw the `YHangman`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YHangman`^.", ch, NULL, NULL, TO_ROOM );
//		act("`#You have been convicted of `RMURDER`^ and imprisoned!", ch,NULL,mob,TO_CHAR);
		act("`#You have been convicted of `RMURDER`^!", ch,NULL,NULL,TO_CHAR);
		ch->pcdata->murder += 1;
//		do_startprison(ch, "");
		return;
	}
	else
	if (card == 17)//worked
	{
		act("`#You draw the `YKing`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YKing`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You gain `R500 Experience`^!", ch,NULL,NULL,TO_CHAR);
		gain_exp(ch, 500);
		return;
	}
	else
	if (card == 18)//worked
	{
		act("`#You draw the `YMists`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YMists`^.", ch, NULL, NULL, TO_ROOM );
		ROOM_INDEX_DATA *pRoomIndex;
		pRoomIndex = get_random_room(ch);
		ch->printf( "Mists swirl around you and you find yourself somewhere else!\r\n" );
		act( "A large cloud of mists swirl around $n and they vanish!", ch, NULL, NULL, TO_ROOM );
		char_from_room( ch );
		char_to_room(ch, pRoomIndex );
		act( "$n slowly appears from a cloud of mists.", ch, NULL, NULL, TO_ROOM );
		do_look( ch, "auto" );
		return;
	}
	else
	if (card == 19)//worked
	{
		act("`#You draw the `YKnight`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YKnight`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You gain a `YKNIGHT `^ as a follower.", ch,NULL,NULL,TO_CHAR);
		if(IS_IMMORTAL(ch)){
			ch->println("Your immortality prevents the Knight from appearing.");
			return;
		}
    		char_data *pet;
		char buf[MSL];
   		pet = create_mobile( get_mob_index(498), 0 );
		pet->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;

		sprintf( buf, "%sA plate on its chest says 'I am pledged to %s'.\r\n",
			pet->description, ch->short_descr );
		free_string( pet->description );
		pet->description = str_dup( buf );
    		char_to_room( pet, ch->in_room );
		pet->level = ch->level;
		pet->leader = ch;
		pet->max_hit = ch->max_hit + 50;
		SET_BIT(pet->act, ACT_PET);
		SET_BIT(pet->affected_by, AFF_CHARM);
	        SET_BIT(pet->dyn, DYN_IS_BEING_ORDERED);
		add_follower( pet, ch );
        pet->leader = ch;
		ch->cardpet = pet;
		return;
	}
	else
	if (card == 20)//worked
	{
//		mob = create_mobile( get_mob_index(92), 0 );
        char *mobname;
		mobname = "`#`rThe Reaper`^";
		act("`#You draw the `YReaper`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YReaper`^.", ch, NULL, NULL, TO_ROOM );
		if(IS_IMMORTAL(ch)){
			ch->println("Your immortality saves you from harm.");
			return;
		}
//		act("`#`YYou LOSE A KARN!!!`^.", ch,NULL,mob,TO_CHAR);
//		ch->pcdata->karns -= 1;
//		ch->position = POS_DEAD;
        raw_kill( ch, ch );
        info_broadcast(ch, "%s has been toasted by %s.",	ch->name, mobname);
//    	kill_char(ch, mob);
//		extract_char( mob1, true );
		do_look(ch, "auto");
		return;
	}
	else
	if (card == 21)//worked
	{
		act("`#You draw the `YArmory`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws the `YArmory`^.", ch, NULL, NULL, TO_ROOM );
		act("`#You gain `Y500 experience and an item`^.", ch,NULL,NULL,TO_CHAR);
		gain_exp(ch, 500);
		item = number_range(1,5);
		obj_data *o;

		int numdie, dietype, random, lblevel;
		lblevel	   = ch->level + 6;
		if (lblevel >= 101){
		  lblevel = 100;}

			if(item == 1)
		{
			o=create_object(get_obj_index(497)); // Celestial Dagger
				o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );
		        obj_to_char( o, ch );
		}
		else if(item == 2)
		{
			o=create_object(get_obj_index(496)); // Whip of Anihilation
				o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );
		        obj_to_char( o, ch );
		}
		else if(item == 3)
		{
			o=create_object(get_obj_index(495)); // Axe of the Rager
				o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );
		        obj_to_char( o, ch );
		}
		else if(item == 4)
		{
			o=create_object(get_obj_index(494)); // Mace of Death
				o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );
				o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );

		        obj_to_char( o, ch );
		}
		else if(item == 5)
		{
			o=create_object(get_obj_index(493)); // Berserker Sword
				o->level = ch->level;
		// first try to get a random damage based on level
		random  = number_range( 0, 7 );
		numdie  = weapon_balance_lookup( lblevel, random, 0 );
		dietype = weapon_balance_lookup( lblevel, random, 1 );

		        obj_to_char( o, ch );
		}

		act("You suddenly notice something in your inventory.", ch,NULL,NULL,TO_CHAR);
		return;
	}
	else
	if (card == 22)//worked
	{
		act("`#You draw `YWrath`^.", ch,NULL,NULL,TO_CHAR);
		act( "$n draws `YWrath`^.", ch, NULL, NULL, TO_ROOM );
		act( "`#`R$N has vanished in a cloud of blackness!`^",  NULL, NULL, ch, TO_NOTVICT );
		act( "`#`RYou have vanished in a cloud of blackness!`^", NULL, NULL, ch, TO_VICT );

		if(IS_IMMORTAL(ch)){
			ch->println("Your immortality saves you from harm.");
			return;
		}

			AFFECT_DATA af;
			ch->println("You feel poison coursing through your veins.");
			af.where     = WHERE_AFFECTS;
			af.type      = gsn_poison;
			af.level     = 125;
			af.duration  = 90;
			af.location  = APPLY_ST;
			af.modifier  = -20;
			af.bitvector = AFF_POISON;
			affect_join( ch, &af );

/*			ch->println("A wave of hopelessness & despair passes over you.");

			af.where     = WHERE_AFFECTS2;
			af.type      = gsn_despair;
			af.level     = 125;
			af.duration  = 80;
			af.location  = APPLY_IN;
			af.modifier = -30;
			af.bitvector = AFF2_DESPAIR;
			affect_to_char (ch, &af);

			af.where = WHERE_AFFECTS2;
			af.type = gsn_despair;
			af.level = 125;
			af.duration = 90;
			af.location = APPLY_HITROLL;
			affect_to_char (ch, &af);
			af.modifier = -20;
			af.bitvector = AFF2_DESPAIR;
			af.location = APPLY_DAMROLL;
			affect_to_char (ch, &af);
			act( "$n falls to the ground in utter despair and hopelessness.",  ch, NULL, NULL, TO_NOTVICT );
*/
			ch->println("You feel cursed by the Deck of Fate.");
			af.where     = WHERE_AFFECTS;
			af.type      = gsn_curse;
			af.level     = 125;
			af.duration  = 90;
			af.location  = APPLY_QU;
			af.modifier  = -20;
			af.bitvector = AFF_CURSE;
			affect_join( ch, &af );

		char_from_room(ch);
		char_to_room(ch, get_room_index(ch->recall_room));
		do_look(ch, "auto");
		return;
	}

	return;

     }


	else
	{
		act("$N says 'Type DRAW CARD on how to draw from the deck of fate.'",
			ch,NULL,NULL,TO_CHAR);
		return;
    }

	return;
}
/**************************************************************************/
/**************************************************************************/

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
