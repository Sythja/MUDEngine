/**************************************************************************/
// deity.cpp - Kereno's uncompleted deity code
/***************************************************************************
 * The Dawn of Time v1.69s.beta6 (c)1997-2010 Kalahn                       *
 * >> A number of people have contributed to the Dawn codebase, with the   *
 *    majority of code written by Kalahn - www.dawnoftime.org              *
 * >> To use this source code, you must fully comply with the dawn license *
 *    in licenses.txt... In particular, you may not remove this copyright  *
 *    notice.                                                              *
 **************************************************************************/

#include "include.h"
#include "olc.h"

void save_area( AREA_DATA *pArea );

RESET_DATA *new_reset_data args ( ( void ) );


// protos
void dedit_read_race(gio_type *, int, void *data, FILE *fp);
void dedit_write_race(gio_type *gio_table, int tableIndex, void *data, FILE *fp);
void dedit_read_rival(gio_type *, int, void *data, FILE *fp);
void dedit_write_rival(gio_type *gio_table, int tableIndex, void *data, FILE *fp);

GIO_CUSTOM_FUNCTION_PROTOTYPE(racetype_write_generic_races_set_for_n_array);
GIO_CUSTOM_FUNCTION_PROTOTYPE(racetype_read_generic_races_set_for_n_array);

// do da GIO thang
GIO_START( DEITY_DATA )
GIO_STRH(  name,			"Name         " )
GIO_STRH(  description,		"Description  " )
GIO_SHINTH(shrinevnum,		"Shrine       " )
GIO_SHINTH(symbol[0],		"Symbol1	  " )
GIO_SHINTH(symbol[1],		"Symbol2	  " )
GIO_SHINTH(symbol[2],		"Symbol3      " )
GIO_SHINTH(symbol[3],		"Symbol4	  " )
GIO_SHINTH(essence,		    "Essence      " )
//GIO_SHINTH(alliance,		"Alliance     " )
GIO_WFLAGH(alliance,		"Align	  ", align_flags )
GIO_SHINTH(followers,		"Followers    " )
GIO_SHINTH(max_followers,	"MaxFollow    " )
GIO_WFLAGH(alignflags,		"Alignflags	  ", align_flags )
//GIO_WFLAGH(tendflags,		"Tendflags	  ", tendency_flags )
GIO_CUSTOM_WRITEH(race_allow_n, "RaceAllowances ", racetype_write_generic_races_set_for_n_array)
GIO_CUSTOM_READH(race_allow_n,  "RaceAllowances ", racetype_read_generic_races_set_for_n_array)
GIO_WFLAGH(sex,				"Sex          ", sex_types )
GIO_CUSTOM_WRITEH(rival,	"Rival		  ", dedit_write_rival )
GIO_CUSTOM_READH( rival,	"Rival		  ", dedit_read_rival )
GIO_CUSTOM_WRITEH(race,		"Race         ", dedit_write_race )
GIO_CUSTOM_READH( race,		"Race         ", dedit_read_race )
GIO_FINISH

deity_type *deity_list;

/**************************************************************************/
// load em up
void load_deity_db( void )
{
	DEITY_DATA* pD = NULL;
	DEITY_DATA* pRival = NULL;

	logf("===Loading deity database from %s...", DEITY_FILE );
	GIOLOAD_LIST( deity_list, DEITY_DATA, DEITY_FILE );

	// Replace temporary rivals with proper ones from the deity_list;
	for ( pD=deity_list; pD; pD=pD->next )
	{
		if ( pD->rival && pD->rival->alignflags == -1
//					   && pD->rival->tendflags  == -1 )
            )
		{
			if ( (pRival = deity_lookup(pD->rival->name) ) == NULL )
			{
				bugf("Dedit found unrecognised rival deity '%s' for '%s'", pD->rival->name, pD->name);
				free(pD->rival);
				pD->rival = NULL;
			} else {
				free(pD->rival);
				pD->rival = pRival;
			}
		}
	}

	log_string( "load_deity_db(): finished" );
}

/**************************************************************************/
// save the deity list
void save_deity_db( void )
{
	logf( "save_deity_db(): saving deity database to %s...", DEITY_FILE );
	GIOSAVE_LIST( deity_list, DEITY_DATA, DEITY_FILE, true );
}

/**************************************************************************/
// do func so it can be used as a command
void do_savedeities( char_data *ch, char * )
{
	save_deity_db();
	ch->println("Deities saved...");
	logf( "do_savedeities(): manual save of deities..." );
}

/**************************************************************************/
DEITY_DATA *deity_lookup( char *name )
{
    DEITY_DATA *pDeity;

	// first try an exact match
	for(pDeity=deity_list;pDeity;pDeity=pDeity->next){
		if(!str_cmp(name,pDeity->name)){
			return pDeity;
		}
	}

	// now try a prefix match
	for(pDeity=deity_list;pDeity;pDeity=pDeity->next){
		if(!str_prefix(name,pDeity->name)){
			return pDeity;
		}
	}

	// not found
    return NULL;
}

/**************************************************************************/
// OLC section
bool deity_create( char_data *ch, char *newName )
{
	DEITY_DATA	*node;
	static DEITY_DATA zero_node;

	node	   = new DEITY_DATA;
	*node	   = zero_node;
	node->next = deity_list;
	deity_list = node;

	deity_list->name			= str_dup( newName );
	deity_list->description		= str_dup( "" );
	deity_list->rival			= NULL;
	deity_list->essence		    = 100;
	deity_list->symbol[0]		= 0;
	deity_list->symbol[1]		= 0;
	deity_list->symbol[2]		= 0;
	deity_list->symbol[3]		= 0;
	deity_list->max_followers	= 1;
	deity_list->race			= race_lookup( "human" );
	deity_list->sex				= sex_lookup( "male" );

	ch->desc->pEdit		= (void *)deity_list;
	ch->desc->editor	= ED_DEITY;
	ch->println("Deity Created.");
	return false;
}

/**************************************************************************/
void do_dedit( char_data *ch, char *argument )
{
	DEITY_DATA	*pDeity;
	char		arg[MIL];

	if ( IS_NPC( ch ))
	{
		ch->println("Players only.");
		return;
	}

	// do security checks
	if ( !HAS_SECURITY( ch, 8 ))
	{
		ch->println("You must have an olc security 8 or higher to use this command.");
		return;
	}


	if (IS_NULLSTR(argument)){
		ch->titlebar("DEDIT: SYNTAX");
		ch->println("syntax: dedit <deity>");
		ch->println("        dedit `=Ccreate`x to make a new deity.");
		ch->println("        dedit `=Clist`x lists existing deities.");
		return;
	}

	argument = one_argument( argument, arg );

	if ( !str_cmp( arg, "list" ))
	{
		DEITY_DATA	*pD;
		int			col = 0;

		ch->titlebar("Existing Deities");

		for( pD=deity_list;pD;pD=pD->next)
		{
            ch->printf(" %-40s", pD->name );
			if ( ++col % 2 == 0 )
				ch->println("");
		}
		ch->println("`x");
		return;
	}

	if ( !str_cmp( arg, "create" ))
	{
		one_argument( argument, arg );
		deity_create( ch, arg );
		return;
	}

	pDeity = deity_lookup( arg );

    if ( !pDeity )
    {
        ch->printlnf("There is no deity named '`Y%s`x'.", arg);
        return;
    }

	ch->desc->pEdit = (void *)pDeity;
	ch->desc->editor = ED_DEITY;
	ch->printlnf("Editing properties '%s'", pDeity->name );
	return;
}
/**************************************************************************/
bool dedit_name(char_data *ch, char *argument)
{
    DEITY_DATA		*pDeity;

    EDIT_DEITY( ch, pDeity );

    if ( argument[0] == '\0' )
    {
		ch->println("Syntax:   name [name]");
		return false;
    }

    free_string( pDeity->name );
    pDeity->name = str_dup( argument );

    ch->println("Deity name set.");

    return true;
}
/**************************************************************************/
bool dedit_show(char_data *ch, char *)
{
	DEITY_DATA *pD;

    EDIT_DEITY(ch, pD);

	if(IS_NULLSTR( pD->name )) {ch->println("NULL DEITY?!?");return false;}

	ch->printlnf("`=rName:  `x%-20s   `=r", pD->name );
	ch->wraplnf("`=rDescription: `x%s",	pD->description );
	ch->printlnf("`=rRace: `x%s     `=rSex:   `x%s",
		race_table[pD->race]->name,
		pD->sex == SEX_MALE ? "male" :
		pD->sex == SEX_FEMALE ? "female" :
		pD->sex == 3 ? "random" : "neutral" );

	ch->printlnf("`=rRival Deity: `x%-20s    `=r",
		pD->rival == NULL ? "none" : pD->rival->name );
/*	ch->printlnf("`=rAlign:    `x%-5d   `=rTendency:    `x%-5d",
		pD->alliance, pD->tendency );
*/	ch->printlnf("`=rAlign:    [`x%s`=r]",
		flag_string(align_flags, pD->alignflags) );
	ch->printlnf("`=rShrine Vnum: `x%-5d",
		pD->shrinevnum);
	ch->printlnf("`=rSymbol1 Vnum: `x%-5d   `=rSymbol2 Vnum: `x%-5d",
		pD->symbol[0], pD->symbol[1] );
	ch->printlnf("`=rSymbol3 Vnum: `x%-5d   `=rSymbol4 Vnum: `x%-5d",
		pD->symbol[2], pD->symbol[3] );
	ch->printlnf("`=rAlignment Restrictions: [`x%s`=r]",
		flag_string(align_flags, pD->alignflags) );
/*	ch->printlnf("`=rTendency Restrictions : [`x%s`=r]",
		flag_string(tendency_flags, pD->tendflags) );
*/	ch->printlnf("`=rRaces allowed: [`x%s`=r]",
		race_get_races_set_for_n_array(pD->race_allow_n));

	return false;
}

/**************************************************************************/
bool dedit_shrinevnum( char_data *ch, char *argument )
{
	DEITY_DATA	*pD;

    if ( IS_NULLSTR(argument))
    {
		ch->println("Syntax: shrinevnum [vnum]");
		ch->println("        The shrinevnum denotes the room vnum where a player");
		ch->println("        is able to devote him/herself to the deity.  Should");
		ch->println("        be a remote place, so that the players have to try");
		ch->println("        and find it.");
		return false;
    }

	if ( !is_number( argument ))
	{
		ch->println("The vnum value must be numeric.");
		dedit_shrinevnum( ch, "" );		// redisplay the help
		return false;
	}

	if ( get_obj_index( atoi( argument )) == '\0' )
	{
		ch->println("That object vnum doesn't exist.");
		return false;
	}

    EDIT_DEITY( ch, pD );

	pD->shrinevnum = atoi( argument );
	ch->printf("The shrine vnum is now set to '%d'.", pD->shrinevnum );
    return true;
}

bool dedit_rival( char_data *ch, char *argument )
{
	char arg[MIL];
	DEITY_DATA *pDeity;
	DEITY_DATA *pD;
	argument = one_argument(argument, arg);

	if ( IS_NULLSTR(arg))
	{
		ch->println( "Syntax: rival [deity name]." );
		return false;
	}

	EDIT_DEITY( ch, pD );

	if ( ( strstr(arg, "none") != '\0') )
	{
		pD->rival = NULL;
		ch->println( "Rival deity is now set to 'none'") ;
		return true;
	}

	if ( !(pDeity = deity_lookup( arg )) )
	{
		ch->printlnf( "`#`Y%s`^ is not a valid deity.", arg );
		return false;
	}


	if ( pDeity == pD )
	{
		ch->println("Rival deity cannot be the deity himself.");
		return false;
	}

	pD->rival = pDeity;
	ch->printlnf( "Rival deity is now set to '`#`Y%s`^'.", pD->rival->name );
	return true;
}

/**************************************************************************/
bool dedit_symbol( char_data *ch, char *argument )
{
	char arg[MIL];
	DEITY_DATA *pD;
	argument = one_argument(argument,arg);

    if ( IS_NULLSTR(argument))
    {
		ch->println("Syntax: symbol [number 1-4] [vnum]");
		ch->println("        The symbol denotes the symbol object that will be");
		ch->println("        used for when the player has stacked up enough favour");
		ch->println("        points.  They will then be able to summon the symbol to");
		ch->println("        show that they are loyal followers.");
		return false;
    }

	if ( !is_number( argument ) || !is_number(arg) )
	{
		ch->println("Both symbol number and  vnum value must be numeric.");
		dedit_symbol( ch, "" );		// redisplay the help
		return false;
	}

	if ( atoi(arg) < 1 || atoi(arg) > 4 )
	{
		ch->println( "The valid range for symbol numbers is between `#`W1`^ and `W4`^." );
		return false;
	}

	if ( get_obj_index( atoi( argument )) == '\0' )
	{
		ch->println("That object doesn't exist, create it first then set this value.");
		return false;
	}

    EDIT_DEITY( ch, pD );

	pD->symbol[atoi(arg)-1] = atoi( argument );
	ch->printlnf ( "The symbol%d vnum is now set as '%d'.", atoi(arg), atoi(argument) );
    return true;
}
/**************************************************************************/
bool dedit_alliance (char_data *ch, char *argument)
{
    DEITY_DATA *pD;
    int value;

    if ( IS_NULLSTR( argument ))
	{
		ch->println("Syntax:  alliance [flag].");
		ch->println("  Type '? alignflag' for a list.");
		return false;
	}

	if ( (value = flag_value(align_flags, argument) ) == NO_FLAG )
	{
		ch->printlnf( "Invalid flag '%s'", argument );
		dedit_alliance( ch, "" ); // redisplay the help
		return false;
	}

    EDIT_DEITY( ch, pD );
/*
    if ( argument[0] == '\0' || !is_number( argument ) )
    {
		ch->println("Syntax:  alliance [number]");
		return false;
    }
*/
 //   value = atoi( argument );

   /* if (value>3 || value<-3)
    {
        ch->println("Alliance must be in the range -3 thru 3.");
        return false;
    } */
/*	if (value>1000 || value<-1000)
    {
        ch->println("Alliance must be in the range -1000 thru 1000.");
        return false;
    }*/

    TOGGLE_BIT( pD->alliance, value );
    //pD->alliance = value;
    ch->println("Alliance set");
    return true;
}

/**************************************************************************/
/*bool dedit_tendency(char_data *ch, char *argument)
{
    DEITY_DATA *pD;
    int value;

    EDIT_DEITY( ch, pD );

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
        ch->println("Syntax:  tendency [number]");
        return false;
    }

    value = atoi( argument );

    if (value>3 || value<-3)
    {
        ch->println("Tendency must be in the range -3 thru 3.");
        return false;
    }
    pD->tendency = value;
    ch->printlnf("Tendency set to %d.", pD->tendency);
    return true;
}*/
/**************************************************************************/
bool dedit_sex(char_data *ch, char *argument)
{
    DEITY_DATA *pD;
	EDIT_DEITY( ch, pD );
    int value;

    if ( argument[0] != '\0' )
    {

		if (( value = flag_value( sex_types, argument ) ) != NO_FLAG )
		{
			pD->sex = value;
			ch->println("Sex set.");
			return true;
		}
    }

	show_olc_options(ch, sex_types, "sex", "sex", pD->sex);
    return false;
}
/**************************************************************************/
bool dedit_race(char_data *ch, char *argument)
{
    DEITY_DATA *pD;
    int race;

    if ( argument[0] != '\0'
		&& ( race = race_lookup( argument )) >= 0 )
    {
		EDIT_DEITY( ch, pD );

		pD->race = race;
        ch->printlnf("Race set to '%s'.", race_table[race]->name);
		return true;
    }

    if ( argument[0] == '?' )
    {
		ch->print("Available races are:");

		for ( race = 1; race_table[race]; race++ )
		{
			if (( race % 3 ) == 0 )
				ch->println("");
			ch->printf(" %-15s", race_table[race]->name );
		}

		ch->println("");
		return false;
    }

    ch->printf("Syntax:  race [race]\r\n"
				 "Type 'race ?' for a list of races.\r\n" );
    return false;
}

/**************************************************************************/
bool dedit_maxfollowers(char_data *ch, char *argument)
{
    DEITY_DATA *pD;
    EDIT_DEITY( ch, pD );
    int value;

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
        ch->println("Syntax:  maxfollowers [number]");
        return false;
    }

    value = atoi( argument );

    if (value>1000 || value<0)
    {
        ch->println("Value must lie between 1 and 1000.");
        return false;
    }
    pD->max_followers = value;
    ch->printlnf("Max Followers set to %d.", pD->max_followers );
    return true;
}

/**************************************************************************/
/*bool dedit_tendflags( char_data *ch, char *argument )
{
	DEITY_DATA *pD;
	int value;

	if ( IS_NULLSTR( argument ))
	{
		ch->println("Syntax:  tendflag [flag].");
		ch->println("  Type '? tendflag' for a list.");
		return false;
	}

	value = flag_value( tendency_flags, argument );

	if ( value == NO_FLAG ) {
		ch->printlnf( "Invalid flag '%s'", argument );
		dedit_tendflags( ch, "" ); // redisplay the help
		return false;
	}

	EDIT_DEITY ( ch, pD );

	TOGGLE_BIT( pD->tendflags, value );
	ch->println( "Flag value toggled." );
	return true;
}*/
/**************************************************************************/
bool dedit_alignflags( char_data *ch, char *argument )
{
	DEITY_DATA *pD;
	int value;

	if ( IS_NULLSTR( argument ))
	{
		ch->println("Syntax:  alignflag [flag].");
		ch->println("  Type '? alignflag' for a list.");
		return false;
	}

	if ( (value = flag_value(align_flags, argument) ) == NO_FLAG )
	{
		ch->printlnf( "Invalid flag '%s'", argument );
		dedit_alignflags( ch, "" ); // redisplay the help
		return false;
	}

	EDIT_DEITY ( ch, pD );

	TOGGLE_BIT( pD->alignflags, value );
	ch->println( "Flag value toggled." );
	return true;
}

/**************************************************************************/
bool dedit_raceallow( char_data *ch, char *argument )
{
	DEITY_DATA *pD;
	EDIT_DEITY( ch, pD );
	int value;

	if ( IS_NULLSTR( argument )){
		ch->println("Syntax:  raceallow [race].");
		return false;
	}

	value = race_lookup(argument);
	if (value>=0)
	{
		TOGGLE_BITn(pD->race_allow_n, value);
		ch->printlnf( "Race '%s' toggled.", race_table[value]->name );
		return true;
	}

	ch->printlnf("Invalid race '%s'", argument );
	dedit_raceallow( ch, "" );	// redisplay the help
	return false;
}

/**************************************************************************/
void dedit_write_race(gio_type *gio_table, int tableIndex, void *data, FILE *fp)
{
	DEITY_DATA *pD;
	pD = (DEITY_DATA * ) data;
	fprintf(fp, "%s %s~\n",
			gio_table[tableIndex].heading,	race_table[pD->race]->name);
}
/**************************************************************************/
void dedit_read_race(gio_type *, int, void *data, FILE *fp)
{
	DEITY_DATA *pD;
	char *pstr;

	pD = (DEITY_DATA * ) data;

	pstr=fread_string(fp);
	pD->race=race_lookup(pstr);
	if(pD->race<0){
		bugf("Dedit found unrecognised race '%s' for '%s'", pstr, pD->name);
		pD->race=0;
	}
	free_string(pstr);
}

/**************************************************************************/
void dedit_write_rival(gio_type *gio_table, int tableIndex, void *data, FILE *fp)
{
	DEITY_DATA *pD;

	pD = (DEITY_DATA * ) data;
	if ( !pD->rival )
		fprintf(fp, "%s none~\n", gio_table[tableIndex].heading);
	else
		fprintf(fp,"%s %s~\n", gio_table[tableIndex].heading , pD->rival->name);
}

/**************************************************************************/
void dedit_read_rival(gio_type *, int, void *data, FILE *fp)
{
	DEITY_DATA *pD, *pRival;
	char *pStr;

	pD = (DEITY_DATA *) data;

	pStr = fread_string(fp);
	if ( strstr(pStr, "none") != '\0' ) {
		pD->rival = NULL;
		return;
	}

	if ( (pRival = deity_lookup(pStr)) == NULL )
	{
		pRival = new DEITY_DATA;
		pRival->name = pStr;
		pRival->alignflags = -1;
//		pRival->tendflags = -1;
		pD->rival = pRival;
	} else {
		pD->rival = pRival;
	}
	free_string(pStr);
}

/**************************************************************************/
bool dedit_description(char_data *ch, char *argument)
{
	DEITY_DATA *pD;
    EDIT_DEITY( ch, pD );

	if(!IS_NULLSTR(argument)){
		ch->println("Syntax:  description (string editor used)");
		ch->println("uses the string editor to edit/add the deity description.");
		return false;
	}
	string_append( ch, &pD->description);
	return true;
}
/**************************************************************************/
/**************************************************************************/
// save the deity for essence update
void save_essence( void )
{
	GIOSAVE_LIST( deity_list, DEITY_DATA, DEITY_FILE, true );
}

/**************************************************************************/
/**************************************************************************/
void do_shrine( char_data *ch, char *argument )
{
    char arg[MIL];

    if(IS_NPC(ch))
	return;

    if(ch->deity == NULL)
    {
	do_huh(ch, "");
	return;
    }

    argument=one_argument(argument,arg);

    DEITY_DATA	*pDeity;

	if ( (pDeity = deity_lookup(ch->deity->name)) == NULL )
	{
		ch->println( "You do not follow any deity.");
		return;
	}

    if (arg[0] == '\0')
    {
	ch->println("Syntax: Shrine <option>");
	return;
    }

    pDeity = deity_lookup(ch->deity->name);

    if (!str_prefix(arg,"build"))
    {
	OBJ_DATA *altar;
	altar = get_obj_list(ch, "altar", ch->in_room->contents);

    	if (altar != NULL)
    	{
	    ch->println( "There is already a shrine here." );
	    return;
    	}

	switch ( ch->in_room->sector_type )
	{
		case (SECT_INSIDE):
		case (SECT_WATER_SWIM):
		case (SECT_UNDERWATER):
		case (SECT_CITY):
		case (SECT_WATER_NOSWIM):
		case (SECT_SWAMP):
		case (SECT_AIR):
//		case (SECT_GATEWAY):
		case (SECT_MAX):
	    ch->println( "This is not a suitable location to construct a shrine." );
	    return;
    	}

    	if(ch->gold < 10000)
	{
		ch->println( "It costs 10000 gold to build a shrine." );
		return;
	}
	ch->gold -= 10000;
	pDeity->essence += 400;
	save_essence();

	OBJ_DATA *obj;
    	AREA_DATA *loc_area;
    	ROOM_INDEX_DATA *loc = NULL;
    	RESET_DATA *loc_reset;

	loc = ch->in_room;
	loc_area = ch->in_room->area;
	obj = create_object(get_obj_index(pDeity->shrinevnum));
	loc_reset               = new_reset_data();
	loc_reset->command      = 'O';
	loc_reset->arg1         = obj->pIndexData->vnum;
	loc_reset->arg2         = 0;
	loc_reset->arg3         = loc->vnum;
	loc_reset->arg4         = 0;
	add_reset( loc, loc_reset, 0 );
	obj_to_room (obj, ch->in_room);
	SET_BIT( loc_area->olc_flags, OLCAREA_CHANGED );
	save_area( loc_area );
	REMOVE_BIT( loc_area->olc_flags, OLCAREA_CHANGED );
	ch->printlnf("You build an altar to %s.", pDeity->name);
	WAIT_STATE( ch, 10*PULSE_VIOLENCE );
        act( "`R$n has built an altar with to thier deity.`x", ch, obj, NULL, TO_ROOM );
	return;
    }


    if (!str_prefix(arg,"desecrate"))
    {

	ch->println( "Desecrate option not finished." );
	    return;

	OBJ_DATA *obj;
	obj = get_obj_list(ch, "altar", ch->in_room->contents);



    	if (obj == NULL)
    	{
	    ch->println( "You can't find an altar to desecrate." );
	    return;
    	}

   	DEITY_DATA	*pDeity = NULL;

	if (obj->pIndexData->vnum == 7500)
	{
	    	pDeity = deity_lookup( "Harpan" );
	}else
	if (obj->pIndexData->vnum == 7510)
	{
	    	pDeity = deity_lookup( "Dorenne" );
	}else
	if (obj->pIndexData->vnum == 7522)
	{
	    	pDeity = deity_lookup( "Jivanor" );
	}else
	if (obj->pIndexData->vnum == 7530)
	{
	    	pDeity = deity_lookup( "Kreav" );
	}else
	if (obj->pIndexData->vnum == 7545)
	{
	    	pDeity = deity_lookup( "Graal" );
	}else
	if (obj->pIndexData->vnum == 7550)
	{
	    	pDeity = deity_lookup( "Venalena" );
	}else
	if (obj->pIndexData->vnum == 7560)
	{
	    	pDeity = deity_lookup( "Seandac" );
	}else
	if (obj->pIndexData->vnum == 7570)
	{
	    	pDeity = deity_lookup( "Anacor" );
	}

    	obj = get_obj_list(ch, "corpse", ch->carrying);

	if (obj == NULL)
	{
	    ch->println( "You need a corpse to desecrate the altar with." );
	    return;
	}

 	if ( obj->item_type != ITEM_CORPSE_NPC && obj->item_type != ITEM_CORPSE_PC)
   	{
	   	ch->println( "You need a real corpse to desecrate the altar with." );
 		return;
	}

	pDeity->essence -= 10;
	save_essence();

	// questing wiznet
	{
		char qbuf[MSL];
		sprintf (qbuf, "`cGODS> %s desecrates an altar to %s with %s.`x", ch->name, pDeity->name, obj->short_descr);
		wiznet(qbuf,ch,NULL,WIZ_QUESTING,0,LEVEL_IMMORTAL);
	 }

         ch->printlnf( "`RYou desecrate the altar of %s with the blood of %s.`x", pDeity->name, obj->short_descr );
         act( "`R$n desecrates the altar with $p.`x", ch, obj, NULL, TO_ROOM );
	 WAIT_STATE( ch, 10*PULSE_VIOLENCE );
	 extract_obj(obj);
	 return;
    }

    ch->println("Syntax: Shrine <option>");
    ch->println("        options: build | desecrate");
    return;

}
/***********************************************************************************************/
GIO_START( MAGNUS_DATA )
GIO_STRH(  name,			"Name         " )
GIO_INTH(  magnus_number,		"Number         ")
GIO_INTH(  level,			"Level         ")
GIO_INTH(  refine,			"Refine         ")
GIO_INTH(  refinecost,			"Cost      ")
GIO_WFLAGH(element,			"Element        ", element_flags)
//GIO_STRH(  element,			"Element         ")
GIO_STRH(  description,			"Description  " )
GIO_STRH(  short_d,			"Short ")
GIO_STRH(  long_d,			"Long  ")
GIO_FINISH

magnus_type *magnus_list;

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
// load em up
void load_magnus_db( void )
{
//	MAGNUS_DATA* pM = NULL;

	logf("===Loading magnus database from %s...", MAGNUS_FILE );
	GIOLOAD_LIST( magnus_list, MAGNUS_DATA, MAGNUS_FILE );



	log_string( "load_magnus_db(): finished" );
}

/**************************************************************************/
// save the deity list
void save_magnus_db( void )
{
	logf( "save_magnus_db(): saving magnus database to %s...", MAGNUS_FILE );
	GIOSAVE_LIST( magnus_list, MAGNUS_DATA, MAGNUS_FILE, true );
}

/**************************************************************************/
// do func so it can be used as a command
void do_savemagnus( char_data *ch, char * )
{
	save_magnus_db();
	ch->println("Magnus saved...");
	logf( "do_savemagnus(): manual save of magnus..." );
}

/**************************************************************************/
MAGNUS_DATA *magnus_lookup( char *name )
{
    MAGNUS_DATA *pMagnus;

	// first try an exact match
	for(pMagnus=magnus_list;pMagnus;pMagnus=pMagnus->next){
		if(!str_cmp(name,pMagnus->name)){
			return pMagnus;
		}
	}

	// now try a prefix match
	for(pMagnus=magnus_list;pMagnus;pMagnus=pMagnus->next){
		if(!str_prefix(name,pMagnus->name)){
			return pMagnus;
		}
	}

/*	for(pMagnus=magnus_list;pMagnus;pMagnus=pMagnus->next){
		if(name == pMagnus->magnus_number)){
			return pMagnus;
		}
	}*/

	// not found
    return NULL;
}

/**************************************************************************/
// OLC section
bool magnus_create( char_data *ch, char *newName )
{
	MAGNUS_DATA	*node;
	static MAGNUS_DATA zero_node;

	node	   = new MAGNUS_DATA;
	*node	   = zero_node;
	node->next = magnus_list;
	magnus_list = node;

	magnus_list->name			    = str_dup( newName );
	magnus_list->description		= str_dup( "" );
    magnus_list->element		    = 0;
    magnus_list->magnus_number		= 0;
    magnus_list->level		        = 0;
    magnus_list->refine		        = 0;
    magnus_list->refinecost	        = 0;
    magnus_list->short_d		    = str_dup( "" );
    magnus_list->long_d		        = str_dup( "A magnus card has been dropped here." );

	ch->desc->pEdit		= (void *)magnus_list;
	ch->desc->editor	= ED_MAGNUS;
	ch->println("Magnus Created.");
	return false;
}

/**************************************************************************/

/**************************************************************************/
void do_magnusedit( char_data *ch, char *argument )
{
	MAGNUS_DATA	*pMagnus;
	char		arg[MIL];

	if ( IS_NPC( ch ))
	{
		ch->println("Players only.");
		return;
	}

	// do security checks
	if ( !HAS_SECURITY( ch, 8 ))
	{
		ch->println("You must have an olc security 8 or higher to use this command.");
		return;
	}


	if (IS_NULLSTR(argument)){
		ch->titlebar("MAGNUSEDIT: SYNTAX");
		ch->println("syntax: magnusedit <magnus>");
		ch->println("        magnusedit `=Ccreate`x to make a new magnus.");
		ch->println("        magnusedit `=Clist`x lists existing magnus.");
		return;
	}

	argument = one_argument( argument, arg );

	if ( !str_cmp( arg, "list" ))
	{
		MAGNUS_DATA	*pM;
		int			col = 0;

		ch->titlebar("Existing Magnus");

		for( pM=magnus_list;pM;pM=pM->next)
		{
            ch->printf(" (%d) %-40s", pM->magnus_number, pM->name );
			if ( ++col % 2 == 0 )
				ch->println("");
		}
		ch->println("`x");
		return;
	}

	if ( !str_cmp( arg, "create" ))
	{
		one_argument( argument, arg );
		magnus_create( ch, arg );
		return;
	}

	pMagnus = magnus_lookup( arg );

    if ( !pMagnus )
    {
        ch->printlnf("There is no magnus named '`Y%s`x'.", arg);
        return;
    }

	ch->desc->pEdit = (void *)pMagnus;
	ch->desc->editor = ED_MAGNUS;
	ch->printlnf("Editing properties '%s'", pMagnus->name );
	return;
}
/**************************************************************************/
/**************************************************************************/
bool magnusedit_descr(char_data *ch, char *argument)
{
	MAGNUS_DATA *pM;
    EDIT_MAGNUS( ch, pM );

	if(!IS_NULLSTR(argument)){
		ch->println("Syntax:  description (string editor used)");
		ch->println("uses the string editor to edit/add the magnus description.");
		return false;
	}
	string_append( ch, &pM->description);
	return true;
}
/**************************************************************************/
bool magnusedit_name(char_data *ch, char *argument)
{
    MAGNUS_DATA		*pMagnus;

    EDIT_MAGNUS( ch, pMagnus );

    if ( argument[0] == '\0' )
    {
		ch->println("Syntax:   name [name]");
		return false;
    }
    MAGNUS_DATA *pMagnuscheck;
    pMagnuscheck = magnus_lookup( argument );
    if ( pMagnuscheck )
    {
        ch->printlnf("There is already a magnus named '`Y%s`x'.", argument);
        return false;
    }

    free_string( pMagnus->name );
    pMagnus->name = str_dup( argument );

    ch->println("Magnus name set.");

    return true;
}
/**************************************************************************/
bool magnusedit_number(char_data *ch, char *argument)
{
	MAGNUS_DATA *pM;
    EDIT_MAGNUS( ch, pM );
    int value;

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
        ch->println("Syntax:  number [number]");
        return false;
    }

    value = atoi( argument );

    if (value<0)
    {
        ch->println("Value must be at least 1 you idiot.");
        return false;
    }

	if (value < ch->in_room->area->min_vnum || value > ch->in_room->area->max_vnum){
	ch->println("Value must be within vnum range of area you are in. Try Again.");
        return false;
    }

    pM->magnus_number = value;
    ch->printlnf("Magnus number set to %d.", pM->magnus_number );
    return true;

}
/**************************************************************************/
bool magnusedit_level(char_data *ch, char *argument)
{
	MAGNUS_DATA *pM;
    EDIT_MAGNUS( ch, pM );
    int value;

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
        ch->println("Syntax:  level [number]");
        return false;
    }

    value = atoi( argument );

    if (value<0)
    {
        ch->println("Value must be at least 1 you idiot.");
        return false;
    }

	if (value > 100 && !HAS_SECURITY( ch, 9 )){
	ch->println("Value may not exceed 100 without approval.");
        return false;
    }

    pM->level = value;
    ch->printlnf("Magnus level set to %d.", pM->level );
    return true;
}
/**************************************************************************/
bool magnusedit_long(char_data *ch, char *argument)
{
	    MAGNUS_DATA *pMagnus;

    EDIT_MAGNUS(ch, pMagnus);

    // trim the spaces to the right of the short
	argument=rtrim_string(argument);

    if ( IS_NULLSTR(argument) )
    {
		ch->println( "Syntax: long <string>" );
		ch->println( "  (this is the description of the magnus seen in the room when you type look.)" );
		ch->println( "  e.g. A magnus card has been dropped here." );
		return false;
	}

	char *ptemp=pMagnus->long_d;
	pMagnus->long_d = str_dup( argument );
	pMagnus->long_d[0] = UPPER( pMagnus->long_d[0] );
	ch->printlnf( "Long description from '%s' to '%s'.",
		ptemp, pMagnus->long_d);

    free_string( ptemp );

    return true;

}
/**************************************************************************/
bool magnusedit_refine(char_data *ch, char *argument)
{
	MAGNUS_DATA *pM;
    EDIT_MAGNUS( ch, pM );
    int value;

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
        ch->println("Syntax:  refine [objvnum]");
        return false;
    }

    value = atoi( argument );

    if (value<0)
    {
        ch->println("Value must be above 1 you idiot.");
        return false;
    }


	bool objexists = false;
	if ( get_obj_index( value ) )
    {
		objexists =true;
	}

	if (objexists == false){
	ch->println("Refine object must exist already.");
	return false;}

    pM->refine = value;
    ch->printlnf("Magnus refine target set to %d.", pM->refine );
    return true;

}
/**************************************************************************/
bool magnusedit_refinecost(char_data *ch, char *argument)
{
	MAGNUS_DATA *pM;
    EDIT_MAGNUS( ch, pM );
    int value;

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
        ch->println("Syntax: refinecost [number]");
        return false;
    }

    value = atoi( argument );

    if (value<0)
    {
        ch->println("Value must be at least 1 you idiot.");
        return false;
    }

	if (value > 10 && !HAS_SECURITY( ch, 9 )){
	ch->println("Value may not exceed 10 without approval.");
        return false;
    }

    pM->refinecost = value;
    ch->printlnf("Magnus refining cost set to %d.", pM->refinecost );
    return true;
}
/**************************************************************************/
bool magnusedit_element(char_data *ch, char *argument)
{
	MAGNUS_DATA *pMagnus;
	EDIT_MAGNUS( ch, pMagnus );



	pMagnus->element = 0;
	return olc_generic_flag_toggle(ch, argument,
		"element", "element", element_flags, &pMagnus->element);

}
/**************************************************************************/
bool magnusedit_short(char_data *ch, char *argument)
{
    MAGNUS_DATA *pMagnus;

    EDIT_MAGNUS(ch, pMagnus);

    // trim the spaces to the right of the short
	argument=rtrim_string(argument);

    if ( IS_NULLSTR(argument) )
    {
        ch->println("Syntax:  short <string>" );
		ch->println("Note: <string> is forced to lowercase unless there are color codes in it.");
		return false;
    }

	if(has_color(argument)){
		ch->wraplnf( "Changed object short description from '%s' to '%s' "
			"(lower case not forced due to color codes).",
			pMagnus->short_d, argument);
		replace_string(pMagnus->short_d, argument);
	}else{
		ch->printlnf( "Changed magnus short description from '%s' to '%s'.",
			pMagnus->short_d, lowercase(argument));
		replace_string(pMagnus->short_d, lowercase(argument));
	}
    return true;
}
/**************************************************************************/
/**************************************************************************/
bool magnusedit_show(char_data *ch, char *)
{
	MAGNUS_DATA *pM;

    EDIT_MAGNUS(ch, pM);

	if(IS_NULLSTR( pM->name )) {ch->println("NULL MAGNUS?!?");return false;}

	ch->printlnf("`=rName:  `x%-20s   `=r", pM->name );
	ch->printlnf( "`=rNumber: `x%-5d", pM->magnus_number);
    ch->printlnf( "`=rLevel: `x%-5d", pM->level);
    ch->printlnf( "`=rRefine Result: `x%-5d", pM->refine);
    ch->printlnf( "`=rNumber to Refine: `x%-5d", pM->refinecost);

	ch->wraplnf("`=rDescription: `x%s",	pM->description );
	ch->printlnf( "`=rShort desc: `=x%s`x", pM->short_d);
        if (has_color(pM->short_d))
        {
		ch->print( "`sShort desc: `x" );
		ch->printlnbw(pM->short_d);
        }
    ch->printlnf( "`=rLong descr: `=x%s`x", pM->long_d);
        if (has_color(pM->long_d))
        {
		ch->print( "`sLong descr: `x" );
		ch->printlnbw(pM->long_d);
        }
    mxp_display_olc_flags(ch, element_flags,	pM->element,"element", "Element:");

	return false;
}

/**************************************************************************/

