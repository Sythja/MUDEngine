// vehicle.cpp - Vehicle movement & Combat by Ixliam, Modified by Sythja
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
//#include "vehicle.h"
#include "math.h"

DECLARE_DO_FUN( do_look		);
bool laston_find(char *argument);


bool extract_ship( SHIP_DATA *ship )
{
    ROOM_INDEX_DATA *room;

    if ( ( room = ship->in_room ) != NULL )
    {
    // UNLINK( ship, room->first_ship, room->last_ship, next_in_room, prev_in_room );
        ship->in_room = NULL;
    }
    return true;
}

/*
//UNHOLY PLANETARY MESS
extern int top_area;
extern int top_r_vnum;
void write_area_list();
void write_starsystem_list();
extern const   char *  sector_name     [SECT_MAX];

PLANET_DATA * first_planet;
PLANET_DATA * last_planet;

GUARD_DATA * first_guard;
GUARD_DATA * last_guard;
*/
/* local routines */
/*
void	fread_planet	args( ( PLANET_DATA *planet, FILE *fp ) );
bool	load_planet_file	args( ( char *planetfile ) );
void	write_planet_list	args( ( void ) );

#ifdef USECARGO
char * const cargo_names[CARGO_MAX] =
{
  "None", "Lommite","Meleenium","Neutronium","Zersium",
  "Steel", "Rhyll","Alazhi"
};
#endif

PLANET_DATA *get_planet( char *name )
{
    PLANET_DATA *planet;

    if ( name[0] == '\0' )
       return NULL;

    for ( planet = first_planet; planet; planet = planet->next )
       if ( !str_cmp( name, planet->name ) )
         return planet;*/
/*
    for ( planet = first_planet; planet; planet = planet->next )
       if ( nifty_is_name( name, planet->name ) )
         return planet;
*/
/*
    for ( planet = first_planet; planet; planet = planet->next )
       if ( !str_prefix( name, planet->name ) )
         return planet;
*/
/*
    for ( planet = first_planet; planet; planet = planet->next )
       if ( nifty_is_name_prefix( name, planet->name ) )
         return planet;
*/
/*
    return NULL;
}

void write_planet_list( )
{
    PLANET_DATA *tplanet;
    FILE *fpout;
    char filename[256];

    sprintf( filename, "%s%s", PLANETS_DIR, PLANETS_FILE );
    fpout = fopen( filename, "w" );
    if ( !fpout )
    {
	bug( "FATAL: cannot open planet.lst for writing!\n\r");
 	return;
    }
    for ( tplanet = first_planet; tplanet; tplanet = tplanet->next )
	fprintf( fpout, "%s\n", tplanet->filename );
    fprintf( fpout, "$\n" );
    fclose( fpout );
}

void save_planet( PLANET_DATA *planet )
{
    FILE *fp;
    char filename[256];
    char buf[MSL];*//*
#ifdef USECARGO
    int i = 0;
#endif*/
/*
    if ( !planet )
    {
	bug( "save_planet: null planet pointer!");
	return;
    }

    if ( !planet->filename || planet->filename[0] == '\0' )
    {
	sprintf( buf, "save_planet: %s has no filename", planet->name );
	bug( buf);
	return;
    }

    sprintf( filename, "%s%s", PLANETS_DIR, planet->filename );

    fclose( fpReserve );
    if ( ( fp = fopen( filename, "w" ) ) == NULL )
    {
    	bug( "save_planet: fopen");
    	perror( filename );
    }
    else
    {
        AREA_DATA *pArea;

	fprintf( fp, "#PLANET\n" );
	fprintf( fp, "Name         %s~\n",	planet->name		);
	fprintf( fp, "Filename     %s~\n",	planet->filename        );
	fprintf( fp, "X            %d\n",	planet->x               );
	fprintf( fp, "Y            %d\n",	planet->y               );
	fprintf( fp, "Z            %d\n",	planet->z               );
	fprintf( fp, "Sector       %d\n",	planet->sector          );
	fprintf( fp, "Type    	   %d\n",	planet->controls );
	fprintf( fp, "PopSupport   %d\n",	(int) (planet->pop_support)      );
	if ( planet->starsystem && planet->starsystem->name )
        	fprintf( fp, "Starsystem   %s~\n",	planet->starsystem->name);
//	if ( planet->governed_by && planet->governed_by->name )
//        	fprintf( fp, "GovernedBy   %s~\n",	planet->governed_by->name);
	//pArea = planet->area;
	//if (pArea->filename)
         //	fprintf( fp, "Area         %s~\n",	pArea->filename  );
        for ( pArea = planet->first_area; pArea; pArea = pArea->next_on_planet )
          fprintf( fp, "Area         %s~\n",	pArea->file_name  );
*//*
    for(i = 1; i<CARGO_MAX; i++)
     {
       fprintf(fp, "Resource %d %d %d %d %d %d\n", i, planet->import[i], planet->export[i], planet->resource[i], planet->consumes[i], planet->produces[i]);
     }
*/
/*
	fprintf( fp, "End\n\n"						);
	fprintf( fp, "#END\n"						);
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = true;			\
				    break;				\
				}


void fread_planet( PLANET_DATA *planet, FILE *fp )
{
    char buf[MSL];
    char *word;
    char *line;
    int x0,x1,x2,x3,x4,x5;
    bool fMatch;

    for ( ; ; )
    {
	word   = feof( fp ) ? (char*)"End" : fread_word( fp );
	fMatch = false;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = true;
	    fread_to_eol( fp );
	    break;

	case 'A':
*//*	    if ( !str_cmp( word, "Area" ) )
	    {
	        char aName[MAX_STRING_LENGTH];
                AREA_DATA *pArea;

	     	sprintf (aName, fread_string(fp));
		for( pArea = first_area ; pArea ; pArea = pArea->next )
	          if (pArea->filename && !str_cmp(pArea->filename , aName ) )
	          {
	             ROOM_INDEX_DATA *room;

	             planet->size = 0;
	             planet->citysize = 0;
	             planet->wilderness = 0;
	             planet->farmland = 0;
	             planet->barracks = 0;
	             planet->controls = 0;
	             pArea->planet = planet;
	             planet->area = pArea;
	             for( room = pArea->first_room ; room ; room = room->next_in_area )
                     {
                       	  planet->size++;
                          if ( room->sector_type <= SECT_CITY )
                             planet->citysize++;
                          else if ( room->sector_type == SECT_FARMLAND )
                             planet->farmland++;
                          else if ( room->sector_type != SECT_DUNNO )
                             planet->wilderness++;

                          if ( IS_SET( room->room_flags , ROOM_CONTROL ))
                             planet->controls++;
                          if ( IS_SET( room->room_flags , ROOM_BARRACKS ))
                             planet->barracks++;
                     }
	          }      */
                //fMatch = true;
	    //}
/*
	    if ( !str_cmp( word, "area" ) )
	    {
	     	planet->area = area_lookup ( fread_string(fp) );
                if (planet->area)
                {
                     AREA_DATA *area = planet->area;
                     area->planet = planet;
                     LINK( area , planet->first_area, planet->last_area, next_on_planet, prev_on_planet );
                }
                fMatch = true;
	    }

	    break;

	case 'E':
	    if ( !str_cmp( word, "End" ) )
	    {
		if (!planet->name)
		  planet->name		= str_dup( "" );
		return;
	    }
	    break;

	case 'F':
	    KEY( "Filename",	planet->filename,		fread_string( fp ) );
	    break;

*//*	case 'G':
	    if ( !str_cmp( word, "GovernedBy" ) )
	    {
	     	planet->governed_by = get_clan ( fread_string(fp) );
                fMatch = true;
	    }
	    break;
*/
/*
	case 'N':
	    KEY( "Name",	planet->name,		fread_string( fp ) );
	    break;

	case 'P':
	    KEY( "PopSupport",	planet->pop_support,		fread_number( fp ) );
	    break;

*//*      case 'R':
          if( !str_cmp( word, "Resource" ) );
          {
             line = fread_line(fp);
             x0=x1=x2=x3=x4=x5=0;
             sscanf(line, "%d %d %d %d %d %d\n",
                    &x0, &x1, &x2, &x3, &x4, &x5);
             planet->import[x0] = x1;
             planet->export[x0] = x2;
             planet->resource[x0] = x3;
             planet->consumes[x0] = x4;
             planet->produces[x0] = x5;
          }
          break;
*/
/*
	case 'S':
	    KEY( "Sector",	planet->sector,		fread_number( fp ) );
*//*	    if ( !str_cmp( word, "Starsystem" ) )
	    {
	     	planet->starsystem = starsystem_from_name ( fread_string(fp) );
                if (planet->starsystem)
                {
                     SPACE_DATA *starsystem = planet->starsystem;

                     LINK( planet , starsystem->first_planet, starsystem->last_planet, next_in_system, prev_in_system );
                }
                fMatch = true;
	    }*/
    /*
	    break;
	case 'T':
	    KEY("Type", planet->controls,	fread_number(fp) );
		break;
	case 'X':
	    KEY( "X",	planet->x,		fread_number( fp ) );
	    break;

	case 'Y':
	    KEY( "Y",	planet->y,		fread_number( fp ) );
	    break;

	case 'Z':
	    KEY( "Z",	planet->z,		fread_number( fp ) );
	    break;

	}

	if ( !fMatch )
	{
	    sprintf( buf, "Fread_planet: no match: %s", word );
	    bug( buf);
	}

    }
}

bool load_planet_file( char *planetfile )
{
    char filename[256];
    PLANET_DATA *planet;
    FILE *fp;
    bool found;

    CREATE( planet, PLANET_DATA, 1 );

//    planet->governed_by = NULL;
    planet->next_in_system = NULL;
    planet->prev_in_system = NULL;
    planet->starsystem = NULL ;
    planet->area = NULL;
    planet->first_guard = NULL;
    planet->last_guard = NULL;

    found = false;
    sprintf( filename, "%s%s", PLANETS_DIR, planetfile );

    if ( ( fp = fopen( filename, "r" ) ) != NULL )
    {

	found = true;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_planet_file: # not found." );
		break;
	    }

	    word = fread_word( fp );
	    if ( !str_cmp( word, "PLANET"	) )
	    {
	    	fread_planet( planet, fp );
	    	break;
	    }
	    else
	    if ( !str_cmp( word, "END"	) )
	        break;
	    else
	    {
		char buf[MSL];

		sprintf( buf, "Load_planet_file: bad section: %s.", word );
		bug( buf );
		break;
	    }
	}
	fclose( fp );
    }

    if ( !found )
      DISPOSE( planet );
    else
      LINK( planet, first_planet, last_planet, next, prev );

    return found;
}

void load_planets( )
{
    FILE *fpList;
    char *filename;
    char planetlist[256];
    char buf[MAX_STRING_LENGTH];

    first_planet	= NULL;
    last_planet	= NULL;

    log_string( "Loading planets..." );

    sprintf( planetlist, "%s%s", PLANET_DIR, PLANET_LIST );
    fclose( fpReserve );
    if ( ( fpList = fopen( planetlist, "r" ) ) == NULL )
    {
	perror( planetlist );
	exit( 1 );
    }

    for ( ; ; )
    {
	filename = feof( fpList ) ? "$" : fread_word( fpList );
	log_string( filename );
	if ( filename[0] == '$' )
	  break;

	if ( !load_planet_file( filename ) )
	{
	  sprintf( buf, "Cannot load planet file: %s", filename );
	  bug( buf, 0 );
	}
    }
    fclose( fpList );
    log_string(" Done planets " );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

AREA_DATA *get_area(char *argument){
  AREA_DATA *pArea;
    for( pArea = first_area ; pArea ; pArea = pArea->next )
      if (pArea->file_name && !str_cmp(pArea->file_name , argument ) )
        return pArea;

   return NULL;
}

void do_setplanet( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    PLANET_DATA *planet;
#ifdef USECARGO
    int i=0;
    char arg3[MAX_INPUT_LENGTH];
#endif

    if ( IS_NPC( ch ) )
    {
	ch->println( "Huh?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	ch->println( "Usage: setplanet <planet> <field> [value]\n\r", ch );
	ch->println( "\n\rField being one of:\n\r", ch );
	ch->println( " name filename area starsystem governed_by x y z\n\r", ch );
#ifdef USECARGO
        ch->println( " import  export   resource   produces  consumes\n\r", ch );
#endif
	return;
    }

    planet = get_planet( arg1 );
    if ( !planet )
    {
	ch->println( "No such planet.\n\r", ch );
	return;
    }


    if ( !strcmp( arg2, "name" ) )
    {
	STRFREE( planet->name );
	planet->name = STRALLOC( argument );
	ch->println( "Done.\n\r", ch );
	save_planet( planet );
	return;
    }
    if( !strcmp(arg2, "type") )
    {
 	if(!argument)  planet->controls = 0;
	else 		planet->controls = atoi(argument);
	ch->println("Done.\n\r", ch);
	save_planet(planet);
	return;
    }
    if ( !strcmp( arg2, "sector" ) )
    {
	planet->sector = atoi(argument);
	ch->println( "Done.\n\r", ch );
	save_planet( planet );
	return;
    }

    if ( !strcmp( arg2, "area")){
    	planet->area = get_area ( argument );
        if (planet->area)
        {
           AREA_DATA *area = planet->area;
           LINK( area , planet->first_area, planet->last_area, next_on_planet, prev_on_planet );
         }
         ch->println( "Done.\n\r", ch );
       	 save_planet( planet );
       	 return;
    }

    if ( !strcmp( arg2, "governed_by" ) )
    {
        CLAN_DATA *clan;
        clan = get_clan( argument );
        if ( clan )
        {
           planet->governed_by = clan;
           ch->println( "Done.\n\r", ch );
       	   save_planet( planet );
        }
        else
           ch->println( "No such clan.\n\r", ch );
	return;
    }

    if ( !strcmp( arg2, "starsystem" ) )
    {
        SPACE_DATA *starsystem;

        if ((starsystem=planet->starsystem) != NULL)
          UNLINK(planet, starsystem->first_planet, starsystem->last_planet, next_in_system, prev_in_system);
	if ( (planet->starsystem = starsystem_from_name(argument)) )
        {
           starsystem = planet->starsystem;
           LINK(planet, starsystem->first_planet, starsystem->last_planet, next_in_system, prev_in_system);
           ch->println( "Done.\n\r", ch );
	}
	else
	       	ch->println( "No such starsystem.\n\r", ch );
	save_planet( planet );
	return;
    }

    if ( !strcmp( arg2, "x" ) )
    {
        planet->x = atoi(argument);
	ch->println( "Done.\n\r", ch );
	save_planet( planet );
	write_planet_list( );
	return;

    }

    if ( !strcmp( arg2, "y" ) )
    {
        planet->y = atoi(argument);
	ch->println( "Done.\n\r", ch );
	save_planet( planet );
	write_planet_list( );
	return;

    }

    if ( !strcmp( arg2, "z" ) )
    {
        planet->z = atoi(argument);
	ch->println( "Done.\n\r", ch );
	save_planet( planet );
	write_planet_list( );
	return;

    }
#ifdef USECARGO
if ( !strcmp( arg2, "import"))
    {
    argument = one_argument( argument, arg3 );

       for (i = 0; i < CARGO_MAX; i++)
       {
          if (!str_cmp( arg3, cargo_names[i]))
          {
             planet->import[i] = atoi(argument);
             planet->export[i] = 0;
             ch->println("done.\n\r", ch );
             save_planet( planet );
             return;
          }
       }
       ch->println("No such resource type\r\n", ch);
       return;
    }

    if ( !strcmp( arg2, "export"))
    {
    argument = one_argument( argument, arg3 );

       for (i = 0; i < CARGO_MAX; i++)
       {
          if (!str_cmp( arg3, cargo_names[i]))
          {
             planet->export[i] = atoi(argument);
             planet->import[i] = 0;
             ch->println("done.\n\r", ch );
             save_planet( planet );
             return;
          }
       }
       ch->println("No such resource type\r\n", ch);
       return;
    }

    if ( !strcmp( arg2, "resource"))
    {
    argument = one_argument( argument, arg3 );

       for (i = 0; i < CARGO_MAX; i++)
       {
          if (!str_cmp( arg3, cargo_names[i]))
          {
             planet->resource[i] = atoi(argument);
             ch->println("done.\n\r", ch );
             save_planet( planet );
             return;
          }
       }
       ch->println("No such resource type\r\n", ch);
       return;
    }

    if ( !strcmp( arg2, "produces"))
    {
    argument = one_argument( argument, arg3 );

       for (i = 0; i < CARGO_MAX; i++)
       {
          if (!str_cmp( arg3, cargo_names[i]))
          {
             planet->produces[i] = atoi(argument);
             ch->println("done.\n\r", ch );
             save_planet( planet );
             return;
          }
       }
       ch->println("No such resource type\r\n", ch);
       return;
    }

    if ( !strcmp( arg2, "consumes"))
    {
    argument = one_argument( argument, arg3 );

       for (i = 0; i < CARGO_MAX; i++)
       {
          if (!str_cmp( arg3, cargo_names[i]))
          {
             planet->consumes[i] = atoi(argument);
             ch->println("done.\n\r", ch );
             save_planet( planet );
             return;
          }
       }
       ch->println("No such resource type\r\n", ch);
       return;
    }
#endif

    if ( !strcmp( arg2, "filename" ) )
    {
	DISPOSE( planet->filename );
	planet->filename = str_dup( argument );
	ch->println( "Done.\n\r", ch );
	save_planet( planet );
	write_planet_list( );
	return;
    }


    do_setplanet( ch, "" );
    return;
}

void do_showplanet( CHAR_DATA *ch, char *argument )
{
    GUARD_DATA * guard;
    PLANET_DATA *planet;
    AREA_DATA *pArea;
    char area[MAX_STRING_LENGTH];
    int num_guards = 0;
    int pf = 0;
    int pc = 0;
    int pw = 0;

    if ( IS_NPC( ch ) )
    {
	ch->println( "Huh?\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	ch->println( "Usage: showplanet <planet>\n\r", ch );
	return;
    }

    planet = get_planet( argument );
    if ( !planet )
    {
	ch->println( "No such planet.\n\r", ch );
	return;
    }

    for ( guard = planet->first_guard ; guard ; guard = guard->next_on_planet )
        num_guards++;

    if ( planet->size > 0 )
    {
       float tempf;

       tempf = planet->citysize;
       pc = tempf / planet->size *  100;

       tempf = planet->wilderness;
       pw = tempf / planet->size *  100;

       tempf = planet->farmland;
       pf = tempf / planet->size *  100;
    }

    ch_printf( ch, "&W%s\n\r", planet->name);
    if ( IS_IMMORTAL(ch) )
          ch_printf( ch, "&WFilename: &G%s\n\r", planet->filename);
    ch_printf( ch, "&WStarsystem: &G%s\n\r", (planet->starsystem && planet->starsystem->name) ? planet->starsystem->name : "(null)");
    ch_printf( ch, "&WTerrain: &G%s\n\r",
                   sector_name[planet->sector]  );
    ch_printf( ch, "&WGoverned by: &G%s\n\r",
                   planet->governed_by ? planet->governed_by->name : "" );
    ch_printf( ch, "&WPlanet Size: &G%d\n\r",
                   planet->size );
    ch_printf( ch, "&WPercent Civilized: &G%d\n\r", pc ) ;
    ch_printf( ch, "&WPercent Wilderness: &G%d\n\r", pw ) ;
    ch_printf( ch, "&WPercent Farmland: &G%d\n\r", pf ) ;
    ch_printf( ch, "&WBarracks: &G%d\n\r", planet->barracks );
    ch_printf( ch, "&WPatrols: &G%d&W/%d\n\r", num_guards , planet->barracks*5 );
    ch_printf( ch, "&WPopulation: &G%d&W\n\r", planet->population );
    ch_printf( ch, "&WPopular Support: &G%.2f\n\r",
                   planet->pop_support );
    ch_printf( ch, "&WCurrent Monthly Revenue: &G%ld\n\r",
                   get_taxes( planet) );
    area[0] = '\0';
    for ( pArea = planet->first_area; pArea; pArea = pArea->next_on_planet ){
      strcat(area, pArea->filename);
      strcat(area, ", ");
    }
    ch_printf( ch, "&WAreas: &G%s\n\r", area);
    if ( IS_IMMORTAL(ch) && !planet->area )
    {
          ch_printf( ch, "&RWarning - this planet is not attached to an area!&G");
          ch_printf( ch, "\n\r" );
    }

    return;
}


void do_makeplanet( CHAR_DATA *ch, char *argument )
{
    char filename[256];
    PLANET_DATA *planet;
    bool found;

    if ( !argument || argument[0] == '\0' )
    {
	ch->println( "Usage: makeplanet <planet name>\n\r", ch );
	return;
    }

    found = false;
    sprintf( filename, "%s%s", PLANET_DIR, strlower(argument) );

    CREATE( planet, PLANET_DATA, 1 );
    LINK( planet, first_planet, last_planet, next, prev );
    planet->governed_by = NULL;
    planet->next_in_system = NULL;
    planet->prev_in_system = NULL;
    planet->starsystem = NULL ;
    planet->first_area = NULL;
    planet->last_area = NULL;
    planet->first_guard = NULL;
    planet->last_guard = NULL;
    planet->name		= STRALLOC( argument );
    planet->flags               = 0;
}


void do_planets( CHAR_DATA *ch, char *argument )
{
    PLANET_DATA *planet;
    int count = 0;
    SPACE_DATA *starsystem;

    //set_char_color( AT_WHITE, ch );
    ch->println( "Planet             Starsystem    Governed By                  Popular Support\n\r" , ch );

    for ( starsystem = first_starsystem ; starsystem; starsystem = starsystem->next )
     for ( planet = starsystem->first_planet; planet; planet = planet->next_in_system )
     {
	if(planet->controls != 0) continue;
        ch_printf( ch, "&G%-18s %-12s  %-25s    ",
                   planet->name , starsystem->name ,
                   planet->governed_by ? planet->governed_by->name : "" );
        ch_printf( ch, "%.1f\n\r", planet->pop_support );
        if ( IS_IMMORTAL(ch) && !planet->area )
        {
          ch_printf( ch, "&RWarning - this planet is not attached to an area!&G");
          ch_printf( ch, "\n\r" );
        }

        count++;
     }

    for ( planet = first_planet; planet; planet = planet->next )
    {
        if ( planet->starsystem )
           continue;

        ch_printf( ch, "&G%-15s %-12s  %-25s    ",
                   planet->name , "",
                   planet->governed_by ? planet->governed_by->name : "" );
        ch_printf( ch, "%.1f\n\r",
			!str_cmp(planet->governed_by->name, "Neutral") ? 100.0 : planet->pop_support );
        if ( IS_IMMORTAL(ch) && !planet->area )
        {
          ch_printf( ch, "&RWarning - this planet is not attached to an area!&G");
          ch_printf( ch, "\n\r" );
        }

        count++;
    }

    if ( !count )
    {
	//set_char_color( AT_BLOOD, ch);
        ch->println( "There are no planets currently formed.\n\r", ch );
    }
    ch->println( "&WUse SHOWPLANET for more information.\n\r", ch );

}

void do_capture ( CHAR_DATA *ch , char *argument )
{
   CLAN_DATA *clan;
   PLANET_DATA *planet;
   PLANET_DATA *cPlanet;
   float support = 0.0;
   int pCount = 0;
   char buf[MAX_STRING_LENGTH];

   if ( !ch->in_room || !ch->in_room->area)
      return;

   if ( IS_NPC(ch) || !ch->pcdata )
   {
       ch->println ( "huh?\n\r" , ch );
       return;
   }

   if ( !ch->pcdata->clan )
   {
       ch->println ( "You need to be a member of an organization to do that!\n\r" , ch );
       return;
   }

   clan = ch->pcdata->clan;

   if ( ( planet = ch->in_room->area->planet ) == NULL )
   {
       ch->println ( "You must be on a planet to capture it.\n\r" , ch );
       return;
   }

   if ( clan == planet->governed_by )
   {
       ch->println ( "Your organization already controls this planet.\n\r" , ch );
       return;
   }

   if ( clan->clan_type == CLAN_CRIME || clan->clan_type == CLAN_GUILD )
   {
        ch->println( "Your clan can't capture planets!\n\r", ch);
        return;
   }

   if ( planet->starsystem )
   {
       SHIP_DATA *ship;
       CLAN_DATA *sClan;

       for ( ship = planet->starsystem->first_ship ; ship ; ship = ship->next_in_starsystem )
       {
          sClan = get_clan(ship->owner);
          if ( !sClan )
             continue;
          if ( sClan == planet->governed_by )
          {
             ch->println ( "A planet cannot be captured while protected by orbiting spacecraft.\n\r" , ch );
             return;
          }
       }
   }

   if ( planet->first_guard )
   {
       ch->println ( "This planet is protected by soldiers.\n\r" , ch );
       ch->println ( "You will have to eliminate all enemy forces before you can capture it.\n\r" , ch );
       return;
   }
  if(planet->governed_by->name && planet->governed_by->name != NULL)
	{
   if(!str_cmp(planet->governed_by->name, "Neutral"))
	{
	   ch->println("This planet cannot be captured.\n\r", ch);
	   return;
	}
	}
   if ( planet->pop_support > 0 )
   {
       ch->println ( "The population is not in favour of changing leaders right now.\n\r" , ch );
       return;
   }

   for ( cPlanet = first_planet ; cPlanet ; cPlanet = cPlanet->next )
        if ( clan == cPlanet->governed_by )
        {
            pCount++;
            support += cPlanet->pop_support;
        }

   if ( support < 0 )
   {
       ch->println ( "There is not enough popular support for your organization!\n\rTry improving loyalty on the planets that you already control.\n\r" , ch );
       return;
   }

   planet->governed_by = clan;
   planet->pop_support = 50;

   sprintf( buf , "%s has been captured by %s!", planet->name, clan->name );
   echo_to_all( AT_RED , buf , 0 );

   save_planet( planet );

   return;
}

long get_taxes( PLANET_DATA *planet )
{
      long gain;

      gain = planet->base_value;
      gain += planet->base_value*(planet->pop_support/100);
      gain += (planet->population * 150);
      gain += UMAX(0, planet->pop_support/10 * (planet->population * 20));

      return gain;
}*/
/*
#ifdef USECARGO
void do_imports( CHAR_DATA *ch, char *argument )
{
   PLANET_DATA *planet;
   int i;

   if (argument[0] == '\0')
   {
      ch->println("Usage: imports <planet>\r\n", ch);
      return;
   }

   planet = get_planet( argument );

   if (!planet)
   {
      ch->println("&RNo such planet\r\n", ch);
      return;
   }
   ch->printlnf( "&BImport and Export data for %s:\r\n", planet->name);
   ch->printlnf( "&GResource    &CImport     &YExport    &PProduces    &RConsumes         &GAmount\r\n");
   ch->printlnf(  "&G----------   ------     ------    --------    --------          ------\r\n");
   for (i = 1; i < CARGO_MAX; i++)
   ch->printlnf( "&G%-10.10s    &C%5d/ton  &Y%5d/ton &P%6d tons  &R%6d tons  &G%9d\r\n",
             cargo_names[i], planet->import[i], planet->export[i],
             planet->produces[i], planet->consumes[i], planet->resource[i]);
   return;
}
#endif
*/

/********************************************************************************/
/********************************************************************************/
//UNHOLY SHIP MESS
/*
BMARKET_DATA *first_market_ship;
BMARKET_DATA *last_market_ship;

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = true;			\
				    break;				\
				}


typedef enum
{
  SP_NOT_SET,SP_COCKPIT,SP_ENTRANCE,SP_HANGER1, SP_HANGER2, SP_HANGER3, SP_HANGER4,SP_ENGINEROOM,
  SP_PILOT,SP_COPILOT,SP_NAVIGATOR,SP_GUNNER,SP_TURRET1,SP_TURRET2,SP_TURRET3,SP_TURRET4,SP_TURRET5,
  SP_TURRET6,SP_TURRET7,SP_TURRET8,SP_TURRET9,SP_TURRET10,SP_OTHER
} ship_proto_room_types;

struct ship_prototypes_struct
{
    char       *name;
    char       *sname;
    char       *clan;
    int         num_rooms;
    int         cost;
    int         class;
    int		tractor;
    int		primaryType;
    int     secondaryType;
	int		primaryCount;
	int		secondaryCount;
    int		range_weapons[3];
    int		hull;
    int		shields;
    int		energy;
    int		chaff;
    int		maxbombs;
    int		speed;
    int		hyperspeed;
    int		manuever;
    int		turrets;
    int         maxcargo;
    int		mods;
    //int         plasma;
};

struct ship_prototypes_struct ship_prototypes[256];
int NUM_PROTOTYPES;


typedef struct prototype_room PROTO_ROOM;

struct prototype_room
{
    int                what_prototype;
    PROTO_ROOM         *next;
    PROTO_ROOM         *prev;
    char               *name;
    char               *desc;
    char               *flags;
    int                tunnel;
    int                exits[10];
    int                exflags[10];
    int                keys[10];
    int                room_num;
    int                room_type;
    char               *rprog[10];
    char               *reset[10];
};

PROTO_ROOM *first_prototype_room;
PROTO_ROOM *last_prototype_room;

void instaroom( AREA_DATA *pArea, ROOM_INDEX_DATA *pRoom, bool dodoors );
void shiplist(CHAR_DATA *ch);
char *primary_beam_name_proto(int shiptype);
char *secondary_beam_name_proto(int shiptype);

void do_buymobship(CHAR_DATA *ch, char *argument )
{
  int x,size,ship_type,vnum,count,caps;
  SHIP_DATA *ship;
  SHIP_DATA *sship;
  char arg[MAX_STRING_LENGTH];
  char shipname[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  bool found_proto = false;
  AREA_DATA *tarea;
  CLAN_DATA   *clan;
  CLAN_DATA   *mainclan;
  SPACE_DATA *system;
  PLANET_DATA *planet;
  bool fsys, fplan, fcap;

   argument = one_argument(argument,arg);

   if(!arg || !argument)
	{
	   ch->println("Usage: buymobship <ship type> <starsystem to be sent to>\n\r", ch);
	   return;
	}
   if ( IS_NPC(ch) || !ch->pcdata )
   {
   	ch->println( "&ROnly players can do that!\r\n" ,ch );
   	return;
   }
   if ( !ch->pcdata->clan )
   {
   	ch->println( "You do not belong to any organization.\r\n" ,ch );
   	return;
   }
   clan = ch->pcdata->clan;
   mainclan = ch->pcdata->clan->mainclan ? ch->pcdata->clan->mainclan : clan;

   if ( ( ch->pcdata->bestowments
    &&    is_name("clanbuyship", ch->pcdata->bestowments))
    ||   !str_cmp( ch->name, clan->leader  ))
	;
   else
   {
   	ch->println( "Your organization hasn't seen fit to bestow you with that ability.\r\n" ,ch );
   	return;
   }

    if(arg == NULL || !arg || arg[0] == '\0')
    {
        ch->println("\n\r&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w                               Available ships                               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);

        for(x=0;x<NUM_PROTOTYPES;x++)
		{
		  if(!ch->pcdata->clan && (!str_cmp(ship_prototypes[x].clan, "The Empire")
								|| !str_cmp(ship_prototypes[x].clan, "The New Republic"))) continue;
		  if(ch->pcdata->clan && str_cmp(ship_prototypes[x].clan, ch->pcdata->clan->name) && str_cmp(ship_prototypes[x].clan, "")) continue;
		  if(ch->pcdata->clan && str_cmp(ship_prototypes[x].clan, ch->pcdata->clan->name)) continue;
		  if(ship_prototypes[x].class!= 0 && ship_prototypes[x].class!= 1 && ship_prototypes[x].class!= 2) continue;

            ch->printlnf( "&W|&w %s%-39.39s&W Type:  &w%-13.13s &WCost:&w %8d &W|\r\n",
            !str_cmp(ship_prototypes[x].clan, "The Empire") ? "&R" :
            !str_cmp(ship_prototypes[x].clan, "The New Republic") ? "&G" : "&w",
            ship_prototypes[x].name,
            ship_prototypes[x].class == 0 ? "Fighter" :
            ship_prototypes[x].class == 1 ? "Bomber" :
            ship_prototypes[x].class == 2 ? "Midtarget" : "Spacecraft",
            ship_prototypes[x].cost);
        }

        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w               Refer to 'shiplist' for a complete ship listing               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\n\r",ch);
      return;
    }

    size = NUM_PROTOTYPES;
    for(x=0;x<size;x++)
    {
    	if(nifty_is_name_prefix(arg,ship_prototypes[x].name)) {
            found_proto = true;
    	  break;
    	}
    }
    if(!found_proto)
    {
        ch->println("\n\r&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w                               Available ships                               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);

        for(x=0;x<NUM_PROTOTYPES;x++)
		{
		  if(!ch->pcdata->clan && (!str_cmp(ship_prototypes[x].clan, "The Empire")
								|| !str_cmp(ship_prototypes[x].clan, "The New Republic"))) continue;
		  if(ch->pcdata->clan && str_cmp(ship_prototypes[x].clan, ch->pcdata->clan->name) && str_cmp(ship_prototypes[x].clan, "")) continue;
		  if(ch->pcdata->clan && str_cmp(ship_prototypes[x].clan, ch->pcdata->clan->name)) continue;
		  if(ship_prototypes[x].class!= 0 && ship_prototypes[x].class!= 1 && ship_prototypes[x].class!= 2) continue;

            ch->printlnf( "&W|&w %s%-39.39s&W Type:  &w%-13.13s &WCost:&w %8d &W|\r\n",
            !str_cmp(ship_prototypes[x].clan, "The Empire") ? "&R" :
            !str_cmp(ship_prototypes[x].clan, "The New Republic") ? "&G" : "&w",
            ship_prototypes[x].name,
            ship_prototypes[x].class == 0 ? "Fighter" :
            ship_prototypes[x].class == 1 ? "Bomber" :
			ship_prototypes[x].class == 2 ? "Midtarget" : "Spacecraft",
            ship_prototypes[x].cost);
        }
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w               Refer to 'shiplist' for a complete ship listing               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\n\r",ch);
      return;
    }
    ship_type = x;

    // Modify later. -||

    if( (clan->clan_type == 0) &&
	 str_cmp(ship_prototypes[ship_type].clan, ch->pcdata->clan->name))
    {
    	ch->println("Your organization may only purchase its own ship models.\n\r", ch);
    	return;
    }
	if(ship_prototypes[x].class!= 0 && ship_prototypes[x].class!= 1 && ship_prototypes[x].class!= 2)
	{
		ch->println("You may only purchase fighters, bombers, or midtargets for mobile ships.\n\r", ch);
		return;
	}
    vnum = find_vnum_block(ship_prototypes[ship_type].num_rooms);
    if(vnum == -1)
    {
    	ch->println("There was a problem with your ship: free vnums. Notify an administrator.\r\n",ch);
    	bug("Ship area is low on vnums.",0);
      return;
    }

    switch(ship_type)
	{
		default: sprintf(shipname, "Mobile Ship MS"); break;
		// NR
		case 0: sprintf(shipname, "X-Wing Snubfighter MXW"); break;
		case 1: sprintf(shipname, "A-Wing Scout MAW"); break;
		case 2: sprintf(shipname, "B-Wing Heavy Fighter MBW"); break;
		case 3: sprintf(shipname, "Y-Wing Bomber MYB"); break;
		case 4: sprintf(shipname, "K-Wing Heavy Bomber MKW"); break;
		// Imp
		case 6: sprintf(shipname, "TIE Fighter MTF"); break;
		case 7: sprintf(shipname, "TIE Bomber MTB"); break;
		case 8: sprintf(shipname, "TIE Defender MTD"); break;
		case 9: sprintf(shipname, "XM-1 Missileboat MXM"); break;
		case 10: sprintf(shipname, "XG-1 Assault Gunboat MXG"); break;
    }

    sprintf(shipname,"%s%d (%s)",shipname,number_range(1111, 9999),ship_prototypes[ship_type].sname);

    if(ch->pcdata->clan->funds < ship_prototypes[ship_type].cost)
    {
        ch->println("Your organization cannot pay for that ship.\r\n",ch);
      return;
    }
   clan = ch->pcdata->clan;
   mainclan = ch->pcdata->clan->mainclan ? ch->pcdata->clan->mainclan : clan;

   if ( ( ch->pcdata->bestowments
    &&    is_name("clanbuyship", ch->pcdata->bestowments))
    ||   !str_cmp( ch->name, clan->leader  ))
	;
   else
   {
   	ch->println( "Your organization hasn't seen fit to bestow you with that ability.\r\n" ,ch );
   	return;
   }

   fsys = fplan = fcap = false;
   for(system = first_starsystem; system; system = system->next)
	{
	   if(nifty_is_name(argument, system->name))
		{
		   fsys = true;
		   break;
		}
	}
   if(!fsys)
	{
	   ch->println("No such starsystem.\n\r", ch);
	   return;
	}
   for(planet = system->first_planet; planet; planet = planet->next_in_system)
	{
	   if(ch->pcdata->clan == planet->governed_by)
		   fplan = true;
	}
   if(!fplan)
	{
	   ch->println("Your organization does not control any planets in that starsystem.\n\r", ch);
	   return;
	}
   count = 0;
   for(sship = system->first_ship; sship; sship = sship->next_in_starsystem)
	{
	   if(sship->type == MOB_SHIP) count++;
	   if(sship->ship_class >= SHIP_CRUISER) { fcap = true; caps++; }
	}
   if(!fcap)
	{
	   ch->println("There isn't a capital class ship in that starsystem.\n\r", ch);
	   return;
	}
	   if(count > 3*caps)
		{
		   ch->println("You can only have 3 mobile ships per capital-class ship in a system.\n\r", ch);
		   return;
		}
    for ( tarea = first_area; tarea; tarea = tarea->next )
      if ( !str_cmp( SHIP_AREA, tarea->filename ) )
        break;
    if(make_prototype_rooms(ship_type,vnum,tarea,shipname) == -1)
    {
    	ch->println("There was a problem with your ship: unable to create a room. Notify an administrator.\r\n",ch);
    	bug("Ship area unable to make_room.",0);
      return;
    }

    ch->pcdata->clan->funds -= ship_prototypes[ship_type].cost*1.3;

	ch->printlnf(  "It costs %d to build the ship and %d to train a pilot.\n\r",
		ship_prototypes[ship_type].cost, ship_prototypes[ship_type].cost/3);
	ch->printlnf(  "%s is quickly dispatched to the %s system.\n\r", shipname, system->name);


    ship = make_prototype_ship(ship_type,vnum,ch,shipname);
    ship->owner = STRALLOC(ch->pcdata->clan->name);
    save_ship( ship );
    write_ship_list( );
	extract_ship(ship);
	ship_to_starsystem(ship, system);
	ship->location = 0;
	ship->inship = NULL;
	ship->type = MOB_SHIP;
	if(ship->home) STRFREE(ship->home);
	ship->home = STRALLOC(system->name);
	ship->hx = ship->hy = ship->hz = 1;
	ship->vx = number_range(-3000, 3000);
	ship->vy = number_range(-3000, 3000);
	ship->vz = number_range(-3000, 3000);
	ship->autopilot = true;
	sprintf(buf, "%s enters the starsystem at %.0f %.0f %.0f", ship->name, ship->vx, ship->vy, ship->vz);
    echo_to_system(AT_YELLOW, ship, buf, NULL);
  return;
}

void do_orderclanship(CHAR_DATA *ch, char *argument )
{
  int x,size,ship_type,vnum;
  SHIP_DATA *ship;
  char arg[MAX_STRING_LENGTH];
  bool found_proto = false;
  AREA_DATA *tarea;
  CLAN_DATA   *clan;
  CLAN_DATA   *mainclan;

   if ( IS_NPC(ch) || !ch->pcdata )
   {
   	ch->println( "&ROnly players can do that!\r\n" ,ch );
   	return;
   }
   if ( !ch->pcdata->clan )
   {
   	ch->println( "You do not belong to any organization.\r\n" ,ch );
   	return;
   }

   clan = ch->pcdata->clan;
   mainclan = ch->pcdata->clan->mainclan ? ch->pcdata->clan->mainclan : clan;

   if ( ( ch->pcdata->bestowments
    &&    is_name("clanbuyship", ch->pcdata->bestowments))
    ||   !str_cmp( ch->name, clan->leader  ))
	;
   else
   {
   	ch->println( "Your organization hasn't seen fit to bestow you with that ability.\r\n" ,ch );
   	return;
   }
    if( !IS_SET(ch->in_room->room_flags2,ROOM_SHIPYARD) )
    {
    	ch->println("You can only purchase a ship at a shipyard.\r\n",ch);
      return;
    }
    if(argument == NULL || !argument || argument[0] == '\0')
    {
        ch->println("\n\r&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w                               Available ships                               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);

        for(x=0;x<NUM_PROTOTYPES;x++)
		{
		  if(!ch->pcdata->clan && (!str_cmp(ship_prototypes[x].clan, "The Empire")
								|| !str_cmp(ship_prototypes[x].clan, "The New Republic"))) continue;
		  if(ch->pcdata->clan && str_cmp(ship_prototypes[x].clan, ch->pcdata->clan->name) && str_cmp(ship_prototypes[x].clan, "")) continue;
		  if(ch->pcdata->clan && str_cmp(ship_prototypes[x].clan, ch->pcdata->clan->name)) continue;

            ch->printlnf( "&W|&w %s%-39.39s&W Type:  &w%-13.13s &WCost:&w %8d &W|\r\n",
            !str_cmp(ship_prototypes[x].clan, "The Empire") ? "&R" :
            !str_cmp(ship_prototypes[x].clan, "The New Republic") ? "&G" : "&w",
            ship_prototypes[x].name,
		        ship_prototypes[x].class == SHIP_FIGHTER ? "Starfighter" :
		        ship_prototypes[x].class == SHIP_BOMBER  ? "Bomber" :
		        ship_prototypes[x].class == SHIP_SHUTTLE ? "Shuttle" :
		        ship_prototypes[x].class == SHIP_FREIGHTER ? "Freighter" :
		        ship_prototypes[x].class == SHIP_FRIGATE ? "Frigate" :
		        ship_prototypes[x].class == SHIP_TT ? "Troop Transport" :
		        ship_prototypes[x].class == SHIP_CORVETTE ? "Corvette" :
		        ship_prototypes[x].class == SHIP_CRUISER ? "Cruiser" :
			    ship_prototypes[x].class == SHIP_DREADNAUGHT ? "Dreadnaught" :
		        ship_prototypes[x].class == SHIP_DESTROYER ? "Star Destroyer" :
		        ship_prototypes[x].class == SHIP_SPACE_STATION ? "Space Station" :
		        ship_prototypes[x].class == LAND_VEHICLE ? "Land vehicle" : "Unknown",

            ship_prototypes[x].cost);
        }

        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w               Refer to 'shiplist' for a complete ship listing               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\n\r",ch);
      return;
    }
    argument = one_argument(argument,arg);
    size = NUM_PROTOTYPES;
    for(x=0;x<size;x++)
    {
    	if(nifty_is_name_prefix(arg,ship_prototypes[x].name)) {
            found_proto = true;
    	  break;
    	}
    }
    if(!found_proto)
    {
        ch->println("\n\r&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w                               Available ships                               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);

        for(x=0;x<NUM_PROTOTYPES;x++)
		{
		  if(!ch->pcdata->clan && (!str_cmp(ship_prototypes[x].clan, "The Empire")
								|| !str_cmp(ship_prototypes[x].clan, "The New Republic"))) continue;
		  if(ch->pcdata->clan && str_cmp(ship_prototypes[x].clan, ch->pcdata->clan->name) && str_cmp(ship_prototypes[x].clan, "")) continue;
		  if(ch->pcdata->clan && str_cmp(ship_prototypes[x].clan, ch->pcdata->clan->name)) continue;

            ch->printlnf( "&W|&w %s%-39.39s&W Type:  &w%-13.13s &WCost:&w %8d &W|\r\n",
            !str_cmp(ship_prototypes[x].clan, "The Empire") ? "&R" :
            !str_cmp(ship_prototypes[x].clan, "The New Republic") ? "&G" : "&w",
            ship_prototypes[x].name,
		        ship_prototypes[x].class == SHIP_FIGHTER ? "Starfighter" :
		        ship_prototypes[x].class == SHIP_BOMBER  ? "Bomber" :
		        ship_prototypes[x].class == SHIP_SHUTTLE ? "Shuttle" :
		        ship_prototypes[x].class == SHIP_FREIGHTER ? "Freighter" :
		        ship_prototypes[x].class == SHIP_FRIGATE ? "Frigate" :
		        ship_prototypes[x].class == SHIP_TT ? "Troop Transport" :
		        ship_prototypes[x].class == SHIP_CORVETTE ? "Corvette" :
		        ship_prototypes[x].class == SHIP_CRUISER ? "Cruiser" :
			    ship_prototypes[x].class == SHIP_DREADNAUGHT ? "Dreadnaught" :
		        ship_prototypes[x].class == SHIP_DESTROYER ? "Star Destroyer" :
		        ship_prototypes[x].class == SHIP_SPACE_STATION ? "Space Station" :
		        ship_prototypes[x].class == LAND_VEHICLE ? "Land vehicle" : "Unknown",

            ship_prototypes[x].cost);
        }
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w               Refer to 'shiplist' for a complete ship listing               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\n\r",ch);
      return;
    }
    ship_type = x;
    if(argument[0] == '\0' || argument == NULL || !argument)
    {
        ch->println("What would you like to name your ship?\r\n",ch);
      return;
    }
    // Modify later. -||

    if( (clan->clan_type == 0) &&
	 str_cmp(ship_prototypes[ship_type].clan, ch->pcdata->clan->name))
    {
    	ch->println("Your organization may only purchase its own ship models.\n\r", ch);
    	return;
    }
    vnum = find_vnum_block(ship_prototypes[ship_type].num_rooms);
    if(vnum == -1)
    {
    	ch->println("There was a problem with your ship: free vnums. Notify an administrator.\r\n",ch);
    	bug("Ship area is low on vnums.",0);
      return;
    }
    argument[0] = UPPER(argument[0]);
    sprintf(argument,"%s (%s)",argument,ship_prototypes[ship_type].sname);
    for ( ship = first_ship; ship; ship = ship->next )
    {
        if(!str_cmp(ship->name,argument))
        {
        ch->println("That ship name is already in use. Choose another.\r\n",ch);
          return;
        }
    }
    if(ch->pcdata->clan->funds < ship_prototypes[ship_type].cost)
    {
        ch->println("Your organization cannot pay for that ship.\r\n",ch);
      return;
    }
    for ( tarea = first_area; tarea; tarea = tarea->next )
      if ( !str_cmp( SHIP_AREA, tarea->filename ) )
        break;
    if(make_prototype_rooms(ship_type,vnum,tarea,argument) == -1)
    {
    	ch->println("There was a problem with your ship: unable to create a room. Notify an administrator.\r\n",ch);
    	bug("Ship area unable to make_room.",0);
      return;
    }

    ch->pcdata->clan->funds -= ship_prototypes[ship_type].cost;

    ch->println("&wA shipyard salesman enters some information into a datapad.\r\n",ch);
    ch->println("&R&CThe salesman says:&R&W You're all set. Thanks.\n\r", ch);
    ship = make_prototype_ship(ship_type,vnum,ch,argument);
    ship->owner = ch->pcdata->clan->name;
    save_ship( ship );
    write_ship_list( );
  return;
}

void do_ordership(CHAR_DATA *ch, char *argument)
{
  int x,size,ship_type,vnum,count;
  SHIP_DATA *ship;
  char arg[MAX_STRING_LENGTH];
  bool found_proto = false;
  AREA_DATA *tarea;
  BMARKET_DATA *marketship;
  char *bmshipname;
  int bmshipcost;

  count=0;
    if( !IS_SET(ch->in_room->room_flags2,ROOM_SHIPYARD) && !IS_SET(ch->in_room->room_flags2, ROOM_BLACKMARKET))
    {
    	ch->println("You can only purchase ships at a shipyard.\r\n",ch);
      return;
    }
   if(!IS_SET(ch->in_room->room_flags2, ROOM_BLACKMARKET))
   {
    if(argument == NULL || !argument || argument[0] == '\0')
    {
        ch->println("\n\r&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w                               Available ships                               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);

        for(x=0;x<NUM_PROTOTYPES;x++)
		{
		  if(!ch->pcdata->clan && (!str_cmp(ship_prototypes[x].clan, "The Empire")
								|| !str_cmp(ship_prototypes[x].clan, "The New Republic"))) continue;
		  if(ch->pcdata->clan && str_cmp(ship_prototypes[x].clan, ch->pcdata->clan->name) && str_cmp(ship_prototypes[x].clan, "")) continue;

            ch->printlnf( "&W|&w %s%-39.39s&W Type:  &w%-13.13s &WCost:&w %8d &W|\r\n",
            !str_cmp(ship_prototypes[x].clan, "The Empire") ? "&R" :
            !str_cmp(ship_prototypes[x].clan, "The New Republic") ? "&G" : "&w",
            ship_prototypes[x].name,
		        ship_prototypes[x].class == SHIP_FIGHTER ? "Starfighter" :
		        ship_prototypes[x].class == SHIP_BOMBER  ? "Bomber" :
		        ship_prototypes[x].class == SHIP_SHUTTLE ? "Shuttle" :
		        ship_prototypes[x].class == SHIP_FREIGHTER ? "Freighter" :
		        ship_prototypes[x].class == SHIP_FRIGATE ? "Frigate" :
		        ship_prototypes[x].class == SHIP_TT ? "Troop Transport" :
		        ship_prototypes[x].class == SHIP_CORVETTE ? "Corvette" :
		        ship_prototypes[x].class == SHIP_CRUISER ? "Cruiser" :
			    ship_prototypes[x].class == SHIP_DREADNAUGHT ? "Dreadnaught" :
		        ship_prototypes[x].class == SHIP_DESTROYER ? "Star Destroyer" :
		        ship_prototypes[x].class == SHIP_SPACE_STATION ? "Space Station" :
		        ship_prototypes[x].class == LAND_VEHICLE ? "Land vehicle" : "Unknown",

            ship_prototypes[x].cost);

        }
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w               Refer to 'shiplist' for a complete ship listing               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\n\r",ch);
      return;
    }
   }
   else
   {
    if(argument == NULL || !argument || argument[0] == '\0')
    {
    ch->printlnf( "&z+&W------------------------&z The Black Market &W--------------------------&z+\n\r");
   	for(marketship = first_market_ship; marketship; marketship = marketship->next)
   	{
         for(x=0;x<NUM_PROTOTYPES;x++)
   	 {
   	  if(!str_cmp(marketship->filename, ship_prototypes[x].sname))
   	  {
   	   bmshipname = ship_prototypes[x].name;
   	   bmshipcost = ship_prototypes[x].cost*3;
	   break;
   	  }
   	 }

   	 ch->printlnf( "&W|&z %-35.35s&W |&z Quantity: %d&W |&z Cost: %-8d&W |\n\r", bmshipname,marketship->quantity,bmshipcost);
   	 count++;
   	}
   if(count==0)
    ch->printlnf( "&W|&z                         No ships available.                        &W|\n\r");
    ch->printlnf( "&z+&W--------------------------------------------------------------------&z+\n\r");
    return;
    }
   }

    argument = one_argument(argument, arg);
    size = NUM_PROTOTYPES;
    for(x=0;x<size;x++)
    {
       if(!IS_SET(ch->in_room->room_flags2, ROOM_BLACKMARKET))
       {
    	if(nifty_is_name_prefix(arg,ship_prototypes[x].name))
    	{
            found_proto = true;
    	    break;
        }
       }
       else
       {
       	for(marketship = first_market_ship; marketship; marketship = marketship->next)
       	{
       	 if(nifty_is_name_prefix(arg, ship_prototypes[x].name) && !str_cmp(ship_prototypes[x].sname, marketship->filename))
	 {
       	   found_proto = 1;
	   break;
	 }
        }
     if(found_proto == 1)
       break;
       }
    }

    if(!found_proto)
    {
     if(!IS_SET(ch->in_room->room_flags2, ROOM_BLACKMARKET))
     {
        ch->println("\n\r&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w                               Available ships                               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);

        for(x=0;x<NUM_PROTOTYPES;x++)
		{
		  if(!ch->pcdata->clan && (!str_cmp(ship_prototypes[x].clan, "The Empire")
								|| !str_cmp(ship_prototypes[x].clan, "The New Republic"))) continue;
		  if(ch->pcdata->clan && str_cmp(ship_prototypes[x].clan, ch->pcdata->clan->name) && str_cmp(ship_prototypes[x].clan, "")) continue;

            ch->printlnf( "&W|&w %s%-39.39s&W Type:  &w%-13.13s &WCost:&w %8d &W|\r\n",
            !str_cmp(ship_prototypes[x].clan, "The Empire") ? "&R" :
            !str_cmp(ship_prototypes[x].clan, "The New Republic") ? "&G" : "&w",
            ship_prototypes[x].name,
		        ship_prototypes[x].class == SHIP_FIGHTER ? "Starfighter" :
		        ship_prototypes[x].class == SHIP_BOMBER  ? "Bomber" :
		        ship_prototypes[x].class == SHIP_SHUTTLE ? "Shuttle" :
		        ship_prototypes[x].class == SHIP_FREIGHTER ? "Freighter" :
		        ship_prototypes[x].class == SHIP_FRIGATE ? "Frigate" :
		        ship_prototypes[x].class == SHIP_TT ? "Troop Transport" :
		        ship_prototypes[x].class == SHIP_CORVETTE ? "Corvette" :
		        ship_prototypes[x].class == SHIP_CRUISER ? "Cruiser" :
			    ship_prototypes[x].class == SHIP_DREADNAUGHT ? "Dreadnaught" :
		        ship_prototypes[x].class == SHIP_DESTROYER ? "Star Destroyer" :
		        ship_prototypes[x].class == SHIP_SPACE_STATION ? "Space Station" :
		        ship_prototypes[x].class == LAND_VEHICLE ? "Land vehicle" : "Unknown",

            ship_prototypes[x].cost);

        }
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w               Refer to 'shiplist' for a complete ship listing               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\n\r",ch);
      return;
     }
     else
     {
      ch->printlnf( "&z+&W------------------------&z The Black Market &W--------------------------&z+\n\r");
        for(marketship = first_market_ship; marketship; marketship = marketship->next)
    	{
          for(x=0;x<NUM_PROTOTYPES;x++)
   	  {
   	    if(!str_cmp(marketship->filename, ship_prototypes[x].sname))
   	    {
   	      bmshipname = ship_prototypes[x].name;
   	      if(marketship->quantity == 1)
   	       bmshipcost = ship_prototypes[x].cost*2.5;
   	      else if(marketship->quantity == 2)
   	       bmshipcost = ship_prototypes[x].cost*2;
   	      else
   	       bmshipcost = ship_prototypes[x].cost*1.5;
   	    }
   	  }
   	   ch->printlnf( "&W|&z %-35.35s&W |&z Quantity: %d&W |&z Cost: %-8d&W |\n\r", bmshipname,marketship->quantity,bmshipcost);
   	   count++;
   	}
   if(count==0)
    ch->printlnf( "&W|&z                         No ships available.                        &W|\n\r");
      ch->printlnf( "&z+&W--------------------------------------------------------------------&z+\n\r");
      return;
      }
   }

    ship_type = x;
    if(argument[0] == '\0' || argument == NULL || !argument)
    {
        ch->println("What do you want to name your ship?\r\n",ch);
      return;
    }
    if(ship_prototypes[ship_type].class == 3 && !IS_IMMORTAL(ch))
    {
    	ch->println("At the moment, capital ships may only be purchased by clans.\n\r", ch);
    	return;
    }

  if(!IS_SET(ch->in_room->room_flags2, ROOM_BLACKMARKET))
  {
   if(!str_cmp(ship_prototypes[ship_type].clan, "The Empire") ||
      !str_cmp(ship_prototypes[ship_type].clan,"The New Republic"))
   {
     if(!ch->pcdata->clan)
     {
      ch->println("You must find a black market to purchase clanned ships.\n\r", ch);
      return;
     }
     else if(str_cmp(ch->pcdata->clan->name, ship_prototypes[ship_type].clan))
     {
      ch->println("You must find a black market to purchase other clan's ships.\n\r", ch);
      return;
     }
   }
  }

    vnum = find_vnum_block(ship_prototypes[ship_type].num_rooms);
    if(vnum == -1)
    {
    	ch->println("There was a problem with your ship: free vnums. Notify an administrator.\r\n",ch);
    	bug("Ship Shop area is low on vnums.",0);
      return;
    }

    argument[0] = UPPER(argument[0]);

    sprintf(argument,"%s (%s)",argument, ship_prototypes[ship_type].sname);
    for ( ship = first_ship; ship; ship = ship->next )
    {
        if(!str_cmp(ship->name,argument))
        {
        ch->println("&CThat ship name is already in use. Choose another.\r\n",ch);
          return;
        }
    }
   if(!IS_SET(ch->in_room->room_flags2, ROOM_BLACKMARKET))
   {
    if(ch->gold < ship_prototypes[ship_type].cost)
    {
        ch->println("You can't afford that ship.\r\n",ch);
      return;
    }
   }
   else
   {
    if(ch->gold < ship_prototypes[ship_type].cost*3)
    {
    	ch->println("You can't afford that ship.\n\r", ch);
    	return;
    }
   }

	if(!str_cmp(argument, "&"))
    {
    	ch->println("No color codes in ship names.\n\r", ch);
    	return;
    }

	for ( tarea = first_area; tarea; tarea = tarea->next )
      if ( !str_cmp( SHIP_AREA, tarea->filename ) )
        break;
    if(make_prototype_rooms(ship_type,vnum,tarea,argument) == -1)
    {
    	ch->println("There was a problem with your ship: unable to create a room. Notify an administrator.\n\r",ch);
    	bug("Ship Shop unable to make_room.",0);
      return;
    }

   if(!IS_SET(ch->in_room->room_flags2, ROOM_BLACKMARKET))
   {
    ch->gold -= ship_prototypes[ship_type].cost;
    ch->println("A shipyard salesman enters some information into a datapad.\r\n",ch);
    ch->println("&R&CA salesman says:&R&W You're all set. Thanks.\n\r", ch);
   }
   else
   {
    ch->gold -= ship_prototypes[ship_type].cost*3;

for(marketship = first_market_ship; marketship; marketship = marketship->next)
{
  if(!str_cmp(ship_prototypes[ship_type].sname, marketship->filename)) {
    remove_market_ship(marketship);
    break;
  }
}

    ch->println("A smuggler gives a grin and shows you to the ship.\n\r", ch);
    ch->println("&R&CA smuggler says:&R&W There ya go, she's all yours.\n\r", ch);
   }
    ship = make_prototype_ship(ship_type,vnum,ch,argument);
  return;
}

SHIP_DATA *make_prototype_ship(int ship_type,int vnum,CHAR_DATA *ch,char *ship_name)
{
  SHIP_DATA *ship;
  PROTO_ROOM *proom;
  ROOM_INDEX_DATA *room;
  char sp_filename[MAX_STRING_LENGTH];
    CREATE( ship, SHIP_DATA, 1 );
    LINK( ship, first_ship, last_ship, next, prev );
    ship->name		= STRALLOC( ship_name );
    ship->owner		= STRALLOC( "" );
    ship->protoname     = STRALLOC( ship_prototypes[ship_type].sname );

    if( !ship_prototypes[ship_type].clan || ( ship_prototypes[ship_type].clan[0] == '\0' ) )
       ship->clanowner = STRALLOC( "" );
    else
       ship->clanowner = STRALLOC( ship_prototypes[ship_type].clan );

    ship->description	= STRALLOC( "" );
    ship->copilot       = STRALLOC( "" );
    ship->pilot         = STRALLOC( "" );

*/    /* Added in check to make sure all things cruiser+ get put in
       the right starsystem by Keberus, thanks Guido
     */
/*
    if(ch->in_room->area && ch->in_room->area->planet)
    {
        if( ship_prototypes[ship_type].class >= SHIP_CRUISER &&  ch->in_room->area->planet->starsystem )
           ship->home = STRALLOC( ch->in_room->area->planet->starsystem->name );
        else
           ship->home = STRALLOC( ch->in_room->area->planet->name );
    }
    else
       ship->home = STRALLOC( "" );

    ship->pbeacon       = STRALLOC( "" );
    ship->type          = PLAYER_SHIP;
    ship->ship_class         = ship_prototypes[ship_type].class;
    ship->cost		= ship_prototypes[ship_type].cost;
    ship->starsystem    = NULL;
    ship->in_room       = NULL;
    ship->next_in_room  = NULL;
    ship->prev_in_room  = NULL;
    ship->currjump      = NULL;
    ship->target0       = NULL;
    ship->target1       = NULL;
    ship->target2       = NULL;
    ship->target3       = NULL;
    ship->target4       = NULL;
    ship->target5       = NULL;
    ship->target6       = NULL;
    ship->target7       = NULL;
    ship->target8       = NULL;
    ship->target9       = NULL;
    ship->target10      = NULL;
    ship->password      = number_range(1111,9999);;
    ship->maxmods	= ship_prototypes[ship_type].mods;
    sprintf(sp_filename,"ship_%d.sss",vnum);
    ship->filename      = str_dup( sp_filename );
	ship->pilotseat = vnum;
    	ship->coseat = vnum;
    	ship->navseat = vnum;
    	ship->gunseat = vnum;
    	ship->entrance = vnum;
    	ship->engineroom = vnum;
    for(proom = first_prototype_room;proom;proom = proom->next)
    {
        if(proom->what_prototype == ship_type)
        {
            if(proom->room_num == 1)
                ship->firstroom = vnum;
            if(proom->room_num == ship_prototypes[ship_type].num_rooms)
                ship->lastroom = proom->room_num+vnum-1;
        }
    }
    for(proom = first_prototype_room;proom;proom = proom->next)
    {
        if(proom->what_prototype == ship_type)
        {
            room = get_room_index(proom->room_num+vnum-1);
            switch(proom->room_type)
            {
              case SP_COCKPIT     : ship->cockpit    = proom->room_num+vnum-1; break;
              case SP_ENTRANCE    : ship->entrance   = proom->room_num+vnum-1; break;
              case SP_HANGER1     : ship->hanger1    = proom->room_num+vnum-1; break;
              case SP_HANGER2     : ship->hanger2    = proom->room_num+vnum-1; break;
              case SP_HANGER3     : ship->hanger3    = proom->room_num+vnum-1; break;
              case SP_HANGER4     : ship->hanger4    = proom->room_num+vnum-1; break;
              case SP_ENGINEROOM  : ship->engineroom = proom->room_num+vnum-1; break;
              case SP_PILOT       : ship->pilotseat  = proom->room_num+vnum-1; break;
              case SP_COPILOT     : ship->coseat     = proom->room_num+vnum-1; break;
              case SP_NAVIGATOR   : ship->navseat    = proom->room_num+vnum-1; break;
              case SP_GUNNER      : ship->gunseat    = proom->room_num+vnum-1; break;
              case SP_TURRET1     : ship->turret1    = proom->room_num+vnum-1; break;
              case SP_TURRET2     : ship->turret2    = proom->room_num+vnum-1; break;
              case SP_TURRET3     : ship->turret3    = proom->room_num+vnum-1; break;
              case SP_TURRET4     : ship->turret4    = proom->room_num+vnum-1; break;
              case SP_TURRET5     : ship->turret5    = proom->room_num+vnum-1; break;
              case SP_TURRET6     : ship->turret6    = proom->room_num+vnum-1; break;
              case SP_TURRET7     : ship->turret7    = proom->room_num+vnum-1; break;
              case SP_TURRET8     : ship->turret8    = proom->room_num+vnum-1; break;
              case SP_TURRET9     : ship->turret9    = proom->room_num+vnum-1; break;
              case SP_TURRET10    : ship->turret10   = proom->room_num+vnum-1; break;
              default             : break;
            }
        }
    }
    ship->tractorbeam = ship_prototypes[ship_type].tractor;
    ship->primaryType = ship_prototypes[ship_type].primaryType;
    ship->secondaryType = ship_prototypes[ship_type].secondaryType;
	ship->primaryCount = ship_prototypes[ship_type].primaryCount;
	ship->secondaryCount = ship_prototypes[ship_type].secondaryCount;
    ship->missiles = ship_prototypes[ship_type].range_weapons[0];
    ship->torpedos = ship_prototypes[ship_type].range_weapons[1];
    ship->rockets = ship_prototypes[ship_type].range_weapons[2];
    ship->maxmissiles = ship_prototypes[ship_type].range_weapons[0];
    ship->maxtorpedos = ship_prototypes[ship_type].range_weapons[1];
    ship->maxrockets = ship_prototypes[ship_type].range_weapons[2];
    ship->maxhull = ship_prototypes[ship_type].hull;
    ship->hull = ship_prototypes[ship_type].hull;
    ship->shield = ship_prototypes[ship_type].shields;
    ship->maxshield = ship_prototypes[ship_type].shields;
    //ship->plasmashield = ship_prototypes[ship_type].plasma;
    //ship->maxplasmashield = ship_prototypes[ship_type].plasma;
    ship->energy = ship_prototypes[ship_type].energy;
    ship->maxenergy = ship_prototypes[ship_type].energy;
    ship->chaff = ship_prototypes[ship_type].chaff;
    ship->maxchaff = ship_prototypes[ship_type].chaff;
    ship->bombs = ship_prototypes[ship_type].maxbombs;
    ship->maxbombs = ship_prototypes[ship_type].maxbombs;
    ship->realspeed = ship_prototypes[ship_type].speed;
    ship->hyperspeed = ship_prototypes[ship_type].hyperspeed;
#ifdef USECARGO
    ship->maxcargo = ship_prototypes[ship_type].maxcargo;
#endif
    ship->manuever = ship_prototypes[ship_type].manuever;
  //MARKER
        ship->shipyard = ch->in_room->vnum;
    resetship( ship );
    ship->owner 	= STRALLOC( ch->name );
    save_ship( ship );
    write_ship_list( );
  return ship;
}

int make_prototype_rooms(int ship_type, int vnum,AREA_DATA *tarea,char *Sname)
{
  PROTO_ROOM *proom;
  ROOM_INDEX_DATA *newroom,*rfrom,*rto;
  int value;
  char  *arg;
  char  arg2[MAX_STRING_LENGTH];
  EXIT_DATA *xit;
  int x,y;
  int *exits;
  int *keys;
  int *exflags;
  char buf[MAX_STRING_LENGTH];
  char newdesc[MAX_STRING_LENGTH];
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  CHAR_DATA *victim;
  MOB_INDEX_DATA *pMobIndex;
  int rvnum;
  char *rarg;
  char rtype[MAX_STRING_LENGTH];

    for(proom = first_prototype_room;proom;proom = proom->next)
    {
        if(proom->what_prototype == ship_type)
        {
            if( (newroom = get_room_index(vnum+proom->room_num-1)) == NULL)
            {
                newroom = make_room(vnum+proom->room_num-1, tarea);
                if(!newroom)
                  return -1;
            }
            newroom->area = tarea;
            if(proom->room_type == SP_NOT_SET)
            {
                if(newroom->name)
                    STRFREE(newroom->name);
                sprintf(buf,"SPARE ROOM FOR: %d",vnum);
                newroom->name = STRALLOC( buf );
                if(newroom->description)
                    STRFREE(newroom->description);
                newroom->description = STRALLOC( "" );
              continue;
            }
	    strcpy(newdesc,strlinwrp(proom->desc,60));
            if(newroom->name) STRFREE(newroom->name);
            newroom->name = STRALLOC( strrep(proom->name,"$SN$",Sname) );
            if(newroom->description) STRFREE(newroom->description);
            newroom->description = STRALLOC( strrep(newdesc,"$SN$",Sname) );
            arg = STRALLOC( proom->flags );
            newroom->tunnel = proom->tunnel;
            while ( arg[0] != '\0' )
	    {
	        arg = one_argument( arg, arg2 );
	        value = get_sp_rflag( arg2 );
  	        TOGGLE_BIT( newroom->room_flags, 1 << value );
	    }
	    for(y=0;y<10;y++)
	    {
	    	if(proom->reset[y] != NULL && proom->reset[y][0] != '\0')
	    	{
	    	    rarg = STRALLOC(proom->reset[y]);
	    	    rarg = one_argument(rarg,rtype);
	    	    rvnum = atoi(rarg);
	    	    switch(UPPER(rtype[0]))
	    	    {
	    	    	case 'O':
     	    	            if( ( pObjIndex = get_obj_index( rvnum ) ) == NULL)
     	    	              break;
     	    	            obj = create_object( pObjIndex, 60 );
     	    	            obj = obj_to_room( obj, newroom );
     	    	          break;
     	    	        case 'M':
     	    	            if( ( pMobIndex = get_mob_index( rvnum ) ) == NULL)
     	    	              break;
     	    	            victim = create_mobile( pMobIndex );
         	    	    char_to_room( victim, newroom );
         	    	  break;
                        default:
                          break;
         	    }
         	}
            }
            instaroom(tarea,newroom,true);
        }
    }
    for(proom = first_prototype_room;proom;proom = proom->next)
    {
        if(proom->what_prototype == ship_type)
        {
            exits = proom->exits;
            keys = proom->keys;
            exflags = proom->exflags;
            for(x=0;x<10;x++)
            {
                if(exits[x] > 0)
                {
                    rfrom = get_room_index(vnum+proom->room_num-1);
                    rto = get_room_index(vnum+exits[x]-1);
                    xit = make_exit( rfrom, rto, x );
                    xit->keyword                = STRALLOC( "" );
                    xit->description    = STRALLOC( "" );
                    xit->key            = keys[x] == 0 ? -1 : vnum+keys[x]-1;
                    xit->exit_info              = exflags[x];
                }
            }
        }
    }
    make_rprogs(ship_type, vnum);
    fold_area( tarea, tarea->filename, false );
  return 1;
}



int find_vnum_block(int num_needed)
{
  bool counting = false;
  int count = 0;
  AREA_DATA *tarea;
  int lrange;
  int trange;
  int vnum;
  int startvnum = -1;
  ROOM_INDEX_DATA *room;

    for ( tarea = first_area; tarea; tarea = tarea->next )
        if ( !str_cmp( SHIP_AREA, tarea->filename ) )
	  break;
    lrange = tarea->low_r_vnum;
    trange = tarea->hi_r_vnum;
    for ( vnum = lrange; vnum <= trange; vnum++ )
    {
//        if ( (room = get_room_index( vnum )) == NULL || !strcmp(room->name,"VNUM_AVAILABLE\0"))
          if ( (room = get_room_index( vnum )) == NULL )
          {
            if(!counting)
            {
                counting = true;
                startvnum = vnum;
            }
            count++;
            if(count == num_needed+1)
              break;
        }
        else if(counting)
        {
            counting = false;
            count = 0;
            startvnum = -1;
        }
    }
  return startvnum;
}

int get_sp_rflag( char *flag )
{
    int x;

    for ( x = 0; x < 32; x++ )
      if ( !str_cmp( flag, r_flags[x] ) )
        return x;
    return -1;
}

void make_rprogs(int ship_type, int vnum)
{
  PROTO_ROOM *proom;
  char argument[MAX_STRING_LENGTH];
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  MPROG_DATA *mprog, *mprg;
  ROOM_INDEX_DATA *room;
  int mptype, x, size;
    for(proom = first_prototype_room;proom;proom = proom->next)
    {
        if(proom->what_prototype == ship_type)
        {
            size = 10;
            for(x=0;x<size;x++)
            {
                strcpy(argument, proom->rprog[x]);
                if(argument[0] == '\0')
                    continue;
                strcpy(argument, one_argument( argument, arg1 ) );
                strcpy(argument, one_argument( argument, arg2 ) );
                sprintf(argument,"%s",parse_prog_string(argument,ship_type,vnum));
                room = get_room_index(proom->room_num+vnum-1);
                mprog = room->mudprogs;
	        mptype = get_mpflag( arg1 );
	        if ( mprog )
	          for ( ; mprog->next; mprog = mprog->next );
	            CREATE( mprg, MPROG_DATA, 1 );
	        if ( mprog )
	          mprog->next		= mprg;
	        else
	         room->mudprogs	= mprg;
	        room->progtypes |= ( 1 << mptype );


    	        if ( mptype != -1 )
	        {
	          mprg->type = 1 << mptype;
	          if ( mprg->arglist )
	            STRFREE( mprg->arglist );
	          mprg->arglist = STRALLOC( arg2 );
	        }
                mprg->comlist = STRALLOC( argument );
	        mprg->next = NULL;
            }
        }
    }
}

char *parse_prog_string(char *inp,int ship_type,int vnum)
{
  char sch[MAX_STRING_LENGTH];
  char rep[MAX_STRING_LENGTH];
  static char newinp[MAX_STRING_LENGTH];
  int x,size;
    strcpy(newinp,inp);
    size = ship_prototypes[ship_type].num_rooms;
    for(x=0;x<size;x++)
    {
        sprintf(sch,"$RNUM:%d$",x+1);
        sprintf(rep,"%d",x+vnum);
        sprintf(newinp,"%s",strrep(newinp,sch,rep));
    }
  return newinp;
}

void save_prototype( int prototype )
{
    PROTO_ROOM          *proom;
    FILE		*fpout;
    char                filename[MAX_STRING_LENGTH];
    int x;
    fclose( fpReserve );
    sprintf(filename,"%s%s.proto",SHIP_PROTOTYPE_DIR,ship_prototypes[prototype].sname);
    if ( ( fpout = fopen( filename, "w" ) ) == NULL )
    {
	perror( filename );
	fpReserve = fopen( NULL_FILE, "r" );
	return;
    }
    fprintf( fpout, "#HEADER\n\n");
    fprintf( fpout, "Name           %s~\n", ship_prototypes[prototype].name );
    fprintf( fpout, "Sname          %s~\n", ship_prototypes[prototype].sname );
    fprintf( fpout, "NumRooms       %d\n",  ship_prototypes[prototype].num_rooms );
    fprintf( fpout, "Cost           %d\n",  ship_prototypes[prototype].cost );
    fprintf( fpout, "Class          %d\n",  ship_prototypes[prototype].class );
    fprintf( fpout, "Tractor        %d\n",  ship_prototypes[prototype].tractor );
    fprintf( fpout, "primaryType         %d\n",  ship_prototypes[prototype].primaryType );
	fprintf( fpout, "secondaryType		 %d\n",  ship_prototypes[prototype].secondaryType);
    fprintf( fpout, "primaryCount		 %d\n",  ship_prototypes[prototype].primaryCount);
	fprintf( fpout, "secondaryCount		 %d\n",  ship_prototypes[prototype].secondaryCount);
    fprintf( fpout, "RangeWeapons0  %d\n",  ship_prototypes[prototype].range_weapons[0] );
    fprintf( fpout, "RangeWeapons1  %d\n",  ship_prototypes[prototype].range_weapons[1] );
    fprintf( fpout, "RangeWeapons2  %d\n",  ship_prototypes[prototype].range_weapons[2] );
    fprintf( fpout, "Hull           %d\n",  ship_prototypes[prototype].hull );
    fprintf( fpout, "Shields        %d\n",  ship_prototypes[prototype].shields );
    fprintf( fpout, "Energy         %d\n",  ship_prototypes[prototype].energy );
    fprintf( fpout, "Chaff          %d\n",  ship_prototypes[prototype].chaff );
    fprintf( fpout, "MaxCargo       %d\n",  ship_prototypes[prototype].maxcargo );
    fprintf( fpout, "Maxbombs       %d\n",  ship_prototypes[prototype].maxbombs );
    fprintf( fpout, "Speed          %d\n",  ship_prototypes[prototype].speed );
    fprintf( fpout, "Hyperspeed     %d\n",  ship_prototypes[prototype].hyperspeed );
    fprintf( fpout, "Manuever       %d\n",  ship_prototypes[prototype].manuever );
    fprintf( fpout, "Turrets        %d\n",  ship_prototypes[prototype].turrets );
    fprintf( fpout, "Mods	    %d\n",  ship_prototypes[prototype].mods );
    fprintf( fpout, "Clan           %s~\n", ship_prototypes[prototype].clan );
    //fprintf( fpout, "Plasma         %d\n",  ship_prototypes[prototype].plasma );
    fprintf( fpout, "End\n");
    fprintf( fpout, "\n#ROOMS\n\n" );
    for(proom = first_prototype_room;proom;proom = proom->next)
    {
      if(proom->what_prototype == prototype)
      {
        fprintf( fpout, "#ROOM\n\n");
        fprintf( fpout, "RoomNum           %d\n", proom->room_num );
        fprintf( fpout, "Name              %s~\n",proom->name  );
        fprintf( fpout, "Desc              %s~\n",proom->desc  );
        fprintf( fpout, "Flags             %s~\n",proom->flags );
        fprintf( fpout, "Tunnel            %d\n", proom->tunnel );
        for(x=0;x<10;x++)
            fprintf( fpout, "Exits%d       %d\n", x, proom->exits[x] );
        for(x=0;x<10;x++)
            fprintf( fpout, "Exflags%d     %d\n", x, proom->exflags[x] );
        for(x=0;x<10;x++)
            fprintf( fpout, "Keys%d        %d\n", x, proom->keys[x] );
        fprintf( fpout, "RoomType          %d\n", proom->room_type );
        for(x=0;x<10;x++)
            fprintf( fpout, "Rprog%d        %s~\n", x, proom->rprog[x] );
        for(x=0;x<10;x++)
            fprintf( fpout, "Reset%d        %s~\n", x, proom->reset[x] );
        fprintf( fpout,"End\n\n");
      }
    }
    fprintf( fpout, "\n#ENDROOMS\n\n" );

    fprintf( fpout,"#END\n" );
    fclose( fpout );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

void write_prototype_list()
{
    FILE		*fpout;
    char                filename[MAX_STRING_LENGTH];
    int x;
    fclose( fpReserve );
    sprintf(filename,"%sprototype.lst",SHIP_PROTOTYPE_DIR);
    if ( ( fpout = fopen( filename, "w" ) ) == NULL )
    {
	perror( filename );
	fpReserve = fopen( NULL_FILE, "r" );
	return;
    }
    for(x=0;x<NUM_PROTOTYPES;x++)
        fprintf(fpout, "%s.proto\n",ship_prototypes[x].sname);
    fprintf(fpout,"$\n");
    fclose( fpout );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

void write_all_prototypes()
{
  int x;
    for(x=0;x<NUM_PROTOTYPES;x++)
        save_prototype(x);
    write_prototype_list();
}

bool load_prototype_header(FILE *fp,int prototype)
{
  char buf[MAX_STRING_LENGTH];
  char *word;
  bool done = false;
  bool fMatch;
    while(!done)
    {
      word   = feof( fp ) ? "End" : fread_word( fp );
      fMatch = false;
	switch ( UPPER(word[0]) )
	{
           case 'C':
               KEY( "Cost",           ship_prototypes[prototype].cost,             fread_number(fp));
               KEY( "Class",          ship_prototypes[prototype].class,            fread_number(fp));
               KEY( "Chaff",          ship_prototypes[prototype].chaff,            fread_number(fp));
               KEY( "Clan",	      ship_prototypes[prototype].clan,		   fread_string(fp));
           case 'E':
               KEY( "Energy",         ship_prototypes[prototype].energy,           fread_number(fp));
  	       if ( !str_cmp( word, "End" ) )
	       {
	       	    done = true;
	       	    fMatch = true;
	       	  break;
	       }
           case 'H':
               KEY( "Hull",           ship_prototypes[prototype].hull,             fread_number(fp));
               KEY( "Hyperspeed",     ship_prototypes[prototype].hyperspeed,       fread_number(fp));

           case 'M':
               KEY( "Maxbombs",       ship_prototypes[prototype].maxbombs,            fread_number(fp));
               KEY( "MaxCargo",       ship_prototypes[prototype].maxcargo,         fread_number(fp));
               KEY( "Manuever",       ship_prototypes[prototype].manuever,         fread_number(fp));
               KEY( "Mods",	      ship_prototypes[prototype].mods,		   fread_number(fp));
           case 'N':
               KEY( "Name",           ship_prototypes[prototype].name,             fread_string(fp));
               KEY( "NumRooms",       ship_prototypes[prototype].num_rooms,        fread_number(fp));
           case 'P':
               KEY( "primaryType",         ship_prototypes[prototype].primaryType,           fread_number(fp));
		       KEY( "primaryCount", ship_prototypes[prototype].primaryCount, fread_number(fp));
           case 'R':
               KEY( "RangeWeapons0",  ship_prototypes[prototype].range_weapons[0], fread_number(fp));
               KEY( "RangeWeapons1",  ship_prototypes[prototype].range_weapons[1], fread_number(fp));
               KEY( "RangeWeapons2",  ship_prototypes[prototype].range_weapons[2], fread_number(fp));
           case 'S':
			   KEY( "secondaryType",  ship_prototypes[prototype].secondaryType, fread_number(fp));
			   KEY( "secondaryCount", ship_prototypes[prototype].secondaryCount, fread_number(fp));
               KEY( "Sname",          ship_prototypes[prototype].sname,            fread_string(fp));
               KEY( "Shields",        ship_prototypes[prototype].shields,          fread_number(fp));
               KEY( "Speed",          ship_prototypes[prototype].speed,            fread_number(fp));
           case 'T':
               KEY( "Tractor",        ship_prototypes[prototype].tractor,          fread_number(fp));
               KEY( "Turrets",        ship_prototypes[prototype].turrets,          fread_number(fp));
        }
	if ( !fMatch )
	{
	    sprintf( buf, "Load_prototype_header: no match: %s", word );
	    bug( buf, 0 );
	}
    }
  return true;
}

bool fread_prototype_room( FILE *fp, int prototype)
{
  PROTO_ROOM *proom;
  char buf[MAX_STRING_LENGTH];
  char *word;
  bool done = false;
  bool fMatch;
    CREATE(proom, PROTO_ROOM, 1);
    proom->what_prototype = prototype;
    while(!done)
    {
      word   = feof( fp ) ? "End" : fread_word( fp );
      fMatch = false;
	switch ( UPPER(word[0]) )
	{
           case 'D':
               KEY( "Desc", proom->desc, fread_string(fp));
           case 'E':
  	       if ( !str_cmp( word, "End" ) )
	       {
	       	    done = true;
	       	    fMatch = true;
	       	  break;
	       }
               KEY( "Exits0", proom->exits[0], fread_number(fp));
               KEY( "Exits1", proom->exits[1], fread_number(fp));
               KEY( "Exits2", proom->exits[2], fread_number(fp));
               KEY( "Exits3", proom->exits[3], fread_number(fp));
               KEY( "Exits4", proom->exits[4], fread_number(fp));
               KEY( "Exits5", proom->exits[5], fread_number(fp));
               KEY( "Exits6", proom->exits[6], fread_number(fp));
               KEY( "Exits7", proom->exits[7], fread_number(fp));
               KEY( "Exits8", proom->exits[8], fread_number(fp));
               KEY( "Exits9", proom->exits[9], fread_number(fp));
               KEY( "Exflags0", proom->exflags[0], fread_number(fp));
               KEY( "Exflags1", proom->exflags[1], fread_number(fp));
               KEY( "Exflags2", proom->exflags[2], fread_number(fp));
               KEY( "Exflags3", proom->exflags[3], fread_number(fp));
               KEY( "Exflags4", proom->exflags[4], fread_number(fp));
               KEY( "Exflags5", proom->exflags[5], fread_number(fp));
               KEY( "Exflags6", proom->exflags[6], fread_number(fp));
               KEY( "Exflags7", proom->exflags[7], fread_number(fp));
               KEY( "Exflags8", proom->exflags[8], fread_number(fp));
               KEY( "Exflags9", proom->exflags[9], fread_number(fp));
           case 'F':
               KEY( "Flags", proom->flags, fread_string(fp));
           case 'K':
               KEY( "Keys0", proom->keys[0], fread_number(fp));
               KEY( "Keys1", proom->keys[1], fread_number(fp));
               KEY( "Keys2", proom->keys[2], fread_number(fp));
               KEY( "Keys3", proom->keys[3], fread_number(fp));
               KEY( "Keys4", proom->keys[4], fread_number(fp));
               KEY( "Keys5", proom->keys[5], fread_number(fp));
               KEY( "Keys6", proom->keys[6], fread_number(fp));
               KEY( "Keys7", proom->keys[7], fread_number(fp));
               KEY( "Keys8", proom->keys[8], fread_number(fp));
               KEY( "Keys9", proom->keys[9], fread_number(fp));
           case 'N':
               KEY( "Name", proom->name, fread_string(fp));
           case 'R':
               KEY( "RoomType", proom->room_type, fread_number(fp));
               KEY( "RoomNum", proom->room_num, fread_number(fp));
               KEY( "Rprog0", proom->rprog[0], fread_string(fp));
               KEY( "Rprog1", proom->rprog[1], fread_string(fp));
               KEY( "Rprog2", proom->rprog[2], fread_string(fp));
               KEY( "Rprog3", proom->rprog[3], fread_string(fp));
               KEY( "Rprog4", proom->rprog[4], fread_string(fp));
               KEY( "Rprog5", proom->rprog[5], fread_string(fp));
               KEY( "Rprog6", proom->rprog[6], fread_string(fp));
               KEY( "Rprog7", proom->rprog[7], fread_string(fp));
               KEY( "Rprog8", proom->rprog[8], fread_string(fp));
               KEY( "Rprog9", proom->rprog[9], fread_string(fp));
               KEY( "Reset0", proom->reset[0], fread_string(fp));
               KEY( "Reset1", proom->reset[1], fread_string(fp));
               KEY( "Reset2", proom->reset[2], fread_string(fp));
               KEY( "Reset3", proom->reset[3], fread_string(fp));
               KEY( "Reset4", proom->reset[4], fread_string(fp));
               KEY( "Reset5", proom->reset[5], fread_string(fp));
               KEY( "Reset6", proom->reset[6], fread_string(fp));
               KEY( "Reset7", proom->reset[7], fread_string(fp));
               KEY( "Reset8", proom->reset[8], fread_string(fp));
               KEY( "Reset9", proom->reset[9], fread_string(fp));
           case 'T':
               KEY( "Tunnel", proom->tunnel, fread_number(fp));
       }
       if ( !fMatch )
       {
	    sprintf( buf, "Fread_prototype_room: no match: %s", word );
	    bug( buf, 0 );
       }
    }
    LINK(proom,first_prototype_room,last_prototype_room,next,prev);
  return true;
}

bool load_prototype_rooms( FILE *fp, int prototype)
{
  char letter;
  char *word;
  bool done = false;
    while(!done)
    {
    	letter = fread_letter( fp );
    	if( letter == '#')
    	{
    	    word = fread_word( fp );
            if(!strcmp("ROOM",word))
            {
            	if(!fread_prototype_room(fp,prototype))
                    return false;
            }
            if(!strcmp("ENDROOMS",word))
            	done = true;
        }
        else
        {
            bug("Load_prototype_rooms, unknown prefix: %s",letter);
          return false;
        }
    }
  return true;
}

int load_prototype( char *prototypefile, int prototype)
{
    char filename[256];
    FILE *fp;
    bool found = false;
    int stage = -1;
    bool ok = true;
    char letter;
    char *word;

    sprintf( filename, "%s%s", SHIP_PROTOTYPE_DIR, prototypefile );

    if ( ( fp = fopen( filename, "r" ) ) != NULL )
    {
        found = true;
        prototype++;
        while(ok)
        {
	    letter = fread_letter( fp );
	    if ( letter != '#' )
	    {
		bug( "Load_prototype: # not found.", 0 );
		break;
	    }
            stage++;
            word = fread_word( fp );
            if(strcmp(word,"END"))
            {
                switch(stage)
                {
                    case 0:
                        if(strcmp(word,"HEADER"))
                        {
                            bug( "Load_prototype: HEADER not found.", 0);
                          break;
                        }
                        if(!load_prototype_header(fp,prototype))
                        {
                           ok = false;
                           prototype--;
                        }
                      break;
                    case 1:
                        if(strcmp(word,"ROOMS"))
                        {
                            bug( "Load_prototype: ROOMS not found.", 0);
                          break;
                        }
                        if(!load_prototype_rooms(fp,prototype))
                        {
                           ok = false;
                           prototype--;
                        }
                      break;
                    default:
                        bug( "Load_prototype: Unknown stage: %d",stage);
                }
            }
            else
              ok = false;
	}
	fclose( fp );
    }
    return prototype;
}

void load_ship_prototypes()
{
    FILE *fpList;
    char *filename;
    char prototypeslist[256];
    int prototype;
    first_prototype_room = NULL;
    last_prototype_room = NULL;

    log_string( "Loading ship prototypes..." );

    sprintf(prototypeslist,"%sprototype.lst",SHIP_PROTOTYPE_DIR);
    fclose( fpReserve );
    if ( ( fpList = fopen( prototypeslist, "r" ) ) == NULL )
    {
	perror( prototypeslist );
	exit( 1 );
    }

    for ( prototype = -1; ; )
    {
	filename = feof( fpList ) ? "$" : fread_word( fpList );
	if ( filename[0] == '$' )
	  break;


	prototype = load_prototype( filename, prototype );
    }
    fclose( fpList );
    log_string(" Done ship prototypes " );
    NUM_PROTOTYPES = prototype+1;
    fpReserve = fopen( NULL_FILE, "r" );
   // write_all_prototypes();
  return;
}

void do_makeprototypeship(CHAR_DATA *ch, char *argument)
{
  SHIP_DATA *ship;
  int prototype;
  int cost;
  int count;
  char ship_name[MAX_STRING_LENGTH];
  char name[MAX_STRING_LENGTH];
  char sname[MAX_STRING_LENGTH];
  char scost[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int x,len;
  ROOM_INDEX_DATA *room;
  MPROG_DATA *mprg;
  EXIT_DATA *eroom;
  int room_count = 1;
  PROTO_ROOM *proom;

    argument = one_argument(argument,ship_name);
    argument = one_argument(argument,scost);
    argument = one_argument(argument,sname);
    strcpy(name,argument);
    if(!ship_name || ship_name[0] == '\0')
    {
        ch->println("You must specify a valid ship name.\r\n",ch);
        ch->println("USAGE: makeprototypeship <ship name> <cost> <short name> <long name>\r\n",ch);
      return;
    }
    cost = atoi(scost);
    if(cost <= 0)
    {
        ch->println("The cost must be greater than 0.\r\n",ch);
        ch->println("USAGE: makeprototypeship <ship name> <cost> <short name> <long name>\r\n",ch);
      return;
    }
    if(!sname || sname[0] == '\0')
    {
        ch->println("You must specify a short name for the ship.\r\n",ch);
        ch->println("USAGE: makeprototypeship <ship name> <cost> <short name> <long name>\r\n",ch);
      return;
    }
    len = strlen(sname);
    for(x=0;x<len;x++)
        sname[x] = UPPER(sname[x]);
    if(!name || name[0] == '\0')
    {
        ch->println("You must specify a long name for the ship.\r\n",ch);
        ch->println("USAGE: makeprototypeship <ship name> <cost> <short name> <long name>\r\n",ch);
      return;
    }
    if(!scost || scost[0] == '\0')
    {
        ch->println("You must specify a cost for the ship.\r\n",ch);
        ch->println("USAGE: makeprototypeship <ship name> <cost> <short name> <long name>\r\n",ch);
      return;
    }
    if(( ship = get_ship(ship_name) ) == NULL)
    {
        ch->println("You must specify a valid ship name.\r\n",ch);
        ch->println("USAGE: makeprototypeship <ship name> <short name> <long name> <cost>\r\n",ch);
      return;
    }

    prototype = NUM_PROTOTYPES++;

    ship_prototypes[prototype].tractor = ship->tractorbeam;
    ship_prototypes[prototype].primaryType = ship->primaryType;
	ship_prototypes[prototype].secondaryType = ship->secondaryType;
	ship_prototypes[prototype].primaryCount = ship->primaryCount;
    ship_prototypes[prototype].secondaryCount = ship->secondaryCount;
    ship_prototypes[prototype].range_weapons[0] = ship->maxmissiles;
    ship_prototypes[prototype].range_weapons[1] = ship->maxtorpedos;
    ship_prototypes[prototype].range_weapons[2] = ship->maxrockets;
    ship_prototypes[prototype].hull = ship->maxhull;
    ship_prototypes[prototype].shields = ship->maxshield;
    //ship_prototypes[prototype].plasma = ship->maxplasmashield;
    ship_prototypes[prototype].energy = ship->maxenergy;
    ship_prototypes[prototype].chaff = ship->maxchaff;
#ifdef USECARGO
    ship_prototypes[prototype].maxcargo = ship->maxcargo;
#endif
    ship_prototypes[prototype].maxbombs = ship->maxbombs;
    ship_prototypes[prototype].speed = ship->realspeed;
    ship_prototypes[prototype].hyperspeed = ship->hyperspeed;
    ship_prototypes[prototype].manuever = ship->manuever;
    ship_prototypes[prototype].cost = cost;
    sprintf(buf,"%s: '%s'",sname,name);
    ship_prototypes[prototype].name = STRALLOC( buf );
    ship_prototypes[prototype].sname = STRALLOC( sname );
    ship_prototypes[prototype].class = ship->ship_class;
    ship_prototypes[prototype].num_rooms = ship->lastroom - ship->firstroom + 1;
    if(ship->turret1 != 0)
      count++;
    if(ship->turret2 != 0)
      count++;
    if(ship->turret3 != 0)
      count++;
    if(ship->turret4 != 0)
      count++;
    if(ship->turret5 != 0)
      count++;
    if(ship->turret6 != 0)
      count++;
    if(ship->turret7 != 0)
      count++;
    if(ship->turret8 != 0)
      count++;
    if(ship->turret9 != 0)
      count++;
    if(ship->turret10 != 0)
      count++;
    ship_prototypes[prototype].turrets = count;

    for ( room = get_room_index(ship->firstroom) ; room ; room = room->next_in_ship )
    {
        CREATE(proom, PROTO_ROOM, 1);
	proom->room_num = room_count;
        proom->name = STRALLOC(room->name);
        proom->desc = STRALLOC(room->description);
        proom->flags = STRALLOC(flag_string(room->room_flags, r_flags));
        proom->tunnel = room->tunnel;

        for(x=0;x<10;x++)
	{
	    eroom = get_exit_num(room, x);
	    if(eroom)
	    {
	    proom->exits[x] = eroom->to_room->vnum;
	    proom->exflags[x] = eroom->exit_info;
	    proom->keys[x] = eroom->key;
	    }
	}

	if( ship->cockpit == room->vnum)
	    proom->room_type = 1;
	if( ship->entrance == room->vnum)
	    proom->room_type = 2;
	if( ship->hanger1 == room->vnum)
	    proom->room_type = 3;
	if( ship->hanger2 == room->vnum)
	    proom->room_type = 4;
	if( ship->hanger3 == room->vnum)
	    proom->room_type = 5;
	if( ship->hanger4 == room->vnum)
	    proom->room_type = 6;
	if( ship->engineroom == room->vnum)
	    proom->room_type = 7;
	if( ship->pilotseat == room->vnum)
	    proom->room_type = 8;
	if( ship->coseat == room->vnum)
	    proom->room_type = 9;
	if( ship->navseat == room->vnum)
	    proom->room_type = 8;
	if( ship->gunseat == room->vnum)
	    proom->room_type = 9;
	if( ship->turret1 == room->vnum)
	    proom->room_type = 10;
	if( ship->turret2 == room->vnum)
	    proom->room_type = 11;
	if( ship->turret3 == room->vnum)
	    proom->room_type = 12;
	if( ship->turret4 == room->vnum)
	    proom->room_type = 13;
	if( ship->turret5 == room->vnum)
	    proom->room_type = 14;
	if( ship->turret6 == room->vnum)
	    proom->room_type = 15;
	if( ship->turret7 == room->vnum)
	    proom->room_type = 16;
	if( ship->turret8 == room->vnum)
	    proom->room_type = 17;
	if( ship->turret9 == room->vnum)
	    proom->room_type = 18;
	if( ship->turret10 == room->vnum)
	    proom->room_type = 19;

    x = 1;
    for ( mprg = room->mudprogs; mprg; mprg = mprg->next )
    {
	sprintf(buf, "%s %s %s ~", mprog_flags[mprg->type], mprg->arglist, mprg->comlist);
	proom->rprog[x] = STRALLOC(buf);
	x++;
    }

*//* Remind me to add resets later
	for(x=0;x<10;x++)
            fprintf( fpout, "Reset%d        %s~\n", x, proom->reset[x] );
        fprintf( fpout,"End\n\n");
*/
/*
    LINK(proom,first_prototype_room,last_prototype_room,next,prev);
    room_count++;
    }

    ch->println("Done.\r\n",ch);
    save_prototype(prototype);
  return;
}

char pbname[MAX_STRING_LENGTH];

char *primary_beam_name_proto(int shiptype)
{
    if(ship_prototypes[shiptype].primaryCount != 0)
	{
		sprintf(pbname, "%d %s", ship_prototypes[shiptype].primaryCount,
		(ship_prototypes[shiptype].primaryType == SINGLE_LASER && ship_prototypes[shiptype].primaryCount == 1)? "Single-laser cannon" :
		(ship_prototypes[shiptype].primaryType == SINGLE_LASER && ship_prototypes[shiptype].primaryCount != 1)? "Single-laser cannons" :
		(ship_prototypes[shiptype].primaryType == DUAL_LASER && ship_prototypes[shiptype].primaryCount == 1)? "Dual-laser cannon" :
		(ship_prototypes[shiptype].primaryType == DUAL_LASER && ship_prototypes[shiptype].primaryCount != 1)? "Dual-laser cannons" :
		(ship_prototypes[shiptype].primaryType == TRI_LASER && ship_prototypes[shiptype].primaryCount == 1)? "Triple-laser cannon" :
		(ship_prototypes[shiptype].primaryType == TRI_LASER && ship_prototypes[shiptype].primaryCount != 1)? "Triple-laser cannons" :
		(ship_prototypes[shiptype].primaryType == QUAD_LASER && ship_prototypes[shiptype].primaryCount == 1)? "Quad-laser cannon" :
		(ship_prototypes[shiptype].primaryType == QUAD_LASER && ship_prototypes[shiptype].primaryCount != 1)? "Quad-laser cannons" :
		(ship_prototypes[shiptype].primaryType == AUTOBLASTER && ship_prototypes[shiptype].primaryCount == 1)? "Autoblaster turret" :
		(ship_prototypes[shiptype].primaryType == AUTOBLASTER && ship_prototypes[shiptype].primaryCount != 1)? "Autoblaster turrets" :
		(ship_prototypes[shiptype].primaryType == HEAVY_LASER && ship_prototypes[shiptype].primaryCount == 1)? "Heavy laser cannon" :
		(ship_prototypes[shiptype].primaryType == HEAVY_LASER && ship_prototypes[shiptype].primaryCount != 1)? "Heavy laser cannons" :
		(ship_prototypes[shiptype].primaryType == LIGHT_ION && ship_prototypes[shiptype].primaryCount == 1)? "Light ion cannon" :
		(ship_prototypes[shiptype].primaryType == LIGHT_ION && ship_prototypes[shiptype].primaryCount != 1)? "Light ion cannons" :
		(ship_prototypes[shiptype].primaryType == REPEATING_ION && ship_prototypes[shiptype].primaryCount == 1)? "Repeating ion cannon" :
		(ship_prototypes[shiptype].primaryType == REPEATING_ION && ship_prototypes[shiptype].primaryCount != 1)? "Repeating ion cannons" :
		(ship_prototypes[shiptype].primaryType == HEAVY_ION && ship_prototypes[shiptype].primaryCount == 1)? "Heavy ion cannon" :
		(ship_prototypes[shiptype].primaryType == HEAVY_ION && ship_prototypes[shiptype].primaryCount != 1)? "Heavy ion cannons" :
			"unknown");

		return pbname;
	}
	else
		return "None.";
}
char *secondary_beam_name_proto(int shiptype)
{
    if(ship_prototypes[shiptype].secondaryCount != 0)
	{
		sprintf(pbname, "%d %s", ship_prototypes[shiptype].secondaryCount,
		(ship_prototypes[shiptype].secondaryType == SINGLE_LASER && ship_prototypes[shiptype].secondaryCount == 1)? "Single-laser cannon" :
		(ship_prototypes[shiptype].secondaryType == SINGLE_LASER && ship_prototypes[shiptype].secondaryCount != 1)? "Single-laser cannons" :
		(ship_prototypes[shiptype].secondaryType == DUAL_LASER && ship_prototypes[shiptype].secondaryCount == 1)? "Dual-laser cannon" :
		(ship_prototypes[shiptype].secondaryType == DUAL_LASER && ship_prototypes[shiptype].secondaryCount != 1)? "Dual-laser cannons" :
		(ship_prototypes[shiptype].secondaryType == TRI_LASER && ship_prototypes[shiptype].secondaryCount == 1)? "Triple-laser cannon" :
		(ship_prototypes[shiptype].secondaryType == TRI_LASER && ship_prototypes[shiptype].secondaryCount != 1)? "Triple-laser cannons" :
		(ship_prototypes[shiptype].secondaryType == QUAD_LASER && ship_prototypes[shiptype].secondaryCount == 1)? "Quad-laser cannon" :
		(ship_prototypes[shiptype].secondaryType == QUAD_LASER && ship_prototypes[shiptype].secondaryCount != 1)? "Quad-laser cannons" :
		(ship_prototypes[shiptype].secondaryType == AUTOBLASTER && ship_prototypes[shiptype].secondaryCount == 1)? "Autoblaster turret" :
		(ship_prototypes[shiptype].secondaryType == AUTOBLASTER && ship_prototypes[shiptype].secondaryCount != 1)? "Autoblaster turrets" :
		(ship_prototypes[shiptype].secondaryType == HEAVY_LASER && ship_prototypes[shiptype].secondaryCount == 1)? "Heavy laser cannon" :
		(ship_prototypes[shiptype].secondaryType == HEAVY_LASER && ship_prototypes[shiptype].secondaryCount != 1)? "Heavy laser cannons" :
		(ship_prototypes[shiptype].secondaryType == LIGHT_ION && ship_prototypes[shiptype].secondaryCount == 1)? "Light ion cannon" :
		(ship_prototypes[shiptype].secondaryType == LIGHT_ION && ship_prototypes[shiptype].secondaryCount != 1)? "Light ion cannons" :
		(ship_prototypes[shiptype].secondaryType == REPEATING_ION && ship_prototypes[shiptype].secondaryCount == 1)? "Repeating ion cannon" :
		(ship_prototypes[shiptype].secondaryType == REPEATING_ION && ship_prototypes[shiptype].secondaryCount != 1)? "Repeating ion cannons" :
		(ship_prototypes[shiptype].secondaryType == HEAVY_ION && ship_prototypes[shiptype].secondaryCount == 1)? "Heavy ion cannon" :
		(ship_prototypes[shiptype].secondaryType == HEAVY_ION && ship_prototypes[shiptype].secondaryCount != 1)? "Heavy ion cannons" :
			"unknown");

		return pbname;
	}
	else
		return "None.";
}
void do_shipstat( CHAR_DATA *ch, char *argument )
{
 int shiptype;
 char buf1[MAX_STRING_LENGTH];
 char buf2[MAX_STRING_LENGTH];
 char buf3[MAX_STRING_LENGTH];
 char buf4[MAX_STRING_LENGTH];
 char buf5[MAX_STRING_LENGTH];
 char buf6[MAX_STRING_LENGTH];
 char buf7[MAX_STRING_LENGTH];
 char buf8[MAX_STRING_LENGTH];
 char buf9[MAX_STRING_LENGTH];
 char buf10[MAX_STRING_LENGTH];

 if ( IS_NPC( ch ) )
 {
  ch->println( "Huh?\n\r", ch );
  return;
 }
 if ( argument[0] == '\0' )
 {
  ch->println( "Usage: shipstat <ship number>\n\r", ch );
  return;
 }
 shiptype = atoi(argument)-1;
 if(shiptype < 0 || shiptype > NUM_PROTOTYPES-1)
 {
  ch->println("Invalid number.\n\r", ch);
  return;
 }

 if(ship_prototypes[shiptype].primaryType > 0) sprintf(buf1, "%d", ship_prototypes[shiptype].primaryType);
  else sprintf(buf1, "&RNone.");
 if(ship_prototypes[shiptype].secondaryType > 0) sprintf(buf2, "%d", ship_prototypes[shiptype].secondaryType);
  else sprintf(buf2, "&RNone.");
 if(ship_prototypes[shiptype].range_weapons[0] > 0) sprintf(buf3, "%d", ship_prototypes[shiptype].range_weapons[0]);
  else sprintf(buf3, "&RNone.");
 if(ship_prototypes[shiptype].shields > 0) sprintf(buf4, "%d", ship_prototypes[shiptype].shields);
  else sprintf(buf4, "&RNone.");
 if(ship_prototypes[shiptype].range_weapons[1] > 0) sprintf(buf5, "%d", ship_prototypes[shiptype].range_weapons[1]);
  else sprintf(buf5, "&RNone.");
 if(ship_prototypes[shiptype].hyperspeed > 0) sprintf(buf6, "Class %d", ship_prototypes[shiptype].hyperspeed);
  else sprintf(buf6, "&RNone.");
 if(ship_prototypes[shiptype].range_weapons[2] > 0) sprintf(buf7, "%d", ship_prototypes[shiptype].range_weapons[2]);
  else sprintf(buf7, "&RNone.");
 if(ship_prototypes[shiptype].maxbombs > 0) sprintf(buf8, "%d", ship_prototypes[shiptype].maxbombs);
  else sprintf(buf8, "&RNone.");
 if(ship_prototypes[shiptype].turrets > 0) sprintf(buf9, "%d", ship_prototypes[shiptype].turrets);
  else sprintf(buf9, "&RNone.");
 if(ship_prototypes[shiptype].chaff > 0) sprintf(buf10, "%d", ship_prototypes[shiptype].chaff);
  else sprintf(buf10, "&RNone.");

 ch->printlnf(  "&R&z+&W---------------------------------------------------------------&z+\n\r");
 ch->printlnf(  "&W| Name: &w%-35.35s      &WCost: &w%8d &W|\r\n",
 		ship_prototypes[shiptype].name, ship_prototypes[shiptype].cost);
 ch->printlnf(  "&z+&W---------------------------------------------------------------&z+\n\r\n\r");

 ch->printlnf(  "&W         Primary Weapon System:&w %-30.30s\n\r", primary_beam_name_proto(shiptype));
 ch->printlnf(  "&W                     Secondary:&w %-30.30s\n\r\n\r", secondary_beam_name_proto(shiptype));
 ch->printlnf(  "&W          Missiles:&w %-5s&W   Torpedos:&w %-5s&W   Rockets:&w %-5s\n\r",
	 buf3, buf5, buf7);
 ch->printlnf(  "&W   Planetary bombs:&w %-5s&W      Chaff:&w %-5s\n\r",
	 buf8, buf10);
 ch->printlnf(  "\n\r&W   Hull:&w %-5d&W   Shields:&w %-5s&W   Speed:&w %-5d&W   Energy:&w %d\n\r",
	ship_prototypes[shiptype].hull, buf4,
	ship_prototypes[shiptype].speed, ship_prototypes[shiptype].energy);
 ch->printlnf(  "&W           Maneuverability:&w %d   &WHyperdrive:&w %-5s\n\r", ship_prototypes[shiptype].manuever, buf6);
 ch->printlnf(  "&W                        Turrets:&w %-5s\n\r",buf9);
 ch->printlnf(  "&W                      MaxCargo %d", ship_prototypes[shiptype].maxcargo);

 return;
}

void load_market_list()
{
    FILE *fpList;
    char *filename;
    char list[256];
    BMARKET_DATA *marketship;
    int	quantity;

    first_market_ship = NULL;
    last_market_ship = NULL;

    log_string( "Loading black market." );

    sprintf( list, "%s%s", SHIP_PROTOTYPE_DIR, "blackmarket.lst" );
    fclose( fpReserve );
    if ( ( fpList = fopen( list, "r" ) ) == NULL )
    {
	perror( list );
	exit( 1 );
    }

    for ( ; ; )
    {
        filename = feof( fpList ) ? "$" : fread_word( fpList );
        if ( filename[0] == '$' )
        break;
	CREATE( marketship, BMARKET_DATA, 1 );
        LINK( marketship, first_market_ship, last_market_ship, next, prev );
	marketship->filename = STRALLOC(filename);
	quantity = fread_number( fpList );
	marketship->quantity = quantity;
    }
    fclose( fpList );
    log_string("Finished loading blackmarket." );
    fpReserve = fopen( NULL_FILE, "r" );

    return;
}

void save_market_list()
{
    BMARKET_DATA *marketship;
    FILE *fpout;
    char filename[256];

    sprintf( filename, "%s%s", SHIP_PROTOTYPE_DIR, "blackmarket.lst" );
    fpout = fopen( filename, "w" );
    if ( !fpout )
    {
         bug( "FATAL: cannot open blackmarket.lst for writing.\n\r", 0 );
         return;
    }
    for ( marketship = first_market_ship; marketship; marketship= marketship->next )
    {
        fprintf( fpout, "%s\n", marketship->filename );
        fprintf( fpout, "%d\n", marketship->quantity );
    }
    fprintf( fpout, "$\n" );
    fclose( fpout );
}

void add_market_ship( SHIP_DATA *ship )
{
    BMARKET_DATA *marketship;
    bool found;

    if(!ship)  return;

    for ( marketship = first_market_ship; marketship; marketship = marketship->next )
    {
    	if ( !str_cmp( marketship->filename , ship->protoname ))
    	{
    		//Debugging  bug("Found, adding quantity", 0);
    		found = true;
    		marketship->quantity++;
    		return;
    	}
    }


    	//Debugging  bug("Not found, adding to .lst", 0);
        CREATE( marketship, BMARKET_DATA, 1 );
        LINK( marketship, first_market_ship, last_market_ship, next, prev );

        marketship->filename      = STRALLOC( ship->protoname );
        marketship->quantity      = 1;

    save_market_list();
}

void remove_market_ship( BMARKET_DATA *marketship )
{
    if(marketship->quantity > 1)
     marketship->quantity--;
    else
    {
	UNLINK( marketship, first_market_ship, last_market_ship, next, prev );
	STRFREE( marketship->filename );
	DISPOSE( marketship );
    }

    save_market_list();
}

void make_random_marketlist()
{
// BMARKET_DATA *marketship;
 BMARKET_DATA *nmarketship;
 FILE *fpout;
 char filename[256];
 int x,priority,count;

 // Clear previously loaded data *** Change of plans, delete file and rebuild from scratch
 // *** db.c does not load from file anymore
*//* for( marketship = first_market_ship; marketship; marketship = marketship->next )
 {
	UNLINK( marketship, first_market_ship, last_market_ship, next, prev );
	STRFREE( marketship->filename );
	DISPOSE( marketship );
 }*/
/*
 sprintf( filename, "%s%s", SHIP_PROTOTYPE_DIR, "blackmarket.lst" );
 fpout = fopen(filename, "w");
 fprintf( fpout, "$\n" );
 fclose( fpout );

 // Make a new list
 count = 0;
while(1)
{
 for(x=0;x<NUM_PROTOTYPES;x++)
 {

  if(str_cmp(ship_prototypes[x].clan,"The Empire") && str_cmp(ship_prototypes[x].clan,"The New Republic"))
   continue;
  if(count == 1)
  {
   if(number_range(1, 10) == 1)
   {
   	save_market_list();
   	return;
   }
  }
  if(count == 2)
  {
   if(number_range(1, 10) < 3)
   {
   	save_market_list();
   	return;
   }
  }
  if(count == 3)
  {
   if(number_range(1, 10) != 3)
   {
  	save_market_list();
  	return;
   }
  }
  if(count == 4)
  {
  	save_market_list();
  	return;
  }

  priority = 0;
  if(!str_cmp(ship_prototypes[x].sname,"TIE"))		priority = 10;
  if(!str_cmp(ship_prototypes[x].sname,"TIE.D"))	priority = 1;
  if(!str_cmp(ship_prototypes[x].sname,"TIE.B"))	priority = 5;
  if(!str_cmp(ship_prototypes[x].sname,"XM-1"))		priority = 3;
  if(!str_cmp(ship_prototypes[x].sname,"XG-1"))		priority = 4;
  if(!str_cmp(ship_prototypes[x].sname,"X-Wing"))	priority = 8;
  if(!str_cmp(ship_prototypes[x].sname,"Y-Wing"))	priority = 8;
  if(!str_cmp(ship_prototypes[x].sname,"B-Wing"))	priority = 2;
  if(!str_cmp(ship_prototypes[x].sname,"A-Wing"))	priority = 6;
  if(!str_cmp(ship_prototypes[x].sname,"K-Wing"))	priority = 3;

  if(priority != 0)
  {
   if(priority == 1 || priority == 2)
   {
    if(number_range(1, 25) >= 24)
    {
*//*    	for(marketship = first_market_ship; marketship; marketship = marketship->next)
    	{
    	 if(str_cmp(marketship->filename, ship_prototypes[x].sname))
   	 {*/
/*   	 	CREATE(nmarketship, BMARKET_DATA, 1);
   		LINK(nmarketship, first_market_ship, last_market_ship, next, prev);

   		nmarketship->filename = STRALLOC(ship_prototypes[x].sname);
   		nmarketship->quantity = 1;
   		count++;
*//*   	 }
   	} */
/*    }
   }
   if(priority <= 5 && priority >= 3)
   {
    if(number_range(1, 25) >= 22)
    {
*//*    	for(marketship = first_market_ship; marketship; marketship = marketship->next)
    	{
    		if(str_cmp(ship_prototypes[x].sname, marketship->filename))
    		{*/
/*		 CREATE( nmarketship, BMARKET_DATA, 1 );
        	 LINK( nmarketship, first_market_ship, last_market_ship, next, prev );

        	 nmarketship->filename = STRALLOC(ship_prototypes[x].sname);
        	 if(priority >= 4 && (number_range(1, 10) == 10))  nmarketship->quantity = number_range(1, 3);
        	 else		   nmarketship->quantity = 1;
        	 count++;
*//*        	}
        }*/
/*    }
   }
   if(priority > 5 && priority < 9)
   {
    if(number_range(1, 25) >= 20)
    {
*//*    	for(marketship = first_market_ship; marketship; marketship = marketship->next)
    	{
    		if(str_cmp(ship_prototypes[x].sname, marketship->filename))
    		{*/
/*   		 CREATE(nmarketship, BMARKET_DATA, 1);
   		 LINK(nmarketship, first_market_ship, last_market_ship, next, prev);

   		 nmarketship->filename = STRALLOC(ship_prototypes[x].sname);
   		 if((priority == 5 || priority == 6) && (number_range(1, 10) == 10)) nmarketship->quantity = number_range(1, 3);
   		 else				    nmarketship->quantity = 1;
   		 count++;
*/ /*  		}
   	}*/
/*    }
   }
   if(priority >= 9)
   {
    if(number_range(1, 25) >= 18)
    {
*//*    	for(marketship = first_market_ship; marketship; marketship = marketship->next)
    	{
    		if(str_cmp(ship_prototypes[x].sname, marketship->filename))
    		{*/
/*   	 	 CREATE(nmarketship, BMARKET_DATA, 1);
   	 	 LINK(nmarketship, first_market_ship, last_market_ship, next, prev);

   		 nmarketship->filename = STRALLOC(ship_prototypes[x].sname);
   		 if(number_range(1, 10) == 10)
   	 	  nmarketship->quantity = number_range(1, 3);
   		 else
   	 	  nmarketship->quantity = 1;
   		 count++;
*//*   		}
   	}*//*
    }
   }
  }
 } // End for prototypes
}
 save_market_list();
}

void do_generate_market(CHAR_DATA *ch, char *argument)
{
 BMARKET_DATA *marketship;


 if(IS_NPC(ch))  return;

 if(!first_market_ship)
 {
   bug("Error 0.");
   return;
 }

 marketship = first_market_ship;
 if(!(marketship))
 {
   bug("Error 2.");
   return;
 }


 // Clear list
 for(marketship = first_market_ship; marketship; marketship = marketship->next)
 {
 	UNLINK( marketship, first_market_ship, last_market_ship, next, prev );
	STRFREE( marketship->filename );
// Pretty sure you don't need this, especially after its been unlinked.
//	DISPOSE( marketship );
 }
 // Generate new
 make_random_marketlist();
 return;
}



void do_installmodule( CHAR_DATA *ch, char *argument )
{
  SHIP_DATA *ship;
  MODULE_DATA *mod;
  int i=0, maxmod;
  bool checktool,checkmod;
  OBJ_DATA *obj;
  OBJ_DATA *modobj;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_INPUT_LENGTH];
  int x;
  int  chance;
//Max slots per mod.
  int maxslot = 2;
//Damned Slots.
  int primary=0;
  int secondary=0;
  int missile=0;
  int rocket=0;
  int torpedo=0;
  int hull=0;
  int shields=0;
  int speed=0;
  int hyperspeed=0;
  int energy=0;
  int manuever=0;
  int alarm=0;
  int chaff=0;
  int slave=0;
  int tractor=0;

  strcpy( arg , argument );
  checktool = false;
  checkmod = false;
  switch( ch->substate )
   {
    default:
    if ( (ship = ship_from_engine(ch->in_room->vnum)) != NULL )
      {
      	ship = ship_from_engine(ch->in_room->vnum);
      	strcpy(arg, ship->name);
      }

     if (!ship)
      {
    	ch->println("You need to be in the ships engine room.\n\r", ch);
    	return;
      }

     for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
      {
      	if (obj->item_type == ITEM_TOOLKIT)
      	  checktool = true;
      	if (obj->item_type == ITEM_MODULE)
	{
      	  checkmod = true;
	  modobj = obj;
	}
      }

     if ( checktool == false )
      {
      	ch->println("You need a toolkit to install a module.\n\r", ch);
      	return;
      }
     if ( checkmod == false )
      {
      	ch->println("You need a module to install!\n\r", ch);
      	return;
      }

  for(i=0,mod=ship->first_module;mod;mod=mod->next)
  {
      ++i;
      if(mod->affect == AFFECT_PRIMARY)
        ++primary;
      if(mod->affect == AFFECT_SECONDARY)
        ++secondary;
      if(mod->affect == AFFECT_MISSILE)
        ++missile;
      if(mod->affect == AFFECT_ROCKET)
        ++rocket;
      if(mod->affect == AFFECT_TORPEDO)
        ++torpedo;
      if(mod->affect == AFFECT_HULL)
        ++hull;
      if(mod->affect == AFFECT_SHIELD)
        ++shields;
      if(mod->affect == AFFECT_SPEED)
        ++speed;
      if(mod->affect == AFFECT_HYPER)
        ++hyperspeed;
      if(mod->affect == AFFECT_ENERGY)
        ++energy;
      if(mod->affect == AFFECT_MANUEVER)
        ++manuever;
      if(mod->affect == AFFECT_ALARM)
        ++alarm;
      if(mod->affect == AFFECT_CHAFF)
        ++chaff;
      if(mod->affect == AFFECT_SLAVE)
	++slave;
      if(mod->affect == AFFECT_TRACTOR)
        ++tractor;
  }

//Holy ifchecks batman!
  if (modobj->value[0] == AFFECT_ALARM && (ship->alarm == 1))
 {
    ch->println("This ship already has an alarm system!\n\r", ch);
    return;
 }
  if (modobj->value[0] == AFFECT_HYPER && (ship->hyperspeed == 1))
 {
    ch->println("This ship already has a first class hyperspeed drive.\n\r", ch);
    return;
 }
 if (modobj->value[0] == AFFECT_SLAVE && (ship->slave > 0))
 {
    ch->println("This ship already has an slave circuit!\n\r", ch);
    return;
 }
 if (modobj->value[0] == AFFECT_TRACTOR && (ship->tractorbeam > 0))
 {
    ch->println("This ship already has a tractor beam!\n\r", ch);
    return;
 }
  if((modobj->value[0] == AFFECT_PRIMARY && primary >= maxslot) ||
      (modobj->value[0] == AFFECT_SECONDARY && secondary >= maxslot) ||
      (modobj->value[0] == AFFECT_MISSILE && missile >= maxslot) ||
      (modobj->value[0] == AFFECT_ROCKET && rocket >= maxslot) ||
      (modobj->value[0] == AFFECT_TORPEDO && torpedo >= maxslot) ||
      (modobj->value[0] == AFFECT_HULL && hull >= maxslot) ||
      (modobj->value[0] == AFFECT_SHIELD && shields >= maxslot) ||
      (modobj->value[0] == AFFECT_SPEED && speed >= maxslot) ||
      (modobj->value[0] == AFFECT_HYPER && hyperspeed >= maxslot) ||
      (modobj->value[0] == AFFECT_ENERGY && energy >= maxslot) ||
      (modobj->value[0] == AFFECT_MANUEVER && manuever >= maxslot) ||
      (modobj->value[0] == AFFECT_ALARM && alarm >= maxslot) ||
      (modobj->value[0] == AFFECT_SLAVE && slave >= maxslot) ||
      (modobj->value[0] == AFFECT_TRACTOR && tractor >= maxslot) ||
      (modobj->value[0] == AFFECT_CHAFF && chaff >= maxslot))

  {
    ch->println("&RYou've already filled that slot to its maximum.\n\r", ch);
    return;
  }

      for(x=0;x<NUM_PROTOTYPES;x++)
       {
         if(!str_cmp(ship->protoname, ship_prototypes[x].sname) )
           {
              maxmod = ship_prototypes[x].mods;
              break;
           }
       }

     if (i >= maxmod)
      {
	ch->println("This ship is already at it's module limit!\n\r", ch);
	return;
      }

     if( IS_SET(ship->flags, SHIP_SIMULATOR))
      {
	ch->println("Simulators can't have modules, fool!\n\r", ch);
	return;
      }

     chance = IS_NPC(ch) ? ch->top_level
        : (int) (ch->pcdata->learned[gsn_installmodule]);
     if ( number_percent( ) < chance )
      {
  	ch->dest_buf   = str_dup(arg);
       	ch->println( "&GYou begin the long process of installing a new module.\n\r", ch);
    	sprintf(buf, "$n takes out $s toolkit and a module and begins to work.\n\r");
     	act( AT_PLAIN, buf, ch, NULL, argument , TO_ROOM );
     	add_timer ( ch , TIMER_DO_FUN , 5 , do_installmodule , 1 );
    	return;
      }

      ch->println("&RYou are unable to figure out what to do.\n\r",ch);
      learn_from_failure( ch, gsn_installmodule );
      return;

        case 1:
    	  if ( !ch->dest_buf )
    	     return;
    	  strcpy(arg, ch->dest_buf);
    	  DISPOSE( ch->dest_buf);
    	  break;

    	case SUB_TIMER_DO_ABORT:
    	  DISPOSE( ch->dest_buf );
    	  ch->substate = SUB_NONE;
    	  ch->println("&RYou are interupted and fail to finish.\n\r", ch);
    	  return;
     }
     ch->substate = SUB_NONE;

     for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
      {
         if (obj->item_type == ITEM_TOOLKIT)
           checktool = true;
         if (obj->item_type == ITEM_MODULE){
           checkmod = true;
           modobj = obj;
         }

      }
    chance = IS_NPC(ch) ? ch->top_level
               : (int) (ch->pcdata->learned[gsn_installmodule]);

    ship = ship_from_engine(ch->in_room->vnum);
    if (!ship){
      ch->println("Error: Something went wrong. Contact an Admin!\n\r", ch);
      return;
    }

    if ( number_percent( ) > chance*2 )
     {
       ch->println("&RYou finish installing the new module and everything's looking good...\n\r", ch);
       ch->println("&RThen it turns bright red and melts!\n\r", ch);
       learn_from_failure( ch, gsn_installmodule );
       return;
     }

      CREATE(mod, MODULE_DATA, 1);
      LINK(mod, ship->first_module, ship->last_module, next, prev);
      mod->affect = modobj->value[0];
      mod->ammount = modobj->value[1];
      if(mod->affect == AFFECT_PRIMARY)
        ship->primaryCount+=mod->ammount;
      if(mod->affect == AFFECT_SECONDARY)
        ship->secondaryCount+=mod->ammount;
      if(mod->affect == AFFECT_MISSILE)
        {
          ship->maxmissiles+=mod->ammount;
          ship->missiles=ship->maxmissiles;
        }
      if(mod->affect == AFFECT_ROCKET)
        {
          ship->maxrockets+=mod->ammount;
          ship->rockets=ship->maxrockets;
        }
      if(mod->affect == AFFECT_TORPEDO)
        {
          ship->maxtorpedos+=mod->ammount;
          ship->torpedos=ship->maxtorpedos;
        }
      if(mod->affect == AFFECT_HULL)
        ship->maxhull+=mod->ammount;
      if(mod->affect == AFFECT_SHIELD)
        ship->maxshield+=mod->ammount;
      if(mod->affect == AFFECT_SPEED)
        ship->realspeed+=mod->ammount;
      if(mod->affect == AFFECT_HYPER)
	ship->hyperspeed-=mod->ammount;
      if(mod->affect == AFFECT_ENERGY)
        ship->maxenergy+=mod->ammount;
      if(mod->affect == AFFECT_MANUEVER)
        ship->manuever+=mod->ammount;
      if(mod->affect == AFFECT_ALARM)
        ship->alarm+=mod->ammount;
      if(mod->affect == AFFECT_CHAFF)
        ship->maxchaff+=mod->ammount;
      if(mod->affect == AFFECT_SLAVE)
        ship->slave+=mod->ammount;
      if(mod->affect == AFFECT_TRACTOR)
        ship->tractorbeam+=mod->ammount;
      save_ship(ship);
      separate_obj( modobj );
      obj_from_char( modobj );
      extract_obj(modobj);
      ch->println("You finish installing your new module.\n\r", ch);

     {
      long xpgain;
      xpgain = ((ch->skill_level[TECHNICIAN_ABILITY]+1) * 300);
      gain_exp(ch, xpgain, TECHNICIAN_ABILITY);
      ch_printf( ch , " You gain %d experience for being a Technician.\n\r", xpgain );
      learn_from_success( ch, gsn_installmodule ) ;
     }
      return;
}

void do_shiplist(CHAR_DATA *ch, char *argument)
{
	int x;

        ch->println("\n\r&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w                             Complete ship listing                           &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);

        for(x=0;x<NUM_PROTOTYPES;x++)
            ch->printlnf( "&W|&w  &w(&W%2d&w) %s%-35.35s&W Type: %s %-15.15s &W   |\r\n", x+1,
            !str_cmp(ship_prototypes[x].clan, "The Empire") ? "&R" :
            !str_cmp(ship_prototypes[x].clan, "The New Republic") ? "&G" : "&w",
            ship_prototypes[x].name,
			!str_cmp(ship_prototypes[x].clan, "The Empire") ?        "&RImperial" :
			!str_cmp(ship_prototypes[x].clan, "The New Republic") ?  "&GRepublic" :
																	"&wCivilian",
		        ship_prototypes[x].class == SHIP_FIGHTER ? "Starfighter" :
		        ship_prototypes[x].class == SHIP_BOMBER  ? "Bomber" :
		        ship_prototypes[x].class == SHIP_SHUTTLE ? "Shuttle" :
		        ship_prototypes[x].class == SHIP_FREIGHTER ? "Freighter" :
		        ship_prototypes[x].class == SHIP_FRIGATE ? "Frigate" :
		        ship_prototypes[x].class == SHIP_TT ? "Troop Transport" :
		        ship_prototypes[x].class == SHIP_CORVETTE ? "Corvette" :
		        ship_prototypes[x].class == SHIP_CRUISER ? "Cruiser" :
			    ship_prototypes[x].class == SHIP_DREADNAUGHT ? "Dreadnaught" :
		        ship_prototypes[x].class == SHIP_DESTROYER ? "Star Destroyer" :
		        ship_prototypes[x].class == SHIP_SPACE_STATION ? "Space Station" :
		        ship_prototypes[x].class == LAND_VEHICLE ? "Land vehicle" : "Unknown");


        ch->println("&z+&W-----------------------------------------------------------------------------&z+\r\n",ch);
        ch->println("&W|&w               Use 'shipstat (number)' to get ship information               &W|\r\n",ch);
        ch->println("&z+&W-----------------------------------------------------------------------------&z+\n\r",ch);
      return;
}

*//*free up ship data by Keberus */
/*
void free_ship( SHIP_DATA *ship )
{
  MODULE_DATA *mod, *mod_next;
  TURRET_DATA *turret, *tnext;

    STRFREE( ship->clanowner );
    STRFREE( ship->copilot );
    STRFREE( ship->description );
    STRFREE( ship->home );
    STRFREE( ship->name );
    STRFREE( ship->owner );
    STRFREE( ship->pilot );
    DISPOSE( ship->filename );
    STRFREE( ship->protoname );
    STRFREE( ship->pbeacon );
    STRFREE( ship->dest );

*/  /*dispose of ship module data*/
/*  for( mod = ship->first_module; mod; mod = mod_next )
  {
     mod_next = mod->next;
     UNLINK( mod, ship->first_module, ship->last_module, next, prev );
     DISPOSE(mod);
  }

*/  /*dispose of ship turret data*/
/*  for( turret = ship->first_turret; turret; turret = tnext )
  {
     tnext = turret->next;
     UNLINK( turret, ship->first_turret, ship->last_turret, next, prev );
     DISPOSE(turret);
  }

  DISPOSE( ship );
  return;
}
*/


//space shit

SHIP_DATA * first_ship;
SHIP_DATA * last_ship;

MISSILE_DATA * first_missile;
MISSILE_DATA * last_missile;

SPACE_DATA * first_starsystem;
SPACE_DATA * last_starsystem;

extern char bname[MSL];
char *primary_beam_name    args ( ( SHIP_DATA *ship ) );
char *secondary_beam_name  args ( ( SHIP_DATA *ship ) );
void free_ship( SHIP_DATA *ship );

#ifdef USECARGO
extern char * const cargo_names[CARGO_MAX];
#endif

int bus_pos =0;
int bus_planet = 0;
int bus2_planet = 5;
int bus3_planet = 3;
int bus4_planet = 8;
int turbocar_stop =0;
int corus_shuttle =0;

#define MAX_STATION    10
#define MAX_BUS_STOP 10

#define STOP_PLANET     1057
#define STOP_SHIPYARD   1057


int     const   station_vnum [MAX_STATION] =
{
    215 , 216 , 217 , 218 , 219 , 220 , 221 ,222 , 223 , 224
};

char *  const   station_name [MAX_STATION] =
{
   "Menari Spaceport" , "Skydome Botanical Gardens" , "Grand Towers" ,
   "Grandis Mon Theater" , "Palace Station" , "Great Galactic Museum" ,
   "College Station" , "Holographic Zoo of Extinct Animals" ,
   "Dometown Station " , "Monument Plaza"
};

int     const   bus_vnum [MAX_BUS_STOP] =
{
    1053, 1850, 250, 4556, 28613, 9701, 7801, 9000, 10154, 18003
};

char *  const   bus_stop [MAX_BUS_STOP+1] =
{
   "Coruscant", "Yavin IV", "Tatooine", "Endor",
   "Kashyyyk", "Mon Calamari", "Nal Hutta", "Ord Mantell", "Corellia",
"Bakura", "Coruscant"  /* last should always be same as first */
};

char *  const   ship_flags [] =
{
"nohijack", "shield_boost", "torp_boost", "chaff_boost", "hull_boost",
"laser_boost", "missile_boost", "rocket_boost", "simulator", "nodestroy", "noslicer", "ion_lasers",
"ion_drive", "ion_ion", "ion_turret1", "ion_turret2", "ion_turret3",
"ion_turret4", "ion_turret5", "ion_turret6", "ion_turret7", "ion_turret8",
"ion_turret9", "ion_turret10", "respawn", "ion_misiles", "ion_hyper", "cloak", "r24",
"r25", "r26", "r27", "r28", "r29",
"r30", "r31"
};

int get_shipflag( char *flag )
{
    int x;

    for ( x = 0; x < 32; x++ )
      if ( !str_cmp( flag, ship_flags[x] ) )
        return x;
    return -1;
}

/* local routines */
void	fread_ship	args( ( SHIP_DATA *ship, FILE *fp ) );
bool	load_ship_file	args( ( char *shipfile ) );
void	write_ship_list	args( ( void ) );
void    fread_starsystem      args( ( SPACE_DATA *starsystem, FILE *fp ) );
bool    load_starsystem  args( ( char *starsystemfile ) );
void    write_starsystem_list args( ( void ) );
void    resetship args( ( SHIP_DATA *ship ) );
void    landship args( ( SHIP_DATA *ship, char *arg ) );
void    launchship args( ( SHIP_DATA *ship ) );
bool    land_bus args( ( SHIP_DATA *ship, int destination ) );
void    launch_bus args( ( SHIP_DATA *ship ) );
void    echo_to_room_dnr args( ( int ecolor , ROOM_INDEX_DATA *room ,  char *argument ) );
//ch_ret drive_ship( CHAR_DATA *ch, SHIP_DATA *ship, EXIT_DATA  *exit , int fall );
bool    autofly(SHIP_DATA *ship);
bool is_facing( SHIP_DATA *ship , SHIP_DATA *target );
void sound_to_ship( SHIP_DATA *ship , char *argument );
int	get_shipflag(char *flag );

/* from comm.c */
bool    write_to_descriptor     args( ( int desc, char *txt, int length ) );

ROOM_INDEX_DATA *generate_exit( ROOM_INDEX_DATA *in_room, EXIT_DATA **pexit );

void echo_to_room_dnr ( int ecolor , ROOM_INDEX_DATA *room ,  char *argument )
{
    char_data *vic;

    if ( room == NULL )
    	return;

    for ( vic = room->people; vic; vic = vic->next_in_room )
    {
//	//set_char_color( ecolor, vic );
	vic->printlnf( argument, vic );
    }
}

bool ship_to_room(SHIP_DATA *ship , int vnum )
{
    ROOM_INDEX_DATA *shipto;

    if ( (shipto=get_room_index(vnum)) == NULL )
            return false;

//    LINK( ship, shipto->ships, shipto->last_ship, next_in_room, prev_in_room );
    ship->in_room = shipto;
    return true;
}

void echo_to_ship( SHIP_DATA *ship , char *argument )
{
     int room;

     for ( room = ship->firstroom ; room <= ship->lastroom ;room++ )
     {
//         do_echo( get_room_index(room) , argument );
     }

}

void ship_from_starsystem( SHIP_DATA *ship , SPACE_DATA *starsystem )
{

     if ( starsystem == NULL )
        return;

     if ( ship == NULL )
        return;

     if ( starsystem->last_ship == ship )
        starsystem->last_ship = ship->prev_in_starsystem;

     if ( starsystem->first_ship == ship )
        starsystem->first_ship = ship->next_in_starsystem;

     if ( ship->prev_in_starsystem )
        ship->prev_in_starsystem->next_in_starsystem = ship->next_in_starsystem;

     if ( ship->next_in_starsystem)
        ship->next_in_starsystem->prev_in_starsystem = ship->prev_in_starsystem;

     ship->starsystem = NULL;
     ship->next_in_starsystem = NULL;
     ship->prev_in_starsystem = NULL;

}


bool  land_bus( SHIP_DATA *ship, int destination )
{
    char buf[MSL];

	if(ship)  extract_ship(ship);

    if ( !ship_to_room( ship , destination ) )
    {
       return false;
    }

    echo_to_ship( ship , "You feel a slight thud as the ship sets down on the ground.");
    ship->location = destination;
    ship->lastdoc = ship->location;
    ship->shipstate = SHIP_DOCKED;
    if (ship->starsystem)
        ship_from_starsystem( ship, ship->starsystem );
    sprintf( buf, "%s lands on the platform.", ship->name );
//    echo_to_room( get_room_index(ship->location) , buf );
    sprintf( buf , "The hatch on %s opens." , ship->name);
//    echo_to_room( AT_YELLOW , get_room_index(ship->location) , buf );
//    echo_to_room( AT_YELLOW , get_room_index(ship->entrance) , "The hatch opens." );
    ship->hatchopen = true;
//    sound_to_room( get_room_index(ship->entrance) , "!!SOUND(door)" );
//    sound_to_room( get_room_index(ship->location) , "!!SOUND(door)" );
    return true;
}

void    launch_bus( SHIP_DATA *ship )
{
      char buf[MSL];

//      sound_to_room( get_room_index(ship->entrance) , "!!SOUND(door)" );
//       sound_to_room( get_room_index(ship->location) , "!!SOUND(door)" );
      sprintf( buf , "The hatch on %s closes and it begins to launch." , ship->name);
//      echo_to_room( AT_YELLOW , get_room_index(ship->location) , buf );
//      echo_to_room( AT_YELLOW , get_room_index(ship->entrance) , "The hatch slides shut." );
      ship->hatchopen = false;
      extract_ship( ship );
//      echo_to_ship( AT_YELLOW , ship , "The ship begins to launch.");
      ship->location = 0;
      ship->shipstate = SHIP_READY;
}
/*
void update_traffic( )
{
    SHIP_DATA *shuttle;
    SHIP_DATA *turbocar;
    char       buf[MSL];

    shuttle = ship_from_cockpit( ROOM_CORUSCANT_SHUTTLE );
    if ( shuttle != NULL )
    {
        switch (corus_shuttle)
        {
             default:
                corus_shuttle++;
                break;

             case 0:
                land_bus( shuttle , STOP_PLANET );
                corus_shuttle++;
                echo_to_ship( AT_CYAN , shuttle , "Welcome to Bulova Spaceport." );
                break;

             case 4:
                launch_bus( shuttle );
                corus_shuttle++;
                break;

             case 5:
                land_bus( shuttle , STOP_SHIPYARD );
                echo_to_ship( AT_CYAN , shuttle , "Welcome to Coruscant Shipyard." );
                corus_shuttle++;
                break;

             case 9:
                launch_bus( shuttle );
                corus_shuttle++;
                break;

        }

        if ( corus_shuttle >= 10 )
              corus_shuttle = 0;
    }

    turbocar = ship_from_cockpit( ROOM_CORUSCANT_TURBOCAR );
    if ( turbocar != NULL )
    {
      	sprintf( buf , "The turbocar doors close and it speeds out of the station.");
      	echo_to_room( AT_YELLOW , get_room_index(turbocar->location) , buf );
      	extract_ship( turbocar );
      	turbocar->location = 0;
       	ship_to_room( turbocar , station_vnum[turbocar_stop] );
       	echo_to_ship( AT_YELLOW , turbocar , "The turbocar makes a quick journey to the next station.");
       	turbocar->location = station_vnum[turbocar_stop];
       	turbocar->lastdoc = turbocar->location;
       	turbocar->shipstate = SHIP_DOCKED;
       	if (turbocar->starsystem)
          ship_from_starsystem( turbocar, turbocar->starsystem );
    	sprintf( buf, "A turbocar pulls into the platform and the doors slide open.");
    	echo_to_room( AT_YELLOW , get_room_index(turbocar->location) , buf );
    	sprintf( buf, "Welcome to %s." , station_name[turbocar_stop] );
    	echo_to_ship( AT_CYAN , turbocar , buf );
        turbocar->hatchopen = true;

        turbocar_stop++;
        if ( turbocar_stop >= MAX_STATION )
           turbocar_stop = 0;
    }

}
*/
/*
void update_bus( )
{
    SHIP_DATA *ship;
    SHIP_DATA *ship2;
	SHIP_DATA *ship3;
	SHIP_DATA *ship4;
    SHIP_DATA *target;
    int        destination;
    char       buf[MAX_STRING_LENGTH];

    ship = ship_from_cockpit( ROOM_SHUTTLE_BUS );
    ship2 = ship_from_cockpit( ROOM_SHUTTLE_BUS_2 );
	ship3 = ship_from_cockpit( ROOM_SHUTTLE_BUS_3 );
	ship4 = ship_from_cockpit( ROOM_SHUTTLE_BUS_4 );

    if ( ship == NULL )
     {
	bug("update_bus: Ship is NULL");
    	return;
     }
    else if ( ship2 == NULL )
     {
	bug("update_bus: Ship2 is NULL");
    	return;
     }
    if ( ship3 == NULL )
     {
	bug("update_bus: Ship3 is NULL");
    	return;
     }
    if ( ship4 == NULL )
     {
	bug("update_bus: Ship4 is NULL");
    	return;
     }

    switch (bus_pos)
    {

       case 0:
            target = ship_from_hanger( bus_vnum[bus_planet] );
            if ( target != NULL && !target->starsystem )
            {
               sprintf( buf,  "An electronic voice says, 'Cannot land at %s ... it seems to have dissapeared.'", bus_stop[bus_planet] );
               echo_to_ship( AT_CYAN , ship , buf );
               bus_pos = 5;
            }

            target = ship_from_hanger( bus_vnum[bus2_planet] );
            if ( target != NULL && !target->starsystem )
            {
               sprintf( buf,  "An electronic voice says, 'Cannot land at %s ... it seems to have dissapeared.'", bus_stop[bus_planet] );
               echo_to_ship( AT_CYAN , ship2 , buf );
               bus_pos = 5;
            }
			target = ship_from_hanger(bus_vnum[bus3_planet]);
			if(target != NULL && !target->starsystem)
		{
				sprintf(buf, "An electronic voice says, 'Cannot land at %s ... it seems to have disappeared.'", bus_stop[bus_planet]);
				echo_to_ship(AT_CYAN, ship3, buf);
				bus_pos = 5;
		}
			target = ship_from_hanger(bus_vnum[bus4_planet]);
			if(target != NULL && !target->starsystem)
		{
				sprintf(buf, "An electronic voice says, 'Cannot land at %s ... it seems to have disappeared.'", bus_stop[bus_planet]);
				echo_to_ship(AT_CYAN, ship4, buf);
				bus_pos = 5;
		}
            bus_pos++;
            break;

       case 6:
            launch_bus ( ship );
            launch_bus ( ship2 );
			launch_bus ( ship3 );
			launch_bus ( ship4 );
            bus_pos++;
            break;

       case 7:
            echo_to_ship( AT_YELLOW , ship , "The ship lurches slightly as it makes the jump to lightspeed.");
            echo_to_ship( AT_YELLOW , ship2 , "The ship lurches slightly as it makes the jump to lightspeed.");
		    echo_to_ship( AT_YELLOW , ship3 , "The ship lurches slightly as it makes the jump to lightspeed.");
			echo_to_ship( AT_YELLOW , ship4 , "The ship lurches slightly as it makes the jump to lightspeed.");

            bus_pos++;
            break;

       case 9:

            echo_to_ship( AT_YELLOW , ship , "The ship lurches slightly as it comes out of hyperspace..");
            echo_to_ship( AT_YELLOW , ship2 , "The ship lurches slightly as it comes out of hyperspace..");
			echo_to_ship( AT_YELLOW , ship3 , "The ship lurches slightly as it comes out of hyperspace..");
			echo_to_ship( AT_YELLOW , ship4 , "The ship lurches slightly as it comes out of hyperspace..");
            bus_pos++;
            break;

       case 1:
            destination = bus_vnum[bus_planet];
            if ( !land_bus( ship, destination ) )
            {
               sprintf( buf, "An electronic voice says, 'Oh My, %s seems to have dissapeared.'" , bus_stop[bus_planet] );
               echo_to_ship( AT_CYAN , ship , buf );
               echo_to_ship( AT_CYAN , ship , "An electronic voice says, 'I do hope it wasn't a superlaser. Landing aborted.'");
            }
            else
            {
               sprintf( buf,  "An electronic voice says, 'Welcome to %s'" , bus_stop[bus_planet] );
               echo_to_ship( AT_CYAN , ship , buf);
               echo_to_ship( AT_CYAN , ship , "It continues, 'Please exit through the main ramp. Enjoy your stay.'");
            }

            destination = bus_vnum[bus2_planet];
            if ( !land_bus( ship2, destination ) )
            {
               sprintf( buf, "An electronic voice says, 'Oh My, %s seems to have dissapeared.'" , bus_stop[bus_planet] );
               echo_to_ship( AT_CYAN , ship2 , buf );
               echo_to_ship( AT_CYAN , ship2 , "An electronic voice says, 'I do hope it wasn't a superlaser. Landing aborted.'");
            }
            else
            {
               sprintf( buf,  "An electronic voice says, 'Welcome to %s'" , bus_stop[bus2_planet] );
               echo_to_ship( AT_CYAN , ship2 , buf);
               echo_to_ship( AT_CYAN , ship2 , "It continues, 'Please exit through the main ramp. Enjoy your stay.'");
            }

			destination = bus_vnum[bus3_planet];
            if ( !land_bus( ship3, destination ) )
            {
               sprintf( buf, "An electronic voice says, 'Oh My, %s seems to have dissapeared.'" , bus_stop[bus3_planet] );
               echo_to_ship( AT_CYAN , ship3 , buf );
               echo_to_ship( AT_CYAN , ship3 , "An electronic voice says, 'I do hope it wasn't a superlaser. Landing aborted.'");
            }
            else
            {
               sprintf( buf,  "An electronic voice says, 'Welcome to %s'" , bus_stop[bus3_planet] );
               echo_to_ship( AT_CYAN , ship3 , buf);
               echo_to_ship( AT_CYAN , ship3 , "It continues, 'Please exit through the main ramp. Enjoy your stay.'");
            }

			destination = bus_vnum[bus4_planet];
            if ( !land_bus( ship4, destination ) )
            {
               sprintf( buf, "An electronic voice says, 'Oh My, %s seems to have dissapeared.'" , bus_stop[bus4_planet] );
               echo_to_ship( AT_CYAN , ship4 , buf );
               echo_to_ship( AT_CYAN , ship4 , "An electronic voice says, 'I do hope it wasn't a superlaser. Landing aborted.'");
            }
            else
            {
               sprintf( buf,  "An electronic voice says, 'Welcome to %s'" , bus_stop[bus4_planet] );
               echo_to_ship( AT_CYAN , ship4 , buf);
               echo_to_ship( AT_CYAN , ship4 , "It continues, 'Please exit through the main ramp. Enjoy your stay.'");
            }

            bus_pos++;
            break;

       case 5:
            sprintf( buf, "It continues, 'Next stop, %s'" , bus_stop[bus_planet+1] );
            echo_to_ship( AT_CYAN , ship , "An electronic voice says, 'Preparing for launch.'");
            echo_to_ship( AT_CYAN , ship , buf);

            sprintf( buf, "It continues, 'Next stop, %s'" , bus_stop[bus2_planet+1] );
            echo_to_ship( AT_CYAN , ship2 , "An electronic voice says, 'Preparing for launch.'");
            echo_to_ship( AT_CYAN , ship2 , buf);

			sprintf( buf, "It continues, 'Next stop, %s'" , bus_stop[bus3_planet+1] );
            echo_to_ship( AT_CYAN , ship3 , "An electronic voice says, 'Preparing for launch.'");
            echo_to_ship( AT_CYAN , ship3 , buf);

			sprintf( buf, "It continues, 'Next stop, %s'" , bus_stop[bus4_planet+1] );
            echo_to_ship( AT_CYAN , ship4 , "An electronic voice says, 'Preparing for launch.'");
            echo_to_ship( AT_CYAN , ship4 , buf);

            bus_pos++;
            break;

       default:
            bus_pos++;
            break;
    }

    if ( bus_pos >= 10 )
    {
       bus_pos = 0;
       bus_planet++;
       bus2_planet++;
	   bus3_planet++;
	   bus4_planet++;
    }

    if ( bus_planet >= MAX_BUS_STOP )
       bus_planet = 0;
    if ( bus2_planet >= MAX_BUS_STOP )
       bus2_planet = 0;
	if ( bus3_planet >= MAX_BUS_STOP)
	   bus3_planet = 0;
	if(bus4_planet >= MAX_BUS_STOP)
	   bus4_planet = 0;

}
*/

/*
void //echo_to_cockpit( int color , SHIP_DATA *ship , char *argument )
{
     int room;

     for ( room = ship->firstroom ; room <= ship->lastroom ;room++ )
     {
         if ( room == ship->cockpit || room == ship->navseat
         || room == ship->pilotseat || room == ship->coseat
         || room == ship->gunseat || room == ship->engineroom
         || room == ship->turret1 || room == ship->turret2
	 || room == ship->turret3 || room == ship->turret4
	 || room == ship->turret5 || room == ship->turret6
	 || room == ship->turret7 || room == ship->turret8
	 || room == ship->turret9 || room == ship->turret10  )
//               echo_to_room( color , get_room_index(room) , argument );
     }

}
*/

/*
void echo_to_system( int color , SHIP_DATA *ship , char *argument , SHIP_DATA *ignore )
{
     SHIP_DATA *target;

     if (!ship->starsystem)
        return;

     for ( target = ship->starsystem->first_ship; target; target = target->next_in_starsystem )
     {
       if (target != ship && target != ignore )
//         //echo_to_cockpit( color , target , argument );
     }

}
*/

void extract_missile( MISSILE_DATA *missile )
{
    SPACE_DATA *starsystem;

     if ( missile == NULL )
        return;

     if ( ( starsystem = missile->starsystem ) != NULL )
     {

      if ( starsystem->last_missile == missile )
        starsystem->last_missile = missile->prev_in_starsystem;

      if ( starsystem->first_missile == missile )
        starsystem->first_missile = missile->next_in_starsystem;

      if ( missile->prev_in_starsystem )
        missile->prev_in_starsystem->next_in_starsystem = missile->next_in_starsystem;

      if ( missile->next_in_starsystem)
        missile->next_in_starsystem->prev_in_starsystem = missile->prev_in_starsystem;

      missile->starsystem = NULL;
      missile->next_in_starsystem = NULL;
      missile->prev_in_starsystem = NULL;

     }

     UNLINK( missile, first_missile, last_missile, next, prev );

     missile->target = NULL;
     missile->fired_from = NULL;
//     if (  missile->fired_by )
//        STRFREE( missile->fired_by );

//     DISPOSE( missile );

}

void save_ship( SHIP_DATA *ship )
{
    FILE *fp;
    char filename[256];
    char buf[MSL];
    MODULE_DATA *mod;

    if ( !ship )
    {
	bug( "save_ship: null ship pointer!" );
	return;
    }

    if ( !ship->filename || ship->filename[0] == '\0' )
    {
	sprintf( buf, "save_ship: %s has no filename", ship->name );
	bug( buf );
	return;
    }

    sprintf( filename, "%s%s", SHIPS_DIR, ship->filename );

    fclose( fpReserve );
    if ( ( fp = fopen( filename, "w" ) ) == NULL )
    {
    	bug( "save_ship: fopen" );
    	perror( filename );
    }
    else
    {
	fprintf( fp, "#SHIP\n" );
	fprintf( fp, "Name         %s~\n",	ship->name		);
	fprintf( fp, "Filename     %s~\n",	ship->filename		);
	fprintf( fp, "Protoname    %s~\n",	ship->protoname		);
	fprintf( fp, "PBeacon      %s~\n",	ship->pbeacon);
	fprintf( fp, "ClanOwner    %s~\n",      ship->clanowner		);
        fprintf( fp, "Description  %s~\n",	ship->description	);
	fprintf( fp, "Owner        %s~\n",	ship->owner		);
	fprintf( fp, "Pilot        %s~\n",      ship->pilot             );
	fprintf( fp, "Copilot      %s~\n",      ship->copilot           );
	fprintf( fp, "Class        %d\n",	ship->ship_class		);
	fprintf( fp, "primaryType        %d\n", ship->primaryType );
	fprintf( fp, "primaryCount       %d\n",	ship->primaryCount    	);
	fprintf( fp, "secondaryType      %d\n", ship->secondaryType  );
	fprintf( fp, "secondaryCount	 %d\n",	ship->secondaryCount		);
        fprintf( fp, "Alarm        %d\n",	ship->alarm		);
        fprintf( fp, "Maxmod       %d\n",	ship->maxmod		);
	fprintf( fp, "Tractorbeam  %d\n",	ship->tractorbeam	);
	fprintf( fp, "Shipyard     %d\n",	ship->shipyard		);
	fprintf( fp, "Turret1      %d\n",	ship->turret1		);
	fprintf( fp, "Turret2      %d\n",	ship->turret2		);
	fprintf( fp, "Turret3      %d\n",	ship->turret3		);
	fprintf( fp, "Turret4	   %d\n",	ship->turret4		);
	fprintf( fp, "Turret5	   %d\n",   ship->turret5		);
	fprintf( fp, "Turret6      %d\n",   ship->turret6       );
	fprintf( fp, "Turret7      %d\n",   ship->turret7		);
	fprintf( fp, "Turret8      %d\n",   ship->turret8       );
	fprintf( fp, "Turret9      %d\n",   ship->turret9       );
	fprintf( fp, "Turret10     %d\n",   ship->turret10      );
	fprintf( fp, "Statet1      %d\n",	ship->statet1		);
	fprintf( fp, "Statet2      %d\n",	ship->statet2		);
	fprintf( fp, "Statet3      %d\n",	ship->statet3		);
	fprintf( fp, "Statet4      %d\n",	ship->statet4		);
	fprintf( fp, "Statet5      %d\n",	ship->statet5		);
	fprintf( fp, "Statet6      %d\n",	ship->statet6		);
	fprintf( fp, "Statet7      %d\n",	ship->statet7		);
	fprintf( fp, "Statet8      %d\n",	ship->statet8		);
	fprintf( fp, "Statet9      %d\n",	ship->statet9		);
	fprintf( fp, "Statet10     %d\n",	ship->statet10		);
	fprintf( fp, "Bombs	   %d\n",	ship->bombs		);
	fprintf( fp, "Hibombstr	   %d\n",	ship->hibombstr		);
	fprintf( fp, "lowbombstr   %d\n",	ship->lowbombstr	);
	fprintf( fp, "Maxbombs     %d\n",	ship->maxbombs		);
	fprintf( fp, "Missiles     %d\n",	ship->missiles		);
	fprintf( fp, "Maxmissiles  %d\n",	ship->maxmissiles	);
	fprintf( fp, "Rockets     %d\n",	ship->rockets		);
	fprintf( fp, "Maxrockets  %d\n",	ship->maxrockets	);
	fprintf( fp, "Torpedos     %d\n",	ship->torpedos		);
	fprintf( fp, "Maxtorpedos  %d\n",	ship->maxtorpedos	);
	fprintf( fp, "Lastdoc      %d\n",	ship->lastdoc		);
	fprintf( fp, "Firstroom    %d\n",	ship->firstroom		);
	fprintf( fp, "Lastroom     %d\n",	ship->lastroom		);
	fprintf( fp, "Hanger1      %d\n",   ship->hanger1		);
	fprintf( fp, "Hanger2      %d\n",   ship->hanger2		);
	fprintf( fp, "Hanger3      %d\n",   ship->hanger3		);
	fprintf( fp, "Hanger4      %d\n",   ship->hanger4		);
	fprintf( fp, "Shield       %d\n",	ship->shield		);
	fprintf( fp, "Maxshield    %d\n",	ship->maxshield		);
	fprintf( fp, "Hull         %d\n",	ship->hull		);
	fprintf( fp, "Maxhull      %d\n",	ship->maxhull		);
	fprintf( fp, "Maxenergy    %d\n",	ship->maxenergy		);
	fprintf( fp, "Hyperspeed   %d\n",	ship->hyperspeed	);
	fprintf( fp, "Comm         %d\n",	ship->comm		);
	fprintf( fp, "Cost	   %d\n",       ship->cost		);
	fprintf( fp, "Chaff        %d\n",	ship->chaff		);
	fprintf( fp, "Maxchaff     %d\n",	ship->maxchaff		);
	fprintf( fp, "Sensor       %d\n",	ship->sensor		);
	fprintf( fp, "Astro_array  %d\n",	ship->astro_array	);
	fprintf( fp, "Realspeed    %d\n",	ship->realspeed		);
	fprintf( fp, "Type         %d\n",	ship->type		);
	fprintf( fp, "Cockpit      %d\n",	ship->cockpit		);
	fprintf( fp, "Coseat       %d\n",	ship->coseat		);
	fprintf( fp, "Pilotseat    %d\n",	ship->pilotseat		);
	fprintf( fp, "Gunseat      %d\n",	ship->gunseat		);
	fprintf( fp, "Navseat      %d\n",	ship->navseat		);
	fprintf( fp, "Engineroom   %d\n",       ship->engineroom        );
	fprintf( fp, "Entrance     %d\n",       ship->entrance          );
	fprintf( fp, "Shipstate    %d\n",	ship->shipstate		);
	fprintf( fp, "Missilestate %d\n",	ship->missilestate	);
	fprintf( fp, "Energy       %d\n",	ship->energy		);
	fprintf( fp, "Manuever     %d\n",       ship->manuever          );
	fprintf( fp, "Password     %d\n",       ship->password          );
#ifdef USECARGO
	fprintf( fp, "MaxCargo     %d\n",       ship->maxcargo          );
	if (ship->cargo > 0)
	{
	   fprintf( fp, "Cargo     %d\n",       ship->cargo          );
	   fprintf( fp, "CargoType %d\n",       ship->cargotype          );
	}
#endif
	fprintf( fp, "Home         %s~\n",      ship->home              );
        for(mod=ship->first_module;mod;mod=mod->next)
          fprintf( fp, "Module     %d %d\n",    mod->affect, mod->ammount);

    fprintf( fp, "Simulator    %d %d\n",
				IS_SET(ship->flags, SHIP_SIMULATOR) ? 1 : 0,
				IS_SET(ship->flags, SHIP_SIMULATOR) ? ship->sim_vnum : 0);

	 fprintf( fp, "End\n\n"						 );
	fprintf( fp, "#END\n"						);
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

/*




void destroy_ship( SHIP_DATA *ship , char_data *ch, char *reason )
{
    char buf[MSL];
    int  roomnum;
    ROOM_INDEX_DATA *room;
    OBJ_DATA *robj;
    char_data *rch;
    int vnum,size;
    ROOM_INDEX_DATA *sroom;
    char shiplog[MSL];
    AREA_DATA *tarea;
    SHIP_DATA *hship;

    sprintf( buf , "%s explodes in a blinding flash of light!", ship->name );
//    echo_to_system( AT_WHITE + AT_BLINK , ship , buf , NULL );
   if(!IS_SET(ship->flags, SHIP_SIMULATOR))
   {
    sprintf(buf, "%s destroyed by %s: %s.\n\r", ship->name,
    ch ? ch->name : "No CH", reason);
    log_string(buf);
//    echo_to_ship( AT_WHITE + AT_BLINK , ship , "A blinding flash of light burns your eyes...");
//    echo_to_ship( AT_WHITE , ship , "But before you have a chance to scream...\n\rYou are ripped apart as your spacecraft explodes...");
   }
   else
   {
    if(ship->type != MOB_SHIP)
//     echo_to_ship( AT_WHITE, ship, "The ship has sustained critical damage, and was destroyed.\n\r");
//    else
    {
     ship->shipstate = SHIP_READY;
     ship->currspeed=0;
     ship->energy=ship->maxenergy;
     ship->chaff=ship->maxchaff;
     ship->hull=ship->maxhull;
     ship->shield=ship->maxshield;

     ship->statet1 = LASER_READY;
     ship->statet2 = LASER_READY;
	 ship->statet3 = LASER_READY;
	 ship->statet4 = LASER_READY;
	 ship->statet5 = LASER_READY;
	 ship->statet6 = LASER_READY;
	 ship->statet7 = LASER_READY;
	 ship->statet8 = LASER_READY;
	 ship->statet9 = LASER_READY;
	 ship->statet10 = LASER_READY;

     ship->primaryState = LASER_READY;
	 ship->secondaryState = LASER_READY;
     ship->missilestate = LASER_READY;

     ship->currjump=NULL;
     ship->target0=NULL;
     ship->target1=NULL;
     ship->target2=NULL;

     ship->hatchopen = false;
     ship->bayopen = false;

     ship->missiles = ship->maxmissiles;
     ship->torpedos = ship->maxtorpedos;
     ship->rockets = ship->maxrockets;
     ship->autorecharge = false;
     ship->autotrack = false;
     ship->autospeed = false;
     ship->autopilot = true;
     ship->vx = number_range(-2000, 2000);
     ship->vy = number_range(-2000, 2000);
     ship->vz = number_range(-2000, 2000);
     return;
    }
   }

    resetship(ship);

    for ( roomnum = ship->firstroom ; roomnum <= ship->lastroom ; roomnum++ )
    {
        room = get_room_index(roomnum);
        if (room != NULL)
        {
//         rch = room->first_person;
        rch = room->people;
         while ( rch )
         {
            if( IS_SET( ship->flags, SHIP_SIMULATOR ) )
            {
                 resetship(ship);
	         ship->shipyard = ship->sim_vnum;
	  	 ship->shipstate = SHIP_READY;
	         extract_ship( ship );
        	 ship_to_room( ship, ship->shipyard );
          	 ship->location = ship->shipyard;
          	 ship->lastdoc = ship->shipyard;
          	 ship->shipstate = SHIP_DOCKED;
          	 if (ship->starsystem)
        	   ship_from_starsystem( ship, ship->starsystem );
                 save_ship(ship);
          	 rch->println("Red lights flash around the cockpit, and the simulation ends.\n\r");

           	 return;
            }
            else if ( IS_IMMORTAL(rch) )
            {
                 rch->println("You are saved from the explosion because of your Immortality.\r\n");
                 char_from_room(rch);
                 char_to_room( rch, get_room_index(100) );
            }
            else
            {
              if ( ch )
                  raw_kill( ch , rch );
               else
                  raw_kill( rch , rch );
            }
            sprintf(buf, "%s killed in space, onboard %s.", rch->name, ship->name);
            log_string(buf);
            rch = room->people;
         }
         for ( robj = room->contents ; robj ; robj = robj->next_content )
         {
//           separate_obj( robj );
           extract_obj( robj );
         }
        }

    }
*/    /* Ships in the hanger of the ship *//*
    for ( hship = first_ship; hship; hship = hship->next )
    {
        if(hship->location >= ship->firstroom && hship->location <= ship->lastroom && hship->location != 0)
	    destroy_ship(hship, NULL, "in hanger of destroyed ship.");
    }

    if ( !IS_SET( ship->flags, SHIP_RESPAWN ) && !IS_SET( ship->flags, SHIP_NODESTROY ) )
    {
        resetship(ship);
        size = ship->lastroom+1;
        sroom = get_room_index(ship->firstroom);
        if(sroom != NULL)
        {
            tarea = sroom->area;
            for(vnum=ship->firstroom;vnum<size;vnum++)
            {
                sroom = get_room_index( vnum );
//                wipe_resets(sroom);
//                delete_room(sroom);
            }
//            fold_area( tarea, tarea->filename, false );
        }
        sprintf(shiplog,"Ship Deleted: %s",ship->name);
        log_string(shiplog);
        extract_ship( ship );
        ship_to_room( ship , 46 );
	ship->location = 46;
	ship->shipyard = 46;
        if (ship->starsystem)
            ship_from_starsystem( ship, ship->starsystem );

         sprintf( buf, "%s%s", SHIPS_DIR, ship->filename );
         remove(buf);

        UNLINK( ship, first_ship, last_ship, next, prev );
//        free_ship( ship );

        write_ship_list();
    }
}



void damage_ship( SHIP_DATA *ship , int min , int max )
{
    int damage , shield_dmg;

    damage = number_range( min , max );

    if ( ship->shield > 0 )
    {
        shield_dmg = UMIN( ship->shield , damage );
    	damage -= shield_dmg;
    	ship->shield -= shield_dmg;
//    	if ( ship->shield == 0 )
//    	  //echo_to_cockpit( AT_BLOOD , ship , "&G[&gShip Computer&G]&w Warning. Shields down." );
    }

    if ( damage > 0 )
    {

        if ( number_range(1, 100) <= 5 && ship->shipstate != SHIP_DISABLED )
        {
//           //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Ship drive disabled." );
           ship->shipstate = SHIP_DISABLED;
        }

        if ( number_range(1, 100) <= 5 && ship->missilestate != MISSILE_DAMAGED && ship->maxmissiles > 0 )
        {
//           echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->gunseat) , "&G[&gShip Computer&G]&w Warning. Missile launcher offline." );
           ship->missilestate = MISSILE_DAMAGED;
        }

        if ( number_range(1, 100) <= 2 && ship->statet1 != LASER_DAMAGED && ship->turret1 )
        {
//           echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret1) , "&G[&gShip Computer&G]&w Warning. Turret offline." );
           ship->statet1 = LASER_DAMAGED;
        }

        if ( number_range(1, 100) <= 2 && ship->statet2 != LASER_DAMAGED && ship->turret2 )
        {
//           echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret2) , "&G[&gShip Computer&G]&w Warning. Turret offline." );
           ship->statet2 = LASER_DAMAGED;
        }

    }

    ship->hull -= damage*5;

    if ( ship->hull <= 0 )
    {
       destroy_ship( ship , NULL, "excessive hull damage" );
       return;
    }

//    if ( ship->hull <= ship->maxhull/20 )
//       //echo_to_cockpit( AT_BLOOD+ AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Ship hull critically damaged." );

}


void move_ships( )
{
   SHIP_DATA *ship;
   MISSILE_DATA *missile;
   MISSILE_DATA *m_next;
   SHIP_DATA *target;
   float dx, dy, dz, change;
   char buf[MSL];
   char_data *ch;
   bool ch_found = false;

   for ( missile = first_missile; missile; missile = m_next )
   {
           m_next = missile->next;

              ship = missile->fired_from;
              target = missile->target;
              if (!target) {  extract_missile (missile);
              continue;
              }

           if ( target->starsystem && target->starsystem == missile->starsystem )
              {

           if ( missile->mx < target->vx )
                  missile->mx += UMIN( missile->speed/5 , target->vx - missile->mx );
                else if ( missile->mx > target->vx )
                  missile->mx -= UMIN( missile->speed/5 , missile->mx - target->vx );
                if ( missile->my < target->vy )
                  missile->my += UMIN( missile->speed/5 , target->vy - missile->my );
                else if ( missile->my > target->vy )
                  missile->my -= UMIN( missile->speed/5 , missile->my - target->vy );
                if ( missile->mz < target->vz )
                  missile->mz += UMIN( missile->speed/5 , target->vz - missile->mz );
                else if ( missile->mz > target->vz )
                  missile->mz -= UMIN( missile->speed/5 , missile->mz - target->vz );

                if ( abs(missile->mx) - abs(target->vx) <= 20 && abs(missile->mx) - abs(target->vx) >= -20
                && abs(missile->my) - abs(target->vy) <= 20 && abs(missile->my) - abs(target->vy) >= -20
                && abs(missile->mz) - abs(target->vz) <= 20 && abs(missile->mz) - abs(target->vz) >= -20 )
                {
            if ( target->chaff_released <= 0)
            {
	            if ( target->manuever/2 < number_range(1, 100) )
				{
					if(target->juking == true)
					{
					 if(number_range(1, 11) != 11)
						{
//						 echo_to_room(AT_YELLOW, get_room_index(ship->gunseat), "Your missile barely misses its target." );
//						 //echo_to_cockpit(AT_ORANGE, target, "An incoming missile is barely evaded.");
						 sprintf(buf, "%s quickly evades an incoming missile.", target->name);
//						 echo_to_system(AT_ORANGE, target, buf, ship);
						 extract_missile(missile);
						}
					}
					else
					{

//                    	echo_to_room( AT_YELLOW , get_room_index(ship->gunseat), "Your missile hits its target dead on!" );
//                    	//echo_to_cockpit( AT_BLOOD, target, "The ship is hit by a missile.");
//                    	echo_to_ship( AT_RED , target , "A loud explosion shakes the ship violently!" );
                    	sprintf( buf, "You see a small explosion as %s is hit by a missile" , target->name );
//                    	echo_to_system( AT_ORANGE , target , buf , ship );
*//*                    	for ( ch = first_char; ch; ch = ch->next )
                      	if ( !IS_NPC( ch ) && is_name( missile->fired_by, ch->name ) )
                     	{
                   	     ch_found = true;
                    	   	damage_ship_ch( target , 20+missile->missiletype*missile->missiletype*20 ,
                    	       30+missile->missiletype*missile->missiletype*missile->missiletype*30 , ch );
                      	}
                    	if ( !ch_found )*//*
                       	damage_ship( target , 20+missile->missiletype*missile->missiletype*20 ,
                       	    30+missile->missiletype*missile->missiletype*ship->missiletype*30 );
                    	extract_missile( missile );
						}
                  }
			  }
                   else
                   {
//                    echo_to_room( AT_YELLOW , get_room_index(ship->gunseat), "Your missile explodes harmlessly in a cloud of chaff!" );
//                    //echo_to_cockpit( AT_YELLOW, target, "A missile explodes in your chaff.");
                    extract_missile( missile );
                   }
                   continue;
                }

               missile->age++;
               if (missile->age >= 50)
               {
                 extract_missile( missile );
                 continue;
               }
              }
              else
              {
                extract_missile( missile );
                continue;
              }

   }

   for ( ship = first_ship; ship; ship = ship->next )
   {

     if ( !ship || !ship->starsystem )
        continue;

     if ( ship->currspeed > 0 )
        {

          change = sqrt( ship->hx*ship->hx + ship->hy*ship->hy + ship->hz*ship->hz );

           if (change > 0)
           {
             dx = ship->hx/change;
             dy = ship->hy/change;
             dz = ship->hz/change;
             ship->vx += (dx * ship->currspeed/5);
             ship->vy += (dy * ship->currspeed/5);
             ship->vz += (dz * ship->currspeed/5);
           }

        }

        if ( autofly(ship) )
           continue;

*//*
          if ( ship->ship_class != SHIP_SPACE_STATION && !autofly(ship) )
          {
            if ( ship->starsystem->star1 && strcmp(ship->starsystem->star1,"") )
            {
              if (ship->vx >= ship->starsystem->s1x + 1 || ship->vx <= ship->starsystem->s1x - 1 )
                ship->vx -= URANGE(-3,(ship->starsystem->gravitys1)/(ship->vx - ship->starsystem->s1x)/2,3);
              if (ship->vy >= ship->starsystem->s1y + 1 || ship->vy <= ship->starsystem->s1y - 1 )
                ship->vy -= URANGE(-3,(ship->starsystem->gravitys1)/(ship->vy - ship->starsystem->s1y)/2,3);
              if (ship->vz >= ship->starsystem->s1z + 1 || ship->vz <= ship->starsystem->s1z - 1 )
                ship->vz -= URANGE(-3,(ship->starsystem->gravitys1)/(ship->vz - ship->starsystem->s1z)/2,3);
            }

            if ( ship->starsystem->star2 && strcmp(ship->starsystem->star2,"") )
            {
              if (ship->vx >= ship->starsystem->s2x + 1 || ship->vx <= ship->starsystem->s2x - 1 )
                ship->vx -= URANGE(-3,(ship->starsystem->gravitys2)/(ship->vx - ship->starsystem->s2x)/2,3);
              if (ship->vy >= ship->starsystem->s2y + 1 || ship->vy <= ship->starsystem->s2y - 1 )
                ship->vy -= URANGE(-3,(ship->starsystem->gravitys2)/(ship->vy - ship->starsystem->s2y)/2,3);
              if (ship->vz >= ship->starsystem->s2z + 1 || ship->vz <= ship->starsystem->s2z - 1 )
                ship->vz -= URANGE(-3,(ship->starsystem->gravitys2)/(ship->vz - ship->starsystem->s2z)/2,3);
            }

            if ( ship->starsystem->planet1 && strcmp(ship->starsystem->planet1,"") )
            {
              if (ship->vx >= ship->starsystem->p1x + 1 || ship->vx <= ship->starsystem->p1x - 1 )
                ship->vx -= URANGE(-3,(ship->starsystem->gravityp1)/(ship->vx - ship->starsystem->p1x)/2,3);
              if (ship->vy >= ship->starsystem->p1y + 1 || ship->vy <= ship->starsystem->p1y - 1 )
                ship->vy -= URANGE(-3,(ship->starsystem->gravityp1)/(ship->vy - ship->starsystem->p1y)/2,3);
              if (ship->vz >= ship->starsystem->p1z + 1 || ship->vz <= ship->starsystem->p1z - 1 )
                ship->vz -= URANGE(-3,(ship->starsystem->gravityp1)/(ship->vz - ship->starsystem->p1z)/2,3);
            }

            if ( ship->starsystem->planet2 && strcmp(ship->starsystem->planet2,"") )
            {
              if (ship->vx >= ship->starsystem->p2x + 1 || ship->vx <= ship->starsystem->p2x - 1 )
                ship->vx -= URANGE(-3,(ship->starsystem->gravityp2)/(ship->vx - ship->starsystem->p2x)/2,3);
              if (ship->vy >= ship->starsystem->p2y + 1 || ship->vy <= ship->starsystem->p2y - 1 )
                ship->vy -= URANGE(-3,(ship->starsystem->gravityp2)/(ship->vy - ship->starsystem->p2y)/2,3);
              if (ship->vz >= ship->starsystem->p2z + 1 || ship->vz <= ship->starsystem->p2z - 1 )
                ship->vz -= URANGE(-3,(ship->starsystem->gravityp2)/(ship->vz - ship->starsystem->p2z)/2,3);
            }

            if ( ship->starsystem->planet3 && strcmp(ship->starsystem->planet3,"") )
            {
              if (ship->vx >= ship->starsystem->p3x + 1 || ship->vx <= ship->starsystem->p3x - 1 )
                ship->vx -= URANGE(-3,(ship->starsystem->gravityp3)/(ship->vx - ship->starsystem->p3x)/2,3);
              if (ship->vy >= ship->starsystem->p3y + 1 || ship->vy <= ship->starsystem->p3y - 1 )
                ship->vy -= URANGE(-3,(ship->starsystem->gravityp3)/(ship->vy - ship->starsystem->p3y)/2,3);
              if (ship->vz >= ship->starsystem->p3z + 1 || ship->vz <= ship->starsystem->p3z - 1 )
                ship->vz -= URANGE(-3,(ship->starsystem->gravityp3)/(ship->vz - ship->starsystem->p3z)/2,3);
            }
          }

*/
/*
          for ( target = ship->starsystem->first_ship; target; target = target->next_in_starsystem)
          {
                if ( target != ship &&
                    abs(ship->vx - target->vx) < 1 &&
                    abs(ship->vy - target->vy) < 1 &&
                    abs(ship->vz - target->vz) < 1 )
                {
                    ship->collision = target->maxhull;
                    target->collision = ship->maxhull;
                }
          }
*//*
          if ( ship->starsystem->star1 && strcmp(ship->starsystem->star1,"") &&
                     abs(ship->vx - ship->starsystem->s1x) < 10 &&
                     abs(ship->vy - ship->starsystem->s1y) < 10 &&
                     abs(ship->vz - ship->starsystem->s1z) < 10 )
                {
//                    //echo_to_cockpit( AT_BLOOD+AT_BLINK, ship, "You fly directly into the sun.");
                    sprintf( buf , "%s flies directly into %s!", ship->name, ship->starsystem->star1);
//                    echo_to_system( AT_ORANGE , ship , buf , NULL );
                    destroy_ship(ship, NULL, "flew into sun");
                    continue;
                }
          if ( ship->starsystem->star2 && strcmp(ship->starsystem->star2,"") &&
               abs(ship->vx - ship->starsystem->s2x) < 10 &&
                     abs(ship->vy - ship->starsystem->s2y) < 10 &&
                     abs(ship->vz - ship->starsystem->s2z) < 10 )
                {
//                    //echo_to_cockpit( AT_BLOOD+AT_BLINK, ship, "You fly directly into the sun.");
                    sprintf( buf , "%s flys directly into %s!", ship->name, ship->starsystem->star2);
//                    echo_to_system( AT_ORANGE , ship , buf , NULL );
                    destroy_ship(ship , NULL, "flew into sun");
                    continue;
                }

        if ( ship->currspeed > 0 )
        {
          if ( ship->starsystem->planet1 && strcmp(ship->starsystem->planet1,"") &&
               abs(ship->vx - ship->starsystem->p1x) < 10 &&
                     abs(ship->vy - ship->starsystem->p1y) < 10 &&
                     abs(ship->vz - ship->starsystem->p1z) < 10 )
                {
                    sprintf( buf , "You begin orbitting %s.", ship->starsystem->planet1);
//                    //echo_to_cockpit( AT_YELLOW, ship, buf);
                    sprintf( buf , "%s begins orbitting %s.", ship->name, ship->starsystem->planet1);
//                    echo_to_system( AT_ORANGE , ship , buf , NULL );
                    ship->currspeed = 0;
                    continue;
                }
          if ( ship->starsystem->planet2 && strcmp(ship->starsystem->planet2,"") &&
               abs(ship->vx - ship->starsystem->p2x) < 10 &&
                     abs(ship->vy - ship->starsystem->p2y) < 10 &&
                     abs(ship->vz - ship->starsystem->p2z) < 10 )
                {
                    sprintf( buf , "You begin orbitting %s.", ship->starsystem->planet2);
//                    //echo_to_cockpit( AT_YELLOW, ship, buf);
                    sprintf( buf , "%s begins orbitting %s.", ship->name, ship->starsystem->planet2);
//                    echo_to_system( AT_ORANGE , ship , buf , NULL );
                    ship->currspeed = 0;
                    continue;
                }
          if ( ship->starsystem->planet3 && strcmp(ship->starsystem->planet3,"") &&
               abs(ship->vx - ship->starsystem->p3x) < 10 &&
                     abs(ship->vy - ship->starsystem->p3y) < 10 &&
                     abs(ship->vz - ship->starsystem->p3z) < 10 )
                {
                    sprintf( buf , "You begin orbitting %s.", ship->starsystem->planet2);
//                    //echo_to_cockpit( AT_YELLOW, ship, buf);
                    sprintf( buf , "%s begins orbitting %s.", ship->name, ship->starsystem->planet2);
//                    echo_to_system( AT_ORANGE , ship , buf , NULL );
                    ship->currspeed = 0;
                    continue;
                }
        }
   }

   for ( ship = first_ship; ship; ship = ship->next )
       if (ship->collision)
       {
//           //echo_to_cockpit( AT_WHITE+AT_BLINK , ship,  "You have collided with another ship!" );
//           echo_to_ship( AT_RED , ship , "A loud explosion shakes the ship violently!" );
           damage_ship( ship , ship->collision , ship->collision );
           ship->collision = 0;
       }
}

void recharge_ships( )
{
   SHIP_DATA *ship;
   char buf[MSL];

   for ( ship = first_ship; ship; ship = ship->next )
   {

        if (ship->primaryState > 0)
        {
		  if(ship->primaryType != AUTOBLASTER)
           ship->energy -= ship->primaryState;

           ship->primaryState = 0;
        }
		if(ship->secondaryState > 0)
	   {
			if(ship->secondaryType != AUTOBLASTER)
				ship->energy -= ship->secondaryState;

			ship->secondaryState = 0;
	   }
        if (ship->statet1 > 0)
        {
           ship->energy -= ship->statet1;
           ship->statet1 = 0;
        }
        if (ship->statet2 > 0)
        {
           ship->energy -= ship->statet2;
           ship->statet2 = 0;
        }
        if (ship->statet3 > 0)
        {
           ship->energy -= ship->statet3;
           ship->statet3 = 0;
        }
        if (ship->statet4 > 0)
        {
           ship->energy -= ship->statet4;
           ship->statet4 = 0;
        }
        if (ship->statet5 > 0)
        {
           ship->energy -= ship->statet5;
           ship->statet5 = 0;
        }
        if (ship->statet6 > 0)
        {
           ship->energy -= ship->statet6;
           ship->statet6 = 0;
        }
        if (ship->statet7 > 0)
        {
           ship->energy -= ship->statet7;
           ship->statet7 = 0;
        }
        if (ship->statet8 > 0)
        {
           ship->energy -= ship->statet8;
           ship->statet8 = 0;
        }
        if (ship->statet9 > 0)
        {
           ship->energy -= ship->statet9;
           ship->statet9 = 0;
        }
        if (ship->statet10 > 0)
        {
           ship->energy -= ship->statet10;
           ship->statet10 = 0;
        }


        if (ship->missilestate == MISSILE_RELOAD_2)
        {
           ship->missilestate = MISSILE_READY;
           if ( ship->missiles > 0 ){}
//               echo_to_room( AT_YELLOW, get_room_index(ship->gunseat), "Missile launcher reloaded.");
        }

        if (ship->missilestate == MISSILE_RELOAD )
        {
           ship->missilestate = MISSILE_RELOAD_2;
        }

        if (ship->missilestate == MISSILE_FIRED )
           ship->missilestate = MISSILE_RELOAD;

        if (ship->torpedostate == MISSILE_RELOAD_2)
        {
           ship->torpedostate = MISSILE_READY;
           if ( ship->missiles > 0 ){}
//               echo_to_room( AT_YELLOW, get_room_index(ship->gunseat), "Torpedo launcher reloaded.");
        }

        if (ship->torpedostate == MISSILE_RELOAD )
        {
           ship->torpedostate = MISSILE_RELOAD_2;
        }

        if (ship->torpedostate == MISSILE_FIRED )
           ship->torpedostate = MISSILE_RELOAD;

        if (ship->rocketstate == MISSILE_RELOAD_2)
        {
           ship->rocketstate = MISSILE_READY;
           if ( ship->rockets > 0 ){}
 //              echo_to_room( AT_YELLOW, get_room_index(ship->gunseat), "Rocket launcher reloaded.");
        }

        if (ship->rocketstate == MISSILE_RELOAD )
        {
           ship->rocketstate = MISSILE_RELOAD_2;
        }

        if (ship->rocketstate == MISSILE_FIRED )
           ship->rocketstate = MISSILE_RELOAD;

       if ( autofly(ship) )
       {
          if ( ship->starsystem )
          {
             if (ship->target0 && ship->primaryState != LASER_DAMAGED )
             {
                 int chance = 50;
                 SHIP_DATA * target = ship->target0;
                 int shots;

                  for ( shots=0 ; shots <= ship->primaryCount && ship->target0; shots++ )
                {
                  if (ship->shipstate != SHIP_HYPERSPACE && ship->energy > 25
                  && ship->target0->starsystem == ship->starsystem
                  && abs(target->vx - ship->vx) <= 1000
                  && abs(target->vy - ship->vy) <= 1000
                  && abs(target->vz - ship->vz) <= 1000
                  && ship->primaryState < ship->primaryCount )
                  {
                    if ( ship->ship_class >= SHIP_SHUTTLE || is_facing ( ship , target ) )
                    {
             		chance += target->ship_class*25;
             		chance -= target->manuever/10;
             		chance -= target->currspeed/20;
             		chance -= ( abs(target->vx - ship->vx)/70 );
             		chance -= ( abs(target->vy - ship->vy)/70 );
             		chance -= ( abs(target->vz - ship->vz)/70 );
             		chance = URANGE( 10 , chance , 90 );
             		if ( number_percent( ) > chance )
             		{
           		    sprintf( buf , "%s fires at you but misses." , ship->name);
//             		    //echo_to_cockpit( AT_ORANGE , target , buf );
      	                    sprintf( buf, "Laserfire from %s barely misses %s." , ship->name , target->name );
 //                           echo_to_system( AT_ORANGE , target , buf , NULL );
             		}
             		else
             		{
             		    sprintf( buf, "Laserfire from %s hits %s." , ship->name, target->name );
//             		    echo_to_system( AT_ORANGE , target , buf , NULL );
                            sprintf( buf , "You are hit by lasers from %s!" , ship->name);
//                            //echo_to_cockpit( AT_BLOOD , target , buf );
//                            echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
                            damage_ship( target , 5 , 10 );
                        }
                        ship->primaryState++;
                    }
                  }
                }
             }
          }
       }

   }
}

void new_missile( SHIP_DATA *ship , SHIP_DATA *target , char_data *ch , int missiletype )
{
     SPACE_DATA *starsystem;
     MISSILE_DATA *missile;

     if ( ship  == NULL )
        return;

     if ( target  == NULL )
        return;

     if ( ( starsystem = ship->starsystem ) == NULL )
        return;

//     CREATE( missile, MISSILE_DATA, 1 );
     LINK( missile, first_missile, last_missile, next, prev );

     missile->target = target;
     missile->fired_from = ship;
     if ( ch )
        missile->fired_by = str_dup( ch->name );
     else
        missile->fired_by = str_dup( "" );
     missile->missiletype = missiletype;
     missile->age =0;
     if ( missile->missiletype == HEAVY_BOMB )
       missile->speed = 250;
     else if ( missile->missiletype == PROTON_TORPEDO )
       missile->speed = 200;
     else if ( missile->missiletype == CONCUSSION_MISSILE )
       missile->speed = 300;
     else
       missile->speed = 50;

     missile->mx = ship->vx;
     missile->my = ship->vy;
     missile->mz = ship->vz;

     if ( starsystem->first_missile == NULL )
        starsystem->first_missile = missile;

     if ( starsystem->last_missile )
     {
         starsystem->last_missile->next_in_starsystem = missile;
         missile->prev_in_starsystem = starsystem->last_missile;
     }

     starsystem->last_missile = missile;

     missile->starsystem = starsystem;

}


void update_space( )
{
   SHIP_DATA *ship;
   SHIP_DATA *target;
   char buf[MSL];
   int too_close, target_too_close;
   int recharge;

   for ( ship = first_ship; ship; ship = ship->next )
   {
        if (ship->starsystem)
        {
          if ( ship->energy > 0 && ship->shipstate == SHIP_DISABLED && ship->ship_class != SHIP_SPACE_STATION )
             ship->energy -= 100;
          else if ( ship->energy > 0 )
             ship->energy += ( 5 + ship->ship_class*5 );
          else if(ship->type == MOB_SHIP)
			 ship->energy = ship->maxenergy;
		  else
             destroy_ship(ship , NULL, "destroyed by lack of energy");
        }

        if ( ship->chaff_released > 0 )
           ship->chaff_released--;

        if (ship->shipstate == SHIP_HYPERSPACE)
        {
		   switch(ship->hyperspeed)
			{
			   case 1: ship->hyperdistance -= number_range(170, 225); break;
			   case 2: ship->hyperdistance -= number_range(135, 180); break;
			   case 3: ship->hyperdistance -= number_range(80, 110); break;
			   case 4: ship->hyperdistance -= number_range(60, 100); break;
			   case 5: ship->hyperdistance -= number_range(40, 60); break;
			   case 6: ship->hyperdistance -= number_range(20, 45); break;
			   default: ship->hyperdistance -= 20; break;
			}
            if (ship->hyperdistance <= 0)
            {
//            	ship_to_starsystem (ship, ship->currjump);

            	if (ship->starsystem == NULL)
            	{
//            	    //echo_to_cockpit( AT_RED, ship, "Ship lost in Hyperspace. Make new calculations.");
            	}
            	else
            	{
//            	    echo_to_room( AT_YELLOW, get_room_index(ship->pilotseat), "Hyperjump complete.");
//            	    echo_to_ship( AT_YELLOW, ship, "The ship lurches slightly as it comes out of hyperspace.");
            	    sprintf( buf ,"%s enters the starsystem at %.0f %.0f %.0f" , ship->name, ship->vx, ship->vy, ship->vz );
//            	    echo_to_system( AT_YELLOW, ship, buf , NULL );
            	    ship->shipstate = SHIP_READY;
//            	    STRFREE( ship->home );
//            	    ship->home = STRALLOC( ship->starsystem->name );
            	    if ( str_cmp("Public",ship->owner) )
                        save_ship(ship);

            	}
            }
            else
            {
               sprintf( buf ,"%d" , ship->hyperdistance );
//               echo_to_room_dnr( AT_YELLOW , get_room_index(ship->pilotseat), "Remaining jump distance: " );
//               echo_to_room( AT_WHITE , get_room_index(ship->pilotseat),  buf );

            }
        }

*/        /* following was originaly to fix ships that lost their pilot
           in the middle of a manuever and are stuck in a busy state
           but now used for timed manouevers such as turning */
/*
    	if (ship->shipstate == SHIP_BUSY_3)
           {
//              echo_to_room( AT_YELLOW, get_room_index(ship->pilotseat), "Manuever complete.");
              ship->shipstate = SHIP_READY;
           }
        if (ship->shipstate == SHIP_BUSY_2)
           ship->shipstate = SHIP_BUSY_3;
        if (ship->shipstate == SHIP_BUSY)
           ship->shipstate = SHIP_BUSY_2;

        if (ship->shipstate == SHIP_LAND_2)
           landship( ship , ship->dest );
        if (ship->shipstate == SHIP_LAND)
           ship->shipstate = SHIP_LAND_2;

        if (ship->shipstate == SHIP_LAUNCH_2)
           launchship( ship );
        if (ship->shipstate == SHIP_LAUNCH)
           ship->shipstate = SHIP_LAUNCH_2;


        ship->shield = UMAX( 0 , ship->shield-1-ship->ship_class);

        if (ship->autorecharge && ship->maxshield > ship->shield && ship->energy > 100)
        {
           recharge  = UMIN( ship->maxshield-ship->shield, 10 + ship->ship_class*10 );
           recharge  = UMIN( recharge , ship->energy/2 -100 );
           recharge  = UMAX( 1, recharge );
           ship->shield += recharge;
           ship->energy -= recharge;
        }

        if (ship->shield > 0)
        {
          if (ship->energy < 200)
          {
          	ship->shield = 0;
//          	//echo_to_cockpit( AT_RED, ship,"The ships shields fizzle and die.");
                ship->autorecharge = false;
          }
        }

        if ( ship->starsystem && ship->currspeed > 0 )
        {
               sprintf( buf, "%d  ",
                          ship->currspeed );
//               echo_to_room_dnr( AT_BLUE , get_room_index(ship->pilotseat),  "Speed: " );
//               echo_to_room_dnr( AT_LBLUE , get_room_index(ship->pilotseat),  buf );
			   if(ship->maxhull > 0){

			   sprintf( buf, "%d%%  ", (ship->hull*100)/ship->maxhull);
//			   echo_to_room_dnr( AT_BLUE, get_room_index(ship->pilotseat), "Hull: ");
//			   echo_to_room_dnr( AT_LBLUE, get_room_index(ship->pilotseat), buf);
			   }

			   if(ship->maxshield > 0){
				   sprintf(buf, "%d%%  ", (ship->shield*100)/ship->maxshield);
//				   echo_to_room_dnr(AT_BLUE, get_room_index(ship->pilotseat), "Shields: ");
//				   echo_to_room_dnr(AT_LBLUE, get_room_index(ship->pilotseat), buf);
			   }
			   if(ship->maxenergy > 0){
			   sprintf( buf, "%d%%  ", (ship->energy*100)/ship->maxenergy);
//			   echo_to_room_dnr(AT_BLUE, get_room_index(ship->pilotseat), "Energy: ");
//			   echo_to_room(AT_LBLUE, get_room_index(ship->pilotseat), buf);
			   }
//               echo_to_room_dnr( AT_BLUE , get_room_index(ship->pilotseat),  "Coordinates in " );
			   if(ship->starsystem)
				   sprintf(buf, "&C%s&B system: ", ship->starsystem->name);
			   else
				   sprintf(buf, "system: ");
//			   echo_to_room_dnr(AT_BLUE, get_room_index(ship->pilotseat), buf);
               sprintf( buf, "%.0f %.0f %.0f",
                           ship->vx , ship->vy, ship->vz );
//               echo_to_room( AT_LBLUE , get_room_index(ship->pilotseat),  buf );


            if ( ship->pilotseat != ship->coseat )
            {
               sprintf( buf, "%d  ",
                          ship->currspeed );
//               echo_to_room_dnr( AT_BLUE , get_room_index(ship->coseat),  "Speed: " );
//               echo_to_room_dnr( AT_LBLUE , get_room_index(ship->coseat),  buf );
			   if(ship->maxhull > 0){

			   sprintf( buf, "%d%%  ", (ship->hull*100)/ship->maxhull);
//			   echo_to_room_dnr( AT_BLUE, get_room_index(ship->coseat), "Hull: ");
//			   echo_to_room_dnr( AT_LBLUE, get_room_index(ship->coseat), buf);
			   }

			   if(ship->maxshield > 0){
				   sprintf(buf, "%d%%  ", (ship->shield*100)/ship->maxshield);
//				   echo_to_room_dnr(AT_BLUE, get_room_index(ship->coseat), "Shields: ");
//				   echo_to_room_dnr(AT_LBLUE, get_room_index(ship->coseat), buf);
			   }
			   if(ship->maxenergy > 0){
			   sprintf( buf, "%d%%  ", (ship->energy*100)/ship->maxenergy);
//			   echo_to_room_dnr(AT_BLUE, get_room_index(ship->coseat), "Energy: ");
//			   echo_to_room(AT_LBLUE, get_room_index(ship->coseat), buf);
			   }
//               echo_to_room_dnr( AT_BLUE , get_room_index(ship->coseat),  "Coordinates in " );
			   if(ship->starsystem)
				   sprintf(buf, "&C%s&B system: ", ship->starsystem->name);
			   else
				   sprintf(buf, "system: ");
//			   echo_to_room_dnr(AT_BLUE, get_room_index(ship->coseat), buf);
               sprintf( buf, "%.0f %.0f %.0f",
                           ship->vx , ship->vy, ship->vz );
//               echo_to_room( AT_LBLUE , get_room_index(ship->coseat),  buf );
            }
        }

        if ( ship->starsystem )
        {
          too_close = ship->currspeed + 50;
          for ( target = ship->starsystem->first_ship; target; target = target->next_in_starsystem)
          {
            target_too_close = too_close+target->currspeed;
            if ( target != ship &&
                 abs(ship->vx - target->vx) < target_too_close &&
                 abs(ship->vy - target->vy) < target_too_close &&
                 abs(ship->vz - target->vz) < target_too_close )
            {
                sprintf( buf, "Proximity alert: %s  %.0f %.0f %.0f"
                            , target->name, target->vx, target->vy, target->vz);
//                echo_to_room( AT_RED , get_room_index(ship->pilotseat),  buf );
            }
          }
          too_close = ship->currspeed + 100;
          if ( ship->starsystem->star1 &&  strcmp(ship->starsystem->star1,"") &&
               abs(ship->vx - ship->starsystem->s1x) < too_close &&
               abs(ship->vy - ship->starsystem->s1y) < too_close &&
               abs(ship->vz - ship->starsystem->s1z) < too_close )
          {
                sprintf( buf, "Proximity alert: %s  %d %d %d", ship->starsystem->star1,
                         ship->starsystem->s1x, ship->starsystem->s1y, ship->starsystem->s1z);
//                echo_to_room( AT_RED , get_room_index(ship->pilotseat),  buf );
          }
          if ( ship->starsystem->star2 && strcmp(ship->starsystem->star2,"") &&
               abs(ship->vx - ship->starsystem->s2x) < too_close &&
               abs(ship->vy - ship->starsystem->s2y) < too_close &&
               abs(ship->vz - ship->starsystem->s2z) < too_close )
          {
                sprintf( buf, "Proximity alert: %s  %d %d %d", ship->starsystem->star2,
                         ship->starsystem->s2x, ship->starsystem->s2y, ship->starsystem->s2z);
//                echo_to_room( AT_RED , get_room_index(ship->pilotseat),  buf );
          }
          if ( ship->starsystem->planet1 && strcmp(ship->starsystem->planet1,"") &&
               abs(ship->vx - ship->starsystem->p1x) < too_close &&
               abs(ship->vy - ship->starsystem->p1y) < too_close &&
               abs(ship->vz - ship->starsystem->p1z) < too_close )
          {
                sprintf( buf, "Proximity alert: %s  %d %d %d", ship->starsystem->planet1,
                         ship->starsystem->p1x, ship->starsystem->p1y, ship->starsystem->p1z);
//                echo_to_room( AT_RED , get_room_index(ship->pilotseat),  buf );
          }
          if ( ship->starsystem->planet2 && strcmp(ship->starsystem->planet2,"") &&
               abs(ship->vx - ship->starsystem->p2x) < too_close &&
               abs(ship->vy - ship->starsystem->p2y) < too_close &&
               abs(ship->vz - ship->starsystem->p2z) < too_close )
          {
                sprintf( buf, "Proximity alert: %s  %d %d %d", ship->starsystem->planet2,
                         ship->starsystem->p2x, ship->starsystem->p2y, ship->starsystem->p2z);
//                echo_to_room( AT_RED , get_room_index(ship->pilotseat),  buf );
          }
          if ( ship->starsystem->planet3 && strcmp(ship->starsystem->planet3,"") &&
               abs(ship->vx - ship->starsystem->p3x) < too_close &&
               abs(ship->vy - ship->starsystem->p3y) < too_close &&
               abs(ship->vz - ship->starsystem->p3z) < too_close )
          {
                sprintf( buf, "Proximity alert: %s  %d %d %d", ship->starsystem->planet3,
                         ship->starsystem->p3x, ship->starsystem->p3y, ship->starsystem->p3z);
//                echo_to_room( AT_RED , get_room_index(ship->pilotseat),  buf );
          }
        }


        if (ship->target0)
        {
               sprintf( buf, "%s   %.0f %.0f %.0f", ship->target0->name,
                          ship->target0->vx , ship->target0->vy, ship->target0->vz );
//               echo_to_room_dnr( AT_BLUE , get_room_index(ship->gunseat), "Target: " );
//               echo_to_room( AT_LBLUE , get_room_index(ship->gunseat),  buf );
               if (ship->starsystem != ship->target0->starsystem)
               		ship->target0 = NULL;
         }

        if (ship->target1)
        {
               sprintf( buf, "%s   %.0f %.0f %.0f", ship->target1->name,
                          ship->target1->vx , ship->target1->vy, ship->target1->vz );
//               echo_to_room_dnr( AT_BLUE , get_room_index(ship->turret1), "Target: " );
//               echo_to_room( AT_LBLUE , get_room_index(ship->turret1),  buf );
               if (ship->starsystem != ship->target1->starsystem)
               		ship->target1 = NULL;
         }

        if (ship->target2)
        {
               sprintf( buf, "%s   %.0f %.0f %.0f", ship->target2->name,
                          ship->target2->vx , ship->target2->vy, ship->target2->vz );
//               echo_to_room_dnr( AT_BLUE , get_room_index(ship->turret2), "Target: " );
//               echo_to_room( AT_LBLUE , get_room_index(ship->turret2),  buf );
               if (ship->starsystem != ship->target2->starsystem)
               		ship->target2 = NULL;
         }

   	if (ship->energy < 100 && ship->starsystem )
   	{
//   	    //echo_to_cockpit( AT_RED , ship,  "Warning: Ship fuel low." );
        }

        ship->energy = URANGE( 0 , ship->energy, ship->maxenergy );
   }

   for ( ship = first_ship; ship; ship = ship->next )
   {

       if (ship->autotrack && ship->target0 && ship->ship_class < SHIP_CRUISER )
       {
           target = ship->target0;
           too_close = ship->currspeed + 10;
           target_too_close = too_close+target->currspeed;
           if ( target != ship && ship->shipstate == SHIP_READY &&
                 abs(ship->vx - target->vx) < target_too_close &&
                 abs(ship->vy - target->vy) < target_too_close &&
                 abs(ship->vz - target->vz) < target_too_close )
           {
              ship->hx = 0-(ship->target0->vx - ship->vx);
              ship->hy = 0-(ship->target0->vy - ship->vy);
              ship->hz = 0-(ship->target0->vz - ship->vz);
              ship->energy -= ship->currspeed/10;
//              echo_to_room( AT_RED , get_room_index(ship->pilotseat), "Autotrack: Evading to avoid collision!\n\r" );
    	      if ( (ship->ship_class >= SHIP_FIGHTER && ship->ship_class<= SHIP_SHUTTLE)
				  || ( (ship->ship_class >= SHIP_FREIGHTER && ship->ship_class<= SHIP_CORVETTE) && ship->manuever > 50) )
        	ship->shipstate = SHIP_BUSY_3;
              else if ( ship->ship_class >= SHIP_CRUISER || (ship->ship_class == SHIP_DREADNAUGHT && ship->manuever > 50) )
        	ship->shipstate = SHIP_BUSY_2;
    	      else
        	ship->shipstate = SHIP_BUSY;
           }
           else if  ( !is_facing(ship, ship->target0) )
           {
              ship->hx = ship->target0->vx - ship->vx;
              ship->hy = ship->target0->vy - ship->vy;
              ship->hz = ship->target0->vz - ship->vz;
              ship->energy -= ship->currspeed/10;
//              echo_to_room( AT_BLUE , get_room_index(ship->pilotseat), "Autotracking target ... setting new course.\n\r" );
    	      if ( (ship->ship_class >= SHIP_FIGHTER && ship->ship_class<= SHIP_SHUTTLE)
				  || ( (ship->ship_class >= SHIP_FREIGHTER && ship->ship_class<= SHIP_CORVETTE) && ship->manuever > 50 ) )
        	ship->shipstate = SHIP_BUSY_3;
              else if ( ship->ship_class >= SHIP_CRUISER || (ship->ship_class == SHIP_DREADNAUGHT && ship->manuever > 50) )
        	ship->shipstate = SHIP_BUSY_2;
    	      else
        	ship->shipstate = SHIP_BUSY;
           }
       }

       if ( autofly(ship) )
       {
          if ( ship->starsystem )
          {
             if (ship->target0)
             {
                 int chance = 50;

*/                 /* auto assist ships */

  /*               for ( target = ship->starsystem->first_ship; target; target = target->next_in_starsystem)
                 {
                    if ( autofly(target) )
		    {
			if(IS_SET(target->flags, SHIP_SIMULATOR))  continue;
                       if ( !str_cmp ( target->owner , ship->owner ) && target != ship )
                         if ( target->target0 == NULL && ship->target0 != target )
                         {
                           target->target0 = ship->target0;
                           sprintf( buf , "You are being targetted by %s." , target->name);
//                           //echo_to_cockpit( AT_BLOOD , target->target0 , buf );
                           break;
                        }
                    }
		}

                 target = ship->target0;
                 ship->autotrack = true;
                 if( ship->ship_class != SHIP_SPACE_STATION )
                      ship->currspeed = ship->realspeed;
                 if ( ship->energy >200  )
                    ship->autorecharge=true;


                 if (ship->shipstate != SHIP_HYPERSPACE && ship->energy > 25
                 && ship->missilestate == MISSILE_READY && ship->target0->starsystem == ship->starsystem
                 && abs(target->vx - ship->vx) <= 1200
                 && abs(target->vy - ship->vy) <= 1200
                 && abs(target->vz - ship->vz) <= 1200
                 && ship->missiles > 0 )
                 {
                   if ( ship->ship_class >= SHIP_CRUISER || is_facing( ship , target ) )
                   {
             		chance -= target->manuever/5;
                        chance -= target->currspeed/20;
                        chance += target->ship_class*target->ship_class*25;
                        chance -= ( abs(target->vx - ship->vx)/100 );
                        chance -= ( abs(target->vy - ship->vy)/100 );
                        chance -= ( abs(target->vz - ship->vz)/100 );
                        chance += ( 30 );
                        chance = URANGE( 10 , chance , 90 );

             		if ( number_percent( ) > chance )
             		{
             		}
                        else
                        {
                                new_missile( ship , target , NULL , CONCUSSION_MISSILE );
            		 	ship->missiles-- ;
             		        sprintf( buf , "Incoming missile from %s." , ship->name);
//             		        //echo_to_cockpit( AT_BLOOD , target , buf );
             		        sprintf( buf, "%s fires a missile towards %s." , ship->name, target->name );
//             		        echo_to_system( AT_ORANGE , target , buf , NULL );

            		 	if ( ship->ship_class >= SHIP_CRUISER )
                                    ship->missilestate = MISSILE_RELOAD_2;
                                else
                                    ship->missilestate = MISSILE_FIRED;
                        }
                   }
                 }

                 if( ship->missilestate ==  MISSILE_DAMAGED )
                     ship->missilestate =  MISSILE_READY;
                 if( ship->primaryState ==  LASER_DAMAGED )
                     ship->primaryState =  LASER_READY;
				 if( ship->secondaryState == LASER_DAMAGED )
					 ship->secondaryState = LASER_READY;
                 if( ship->shipstate ==  SHIP_DISABLED )
                     ship->shipstate =  SHIP_READY;

             }
             else
             {
                 ship->currspeed = 0;

                 if ( !str_cmp( ship->owner , "The Empire" ) )
                   for ( target = first_ship; target; target = target->next )
                     if ( ship->starsystem == target->starsystem )
                        if ( !str_cmp( target->owner , "The New Republic" ) )
                        {
                          ship->target0 = target;
                          sprintf( buf , "You are being targetted by %s." , ship->name);
//                          //echo_to_cockpit( AT_BLOOD , target , buf );
                          break;
                        }
                if ( !str_cmp( ship->owner , "The New Republic" ) )
                   for ( target = first_ship; target; target = target->next )
                     if ( ship->starsystem == target->starsystem )
                        if ( !str_cmp( target->owner , "The Empire" ) )
                        {
                          sprintf( buf , "You are being targetted by %s." , ship->name);
//                          //echo_to_cockpit( AT_BLOOD , target , buf );
                          ship->target0 = target;
                          break;
                        }

               if ( !str_cmp( ship->owner , "Pirates" ) )
                   for ( target = first_ship; target; target = target->next )
                     if ( ship->starsystem == target->starsystem )
                     {
                          sprintf( buf , "You are being targetted by %s." , ship->name);
//                          //echo_to_cockpit( AT_BLOOD , target , buf );
                          ship->target0 = target;
                          break;
                     }

             }
          }
          else
          {
               if ( number_range(1, 25) == 25 )
               {
//          	  ship_to_starsystem(ship, starsystem_from_name(ship->home) );
          	  ship->vx = number_range( -5000 , 5000 );
          	  ship->vy = number_range( -5000 , 5000 );
          	  ship->vz = number_range( -5000 , 5000 );
                  ship->hx = 1;
                  ship->hy = 1;
                  ship->hz = 1;
               }
          }
       }

       if ( ( ship->ship_class >= SHIP_CRUISER )
       && ship->target0 == NULL )
       {
          if( ship->missiles < ship->maxmissiles )
             ship->missiles++;
          if( ship->torpedos < ship->maxtorpedos )
             ship->torpedos++;
          if( ship->rockets < ship->maxrockets )
             ship->rockets++;
       }
   }

}
*/


void write_starsystem_list( )
{
    SPACE_DATA *tstarsystem;
    FILE *fpout;
    char filename[256];

    sprintf( filename, "%s%s", SPACE_DIR, SPACE_FILE );
    fpout = fopen( filename, "w" );
    if ( !fpout )
    {
         bug( "FATAL: cannot open starsystem.lst for writing!\n\r");
         return;
    }
    for ( tstarsystem = first_starsystem; tstarsystem; tstarsystem = tstarsystem->next )
    fprintf( fpout, "%s\n", tstarsystem->filename );
    fprintf( fpout, "$\n" );
    fclose( fpout );
}


/*
 * Get pointer to space structure from starsystem name.
 */
SPACE_DATA *starsystem_from_name( char *name )
{
    SPACE_DATA *starsystem;

    for ( starsystem = first_starsystem; starsystem; starsystem = starsystem->next )
       if ( !str_cmp( name, starsystem->name ) )
         return starsystem;

    for ( starsystem = first_starsystem; starsystem; starsystem = starsystem->next )
       if ( !str_prefix( name, starsystem->name ) )
         return starsystem;

    return NULL;
}

/*
 * Get pointer to space structure from the dock vnun.
 */
SPACE_DATA *starsystem_from_vnum( int vnum )
{
    SPACE_DATA *starsystem;
    SHIP_DATA *ship;

    for ( starsystem = first_starsystem; starsystem; starsystem = starsystem->next )
       if ( vnum == starsystem->doc1a || vnum == starsystem->doc2a    || vnum == starsystem->doc3a ||
            vnum == starsystem->doc1b || vnum == starsystem->doc2b    || vnum == starsystem->doc3b ||
            vnum == starsystem->doc1c || vnum == starsystem->doc2c    || vnum == starsystem->doc3c )
         return starsystem;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( vnum == ship->hanger1 || vnum == ship->hanger2
		 || vnum == ship->hanger3 || vnum == ship->hanger4)
            return ship->starsystem;

    return NULL;
}
SPACE_DATA *starsystem_from_room( ROOM_INDEX_DATA *room )
{
    SHIP_DATA * ship;
//    ROOM_INDEX_DATA  *sRoom;

    if ( room == NULL )
        return NULL;

    if ( room->area != NULL && room->area->planet != NULL )
          return room->area->planet->starsystem;

    for ( ship = first_ship; ship; ship = ship->next )

        //for ( sRoom->vnum = ship->firstroom ; sRoom ; sRoom = sRoom->next_in_ship )
			//if ( room == sRoom )
	{
		if(room->vnum == ship->hanger1 || room->vnum == ship->hanger2
			|| room->vnum == ship->hanger3 || room->vnum == ship->hanger4)
              return ship->starsystem;
	}

    return NULL;
}



/*
 * Save a starsystem's data to its data file
 */
void save_starsystem( SPACE_DATA *starsystem )
{
    FILE *fp;
    char filename[256];
    char buf[MSL];

    if ( !starsystem )
    {
	bug( "save_starsystem: null starsystem pointer!" );
	return;
    }

    if ( !starsystem->filename || starsystem->filename[0] == '\0' )
    {
	sprintf( buf, "save_starsystem: %s has no filename", starsystem->name );
	bug( buf);
	return;
    }

    sprintf( filename, "%s%s", SPACE_DIR, starsystem->filename );

    fclose( fpReserve );
    if ( ( fp = fopen( filename, "w" ) ) == NULL )
    {
    	bug( "save_starsystem: fopen" );
    	perror( filename );
    }
    else
    {
	fprintf( fp, "#SPACE\n" );
	fprintf( fp, "Name         %s~\n",	starsystem->name	);
	fprintf( fp, "Filename     %s~\n",	starsystem->filename	);
	fprintf( fp, "Planet1      %s~\n",	starsystem->planet1	);
	fprintf( fp, "Planet2      %s~\n",	starsystem->planet2	);
	fprintf( fp, "Planet3      %s~\n",	starsystem->planet3	);
	fprintf( fp, "Star1        %s~\n",	starsystem->star1	);
	fprintf( fp, "Star2        %s~\n",	starsystem->star2	);
	fprintf( fp, "Location1a      %s~\n",	starsystem->location1a	);
	fprintf( fp, "Location1b      %s~\n",	starsystem->location1b	);
	fprintf( fp, "Location1c      %s~\n",	starsystem->location1c	);
	fprintf( fp, "Location2a       %s~\n",	starsystem->location2a	);
	fprintf( fp, "Location2b      %s~\n",	starsystem->location2b	);
	fprintf( fp, "Location2c      %s~\n",	starsystem->location2c	);
	fprintf( fp, "Location3a      %s~\n",	starsystem->location3a	);
	fprintf( fp, "Location3b      %s~\n",	starsystem->location3b	);
	fprintf( fp, "Location3c      %s~\n",	starsystem->location3c	);
	fprintf( fp, "Doc1a          %d\n",	starsystem->doc1a	);
	fprintf( fp, "Doc2a          %d\n",      starsystem->doc2a       );
	fprintf( fp, "Doc3a          %d\n",      starsystem->doc3a       );
	fprintf( fp, "Doc1b          %d\n", 	 starsystem->doc1b	);
	fprintf( fp, "Doc2b          %d\n",      starsystem->doc2b       );
	fprintf( fp, "Doc3b          %d\n",      starsystem->doc3b       );
	fprintf( fp, "Doc1c          %d\n",	 starsystem->doc1c	);
	fprintf( fp, "Doc2c          %d\n",      starsystem->doc2c       );
	fprintf( fp, "Doc3c          %d\n",      starsystem->doc3c       );
	fprintf( fp, "P1x          %d\n",       starsystem->p1x         );
	fprintf( fp, "P1y          %d\n",       starsystem->p1y         );
	fprintf( fp, "P1z          %d\n",       starsystem->p1z         );
	fprintf( fp, "P2x          %d\n",       starsystem->p2x         );
	fprintf( fp, "P2y          %d\n",       starsystem->p2y         );
	fprintf( fp, "P2z          %d\n",       starsystem->p2z         );
	fprintf( fp, "P3x          %d\n",       starsystem->p3x         );
	fprintf( fp, "P3y          %d\n",       starsystem->p3y         );
	fprintf( fp, "P3z          %d\n",       starsystem->p3z         );
	fprintf( fp, "S1x          %d\n",       starsystem->s1x         );
	fprintf( fp, "S1y          %d\n",       starsystem->s1y         );
	fprintf( fp, "S1z          %d\n",       starsystem->s1z         );
	fprintf( fp, "S2x          %d\n",       starsystem->s2x         );
	fprintf( fp, "S2y          %d\n",       starsystem->s2y         );
	fprintf( fp, "S2z          %d\n",       starsystem->s2z         );
	fprintf( fp, "Gravitys1     %d\n",       starsystem->gravitys1    );
	fprintf( fp, "Gravitys2     %d\n",       starsystem->gravitys2    );
	fprintf( fp, "Gravityp1     %d\n",       starsystem->gravityp1    );
	fprintf( fp, "Gravityp2     %d\n",       starsystem->gravityp2    );
	fprintf( fp, "Gravityp3     %d\n",       starsystem->gravityp3    );
	fprintf( fp, "Xpos          %d\n",       starsystem->xpos    );
	fprintf( fp, "Ypos          %d\n",       starsystem->ypos    );
	fprintf( fp, "End\n\n"						);
	fprintf( fp, "#END\n"						);
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}


/*
 * Read in actual starsystem data.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = true;			\
				    break;				\
				}

void fread_starsystem( SPACE_DATA *starsystem, FILE *fp )
{
    char buf[MSL];
    char *word;
    bool fMatch;


    for ( ; ; )
    {
	word   = feof( fp ) ? (char*)"End" : fread_word( fp );
	fMatch = false;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = true;
	    fread_to_eol( fp );
	    break;

        case 'D':
             KEY( "Doc1a",  starsystem->doc1a,          fread_number( fp ) );
             KEY( "Doc2a",  starsystem->doc2a,          fread_number( fp ) );
             KEY( "Doc3a",  starsystem->doc3a,          fread_number( fp ) );
             KEY( "Doc1b",  starsystem->doc1b,          fread_number( fp ) );
             KEY( "Doc2b",  starsystem->doc2b,          fread_number( fp ) );
             KEY( "Doc3b",  starsystem->doc3b,          fread_number( fp ) );
             KEY( "Doc1c",  starsystem->doc1c,          fread_number( fp ) );
             KEY( "Doc2c",  starsystem->doc2c,          fread_number( fp ) );
             KEY( "Doc3c",  starsystem->doc3c,          fread_number( fp ) );
             break;


	case 'E':
	    if ( !str_cmp( word, "End" ) )
	    {
		if (!starsystem->name)
		  starsystem->name		= str_dup( "" );
		if (!starsystem->location1a)
		  starsystem->location1a            = str_dup( "" );
		if (!starsystem->location2a)
		  starsystem->location2a            = str_dup( "" );
		if (!starsystem->location3a)
		  starsystem->location3a            = str_dup( "" );
		if (!starsystem->location1b)
		  starsystem->location1b            = str_dup( "" );
		if (!starsystem->location2b)
		  starsystem->location2b            = str_dup( "" );
		if (!starsystem->location3b)
		  starsystem->location3b            = str_dup( "" );
		if (!starsystem->location1c)
		  starsystem->location1c            = str_dup( "" );
		if (!starsystem->location2c)
		  starsystem->location2c            = str_dup( "" );
		if (!starsystem->location3c)
		  starsystem->location3c            = str_dup( "" );
		if (!starsystem->planet1)
		  starsystem->planet1            = str_dup( "" );
		if (!starsystem->planet2)
		  starsystem->planet2            = str_dup( "" );
		if (!starsystem->planet3)
		  starsystem->planet3            = str_dup( "" );
		if (!starsystem->star1)
		  starsystem->star1            = str_dup( "" );
		if (!starsystem->star2)
		  starsystem->star2            = str_dup( "" );
		return;
	    }
	    break;

/*	case 'F':
	    KEY( "Filename",	starsystem->filename,		fread_string( fp ) );
	    break;
*/
    case 'G':
            KEY( "Gravitys1",  starsystem->gravitys1,     fread_number( fp ) );
            KEY( "Gravitys2",  starsystem->gravitys2,     fread_number( fp ) );
            KEY( "Gravityp1",  starsystem->gravityp1,     fread_number( fp ) );
            KEY( "Gravityp2",  starsystem->gravityp2,     fread_number( fp ) );
            KEY( "Gravityp3",  starsystem->gravityp3,     fread_number( fp ) );
            break;

        case 'L':
	    KEY( "Location1a",	starsystem->location1a,	fread_string( fp ) );
	    KEY( "Location2a",	starsystem->location2a,	fread_string( fp ) );
	    KEY( "Location3a",	starsystem->location3a,	fread_string( fp ) );
	    KEY( "Location1b",	starsystem->location1b,	fread_string( fp ) );
	    KEY( "Location2b",	starsystem->location2b,	fread_string( fp ) );
	    KEY( "Location3b",	starsystem->location3b,	fread_string( fp ) );
	    KEY( "Location1c",	starsystem->location1c,	fread_string( fp ) );
	    KEY( "Location2c",	starsystem->location2c,	fread_string( fp ) );
	    KEY( "Location3c",	starsystem->location3c,	fread_string( fp ) );
	    break;

	case 'N':
	    KEY( "Name",	starsystem->name,		fread_string( fp ) );
	    break;

        case 'P':
             KEY( "Planet1",	starsystem->planet1,	fread_string( fp ) );
	     KEY( "Planet2",	starsystem->planet2,	fread_string( fp ) );
	     KEY( "Planet3",	starsystem->planet3,	fread_string( fp ) );
	     KEY( "P1x",  starsystem->p1x,          fread_number( fp ) );
             KEY( "P1y",  starsystem->p1y,          fread_number( fp ) );
             KEY( "P1z",  starsystem->p1z,          fread_number( fp ) );
             KEY( "P2x",  starsystem->p2x,          fread_number( fp ) );
             KEY( "P2y",  starsystem->p2y,          fread_number( fp ) );
             KEY( "P2z",  starsystem->p2z,          fread_number( fp ) );
             KEY( "P3x",  starsystem->p3x,          fread_number( fp ) );
             KEY( "P3y",  starsystem->p3y,          fread_number( fp ) );
             KEY( "P3z",  starsystem->p3z,          fread_number( fp ) );
             break;

       	case 'S':
       	     KEY( "Star1",	starsystem->star1,	fread_string( fp ) );
	     KEY( "Star2",	starsystem->star2,	fread_string( fp ) );
	     KEY( "S1x",  starsystem->s1x,          fread_number( fp ) );
             KEY( "S1y",  starsystem->s1y,          fread_number( fp ) );
             KEY( "S1z",  starsystem->s1z,          fread_number( fp ) );
             KEY( "S2x",  starsystem->s2x,          fread_number( fp ) );
             KEY( "S2y",  starsystem->s2y,          fread_number( fp ) );
             KEY( "S2z",  starsystem->s2z,          fread_number( fp ) );

        case 'X':
            KEY( "Xpos",  starsystem->xpos,     fread_number( fp ) );

        case 'Y':
            KEY( "Ypos",  starsystem->ypos,     fread_number( fp ) );

       	}

	if ( !fMatch )
	{
	    sprintf( buf, "Fread_starsystem: no match: %s", word );
	    bug( buf );
	}
    }
}

/*
 * Load a starsystem file
 */
/*
bool load_starsystem( char *starsystemfile )
{
    char filename[256];
    SPACE_DATA *starsystem;
    FILE *fp;
    bool found;

//    CREATE( starsystem, SPACE_DATA, 1 );

    found = false;
    sprintf( filename, "%s%s", SPACE_DIR, starsystemfile );

    if ( ( fp = fopen( filename, "r" ) ) != NULL )
    {

	found = true;
        LINK( starsystem, first_starsystem, last_starsystem, next, prev );
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_starsystem_file: # not found." );
		break;
	    }

	    word = fread_word( fp );
	    if ( !str_cmp( word, "SPACE"	) )
	    {
	    	fread_starsystem( starsystem, fp );
	    	break;
	    }
	    else
	    if ( !str_cmp( word, "END"	) )
	        break;
	    else
	    {
		char buf[MSL];

		sprintf( buf, "Load_starsystem_file: bad section: %s.", word );
		bug( buf );
		break;
	    }
	}
	fclose( fp );
    }
*//*
    if ( !(found) )
      DISPOSE( starsystem );
*/
/*    return found;
}

*//*
 * Load in all the starsystem files.
 *//*
void load_space( )
{
    FILE *fpList;
    char *filename;
    char starsystemlist[256];
    char buf[MSL];


    first_starsystem	= NULL;
    last_starsystem	= NULL;

    log_string( "Loading space..." );

    sprintf( starsystemlist, "%s%s", SPACE_DIR, SPACE_FILE );
    fclose( fpReserve );
    if ( ( fpList = fopen( starsystemlist, "r" ) ) == NULL )
    {
	perror( starsystemlist );
	exit( 1 );
    }

    for ( ; ; )
    {
	filename = feof( fpList ) ? (char*)"$" : fread_word( fpList );
	if ( filename[0] == '$' )
	  break;


	if ( !load_starsystem( filename ) )
	{
	  sprintf( buf, "Cannot load starsystem file: %s", filename );
	  bug( buf );
	}
    }
    fclose( fpList );
    log_string(" Done starsystems " );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}
*//*
void do_setstarsystem( char_data *ch, char *argument )
{
    char arg1[MIL];
    char arg2[MIL];
    SPACE_DATA *starsystem;

    if ( IS_NPC( ch ) )
    {
	ch->println( "Huh?\n\r" );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg2[0] == '\0' || arg1[0] == '\0' )
    {
	ch->println( "Usage: setstarsystem <starsystem> <field> <values>\n\r" );
	ch->println( "\n\rField being one of:\n\r" );
	ch->println( "name xpos ypos,\n\r" );
	ch->println( "star1 s1x s1y s1z gravitys1\n\r" );
	ch->println( "star2 s2x s2y s2z gravitys2\n\r" );
	ch->println( "planet1 p1x p1y p1z gravityp1\n\r" );
	ch->println( "planet2 p2x p2y p2z gravityp2\n\r" );
	ch->println( "planet3 p3x p3y p3z gravityp3\n\r" );
	ch->println( "location1a location1b location1c doc1a doc1b doc1c\n\r" );
	ch->println( "location2a location2b location2c doc2a doc2b doc2c\n\r" );
	ch->println( "location3a location3b location3c doc3a doc3b doc3c\n\r" );
	ch->println( "" );
	return;
    }

    starsystem = starsystem_from_name( arg1 );
    if ( !starsystem )
    {
	ch->println( "No such starsystem.\n\r" );
	return;
    }


    if ( !str_cmp( arg2, "doc1a" ) )
    {
	starsystem->doc1a = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "doc1b" ) )
    {
	starsystem->doc1b = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "doc1c" ) )
    {
	starsystem->doc1c = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "doc2a" ) )
    {
	starsystem->doc2a = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "doc2b" ) )
    {
	starsystem->doc2b = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "doc2c" ) )
    {
	starsystem->doc2c = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "doc3a" ) )
    {
	starsystem->doc3a = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "doc3b" ) )
    {
	starsystem->doc3b = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "doc3c" ) )
    {
	starsystem->doc3c = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "s1x" ) )
    {
	starsystem->s1x = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "s1y" ) )
    {
	starsystem->s1y = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "s1z" ) )
    {
	starsystem->s1z = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "s2x" ) )
    {
	starsystem->s2x = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "s2y" ) )
    {
	starsystem->s2y = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "s2z" ) )
    {
	starsystem->s2z = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "p1x" ) )
    {
	starsystem->p1x = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "p1y" ) )
    {
	starsystem->p1y = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "p1z" ) )
    {
	starsystem->p1z = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "p2x" ) )
    {
	starsystem->p2x = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "p2y" ) )
    {
	starsystem->p2y = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "p2z" ) )
    {
	starsystem->p2z = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "p3x" ) )
    {
	starsystem->p3x = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "p3y" ) )
    {
	starsystem->p3y = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "p3z" ) )
    {
	starsystem->p3z = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "xpos" ) )
    {
	starsystem->xpos = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "ypos" ) )
    {
	starsystem->ypos = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "gravitys1" ) )
    {
	starsystem->gravitys1 = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "gravitys2" ) )
    {
	starsystem->gravitys2 = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "gravityp1" ) )
    {
	starsystem->gravityp1 = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "gravityp2" ) )
    {
	starsystem->gravityp2 = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "gravityp3" ) )
    {
	starsystem->gravityp3 = atoi( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

   if( !str_cmp( arg2, "name" ) )
   {
      SPACE_DATA *tstarsystem = NULL;

      if( !argument || argument[0] == '\0' )
      {
         ch->println( "You can't name a starsystem nothing.\r\n" );
         return;
      }
      if( ( tstarsystem = starsystem_from_name( argument ) ) != NULL )
      {
         ch->println( "There is already another starsystem with that name.\r\n" );
         return;
      }
//      STRFREE( starsystem->name );
      starsystem->name = str_dup( argument );
      ch->println( "Done.\n\r" );
      save_starsystem( starsystem );
      return;
   }

    if ( !str_cmp( arg2, "star1" ) )
    {
//	STRFREE( starsystem->star1 );
	starsystem->star1 = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "star2" ) )
    {
//	STRFREE( starsystem->star2 );
	starsystem->star2 = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if( !str_cmp( arg2, "planet1" ) || !str_cmp( arg2, "planet2" ) || !str_cmp( arg2, "planet3" ) )
    {
        PLANET_DATA *tplanet;
        bool pFound = false;

        for( tplanet = first_planet; tplanet; tplanet = tplanet->next)
        {
            if( !str_cmp(tplanet->name, argument ) )
            {
                 pFound = true;
                 break;
            }
        }

         if( !pFound )
        {
//            ch->printlnf(ch, "&YThe planet &G%s &Ydoes not exist!\n\r", argument );
            return;
        }
    }

    if ( !str_cmp( arg2, "planet1" ) )
    {
//	STRFREE( starsystem->planet1 );
	starsystem->planet1 = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "planet2" ) )
    {
//	STRFREE( starsystem->planet2 );
	starsystem->planet2 = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "planet3" ) )
    {
//	STRFREE( starsystem->planet3 );
	starsystem->planet3 = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "location1a" ) )
    {
//	STRFREE( starsystem->location1a );
	starsystem->location1a = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "location1b" ) )
    {
//	STRFREE( starsystem->location1b );
	starsystem->location1b = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "location1c" ) )
    {
//	STRFREE( starsystem->location1c );
	starsystem->location1c = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "location2a" ) )
    {
//	STRFREE( starsystem->location2a  );
	starsystem->location2a = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "location2b" ) )
    {
//	STRFREE( starsystem->location2a );
	starsystem->location2b = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "location2c" ) )
    {
//	STRFREE( starsystem->location2c );
	starsystem->location2c = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }

    if ( !str_cmp( arg2, "location3a" ) )
    {
//	STRFREE( starsystem->location3a );
	starsystem->location3a = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "location3b" ) )
    {
//	STRFREE( starsystem->location3b );
	starsystem->location3b = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }
    if ( !str_cmp( arg2, "location3c" ) )
    {
//	STRFREE( starsystem->location3c );
	starsystem->location3c = str_dup( argument );
	ch->println( "Done.\n\r" );
	save_starsystem( starsystem );
	return;
    }


    do_setstarsystem( ch, "" );
    return;
}
*/
void showstarsystem( char_data *ch , SPACE_DATA *starsystem )
{

	ch->printlnf( "Starsystem:%s     Filename: %s    Xpos: %d   Ypos: %d\n\r",
    			starsystem->name,
    			starsystem->filename,
    			starsystem->xpos, starsystem->ypos);
    ch->printlnf( "Star1: %s   Gravity: %d   Coordinates: %d %d %d\n\r",
    			starsystem->star1, starsystem->gravitys1,
    			starsystem->s1x , starsystem->s1y, starsystem->s1z);
    ch->printlnf( "Star2: %s   Gravity: %d   Coordinates: %d %d %d\n\r",
    			starsystem->star2, starsystem->gravitys2,
    			starsystem->s2x , starsystem->s2y, starsystem->s2z);
    ch->printlnf( "Planet1: %s   Gravity: %d   Coordinates: %d %d %d\n\r",
    			starsystem->planet1, starsystem->gravityp1,
    			starsystem->p1x , starsystem->p1y, starsystem->p1z);
    ch->printlnf( "     Doc1a: %5d (%s)\n\r",
    			starsystem->doc1a, starsystem->location1a);
    ch->printlnf( "     Doc1b: %5d (%s)\n\r",
    			starsystem->doc1b, starsystem->location1b);
    ch->printlnf( "     Doc1c: %5d (%s)\n\r",
    			starsystem->doc1c, starsystem->location1c);
    ch->printlnf( "Planet2: %s   Gravity: %d   Coordinates: %d %d %d\n\r",
    			starsystem->planet2, starsystem->gravityp2,
    			starsystem->p2x , starsystem->p2y, starsystem->p2z);
    ch->printlnf( "     Doc2a: %5d (%s)\n\r",
    			starsystem->doc2a, starsystem->location2a);
    ch->printlnf( "     Doc2b: %5d (%s)\n\r",
    			starsystem->doc2b, starsystem->location2b);
    ch->printlnf( "     Doc2c: %5d (%s)\n\r",
    			starsystem->doc2c, starsystem->location2c);
    ch->printlnf( "Planet3: %s   Gravity: %d   Coordinates: %d %d %d\n\r",
    			starsystem->planet3, starsystem->gravityp3,
    			starsystem->p3x , starsystem->p3y, starsystem->p3z);
    ch->printlnf( "     Doc3a: %5d (%s)\n\r",
    			starsystem->doc3a, starsystem->location3a);
    ch->printlnf( "     Doc3b: %5d (%s)\n\r",
    			starsystem->doc3b, starsystem->location3b);
    ch->printlnf( "     Doc3c: %5d (%s)\n\r",
    			starsystem->doc3c, starsystem->location3c);
    return;
}

void do_showstarsystem( char_data *ch, char *argument )
{
   SPACE_DATA *starsystem;

   starsystem = starsystem_from_name( argument );

   if ( starsystem == NULL )
      ch->println("&RNo such starsystem.\n\r");
   else
      showstarsystem(ch , starsystem);

}/*

void do_makestarsystem( char_data *ch, char *argument )
{
    char arg[MIL];
    char filename[256];
    SPACE_DATA *starsystem;

    if ( !argument || argument[0] == '\0' )
    {
	ch->println( "Usage: makestarsystem <starsystem name>\n\r");
	return;
    }


//    CREATE( starsystem, SPACE_DATA, 1 );
    LINK( starsystem, first_starsystem, last_starsystem, next, prev );

    starsystem->name		= str_dup( argument );

		  starsystem->location1a            = str_dup( "" );
		  starsystem->location2a            = str_dup( "" );
		  starsystem->location3a            = str_dup( "" );
		  starsystem->location1b            = str_dup( "" );
		  starsystem->location2b            = str_dup( "" );
		  starsystem->location3b            = str_dup( "" );
		  starsystem->location1c            = str_dup( "" );
		  starsystem->location2c            = str_dup( "" );
		  starsystem->location3c            = str_dup( "" );
		  starsystem->planet1            = str_dup( "" );
		  starsystem->planet2            = str_dup( "" );
		  starsystem->planet3            = str_dup( "" );
		  starsystem->star1            = str_dup( "" );
		  starsystem->star2            = str_dup( "" );

    argument = one_argument( argument, arg );
//  sprintf( filename, "%s.system" , strlower(arg) );
    sprintf( filename, "%s.system",arg );
    starsystem->filename = str_dup( filename );
    save_starsystem( starsystem );
    write_starsystem_list();
}

void do_starsystems( char_data *ch, char *argument )
{
    SPACE_DATA *starsystem;
    int count = 0;

//        //set_char_color( AT_NOTE, ch );
        ch->println( "\n\r&R&w+&R&W--------------------------------&R&w+&R&W--------------&R&w+\n\r");
        ch->println( "&R&W|&Y Starsystem Name                &R&W|   &YLocation   &R&W|\n\r");
        ch->println( "&R&w+&R&W--------------------------------&R&w+&R&W--------------&R&w+\n\r");

    for ( starsystem = first_starsystem; starsystem; starsystem = starsystem->next )
    {
    	if(!str_cmp(starsystem->name, "Simulator"))
    	 continue;

        ch->printlnf("&R&W| &O%-30s&R&W | &Y%5d&O, &Y%-5d&R&W |\n\r", starsystem->name,
        	   starsystem->xpos, starsystem->ypos);
        count++;
    }
        ch->println( "&R&w+&R&W--------------------------------&R&w+&R&W--------------&R&w+\n\r");


    if ( !count )
    {
        ch->println( "There are no starsystems currently formed.\n\r" );
	return;
    }
}



void sound_to_ship( SHIP_DATA *ship , char *argument )
{
     int roomnum;
     ROOM_INDEX_DATA *room;
     char_data *vic;

     for ( roomnum = ship->firstroom ; roomnum <= ship->lastroom ;roomnum++ )
     {
        room = get_room_index( roomnum );
        if ( room == NULL ) continue;

        for ( vic = room->people; vic; vic = vic->next_in_room )
        {
	   if ( !IS_NPC(vic) *//*&& IS_SET( vic->act, PLR_SOUND )*/ /*)
	     vic->println( argument );
        }
     }

}

bool is_facing( SHIP_DATA *ship , SHIP_DATA *target )
{
    	float dy, dx, dz, hx, hy, hz;
        float cosofa;

     	hx = ship->hx;
     	hy = ship->hy;
     	hz = ship->hz;

     	     					dx = target->vx - ship->vx;
     	     						dy = target->vy - ship->vy;
     	     							dz = target->vz - ship->vz;

     	    cosofa = ( hx*dx + hy*dy + hz*dz )
     	     		/ ( sqrt(hx*hx+hy*hy+hz*hz) + sqrt(dx*dx+dy*dy+dz*dz) );

     	           	if ( cosofa > 0.75 )
     	                  	             return true;

     	     								 return false;
     	}


long int get_ship_value( SHIP_DATA *ship )
{
     long int price;

     price = ship->cost;

     return price;
}

void write_ship_list( )
{
    SHIP_DATA *tship;
    FILE *fpout;
    char filename[256];

    sprintf( filename, "%s%s", SHIPS_DIR, SHIPS_FILE );
    fpout = fopen( filename, "w" );
    if ( !fpout )
    {
         bug( "FATAL: cannot open ship.lst for writing!\n\r" );
         return;
    }
    for ( tship = first_ship; tship; tship = tship->next )
    fprintf( fpout, "%s\n", tship->filename );
    fprintf( fpout, "$\n" );
    fclose( fpout );
}

SHIP_DATA * ship_in_room( ROOM_INDEX_DATA *room, char *name )
{
    SHIP_DATA *ship;

    if ( !room )
     return NULL;

    for ( ship = room->ships ; ship ; ship = ship->next_in_room )
     if ( !str_cmp( name, ship->name ) )
         return ship;

    for ( ship = room->ships ; ship ; ship = ship->next_in_room )
     if ( !str_prefix( name, ship->name ) )
         return ship;

    return NULL;
}

*//*
 * Get pointer to ship structure from ship name.
 *//*
SHIP_DATA *get_ship( char *name )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( !str_cmp( name, ship->name ) )
         return ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( !str_prefix( name, ship->name ) )
         return ship;

    return NULL;
}
*/
/*
 * Checks if ships in a starsystem and returns poiner if it is.
 *//*
SHIP_DATA *get_ship_here( char *name , SPACE_DATA *starsystem)
{
    SHIP_DATA *ship;

    if ( starsystem == NULL )
         return NULL;

    for ( ship = starsystem->first_ship ; ship; ship = ship->next_in_starsystem )
       if ( !str_cmp( name, ship->name ) )
         return ship;

    for ( ship = starsystem->first_ship; ship; ship = ship->next_in_starsystem )
       if ( !str_prefix( name, ship->name ) )
         return ship;

    return NULL;
}


*//*
 * Get pointer to ship structure from ship name.
 *//*
SHIP_DATA *ship_from_pilot( char *name )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( !str_cmp( name, ship->pilot ) )
         return ship;
       if ( !str_cmp( name, ship->copilot ) )
         return ship;
       if ( !str_cmp( name, ship->owner ) )
         return ship;
    return NULL;
}


*//*
 * Get pointer to ship structure from cockpit, turret, or entrance ramp vnum.
 */
/*
SHIP_DATA *ship_from_cockpit( int vnum )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( vnum == ship->cockpit || vnum == ship->turret1 || vnum == ship->turret2
       || vnum == ship->turret3 || vnum == ship->turret4 || vnum == ship->turret5
       || vnum == ship->turret6 || vnum == ship->turret7 || vnum == ship->turret8
       || vnum == ship->turret9 || vnum == ship->turret10
       || vnum == ship->pilotseat || vnum == ship->coseat || vnum == ship->navseat
        || vnum == ship->gunseat  || vnum == ship->engineroom )
         return ship;
    return NULL;
}

SHIP_DATA *ship_from_pilotseat( int vnum )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( vnum == ship->pilotseat || vnum == ship->cockpit )
         return ship;
    return NULL;
}

SHIP_DATA *ship_from_coseat( int vnum )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( vnum == ship->coseat )
         return ship;
    return NULL;
}

SHIP_DATA *ship_from_navseat( int vnum )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( vnum == ship->navseat )
         return ship;
    return NULL;
}

SHIP_DATA *ship_from_gunseat( int vnum )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( vnum == ship->gunseat )
         return ship;
    return NULL;
}

SHIP_DATA *ship_from_engine( int vnum )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
    {
      if (ship->engineroom)
      {
        if ( vnum == ship->engineroom )
          return ship;
      }
      else
      {
        if ( vnum == ship->cockpit )
          return ship;
      }
    }

    return NULL;
}



SHIP_DATA *ship_from_turret( int vnum )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( vnum == ship->gunseat || vnum == ship->turret1 || vnum == ship->turret2
         || vnum == ship->turret3 || vnum == ship->turret4 || vnum == ship->turret5
         || vnum == ship->turret6 || vnum == ship->turret7 || vnum == ship->turret8
         || vnum == ship->turret9 || vnum == ship->turret10 )
         return ship;
    return NULL;
}

SHIP_DATA *ship_from_entrance( int vnum )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( vnum == ship->entrance )
         return ship;
    return NULL;
}

SHIP_DATA *ship_from_hanger( int vnum )
{
    SHIP_DATA *ship;

    for ( ship = first_ship; ship; ship = ship->next )
       if ( vnum == ship->hanger1 || vnum == ship->hanger2
		 || vnum == ship->hanger3 || vnum == ship->hanger4)
         return ship;
    return NULL;
}

*//*
void save_ship( SHIP_DATA *ship )
{
    FILE *fp;
    char filename[256];
    char buf[MSL];
    MODULE_DATA *mod;

    if ( !ship )
    {
	bug( "save_ship: null ship pointer!" );
	return;
    }

    if ( !ship->filename || ship->filename[0] == '\0' )
    {
	sprintf( buf, "save_ship: %s has no filename", ship->name );
	bug( buf );
	return;
    }

    sprintf( filename, "%s%s", SHIPS_DIR, ship->filename );

    fclose( fpReserve );
    if ( ( fp = fopen( filename, "w" ) ) == NULL )
    {
    	bug( "save_ship: fopen", 0 );
    	perror( filename );
    }
    else
    {
	fprintf( fp, "#SHIP\n" );
	fprintf( fp, "Name         %s~\n",	ship->name		);
	fprintf( fp, "Filename     %s~\n",	ship->filename		);
	fprintf( fp, "Protoname    %s~\n",	ship->protoname		);
	fprintf( fp, "PBeacon      %s~\n",	ship->pbeacon);
	fprintf( fp, "ClanOwner    %s~\n",      ship->clanowner		);
        fprintf( fp, "Description  %s~\n",	ship->description	);
	fprintf( fp, "Owner        %s~\n",	ship->owner		);
	fprintf( fp, "Pilot        %s~\n",      ship->pilot             );
	fprintf( fp, "Copilot      %s~\n",      ship->copilot           );
	fprintf( fp, "Class        %d\n",	ship->ship_class		);
	fprintf( fp, "primaryType        %d\n", ship->primaryType );
	fprintf( fp, "primaryCount       %d\n",	ship->primaryCount    	);
	fprintf( fp, "secondaryType      %d\n", ship->secondaryType  );
	fprintf( fp, "secondaryCount	 %d\n",	ship->secondaryCount		);
        fprintf( fp, "Alarm        %d\n",	ship->alarm		);
        fprintf( fp, "Maxmod       %d\n",	ship->maxmod		);
	fprintf( fp, "Tractorbeam  %d\n",	ship->tractorbeam	);
	fprintf( fp, "Shipyard     %d\n",	ship->shipyard		);
	fprintf( fp, "Turret1      %d\n",	ship->turret1		);
	fprintf( fp, "Turret2      %d\n",	ship->turret2		);
	fprintf( fp, "Turret3      %d\n",	ship->turret3		);
	fprintf( fp, "Turret4	   %d\n",	ship->turret4		);
	fprintf( fp, "Turret5	   %d\n",   ship->turret5		);
	fprintf( fp, "Turret6      %d\n",   ship->turret6       );
	fprintf( fp, "Turret7      %d\n",   ship->turret7		);
	fprintf( fp, "Turret8      %d\n",   ship->turret8       );
	fprintf( fp, "Turret9      %d\n",   ship->turret9       );
	fprintf( fp, "Turret10     %d\n",   ship->turret10      );
	fprintf( fp, "Statet1      %d\n",	ship->statet1		);
	fprintf( fp, "Statet2      %d\n",	ship->statet2		);
	fprintf( fp, "Statet3      %d\n",	ship->statet3		);
	fprintf( fp, "Statet4      %d\n",	ship->statet4		);
	fprintf( fp, "Statet5      %d\n",	ship->statet5		);
	fprintf( fp, "Statet6      %d\n",	ship->statet6		);
	fprintf( fp, "Statet7      %d\n",	ship->statet7		);
	fprintf( fp, "Statet8      %d\n",	ship->statet8		);
	fprintf( fp, "Statet9      %d\n",	ship->statet9		);
	fprintf( fp, "Statet10     %d\n",	ship->statet10		);
	fprintf( fp, "Bombs	   %d\n",	ship->bombs		);
	fprintf( fp, "Hibombstr	   %d\n",	ship->hibombstr		);
	fprintf( fp, "lowbombstr   %d\n",	ship->lowbombstr	);
	fprintf( fp, "Maxbombs     %d\n",	ship->maxbombs		);
	fprintf( fp, "Missiles     %d\n",	ship->missiles		);
	fprintf( fp, "Maxmissiles  %d\n",	ship->maxmissiles	);
	fprintf( fp, "Rockets     %d\n",	ship->rockets		);
	fprintf( fp, "Maxrockets  %d\n",	ship->maxrockets	);
	fprintf( fp, "Torpedos     %d\n",	ship->torpedos		);
	fprintf( fp, "Maxtorpedos  %d\n",	ship->maxtorpedos	);
	fprintf( fp, "Lastdoc      %d\n",	ship->lastdoc		);
	fprintf( fp, "Firstroom    %d\n",	ship->firstroom		);
	fprintf( fp, "Lastroom     %d\n",	ship->lastroom		);
	fprintf( fp, "Hanger1      %d\n",   ship->hanger1		);
	fprintf( fp, "Hanger2      %d\n",   ship->hanger2		);
	fprintf( fp, "Hanger3      %d\n",   ship->hanger3		);
	fprintf( fp, "Hanger4      %d\n",   ship->hanger4		);
	fprintf( fp, "Shield       %d\n",	ship->shield		);
	fprintf( fp, "Maxshield    %d\n",	ship->maxshield		);
	fprintf( fp, "Hull         %d\n",	ship->hull		);
	fprintf( fp, "Maxhull      %d\n",	ship->maxhull		);
	fprintf( fp, "Maxenergy    %d\n",	ship->maxenergy		);
	fprintf( fp, "Hyperspeed   %d\n",	ship->hyperspeed	);
	fprintf( fp, "Comm         %d\n",	ship->comm		);
	fprintf( fp, "Cost	   %d\n",       ship->cost		);
	fprintf( fp, "Chaff        %d\n",	ship->chaff		);
	fprintf( fp, "Maxchaff     %d\n",	ship->maxchaff		);
	fprintf( fp, "Sensor       %d\n",	ship->sensor		);
	fprintf( fp, "Astro_array  %d\n",	ship->astro_array	);
	fprintf( fp, "Realspeed    %d\n",	ship->realspeed		);
	fprintf( fp, "Type         %d\n",	ship->type		);
	fprintf( fp, "Cockpit      %d\n",	ship->cockpit		);
	fprintf( fp, "Coseat       %d\n",	ship->coseat		);
	fprintf( fp, "Pilotseat    %d\n",	ship->pilotseat		);
	fprintf( fp, "Gunseat      %d\n",	ship->gunseat		);
	fprintf( fp, "Navseat      %d\n",	ship->navseat		);
	fprintf( fp, "Engineroom   %d\n",       ship->engineroom        );
	fprintf( fp, "Entrance     %d\n",       ship->entrance          );
	fprintf( fp, "Shipstate    %d\n",	ship->shipstate		);
	fprintf( fp, "Missilestate %d\n",	ship->missilestate	);
	fprintf( fp, "Energy       %d\n",	ship->energy		);
	fprintf( fp, "Manuever     %d\n",       ship->manuever          );
	fprintf( fp, "Password     %d\n",       ship->password          );
#ifdef USECARGO
	fprintf( fp, "MaxCargo     %d\n",       ship->maxcargo          );
	if (ship->cargo > 0)
	{
	   fprintf( fp, "Cargo     %d\n",       ship->cargo          );
	   fprintf( fp, "CargoType %d\n",       ship->cargotype          );
	}
#endif
	fprintf( fp, "Home         %s~\n",      ship->home              );
        for(mod=ship->first_module;mod;mod=mod->next)
          fprintf( fp, "Module     %d %d\n",    mod->affect, mod->ammount);

    fprintf( fp, "Simulator    %d %d\n",
				IS_SET(ship->flags, SHIP_SIMULATOR) ? 1 : 0,
				IS_SET(ship->flags, SHIP_SIMULATOR) ? ship->sim_vnum : 0);

	 fprintf( fp, "End\n\n"						 );
	fprintf( fp, "#END\n"						);
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}


*//*
 * Read in actual ship data.
 */
/*
#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = true;			\
				    break;				\
				}

void fread_ship( SHIP_DATA *ship, FILE *fp )
{
    char buf[MSL];
    char *word;
    bool fMatch;
    int dummy_number;
    int x1, x2;
    char *line;

    for ( ; ; )
    {
	word   = feof( fp ) ? (char*)"End" : fread_word( fp );
	fMatch = false;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = true;
	    fread_to_eol( fp );
	    break;

        case 'A':
             KEY( "Alarm",            ship->alarm,             fread_number( fp ) );
             KEY( "Astro_array",      ship->astro_array,       fread_number( fp ) );
             break;

        case 'B':
             KEY( "Bombs",      ship->bombs,       	fread_number( fp ) );
             break;

        case 'C':
             KEY( "Cockpit",     ship->cockpit,          fread_number( fp ) );
             KEY( "Coseat",     ship->coseat,          fread_number( fp ) );
             KEY( "ClanOwner",  ship->clanowner,	fread_string(fp));
             KEY( "Class",       ship->ship_class,            fread_number( fp ) );
             KEY( "Copilot",     ship->copilot,          fread_string( fp ) );
             KEY( "Comm",        ship->comm,      fread_number( fp ) );
             KEY( "Cost",	 ship->cost,	  fread_number(fp));
             KEY( "Chaff",       ship->chaff,      fread_number( fp ) );
#ifdef USECARGO
             KEY( "Cargo",       ship->cargo,      fread_number( fp ) );
             KEY( "CargoType",   ship->cargotype,  fread_number( fp ) );
#endif
             break;


	case 'D':
	    KEY( "Description",	ship->description,	fread_string( fp ) );
	    break;

	case 'E':
	    KEY( "Engineroom",    ship->engineroom,      fread_number( fp ) );
	    KEY( "Entrance",	ship->entrance,	        fread_number( fp ) );
	    KEY( "Energy",      ship->energy,        fread_number( fp ) );
	    if ( !str_cmp( word, "End" ) )
	    {
		if (!ship->home)
		  ship->home		= str_dup( "" );
		if (!ship->name)
		  ship->name		= str_dup( "" );
		if (!ship->owner)
		  ship->owner		= str_dup( "" );
		if (!ship->description)
		  ship->description 	= str_dup( "" );
		if (!ship->copilot)
		  ship->copilot 	= str_dup( "" );
		if (!ship->pilot)
		  ship->pilot   	= str_dup( "" );
#ifdef USECARGO
                if (ship->cargotype != CARGO_NONE && ship->cargo < 1)
                   ship->cargotype = CARGO_NONE;
#endif

		if (ship->shipstate != SHIP_DISABLED)
		  ship->shipstate = SHIP_DOCKED;

		  ship->primaryState = LASER_READY;
		  ship->secondaryState = LASER_READY;
		if (ship->statet1 != LASER_DAMAGED)
		  ship->statet1 = LASER_READY;
		if (ship->statet2 != LASER_DAMAGED)
		  ship->statet2 = LASER_READY;
		if (ship->missilestate != MISSILE_DAMAGED)
		  ship->missilestate = MISSILE_READY;
		if (ship->shipyard <= 0)
		  ship->shipyard = ROOM_LIMBO_SHIPYARD;
		if (ship->lastdoc <= 0)
		  ship->lastdoc = ship->shipyard;
		ship->bayopen     = false;
	   if(ship->ship_class >= SHIP_CRUISER)
		ship->autopilot   = true;
	   else
	    ship->autopilot   = false;

		ship->hatchopen = false;
		if (ship->navseat <= 0)
		  ship->navseat = ship->cockpit;
		if (ship->gunseat <= 0)
		  ship->gunseat = ship->cockpit;
		if (ship->coseat <= 0)
		  ship->coseat = ship->cockpit;
		if (ship->pilotseat <= 0)
		  ship->pilotseat = ship->cockpit;
		if (ship->missiletype == 1)
		{
*		  ship->torpedos = ship->missiles;   */ /* for back compatability *//*
		  ship->missiles = 0;
		}
		ship->starsystem = NULL;
		ship->energy = ship->maxenergy;
		ship->hull = ship->maxhull;
		ship->in_room=NULL;
                ship->next_in_room=NULL;
                ship->prev_in_room=NULL;

		return;
	    }
	    break;

	case 'F':
	    KEY( "Filename",	ship->filename,		fread_string( fp ) );
            KEY( "Firstroom",   ship->firstroom,        fread_number( fp ) );
            break;

        case 'G':
            KEY( "Gunseat",     ship->gunseat,          fread_number( fp ) );
            break;

        case 'H':
            KEY( "Home" , ship->home, fread_string( fp ) );
            KEY( "Hyperspeed",   ship->hyperspeed,      fread_number( fp ) );
            KEY( "Hibombstr", ship->hibombstr,   fread_number( fp ) );
            KEY( "Hull",      ship->hull,        fread_number( fp ) );
            KEY( "Hanger1",  ship->hanger1,      fread_number( fp ) );
            KEY( "Hanger2",  ship->hanger2,      fread_number( fp ) );
            KEY( "Hanger3",  ship->hanger3,      fread_number( fp ) );
            KEY( "Hanger4",  ship->hanger4,      fread_number( fp ) );
            break;
        case 'L':
            KEY( "Lastdoc",    ship->lastdoc,       fread_number( fp ) );
            KEY( "Lastroom",   ship->lastroom,        fread_number( fp ) );
            KEY( "Lowbombstr", ship->lowbombstr,   fread_number( fp ) );
            break;

        case 'M':
            KEY( "Manuever",   ship->manuever,      fread_number( fp ) );
            KEY( "Maxbombs",   ship->maxbombs,      fread_number( fp ) );
            KEY( "Maxmissiles",   ship->maxmissiles,      fread_number( fp ) );
            KEY( "Maxmod",   ship->maxmod,      fread_number( fp ) );
            KEY( "Maxtorpedos",   ship->maxtorpedos,      fread_number( fp ) );
            KEY( "Maxrockets",   ship->maxrockets,      fread_number( fp ) );
#ifdef USECARGO
             KEY( "MaxCargo",    ship->maxcargo,   fread_number( fp ) );
#endif

            KEY( "Missiles",   ship->missiles,      fread_number( fp ) );
            KEY( "Missiletype",   ship->missiletype,      fread_number( fp ) );
            KEY( "Maxshield",      ship->maxshield,        fread_number( fp ) );
            KEY( "Maxenergy",      ship->maxenergy,        fread_number( fp ) );
            KEY( "Missilestate",   ship->missilestate,        fread_number( fp ) );
            KEY( "Maxhull",      ship->maxhull,        fread_number( fp ) );
            KEY( "Maxchaff",       ship->maxchaff,      fread_number( fp ) );
*//*            if ( !str_cmp( word, "Module"  ) ){
                MODULE_DATA *mod;
                line = fread_line( fp );
                CREATE(mod, MODULE_DATA, 1);
                LINK(mod, ship->first_module, ship->last_module, next, prev);
                x1=x2=0;
                sscanf( line, "%d %d",
                      &x1, &x2 );
                mod->affect = x1;
                mod->ammount = x2;
                fMatch = true;
                break;
            }*//*
             break;

	case 'N':
	    KEY( "Name",	ship->name,		fread_string( fp ) );
	    KEY( "Navseat",     ship->navseat,          fread_number( fp ) );
            break;

        case 'O':
            KEY( "Owner",            ship->owner,            fread_string( fp ) );
            KEY( "Objectnum",        dummy_number,        fread_number( fp ) );
            break;

        case 'P':
            KEY( "Password",     ship->password,          fread_number( fp ) );
            KEY( "Pilot",            ship->pilot,            fread_string( fp ) );
            KEY( "Pilotseat",     ship->pilotseat,          fread_number( fp ) );
            KEY( "Protoname",	 ship->protoname,	fread_string(fp));
			KEY( "primaryCount", ship->primaryCount, fread_number(fp));
			KEY( "primaryType", ship->primaryType, fread_number(fp));
	    KEY( "PBeacon",  ship->pbeacon,	fread_string(fp));
            break;

        case 'R':
            KEY( "Realspeed",   ship->realspeed,       fread_number( fp ) );
            KEY( "Rockets",     ship->rockets,         fread_number( fp ) );
            break;

        case 'S':
            KEY( "Shipyard",    ship->shipyard,      fread_number( fp ) );
            KEY( "Sensor",      ship->sensor,       fread_number( fp ) );
			KEY( "secondaryType", ship->secondaryType, fread_number(fp));
			KEY( "secondaryCount", ship->secondaryCount, fread_number(fp));
            KEY( "Shield",      ship->shield,        fread_number( fp ) );
            KEY( "Shipstate",   ship->shipstate,        fread_number( fp ) );
            KEY( "Statet1",   ship->statet1,        fread_number( fp ) );
            KEY( "Statet2",   ship->statet2,        fread_number( fp ) );
			KEY( "Statet3",   ship->statet3,        fread_number( fp ) );
			KEY( "Statet4",   ship->statet4,        fread_number( fp ) );
			KEY( "Statet5",   ship->statet5,        fread_number( fp ) );
			KEY( "Statet6",   ship->statet6,        fread_number( fp ) );
			KEY( "Statet7",   ship->statet7,        fread_number( fp ) );
			KEY( "Statet8",   ship->statet8,        fread_number( fp ) );
			KEY( "Statet9",   ship->statet9,        fread_number( fp ) );
			KEY( "Statet10",   ship->statet10,        fread_number( fp ) );
*//*			if(!str_cmp(word, "Simulator"))
			{
				line = fread_line(fp);
				x1=x2=0;
                sscanf( line, "%d %d", &x1, &x2);
                if(x1 == 1){  SET_BIT(ship->flags, SHIP_SIMULATOR); ship->sim_vnum = x2; }
                else ship->sim_vnum = 0;
				fMatch = true;
            }*//*

            break;

	case 'T':
	    KEY( "Type",	ship->type,	fread_number( fp ) );
	    KEY( "Tractorbeam", ship->tractorbeam,      fread_number( fp ) );
	    KEY( "Turret1",	ship->turret1,	fread_number( fp ) );
	    KEY( "Turret2",	ship->turret2,	fread_number( fp ) );
		KEY( "Turret3",	ship->turret3,	fread_number( fp ) );
		KEY( "Turret4",	ship->turret4,	fread_number( fp ) );
		KEY( "Turret5",	ship->turret5,	fread_number( fp ) );
		KEY( "Turret6",	ship->turret6,	fread_number( fp ) );
		KEY( "Turret7",	ship->turret7,	fread_number( fp ) );
		KEY( "Turret8",	ship->turret8,	fread_number( fp ) );
		KEY( "Turret9",	ship->turret9,	fread_number( fp ) );
		KEY( "Turret10",	ship->turret10,	fread_number( fp ) );
	    KEY( "Torpedos",	ship->torpedos,	fread_number( fp ) );
	    break;
	}

	if ( !fMatch )
	{
	    sprintf( buf, "Fread_ship: no match: %s", word );
	    bug( buf );
	}
    }
}

void ship_to_starsystem( SHIP_DATA *ship , SPACE_DATA *starsystem )
{
     if ( starsystem == NULL )
        return;

     if ( ship == NULL )
        return;

     if ( starsystem->first_ship == NULL )
        starsystem->first_ship = ship;

     if ( starsystem->last_ship )
     {
         starsystem->last_ship->next_in_starsystem = ship;
         ship->prev_in_starsystem = starsystem->last_ship;
         ship->next_in_starsystem = NULL;
     }

     starsystem->last_ship = ship;

     ship->starsystem = starsystem;

}

*//*
 * Load a ship file
 */
/*
bool load_ship_file( char *shipfile )
{
    char filename[256];
    SHIP_DATA *ship;
    FILE *fp;
    bool found;
    ROOM_INDEX_DATA *pRoomIndex;
//    CLAN_DATA *clan;


//    CREATE( ship, SHIP_DATA, 1 );

    found = false;
    sprintf( filename, "%s%s", SHIPS_DIR, shipfile );

    if ( ( fp = fopen( filename, "r" ) ) != NULL )
    {

	found = true;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_ship_file: # not found." );
		break;
	    }

	    word = fread_word( fp );
	    if ( !str_cmp( word, "SHIP"	) )
	    {
	    	fread_ship( ship, fp );
	    	break;
	    }
	    else
	    if ( !str_cmp( word, "END"	) )
	        break;
	    else
	    {
		char buf[MSL];

		sprintf( buf, "Load_ship_file: bad section: %s.", word );
		bug( buf );
		break;
	    }
	}
	fclose( fp );
    }
    if ( !(found) )
//      DISPOSE( ship );
//    else
    {
       LINK( ship, first_ship, last_ship, next, prev );
       if ( !str_cmp("Public",ship->owner) || ship->type == MOB_SHIP )
       {

         if ( ship->ship_class <= SHIP_CORVETTE && ship->type != MOB_SHIP )
         {
           extract_ship( ship );
           ship_to_room( ship , ship->shipyard );

           ship->location = ship->shipyard;
           ship->lastdoc = ship->shipyard;
           ship->shipstate = SHIP_DOCKED;
           }

  if ( IS_SET(ship->flags, XSHIP_ION_DRIVE) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_DRIVE);
    ship->shipstate = SHIP_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_HYPER) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_HYPER);
    ship->shipstate = LASER_READY;

  }

  if ( IS_SET(ship->flags, XSHIP_ION_ION) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_ION);
    ship->secondaryState = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_LASERS) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_LASERS);
    ship->primaryState = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_MISSILES) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_MISSILES);
    ship->missilestate = MISSILE_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET1) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET1);
    ship->turret1 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET2) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET2);
    ship->turret2 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET3) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET3);
    ship->turret3 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET4) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET4);
    ship->turret4 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET5) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET5);
    ship->turret5 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET6) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET6);
    ship->turret6 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET7) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET7);
    ship->turret7 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET8) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET8);
    ship->turret8 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET9) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET9);
    ship->turret9 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET10) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET10);
    ship->turret10 = LASER_READY;
  }

     ship->currspeed=0;
     ship->energy=ship->maxenergy;
     ship->chaff=ship->maxchaff;
     ship->hull=ship->maxhull;
     ship->shield=0;

     ship->statet1 = LASER_READY;
     ship->statet2 = LASER_READY;
     ship->primaryState = LASER_READY;
	 ship->secondaryState = LASER_READY;
     ship->missilestate = LASER_READY;

     ship->currjump=NULL;
     ship->target0=NULL;
     ship->target1=NULL;
     ship->target2=NULL;

     ship->hatchopen = false;
     ship->bayopen = false;
	 ship->bayopen = false;

     ship->missiles = ship->maxmissiles;
     ship->torpedos = ship->maxtorpedos;
     ship->rockets = ship->maxrockets;
     ship->autorecharge = false;
     ship->autotrack = false;
     ship->autospeed = false;


       }

*//*       else if ( ship->cockpit == ROOM_SHUTTLE_BUS ||
                 ship->cockpit == ROOM_SHUTTLE_BUS_2 ||
                 ship->cockpit == ROOM_SENATE_SHUTTLE ||
                 ship->cockpit == ROOM_CORUSCANT_TURBOCAR ||
                 ship->cockpit == ROOM_CORUSCANT_SHUTTLE   )
       {}*//*
       else if ( ( pRoomIndex = get_room_index( ship->lastdoc ) ) != NULL
            && ship->ship_class <= SHIP_CORVETTE )
       {
//              LINK( ship, pRoomIndex->first_ship, pRoomIndex->last_ship, next_in_room, prev_in_room );
              ship->in_room = pRoomIndex;
              ship->location = ship->lastdoc;
       }


       if ( ship->ship_class >= SHIP_CRUISER || ship->type == MOB_SHIP )
       {
          ship_to_starsystem(ship, starsystem_from_name(ship->home) );
		  if(ship->ship_class >= SHIP_CRUISER)
		  {

			 if(!str_cmp(ship->home, "Coruscant") || !str_cmp(ship->home, "coruscant"))
			  {
           ship->vx = -345;
           ship->vy = -987;
           ship->vz = 1562;
		      }
			else if(!str_cmp(ship->home, "Calamari") || !str_cmp(ship->home, "calamari"))
			{
				ship->vx = 3300;
				ship->vy = 1300;
				ship->vz = 1700;
			}
			else
			  {
			ship->vx = number_range(-5000, 5000);
			ship->vy = number_range(-5000, 5000);
			ship->vz = number_range(-5000, 5000);
			  }
		  }

           ship->hx = 1;
           ship->hy = 1;
           ship->hz = 1;
           ship->shipstate = SHIP_READY;
           ship->autopilot = true;
           ship->autorecharge = true;
           ship->shield = ship->maxshield;

		  if(ship->type == MOB_SHIP)
		   {
		if(!str_cmp(ship->home, "Coruscant") || !str_cmp(ship->home, "coruscant"))
	    {
          if(number_range(1,2) == 1)
           ship->vx = (-345)+(number_range(50, 150));
		  else
		   ship->vx = (-345)-(number_range(50, 150));
		  if(number_range(1,2) == 1)
           ship->vy = (-987)+(number_range(50, 150));
		  else
	       ship->vy = (-987)-(number_range(50, 150));
		  if(number_range(1,2) == 1)
		   ship->vz = (1562)+(number_range(50, 150));
		  else
           ship->vz = (1562)-(number_range(50, 150));
	    }
		else if(!str_cmp(ship->home, "Calamari") || !str_cmp(ship->home, "calamari"))
		{
          if(number_range(1,2) == 1)
           ship->vx = (3300)+(number_range(50, 150));
		  else
		   ship->vx = (3300)-(number_range(50, 150));
		  if(number_range(1,2) == 1)
           ship->vy = (1300)+(number_range(50, 150));
		  else
	       ship->vy = (1300)-(number_range(50, 150));
		  if(number_range(1,2) == 1)
		   ship->vz = (1700)+(number_range(50, 150));
		  else
           ship->vz = (1700)-(number_range(50, 150));

		}
	    else
			   {
				   ship->vx = number_range(-5000, 5000);
				   ship->vy = number_range(-5000, 5000);
				   ship->vz = number_range(-5000, 5000);
			   }

           ship->hx = 1;
           ship->hy = 1;
           ship->hz = 1;
           ship->shipstate = SHIP_READY;
           ship->autopilot = true;
           ship->autorecharge = true;
           ship->shield = ship->maxshield;
		   }
       }

       if ( !ship->password )
 	 ship->password = number_range(1111, 9999);

         if ( ship->type != MOB_SHIP *//*&& (clan = get_clan( ship->owner )) != NULL*//* )
         {*//*
          if ( ship->ship_class != LAND_VEHICLE )
             clan->spacecraft++;
          else
             clan->vehicles++;
         }

    }

    return found;
}

/*
 * Load in all the ship files.
 *//*
void load_ships( )
{
    FILE *fpList;
    char *filename;
    char shiplist[256];
    char buf[MSL];


    first_ship	= NULL;
    last_ship	= NULL;
    first_missile = NULL;
    last_missile = NULL;

    log_string( "Loading ships..." );

    sprintf( shiplist, "%s%s", SHIPS_DIR, SHIPS_FILE );
    fclose( fpReserve );
    if ( ( fpList = fopen( shiplist, "r" ) ) == NULL )
    {
	perror( shiplist );
	exit( 1 );
    }

    for ( ; ; )
    {

	filename = feof( fpList ) ? (char*)"$" : fread_word( fpList );

	if ( filename[0] == '$' )
	  break;

	if ( !load_ship_file( filename ) )
	{
	  sprintf( buf, "Cannot load ship file: %s", filename );
	  bug( buf );
	}

    }
    fclose( fpList );
    log_string(" Done ships " );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

void untargetship( SHIP_DATA *target )
{
  SHIP_DATA *ship;
  MISSILE_DATA *missile;

  for ( ship = first_ship; ship; ship = ship->next ) {
    if (ship == target)
 continue;
    if (ship->target0 == target)
 ship->target0 = NULL;
    if (ship->target1 == target)
 ship->target1 = NULL;
    if (ship->target2 == target)
 ship->target2 = NULL;
    if (ship->target3 == target)
 ship->target3 = NULL;
    if (ship->target4 == target)
 ship->target4 = NULL;
    if (ship->target5 == target)
 ship->target5 = NULL;
    if (ship->target6 == target)
 ship->target6 = NULL;
    if (ship->target7 == target)
 ship->target7 = NULL;
    if (ship->target8 == target)
 ship->target8 = NULL;
    if (ship->target9 == target)
 ship->target9 = NULL;
    if (ship->target10 == target)
 ship->target10 = NULL;
  }
for (missile = first_missile; missile; missile = missile->next)
    if (missile->target == target)
 missile->target = NULL;
}


void resetship( SHIP_DATA *ship )
{
    untargetship(ship);
    ship->shipstate = SHIP_READY;

     if ( ship->ship_class != SHIP_SPACE_STATION && ship->type != MOB_SHIP )
     {
           extract_ship( ship );
           ship_to_room( ship , ship->shipyard );

           ship->location = ship->shipyard;
           ship->lastdoc = ship->shipyard;
           ship->shipstate = SHIP_DOCKED;
     }

     if (ship->starsystem)
        ship_from_starsystem( ship, ship->starsystem );

     ship->currspeed=0;
     ship->energy=ship->maxenergy;
     ship->chaff=ship->maxchaff;
     ship->hull=ship->maxhull;
     ship->shield=0;

     ship->statet1 = LASER_READY;
     ship->statet2 = LASER_READY;
     ship->primaryState = LASER_READY;
	 ship->secondaryState = LASER_READY;
     ship->missilestate = LASER_READY;

     ship->currjump=NULL;
     ship->target0=NULL;
     ship->target1=NULL;
     ship->target2=NULL;

     ship->hatchopen = false;
     ship->bayopen = false;

     ship->missiles = ship->maxmissiles;
     ship->torpedos = ship->maxtorpedos;
     ship->rockets = ship->maxrockets;
     ship->autorecharge = false;
     ship->autotrack = false;
     ship->autospeed = false;

  if ( IS_SET(ship->flags, XSHIP_ION_DRIVE) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_DRIVE);
    ship->shipstate = SHIP_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_HYPER) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_HYPER);
    ship->shipstate = LASER_READY;

  }

  if ( IS_SET(ship->flags, XSHIP_ION_ION) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_ION);
    ship->secondaryState = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_LASERS) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_LASERS);
    ship->primaryState = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_MISSILES) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_MISSILES);
    ship->missilestate = MISSILE_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET1) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET1);
    ship->turret1 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET2) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET2);
    ship->turret2 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET3) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET3);
    ship->turret3 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET4) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET4);
    ship->turret4 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET5) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET5);
    ship->turret5 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET6) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET6);
    ship->turret6 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET7) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET7);
    ship->turret7 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET8) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET8);
    ship->turret8 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET9) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET9);
    ship->turret9 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET10) )
  {
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET10);
    ship->turret10 = LASER_READY;
  }

     if ( str_cmp("Public",ship->owner) && ship->type != MOB_SHIP )
     {
  //      CLAN_DATA *clan;

        if ( ship->type != MOB_SHIP /*&& (clan = get_clan( ship->owner )) != NULL*//* )
        {
        /*  if ( ship->ship_class != LAND_VEHICLE )
          //   clan->spacecraft--;
          else
             clan->vehicles--;*/
        //}

//        STRFREE( ship->owner );
/*        ship->owner = str_dup( "" );
//        STRFREE( ship->pilot );
        ship->pilot = str_dup( "" );
//        STRFREE( ship->copilot );
        ship->copilot = str_dup( "" );
     }


     save_ship(ship);
}
*//*
void do_resetship( char_data *ch, char *argument )
{
     SHIP_DATA *ship;

     ship = get_ship( argument );
     if (ship == NULL)
     {
        ch->println("&RNo such ship!");
        return;
     }

     resetship( ship );

     if ( ( ship->ship_class >=  SHIP_CRUISER || ship->type == MOB_SHIP )
          && ship->home )
     {
          ship_to_starsystem(ship, starsystem_from_name(ship->home) );
          ship->vx = number_range( -5000 , 5000 );
          ship->vy = number_range( -5000 , 5000 );
          ship->vz = number_range( -5000 , 5000 );
          ship->shipstate = SHIP_READY;
          ship->autopilot = true;
          ship->autorecharge = true;
          ship->shield = ship->maxshield;
     }

}

void do_setship( char_data *ch, char *argument )
{
    char arg1[MIL];
    char arg2[MIL];
    SHIP_DATA *ship;
    int  tempnum;
    ROOM_INDEX_DATA *roomindex;

    if ( IS_NPC( ch ) )
    {
	ch->println( "Huh?\n\r" );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg1[0] == '\0' )
    {
	ch->println( "Usage: setship <ship> <field> <values>\n\r" );
	ch->println( "\n\rField being one of:\n\r" );
	ch->println( "filename name owner copilot pilot description home\n\r" );
	ch->println( "cockpit entrance turret1 turret2 turret3 turret4\n\r" );
	ch->println( "turret5 turret6 turret7 turret8 turret9 turret10\n\r");
	ch->println( "hanger engineroom firstroom lastroom shipyard\n\r" );
	ch->println( "manuever speed hyperspeed tractorbeam\n\r" );
	ch->println( "pcount primary scount secondary missiles shield hull energy chaff\n\r" );
	ch->println( "comm sensor astroarray class torpedos bombs\n\r" );
#ifdef USECARGO
        ch->println( "pilotseat coseat gunseat navseat rockets simvnum maxcargo\n\r", ch );
#else
        ch->println( "pilotseat coseat gunseat navseat rockets simvnum\n\r" );
#endif
	return;
    }

    ship = get_ship( arg1 );
    if ( !ship )
    {
	ch->println( "No such ship.\n\r" );
	return;
    }

    if ( !str_cmp( arg2, "owner" ) )
    {
//         CLAN_DATA *clan;
         if ( ship->type != MOB_SHIP /*&& (clan = get_clan( ship->owner )) != NULL*/ /*)
         {/*
          if ( ship->ship_class != LAND_VEHICLE )
             clan->spacecraft--;
          else
             clan->vehicles--;
         *//*}
	//STRFREE( ship->owner );
	ship->owner = str_dup( argument );
	ch->println( "Done.\n\r");
	save_ship( ship );
	if ( ship->type != MOB_SHIP/* && (clan = get_clan( ship->owner )) != NULL*//* )
         {/*
          if ( ship->ship_class != LAND_VEHICLE )
             clan->spacecraft++;
          else
             clan->vehicles++;
         *///}
/*	return;
    }

    if ( !str_cmp( arg2, "home" ) )
    {
	//STRFREE( ship->home );
	ship->home = str_dup( argument );
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "pilot" ) )
    {
//	STRFREE( ship->pilot );
	ship->pilot = str_dup( argument );
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "copilot" ) )
    {
//	STRFREE( ship->copilot );
	ship->copilot = str_dup( argument );
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "firstroom" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}
	ship->firstroom = tempnum;
        ship->lastroom = tempnum;
        ship->cockpit = tempnum;
        ship->coseat = tempnum;
        ship->pilotseat = tempnum;
        ship->gunseat = tempnum;
        ship->navseat = tempnum;
        ship->entrance = tempnum;
        ship->turret1 = 0;
        ship->turret2 = 0;
        ship->turret3 = 0;
        ship->turret4 = 0;
        ship->turret5 = 0;
        ship->turret6 = 0;
        ship->turret7 = 0;
        ship->turret8 = 0;
        ship->turret9 = 0;
        ship->turret10 = 0;
        ship->hanger1 = 0; ship->hanger2 = 0; ship->hanger3 = 0; ship->hanger4 = 0;
	ch->println( "You will now need to set the other rooms in the ship.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "lastroom" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}
    	if ( tempnum < ship->firstroom )
    	{
    	   ch->println("The last room on a ship must be greater than or equal to the first room.\n\r");
           return;
    	}
	ship->lastroom = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "cockpit" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->cockpit = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

      if ( !str_cmp( arg2, "primary" ) )
      {
          ship->primaryType = atoi(argument);
          ch->println( "Done.\n\r");
          save_ship( ship );
          return;
      }

      if ( !str_cmp( arg2, "secondary" ) )
      {
         ship->secondaryType = atoi(argument);
         ch->println( "Done.\n\r");
         save_ship( ship );
         return;
      }
    if ( !str_cmp( arg2, "pilotseat" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->pilotseat = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
    if ( !str_cmp( arg2, "coseat" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->coseat = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
    if ( !str_cmp( arg2, "navseat" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->navseat = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
    if ( !str_cmp( arg2, "gunseat" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->gunseat = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "entrance" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->entrance = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "turret1" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->turret1 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "turret2" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->turret2 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
        if ( !str_cmp( arg2, "turret3" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->turret3 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
        if ( !str_cmp( arg2, "turret4" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->turret4 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
        if ( !str_cmp( arg2, "turret5" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->turret5 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
        if ( !str_cmp( arg2, "turret6" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->turret6 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
        if ( !str_cmp( arg2, "turret7" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}
	if ( tempnum < ship->firstroom || tempnum > ship->lastroom )
    	{
    	   ch->println("That room number is not in that ship .. \n\rIt must be between Firstroom and Lastroom.\n\r");
           return;
    	}

	ship->turret7 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
        if ( !str_cmp( arg2, "turret8" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->turret8 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
        if ( !str_cmp( arg2, "turret9" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->turret9 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
        if ( !str_cmp( arg2, "turret10" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->turret10 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "hanger" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}
	ship->hanger1 = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

   if ( !str_cmp( arg2, "engineroom" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}

	ship->engineroom = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "shipyard" ) )
    {
        tempnum = atoi(argument);
    	roomindex = get_room_index(tempnum);
    	if (roomindex == NULL)
    	{
    	   ch->println("That room doesn't exist.\n\r");
    	   return;
    	}
	ship->shipyard = tempnum;
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "type" ) )
    {
/*	if ( !str_cmp( argument, "republic" ) )
	  ship->type = SHIP_REPUBLIC;
	else
	if ( !str_cmp( argument, "imperial" ) )
	  ship->type = SHIP_IMPERIAL;
	else
*//*	if ( !str_cmp( argument, "player" ) )
	  ship->type = PLAYER_SHIP;
	else
	if ( !str_cmp( argument, "mob" ) )
	  ship->type = MOB_SHIP;
	else
	{
	   ch->println( "Ship type must be either: player or mob.\n\r" );
	   return;
	}
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

   if( !str_cmp( arg2, "name" ) )
   {
      SHIP_DATA *uship = NULL;
      if( !argument || argument[0] == '\0' )
      {
         ch->println( "You can't name a ship nothing.\r\n" );
         return;
      }
      if( ( uship = get_ship( argument ) ) != NULL  )
      {
         ch->println( "There is already another ship with that name.\r\n" );
         return;
      }
  //    STRFREE( ship->name );
      ship->name = str_dup( argument );
      ch->println( "Done.\n\r");
      save_ship( ship );
      return;
   }

   if( !str_cmp( arg2, "filename" ) )
   {
      char filename[256];

      if( !file_exists( /*ch, SHIPS_DIR,*//* argument ) )
         return;

      snprintf( filename, sizeof( filename ), "%s%s", SHIPS_DIR, ship->filename );
      if( !remove( filename ) )
         ch->println( "Old ship file deleted.\r\n" );

 //     DISPOSE( ship->filename );
      ship->filename = str_dup( argument );
	  ch->println( "Done.\n\r");
      save_ship( ship );
      write_ship_list(  );
      return;
   }
    if ( !str_cmp( arg2, "desc" ) )
    {
//	STRFREE( ship->description );
	ship->description = str_dup( argument );
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "manuever" ) )
    {
	ship->manuever = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "pcount" ) )
    {
	ship->primaryCount = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
    if (!str_cmp(arg2, "scount"))
    {
    	ship->secondaryCount = atoi(argument);
        ch->println( "Done.\n\r");
    	save_ship(ship);
    	return;
    }

    if ( !str_cmp( arg2, "class" ) )
    {
	ship->ship_class = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "missiles" ) )
    {
	ship->maxmissiles = atoi(argument);
	ship->missiles = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "torpedos" ) )
    {
	ship->maxtorpedos = atoi(argument);
	ship->torpedos = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "rockets" ) )
    {
	ship->maxrockets = atoi(argument);
	ship->rockets = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "speed" ) )
    {
	ship->realspeed = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "tractorbeam" ) )
    {
	ship->tractorbeam = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "hyperspeed" ) )
    {
	ship->hyperspeed = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "shield" ) )
    {
	ship->maxshield = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "hull" ) )
    {
	ship->hull = atoi(argument);
	ship->maxhull = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "energy" ) )
    {
	ship->energy = atoi(argument);
	ship->maxenergy = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "sensor" ) )
    {
	ship->sensor = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "astroarray" ) )
    {
	ship->astro_array = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "comm" ) )
    {
	ship->comm = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

    if ( !str_cmp( arg2, "chaff" ) )
    {
	ship->chaff = URANGE( 0, atoi(argument) , 25 );
	ship->maxchaff = URANGE( 0, atoi(argument) , 25 );
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }

#ifdef USECARGO
    if ( !str_cmp( arg2, "maxcargo" ) )
    {
        ship->maxcargo = URANGE(0, atoi(argument), 500);
        ch->println( "Done.\n\r", ch );
        save_ship(ship);
        return;
    }
#endif

    if ( !str_cmp( arg2, "bombs" ) )
    {
	ship->bombs = atoi(argument);
	ship->maxbombs = atoi(argument);
	ch->println( "Done.\n\r");
	save_ship( ship );
	return;
    }
    if( !str_cmp(arg2, "simvnum") )
    {
       ship->sim_vnum = atoi(argument);
       ch->println( "Done.\n\r");
       return;
    }
        if ( !str_cmp( arg2, "flags" ) )
        {
        char arg3[MIL];
        if ( !argument || argument[0] == '\0' )
        {
            ch->println( "Usage: setship <ship> flags <flag> [flag]...\n\r");
            ch->println( "Valid flags are: simulator\n\r");
            return;
        }

        while ( argument[0] != '\0' )
        {
            argument = one_argument( argument, arg3 );
            tempnum = get_shipflag( arg3 );

            if ( tempnum < 0 || tempnum > 31 )
            {
                ch->printlnf( "Unknown flag: %s\n\r", arg3 );
                return;
            }
            TOGGLE_BIT( ship->flags, 1 << tempnum );
        }
        save_ship( ship );
        return;
        }

    do_setship( ch, "" );
    return;
}

void do_showship( char_data *ch, char *argument )
{
    SHIP_DATA *ship;

    if ( IS_NPC( ch ) )
    {
	ch->println( "Huh?\n\r" );
	return;
    }

    if ( argument[0] == '\0' )
    {
	ch->println( "Usage: showship <ship>\n\r" );
	return;
    }

    ship = get_ship( argument );
    if ( !ship )
    {
	ch->println( "No such ship.\n\r" );
	return;
    }
//    //set_char_color( AT_YELLOW, ch );
    ch->printlnf( "Filename: %s\n\r", ship->filename);
    ch->printlnf( "Name: %s\n\r", ship->name);

    ch->printlnf( "Type: %s\n\r", ship->type);
    ch->printlnf( "Class: %s\n\r", ship->ship_class);


    ch->printlnf( "Home: %s   Description: %s  Current system: %s\n\rOwner: %s   Pilot: %s   Copilot: %s\n\r",
    			ship->home,  ship->description,
		ship->starsystem ? ship->starsystem->name : "None",
    			ship->owner, ship->pilot,  ship->copilot );
    ch->printlnf( "Firstroom: %d   Lastroom: %d ",
    			ship->firstroom,
    			ship->lastroom);
    ch->printlnf( "Cockpit: %d   Entrance: %d\n\rEngineroom: %d       Bay Doors: %s\n\r",
    			ship->cockpit,
    			ship->entrance,
    			ship->engineroom,
		ship->bayopen == true ? "Open" : "Closed");
	ch->printlnf( "Hanger1: %d   Hanger2: %d   Hanger3: %d  Hanger4: %d\n\r",
				ship->hanger1,
				ship->hanger2,
				ship->hanger3,
				ship->hanger4);
    ch->printlnf( "Pilotseat: %d   Coseat: %d   Navseat: %d  Gunseat: %d\n\r",
    			ship->pilotseat,
    			ship->coseat,
    			ship->navseat,
    			ship->gunseat);
    ch->printlnf( "Location: %d   Lastdoc: %d   Shipyard: %d\n\r",
    			ship->location,
    			ship->lastdoc,
    			ship->shipyard);
    ch->printlnf( "Tractor Beam: %d   Comm: %d   Sensor: %d   Astro Array: %d\n\r",
    			ship->tractorbeam,
    			ship->comm,
    			ship->sensor,
    			ship->astro_array);
    ch->printlnf( "Primary Count: %d Type: %d Condition: %s\n\r",
    			ship->primaryCount,
				ship->primaryType,
    			ship->primaryState == LASER_DAMAGED ? "Damaged" : "Good");
	ch->printlnf( "Secondary Count: %d Type: %d Condition: %s\n\r",
				ship->primaryCount,
				ship->primaryType,
				ship->primaryState == LASER_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Turret One: %d  Condition: %s\n\r",
    			ship->turret1,
    			ship->statet1 == LASER_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Turret Two: %d  Condition: %s\n\r",
    			ship->turret2,
    			ship->statet2 == LASER_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Turret Three: %d  Condition: %s\n\r",
    			ship->turret3,
    			ship->statet3 == LASER_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Turret Four: %d  Condition: %s\n\r",
    			ship->turret4,
    			ship->statet4 == LASER_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Turret Five: %d  Condition: %s\n\r",
    			ship->turret5,
    			ship->statet5 == LASER_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Turret Six: %d  Condition: %s\n\r",
    			ship->turret6,
    			ship->statet6 == LASER_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Turret Seven: %d  Condition: %s\n\r",
    			ship->turret7,
    			ship->statet7 == LASER_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Turret Eight: %d  Condition: %s\n\r",
    			ship->turret8,
    			ship->statet8 == LASER_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Turret Nine: %d  Condition: %s\n\r",
    			ship->turret9,
    			ship->statet9 == LASER_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Turret Ten: %d  Condition: %s\n\r",
    			ship->turret10,
    			ship->statet10 == LASER_DAMAGED ? "Damaged" : "Good");

    ch->printlnf( "Missiles: %d/%d  Torpedos: %d/%d  Rockets: %d/%d  Condition: %s\n\r",
       			ship->missiles,
    			ship->maxmissiles,
    			ship->torpedos,
    			ship->maxtorpedos,
    			ship->rockets,
    			ship->maxrockets,
    			ship->missilestate == MISSILE_DAMAGED ? "Damaged" : "Good");
    ch->printlnf( "Hull: %d/%d  Ship Condition: %s\n\r",
                        ship->hull,
    		        ship->maxhull,
    			ship->shipstate == SHIP_DISABLED ? "Disabled" : "Running");

    ch->printlnf( "Shields: %d/%d   Energy(fuel): %d/%d\n\rBombs: %d/%d  Chaff: %d/%d\n\r",
                        ship->shield,
    		        ship->maxshield,
    		        ship->energy,
    		        ship->maxenergy,
			ship->bombs,
			ship->maxbombs,
    		        ship->chaff,
    		        ship->maxchaff);
    ch->printlnf( "Current Coordinates: %.0f %.0f %.0f\n\r",
                        ship->vx, ship->vy, ship->vz );
    ch->printlnf( "Current Heading: %.0f %.0f %.0f\n\r",
                        ship->hx, ship->hy, ship->hz );
    ch->printlnf( "Speed: %d/%d   Hyperspeed: %d\n\r  Manueverability: %d\n\r",
                        ship->currspeed, ship->realspeed, ship->hyperspeed , ship->manuever );
//    ch->printlnf( "Flags: %s", flag_string(ship->flags, ship_flags));
#ifdef USECARGO
    ch_printf( ch, "Cargo: %d/%d, Cargo Type: %s \n\r",
                        ship->cargo,
                        ship->maxcargo,
                        cargo_names[ship->cargotype]);
#endif

    return;
}
/*
void do_makeship( char_data *ch, char *argument )
{
    SHIP_DATA *ship;
    char arg[MIL];

    argument = one_argument( argument, arg );

    if ( !argument || argument[0] == '\0' )
    {
	ch->println( "Usage: makeship <filename> <ship name>\n\r" );
	return;
    }

//    CREATE( ship, SHIP_DATA, 1 );
    LINK( ship, first_ship, last_ship, next, prev );

    ship->name		        = str_dup( argument );
    ship->description	    = str_dup( "" );
    ship->owner 	        = str_dup( "" );
    ship->copilot           = str_dup( "" );
    ship->pilot             = str_dup( "" );
    ship->home              = str_dup( "" );
    ship->type              = PLAYER_SHIP;
    ship->starsystem        = NULL;
    ship->energy            = ship->maxenergy;
    ship->hull              = ship->maxhull;
    ship->in_room           = NULL;
    ship->next_in_room      = NULL;
    ship->prev_in_room      = NULL;
    ship->currjump          = NULL;
    ship->target0           = NULL;
    ship->target1           = NULL;
    ship->target2           = NULL;
    ship->target3           = NULL;
    ship->target4           = NULL;
    ship->target5           = NULL;
    ship->target6           = NULL;
    ship->target7           = NULL;
    ship->target8           = NULL;
    ship->target9           = NULL;
    ship->target10          = NULL;
#ifdef USECARGO
    ship->maxcargo=0;
    ship->cargo=0;
    ship->cargotype=0;
#endif

    ship->filename = str_dup( arg );
    save_ship( ship );
    write_ship_list( );

}
/*
void do_copyship( char_data *ch, char *argument )
{
    SHIP_DATA *ship;
    SHIP_DATA *old;
    char arg[MIL];
    char arg2[MIL];

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    if ( !argument || argument[0] == '\0' )
    {
	ch->println( "Usage: copyship <oldshipname> <filename> <newshipname>\n\r" );
	return;
    }

    old = get_ship ( arg );

    if (!old)
    {
	ch->println( "Thats not a ship!\n\r" );
	return;
    }

//    CREATE( ship, SHIP_DATA, 1 );
    LINK( ship, first_ship, last_ship, next, prev );

    ship->name		            = str_dup( argument );
    ship->description	        = str_dup( "" );
    ship->owner 	            = str_dup( "" );
    ship->copilot               = str_dup( "" );
    ship->pilot                 = str_dup( "" );
    ship->home                  = str_dup( "" );
    ship->type                  = old->type;
    ship->ship_class            = old->ship_class;
    ship->primaryCount          = old->primaryCount  ;
    ship->maxmissiles           = old->maxmissiles  ;
    ship->maxrockets            = old->maxrockets  ;
    ship->maxtorpedos           = old->maxtorpedos  ;
    ship->maxshield             = old->maxshield  ;
    ship->maxhull               = old->maxhull  ;
    ship->maxenergy             = old->maxenergy  ;
    ship->hyperspeed            = old->hyperspeed  ;
    ship->maxchaff              = old->maxchaff  ;
    ship->maxbombs              = old->maxbombs  ;
    ship->realspeed             = old->realspeed  ;
    ship->manuever              = old->manuever  ;
    ship->in_room               = NULL;
    ship->next_in_room          = NULL;
    ship->prev_in_room          = NULL;
    ship->currjump              = NULL;
    ship->target0               = NULL;
    ship->target1               = NULL;
    ship->target2               = NULL;
    ship->target3               = NULL;
    ship->target4               = NULL;
    ship->target5               = NULL;
    ship->target6               = NULL;
    ship->target7               = NULL;
    ship->target8               = NULL;
    ship->target9               = NULL;
    ship->target10               = NULL;

    ship->filename              = str_dup(arg2);
    save_ship( ship );
    write_ship_list();
}
*//*
void do_ships( char_data *ch, char *argument )
{
    SHIP_DATA *ship;
    int count;

    if ( !IS_NPC(ch) )
    {
      count = 0;
      ch->println( "&YThe following ships are owned by you or by your organization:\n\r" );
      ch->println( "\n\r&WShip                               Owner\n\r");
      for ( ship = first_ship; ship; ship = ship->next )
      {
        if ( str_cmp(ship->owner, ch->name) )
        {
           if ( !ch->pcdata || /*!ch->pcdata->clan || str_cmp(ship->owner,ch->pcdata->clan->name) || ship->ship_class > SHIP_SPACE_STATION )
               continue;
        }

        if (ship->type == MOB_SHIP)
           continue;
/*        else if (ship->type == SHIP_REPUBLIC)
           //set_char_color( AT_BLOOD, ch );
        else if (ship->type == SHIP_IMPERIAL)
           //set_char_color( AT_DGREEN, ch );
        else
          //set_char_color( AT_BLUE, ch );
*//*
        if  ( ship->in_room )
          ch->printlnf( "%s (%s) - %s\n\r", ship->name, ship->in_room->name, ship->owner );
        else
          ch->printlnf( "%s - %s\n\r", ship->name, ship->owner );

        count++;
      }

      if ( !count )
      {
        ch->println( "There are no ships owned by you.\n\r" );
      }

    }


/*    count =0;
    ch->println( "&Y\n\rThe following ships are docked here:\n\r", ch );

    ch->println( "\n\r&WShip                               Owner          Cost/Rent\n\r", ch );
    for ( ship = first_ship; ship; ship = ship->next )
    {
        if ( ship->location != ch->in_room->vnum || ship->ship_class > SHIP_SPACE_STATION)
               continue;

        if (ship->type == MOB_SHIP)
           continue;
        else if (ship->type == SHIP_REPUBLIC)
           //set_char_color( AT_BLOOD, ch );
        else if (ship->type == SHIP_IMPERIAL)
           //set_char_color( AT_DGREEN, ch );
        else
          //set_char_color( AT_BLUE, ch );

        ch_printf( ch, "%-35s %-15s", ship->name, ship->owner );
        if (ship->type == MOB_SHIP || ship->ship_class == SHIP_SPACE_STATION )
        {
          ch_printf( ch, "\n\r");
          continue;
        }
        if ( !str_cmp(ship->owner, "Public") )
        {
          ch_printf( ch, "%ld to rent.\n\r", get_ship_value(ship)/100 );
        }
        else if ( str_cmp(ship->owner, "") )
          ch_printf( ch, "%s", "\n\r" );
        else
           ch_printf( ch, "%ld to buy.\n\r", get_ship_value(ship) );

        count++;
    }

    if ( !count )
    {
        ch->println( "There are no ships docked here.\n\r", ch );
    }
}


/*
void do_speeders( CHAR_DATA *ch, char *argument )
{
    SHIP_DATA *ship;
    int count;

    if ( !IS_NPC(ch) )
    {
      count = 0;
      ch->println( "&YThe following are owned by you or by your organization:\n\r", ch );
      ch->println( "\n\r&WVehicle                            Owner\n\r",ch);
      for ( ship = first_ship; ship; ship = ship->next )
      {
        if ( str_cmp(ship->owner, ch->name) )
        {
           if ( !ch->pcdata || !ch->pcdata->clan || str_cmp(ship->owner,ch->pcdata->clan->name) || ship->ship_class <= SHIP_SPACE_STATION )
               continue;
        }
        if ( ship->location != ch->in_room->vnum || ship->ship_class != LAND_VEHICLE)
               continue;

        if (ship->type == MOB_SHIP)
           continue;
        else if (ship->type == SHIP_REPUBLIC)
           //set_char_color( AT_BLOOD, ch );
        else if (ship->type == SHIP_IMPERIAL)
           //set_char_color( AT_DGREEN, ch );
        else
          //set_char_color( AT_BLUE, ch );

        ch_printf( ch, "%-35s %-15s\n\r", ship->name, ship->owner );

        count++;
      }

      if ( !count )
      {
        ch->println( "There are no land or air vehicles owned by you.\n\r", ch );
      }

    }


    count =0;
    ch->println( "&Y\n\rThe following vehicles are parked here:\n\r", ch );

    ch->println( "\n\r&WVehicle                            Owner          Cost/Rent\n\r", ch );
    for ( ship = first_ship; ship; ship = ship->next )
    {
        if ( ship->location != ch->in_room->vnum || ship->ship_class != LAND_VEHICLE)
               continue;

        if (ship->type == MOB_SHIP)
           continue;
        else if (ship->type == SHIP_REPUBLIC)
           //set_char_color( AT_BLOOD, ch );
        else if (ship->type == SHIP_IMPERIAL)
           //set_char_color( AT_DGREEN, ch );
        else
          //set_char_color( AT_BLUE, ch );


        ch_printf( ch, "%-35s %-15s", ship->name, ship->owner );

        if ( !str_cmp(ship->owner, "Public") )
        {
          ch_printf( ch, "%ld to rent.\n\r", get_ship_value(ship)/100 );
        }
        else if ( str_cmp(ship->owner, "") )
          ch_printf( ch, "%s", "\n\r" );
        else
           ch_printf( ch, "%ld to buy.\n\r", get_ship_value(ship) );

        count++;
    }

    if ( !count )
    {
        ch->println( "There are no sea air or land vehicles here.\n\r", ch );
    }
}

void do_allspeeders( CHAR_DATA *ch, char *argument )
{
    SHIP_DATA *ship;
    int count = 0;

      count = 0;
      ch->println( "&Y\n\rThe following sea/land/air vehicles are currently formed:\n\r", ch );

      ch->println( "\n\r&WVehicle                            Owner\n\r", ch );
      for ( ship = first_ship; ship; ship = ship->next )
      {
        if ( ship->ship_class != LAND_VEHICLE )
           continue;

        if (ship->type == MOB_SHIP)
           continue;
        else if (ship->type == SHIP_REPUBLIC)
           //set_char_color( AT_BLOOD, ch );
        else if (ship->type == SHIP_IMPERIAL)
           //set_char_color( AT_DGREEN, ch );
        else
          //set_char_color( AT_BLUE, ch );


        ch_printf( ch, "%-35s %-15s ", ship->name, ship->owner );

        if ( !str_cmp(ship->owner, "Public") )
        {
          ch_printf( ch, "%ld to rent.\n\r", get_ship_value(ship)/100 );
        }
        else if ( str_cmp(ship->owner, "") )
          ch_printf( ch, "%s", "\n\r" );
        else
           ch_printf( ch, "%ld to buy.\n\r", get_ship_value(ship) );

        count++;
      }

      if ( !count )
      {
        ch->println( "There are none currently formed.\n\r", ch );
	return;
      }

}
*/

/*
void do_allships( char_data *ch, char *argument )
{
    SHIP_DATA *ship;
    int count = 0;

      count = 0;
      ch->println( "&Y\n\rThe following ships are currently formed:\n\r" );
      ch->println( "\n\r&WShip                               Owner\n\r" );

      if ( IS_IMMORTAL( ch ) )
        for ( ship = first_ship; ship; ship = ship->next )
           if (ship->type == MOB_SHIP)
              ch->printlnf( "&w%-35s %-15s\n\r", ship->name, ship->owner );

      for ( ship = first_ship; ship; ship = ship->next )
      {
        if ( ship->ship_class == LAND_VEHICLE )
           continue;

        if (ship->type == MOB_SHIP)
           continue;
/*        else if (ship->type == SHIP_REPUBLIC)
           //set_char_color( AT_BLOOD, ch );
        else if (ship->type == SHIP_IMPERIAL)
           //set_char_color( AT_DGREEN, ch );
        else
          //set_char_color( AT_BLUE, ch );
*//*
        ch->printlnf( "%-35s %-15s ", ship->name, ship->owner );
        if (ship->type == MOB_SHIP || ship->ship_class == SHIP_SPACE_STATION )
        {
          ch->println( "\n\r");
          continue;
        }
        if ( !str_cmp(ship->owner, "Public") )
        {
          ch->printlnf( "%ld to rent.\n\r", get_ship_value(ship)/100 );
        }
        else if ( str_cmp(ship->owner, "") )
          ch->printlnf( "%s", "\n\r" );
        else
          ch->printlnf( "%ld to buy.\n\r", get_ship_value(ship) );

        count++;
      }

      if ( !count )
      {
        ch->println( "There are no ships currently formed.\n\r" );
	return;
      }

}

/*
void ship_to_starsystem( SHIP_DATA *ship , SPACE_DATA *starsystem )
{
     if ( starsystem == NULL )
        return;

     if ( ship == NULL )
        return;

     if ( starsystem->first_ship == NULL )
        starsystem->first_ship = ship;

     if ( starsystem->last_ship )
     {
         starsystem->last_ship->next_in_starsystem = ship;
         ship->prev_in_starsystem = starsystem->last_ship;
         ship->next_in_starsystem = NULL;
     }

     starsystem->last_ship = ship;

     ship->starsystem = starsystem;

}
*/

/*
void new_missile( SHIP_DATA *ship , SHIP_DATA *target , char_data *ch , int missiletype )
{
     SPACE_DATA *starsystem;
     MISSILE_DATA *missile;

     if ( ship  == NULL )
        return;

     if ( target  == NULL )
        return;

     if ( ( starsystem = ship->starsystem ) == NULL )
        return;

     CREATE( missile, MISSILE_DATA, 1 );
     LINK( missile, first_missile, last_missile, next, prev );

     missile->target = target;
     missile->fired_from = ship;
     if ( ch )
        missile->fired_by = STRALLOC( ch->name );
     else
        missile->fired_by = STRALLOC( "" );
     missile->missiletype = missiletype;
     missile->age =0;
     if ( missile->missiletype == HEAVY_BOMB )
       missile->speed = 250;
     else if ( missile->missiletype == PROTON_TORPEDO )
       missile->speed = 200;
     else if ( missile->missiletype == CONCUSSION_MISSILE )
       missile->speed = 300;
     else
       missile->speed = 50;

     missile->mx = ship->vx;
     missile->my = ship->vy;
     missile->mz = ship->vz;

     if ( starsystem->first_missile == NULL )
        starsystem->first_missile = missile;

     if ( starsystem->last_missile )
     {
         starsystem->last_missile->next_in_starsystem = missile;
         missile->prev_in_starsystem = starsystem->last_missile;
     }

     starsystem->last_missile = missile;

     missile->starsystem = starsystem;

}
*/

/*
void ship_from_starsystem( SHIP_DATA *ship , SPACE_DATA *starsystem )
{

     if ( starsystem == NULL )
        return;

     if ( ship == NULL )
        return;

     if ( starsystem->last_ship == ship )
        starsystem->last_ship = ship->prev_in_starsystem;

     if ( starsystem->first_ship == ship )
        starsystem->first_ship = ship->next_in_starsystem;

     if ( ship->prev_in_starsystem )
        ship->prev_in_starsystem->next_in_starsystem = ship->next_in_starsystem;

     if ( ship->next_in_starsystem)
        ship->next_in_starsystem->prev_in_starsystem = ship->prev_in_starsystem;

     ship->starsystem = NULL;
     ship->next_in_starsystem = NULL;
     ship->prev_in_starsystem = NULL;

}
*/


bool is_rental( char_data *ch , SHIP_DATA *ship )
{
   if ( !str_cmp("Public",ship->owner) )
          return true;

   return false;
}

bool check_pilot( char_data *ch , SHIP_DATA *ship )
{
   if ( !str_cmp(ch->name,ship->owner) || !str_cmp(ch->name,ship->pilot)
   || !str_cmp(ch->name,ship->copilot) || !str_cmp("Public",ship->owner) )
      return true;
/*
   if ( !IS_NPC(ch) && ch->pcdata && ch->pcdata->clan )
   {
      if ( !str_cmp(ch->pcdata->clan->name,ship->owner) )
      {
        if ( !str_cmp(ch->pcdata->clan->leader,ch->name) )
          return true;
        if ( !str_cmp(ch->pcdata->clan->number1,ch->name) )
          return true;
        if ( !str_cmp(ch->pcdata->clan->number2,ch->name) )
          return true;
        if ( ch->pcdata->bestowments && is_name( "pilot", ch->pcdata->bestowments) )
          return true;
      }
   }
*/
   return false;
}
/*

void damage_ship_ch( SHIP_DATA *ship , int min , int max , char_data *ch )
{
    int damage , shield_dmg;
    long xp;

    damage = number_range( min , max );

//    xp = ( exp_level( ch->skill_level[PILOTING_ABILITY]+1) - exp_level( ch->skill_level[PILOTING_ABILITY]) ) / 25 ;
//    xp = UMIN( get_ship_value( ship ) /100 , xp ) ;
//    gain_exp( ch , xp , PILOTING_ABILITY );

    if ( ship->shield > 0 )
    {
        shield_dmg = UMIN( ship->shield , damage );
    	damage -= shield_dmg;
    	ship->shield -= shield_dmg;
//    	if ( ship->shield == 0 )
//    	  //echo_to_cockpit( AT_BLOOD , ship , "&G[&gShip Computer&G] &wShields down..." );
    }

    if ( damage > 0 )
    {
        if ( number_range(1, 100) <= 5 && ship->shipstate != SHIP_DISABLED )
        {
//           //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G] &wDrive disabled." );
           ship->shipstate = SHIP_DISABLED;
        }

        if ( number_range(1, 100) <= 5 && ship->missilestate != MISSILE_DAMAGED && ship->maxmissiles > 0 )
        {
//           echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->gunseat) , "&G[&gShip Computer&G] &wMissile launcher offline." );
           ship->missilestate = MISSILE_DAMAGED;
        }

        if ( number_range(1, 100) <= 2 )
        {
           if (IS_SET(ship->flags, XSHIP_ION_LASERS) )
           	REMOVE_BIT(ship->flags, XSHIP_ION_LASERS);
           if (ship->primaryState != LASER_DAMAGED){
//           echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->gunseat) , "&G[&gShip Computer&G] &wPrimary weapon system offline." );

           ship->primaryState = LASER_DAMAGED;
        }
        }
        if ( number_range(1, 100) <= 5)
        {
           if (IS_SET(ship->flags, XSHIP_ION_TURRET1) )
           	REMOVE_BIT(ship->flags, XSHIP_ION_TURRET1);
           if (ship->statet1 != LASER_DAMAGED && ship->turret1)
           {
           ship->statet1 = LASER_DAMAGED;
//           echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret1) , "&G[&gShip Computer&G] &wTurret offline." );
        }
        }

        if ( number_range(1, 100) <= 5 )
        {
           if (IS_SET(ship->flags, XSHIP_ION_TURRET2) )
           	REMOVE_BIT(ship->flags, XSHIP_ION_TURRET2);
           if ( ship->statet2 != LASER_DAMAGED && ship->turret2 )
           {
//          echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret2) , "&G[&gShip Computer&G] &wTurret offline." );
           ship->statet2 = LASER_DAMAGED;
        }
        }

		if ( number_range(1, 100) <= 5 )
		{
           	if (IS_SET(ship->flags, XSHIP_ION_TURRET3) )
           	   REMOVE_BIT(ship->flags, XSHIP_ION_TURRET3);
                if (ship->statet3 != LASER_DAMAGED && ship->turret3 )
                {
//		   echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret3) , "&G[&gShip Computer&G] &wTurret offline." );
		   ship->statet3 = LASER_DAMAGED;
		}
		}

		if ( number_range(1, 100) <= 5 )
		{
           	if (IS_SET(ship->flags, XSHIP_ION_TURRET4) )
           	   REMOVE_BIT(ship->flags, XSHIP_ION_TURRET4);
                if (ship->statet4 != LASER_DAMAGED && ship->turret4)
                {
//		   echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret4) , "&G[&gShip Computer&G] &wTurret offline." );
		   ship->statet4 = LASER_DAMAGED;
		}
		}

		if ( number_range(1, 100) <= 5 )
		{
           	if (IS_SET(ship->flags, XSHIP_ION_TURRET5) )
           	   REMOVE_BIT(ship->flags, XSHIP_ION_TURRET5);
		if (ship->statet5 != LASER_DAMAGED && ship->turret5)
		{
//		   echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret5) , "&G[&gShip Computer&G] &wTurret offline." );
		   ship->statet5 = LASER_DAMAGED;
		}
		}

		if ( number_range(1, 100) <= 5 )
		{

           	if (IS_SET(ship->flags, XSHIP_ION_TURRET6) )
           	   REMOVE_BIT(ship->flags, XSHIP_ION_TURRET6);
                if (ship->statet6 != LASER_DAMAGED && ship->turret6 )
                {
//		   echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret6) , "&G[&gShip Computer&G] &wTurret offline." );
		   ship->statet6 = LASER_DAMAGED;
		}
		}

		if ( number_range(1, 100) <= 5) {
           	if (IS_SET(ship->flags, XSHIP_ION_TURRET7) )
           	   REMOVE_BIT(ship->flags, XSHIP_ION_TURRET7);

		 if (ship->statet7 != LASER_DAMAGED && ship->turret7 )
		 {
//		   echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret7) , "&G[&gShip Computer&G] &wTurret offline." );
		   ship->statet7 = LASER_DAMAGED;
		}
                }
		if ( number_range(1, 100) <= 5){
           	if (IS_SET(ship->flags, XSHIP_ION_TURRET8) )
           	   REMOVE_BIT(ship->flags, XSHIP_ION_TURRET8);
		if (ship->statet8 != LASER_DAMAGED && ship->turret8 )
		{
//		   echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret8) , "&G[&gShip Computer&G] &wTurret offline." );
		   ship->statet8 = LASER_DAMAGED;
		}}

		if ( number_range(1, 100) <= 5 )
		{
           	if (IS_SET(ship->flags, XSHIP_ION_TURRET9) )
           	   REMOVE_BIT(ship->flags, XSHIP_ION_TURRET9);
                if (ship->statet9 != LASER_DAMAGED && ship->turret9)
                {
//		   echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret9) , "&G[&gShip Computer&G] &wTurret offline." );

		   ship->statet9 = LASER_DAMAGED;
		}
                }
		if ( number_range(1, 100) <= 5 )
		{
           	if (IS_SET(ship->flags, XSHIP_ION_TURRET10) )
           	   REMOVE_BIT(ship->flags, XSHIP_ION_TURRET10);
                if (ship->statet10 != LASER_DAMAGED && ship->turret10 )
                {
//		   echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret10) , "&G[&gShip Computer&G] &wTurret offline." );
		   ship->statet10 = LASER_DAMAGED;
		}
		}
    }
/*
    if(ch->pcdata->learned[gsn_spacecombat] > 1 && ch->pcdata->learned[gsn_spacecombat2] < 1 && ch->pcdata->learned[gsn_spacecombat3] < 1 )
     ship->hull -= damage*4;
    else if(ch->pcdata->learned[gsn_spacecombat] > 1 && ch->pcdata->learned[gsn_spacecombat2] > 1 && ch->pcdata->learned[gsn_spacecombat3] < 1 )
     ship->hull -= damage*5;
    else if(ch->pcdata->learned[gsn_spacecombat] > 1 && ch->pcdata->learned[gsn_spacecombat2] > 1 && ch->pcdata->learned[gsn_spacecombat3] > 1 )
     ship->hull -= damage*6;
    else
*/  //   ship->hull -= damage*5;
/*
    if ( ship->hull <= 0 )
    {
       destroy_ship( ship , ch, "shot down" );

//       xp =  ( exp_level( ch->skill_level[PILOTING_ABILITY]+1) - exp_level( ch->skill_level[PILOTING_ABILITY]) );
//       xp = UMIN( get_ship_value( ship ) , xp );
//       gain_exp( ch , xp , PILOTING_ABILITY);
//       ch->printlnf( "&WYou gain %ld piloting experience!\n\r", xp );
       return;
    }

  //  if ( ship->hull <= ship->maxhull/20 )
  //     //echo_to_cockpit( AT_BLOOD+ AT_BLINK , ship , "&G[&gShip Computer&G] &wShip hull severely damaged." );

}

void damage_ship_ch_ion( SHIP_DATA *ship , int min , int max , char_data *ch )
{
    int damage,numloop,shields,range,chance;
    long xp;
    //Used to store all systems damaged to then be displayed when done.

    damage = number_range(min, max);

    /*
     * Random # used for a loop. The loop will run through accordingly disabling a random system each time.
     * Could disable as many as all 11 systems in one hit though highly unlikely.
     *
    */
 //   numloop = number_range(1, 7);


//    xp = ( exp_level( ch->skill_level[PILOTING_ABILITY]+1) - exp_level( ch->skill_level[PILOTING_ABILITY]) ) / 25;
//    xp = UMIN( get_ship_value( ship ) /100 , xp ) ;
//    gain_exp( ch , xp , PILOTING_ABILITY );

 /*   if (ship->shield > 0)
     {
      //Small formula to determine how much damage to cause to shields but shields will be ignored just damaged.
      shields = ( ( ship->shield - damage ) / ( damage / 2 ) );

      ship->shield -= shields;

      if (ship->shield < 0)
        ship->shield = 0;
//      if (ship->shield == 0)
//       //echo_to_cockpit( AT_BLOOD, ship, "&G[&gShip Computer&G]&w Warning. Shields down.\n\r");



    }
/*
    if (ship->plasmashield > 0)
    {
      //The ship has plasma shielding which will protect it from those nasty ion cannons :P
      int plasmashields;
      plasmashields = ( ( ship->maxplasmashield - damage) / (damage / 2) );
      ship->plasmashield -= plasmashields;
      if (ship->plasmashield < 0)
        ship->plasmashield = 0;
      if (ship->plasmashield == 0)
        //echo_to_cockpit( AT_BLOOD, ship, "&R[&WALERT&R] &WWarning Plasma Shielding Down!\n\r");

      //Exit out of the damage function with no damage recieved to the ship for now :)

      return;
   }
*/
     	//This is what picks out
 //    	range = number_range(1, 7);

	/* Update for new space/ion types
	 * Low damage has much less chance to disable a system, high damage is increased.
	 * If no shield protection, much higher chance to disable a system
	 * -||
	 *//*
  if(damage < 5 && ship->shield <= 0) chance = 15;							//
  else if(damage < 5 && ship->shield > 0) chance = 5;						// Repeating ions
  else if(damage > 15 && ship->shield <= 0) chance = 40;					//
  else if(damage > 15 && ship->shield > 0) chance = 18;						// Heavy ions
  else if((damage >= 6 && damage <= 14) && ship->shield <= 0) chance = 20;  //
  else if((damage >= 6 && damage <= 14) && ship->shield > 0) chance = 12;   // Light ions
  else chance = 11;

       if(number_range(1, 100) < chance)
       {
	switch(range)
        {
	  case 1:
            if ( ship->shipstate != SHIP_DISABLED )
            {
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Ship drive disabled." );
                ship->shipstate = SHIP_DISABLED;
                SET_BIT(ship->flags, XSHIP_ION_DRIVE);
                if (ship->tractoring)
                {
                    ship->tractoring->tractored_by = NULL;
                    ship->tractoring = NULL;
                }
            }
            break;
	  case 2:
            if ( ship->missilestate != MISSILE_DAMAGED && ship->maxmissiles > 0 )
            {
//                echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->gunseat) , "&G[&gShip Computer&G]&w Warning. Ship missile launcher offline." );
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Ship missile launcher disabled." );
                ship->missilestate = MISSILE_DAMAGED;
                SET_BIT(ship->flags, XSHIP_ION_MISSILES);
            }
            break;
	  case 3:
            if ( ship->secondaryState != LASER_DAMAGED )
            {
//                echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->gunseat) , "&G[&gShip Computer&G]&w Warning. Secondary weapon system offline." );
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Secondary weapon system offline." );
                ship->secondaryState = LASER_DAMAGED;
                SET_BIT(ship->flags, XSHIP_ION_ION);
            }
            break;
	  case 4:
            if ( ship->primaryState != LASER_DAMAGED )
            {
//                echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->gunseat) , "&G[&gShip Computer&G]&w Warning. Primary weapon system offline.." );
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Primary weapon system offline.");
                ship->primaryState = LASER_DAMAGED;
                SET_BIT(ship->flags, XSHIP_ION_LASERS);
            }
            break;
	  case 5:
            if ( ship->statet1 != LASER_DAMAGED && ship->turret1 )
            {
//                echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret1) , "&G[&gShip Computer&G]&w Warning. Turret offline." );
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Turret offline." );
                ship->statet1 = LASER_DAMAGED;
                SET_BIT(ship->flags, XSHIP_ION_TURRET1);
            }
            break;
	  case 6:
            if ( number_range(1, 100) <= 80 && ship->statet2 != LASER_DAMAGED && ship->turret2 )
            {
//                echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret2) , "&G[&gShip Computer&G]&w Warning. Turret offline." );
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Turret offline.");
                ship->statet2 = LASER_DAMAGED;
                SET_BIT(ship->flags, XSHIP_ION_TURRET2);
            }
            break;
          case 7:
            if ( number_range(1, 100) <= 80 && ship->hyperstate != LASER_DAMAGED )
            {
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Hyperdrive disabled." );
                ship->hyperstate = LASER_DAMAGED;
                SET_BIT(ship->flags, XSHIP_ION_HYPER);
            }
	    break;
	  case 8:
            if ( ship->statet3 != LASER_DAMAGED && ship->turret3 )
            {
//                echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret3) , "&G[&gShip Computer&G]&w Warning. Turret offline." );
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Turret offline.");
                ship->statet3 = LASER_DAMAGED;
                SET_BIT(ship->flags, XSHIP_ION_TURRET3);
            }
            break;
	  case 9:
            if ( ship->statet4 != LASER_DAMAGED && ship->turret4 )
            {
//                echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret4) , "&G[&gShip Computer&G]&w Warning. Turret offline." );
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Turret offline.");
                ship->statet4 = LASER_DAMAGED;
                SET_BIT(ship->flags, XSHIP_ION_TURRET4);
            }
            break;
	  case 10:
            if ( ship->statet5 != LASER_DAMAGED && ship->turret5 )
            {
//                echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret5) , "&G[&gShip Computer&G]&w Warning. Turret offline." );
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Turret offline.");
                ship->statet5 = LASER_DAMAGED;
                SET_BIT(ship->flags, XSHIP_ION_TURRET5);
            }
            break;
	  case 11:
            if ( ship->statet6 != LASER_DAMAGED && ship->turret6 )
            {
//                echo_to_room( AT_BLOOD + AT_BLINK , get_room_index(ship->turret6) , "&G[&gShip Computer&G]&w Warning. Turret offline." );
//                //echo_to_cockpit( AT_BLOOD + AT_BLINK , ship , "&G[&gShip Computer&G]&w Warning. Turret offline.");
                ship->statet6 = LASER_DAMAGED;
                SET_BIT(ship->flags, XSHIP_ION_TURRET6);
            }
            break;
	  default:
	    bug("Reached 'default' in damage_ship_ch_ion");
	    break;
        }
      }
}
*/

/*
bool ship_to_room(SHIP_DATA *ship , int vnum )
{
    ROOM_INDEX_DATA *shipto;

    if ( (shipto=get_room_index(vnum)) == NULL )
            return false;

    LINK( ship, shipto->ships, shipto->last_ship, next_in_room, prev_in_room );
    ship->in_room = shipto;
    return true;
}

*//*
void do_board( char_data *ch, char *argument )
{
   ROOM_INDEX_DATA *fromroom;
   ROOM_INDEX_DATA *toroom;
   SHIP_DATA *ship;

   if ( !argument || argument[0] == '\0')
   {
       ch->println( "Board what?\n\r" );
       return;
   }

   if ( ( ship = ship_in_room( ch->in_room , argument ) ) == NULL )
   {
            act( "I see no $T here.", ch, NULL, argument, TO_CHAR );
           return;
   }
/*
   if ( IS_SET( ch->act, ACT_MOUNTED ) )
   {
          act( "You can't go in there riding THAT.", ch, NULL, argument, TO_CHAR );
          return;
   }
*//*
   fromroom = ch->in_room;

        if ( ( toroom = get_room_index( ship->entrance ) ) != NULL )
   	{
   	   if ( ! ship->hatchopen )
   	   {
   	      ch->println( "&RThe hatch is closed!\n\r");
   	      return;
   	   }*/

/*           if ( toroom->tunnel > 0 )
           {
	        char_data *ctmp;
	        int count = 0;

	       for ( ctmp = toroom->first_person; ctmp; ctmp = ctmp->next_in_room )
	       if ( ++count >= toroom->tunnel )
	       {
                  ch->println( "There is no room for you in there.\n\r" );
		  return;
	       }
           }*//*
            if ( ship->shipstate == SHIP_LAUNCH || ship->shipstate == SHIP_LAUNCH_2 )
            {
                 ch->println("&rThat ship has already started launching!\n\r");
                 return;
            }

        act( "$n enters $T.", ch, NULL, ship->name , TO_ROOM );
	    act( "You enter $T.", ch, NULL, ship->name , TO_CHAR );
   	    char_from_room( ch );
   	    char_to_room( ch , toroom );
   	    act( "$n enters the ship.", ch,	NULL, argument , TO_ROOM );
        do_look( ch , "auto" );

        }
        else
          ch->println("That ship has no entrance!\n\r");
}*/

/*

bool rent_ship( char_data *ch , SHIP_DATA *ship )
{

    long price;

    if ( IS_NPC ( ch ) )
       return false;

    price = get_ship_value( ship )/100;

      if( IS_SET(ship->flags, SHIP_SIMULATOR))
       {
	 ch->println( "&wThe simulation begins.\n\r");
	 return true;
       }

       if ( ch->gold < price )
       {
         ch->printlnf( "&RRenting this ship costs %ld. You don't have enough credits!\n\r" , price );
         return false;
       }

       ch->gold -= price;
       ch->printlnf( "&GYou pay %ld gold to rent the ship.\n\r" , price );
       return true;

}

void do_leaveship( char_data *ch, char *argument )
{
    ROOM_INDEX_DATA *fromroom;
    ROOM_INDEX_DATA *toroom;
    SHIP_DATA *ship;

    fromroom = ch->in_room;

    if  ( (ship = ship_from_entrance(fromroom->vnum)) == NULL )
    {
        ch->println( "I see no exit here.\n\r" );
        return;
    }

    if  ( ship->ship_class == SHIP_SPACE_STATION )
    {
        ch->println( "You can't do that here.\n\r" );
        return;
    }

    if ( ship->lastdoc != ship->location )
    {
        ch->println("&rMaybe you should wait until the ship lands.\n\r");
        return;
    }

    if ( ship->shipstate != SHIP_DOCKED && ship->shipstate != SHIP_DISABLED )
    {
        ch->println("&rPlease wait till the ship is properly docked.\n\r");
        return;
    }

    if ( ! ship->hatchopen )
    {
    	ch->println("&RYou need to open the hatch first.\n\r" );
    	return;
    }

    if ( ( toroom = get_room_index( ship->location ) ) != NULL )
    {
        act( "$n exits the ship.", ch, NULL, argument , TO_ROOM );
	    act( "You exit the ship.", ch, NULL, argument , TO_CHAR );
   	    char_from_room( ch );
   	    char_to_room( ch , toroom );
   	    act( "$n steps out of a ship.", ch, NULL, argument , TO_ROOM );
        do_look( ch , "auto" );
     }
     else
        ch->println ( "The exit doesn't seem to be working properly.\n\r" );
}

SHIP_DATA *ship_from_room( int vnum )
{
   SHIP_DATA *ship;
   int sRoom;

   for ( ship = first_ship; ship; ship = ship->next )
     for ( sRoom = ship->firstroom ; sRoom < ship->lastroom+1 ; sRoom++ )
        if ( vnum == sRoom )
              return ship;
    return NULL;
}

void do_launch( char_data *ch, char *argument )
{
    int chance, password;
    long price = 0;
    SHIP_DATA *ship, *ship2;

    char buf[MSL];

                password = atoi ( argument );
                if (argument[0] != '\0' && (password < 1000 || password > 9999))
                {
                    ch->println("This is not within the valid range of lock codes.\n\r");
                    return;
                }
    	        if ( (ship = ship_from_cockpit(ch->in_room->vnum)) == NULL )
    	        {
    	            ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
    	            return;
    	        }

    	        if ( ship->ship_class == LAND_VEHICLE )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r");
    	            return;
    	        }

    	        if ( (ship = ship_from_pilotseat(ch->in_room->vnum)) == NULL )
    	        {
    	            ch->println("&RYou don't seem to be in the pilot seat!\n\r");
    	            return;
    	        }

                if ((ship2 = ship_from_room (ship->location)) && !ship2->bayopen)
                {
                    ch->println("&RThe bay doors are closed.\n\r");
                    return;
                }
    	        if ( autofly(ship) )
    	        {
    	            ch->println("&RThe ship is set on autopilot, you'll have to turn it off first.\n\r");
    	            return;
    	        }

                if  ( ship->ship_class == SHIP_SPACE_STATION )
                {
                   ch->println( "You can't do that here.\n\r" );
                   return;
                }
                if( argument[0] != '\0' && password != ship->password )
                {
               	  ch->println( "&RYou enter in the wrong code and the computer gives you a small &Yelectric shock&W.\r\n");
               	  ch->hit-=number_range(0,50);
                  return;
                }
    	        else if ( !check_pilot( ch , ship ) && password != ship->password )
    	        {
    	            ch->println("&RHey, thats not your ship! Try renting a public one.\n\r");
    	            return;
    	        }

		if(ship->tractored_by != NULL)
		{
		    ch->println("&RYou're being held in by a tractor beam!\n\r");
		    return;
		}

    	        if ( ship->lastdoc != ship->location )
                {
                     ch->println("&rYou don't seem to be docked right now.\n\r");
                     return;
                }

    	        if ( ship->shipstate != SHIP_DOCKED && ship->shipstate != SHIP_DISABLED )
    	        {
    	            ch->println("The ship is not docked right now.\n\r");
    	            return;
    	        }

    	        if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
//                    chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_smallspace]);
                      chance = ch->level;
                if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class<= SHIP_CORVETTE )
//                    chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_mediumspace]);
                      chance = ch->level/2;
                if ( ship->ship_class >= SHIP_CRUISER )
//                    chance = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_largespace]);
                      chance = ch->level/10;
                if (chance <= 20){
                    chance = 20;
                }

                if ( number_percent( ) < chance )
    		{
    		   if ( is_rental(ch,ship) )
    		     if( !rent_ship(ch,ship) )
    		        return;
    		    if ( !is_rental(ch,ship) )
                    {
    		     if ( ship->ship_class <= SHIP_SHUTTLE )
                       price=20;
                     else if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class<= SHIP_CORVETTE )
                       price=50;
                     else
                       price=500;

                     price += ( ship->maxhull-ship->hull );
                     if (ship->missiles )
     	                 price += ( 50 * (ship->maxmissiles-ship->missiles) );
                     else if (ship->torpedos )
     	                 price += ( 75 * (ship->maxtorpedos-ship->torpedos) );
                     else if (ship->rockets )
                         price += ( 150 * (ship->maxrockets-ship->rockets) );

                     if (ship->shipstate == SHIP_DISABLED )
                            price += 200;
                     if ( ship->missilestate == MISSILE_DAMAGED )
                            price += 100;
                     if ( ship->primaryState == LASER_DAMAGED )
                            price += 50;
					 if ( ship->secondaryState == LASER_DAMAGED )
						 price += 50;
                     if ( ship->statet1 == LASER_DAMAGED )
                            price += 50;
                     if ( ship->statet2 == LASER_DAMAGED )
                            price += 50;
					 if ( ship->statet3 == LASER_DAMAGED )
						    price += 50;
					 if ( ship->statet4 == LASER_DAMAGED )
							price += 50;
					 if ( ship->statet5 == LASER_DAMAGED )
							price += 500;
					 if ( ship->statet6 == LASER_DAMAGED )
							price += 500;
					 if ( ship->statet7 == LASER_DAMAGED )
							price += 500;
					 if ( ship->statet8 == LASER_DAMAGED )
							price += 500;
					 if ( ship->statet9 == LASER_DAMAGED )
							price += 500;
					 if ( ship->statet10 == LASER_DAMAGED )
							price += 500;
                    }


/*    	          if ( ch->pcdata && ch->pcdata->clan && !str_cmp(ch->pcdata->clan->name,ship->owner) )
                  {
                   if ( ch->pcdata->clan->funds < price )
                   {
                       ch->printlnf(  "&R%s doesn't have enough funds to prepare this ship for launch.\n\r", ch->pcdata->clan->name );
                       return;
                   }

                   ch->pcdata->clan->funds -= price;
                   ch->printlnf(  "&GIt costs %s %ld credits to ready this ship for launch.\n\r", ch->pcdata->clan->name, price );
                  }
                  else *//*if ( str_cmp( ship->owner , "Public" ) )
                  {
                    if ( ch->gold < price )
                    {
                       ch->println( "&RYou don't have enough funds to prepare this ship for launch.\n\r");
                       return;
                    }

                    ch->gold -= price;
                    ch->printlnf( "&GYou pay %ld credits to ready the ship for launch.\n\r", price );
                  }

              ship->energy = ship->maxenergy;
              ship->chaff = ship->maxchaff;
              ship->missiles = ship->maxmissiles;
              ship->torpedos = ship->maxtorpedos;
       		  ship->rockets = ship->maxrockets;
       		  ship->shield = 0;
       		  ship->autorecharge = false;
       		  ship->autotrack = false;
       		  ship->autospeed = false;
       		  ship->hull = ship->maxhull;

       		  ship->missilestate = MISSILE_READY;
       		  ship->primaryState = LASER_READY;
			  ship->secondaryState = LASER_READY;
       		  ship->statet1 = LASER_READY;
       		  ship->statet2 = LASER_READY;
			  ship->statet3 = LASER_READY;
			  ship->statet4 = LASER_READY;
			  ship->statet5 = LASER_READY;
			  ship->statet6 = LASER_READY;
			  ship->statet7 = LASER_READY;
			  ship->statet8 = LASER_READY;
			  ship->statet9 = LASER_READY;
			  ship->statet10 = LASER_READY;
       		  ship->shipstate = SHIP_DOCKED;

    		   if (ship->hatchopen)
    		   {
    		     ship->hatchopen = false;
    		     sprintf( buf , "The hatch on %s closes." , ship->name);
//       	             echo_to_room( AT_YELLOW , get_room_index(ship->location) , buf );
//       	             echo_to_room( AT_YELLOW , get_room_index(ship->entrance) , "The hatch slides shut." );
//       	             sound_to_room( get_room_index(ship->entrance) , "!!SOUND(door)" );
//      		     sound_to_room( get_room_index(ship->location) , "!!SOUND(door)" );
       	           }
//    		   //set_char_color( AT_GREEN, ch );
    		   ch->println( "&G[&gShip Computer&G]&w Launch sequence initiated.\n\r");
    		   act( "$n starts up the ship and begins the launch sequence.", ch, NULL, argument , TO_ROOM );
//		   echo_to_ship( AT_YELLOW , ship , "The ship hums as it lifts off the ground.");
    		   sprintf( buf, "%s begins to launch.", ship->name );
//    		   echo_to_room( AT_YELLOW , get_room_index(ship->location) , buf );
    		   ship->shipstate = SHIP_LAUNCH;
    		   ship->currspeed = ship->realspeed;
/*
    		   if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                      learn_from_success( ch, gsn_smallspace );
               if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
                      learn_from_success( ch, gsn_mediumspace );
               if ( ship->ship_class >= SHIP_CRUISER )
                      learn_from_success( ch, gsn_largespace );*/
//                   sound_to_ship(ship , "!!SOUND(xwing)" );
//                   return;
//                }
//                //set_char_color( AT_RED, ch );
/*	        ch->println("You fail to work the controls properly!\n\r");/*
	        if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                    learn_from_failure( ch, gsn_smallspace );
            if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
    	            learn_from_failure( ch, gsn_mediumspace );
            if ( ship->ship_class >= SHIP_CRUISER )
                    learn_from_failure( ch, gsn_largespace );*/
/*    	   	return;

}

void launchship( SHIP_DATA *ship )
{
    char buf[MSL];
    SHIP_DATA *source;
    int plusminus;
    ROOM_INDEX_DATA * room;
    SPACE_DATA *simul;
    for ( simul = first_starsystem; simul; simul = simul->next ) {
    	if(!strcmp(simul->name,"Simulator\0"))
    	    break;
    }
    if(IS_SET(ship->flags, SHIP_SIMULATOR) && simul)
    {
    	ship_to_starsystem( ship, simul);
		ship->currjump = simul;
        ship->jx = 11;
        ship->jy = -11;
        ship->jz = 11;
    }
    else
	{
        ship_to_starsystem( ship, starsystem_from_room( ship->in_room ) );
    }
    if ( ship->starsystem == NULL )
    {
//       echo_to_room( AT_YELLOW , get_room_index(ship->pilotseat) , "&G[&gShip Computer&G]&w Launch path blocked .. Launch aborted.");
//       echo_to_ship( AT_YELLOW , ship , "The ship slowly sets back back down on the landing pad.");
       sprintf( buf ,  "%s slowly sets back down." ,ship->name );
//       echo_to_room( AT_YELLOW , get_room_index(ship->location) , buf );
       ship->shipstate = SHIP_DOCKED;
       return;
    }

    source = ship_from_room( ship->in_room->vnum );
    extract_ship(ship);

    ship->location = 0;

    if (ship->shipstate != SHIP_DISABLED)
       ship->shipstate = SHIP_READY;

    plusminus = number_range ( -1 , 2 );
    if (plusminus > 0 )
        ship->hx = 1;
    else
        ship->hx = -1;

    plusminus = number_range ( -1 , 2 );
    if (plusminus > 0 )
        ship->hy = 1;
    else
        ship->hy = -1;

    plusminus = number_range ( -1 , 2 );
    if (plusminus > 0 )
        ship->hz = 1;
    else
        ship->hz = -1;

    if ( ( room = get_room_index(ship->lastdoc) ) != NULL &&
       room->area && room->area->planet && room->area->planet->starsystem
       && room->area->planet->starsystem == ship->starsystem )
    {
           ship->vx = room->area->planet->x;
           ship->vy = room->area->planet->y;
           ship->vz = room->area->planet->z;
    }
    else if ( source )
    {
             ship->vx = source->vx;
             ship->vy = source->vy;
             ship->vz = source->vz;
    }

    ship->energy -= 100;

    ship->vx += (ship->hx*ship->currspeed*2);
    ship->vy += (ship->hy*ship->currspeed*2);
    ship->vz += (ship->hz*ship->currspeed*2);

//    echo_to_room( AT_GREEN , get_room_index(ship->location) , "Launch complete.\n\r");
//    echo_to_ship( AT_YELLOW , ship , "The ship leaves the platform far behind as it flies into space." );
    sprintf( buf ,"%s enters the starsystem at %.0f %.0f %.0f" , ship->name, ship->vx, ship->vy, ship->vz );
//    echo_to_system( AT_YELLOW, ship, buf , NULL );
    sprintf( buf, "%s lifts off into space.", ship->name );
//    echo_to_room( AT_YELLOW , get_room_index(ship->lastdoc) , buf );
    ship->inship = NULL;

}

void showland( AREA_DATA *tarea, char_data *ch){
   ROOM_INDEX_DATA *room;
   int vnum;
   int num=0;
   for ( vnum = tarea->min_vnum; vnum <= tarea->max_vnum; vnum++ )
    {
	if ( (room = get_room_index( vnum )) != NULL )
	 {
//       	  if(IS_SET(room->room_flags2, ROOM_HIDDENPAD))
//            continue;
//          if ( IS_SET( room->room_flags, ROOM_CAN_LAND ) ) {
           num+=1;
           ch->printlnf("&c| &c(&C%2.2d&c) &Y%-43.43s &c|\n\r", num, room->name);
//          }
	 }
    }
   return;

}

void do_sland( char_data *ch, char *argument )
{
    char arg[MIL];
    char arg1[MIL];
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    PLANET_DATA * planet;
    bool pfound = false;
    ROOM_INDEX_DATA * room;
    bool rfound = false;
   int vnum;

    	        strcpy( arg , argument );
                argument = one_argument( argument , arg1 );

    	        if ( (ship = ship_from_pilotseat(ch->in_room->vnum)) == NULL )
    	        {
    	            ch->println("&RYou must be in the pilots seat of a ship to do that!\n\r");
    	            return;
    	        }

    	        if ( ship->ship_class > SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r");
    	            return;
    	        }

    	        if ( autofly(ship) )
    	        {
    	            ch->println("&RYou'll have to turn off the ships autopilot first.\n\r");
    	            return;
    	        }

                if  ( ship->ship_class == SHIP_SPACE_STATION )
                {
                   ch->println( "&RYou can't land space stations.\n\r" );
                   return;
                }

                if (ship->ship_class >= SHIP_CRUISER)
                {
                   ch->println("&RCapital ships are far to large to land.\n\r");
                   return;
                }

    	        if (ship->shipstate == SHIP_DISABLED)
    	        {
    	            ch->println("&RThe ships drive is disabled. Unable to land.\n\r");
    	            return;
    	        }
    	        if (ship->shipstate == SHIP_DOCKED)
    	        {
    	            ch->println("&RThe ship is already docked!\n\r");
    	            return;
    	        }

               if (ship->shipstate == SHIP_HYPERSPACE)
               {
                  ch->println("&RYou can only do that in realspace!\n\r");
                  return;
               }

    	        if (ship->shipstate != SHIP_READY)
    	        {
    	            ch->println("&RPlease wait until the ship has finished its current maneuver.\n\r");
    	            return;
    	        }
    	        if ( ship->starsystem == NULL )
    	        {
    	            ch->println("&RThere's nowhere to land around here!");
    	            return;
    	        }

    	        if ( ship->energy < 25 )
    	        {
    	           ch->println("&RTheres not enough fuel!\n\r");
    	           return;
    	        }
		if ( ship->tractored_by )
		{
		ch->println("&RYou can't land while locked in a tractor beam!\n\r");
		return;
		}

		if ( ship->tractoring )
		{
		ch->println("&RYou can't land while you have a ship in tow!\n\r");
		return;
		}
    	        if ( arg[0] == '\0' )
    	        {
//    	           //set_char_color(  AT_CYAN, ch );
    	           ch->println( "Land where?\n\r\n\rChoices: \n\r");

    	           for ( target = ship->starsystem->first_ship; target; target = target->next_in_starsystem )
                   {
                   	/*for(hanger=target->first_hanger;hanger;hanger=hanger->next)*//*
                   	  if (target->hanger1 || target->hanger2)
                           ch->printlnf("%s%s    %.0f %.0f %.0f\n\r         ",
						    target->ship_class==11 ? "Asteroid" : "",
                           	target->name,
                           	target->vx,
                           	target->vy,
                           	target->vz);
                   }
                   for ( planet = ship->starsystem->first_planet ; planet ; planet = planet->next_in_system )
                       ch->printlnf( "%s%s   %d %d %d\n\r",
			    planet->controls != 0 ? "Asteroid " : "",
    			planet->name,
    			planet->x , planet->y, planet->z);

                   ch->printlnf( "\n\rYour Coordinates: %.0f %.0f %.0f\n\r" ,
                             ship->vx , ship->vy, ship->vz);
                   return;
    	        }


    	        for ( planet = ship->starsystem->first_planet ; planet ; planet = planet->next_in_system )
                {
                    if ( !str_prefix( arg1 , planet->name) )
                    {
                       pfound = true;

                       if ( !planet->area )
                       {
    	                 ch->println("&RThat planet doesn't have any landing areas.\n\r");
    	                 return;
    	               }

     	               if (  (planet->x > ship->vx + 200) || (planet->x < ship->vx - 200) ||
    	                  (planet->y > ship->vy + 200) || (planet->y < ship->vy - 200) ||
    	                  (planet->z > ship->vz + 200) || (planet->z < ship->vz - 200) )
    	               {
    	                  ch->println("&RThat planet is too far away! You'll have to fly a little closer.\n\r");
    	                  return;
    	               }

    	               if ( argument[0] != '\0' ){
                       	   AREA_DATA *pArea;
                           for ( pArea = planet->first_area; pArea; pArea = pArea->next_on_planet ){
                           for ( vnum = pArea->min_vnum; vnum <= pArea->max_vnum; vnum++ )
                           {
	                       if ( (room = get_room_index( vnum )) == NULL )
                                 continue;
                               if ( /*IS_SET( room->room_flags , ROOM_CAN_LAND ) &&*//* !str_prefix( argument , room->name) )
                               {
//                               	if (IS_SET(room->room_flags2, ROOM_CLANLAND))
//                               	{
                               	  if( room->area
                               	   && room->area->planet
//                               	   && room->area->planet->governed_by
//                               	   && ch->pcdata->clan
//                               	   && str_cmp(room->area->planet->governed_by->name, ship->owner)
                               	   /*&& str_cmp(room->area->planet->governed_by->name, ch->pcdata->clan->name)*/ /* )
                               	  {
                                    act("Just as you're about to land, several planetary defense ships appear from the clouds and order you to fall back.", ch, NULL, NULL, TO_CHAR);
                                    act("Just as $n is about to land, several planetary defense ships appear from the clouds and order $m to fall back.", ch, NULL, NULL, TO_ROOM);
                                    return;
                                  }
/*                                  if(!ch->pcdata->clan)
                                  {
                                   act(AT_RED, "Just as you're about to land, several planetary defense ships appear from the clouds and order you to fall back.", ch, NULL, NULL, TO_CHAR);
                                   act(AT_RED, "Just as $n is about to land, several planetary defense ships appear from the clouds and order $m to fall back.", ch, NULL, NULL, TO_ROOM);
                                   return;
                                  }*/
//                                }
/*

                                rfound = true;
                                break;
                               }
                             }
                           }
                         }

                       if ( !rfound )
                       {
                       	   AREA_DATA *pArea;
                       	   ch->println("&c+--------------------------------------------------+\n\r");
      	                   ch->println("&c| &CPlease type the location after the planet name.  &c|\n\r");
      	                   ch->printlnf("&c| &CPossible choices for %-26.26s  &c|\n\r", planet->name);
      	                   ch->println("&c+--------------------------------------------------+\n\r");
                           for ( pArea = planet->first_area; pArea; pArea = pArea->next_on_planet ){
                             showland(pArea, ch);
                           }
                           ch->println("&c+--------------------------------------------------+\n\r");
                           return;
                       }

                       break;
                    }
                }

                if ( !pfound )
                {
    	           target = get_ship_here( arg , ship->starsystem );

    	           if ( target != NULL )
    	           {
    	              if ( target == ship )
    	              {
    	                 ch->println("&RYou can't land your ship inside itself!\n\r");
    	                 return;
    	              }
    	              if((target->hanger1 &&
			(ship->ship_class >= SHIP_FIGHTER && ship->ship_class<= SHIP_SHUTTLE))
			|| (target->hanger2 && (ship->ship_class >= SHIP_FREIGHTER && ship->ship_class<= SHIP_CORVETTE) ))
    	              {
    	              	if ( (target->hanger1 && !target->hanger2)
						&& (ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE))
							{
    	              		ch->println("&RThat ship only has a small-sized ship hanger.\n\r");
    	              		return;
    	              	}
    	              	if (ship->ship_class >= SHIP_FREIGHTER && target->hanger2 == 0)
    	              	{
						 ch->println("&RThat ship doesn't have a midsized ship hanger.\n\r");
						 return;
						}
    	              	if (!target->hanger1 && !target->hanger2 )
			{
    	              		ch->println("&RThat ship has no hanger to land in!\n\r");
    	              		return;
    	              	}

		      }
    	              if (  (target->vx > ship->vx + 200) || (target->vx < ship->vx - 200) ||
    	                  (target->vy > ship->vy + 200) || (target->vy < ship->vy - 200) ||
    	                  (target->vz > ship->vz + 200) || (target->vz < ship->vz - 200) )
    	              {
    	                ch->println("&R That ship is too far away! You'll have to fly a little closer.\n\r");
    	                return;
    	              }


    	              if(target->bayopen == false)
    	              {
    	               ch->println("&RThe bay doors on that ship are closed.\n\r");
    	               return;
    	              }

    	           }
                   else
                   {
    	                ch->println("&RI don't see that here.\n\r&W");
    	                do_sland( ch , "" );
    	                return;
    	           }
    	        }

//                chance = IS_NPC(ch) ? 100 : (int)  (ch->pcdata->learned[gsn_shipsystems]) ;
                chance = ch->level;
                if ( number_percent( ) < chance )
    		{
//    		   //set_char_color( AT_GREEN, ch );
    		   ch->println( "&G[&gShip Computer&G] &wLanding sequence initiated.\n\r");
    		   act( "$n begins the landing sequence.", ch, NULL, "" , TO_ROOM );
//		   echo_to_ship( AT_YELLOW , ship , "The ship slowly begins its landing approach.");
    		   ship->dest = str_dup(arg);
    		   ship->shipstate = SHIP_LAND;
    		   ship->currspeed = 0;
/*
    		   if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                      learn_from_success( ch, gsn_smallspace );
               if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
                      learn_from_success( ch, gsn_mediumspace );
*/

//                   if ( get_room_index(ship->lastdoc)&& starsystem_from_room(get_room_index(ship->lastdoc)) != ship->starsystem )
//                   {
//                      int xp =  (exp_level( ch->skill_level[PILOTING_ABILITY]+1) - exp_level( ch->skill_level[PILOTING_ABILITY])) ;
//                      xp = UMIN( get_ship_value( ship ) , xp );
//                      gain_exp( ch , xp , PILOTING_ABILITY );
//                      ch->println( "&WYou gain %ld points of flight experience!\n\r"/*, UMIN( get_ship_value( ship ) , xp )*/ );
/*                   }

                   return;
	        }
	        ch->println("You fail to work the controls properly.\n\r");
/*	        if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                    learn_from_failure( ch, gsn_smallspace );
            if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
    	            learn_from_failure( ch, gsn_mediumspace );;
*//*    	   	return;
}

void landship( SHIP_DATA *ship, char *argument )
{
    SHIP_DATA *target;
    char buf[MSL];
    int destination=0;
    char arg[MIL];
    char arg1[MIL];
    HANGER_DATA *hanger;
    PLANET_DATA * planet;
    ROOM_INDEX_DATA * room;
    AREA_DATA *pArea;
    int vnum;

    strcpy( arg , argument );
    argument = one_argument( argument , arg1 );

    for ( planet = ship->starsystem->first_planet ; planet ; planet = planet->next_in_system )
    {
         if ( !str_prefix( arg1 , planet->name) )
         {
           for ( pArea = planet->first_area; pArea; pArea = pArea->next_on_planet ){
             for ( vnum = pArea->min_vnum; vnum <= pArea->max_vnum; vnum++ ){
	       if ( (room = get_room_index( vnum )) == NULL )
                  continue;
               if ( /*IS_SET( room->room_flags , ROOM_CAN_LAND ) &&*//* !str_prefix( argument , room->name) )
               {
                 destination = room->vnum;
                 break;
               }
             }
           }

           break;
         }
    }

   if ( destination != 0 )
    {

   	 if ( !ship_to_room( ship , destination ) )
   	 {
//   	    echo_to_room( AT_YELLOW , get_room_index(ship->pilotseat), "Could not complete approach. Landing aborted.");
//   	    echo_to_ship( AT_YELLOW , ship , "The ship pulls back up out of its landing sequence.");
    	   if (ship->shipstate != SHIP_DISABLED)
    	       ship->shipstate = SHIP_READY;
    	   return;
    	}

//	    echo_to_room( AT_YELLOW , get_room_index(ship->pilotseat), "&G[&gShip Computer&G] &wLanding sequence complete.");
//	    echo_to_ship( AT_YELLOW , ship , "You feel a slight thud as the ship sets down on the ground.");
	    sprintf( buf ,"%s disappears from your scanner." , ship->name  );
//	    echo_to_system( AT_YELLOW, ship, buf , NULL );

 	   extract_ship(ship);
 	   ship_to_room(ship, destination);
 	   ship->location = destination;
 	   ship->lastdoc = ship->location;
 	   if (ship->shipstate != SHIP_DISABLED)
 	      ship->shipstate = SHIP_DOCKED;
 	   ship_from_starsystem(ship, ship->starsystem);

  	  sprintf( buf, "%s lands on the platform.", ship->name );
//  	  echo_to_room( AT_YELLOW , get_room_index(ship->location) , buf );

   	 ship->energy = ship->maxenergy;

    	save_ship(ship);
	return;

   }
   else
   {
    ROOM_INDEX_DATA *room;
    target = get_ship_here( arg , ship->starsystem );


            if ((ship->type == 0 || ship->type == 1) && target->hanger1)
              room = get_room_index(target->hanger1);
            if (ship->type == 2)
               room = get_room_index(target->hanger2);
            if (!ship_to_room(ship, room->vnum)){
//              echo_to_room( AT_YELLOW , get_room_index(ship->pilotseat), "Could not complete approach. Landing aborted.");
//              echo_to_ship( AT_YELLOW , ship , "The ship pulls back up out of its landing sequence.");
              if (ship->shipstate != SHIP_DISABLED)
                ship->shipstate = SHIP_READY;
              return;
            }
			extract_ship(ship);
            ship_to_room(ship, room->vnum);
//    	    echo_to_room( AT_YELLOW , get_room_index(ship->pilotseat), "Landing sequence complete.");
//            echo_to_ship( AT_YELLOW , ship , "You feel a slight thud as the ship sets down on the ground.");
            sprintf( buf ,"%s disappears from your scanner." , ship->name  );
//            echo_to_system( AT_YELLOW, ship, buf , NULL );
            ship_from_starsystem(ship, ship->starsystem);
            ship->energy = ship->energy - 25;
            ship->shipstate = SHIP_DOCKED;
            ship->location = room->vnum;
            ship->lastdoc = ship->location;
            save_ship(ship);
            return;
   }



}

void do_accelerate( char_data *ch, char *argument )
{
    int chance;
    int change;
    SHIP_DATA *ship;
    char buf[MSL];

    	        if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
    	        {
    	            ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
    	            return;
    	        }

                if ( ship->ship_class > SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r");
    	            return;
    	        }

                if (  (ship = ship_from_pilotseat(ch->in_room->vnum))  == NULL )
    	        {
    	            ch->println("&RThe controls must be at the pilots chair...\n\r");
    	            return;
    	        }

                if ( autofly(ship) )
    	        {
    	            ch->println("&RYou'll have to turn off the ships autopilot first.\n\r");
    	            return;
    	        }

                if  ( ship->ship_class == SHIP_SPACE_STATION )
                {
                   ch->println( "&RSpace stations can't move.\n\r" );
                   return;
                }

                if (ship->shipstate == SHIP_HYPERSPACE)
                {
                  ch->println("&RYou can only do that in realspace!\n\r");
                  return;
                }
                if (ship->shipstate == SHIP_DISABLED)
    	        {
    	            ch->println("&RThe ships drive is disabled. Unable to accelerate.\n\r");
    	            return;
    	        }
    	        if (ship->shipstate == SHIP_DOCKED)
    	        {
    	            ch->println("&RYou can't do that until after you've launched!\n\r");
    	            return;
    	        }
                if (ship->tractored_by != NULL) /* Part of the new tractor stop code */
/*                {
                    ch->println("&RYou are being held in place by a tractor beam!\n\r");
                    return;
                }
    	        if ( ship->energy < abs((atoi(argument)-abs(ship->currspeed))/10) )
    	        {
    	           ch->println("&RTheres not enough fuel!\n\r");
    	           return;
    	        }

    	        if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
//                    chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_smallspace]) ;
                    chance = ch->level;
                if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class<= SHIP_CORVETTE )
//                    chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_mediumspace]) ;
                    chance = ch->level/2;
                if ( ship->ship_class >= SHIP_CRUISER )
//                    chance = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_largespace]);
                    chance = ch->level/10;

                if(chance<=20){
                chance = 20;
                }
                if ( number_percent( ) >= chance )
    		{
	           ch->println("&RYou fail to work the controls properly.\n\r");/*
	        if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                    learn_from_failure( ch, gsn_smallspace );
            if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
    	            learn_from_failure( ch, gsn_mediumspace );
            if ( ship->ship_class >= SHIP_CRUISER )
                    learn_from_failure( ch, gsn_largespace );*/
  /*  	   	   return;
                }

    change = atoi(argument);

    act( "$n manipulates the ships controls.", ch,
    NULL, argument , TO_ROOM );

    if ( change > ship->currspeed )
    {
       ch->println( "&GAccelerating.\n\r");
//       //echo_to_cockpit( AT_YELLOW , ship , "The ship begins to accelerate.");
       sprintf( buf, "%s begins to speed up." , ship->name );
//       echo_to_system( AT_ORANGE , ship , buf , NULL );
    }

    if ( change < ship->currspeed )
    {
       ch->println( "&GDecelerating.\n\r");
//       //echo_to_cockpit( AT_YELLOW , ship , "The ship begins to slow down.");
       sprintf( buf, "%s begins to slow down." , ship->name );
//       echo_to_system( AT_ORANGE , ship , buf , NULL );
    }

    ship->energy -= abs((change-abs(ship->currspeed))/10);

    ship->currspeed = URANGE( 0 , change , ship->realspeed );
/*
    		   if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                      learn_from_success( ch, gsn_smallspace );
                   if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
                      learn_from_success( ch, gsn_mediumspace );
                   if ( ship->ship_class >= SHIP_CRUISER )
                      learn_from_success( ch, gsn_largespace );*/
/*
}

void do_trajectory( char_data *ch, char *argument )
{
    PLANET_DATA *planet;
    bool foundplanet=false;
    char  buf[MSL];
    char  arg2[MIL];
    char  arg3[MIL];
    int chance;
    float vx,vy,vz;
    SHIP_DATA *ship;


    	        if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
    	        {
    	            ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
    	            return;
    	        }

                if ( ship->ship_class > SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r");
    	            return;
    	        }

                if (  (ship = ship_from_pilotseat(ch->in_room->vnum))  == NULL )
    	        {
    	            ch->println("&RYour not in the pilots seat.\n\r");
    	            return;
    	        }

                if ( autofly(ship))
    	        {
    	            ch->println("&RYou'll have to turn off the ships autopilot first.\n\r");
    	            return;
    	        }

                if (ship->shipstate == SHIP_DISABLED)
    	        {
    	            ch->println("&RThe ships drive is disabled. Unable to manuever.\n\r");
    	            return;
    	        }
                if  ( ship->ship_class == SHIP_SPACE_STATION )
                {
                   ch->println( "&RSpace stations can't turn.\n\r" );
                   return;
                }

    	        if (ship->shipstate == SHIP_HYPERSPACE)
                {
                  ch->println("&RYou can only do that in realspace!\n\r");
                  return;
                }
    	        if (ship->shipstate == SHIP_DOCKED)
    	        {
    	            ch->println("&RYou can't do that until after you've launched!\n\r");
    	            return;
    	        }
    	        if (ship->shipstate != SHIP_READY)
    	        {
    	            ch->println("&RPlease wait until the ship has finished its current manouver.\n\r");
    	            return;
    	        }
    	        if ( ship->energy < (ship->currspeed/10) )
    	        {
    	           ch->println("&RTheres not enough fuel!\n\r");
    	           return;
    	        }

    	        if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
//                    chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_smallspace]) ;
                    chance = ch->level;
                if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class<= SHIP_CORVETTE )
//                    chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_mediumspace]) ;
                    chance = ch->level/2;
                if ( ship->ship_class >= SHIP_CRUISER )
//                    chance = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_largespace]);
                    chance = ch->level/10;

                if (chance <= 20){
                chance = 20;
                }
                if ( number_percent( ) > chance )
    		{

	        ch->println("&RYou fail to work the controls properly.\n\r");
/*	        if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                    learn_from_failure( ch, gsn_smallspace );
            if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
    	            learn_from_failure( ch, gsn_mediumspace );
            if ( ship->ship_class >= SHIP_CRUISER )
                    learn_from_failure( ch, gsn_largespace );*/
  /*  	   	return;
    	        }

    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if(arg3[0] == '\0')
    {
    for ( planet = ship->starsystem->first_planet ;planet ; planet = planet->next_in_system )
      {
       if ( !str_prefix( arg2 , planet->name) )
       {
	 foundplanet = true;
	 vx = planet->x;
	 vy = planet->y;
	 vz = planet->z;
	 break;
       }
      }
   }

   if(!foundplanet)
   {
    if(!str_cmp(arg2, "inf")) vx = 0; else vx = atof( arg2 );
    if(!str_cmp(arg3, "inf")) vy = 0; else vy = atof( arg3 );
    if(!str_cmp(argument, "inf")) vz = 0; else vz = atof( argument );
   }

    if ( vx == ship->vx && vy == ship->vy && vz == ship->vz )
    {
       ch->printlnf( "The ship is already at %.0f %.0f %.0f !" ,vx,vy,vz);
    }

    ship->hx = vx - ship->vx;
    ship->hy = vy - ship->vy;
    ship->hz = vz - ship->vz;

    ship->energy -= (ship->currspeed/10);

    ch->printlnf("&GNew course set, aproaching %.0f %.0f %.0f.\n\r" , vx,vy,vz );
    act(  "$n manipulates the ships controls.", ch, NULL, argument , TO_ROOM );

//    //echo_to_cockpit( AT_YELLOW ,ship, "The ship begins to turn.\n\r" );
    sprintf( buf, "%s turns altering its present course." , ship->name );
//    echo_to_system( AT_ORANGE , ship , buf , NULL );

    if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
        ship->shipstate = SHIP_BUSY_3;
    else if( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
        ship->shipstate = SHIP_BUSY_2;
    else
        ship->shipstate = SHIP_BUSY;

    		   /*if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                      learn_from_success( ch, gsn_smallspace );
               if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
                      learn_from_success( ch, gsn_mediumspace );
               if ( ship->ship_class >= SHIP_CRUISER )
                      learn_from_success( ch, gsn_largespace );*/
/*
}


void do_buyship(char_data *ch, char *argument )
{
    long         price;
    SHIP_DATA   *ship;

   if ( IS_NPC(ch) || !ch->pcdata )
   {
   	ch->println( "&ROnly players can do that!\n\r" );
   	return;
   }

   ship = ship_in_room( ch->in_room , argument );
   if ( !ship )
   {
      ship = ship_from_cockpit( ch->in_room->vnum );

      if ( !ship )
      {
           act( "I see no $T here.", ch, NULL, argument, TO_CHAR );
           return;
      }
   }

   if ( str_cmp( ship->owner , "" )  || ship->type == MOB_SHIP )
   {
   	ch->println( "&RThat ship isn't for sale!" );
   	return;
   }

/*
   if ( ship->type == SHIP_IMPERIAL )
   {
        if ( !ch->pcdata->clan || str_cmp( ch->pcdata->clan->name , "the empire" ) )
       {
         if ( !ch->pcdata->clan || !ch->pcdata->clan->mainclan || str_cmp( ch->pcdata->clan->mainclan->name , "The Empire" ) )
   	 {
           ch->println( "&RThat ship may only be purchased by the Empire!\n\r" ,ch );
   	   return;
   	 }
       }
   }
   else if ( ship->type == SHIP_REPUBLIC )
   {
        if ( !ch->pcdata->clan || str_cmp( ch->pcdata->clan->name , "the new republic" ) )
        {
         if ( !ch->pcdata->clan || !ch->pcdata->clan->mainclan || str_cmp( ch->pcdata->clan->mainclan->name , "The New Republic" ) )
   	 {
   	  ch->println( "&RThat ship may only be purchased by The New Republic!\n\r" ,ch );
   	  return;
         }
   	}
   }
*/
/*   price = get_ship_value( ship );

    if ( ch->gold < price )
    {
       ch->printlnf( "&RThis ship costs %ld. You don't have enough gold!\n\r" , price );
       return;
    }

    ch->gold -= price;
    ch->printlnf( "&GYou pay %ld gold to purchace the ship.\n\r" , price );

    act( "$n walks over to a terminal and makes a transaction.",ch, NULL, argument , TO_ROOM );

//    	STRFREE( ship->owner );
	ship->owner = str_dup( ch->name );
	save_ship( ship );

}
/*
void do_clanbuyship(char_data *ch, char *argument )
{
    long         price;
    SHIP_DATA   *ship;
//    CLAN_DATA   *clan;
//    CLAN_DATA   *mainclan;

   if ( IS_NPC(ch) || !ch->pcdata )
   {
   	ch->println( "&ROnly players can do that!\n\r" ,ch );
   	return;
   }
   if ( !ch->pcdata->clan )
   {
   	ch->println( "&RYou aren't a member of any organizations!\n\r" ,ch );
   	return;
   }

   clan = ch->pcdata->clan;
   mainclan = ch->pcdata->clan->mainclan ? ch->pcdata->clan->mainclan : clan;

   if ( ( ch->pcdata->bestowments
    &&    is_name("clanbuyship", ch->pcdata->bestowments))
    ||   !str_cmp( ch->name, clan->leader  ))
	;
   else
   {
   	ch->println( "&RYour organization hasn't seen fit to bestow you with that ability.\n\r" ,ch );
   	return;
   }

   ship = ship_in_room( ch->in_room , argument );
   if ( !ship )
   {
      ship = ship_from_cockpit( ch->in_room->vnum );

      if ( !ship )
      {
           act( AT_PLAIN, "I see no $T here.", ch, NULL, argument, TO_CHAR );
           return;
      }
   }

   if ( str_cmp( ship->owner , "" )  || ship->type == MOB_SHIP )
   {
   	ch->println( "&RThat ship isn't for sale!\n\r" ,ch );
   	return;
   }

   if ( str_cmp( mainclan->name , "The Empire" )  && ship->type == SHIP_IMPERIAL )
   {
   	   ch->println( "&RThat ship may only be purchaced by the Empire!\n\r" ,ch );
   	   return;
   }

   if ( str_cmp( mainclan->name , "The New Republic" )  && ship->type == SHIP_REPUBLIC )
   {
   	ch->println( "&RThat ship may only be purchaced by The New Republic!\n\r" ,ch );
   	return;
   }

   if ( !str_cmp( mainclan->name , "The Empire" )  && ship->type != SHIP_IMPERIAL )
   {
   	ch->println( "&RDue to contractual agreements that ship may not be purchaced by the empire!\n\r" ,ch );
   	return;
   }

   if ( !str_cmp( mainclan->name , "The New Republic" )  && ship->type != SHIP_REPUBLIC )
   {
   	ch->println( "&RBecause of contractual agreements, the NR can only purchase NR ships!\n\r" ,ch );
   	return;
   }

   price = get_ship_value( ship );

    if ( ch->pcdata->clan->funds < price )
    {
       ch->printlnf(  "&RThis ship costs %ld. You don't have enough credits!\n\r" , price );
       return;
    }

    clan->funds -= price;
    ch->printlnf(  "&G%s pays %ld credits to purchace the ship.\n\r", clan->name , price );

    act( AT_PLAIN, "$n walks over to a terminal and makes a credit transaction.",ch,
       NULL, argument , TO_ROOM );

    	STRFREE( ship->owner );
	ship->owner = STRALLOC( clan->name );
	save_ship( ship );

   if ( ship->ship_class != LAND_VEHICLE )
             clan->spacecraft++;
   else
             clan->vehicles++;
}
*/

/*
void do_sellship(char_data *ch, char *argument )
{
    long         price;
    SHIP_DATA   *ship;

   ship = ship_in_room( ch->in_room , argument );
   if ( !ship )
   {
            act( "I see no $T here.", ch, NULL, argument, TO_CHAR );
           return;
   }

   if ( str_cmp( ship->owner , ch->name ) )
   {
   	ch->println( "&RThat isn't your ship!" );
   	return;
   }

   if (!IS_NPC(ch) && ch->level <= 10)
   {
    ch->println( "Due to cheating, players under level 11 are not allowed to sell ships.\n\r");
    return;
   }

   price = get_ship_value( ship );

    ch->gold += ( price - price/10 );
    ch->printlnf( "&GYou receive %ld credits from selling your ship.\n\r" , price - price/10 );

    act( "$n walks over to a terminal and makes a transaction.",ch, NULL, argument , TO_ROOM );

//	STRFREE( ship->owner );
	ship->owner = str_dup( "" );
	save_ship( ship );

}

void do_info( char_data *ch, char *argument )
{
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MSL];

    if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
    {
            if ( argument[0] == '\0' )
            {
               act( "Which ship do you want info on?.", ch, NULL, NULL, TO_CHAR );
               return;
            }

            ship = ship_in_room( ch->in_room , argument );
            if ( !ship )
            {
               act( "I see no $T here.", ch, NULL, argument, TO_CHAR );
               return;
            }

            target = ship;
    }
    else if (argument[0] == '\0')
       target = ship;
    else
       target = get_ship_here( argument , ship->starsystem );

    if ( target == NULL )
    {
         ch->println("&RI don't see that here.\n\rTry the radar, or type info by itself for info on this ship.\n\r");
         return;
    }

    if ( abs(target->vx - ship->vx) > 500+ship->sensor*2 ||
         abs(target->vy - ship->vy) > 500+ship->sensor*2 ||
         abs(target->vz - ship->vz) > 500+ship->sensor*2 )
    {
         ch->println("&RThat ship is to far away to scan.\n\r");
         return;
    }
    ch->println("You take a look at the ship...\n\r\n\r");

	ch->printlnf("&G| &W%s %s:&w %s &G|\n\r\n\r",
//			target->type == SHIP_REPUBLIC ? "New Republic" :
//		    target->type == SHIP_IMPERIAL ? "Imperial" :
		    target->type == PLAYER_SHIP ? "Civilian" : "Mob",
		        target->ship_class == SHIP_FIGHTER ? "Starfighter" :
		        target->ship_class == SHIP_BOMBER  ? "Bomber" :
		        target->ship_class == SHIP_SHUTTLE ? "Shuttle" :
		        target->ship_class == SHIP_FREIGHTER ? "Freighter" :
		        target->ship_class == SHIP_FRIGATE ? "Frigate" :
		        target->ship_class == SHIP_TT ? "Troop Transport" :
		        target->ship_class == SHIP_CORVETTE ? "Corvette" :
		        target->ship_class == SHIP_CRUISER ? "Cruiser" :
			    target->ship_class == SHIP_DREADNAUGHT ? "Dreadnaught" :
		        target->ship_class == SHIP_DESTROYER ? "Star Destroyer" :
		        target->ship_class == SHIP_SPACE_STATION ? "Space Station" :
		        target->ship_class == LAND_VEHICLE ? "Land vehicle" : "Unknown",
			target->name);

    if(!str_cmp(target->owner, ch->name) || !str_cmp(target->pilot, ch->name) || !str_cmp(target->copilot, ch->name))
	 ch->printlnf( "&G| &WOwner: &w%s&G || &WPilot: &w%s&G || &WCoPilot: &w%s&G |\n\r",
		target->owner != '\0' ? target->owner : "None.", target->pilot != '\0' ? target->pilot : "None.", target->copilot != '\0' ? target->copilot : "None.");
	else if(!str_cmp(target->owner, "Public") || !str_cmp(target->owner, "public"))
	 ch->printlnf( "&G| &WOwner: &wPublic &G || &WPilot: &w---------&G || &WCoPilot: &w---------&G |\n\r");
/*	else if(ch->pcdata->clan)
	{
		if(!str_cmp(target->owner, ch->pcdata->clan->name))
		ch->printlnf(  "&G| &WOwner: &w%s&G || &WPilot: &w%s&G || &WCoPilot: &w%s&G |\n\r",
		   target->owner != '\0' ? target->owner : "None.", target->pilot != '\0' ? target->pilot : "None.", target->copilot != '\0' ? target->copilot : "None.");
    }*//*
	else
	 ch->printlnf( "&G| &WOwner: &w%s&G || &WPilot: &w%s&G || &WCoPilot: &w%s&G |\n\r\n\r",
		ship->owner != '\0' ? "---------" : "None.", "---------", "---------");

    sprintf(buf, "$n checks over %s thoroughly.", target->name);
    act( buf, ch, NULL, argument , TO_ROOM );

}

void do_autorecharge(char_data *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    int recharge;


        if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
            return;
        }

        if (  (ship = ship_from_coseat(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou must be in the co-pilots seat!\n\r");
            return;
        }

        if ( autofly(ship)  )
    	        {
    	            ch->println("&RYou'll have to turn off the ships autopilot first.\n\r");
    	            return;
    	        }

//        chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_shipsystems]) ;
        chance = ch->level;
        if ( number_percent( ) > chance )
        {
           ch->println("&RYou fail to work the controls properly.\n\r");
//           learn_from_failure( ch, gsn_shipsystems );
           return;
        }

    act( "$n flips a switch on the control panell.", ch, NULL, argument , TO_ROOM );

    if ( !str_cmp(argument,"on" ) )
    {
        ship->autorecharge=true;
        ch->println( "&GYou power up the shields.\n\r");
//        //echo_to_cockpit( AT_YELLOW , ship , "Shields ON. Autorecharge ON.");
    }
    else if ( !str_cmp(argument,"off" ) )
    {
        ship->autorecharge=false;
        ch->println( "&GYou shutdown the shields.\n\r");
//        //echo_to_cockpit( AT_YELLOW , ship , "Shields OFF. Shield strength set to 0. Autorecharge OFF.");
        ship->shield = 0;
    }
    else if ( !str_cmp(argument,"idle" ) )
    {
        ship->autorecharge=false;
        ch->println( "&GYou let the shields idle.\n\r");
//        //echo_to_cockpit( AT_YELLOW , ship , "Autorecharge OFF. Shields IDLEING.");
    }
    else
    {
        if (ship->autorecharge == true)
        {
           ship->autorecharge=false;
           ch->println( "&GYou toggle the shields.\n\r");
//           //echo_to_cockpit( AT_YELLOW , ship , "Autorecharge OFF. Shields IDLEING.");
        }
        else
        {
           ship->autorecharge=true;
           ch->println( "&GYou toggle the shields.\n\r");
//           //echo_to_cockpit( AT_YELLOW , ship , "Shields ON. Autorecharge ON");
        }
    }

    if (ship->autorecharge)
    {
       recharge  = URANGE( 1, ship->maxshield-ship->shield, 25+ship->ship_class*25 );
       recharge  = UMIN( recharge, ship->energy*5 + 100 );
       ship->shield += recharge;
       ship->energy -= ( recharge*2 + recharge * ship->ship_class );
    }
    if(ship->shield > ship->maxshield)
		ship->shield = ship->maxshield;
//    learn_from_success( ch, gsn_shipsystems );
}

void do_autopilot(char_data *ch, char *argument )
{
    SHIP_DATA *ship;

        if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
            return;
        }

        if (  (ship = ship_from_pilotseat(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou must be in the pilots seat!\n\r");
            return;
        }

         if ( ! check_pilot(ch,ship) )
       	     {
       	       ch->println("&RHey! Thats not your ship!\n\r");
       	       return;
       	     }

         if ( ship->target0 || ship->target1 || ship->target2 )
       	     {
       	       ch->println("&RNot while the ship is enganged with an enemy!\n\r");
       	       return;
       	     }


    act( "$n flips a switch on the control panell.", ch, NULL, argument , TO_ROOM );

        if (ship->autopilot == true)
        {
           ship->autopilot=false;
           ch->println( "&GYou toggle the autopilot.\n\r");
//           //echo_to_cockpit( AT_YELLOW , ship , "Autopilot OFF.");
        }
        else
        {
           ship->autopilot=true;
           ship->autorecharge = true;
           ch->println( "&GYou toggle the autopilot.\n\r");
//           //echo_to_cockpit( AT_YELLOW , ship , "Autopilot ON.");
        }

}

void do_openhatch(char_data *ch, char *argument )
{
   SHIP_DATA *ship;
   char buf[MSL];
   char arg1[MSL];
   char arg2[MSL];
   int password;

   argument = one_argument(argument, arg1);
   argument = one_argument(argument, arg2);
   if ( !arg1 || arg1[0] == '\0' || !str_cmp(argument,"hatch") )
   {
       ship = ship_from_entrance( ch->in_room->vnum );
       if( ship == NULL)
       {
          ch->println( "&ROpen what?\n\r" );
          return;
       }
       else
       {
          if ( !ship->hatchopen)
       	  {

                if  ( ship->ship_class >= SHIP_CRUISER )
                {
                   ch->println( "&RTry one of the docking bays!\n\r" );
                   return;
                }
       	     if ( ship->location != ship->lastdoc ||
       	        ( ship->shipstate != SHIP_DOCKED && ship->shipstate != SHIP_DISABLED ) )
       	     {
       	       ch->println("&RPlease wait till the ship lands!\n\r");
       	       return;
       	     }
       	     ship->hatchopen = true;
       	     ch->println("&GYou open the hatch.\n\r");
       	     act( "$n opens the hatch.", ch, NULL, argument, TO_ROOM );
       	     sprintf( buf , "The hatch on %s opens." , ship->name);
//       	     echo_to_room( AT_YELLOW , get_room_index(ship->location) , buf );
//       	       sound_to_room( get_room_index(ship->entrance) , "!!SOUND(door)" );
//      		     sound_to_room( get_room_index(ship->location) , "!!SOUND(door)" );
		return;
       	  }
       	  else
       	  {
       	     ch->println("&RIt's already open.\n\r");
       	     return;
       	  }
       }
   }

   password = atoi ( arg2 );
   if ( ( arg2[0] != '\0' ) && ( password < 1000 || password > 9999 ) )
    {
      ch->println("This is not within the valid range of lock codes.\n\r");
      return;
    }
   ship = ship_in_room( ch->in_room , arg1 );
   if ( !ship )
   {
            act( "I see no $T here.", ch, NULL, arg1, TO_CHAR );
           return;
   }

   if ( ship->shipstate != SHIP_DOCKED && ship->shipstate != SHIP_DISABLED )
   {
        ch->println( "&RThat ship has already started to launch.\n\r");
        return;
   }

   if( arg2[0] != '\0' && password != ship->password )
   {
   	ch->println( "&RYou enter in the wrong code and the computer gives you a small &Yelectric shock&R.\r\n");
   	ch->hit-=number_range(0,75);
     return;
   }

   else if ( ! check_pilot(ch,ship) && password != ship->password )
       	     {
       	       ch->println("&RHey! Thats not your ship!\n\r");
       	       return;
       	     }
   if ( !ship->hatchopen)
   {
   	ship->hatchopen = true;
   	act( "You open the hatch on $T.", ch, NULL, ship->name, TO_CHAR );
   	act( "$n opens the hatch on $T.", ch, NULL, ship->name, TO_ROOM );
//   	echo_to_room( AT_YELLOW , get_room_index(ship->entrance) , "The hatch opens from the outside." );
//   	  sound_to_room( get_room_index(ship->entrance) , "!!SOUND(door)" );
//      		     sound_to_room( get_room_index(ship->location) , "!!SOUND(door)" );
	return;
   }

   ch->println("&GIts already open!\n\r");

}

void do_closehatch(char_data *ch, char *argument )
{
   SHIP_DATA *ship;
   char buf[MSL];

   if ( !argument || argument[0] == '\0' || !str_cmp(argument,"hatch") )
   {
       ship = ship_from_entrance( ch->in_room->vnum );
       if( ship == NULL)
       {
          ch->println( "&RClose what?\n\r" );
          return;
       }
       else
       {

                if  ( ship->ship_class == SHIP_SPACE_STATION )
                {
                   ch->println( "&RTry one of the docking bays!\n\r" );
                   return;
                }
          if ( ship->hatchopen)
       	  {
       	     ship->hatchopen = false;
       	     ch->println("&GYou close the hatch.\n\r");
       	     act( "$n closes the hatch.", ch, NULL, argument, TO_ROOM );
       	     sprintf( buf , "The hatch on %s closes." , ship->name);
//       	     echo_to_room( AT_YELLOW , get_room_index(ship->location) , buf );
//       	      sound_to_room( get_room_index(ship->entrance) , "!!SOUND(door)" );
//      		     sound_to_room( get_room_index(ship->location) , "!!SOUND(door)" );
 	     return;
       	  }
       	  else
       	  {
       	     ch->println("&RIt's already closed.\n\r");
       	     return;
       	  }
       }
   }

   ship = ship_in_room( ch->in_room , argument );
   if ( !ship )
   {
            act( "I see no $T here.", ch, NULL, argument, TO_CHAR );
           return;
   }

   if ( ship->shipstate != SHIP_DOCKED && ship->shipstate != SHIP_DISABLED )
   {
        ch->println( "&RThat ship has already started to launch");
        return;
   }
   else
   {
      if(ship->hatchopen)
      {
   	ship->hatchopen = false;
   	act( "You close the hatch on $T.", ch, NULL, ship->name, TO_CHAR );
   	act( "$n closes the hatch on $T.", ch, NULL, ship->name, TO_ROOM );
//        echo_to_room( AT_YELLOW , get_room_index(ship->entrance) , "The hatch is closed from outside.");
//     sound_to_room( get_room_index(ship->entrance) , "!!SOUND(door)" );
//      		     sound_to_room( get_room_index(ship->location) , "!!SOUND(door)" );

   	return;
      }
      else
      {
      	ch->println("&RIts already closed.\n\r");
      	return;
      }
   }


}
/*
void do_status(char_data *ch, char *argument )
{
    int chance,x;
    SHIP_DATA *ship;
    SHIP_DATA *target;

    if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
    {
         ch->println("&RYou must be in the cockpit, turret or engineroom of a ship to do that!\n\r");
         return;
    }

    if (argument[0] == '\0')
       target = ship;
    else
       target = get_ship_here( argument , ship->starsystem );

    if ( target == NULL )
    {
         ch->println("&RI don't see that here.\n\rTry the radar, or type status by itself for your ships status.\n\r");
         return;
    }

    if ( abs(target->vx - ship->vx) > 500+ship->sensor*2 ||
         abs(target->vy - ship->vy) > 500+ship->sensor*2 ||
         abs(target->vz - ship->vz) > 500+ship->sensor*2 )
    {
         ch->println("&RThat ship is to far away to scan.\n\r");
         return;
    }

//    chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_shipsystems]) ;
    chance = ch->level;

    if ( number_percent( ) > chance )
    {
        ch->println("&RYou cant figure out what the readout means.\n\r");
//        learn_from_failure( ch, gsn_shipsystems );
        return;
    }

    act( "$n checks various gauges and displays on the control panel.", ch, NULL, argument , TO_ROOM );

    ch->printlnf( "&w[&W %s &w]   &zShip Condition: %s\n\r\n\r",target->name,
		target->shipstate == SHIP_DISABLED ? "&RDisabled" : "&GRunning");
    ch->printlnf( "&zCurrent Coordinates:&w %.0f %.0f %.0f   &zCurrent Heading:&w %.0f %.0f %.0f\n\r",
                        target->vx, target->vy, target->vz,
                        target->hx, target->hy, target->hz );
    ch->printlnf( "&zSpeed:&w %d&W/%d   &zHyperdrive: &wClass %d   %s %s\n\r",
                        target->currspeed , target->realspeed, target->hyperspeed,
			ship->ship_class >= SHIP_CRUISER ? "&zBay doors:" : "",
			(ship->ship_class >= SHIP_CRUISER && ship->bayopen == true) ? "&wOpen" :
			(ship->ship_class>= SHIP_CRUISER && ship->bayopen == false) ? "&wClosed" : "" );

    ch->printlnf( "&zHull:&w %d&W/%d   &zShields:&w %d&W/%d   &zFuel:&w %d&W/%d\n\r\n\r",
                        target->hull, target->maxhull, target->shield, target->maxshield,
						target->energy, target->maxenergy);

	ch->printlnf( "&zPrimary Weapon System:   &w%-7s&W::  &w%s\n\r",
		target->primaryState == LASER_DAMAGED ? "&ROffline" : "&GOnline", primary_beam_name(target));

	switch(ship->primaryType)
	{
	 case 0: x = 0; break;
	 case 1: x = ship->primaryCount; break;
	 case 2: x = 2*ship->primaryCount; break;
	 case 3: x = 3*ship->primaryCount; break;
	 case 4: x = 4*ship->primaryCount; break;
	 case 5: x = ship->primaryCount; break;
	 case 6: x = ship->primaryCount; break;
	 case 7: x = ship->primaryCount; break;
	 case 8: x = ship->primaryCount; break;
	 case 9: x = ship->primaryCount; break;
	 default: x = 1; break;
	}

	ch->printlnf( "&z   Linked fire: &w%s\n\r", (x <= 1) ? "Unavailable" : ship->primaryLinked == true ? "&GON" : "&ROFF");

	switch(ship->secondaryType)
	{
	 case 0: x = 0; break;
	 case 1: x = ship->secondaryCount; break;
	 case 2: x = 2*ship->secondaryCount; break;
	 case 3: x = 3*ship->secondaryCount; break;
	 case 4: x = 4*ship->secondaryCount; break;
	 case 5: x = ship->secondaryCount; break;
	 case 6: x = ship->secondaryCount; break;
	 case 7: x = ship->secondaryCount; break;
	 case 8: x = ship->secondaryCount; break;
	 case 9: x = ship->secondaryCount; break;
	 default: x = 1; break;
	}
   if(target->secondaryCount != 0)
	{
	ch->printlnf( "&zSecondary Weapon System: &w%-7s&W::  &w%s\n\r",
		target->secondaryState == LASER_DAMAGED ? "&ROffline" : "&GOnline",
		secondary_beam_name(target));
	ch->printlnf( "&z   Linked fire: &w%s\n\r",
	(x <= 1) ? "Unavailable" : ship->secondaryLinked == true ? "&GON" : "&ROFF");
	}
    if (target->turret1)
        ch->printlnf(  "\n\r&zTurret One:  &w %s  &zCurrent Target:&w %s\n\r",
    			target->statet1 == LASER_DAMAGED ? "Damaged" : "Operational" , target->target1 ? target->target1->name : "none");
    if (target->turret2)
        ch->printlnf(  "&zTurret Two:  &w %s  &zCurrent Target:&w %s\n\r",
    			target->statet2 == LASER_DAMAGED ? "Damaged" : "Operational" , target->target2 ? target->target2->name : "none");
    if (target->turret3)
        ch->printlnf(  "&zTurret Three:&w %s  &zCurrent Target:&w %s\n\r",
    			target->statet3 == LASER_DAMAGED ? "Damaged" : "Operational" , target->target3 ? target->target3->name : "none");
    if (target->turret4)
        ch->printlnf(  "&zTurret Four: &w %s  &zCurrent Target:&w %s\n\r",
    			target->statet4 == LASER_DAMAGED ? "Damaged" : "Operational" , target->target4 ? target->target4->name : "none");
    if (target->turret5)
        ch->printlnf(  "&zTurret Five: &w %s  &zCurrent Target:&w %s\n\r",
    			target->statet5 == LASER_DAMAGED ? "Damaged" : "Operational" , target->target5 ? target->target5->name : "none");
    if (target->turret6)
        ch->printlnf(  "&zTurret Six:  &w %s  &zCurrent Target:&w %s\n\r",
    			target->statet6 == LASER_DAMAGED ? "Damaged" : "Operational" , target->target6 ? target->target6->name : "none");
    if (target->turret7)
        ch->printlnf(  "&zTurret Seven:&w %s  &zCurrent Target:&w %s\n\r",
    			target->statet7 == LASER_DAMAGED ? "Damaged" : "Operational" , target->target7 ? target->target7->name : "none");
    if (target->turret8)
        ch->printlnf(  "&zTurret Eight:&w %s  &zCurrent Target:&w %s\n\r",
    			target->statet8 == LASER_DAMAGED ? "Damaged" : "Operational" , target->target8 ? target->target8->name : "none");
    if (target->turret9)
        ch->printlnf(  "&zTurret Nine: &w %s  &zCurrent Target:&w %s\n\r",
  			target->statet9 == LASER_DAMAGED ? "Damaged" : "Operational" , target->target9 ? target->target9->name : "none");
    if (target->turret10)
        ch->printlnf(  "&zTurret Ten:  &w %s  &zCurrent Target:&w %s\n\r",
    			target->statet10 == LASER_DAMAGED ? "Damaged" : "Operational" , target->target10 ? target->target10->name : "none");


	ch->printlnf( "\n\r&w          | &zMissile launcher:&w %-15s   &zPayload: &w%d&W/%d\n\r",
		!ship->maxmissiles ? "&RNot installed" :
		ship->missilestate == LASER_DAMAGED ? "&ROffline" : "&GOnline",
		ship->missiles, ship->maxmissiles);
	ch->printlnf( " %-10s&w | &zTorpedo launcher:&w %-15s   &zPayload: &w%d&W/%d\n\r",
		ship->warheadLinked == true ? "&GLinked" : "&RUnlinked",
		!ship->maxtorpedos ? "&RNot installed" :
		ship->torpedostate == LASER_DAMAGED ? "&ROffline" : "&GOnline",
		ship->torpedos, ship->maxtorpedos);
	ch->printlnf( "          &w| &zRocket launcher: &w %-15s   &zPayload: &w%d&W/%d\n\r",
		!ship->maxrockets ? "&RNot installed" :
		ship->rocketstate == LASER_DAMAGED ? "&ROffline" : "&GOnline",
		ship->rockets, ship->maxrockets);
	if(ship->maxbombs > 0) ch->printlnf( "&zBomb Payload: &w%d&W/%d\n\r", ship->bombs, ship->maxbombs);
#ifdef USECARGO
    ch_printf( ch, "&OCargo: &Y%d/&O%d   Cargo Type: &Y%s&w\n\r",
                        ship->cargo,
                        ship->maxcargo,
                        cargo_names[ship->cargotype]);
#endif

//    learn_from_success( ch, gsn_shipsystems );
}
*//*
void do_hyperspace(char_data *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *eShip;
    char buf[MSL];

        if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
            return;
        }

        if ( ship->ship_class > SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r");
    	            return;
    	        }


        if (  (ship = ship_from_pilotseat(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou aren't in the pilots seat.\n\r");
            return;
        }

        if ( autofly(ship)  )
    	        {
    	            ch->println("&RYou'll have to turn off the ships autopilot first.\n\r");
    	            return;
    	        }


                if  ( ship->ship_class == SHIP_SPACE_STATION )
                {
                   ch->println( "&RSpace Stations can't move!\n\r" );
                   return;
                }
                if (ship->hyperspeed == 0)
                {
                  ch->println("&RThis ship is not equipped with a hyperdrive!\n\r");
                  return;
                }
                if (ship->shipstate == SHIP_HYPERSPACE)
                {
                  ch->println("&RYou are already travelling lightspeed!\n\r");
                  return;
                }
		if(ship->hyperstate == LASER_DAMAGED)
		{
		  ch->println("&RThe ships hyperdrive is disabled. Unable to jump to lightspeed.\n\r");
		  return;
		}
                if (ship->shipstate == SHIP_DISABLED)
    	        {
    	            ch->println("&RThe ships drive is disabled. Unable to manuever.\n\r");
    	            return;
    	        }
    	        if (ship->shipstate == SHIP_DOCKED)
    	        {
    	            ch->println("&RYou can't do that until after you've launched!\n\r");
    	            return;
    	        }
    	        if (ship->shipstate != SHIP_READY)
    	        {
    	            ch->println("&RPlease wait until the ship has finished its current maneuver.\n\r");
    	            return;
    	        }
                if (!ship->currjump)
    	        {
    	            ch->println("&RYou need to calculate your jump first!\n\r");
    	            return;
    	        }

        if ( ship->energy < ((66*ship->hyperspeed)+((1+ship->hyperdistance)/6)) )
        {
              ch->println("&RTheres not enough fuel!\n\r");
              return;
        }

        if ( ship->currspeed <= 0 )
        {
              ch->println("&RYou need to speed up a little first!\n\r");
              return;
        }

    	for ( eShip = ship->starsystem->first_ship; eShip; eShip = eShip->next_in_starsystem )
    	{
    	   if ( eShip == ship )
    	      continue;

    	   if ( abs( eShip->vx - ship->vx ) < 500
    	   &&  abs( eShip->vy - ship->vy ) < 500
    	   &&  abs( eShip->vz - ship->vz ) < 500
	   && eShip->ship_class > SHIP_CRUISER )
           {
              ch->printlnf( "&RYou are too close to %s to make the jump to lightspeed.\n\r", eShip->name );
              return;
           }
    	}
/*
    	        if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                    chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_smallspace]) ;
                if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class<= SHIP_CORVETTE )
                    chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_mediumspace]) ;
                if ( ship->ship_class >= SHIP_CRUISER )
                    chance = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_largespace]);
*//*              chance = ch->level;

        if ( number_percent( ) > chance )
        {
            ch->println("&RYou can't figure out which lever to use.\n\r");
/*	        if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                    learn_from_failure( ch, gsn_smallspace );
                if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
    	            learn_from_failure( ch, gsn_mediumspace );
                if ( ship->ship_class >= SHIP_CRUISER )
                    learn_from_failure( ch, gsn_largespace );*/
/*    	   return;
        }
    sprintf( buf ,"%s disappears from your scanner." , ship->name );
//    echo_to_system( AT_YELLOW, ship, buf , NULL );

    ship_from_starsystem( ship , ship->starsystem );
    ship->shipstate = SHIP_HYPERSPACE;

    ch->println( "&GYou push forward the hyperspeed lever.\n\r");
    act( "$n pushes a lever forward on the control panel.", ch,
         NULL, argument , TO_ROOM );
//    echo_to_ship( AT_YELLOW , ship , "The ship lurches slightly as it makes the jump to lightspeed." );
//    //echo_to_cockpit( AT_YELLOW , ship , "The stars become streaks of light as you enter hyperspace.");

    ship->energy -= ((66*ship->hyperspeed)+((1+ship->hyperdistance)/6));

    ship->vx = ship->jx;
    ship->vy = ship->jy;
    ship->vz = ship->jz;
/*
    		   if ( ship->ship_class >= SHIP_FIGHTER && ship->ship_class <= SHIP_SHUTTLE )
                      learn_from_success( ch, gsn_smallspace );
               if ( ship->ship_class >= SHIP_FREIGHTER && ship->ship_class <= SHIP_CORVETTE )
                      learn_from_success( ch, gsn_mediumspace );
               if ( ship->ship_class >= SHIP_CRUISER )
                      learn_from_success( ch, gsn_largespace );
*/
//}
/*

void do_target(char_data *ch, char *argument )
{
    char arg[MIL];
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MSL];

    strcpy( arg, argument );

    switch( ch->substate )
    {
    	default:
    	        if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
    	        {
    	            ch->println("&RYou must be in the gunners seat or turret of a ship to do that!\n\r");
    	            return;
    	        }

                if ( ship->ship_class > SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r");
    	            return;
    	        }

                if (ship->shipstate == SHIP_HYPERSPACE)
                {
                  ch->println("&RYou can only do that in realspace!\n\r");
                  return;
                }
    	        if (! ship->starsystem )
    	        {
    	            ch->println("&RYou can't do that until you've finished launching!\n\r");
    	            return;
    	        }

                if ( autofly(ship) )
    	        {
    	            ch->println("&RYou'll have to turn off the ships autopilot first....\n\r");
    	            return;
    	        }

                if (arg[0] == '\0')
    	        {
    	            ch->println("&RYou need to specify a target!\n\r");
    	            return;
    	        }

                if ( !str_cmp( arg, "none") )
    	        {
    	            ch->println("&GTarget set to none.\n\r");
    	            if ( ch->in_room->vnum == ship->gunseat )
    	                   ship->target0 = NULL;
    	            if ( ch->in_room->vnum == ship->turret1 )
    	                   ship->target1 = NULL;
    	            if ( ch->in_room->vnum == ship->turret2 )
    	                   ship->target2 = NULL;
					if ( ch->in_room->vnum == ship->turret3 )
						   ship->target3 = NULL;
					if ( ch->in_room->vnum == ship->turret4 )
						   ship->target4 = NULL;
					if ( ch->in_room->vnum == ship->turret5 )
						   ship->target5 = NULL;
					if ( ch->in_room->vnum == ship->turret6 )
						   ship->target6 = NULL;
					if ( ch->in_room->vnum == ship->turret7 )
						   ship->target7 = NULL;
					if ( ch->in_room->vnum == ship->turret8 )
						   ship->target8 = NULL;
					if ( ch->in_room->vnum == ship->turret9 )
						   ship->target9 = NULL;
					if ( ch->in_room->vnum == ship->turret10 )
						   ship->target10 = NULL;
    	            return;
    	        }

                target = get_ship_here( arg, ship->starsystem );
                if (  target == NULL )
                {
                    ch->println("&RThat ship isn't here!\n\r");
                    return;
                }

                if (  target == ship )
                {
                    ch->println("&RYou can't target your own ship!\n\r");
                    return;
                }

                if ( !str_cmp(target->owner, ship->owner) && str_cmp(target->owner , "Public" ) )
                {
                    ch->println("&RThat ship has the same owner... try targetting an enemy ship instead!\n\r");
                    return;
                }

                if ( abs(ship->vx-target->vx) > 5000 ||
                     abs(ship->vy-target->vy) > 5000 ||
                     abs(ship->vz-target->vz) > 5000 )
                {
                    ch->println("&RThat ship is too far away to target.\n\r");
                    return;
                }

//                chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_weaponsystems]) ;
                chance = ch->level;
                if ( number_percent( ) < chance )
    		{
    		   ch->println( "&GTracking target.\n\r");
    		   act( "$n makes some adjustments on the targeting computer.", ch, NULL, argument , TO_ROOM );
//    		   add_timer ( ch , TIMER_DO_FUN , 1 , do_target , 1 );
//    		   ch->dest_buf = str_dup(arg);
    		   return;
	        }
	        ch->println("&RYou fail to work the controls properly.\n\r");
//	        learn_from_failure( ch, gsn_weaponsystems );
    	   	return;

    	case 1:
//    		if ( !ch->dest_buf )
//    		   return;
//    		strcpy(arg, ch->dest_buf);
//    		DISPOSE( ch->dest_buf);
    		break;

    	case SUB_TIMER_DO_ABORT:
//    		DISPOSE( ch->dest_buf );
    		ch->substate = SUB_NONE;
    		if ( (ship = ship_from_cockpit(ch->in_room->vnum)) == NULL )
    		      return;
    	        ch->println("&RYour concentration is broken. You fail to lock onto your target.\n\r");
    		return;
    }

    ch->substate = SUB_NONE;

    if ( (ship = ship_from_turret(ch->in_room->vnum)) == NULL )
    {
       return;
    }

    target = get_ship_here( arg, ship->starsystem );
    if (  target == NULL || target == ship)
    {
           ch->println("&RThe ship has left the starsystem. Targetting aborted.\n\r");
           return;
    }

    if ( ch->in_room->vnum == ship->gunseat )
       ship->target0 = target;

    if ( ch->in_room->vnum == ship->turret1 )
       ship->target1 = target;

    if ( ch->in_room->vnum == ship->turret2 )
        ship->target2 = target;
	if ( ch->in_room->vnum == ship->turret3 )
		ship->target3 = target;

	if ( ch->in_room->vnum == ship->turret4 )
		ship->target4 = target;

	if ( ch->in_room->vnum == ship->turret5 )
		ship->target5 = target;

	if ( ch->in_room->vnum == ship->turret6 )
		ship->target6 = target;

	if ( ch->in_room->vnum == ship->turret7 )
		ship->target7 = target;

	if ( ch->in_room->vnum == ship->turret8 )
		ship->target8 = target;

	if ( ch->in_room->vnum == ship->turret9 )
		ship->target9 = target;

	if ( ch->in_room->vnum == ship->turret10 )
		ship->target10 = target;

    ch->println( "&GTarget Locked.\n\r");
    sprintf( buf , "You are being targetted by %s." , ship->name);
//    //echo_to_cockpit( AT_BLOOD , target , buf );

//    sound_to_room( ch->in_room , "!!SOUND(targetlock)" );
//    learn_from_success( ch, gsn_weaponsystems );

    if ( autofly(target) && !target->target0)
    {
       sprintf( buf , "You are being targetted by %s." , target->name);
//       //echo_to_cockpit( AT_BLOOD , ship , buf );
       target->target0 = ship;
    }
}

/* Fire update by ||, primary/secondary usage, autoblasters, repeating ions... */
/*
void do_sfire(char_data *ch, char *argument )
{
    int chance;
    int range = 1500;
    int iTurret;
    int damage;
	int p,times,size;
	char arg[MSL];
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MSL];

/*
	OBJ_DATA *obj;

	obj = get_eq_char(ch, WEAR_WIELD);
	if(obj)
	{
		if(obj->item_type == ITEM_RLAUNCHER || obj->item_type == ITEM_GLAUNCHER)
	    {
		 do_launch2(ch, argument);
		 return;
		}
	}
*//*

        if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou must be in the gunners chair or turret of a ship to do that!\n\r");
            return;
        }

        if ( ship->ship_class > SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r");
    	            return;
    	        }

        if (ship->shipstate == SHIP_HYPERSPACE)
        {
             ch->println("&RYou can only do that in realspace!\n\r");
             return;
        }
    	if (ship->starsystem == NULL)
    	{
    	     ch->println("&RYou can't do that until after you've finished launching!\n\r");
    	     return;
    	}
    	if ( ship->energy <5 )
        {
             ch->println("&RTheres not enough energy left to fire!\n\r");
             return;
        }

		argument = one_argument(argument, arg);

                if ( autofly(ship) )
    	        {
    	            ch->println("&RYou'll have to turn off the ships autopilot first.\n\r");
    	            return;
    	        }


//        chance = IS_NPC(ch) ? ch->top_level : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
//                           + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );
        chance = ch->level;

    	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( arg , "primary") )
    	{
    	      if (ship->primaryType == B_NONE)
             {
                ch->println("&RThis ship doesn't have a primary weapon system.\n\r");
                return;
             }
    	     if (ship->primaryState == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ship's primary weapon system is damaged.\n\r");
    	      	return;
    	     }
			if(ship->primaryLinked == true)
			{
				switch(ship->primaryType)
				{
					case 0: p = 0; break;
					case 1: p = ship->primaryCount; break;
					case 2: p = 2*ship->primaryCount; break;
					case 3: p = 3*ship->primaryCount; break;
					case 4: p = 4*ship->primaryCount; break;
					case 5: p = 3*ship->primaryCount; break;
					case 6: p = ship->primaryCount; break;
					case 7: p = ship->primaryCount; break;
					case 8: p = 3*ship->primaryCount; break;
					case 9: p = ship->primaryCount; break;
					default: p = 1; break;
				}
			}
			else
			{
				if(ship->primaryType == AUTOBLASTER)
					p = 3;
				else
					p = 1;
			}

			 if(ship->primaryState >= p && ship->primaryType != AUTOBLASTER)
			 {
				ch->println("&RThe primary weapon systems are still recharging.\n\r");
				return;
			 }

			 if(ship->primaryType == AUTOBLASTER && ship->primaryState >= 12)
			{
				 ch->println("&RYou need to let the autoblaster cool down.\n\r");
				 return;
			}
    	     if (ship->target0 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target0;
    	     if (ship->target0->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target0 = NULL;
    	     	return;
    	     }
             if ( abs(target->vx - ship->vx) >1000 ||
                  abs(target->vy - ship->vy) >1000 ||
                  abs(target->vz - ship->vz) >1000 )
             {
                ch->println("&RThat ship is out of range.\n\r");
    	     	return;
             }
             if ( ship->ship_class < SHIP_CRUISER && !is_facing( ship, target ) )
             {
                ch->println("&RYou can only fire in a forward direction. You'll need to turn your ship!\n\r");
    	     	return;
             }
             ship->primaryState++;

		 switch(target->ship_class)
		 {
			 case 0: size = 10;		break;
			 case 1: size = 10;		break;
			 case 2: size = 10;		break;
			 case 3: size = 12;		break;
			 case 4: size = 18;		break;
			 case 5: size = 20;		break;
			 case 6: size = 25;		break;
			 case 7: size = 40;		break;
			 case 8: size = 55;		break;
			 case 9: size = 65;		break;
			 case 10: size = 70;	break;
			 default: size = 70;	break;
		 }

	     chance = 100-((target->manuever - ship->manuever)+65);
             chance -= ( abs(target->vx - ship->vx)/100 );
             chance -= ( abs(target->vy - ship->vy)/100 );
             chance -= ( abs(target->vz - ship->vz)/100 );
		 chance += size-10;
		   if(target->rolling == false)
             chance = URANGE( 5 , chance , 95 );
		   else
			 chance = number_range(5, 8);

             act( "$n presses the fire button.", ch, NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
			  if(ship->primaryType == HEAVY_LASER)
				 {
				  sprintf(buf, "&YHeavy lasers&O fire from %s at you but miss.", ship->name);
//				  //echo_to_cockpit(AT_ORANGE, target, buf);
				  sprintf(buf, "The ship's &Yheavy lasers&O fire at %s but miss.", target->name);
//				  //echo_to_cockpit(AT_ORANGE, ship, buf);
				  sprintf(buf, "&YHeavy laserfire&O from %s barely misses %s.", ship->name, target->name);
//				  echo_to_system(AT_ORANGE, ship, buf, target);
				 }
			 else if(ship->primaryType == AUTOBLASTER)
				 {
				  sprintf(buf, "&YAutoblaster fire&O from %s comes at you but misses.", ship->name);
//				  //echo_to_cockpit(AT_ORANGE, target, buf);
				  sprintf(buf, "The &Yautoblaster&O fires at %s but misses.", target->name);
//				  //echo_to_cockpit(AT_ORANGE, ship, buf);
				  sprintf(buf, "&YAutoblaster fire&O from %s barely misses %s.", ship->name, target->name);
//				  echo_to_system(AT_ORANGE, ship, buf, target);
				 }
			 else
				 {
                  sprintf( buf , "&YLasers&O fire from %s at you but miss." , ship->name);
//                  //echo_to_cockpit( AT_ORANGE , target , buf );
                  sprintf( buf , "The ships &Ylasers&O fire at %s but miss." , target->name);
//                  //echo_to_cockpit( AT_ORANGE , ship , buf );
    	          sprintf( buf, "&YLaserfire&O from %s barely misses %s." , ship->name , target->name );
//                  echo_to_system( AT_ORANGE , ship , buf , target );
				 }
//                learn_from_failure( ch, gsn_spacecombat );
//    	        learn_from_failure( ch, gsn_spacecombat2 );
//    	        learn_from_failure( ch, gsn_spacecombat3 );

			if(p > 1 && str_cmp(argument, "noloop")) // Infinite recursion == bad
              for(times = 1; times < p; times++ )
				do_sfire(ch, "primary noloop");

    	        return;
             }
			if(ship->primaryType == HEAVY_LASER)
			{
				sprintf(buf, "&YHeavy laserfire&O from %s hits %s.", ship->name, target->name);
//				echo_to_system(AT_ORANGE, ship, buf, target);
				sprintf(buf, "You are hit by &Rheavy lasers&r from %s!", ship->name);
//				//echo_to_cockpit(AT_BLOOD, target, buf);
				sprintf(buf, "Your ships lasers hit %s!", target->name);
//				//echo_to_cockpit(AT_YELLOW, ship, buf);
			}
			else if(ship->primaryType == AUTOBLASTER)
			{
				sprintf(buf, "&YAutoblaster fire&O from %s hits %s.", ship->name, target->name);
//				echo_to_system(AT_ORANGE, ship, buf, target);
				sprintf(buf, "You are hit by &Rautoblaster fire&r from %s!", ship->name);
//				//echo_to_cockpit(AT_BLOOD, target, buf);
				sprintf(buf, "Your ship's autoblaster hits %s!", target->name);
//				//echo_to_cockpit(AT_YELLOW, ship, buf);
			}
			else
			{
             sprintf( buf, "&YLaserfire&O from %s hits %s." , ship->name, target->name );
//             echo_to_system( AT_ORANGE , ship , buf , target );
             sprintf( buf , "You are hit by &Rlasers&r from %s!" , ship->name);
//             //echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf , "Your ships lasers hit %s!." , target->name);
//             //echo_to_cockpit( AT_YELLOW , ship , buf );
			}

//             learn_from_success( ch, gsn_spacecombat );
//             learn_from_success( ch, gsn_spacecombat2 );
//             learn_from_success( ch, gsn_spacecombat3 );
             //echo_to_ship( AT_RED , target , "The ship shakes with vibration." );

        if(ship->primaryType == HEAVY_LASER)
             damage_ship_ch( target, 15, 30, ch);
        else if(ship->primaryType == AUTOBLASTER)
             damage_ship_ch( target, 1, 2, ch );
		else
			damage_ship_ch( target, 3, 10, ch);

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
//                //echo_to_cockpit( AT_BLOOD , ship , buf );
             }

			if(p > 1 && str_cmp(argument, "noloop")) // Infinite recursion == bad
              for(times = 1; times < p; times++ )
				do_sfire(ch, "primary noloop");

             return;
    	} // End firing primary

                if ( ch->in_room->vnum == ship->gunseat && !str_prefix( arg , "secondary") )
        {
             if (ship->secondaryType == B_NONE)
             {
                ch->println("&RThis ship doesn't have a secondary weapon system.\n\r");
                return;
             }
             if (ship->secondaryState == LASER_DAMAGED)
             {
                ch->println("&RThe ship's secondary weapon systems are damaged.\n\r");
                return;
             }
			 if(ship->secondaryType == AUTOBLASTER && ship->secondaryState >= 15)
			{
				 ch->println("&RYou need to let the autoblaster cool down.\n\r");
				 return;
			}
			if(ship->secondaryType == REPEATING_ION && ship->secondaryState >= 15)
			{
				ch->println("&RYou need to let the repeating ion cannon cool down.\n\r");
				return;
			}
			if(ship->secondaryLinked == true)
			{
				switch(ship->secondaryType)
				{
					case 0: p = 0; break;
					case 5: p = 3*ship->secondaryCount; break;
					case 7: p = 1*ship->secondaryCount; break;
					case 8: p = 3*ship->secondaryCount; break;
					case 9: p = 1*ship->secondaryCount; break;
					default: p = 1; break;
				}
			}
			else
			{
				if(ship->secondaryType == AUTOBLASTER || ship->secondaryType == REPEATING_ION)
					p = 3;
				else
					p = 1;
			}
             if (ship->secondaryState >= ship->secondaryCount*p && ship->secondaryType != AUTOBLASTER && ship->secondaryType != REPEATING_ION)
    	     {
    	     	ch->println("&RThe secondary weapon systems are still recharging.\n\r");
    	     	return;
    	     }

             if (ship->target0 == NULL )
             {
                ch->println("&RYou need to choose a target first.\n\r");
                return;
             }
             target = ship->target0;
             if (ship->target0->starsystem != ship->starsystem)
             {
                ch->println("&RYour target seems to have left.\n\r");
                ship->target0 = NULL;
                return;
             }
             if ( abs(target->vx - ship->vx) > range ||
                  abs(target->vy - ship->vy) > range ||
                  abs(target->vz - ship->vz) > range )
             {
                ch->println("&RThat ship is out of range.\n\r");
                return;
             }
             if ( ship->ship_class < SHIP_CRUISER && !is_facing( ship, target ) )
             {
                ch->println("&RYou can only fire in a forward direction. You'll need to turn your ship!\n\r");
                return;
             }
             ship->secondaryState++;
		 switch(target->ship_class)
		 {
			 case 0: size = 10;		break;
			 case 1: size = 10;		break;
			 case 2: size = 10;		break;
			 case 3: size = 12;		break;
			 case 4: size = 18;		break;
			 case 5: size = 20;		break;
			 case 6: size = 25;		break;
			 case 7: size = 40;		break;
			 case 8: size = 55;		break;
			 case 9: size = 65;		break;
			 case 10: size = 70;	break;
			 default: size = 70;	break;
		 }
	     chance = 100-((target->manuever - ship->manuever)+65);
             chance -= ( abs(target->vx - ship->vx)/100 );
             chance -= ( abs(target->vy - ship->vy)/100 );
             chance -= ( abs(target->vz - ship->vz)/100 );
		 chance += size-10;
             chance = URANGE( 5 , chance , 95 );
             act( "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
			    if(ship->secondaryType == AUTOBLASTER)
				{
				  sprintf(buf, "&YAutoblaster fire&O from %s comes at you but misses.", ship->name);
//				  //echo_to_cockpit(AT_ORANGE, target, buf);
				  sprintf(buf, "The &Yautoblaster&O fires at %s but misses.", target->name);
//				  //echo_to_cockpit(AT_ORANGE, ship, buf);
				  sprintf(buf, "&YAutoblaster fire&O from %s barely misses %s.", ship->name, target->name);
//				  echo_to_system(AT_ORANGE, ship, buf, target);
			    }
				else if(ship->secondaryType == LIGHT_ION)
				 {
					sprintf( buf , "&CLight ionfire&c from %s speeds toward you but misses." , ship->name);
//					//echo_to_cockpit( AT_PLAIN , target , buf );
					sprintf( buf , "&cThe &Clight ionfire&c speeds toward %s but misses." , target->name);
//					//echo_to_cockpit( AT_PLAIN , ship , buf );
					sprintf( buf, "&CLight ionfire&c from %s barely misses %s." , ship->name , target->name );
//					echo_to_system( AT_PLAIN , ship , buf , target );
				 }
				 else if(ship->secondaryType == REPEATING_ION)
				 {
					sprintf( buf , "&CRepeating ionfire&c from %s speeds toward you but misses." , ship->name);
//					//echo_to_cockpit( AT_PLAIN , target , buf );
					sprintf( buf , "&cThe &Crepeating ionfire&c speeds toward %s but misses." , target->name);
//					//echo_to_cockpit( AT_PLAIN , ship , buf );
					sprintf( buf, "&CRepeating ionfire&c from %s barely misses %s." , ship->name , target->name );
//					echo_to_system( AT_PLAIN , ship , buf , target );
				 }
				 else if(ship->secondaryType == HEAVY_ION)
				 {
					sprintf( buf , "&CHeavy ionfire&c from %s comes at you but misses." , ship->name);
//					//echo_to_cockpit( AT_PLAIN , target , buf );
					sprintf( buf , "&cThe &Cheavy ionfire&c speeds toward %s but misses." , target->name);
//					//echo_to_cockpit( AT_PLAIN , ship , buf );
					sprintf( buf, "&CHeavy ionfire&c from %s barely misses %s." , ship->name , target->name );
//					echo_to_system( AT_PLAIN , ship , buf , target );
				 }
//	            learn_from_failure( ch, gsn_spacecombat );
//   	        learn_from_failure( ch, gsn_spacecombat2 );
//    	        learn_from_failure( ch, gsn_spacecombat3 );

			if(p > 1 && str_cmp(argument, "noloop")) // Infinite recursion == bad
              for(times = 1; times < p; times++ )
				do_sfire(ch, "secondary noloop");
                return;
             }
			 if(ship->secondaryType == LIGHT_ION)
			{
					sprintf( buf , "&cYou are hit with &Clight ionfire&c from %s!" , ship->name);
//					//echo_to_cockpit( AT_PLAIN , target , buf );
					sprintf( buf , "&cThe &Clight ionfire&c hits %s!" , target->name);
//					//echo_to_cockpit( AT_PLAIN , ship , buf );
					sprintf( buf, "&CLight ionfire&c from %s hits %s." , ship->name , target->name );
//					echo_to_system( AT_PLAIN , ship , buf , target );
			}
			else if(ship->secondaryType == REPEATING_ION)
			{
				sprintf(buf, "&cYou are hit with &Crepeating ionfire&c from %s!", ship->name);
//				//echo_to_cockpit( AT_PLAIN, target, buf);
				sprintf(buf, "&cThe &Crepeating ionfire&c hits %s!", target->name);
//				//echo_to_cockpit(AT_PLAIN, ship, buf);
				sprintf(buf, "&CRepeating ionfire&c from %s hits %s.", ship->name, target->name);
//				echo_to_system(AT_PLAIN, ship, buf, target);
			}
			else if(ship->secondaryType == HEAVY_ION)
			{
				sprintf(buf, "&cYou are hit with &Cheavy ionfire&c from %s!", ship->name);
//				//echo_to_cockpit(AT_PLAIN, target, buf);
				sprintf(buf, "&cThe &Cheavy ionfire&c hits %s!", target->name);
//				//echo_to_cockpit(AT_PLAIN, target, buf);
				sprintf(buf, "&CHeavy ionfire&c from %s hits %s.", ship->name, target->name);
//				echo_to_system(AT_PLAIN, ship, buf, target);
			}
			else if(ship->secondaryType == AUTOBLASTER)
			{
				sprintf(buf, "&YAutoblaster fire&O from %s hits %s.", ship->name, target->name);
//				echo_to_system(AT_ORANGE, ship, buf, target);
				sprintf(buf, "You are hit by &Rautoblaster fire&r from %s!", ship->name);
//				//echo_to_cockpit(AT_BLOOD, target, buf);
				sprintf(buf, "Your ship's autoblaster hits %s!", target->name);
//				//echo_to_cockpit(AT_YELLOW, ship, buf);
			}


//             learn_from_success( ch, gsn_spacecombat );
//             learn_from_success( ch, gsn_spacecombat2 );
//             learn_from_success( ch, gsn_spacecombat3 );

             //echo_to_ship( AT_RED , target , "&RThe ship vibrates and the lights dim momentarily." );

			 if(ship->secondaryType == LIGHT_ION)
              damage_ship_ch_ion( target , 10 , 15 , ch );
             else if(ship->secondaryType == REPEATING_ION)
			  damage_ship_ch_ion(target, 2, 4, ch);
			 else if(ship->secondaryType == HEAVY_ION)
			  damage_ship_ch_ion(target, 15, 25, ch);
			 else
			  damage_ship_ch(target, 1, 4, ch);

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
//                //echo_to_cockpit( AT_BLOOD , ship , buf );
             }

			if(p > 1 && str_cmp(argument, "noloop")) // Infinite recursion == bad
              for(times = 1; times < p; times++ )
				do_sfire(ch, "secondary noloop");
             return;
        } // End secondary fire

        if ( ch->in_room->vnum == ship->gunseat && !str_prefix( arg , "missile") )
    	{
    	     if (ship->missilestate == MISSILE_DAMAGED)
    	     {
    	        ch->println("&RThe ships missile launchers are damaged.\n\r");
    	      	return;
    	     }
             if (ship->missiles <= 0)
    	     {
    	     	ch->println("&RYou have no missiles to fire!\n\r");
    	        return;
    	     }
    	     if (ship->missilestate != MISSILE_READY )
    	     {
    	     	ch->println("&RThe missiles are still reloading.\n\r");
    	     	return;
    	     }
    	     if (ship->target0 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target0;
             if (ship->target0->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target0 = NULL;
    	     	return;
    	     }
             if ( abs(target->vx - ship->vx) >1000 ||
                  abs(target->vy - ship->vy) >1000 ||
                  abs(target->vz - ship->vz) >1000 )
             {
                ("&RThat ship is out of missile range.\n\r");
    	     	return;
             }
             if ( ship->ship_class < SHIP_CRUISER && !is_facing( ship, target ) )
             {
                ch->println("&RMissiles can only fire forward. You'll need to turn your ship!\n\r");
    	     	return;
             }
	     chance = 100-((target->manuever - ship->manuever)+50);
             chance -= ( abs(target->vx - ship->vx)/100 );
             chance -= ( abs(target->vy - ship->vy)/100 );
             chance -= ( abs(target->vz - ship->vz)/100 );
	     //Extra chance for missiles due to computer compensation.
	     chance += 20;
             chance = URANGE( 5 , chance , 95 );
             act( "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
                ch->println( "&RYou fail to lock onto your target, and the missile goes off on a tangent.\n\r" );
                ship->missilestate = MISSILE_RELOAD_2;
    	        return;
             }
             new_missile( ship , target , ch , CONCUSSION_MISSILE );
             ship->missiles-- ;
             act( "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
//             //echo_to_cockpit( AT_YELLOW , ship , "Missiles launched.");
             sprintf( buf , "Incoming missile from %s." , ship->name);
//             //echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf, "%s fires a missile towards %s." , ship->name, target->name );
//             echo_to_system( AT_ORANGE , ship , buf , target );
//             learn_from_success( ch, gsn_weaponsystems );
             if ( ship->ship_class >= SHIP_CRUISER )
                   ship->missilestate = MISSILE_RELOAD;
             else
                   ship->missilestate = MISSILE_FIRED;

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
//                //echo_to_cockpit( AT_BLOOD , ship , buf );
             }

			if(ship->warheadLinked == true && str_cmp(argument, "noloop"))
			{
				if(ship->maxtorpedos > 0 && ship->torpedos > 0) do_sfire(ch, "torpedo noloop");
				if(ship->maxrockets > 0 && ship->rockets > 0) do_sfire(ch, "rocket noloop");
			}
             return;
    	}
        if ( ch->in_room->vnum == ship->gunseat && !str_prefix( arg , "torpedo") )
    	{
    	     if (ship->torpedostate == MISSILE_DAMAGED)
    	     {
    	        ch->println("&RThe ship's torpedo launchers are damaged.\n\r");
    	      	return;
    	     }
             if (ship->torpedos <= 0)
    	     {
    	     	ch->println("&RYou have no torpedos to fire!\n\r");
    	        return;
    	     }
    	     if (ship->torpedostate != MISSILE_READY )
    	     {
    	     	ch->println("&RThe torpedos are still reloading.\n\r");
    	     	return;
    	     }
    	     if (ship->target0 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target0;
             if (ship->target0->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target0 = NULL;
    	     	return;
    	     }
             if ( abs(target->vx - ship->vx) >1000 ||
                  abs(target->vy - ship->vy) >1000 ||
                  abs(target->vz - ship->vz) >1000 )
             {
                ch->println("&RThat ship is out of torpedo range.\n\r");
    	     	return;
             }
             if ( ship->ship_class < SHIP_CRUISER && !is_facing( ship, target ) )
             {
                ch->println("&RTorpedos can only fire forward. You'll need to turn your ship!\n\r");
    	     	return;
             }
	     chance = 100-((target->manuever - ship->manuever)+50);
             chance -= ( abs(target->vx - ship->vx)/100 );
             chance -= ( abs(target->vy - ship->vy)/100 );
             chance -= ( abs(target->vz - ship->vz)/100 );
	     //Extra chance for missiles due to computer compensation.
	     chance += 20;
             chance = URANGE( 5 , chance , 95 );
             act(  "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
                ch->println( "&RYou fail to lock onto your target, and the torpedo goes off on a tangent.\n\r" );
                ship->torpedostate = MISSILE_RELOAD_2;
    	        return;
             }
             new_missile( ship , target , ch , PROTON_TORPEDO );
             ship->torpedos-- ;
             act( "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
//             //echo_to_cockpit( AT_YELLOW , ship , "Torpedo launched.");
             sprintf( buf , "Incoming torpedo from %s." , ship->name);
//             //echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf, "%s fires a torpedo towards %s." , ship->name, target->name );
//             echo_to_system( AT_ORANGE , ship , buf , target );
//             learn_from_success( ch, gsn_weaponsystems );
             if ( ship->ship_class >= SHIP_CRUISER )
                   ship->torpedostate = MISSILE_RELOAD;
             else
                   ship->torpedostate = MISSILE_FIRED;

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
//                //echo_to_cockpit( AT_BLOOD , ship , buf );
             }

			 if(ship->warheadLinked == true && str_cmp(argument, "noloop"))
			{
				 if(ship->maxmissiles > 0 && ship->missiles > 0)  do_sfire(ch, "missile noloop");
				 if(ship->maxrockets > 0 && ship->rockets > 0) do_sfire(ch, "rocket noloop");
			}
             return;
    	}

        if ( ch->in_room->vnum == ship->gunseat && !str_prefix( arg , "rocket") )
    	{
    	     if (ship->rocketstate == MISSILE_DAMAGED)
    	     {
    	        ch->println("&RThe ship's rocket launchers are damaged.\n\r");
    	      	return;
    	     }
             if (ship->rockets <= 0)
    	     {
    	     	ch->println("&RYou have no rockets to fire!\n\r");
    	        return;
    	     }
    	     if (ship->rocketstate != MISSILE_READY )
    	     {
    	     	ch->println("&RThe rockets are still reloading.\n\r");
    	     	return;
    	     }
    	     if (ship->target0 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target0;
             if (ship->target0->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target0 = NULL;
    	     	return;
    	     }
             if ( abs(target->vx - ship->vx) >800 ||
                  abs(target->vy - ship->vy) >800 ||
                  abs(target->vz - ship->vz) >800 )
             {
                ch->println("&RThat ship is out of rocket range.\n\r");
    	     	return;
             }
             if ( ship->ship_class < SHIP_CRUISER && !is_facing( ship, target ) )
             {
                ch->println("&RRockets can only fire forward. You'll need to turn your ship!\n\r");
    	     	return;
             }
	     chance = 100-((target->manuever - ship->manuever)+50);
             chance -= ( abs(target->vx - ship->vx)/100 );
             chance -= ( abs(target->vy - ship->vy)/100 );
             chance -= ( abs(target->vz - ship->vz)/100 );
	     //Extra chance for missiles due to computer compensation.
	     chance += 20;
             chance = URANGE( 5 , chance , 95 );
             act( "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
                ch->println( "&RYou fail to lock onto your target, and the rocket goes off on a tangent.\n\r" );
                ship->rocketstate = MISSILE_RELOAD_2;
    	        return;
             }
             new_missile( ship , target , ch , HEAVY_ROCKET );
             ship->rockets-- ;
             act("$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
//             //echo_to_cockpit( AT_YELLOW , ship , "Rocket launched.");
             sprintf( buf , "Incoming rocket from %s." , ship->name);
//             //echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf, "%s fires a heavy rocket towards %s." , ship->name, target->name );
//             echo_to_system( AT_ORANGE , ship , buf , target );
//             learn_from_success( ch, gsn_weaponsystems );
             if ( ship->ship_class >= SHIP_CRUISER )
                   ship->rocketstate = MISSILE_RELOAD;
             else
                   ship->rocketstate = MISSILE_FIRED;

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
//                //echo_to_cockpit( AT_BLOOD , ship , buf );
             }

			 if(ship->warheadLinked == true && str_cmp(argument, "noloop"))
			{
				 if(ship->maxmissiles > 0 && ship->missiles > 0) do_sfire(ch, "missile noloop");
				 if(ship->maxtorpedos > 0 && ship->torpedos > 0) do_sfire(ch, "torpedo noloop");
			}
             return;
    	}


  /* Turrets ....... Shit. A lot of bullshit :) */
/*
        if ( (ch->in_room->vnum == ship->turret1  && !str_prefix(argument, "lasers")) ||
             (ch->in_room->vnum == ship->turret2  && !str_prefix(argument, "lasers")) ||
             (ch->in_room->vnum == ship->turret3  && !str_prefix(argument, "lasers")) ||
             (ch->in_room->vnum == ship->turret4  && !str_prefix(argument, "lasers")) ||
             (ch->in_room->vnum == ship->turret5  && !str_prefix(argument, "lasers")) ||
             (ch->in_room->vnum == ship->turret5  && !str_prefix(argument, "lasers")) ||
             (ch->in_room->vnum == ship->turret6  && !str_prefix(argument, "lasers")) ||
             (ch->in_room->vnum == ship->turret7  && !str_prefix(argument, "lasers")) ||
             (ch->in_room->vnum == ship->turret8  && !str_prefix(argument, "lasers")) ||
             (ch->in_room->vnum == ship->turret9  && !str_prefix(argument, "lasers")) ||
             (ch->in_room->vnum == ship->turret10 && !str_prefix(argument, "lasers"))   )
    	{

    if(ch->in_room->vnum == ship->turret1)   		iTurret = 1;
    else if(ch->in_room->vnum == ship->turret2)   	iTurret = 2;
    else if(ch->in_room->vnum == ship->turret3)		iTurret = 3;
    else if(ch->in_room->vnum == ship->turret4)		iTurret = 4;
    else if(ch->in_room->vnum == ship->turret5)		iTurret = 5;
    else if(ch->in_room->vnum == ship->turret6)		iTurret = 6;
    else if(ch->in_room->vnum == ship->turret7)		iTurret = 7;
    else if(ch->in_room->vnum == ship->turret8)		iTurret = 8;
    else if(ch->in_room->vnum == ship->turret9)		iTurret = 9;
    else if(ch->in_room->vnum == ship->turret10)	iTurret = 10;
    else
    {
    	ch->println("Error: do_sfire: turrets. Tell someone important.\n\r");
    	return;
    }

    	 switch( iTurret )
    	 {
    	  case 1:
    	     if (ship->statet1 == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ships turret is damaged.\n\r");
    	      	return;
    	     }
             if (ship->statet1 > ship->ship_class )
    	     {
    	     	ch->println("&RThe turbolaser is recharging.\n\r");
    	     	return;
    	     }
    	     if (ship->target1 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target1;
             if (ship->target1->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target1 = NULL;
    	     	return;
    	     }
	     break;
    	  case 2:
    	     if (ship->statet2 == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ships turret is damaged.\n\r");
    	      	return;
    	     }
             if (ship->statet2 > ship->ship_class )
    	     {
    	     	ch->println("&RThe turbolaser is recharging.\n\r");
    	     	return;
    	     }
    	     if (ship->target2 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target2;
             if (ship->target2->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target2 = NULL;
    	     	return;
    	     }
	     break;
    	  case 3:
    	     if (ship->statet3 == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ships turret is damaged.\n\r");
    	      	return;
    	     }
             if (ship->statet3 > ship->ship_class )
    	     {
    	     	ch->println("&RThe turbolaser is recharging.\n\r");
    	     	return;
    	     }
    	     if (ship->target3 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target3;
             if (ship->target3->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target3 = NULL;
    	     	return;
    	     }
	     break;
    	  case 4:
    	     if (ship->statet4 == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ships turret is damaged.\n\r");
    	      	return;
    	     }
             if (ship->statet4 > ship->ship_class )
    	     {
    	     	ch->println("&RThe turbolaser is recharging.\n\r");
    	     	return;
    	     }
    	     if (ship->target4 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target4;
             if (ship->target4->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target4 = NULL;
    	     	return;
    	     }
	     break;
    	  case 5:
    	     if (ship->statet5 == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ships turret is damaged.\n\r");
    	      	return;
    	     }
             if (ship->statet5 > ship->ship_class )
    	     {
    	     	ch->println("&RThe turbolaser is recharging.\n\r");
    	     	return;
    	     }
    	     if (ship->target5 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target5;
             if (ship->target5->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target5 = NULL;
    	     	return;
    	     }
	     break;
    	  case 6:
    	     if (ship->statet6 == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ships turret is damaged.\n\r");
    	      	return;
    	     }
             if (ship->statet6 > ship->ship_class )
    	     {
    	     	ch->println("&RThe turbolaser is recharging.\n\r");
    	     	return;
    	     }
    	     if (ship->target6 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target6;
             if (ship->target6->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target6 = NULL;
    	     	return;
    	     }
	     break;
    	  case 7:
    	     if (ship->statet7 == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ships turret is damaged.\n\r");
    	      	return;
    	     }
             if (ship->statet7 > ship->ship_class )
    	     {
    	     	ch->println("&RThe turbolaser is recharging.\n\r");
    	     	return;
    	     }
    	     if (ship->target7 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target7;
             if (ship->target7->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target7 = NULL;
    	     	return;
    	     }
	     break;
    	  case 8:
    	     if (ship->statet8 == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ships turret is damaged.\n\r");
    	      	return;
    	     }
             if (ship->statet8 > ship->ship_class )
    	     {
    	     	ch->println("&RThe turbolaser is recharging.\n\r");
    	     	return;
    	     }
    	     if (ship->target8 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target8;
             if (ship->target8->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target8 = NULL;
    	     	return;
    	     }
	     break;
    	  case 9:
    	     if (ship->statet9 == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ships turret is damaged.\n\r");
    	      	return;
    	     }
             if (ship->statet9 > ship->ship_class )
    	     {
    	     	ch->println("&RThe turbolaser is recharging.\n\r");
    	     	return;
    	     }
    	     if (ship->target9 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target9;
             if (ship->target9->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target9 = NULL;
    	     	return;
    	     }
	     break;
    	  case 10:
    	     if (ship->statet10 == LASER_DAMAGED)
    	     {
    	        ch->println("&RThe ships turret is damaged.\n\r");
    	      	return;
    	     }
             if (ship->statet10 > ship->ship_class )
    	     {
    	     	ch->println("&RThe turbolaser is recharging.\n\r");
    	     	return;
    	     }
    	     if (ship->target10 == NULL )
    	     {
    	     	ch->println("&RYou need to choose a target first.\n\r");
    	     	return;
    	     }
    	     target = ship->target10;
             if (ship->target10->starsystem != ship->starsystem)
    	     {
    	     	ch->println("&RYour target seems to have left.\n\r");
    	        ship->target10 = NULL;
    	     	return;
    	     }
    	  } /* end switch */
/*
             if ( abs(target->vx - ship->vx) >1000 ||
                  abs(target->vy - ship->vy) >1000 ||
                  abs(target->vz - ship->vz) >1000 )
             {
                ch->println("&RThat ship is out of laser range.\n\r");
    	     	return;
             }

             switch(iTurret)
             {
              case 1: ship->statet1++; 	break;
              case 2: ship->statet2++;	break;
              case 3: ship->statet3++;	break;
              case 4: ship->statet4++;	break;
              case 5: ship->statet5++;	break;
              case 6: ship->statet6++;	break;
              case 7: ship->statet7++;	break;
              case 8: ship->statet8++;	break;
              case 9: ship->statet9++;	break;
              case 10: ship->statet10++; break;
              default: ch->println("Something bad happened.\n\r"); return;
             }

             act( "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );

	     chance = 100-((target->manuever - ship->manuever)+50);
             chance -= ( abs(target->vx - ship->vx)/100 );
             chance -= ( abs(target->vy - ship->vy)/100 );
             chance -= ( abs(target->vz - ship->vz)/100 );
             chance = URANGE( 5 , chance , 95 );

	     if(number_percent() > chance)
             {
                sprintf( buf , "Turbolasers fire from %s at you but miss." , ship->name);
//                //echo_to_cockpit( AT_ORANGE , target , buf );
                sprintf( buf , "Turbolasers fire from the ships turret at %s but miss." , target->name);
//                //echo_to_cockpit( AT_ORANGE , ship , buf );
                sprintf( buf, "%s fires at %s but misses." , ship->name, target->name );
//                echo_to_system( AT_ORANGE , ship , buf , target );

//                learn_from_failure( ch, gsn_spacecombat );
//    	        learn_from_failure( ch, gsn_spacecombat2 );
//    	        learn_from_failure( ch, gsn_spacecombat3 );

    	        return;
             }
             damage = number_range(3, 18);
             sprintf( buf, "Turbolasers fire from %s, hitting %s." , ship->name, target->name );
//             echo_to_system( AT_ORANGE , ship , buf , target );
             if(damage > 0)
              sprintf(buf, "You are hit by turbolasers from %s!", ship->name);
             else
              sprintf( buf , "You are hit by turbolasers from %s, but the shields hold." , ship->name);

//             //echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf , "Turbolasers fire from the turret, hitting %s!." , target->name);
//             //echo_to_cockpit( AT_YELLOW , ship , buf );

//             learn_from_success( ch, gsn_spacecombat );
//             learn_from_success( ch, gsn_spacecombat2 );
//             learn_from_success( ch, gsn_spacecombat3 );


//             echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
             damage_ship_ch( target , 2, damage, ch );

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
//                //echo_to_cockpit( AT_BLOOD , ship , buf );
             }

             return;
    	}

        ch->println( "&RYou can't fire that!\n\r" );

}


void do_calculate(char_data *ch, char *argument )
{
    char arg1[MIL];
    char arg2[MIL];
    char arg3[MIL];
    int chance , count;
    SHIP_DATA *ship;
    SPACE_DATA *starsystem;

    argument = one_argument( argument , arg1);
    argument = one_argument( argument , arg2);
    argument = one_argument( argument , arg3);


    	        if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
    	        {
    	            ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
    	            return;
    	        }

                if ( ship->ship_class > SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r");
    	            return;
    	        }

                if (  (ship = ship_from_navseat(ch->in_room->vnum))  == NULL )
    	        {
    	            ch->println("&RYou must be at a nav computer to calculate jumps.\n\r");
    	            return;
    	        }

                if ( autofly(ship)  )
    	        {
    	            ch->println("&RYou'll have to turn off the ships autopilot first....\n\r");
    	            return;
    	        }

                if  ( ship->ship_class == SHIP_SPACE_STATION )
                {
                   ch->println( "&RAnd what exactly are you going to calculate...?\n\r" );
                   return;
                }
    	        if (ship->hyperspeed == 0)
                {
                  ch->println("&RThis ship is not equipped with a hyperdrive!\n\r");
                  return;
                }
                if (ship->shipstate == SHIP_DOCKED)
    	        {
    	            ch->println("&RYou can't do that until after you've launched!\n\r");
    	            return;
    	        }
    	        if (ship->starsystem == NULL)
    	        {
    	            ch->println("&RYou can only do that in realspace.\n\r");
    	            return;
    	        }
    	        if (argument[0] == '\0')
    	        {
		 if(IS_SET(ship->flags, SHIP_SIMULATOR))
		 {
		  ch->println("&WFormat: Calculate simulator <x> <y> <z>\n\r");
		  return;
		 }
    	            ch->println("&WFormat: Calculate <starsystem> <entry x> <entry y> <entry z>\n\r&wPossible destinations:\n\r");
    	            for ( starsystem = first_starsystem; starsystem; starsystem = starsystem->next )
                    {
			if(!str_cmp(starsystem->name, "Simulator")) continue;
//                       //set_char_color( AT_NOTE, ch );
                       ch->printlnf( "%-30s %d\n\r",starsystem->name,
                                 (abs(starsystem->xpos - ship->starsystem->xpos)+
                                 abs(starsystem->ypos - ship->starsystem->ypos))/2);
                       count++;
                    }
                    if ( !count )
                    {
                        ch->println( "No Starsystems found.\n\r");
                    }
                    return;
    	        }
//                chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_navigation]) ;
                chance = ch->level;
                if ( number_percent( ) > chance )
    		{
	           ch->println("&RYou cant seem to figure the charts out today.\n\r");
//	           learn_from_failure( ch, gsn_navigation );
    	   	   return;
    	   	}


    ship->currjump = starsystem_from_name( arg1 );
    ship->jx = atoi(arg2);
    ship->jy = atoi(arg3);
    ship->jz = atoi(argument);

    if ( ship->currjump == NULL)
    {
       ch->println( "&RYou can't seem to find that starsytsem on your charts.\n\r");
       return;
    }
    else if (IS_SET(ship->flags, SHIP_SIMULATOR) && ship->currjump != starsystem_from_name("Simulator") )
    {
       ch->println( "&RYou can't seem to find that starsytsem on your charts.\n\r");
       ship->currjump = NULL;
       return;
    }
    else if (!IS_SET(ship->flags, SHIP_SIMULATOR) && ship->currjump == starsystem_from_name("Simulator") )
    {
       ch->println( "&RYou can't seem to find that starsytsem on your charts.\n\r");
       ship->currjump = NULL;
       return;
    }
    else
    {
        SPACE_DATA * starsystem;

        starsystem = ship->currjump;

          if ( starsystem->star1 && strcmp(starsystem->star1,"") &&
                     abs(ship->jx - starsystem->s1x) < 300 &&
                     abs(ship->jy - starsystem->s1y) < 300 &&
                     abs(ship->jz - starsystem->s1z) < 300 )
                {
                    //echo_to_cockpit( AT_RED, ship, "WARNING.. Jump coordinates too close to stellar object.");
                    //echo_to_cockpit( AT_RED, ship, "WARNING.. Hyperjump NOT set.");
                    ship->currjump = NULL;
                    return;
                }
          else if ( starsystem->star2 && strcmp(starsystem->star2,"") &&
               abs(ship->jx - starsystem->s2x) < 300 &&
                     abs(ship->jy - starsystem->s2y) < 300 &&
                     abs(ship->jz - starsystem->s2z) < 300 )
                {
                    //echo_to_cockpit( AT_RED, ship, "WARNING.. Jump coordinates too close to stellar object.");
                    //echo_to_cockpit( AT_RED, ship, "WARNING.. Hyperjump NOT set.");
                    ship->currjump = NULL;
                    return;
                }
          else if ( starsystem->planet1 && strcmp(starsystem->planet1,"") &&
               abs(ship->jx - starsystem->p1x) < 300 &&
                     abs(ship->jy - starsystem->p1y) < 300 &&
                     abs(ship->jz - starsystem->p1z) < 300 )
                {
                    //echo_to_cockpit( AT_RED, ship, "WARNING.. Jump coordinates too close to stellar object.");
                    //echo_to_cockpit( AT_RED, ship, "WARNING.. Hyperjump NOT set.");
                    ship->currjump = NULL;
                    return;
                }
          else if ( starsystem->planet2 && strcmp(starsystem->planet2,"") &&
               abs(ship->jx - starsystem->p2x) < 300 &&
                     abs(ship->jy - starsystem->p2y) < 300 &&
                     abs(ship->jz - starsystem->p2z) < 300 )
                {
                    //echo_to_cockpit( AT_RED, ship, "WARNING.. Jump coordinates too close to stellar object.");
                    //echo_to_cockpit( AT_RED, ship, "WARNING.. Hyperjump NOT set.");
                    ship->currjump = NULL;
                    return;
                }
          else if ( starsystem->planet3 && strcmp(starsystem->planet3,"") &&
               abs(ship->jx - starsystem->p3x) < 300 &&
                     abs(ship->jy - starsystem->p3y) < 300 &&
                     abs(ship->jz - starsystem->p3z) < 300 )
                {
                    //echo_to_cockpit( AT_RED, ship, "WARNING.. Jump coordinates too close to stellar object.");
                    //echo_to_cockpit( AT_RED, ship, "WARNING.. Hyperjump NOT set.");
                    ship->currjump = NULL;
                    return;
                }
          else
          {
             ship->jx += number_range ( -250 , 250 );
             ship->jy += number_range ( -250 , 250 );
             ship->jz += number_range ( -250 , 250 );
          }
    }

    ship->hyperdistance  = abs(ship->starsystem->xpos - ship->currjump->xpos) ;
    ship->hyperdistance += abs(ship->starsystem->ypos - ship->currjump->ypos) ;
    ship->hyperdistance /= 5;

    if (ship->hyperdistance<100)
      ship->hyperdistance = 100;

    ship->hyperdistance += number_range(0, 200);

//    sound_to_room( ch->in_room , "!!SOUND(computer)" );

    ch->println( "&GHyperspace course set. Ready for the jump to lightspeed.\n\r");
    act( "$n does some calculations using the ships computer.", ch, NULL, argument , TO_ROOM );

//    learn_from_success( ch, gsn_navigation );

    WAIT_STATE( ch , 2*PULSE_VIOLENCE );
}

void do_recharge(char_data *ch, char *argument )
{
    int recharge;
    int chance;
    SHIP_DATA *ship;


        if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
            return;
        }
        if (  (ship = ship_from_coseat(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RThe controls must be at the co-pilot station.\n\r");
            return;
        }

                if ( autofly(ship)  )
    	        {
    	            ch->println("&R...\n\r");
    	            return;
    	        }

                if (ship->shipstate == SHIP_DISABLED)
    	        {
    	            ch->println("&RThe ships drive is disabled. Unable to manuever.\n\r");
    	            return;
    	        }

        if ( ship->energy < 100 )
        {
              ch->println("&RTheres not enough energy!\n\r");
              return;
        }

//        chance = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_shipsystems]);
        chance = ch->level;

        if ( number_percent( ) > chance )
        {
            ch->println("&RYou fail to work the controls properly.\n\r");
//            learn_from_failure( ch, gsn_shipsystems );
    	   return;
        }
   if(ship->shield < ship->maxshield)
   {
    ch->println( "&GRecharging shields..\n\r");
    act(  "$n pulls back a lever on the control panel.", ch,
         NULL, argument , TO_ROOM );

//    learn_from_success( ch, gsn_shipsystems );

    recharge  = UMIN(  ship->maxshield-ship->shield , ship->energy*5 + 100 );
    recharge  = URANGE( 1, recharge , 25+ship->ship_class*25 );
    ship->shield += recharge;
    ship->energy -= recharge*3;
   }
   else
    ch->println("&GShip shields are at maximum.\n\r");
}



void do_refuel(char_data *ch, char *argument )
{
}

void do_addpilot(char_data *ch, char *argument )
{
    SHIP_DATA *ship;

   if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
   {
           ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
           return;
   }

                if  ( ship->ship_class == SHIP_SPACE_STATION )
                {
                   ch->println( "&RYou can't do that here.\n\r" );
                   return;
                }

   if ( str_cmp( ship->owner , ch->name ) )
   {

    /* if ( !IS_NPC(ch) && ch->pcdata && ch->pcdata->clan && !str_cmp(ch->pcdata->clan->name,ship->owner) )
         if ( !str_cmp(ch->pcdata->clan->leader,ch->name) )
           ;
         else if ( !str_cmp(ch->pcdata->clan->number1,ch->name) )
           ;
         else if ( !str_cmp(ch->pcdata->clan->number2,ch->name) )
           ;
         else
         {
           ch->println( "&RThat isn't your ship!" ,ch );
   	   return;
         }
     else*/
//     {
/*   	ch->println( "&RThat isn't your ship!" );
   	return;
//     }

   }

   if (argument[0] == '\0')
   {
       ch->println( "&RAdd which pilot?\n\r" );
       return;
   }

   if ( str_cmp( ship->pilot , "" ) )
   {
        if ( str_cmp( ship->copilot , "" ) )
        {
             ch->println( "&RYou already have a pilot and copilot..\n\r"  );
             ch->println( "&RTry rempilot first.\n\r" );
             return;
        }

//      STRFREE( ship->copilot );
	ship->copilot = str_dup( argument );
	ch->println( "Copilot Added.\n\r" );
	save_ship( ship );
	return;

        return;
   }

//   STRFREE( ship->pilot );
   ship->pilot = str_dup( argument );
   ch->println( "Pilot Added.\n\r" );
   save_ship( ship );

}

void do_rempilot(char_data *ch, char *argument )
{
    SHIP_DATA *ship;

   if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
   {
           ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
           return;
   }

                if  ( ship->ship_class == SHIP_SPACE_STATION )
                {
                   ch->println( "&RYou can't do that here.\n\r" );
                   return;
                }

   if ( str_cmp( ship->owner , ch->name ) )
   {
/*
     if ( !IS_NPC(ch) && ch->pcdata && ch->pcdata->clan && !str_cmp(ch->pcdata->clan->name,ship->owner) )
         if ( !str_cmp(ch->pcdata->clan->leader,ch->name) )
           ;
         else if ( !str_cmp(ch->pcdata->clan->number1,ch->name) )
           ;
         else if ( !str_cmp(ch->pcdata->clan->number2,ch->name) )
           ;
         else
         {
           ch->println( "&RThat isn't your ship!" );
   	   return;
         }
     else
     {*/
  /* 	ch->println( "&RThat isn't your ship!" );
   	return;
//     }

   }

   if (argument[0] == '\0')
   {
       ch->println( "&RRemove which pilot?\n\r" );
       return;
   }

   if ( !str_cmp( ship->pilot , argument ) )
   {
 //     STRFREE( ship->pilot );
	ship->pilot = str_dup( "" );
	ch->println( "Pilot Removed.\n\r" );
	save_ship( ship );
	return;
   }

   if ( !str_cmp( ship->copilot , argument ) )
   {
//        STRFREE( ship->copilot );
	ship->copilot = str_dup( "" );
	ch->println( "Copilot Removed.\n\r" );
	save_ship( ship );
	return;
    }

    ch->println( "&RThat person isn't listed as one of the ships pilots.\n\r" );

}
/*
void do_radar( char_data *ch, char *argument )
{
    SHIP_DATA *target;
	PLANET_DATA *planet;
    int chance;
    SHIP_DATA *ship;
    MISSILE_DATA *missile;

        if (   (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou must be in the cockpit or turret of a ship to do that!\n\r");
            return;
        }

        if ( ship->ship_class > SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r");
    	            return;
    	        }

        if (ship->shipstate == SHIP_DOCKED)
        {
            ch->println("&RWait until after you launch!\n\r");
            return;
        }

        if (ship->shipstate == SHIP_HYPERSPACE)
        {
            ch->println("&RYou can only do that in realspace!\n\r");
            return;
        }

    	if (ship->starsystem == NULL)
    	{
    	       ch->println("&RYou can't do that unless the ship is flying in realspace!\n\r");
    	       return;
    	}

//        chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_navigation]) ;
        chance = ch->level;
        if ( number_percent( ) > chance )
        {
           ch->println("&RYou fail to work the controls properly.\n\r");
//           learn_from_failure( ch, gsn_navigation );
    	   return;
        }


    act( "$n checks the radar.", ch, NULL, argument , TO_ROOM );

    //set_char_color(  AT_WHITE, ch );
    ch->printlnf(  "&W%s Starsystem\n\r\n\r" , ship->starsystem->name );
    	           //set_char_color(  AT_LBLUE, ch );
    	           if ( ship->starsystem->star1 && str_cmp(ship->starsystem->star1,"") )
    	                 ch->printlnf(  "&YThe star, %s   %d %d %d\n\r" ,
    	                        ship->starsystem->star1,
    	                        ship->starsystem->s1x,
    	                        ship->starsystem->s1y,
    	                        ship->starsystem->s1z );
    	           if ( ship->starsystem->star2 && str_cmp(ship->starsystem->star2,"")  )
    	                 ch->printlnf(  "&YThe star, %s   %d %d %d\n\r" ,
    	                        ship->starsystem->star2,
    	                        ship->starsystem->s2x,
    	                        ship->starsystem->s2y,
    	                        ship->starsystem->s2z );

    			   if ( ship->starsystem->planet1 && str_cmp(ship->starsystem->planet1,"")  ){

					 for(planet = first_planet; planet; planet = planet->next){

					   if(!str_cmp(planet->name, ship->starsystem->planet1))
						   break;
				   }

					   if(planet->controls == 0)
    	                 ch->printlnf(  "&GThe planet, %s   %d %d %d\n\r" ,
    	                        ship->starsystem->planet1,
    	                        ship->starsystem->p1x,
    	                        ship->starsystem->p1y,
    	                        ship->starsystem->p1z );
				       else
						 ch->printlnf(  "&OA fairly large asteroid, designation %s    %d %d %d\n\r",
								ship->starsystem->planet1,
    	                        ship->starsystem->p1x,
    	                        ship->starsystem->p1y,
    	                        ship->starsystem->p1z );
				   }

    			   if ( ship->starsystem->planet2 && str_cmp(ship->starsystem->planet2,"")  ){

					 for(planet = first_planet; planet; planet = planet->next){

					   if(!str_cmp(planet->name, ship->starsystem->planet2))
						   break;
				   }

					   if(planet->controls == 0)
    	                 ch->printlnf(  "&GThe planet, %s   %d %d %d\n\r" ,
    	                        ship->starsystem->planet2,
    	                        ship->starsystem->p2x,
    	                        ship->starsystem->p2y,
    	                        ship->starsystem->p2z );
				       else
						 ch->printlnf(  "&OA fairly large asteroid, designation %s    %d %d %d\n\r",
								ship->starsystem->planet2,
    	                        ship->starsystem->p2x,
    	                        ship->starsystem->p2y,
    	                        ship->starsystem->p2z );
				   }
    			   if ( ship->starsystem->planet3 && str_cmp(ship->starsystem->planet3,"")  ){

					 for(planet = first_planet; planet; planet = planet->next){

					   if(!str_cmp(planet->name, ship->starsystem->planet3))
						   break;
				   }

					   if(planet->controls == 0)
    	                 ch->printlnf(  "&GThe planet, %s   %d %d %d\n\r" ,
    	                        ship->starsystem->planet3,
    	                        ship->starsystem->p3x,
    	                        ship->starsystem->p3y,
    	                        ship->starsystem->p3z );
				       else
						 ch->printlnf(  "&OA fairly large asteroid, designation %s    %d %d %d\n\r",
								ship->starsystem->planet3,
    	                        ship->starsystem->p3x,
    	                        ship->starsystem->p3y,
    	                        ship->starsystem->p3z );
				   }
    	           ch->printlnf( "\n\r");
    	           for ( target = ship->starsystem->first_ship; target; target = target->next_in_starsystem )
                   {
                        if ( target != ship )
					   {
						  if(ship->ship_class!=11)
                           ch->printlnf(  "&R&C%s    %.0f %.0f %.0f\n\r",
                           	target->name,
                           	target->vx,
                           	target->vy,
                           	target->vz);
						  else
							ch->printlnf(  "&R&O%s     %.0f %.0f %.0f\n\r",
							  target->name,
							  target->vx,
							  target->vy,
							  target->vz);
					   }
                   }
                   ch->printlnf( "\n\r");
    	           for ( missile = ship->starsystem->first_missile; missile; missile = missile->next_in_starsystem )
                   {
                           ch->printlnf(  "&G&R%s    %d %d %d\n\r",
                           	missile->missiletype == CONCUSSION_MISSILE ? "A Concussion Missile" :
    			        ( missile->missiletype ==  PROTON_TORPEDO ? "A Torpedo" :
    			        ( missile->missiletype ==  HEAVY_ROCKET ? "A Heavy Rocket" : "A Heavy Bomb" ) ),
                           	missile->mx,
                           	missile->my,
                                missile->mz );
                   }

                   ch->printlnf(  "\n\r&WYour coordinates: %.0f %.0f %.0f\n\r" ,
                             ship->vx , ship->vy, ship->vz);


//        learn_from_success( ch, gsn_navigation );

}
*//*
void do_sautotrack( char_data *ch, char *argument )
{
   SHIP_DATA *ship;
   int chance;

   if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
   {
        ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
        return;
   }

        if ( ship->ship_class > SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r");
    	            return;
    	        }


        if ( ship->ship_class == SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RPlatforms don't have autotracking systems!\n\r");
    	            return;
    	        }
        if ( ship->ship_class >= SHIP_CRUISER )
    	        {
    	            ch->println("&RThis ship is too big for autotracking!\n\r");
    	            return;
    	        }

   if (  (ship = ship_from_pilotseat(ch->in_room->vnum))  == NULL )
   {
        ch->println("&RYou aren't in the pilots chair!\n\r");
        return;
   }
   if(!ship->target0)
	{
	   ch->println("&RYou don't have a target to track.\n\r");
	   return;
	}
                if ( autofly(ship)  )
    	        {
    	            ch->println("&RYou'll have to turn off the ships autopilot first....\n\r");
    	            return;
    	        }

   //chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_shipsystems]) ;
   chance = ch->level;

        if ( number_percent( ) > chance )
        {
           ch->println("&RYou're not sure which switch to flip.\n\r");
//           learn_from_failure( ch, gsn_shipsystems );
    	   return;
        }

   act( "$n flips a switch on the control panel.", ch,
         NULL, argument , TO_ROOM );
   if (ship->autotrack)
   {
     ship->autotrack = false;
     //echo_to_cockpit( AT_YELLOW , ship, "Autotracking off.");
   }
   else
   {
      ship->autotrack = true;
      //echo_to_cockpit( AT_YELLOW , ship, "Autotracking on.");
   }

//   learn_from_success( ch, gsn_shipsystems );

}

void do_jumpvector( char_data *ch, char *argument )
{}
void do_reload( char_data *ch, char *argument )
{}
void do_closebay( char_data *ch, char *argument )
{}
void do_openbay( char_data *ch, char *argument )
{
	SHIP_DATA *ship;
	char buf[MSL];

 	if( (ship = ship_from_pilotseat(ch->in_room->vnum)) == NULL )
 	{
  	 ch->println("You must be at the pilot controls of a ship to manipulate the bay doors.\n\r");
  	 return;
 	}

	if(ship->bayopen == false)
	{
//	    echo_to_room(AT_WHITE, get_room_index(ship->pilotseat), "&G[&gShip Computer&G] &wOpening bay doors.");
	    sprintf( buf ,"The bay doors on %s slowly slide open." , ship->name  );
//	    echo_to_system( AT_YELLOW, ship, buf , NULL );
		if(ship->hanger1 != 0)
//	     echo_to_room(AT_WHITE, get_room_index(ship->hanger1), "&R&YThe bay doors slowly slide open.");
	    if(ship->hanger2 != 0)
//		 echo_to_room(AT_WHITE, get_room_index(ship->hanger2), "&R&YThe bay doors slowly slide open.");
		if(ship->hanger3 != 0)
//	     echo_to_room(AT_WHITE, get_room_index(ship->hanger3), "&R&YThe bay doors slowly slide open.");
		if(ship->hanger4 != 0)
//	     echo_to_room(AT_WHITE, get_room_index(ship->hanger4), "&R&YThe bay doors slowly slide open.");

		ship->bayopen = true;
		return;
	}
	else
	{
//	    echo_to_room(AT_WHITE, get_room_index(ship->pilotseat), "&G[&gShip Computer&G] &wClosing bay doors.");
	    sprintf( buf ,"The bay doors on %s slowly slide closed." , ship->name  );
//	    echo_to_system( AT_YELLOW, ship, buf , NULL );
		if(ship->hanger1 != 0)
//	     echo_to_room(AT_WHITE, get_room_index(ship->hanger1), "&R&YThe bay doors slowly slide closed.");
	    if(ship->hanger2 != 0)
//		 echo_to_room(AT_WHITE, get_room_index(ship->hanger2), "&R&YThe bay doors slowly slide closed.");
		if(ship->hanger3 != 0)
//	     echo_to_room(AT_WHITE, get_room_index(ship->hanger3), "&R&YThe bay doors slowly slide closed.");
		if(ship->hanger4 != 0)
//	     echo_to_room(AT_WHITE, get_room_index(ship->hanger4), "&R&YThe bay doors slowly slide closed.");

		ship->bayopen = false;
		return;
	}
}

void do_tractorbeam( char_data *ch, char *argument )
/* Added 'stop' capability for tractor beams. --Trillen of SW:FotE
 * Idea from Mikor of SW:RotR
 *//*
{

    char arg1[MIL];
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
	char buf[MSL];

    argument = one_argument(argument, arg1);

        if ( !arg1 || arg1[0] == '\0')
        {

                ch->println( "Usage: tractor pull/stop/none <ship>\n\r" );
                return;
        }

	if(str_cmp(arg1, "pull") && str_cmp(arg1, "stop") && str_cmp(arg1, "none"))
       	{
   		ch->println("Usage: tractor pull/stop/none <ship>\n\r");
   	        return;
    	}

	if ( (ship = ship_from_cockpit(ch->in_room->vnum)) == NULL )
	{
		ch->println("&RYou must be in the cockpit of a ship to do that!\n\r");
		return;
	}

	if ( !check_pilot( ch , ship ) )
	{
		ch->println("This isn't your ship!\n\r" );
		return;
	}

	if ( ship->tractorbeam == 0 )
	{
		ch->println("You might want to install a tractorbeam!\n\r" );
		return;
	}

        if(!str_cmp(arg1, "none"))
        {
                ch->println("&RShip tractoring cleared.\n\r");
                if(ship->tractoring != NULL)
                {
                   ship->tractoring->tractored_by = NULL;
                   ship->tractoring = NULL;
                }
                return;
        }


        if ( !argument || argument[0] == '\0')
        {

                ch->println( "Usage: tractor pull/stop/none <ship>\n\r" );
                return;
        }

        if (!str_cmp(arg1, "pull"))
        {

	  if ( ship->hanger1 == 0 && ship->hanger2 == 0)
	  {
		ch->println("No hanger available.\n\r");
		return;
	  }

    	  if ( !ship->bayopen )
	  {
		ch->println("Your hanger is closed.\n\r");
		return;
	  }

	  if ( (ship = ship_from_pilotseat(ch->in_room->vnum)) == NULL )
	  {
		ch->println("&RYou need to be in the pilot seat!\n\r");
		return;
	  }

	  if (ship->shipstate == SHIP_DISABLED)
	  {
		ch->println("&RThe ships drive is disabled. No power available.\n\r");
		return;
	  }

   	  if (ship->shipstate == SHIP_DOCKED)
	  {
		ch->println("&RYour ship is docked!\n\r");
		return;
	  }

  	  if (ship->shipstate == SHIP_HYPERSPACE)
	  {

		ch->println("&RYou can only do that in realspace!\n\r");
		return;
	  }

	if ( argument[0] == '\0' )
	{
		ch->println("&RCapture what? (use none to clear)\n\r");
		return;
	}

	  if (ship->shipstate != SHIP_READY)
	  {
		ch->println("&RPlease wait until the ship has finished its current maneuver.\n\r");
		return;
  	  }
	target = get_ship_here( argument , ship->starsystem );

	if ( target == NULL )
	{
		ch->println("&RI don't see that here. (use 'none' to clear)\n\r");
		return;
	}

	if ( target == ship )
	{
		ch->println("&RTractor your own ship?\n\r");
		return;
	}

	if ( target->shipstate == SHIP_LAND )
	{
		ch->println("&RThat ship is already in a landing sequence.\n\r");
		return;
	}

	if (  (target->vx > ship->vx + 200) || (target->vx < ship->vx - 200) ||
	   (target->vy > ship->vy + 200) || (target->vy < ship->vy - 200) ||
	   (target->vz > ship->vz + 200) || (target->vz < ship->vz - 200) )
	{
		ch->println("&RThat ship is too far away! You'll have to fly a little closer.\n\r");
		return;
	}

	if (ship->ship_class <= target->ship_class)
	{
		ch->println("&RThat ship is too big for your hanger.\n\r");
		return;
	}

	if ( target->ship_class >= SHIP_CRUISER)
	{
		ch->println("&RYou can't capture capital ships.\n\r");
		return;
	}


	if ( ship->energy < (25 + 25*target->ship_class) )
	{
		ch->println("&RThere's not enough fuel!\n\r");
		return;
	}

//	chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_tractorbeams]);
    chance = ch->level;

	if ( number_percent( ) < chance )
	{
		ch->println( "&GCapture sequence initiated.\n\r");
		act( "$n begins the capture sequence.", ch,
			NULL, argument , TO_ROOM );
// 		echo_to_ship( AT_YELLOW , target , "The ship shudders as a tractorbeam locks on." );
		sprintf( buf , "You are being captured by %s." , ship->name);
		//echo_to_cockpit( AT_BLOOD , target , buf );

		if ( autofly(target) && !target->target0)
			target->target0 = ship;

		target->dest = str_dup(ship->name);
		target->shipstate = SHIP_LAND;
		target->currspeed = 0;
		target->tractored_by = ship;
		ship->tractoring = target;
//		learn_from_success( ch, gsn_tractorbeams );
		return;

	}
	ch->println("You fail to work the controls properly.\n\r");
//   	echo_to_ship( AT_YELLOW , target , "The ship shudders and then stops as a tractor beam attempts to lock on." );
	sprintf( buf , "The %s attempted to capture your ship!" , ship->name);
	//echo_to_cockpit( AT_BLOOD , target , buf );
	if ( autofly(target) && !target->target0)
		target->target0 = ship;


//	learn_from_failure( ch, gsn_tractorbeams );
   	return;
}
          /* End of tractor pull */
/*
        if(!str_cmp(arg1, "stop"))
        {
	if ( argument[0] == '\0' )
	{
		ch->println("&RCapture what? (use none to clear)\n\r");
		return;
	}

	target = get_ship_here( argument , ship->starsystem );

	if ( target == NULL )
	{
		ch->println("&RI don't see that here. (use 'none' to clear)\n\r");
		return;
	}

	if ( target == ship )
	{
		ch->println("&RTractor your own ship?\n\r");
		return;
	}

	if ( target->shipstate == SHIP_LAND )
	{
		ch->println("&RThat ship is already in a landing sequence.\n\r");
		return;
	}

	if (  (target->vx > ship->vx + 200) || (target->vx < ship->vx - 200) ||
	   (target->vy > ship->vy + 200) || (target->vy < ship->vy - 200) ||
	   (target->vz > ship->vz + 200) || (target->vz < ship->vz - 200) )
	{
		ch->println("&RThat ship is too far away! You'll have to fly a little closer.\n\r");
		return;
	}

	if (ship->ship_class <= target->ship_class)
	{
		ch->println("&RThat ship is too powerful for you to hold.\n\r");
		return;
	}

	if ( target->ship_class >= SHIP_CRUISER)
	{
		ch->println("&RYou can't hold capital ships.\n\r");
		return;
	}


	if ( ship->energy < (25 + 25*target->ship_class) )
	{
		ch->println("&RThere's not enough fuel!\n\r");
		return;
	}

	//chance = IS_NPC(ch) ? ch->top_level : (int)  (ch->pcdata->learned[gsn_tractorbeams]);
    chance = ch->level;

	if ( number_percent( ) < chance )
	{
		ch->println( "&GHolding sequence initiated.\n\r");
		act( "$n begins the tractor beam sequence.", ch,
			NULL, argument , TO_ROOM );
//    		echo_to_ship( AT_YELLOW , target , "The ship shudders as a tractorbeam locks on." );
		sprintf( buf , "You are being held in place by %s." , ship->name);
		//echo_to_cockpit( AT_BLOOD , target , buf );

	if ( autofly(target) && !target->target0)
		target->target0 = ship;
		target->currspeed = 0;
		target->tractored_by = ship;
		ship->tractoring = target;
//		learn_from_success( ch, gsn_tractorbeams );
		return;

	}
        }
	ch->println("You fail to work the controls properly.\n\r");
//   	echo_to_ship( AT_YELLOW , target , "The ship shudders and then stops as a tractorbeam attempts to lock on." );
	sprintf( buf , "The %s attempted to capture your ship!" , ship->name);
	//echo_to_cockpit( AT_BLOOD , target , buf );
	if ( autofly(target) && !target->target0)
		target->target0 = ship;


//	learn_from_failure( ch, gsn_tractorbeams );
   	return;
}


/*
void do_pluogus( CHAR_DATA *ch, char *argument )
{
     bool ch_comlink = false;
     OBJ_DATA *obj;
     int next_planet, itt;

     for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
     {
         if (obj->pIndexData->item_type == ITEM_COMLINK)
         ch_comlink = true;
     }

     if ( !ch_comlink )
     {
        ch->println( "You need a comlink to do that!\n\r", ch);
        return;
     }

     ch->println( "Serin Sol Schedule Information:\n\r", ch );
     // current port
     if ( bus_pos < 7 && bus_pos > 1 )
        ch_printf( ch, "The shuttle is currently docked at %s.\n\r", bus_stop[bus_planet] );
     // destinations
     next_planet = bus_planet;
     ch->println( "Next stops: ", ch);
     if ( bus_pos <= 1 )
        ch_printf( ch, "%s  ", bus_stop[next_planet] );
     for ( itt = 0 ; itt < 3 ; itt++ )
     {
         next_planet++;
         if ( next_planet >= MAX_BUS_STOP )
            next_planet = 0;
         ch_printf( ch, "%s  ", bus_stop[next_planet] );
     }
     ch_printf( ch, "\n\r\n\r" );

     ch->println( "Serin Monir Schedule Information:\n\r", ch );
     // current port
     if ( bus_pos < 7 && bus_pos > 1 )
        ch_printf( ch, "The shuttle is currently docked at %s.\n\r", bus_stop[bus2_planet] );
     // destinations
     next_planet = bus2_planet;
     ch->println( "Next stops: ", ch);
     if ( bus_pos <= 1 )
        ch_printf( ch, "%s  ", bus_stop[next_planet] );
     for ( itt = 0 ; itt < 3 ; itt++ )
     {
         next_planet++;
         if ( next_planet >= MAX_BUS_STOP )
            next_planet = 0;
         ch_printf( ch, "%s  ", bus_stop[next_planet] );
     }
     ch_printf( ch, "\n\r\n\r" );

     ch->println( "Serin Faun Schedule Information:\n\r", ch );
     // current port
     if ( bus_pos < 7 && bus_pos > 1 )
        ch_printf( ch, "The shuttle is currently docked at %s.\n\r", bus_stop[bus3_planet] );
     // destinations
     next_planet = bus3_planet;
     ch->println( "Next stops: ", ch);
     if ( bus_pos <= 1 )
        ch_printf( ch, "%s  ", bus_stop[next_planet] );
     for ( itt = 0 ; itt < 3 ; itt++ )
     {
         next_planet++;
         if ( next_planet >= MAX_BUS_STOP )
            next_planet = 0;
         ch_printf( ch, "%s  ", bus_stop[next_planet] );
     }
     ch_printf( ch, "\n\r\n\r" );

     ch->println( "Serin Taw Schedule Information:\n\r", ch );
     // current port
     if ( bus_pos < 7 && bus_pos > 1 )
        ch_printf( ch, "The shuttle is currently docked at %s.\n\r", bus_stop[bus4_planet] );
     // destinations
     next_planet = bus4_planet;
     ch->println( "Next stops: ", ch);
     if ( bus_pos <= 1 )
        ch_printf( ch, "%s  ", bus_stop[next_planet] );
     for ( itt = 0 ; itt < 3 ; itt++ )
     {
         next_planet++;
         if ( next_planet >= MAX_BUS_STOP )
            next_planet = 0;
         ch_printf( ch, "%s  ", bus_stop[next_planet] );
     }
     ch_printf( ch, "\n\r" );

}
*//*
void do_fly( char_data *ch, char *argument )
{}

/**************************************************************************/
int get_direction( char *arg )
{
	if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) return 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) return 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) return 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) return 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) return 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) return 5;
    else if ( !str_cmp( arg, "ne" ) || !str_cmp( arg, "northeast" ) ) return 6;
    else if ( !str_cmp( arg, "se" ) || !str_cmp( arg, "southeast" ) ) return 7;
    else if ( !str_cmp( arg, "sw" ) || !str_cmp( arg, "southwest" ) ) return 8;
    else if ( !str_cmp( arg, "nw" ) || !str_cmp( arg, "northwest" ) ) return 9;
    return -1;
}
/***************************************************************************
/*
 * Function to get the equivelant exit of DIR 0-MAXDIR out of linked list.
 * Made to allow old-style diku-merc exit functions to work.	-Thoric
 *
EXIT_DATA *get_exit( ROOM_INDEX_DATA *room, sh_int dir )
{
    EXIT_DATA *xit;

    if ( !room )
    {
	bug( "Get_exit: NULL room" );
	return NULL;
    }

    for (xit = room->exit; xit; xit = xit->next )
       if ( xit->vdir == dir )
         return xit;
    return NULL;
}

/***************************************************************************
void do_sdrive( char_data *ch, char *argument )
{
    int dir;
    SHIP_DATA *ship;

        if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou must be in the drivers seat of a land vehicle to do that!\n\r");
            return;
        }

        if ( ship->ship_class != LAND_VEHICLE )
    	{
    	      ch->println("&RThis isn't a land vehicle!\n\r");
    	      return;
    	}


        if (ship->shipstate == SHIP_DISABLED)
    	{
    	     ch->println("&RThe drive is disabled.\n\r");
    	     return;
    	}

        if ( ship->energy <1 )
        {
              ch->println("&RTheres not enough fuel!\n\r");
              return;
        }

//        if ( ( dir = get_door( argument ) ) == -1 )
        if ( ( dir = get_direction( argument ) ) == -1 )
        {
             ch->println( "Usage: drive <direction>\n\r");
             return;
        }

//        drive_ship( ch, ship, get_exit(get_room_index(ship->location), dir), 0 );

}

/*
ch_ret drive_ship( CHAR_DATA *ch, SHIP_DATA *ship, EXIT_DATA  *pexit , int fall )
{
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    ROOM_INDEX_DATA *from_room;
    ROOM_INDEX_DATA *original;
    char buf[MAX_STRING_LENGTH];
    char *txt;
    char *dtxt;
    ch_ret retcode;
    sh_int door, distance;
    bool drunk = false;
    CHAR_DATA * rch;
    CHAR_DATA * next_rch;


    if ( !IS_NPC( ch ) )
      if ( IS_DRUNK( ch, 2 ) && ( ch->position != POS_SHOVE )
	&& ( ch->position != POS_DRAG ) )
	drunk = true;

    if ( drunk && !fall )
    {
      door = number_door();
      pexit = get_exit( get_room_index(ship->location), door );
    }

#ifdef DEBUG
    if ( pexit )
    {
	sprintf( buf, "drive_ship: %s to door %d", ch->name, pexit->vdir );
	log_string( buf );
    }
#endif

    retcode = rNONE;
    txt = NULL;

    in_room = get_room_index(ship->location);
    from_room = in_room;
    if ( !pexit || (to_room = pexit->to_room) == NULL )
    {
	if ( drunk )
	  ch->println( "You drive into a wall in your drunken state.\n\r", ch );
	 else
	  ch->println( "Alas, you cannot go that way.\n\r", ch );
	return rNONE;
    }

    door = pexit->vdir;
    distance = pexit->distance;

    if ( IS_SET( pexit->exit_info, EX_WINDOW )
    &&  !IS_SET( pexit->exit_info, EX_ISDOOR ) )
    {
	ch->println( "Alas, you cannot go that way.\n\r", ch );
	return rNONE;
    }

    if (  IS_SET(pexit->exit_info, EX_PORTAL)
       && IS_NPC(ch) )
    {
        act( AT_PLAIN, "Mobs can't use portals.", ch, NULL, NULL, TO_CHAR );
	return rNONE;
    }

    if ( IS_SET(pexit->exit_info, EX_NOMOB)
	&& IS_NPC(ch) )
    {
	act( AT_PLAIN, "Mobs can't enter there.", ch, NULL, NULL, TO_CHAR );
	return rNONE;
    }

    if ( IS_SET(pexit->exit_info, EX_CLOSED)
    && (IS_SET(pexit->exit_info, EX_NOPASSDOOR)) )
    {
	if ( !IS_SET( pexit->exit_info, EX_SECRET )
	&&   !IS_SET( pexit->exit_info, EX_DIG ) )
	{
	  if ( drunk )
	  {
	    act( AT_PLAIN, "$n drives into the $d in $s drunken state.", ch,
		NULL, pexit->keyword, TO_ROOM );
	    act( AT_PLAIN, "You drive into the $d in your drunken state.", ch,
		NULL, pexit->keyword, TO_CHAR );
	  }
	 else
	  act( AT_PLAIN, "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	}
       else
	{
	  if ( drunk )
	    ch->println( "You hit a wall in your drunken state.\n\r", ch );
	   else
	    ch->println( "Alas, you cannot go that way.\n\r", ch );
	}

	return rNONE;
    }

/*
    if ( distance > 1 )
	if ( (to_room=generate_exit(in_room, &pexit)) == NULL )
	    ch->println( "Alas, you cannot go that way.\n\r", ch );
*//*
    if ( room_is_private( ch, to_room ) )
    {
	ch->println( "That room is private right now.\n\r", ch );
	return rNONE;
    }

    if ( !IS_IMMORTAL(ch)
    &&  !IS_NPC(ch)
    &&  ch->in_room->area != to_room->area )
    {
	if ( ch->top_level < to_room->area->low_hard_range )
	{
	    //set_char_color( AT_TELL, ch );
	    switch( to_room->area->low_hard_range - ch->top_level )
	    {
		case 1:
		  ch->println( "A voice in your mind says, 'You are nearly ready to go that way...'", ch );
		  break;
		case 2:
		  ch->println( "A voice in your mind says, 'Soon you shall be ready to travel down this path... soon.'", ch );
		  break;
		case 3:
		  ch->println( "A voice in your mind says, 'You are not ready to go down that path... yet.'.\n\r", ch);
		  break;
		default:
		  ch->println( "A voice in your mind says, 'You are not ready to go down that path.'.\n\r", ch);
	    }
	    return rNONE;
	}
	else
	if ( ch->top_level > to_room->area->hi_hard_range )
	{
	    //set_char_color( AT_TELL, ch );
	    ch->println( "A voice in your mind says, 'There is nothing more for you down that path.'", ch );
	    return rNONE;
	}
    }

    if ( !fall )
    {
        if ( IS_SET( to_room->room_flags, ROOM_INDOORS )
        || IS_SET( to_room->room_flags, ROOM_SPACECRAFT )
        || to_room->sector_type == SECT_INSIDE )
	{
		ch->println( "You can't drive indoors!\n\r", ch );
		return rNONE;
	}

        if ( IS_SET( to_room->room_flags, ROOM_NO_DRIVING ) )
	{
		ch->println( "You can't take a vehicle through there!\n\r", ch );
		return rNONE;
	}

	if ( in_room->sector_type == SECT_AIR
	||   to_room->sector_type == SECT_AIR
	||   IS_SET( pexit->exit_info, EX_FLY ) )
	{

		ch->println( "You can't go there.\n\r", ch );
		return rNONE;

	}

	if ( in_room->sector_type == SECT_WATER_NOSWIM
	||   to_room->sector_type == SECT_WATER_NOSWIM
	||   to_room->sector_type == SECT_WATER_SWIM
	||   to_room->sector_type == SECT_UNDERWATER
	||   to_room->sector_type == SECT_OCEANFLOOR )
	{

		ch->println( "You'd need a boat to go there.\n\r", ch );
		return rNONE;


	}

	if ( IS_SET( pexit->exit_info, EX_CLIMB ) )
	{

		ch->println( "You need to fly or climb to get up there.\n\r", ch );
		return rNONE;

	}

    }

    if ( to_room->tunnel > 0 )
    {
	CHAR_DATA *ctmp;
	int count = 0;

	for ( ctmp = to_room->first_person; ctmp; ctmp = ctmp->next_in_room )
	  if ( ++count >= to_room->tunnel )
	  {
		  ch->println( "There is no room for you in there.\n\r", ch );
		return rNONE;
	  }
    }

      if ( fall )
        txt = "falls";
      else
      if ( !txt )
      {
	  if ( ship->ship_class == LAND_VEHICLE  )
	  {
	      txt = "drive";
	  }
      }
      sprintf( buf, "$n %ss the vehicle $T.", txt );
      act( AT_ACTION, buf, ch, NULL, dir_name[door], TO_ROOM );
      sprintf( buf, "You %s the vehicle $T.", txt );
      act( AT_ACTION, buf, ch, NULL, dir_name[door], TO_CHAR );
      sprintf( buf, "%s %ss %s.", ship->name, txt, dir_name[door] );
      echo_to_room( AT_ACTION , get_room_index(ship->location) , buf );

      extract_ship( ship );
      ship_to_room(ship, to_room->vnum );

      ship->location = to_room->vnum;
      ship->lastdoc = ship->location;

      if ( fall )
        txt = "falls";
	  if ( ship->ship_class == LAND_VEHICLE )
	  {
	      txt = "drives in";
	  }

      switch( door )
      {
      default: dtxt = "somewhere";	break;
      case 0:  dtxt = "the south";	break;
      case 1:  dtxt = "the west";	break;
      case 2:  dtxt = "the north";	break;
      case 3:  dtxt = "the east";	break;
      case 4:  dtxt = "below";		break;
      case 5:  dtxt = "above";		break;
      case 6:  dtxt = "the south-west";	break;
      case 7:  dtxt = "the south-east";	break;
      case 8:  dtxt = "the north-west";	break;
      case 9:  dtxt = "the north-east";	break;
      }

    sprintf( buf, "%s %s from %s.", ship->name, txt, dtxt );
    echo_to_room( AT_ACTION , get_room_index(ship->location) , buf );

    for ( rch = ch->in_room->last_person ; rch ; rch = next_rch )
    {
        next_rch = rch->prev_in_room;
        original = rch->in_room;
        char_from_room( rch );
        char_to_room( rch, to_room );
        do_look( rch, "auto" );
        char_from_room( rch );
        char_to_room( rch, original );
    }

/*
    if (  CHECK FOR FALLING HERE
    &&   fall > 0 )
    {
	if (!IS_AFFECTED( ch, AFF_FLOATING )
	|| ( ch->mount && !IS_AFFECTED( ch->mount, AFF_FLOATING ) ) )
	{
	  //set_char_color( AT_HURT, ch );
	  ch->println( "OUCH! You hit the ground!\n\r", ch );
	  WAIT_STATE( ch, 20 );
	  retcode = damage( ch, ch, 50 * fall, TYPE_UNDEFINED );
	}
	else
	{
	  //set_char_color( AT_MAGIC, ch );
	  ch->println( "You lightly float down to the ground.\n\r", ch );
	}
    }

*//*
    return retcode;

}*/
/*
void do_bomb( CHAR_DATA *ch, char *argument )
{
  int vnum,i,hibomb,lowbomb;
  SHIP_DATA *ship;
  OBJ_INDEX_DATA  * pobj;
  OBJ_DATA *obj;
  ROOM_INDEX_DATA *room;
  PLANET_DATA *planet;
  int bombs;
  bool foundplanet=false;
  char buf[MAX_STRING_LENGTH];
  int chance;

  if ( (ship = ship_from_cockpit(ch->in_room->vnum)) == NULL ){
  	ch->println("You must be in the cockpit of the bomber to do this.\n\r", ch);
  	return;
  }

  if(argument[0] == '\0' || isalpha(argument[0]))
  {
	ch->println("&RDrop how many bombs?\n\r", ch);
	return;
  }
  bombs = atoi(argument);


  if( ship->ship_class != SHIP_BOMBER )
  {
  	ch->println("&RYou must be in the cockpit of the bomber to do this.\n\r", ch);
  	return;
  }

  if( ship->maxbombs <= 0)
  {
	ch->println("&RThis ship doesn't have the ability to carry bombs!\n\r", ch);
	return;
  }

  if( ship->bombs == 0)
  {
	ch->println("&RYou have no bombs left!\n\r", ch);
	return;
  }

  if( ship->bombs < bombs)
  {
	ch->println("&RYou don't have that many bombs left!\n\r", ch);
	return;
  }

  if (ship->shipstate == SHIP_DISABLED)
  {
      ch->println("&RYou can't bomb while your ship is disabled.\n\r", ch);
      return;
  }
  if (ship->shipstate == SHIP_DOCKED)
  {
      ch->println("&RYou can't do that while the ship is docked!\n\r",ch);
      return;
  }

  if (ship->shipstate == SHIP_HYPERSPACE)
  {
    ch->println("&RYou can only do that in realspace!\n\r",ch);
    return;
  }

 if (ship->shipstate != SHIP_READY)
 {
    ch->println("&RPlease wait until the ship has finished its current maneuver.\n\r", ch);
    return;
  }

  if ( ship->starsystem == NULL )
  {
    ch->println("&RThere's no planets around here!",ch);
    return;
  }

  chance = IS_NPC(ch) ? ch->top_level : (int)(ch->pcdata->learned[gsn_bomb]) ;
  if ( number_percent( ) > chance )
  {
    ch->println("&RYou can't seem to maneuver into the right position to unleash your payload.\n\r", ch);
    return;
  }

   for ( planet = ship->starsystem->first_planet ; planet ; planet = planet->next_in_system )
    if((ship->vx > planet->x - 20) && (ship->vx < planet->x + 20) && (ship->vy > planet->y - 20) && (ship->vy < planet->y + 20) && (ship->vz > planet->z - 20) && (ship->vz < planet->z + 20))
    {
      foundplanet=true;
      break;
    }

   if(!foundplanet)
   {
     ch->println("&RYou must be directly orbitting a planet in order to bomb it.\n\r", ch);
     return;
   }

  if(!planet->first_area)
  {
    ch->println("&RThat planet has no place to bomb!\n\r", ch);
    return;
  }

  if(!ship->lowbombstr || ship->lowbombstr == 0)
    lowbomb = 75;
  else
    lowbomb = ship->lowbombstr;

  if(!ship->hibombstr || ship->hibombstr == 0)
    hibomb = 150;
  else
    hibomb = ship->hibombstr;

  for (i=1;i<=bombs;i++)
  {
    vnum = number_range(planet->first_area->low_r_vnum, planet->first_area->hi_r_vnum);
    if ((room = get_room_index(vnum)) == NULL || !str_cmp(room->name, "Floating in a void"))
      --i;
    else if ( ( pobj = get_obj_index( OBJ_VNUM_SHIPBOMB ) ) != NULL )
    {
      if( !IS_SET(room->room_flags,ROOM_INDOORS))
      {
	sprintf(buf, "Bomb dropped in room %s", room->name);
	log_string(buf);
	obj = create_object( pobj, 30 );
	obj->value[0] = lowbomb;
	obj->value[1] = hibomb;
	obj->armed_by = ch->name;
	obj_to_room( obj, room );
	echo_to_room( AT_WHITE, room, "A bomb falls from the sky...");
	explode(obj);
      }
      else
	--i;
    }
  }
  sprintf(buf, "&GYou unleash your payload of %d bombs onto the lands of %s, and watch as the bombs make small explosions viewable from orbit.\n\r", bombs, planet->name);
  ship->bombs -= bombs;
  //echo_to_cockpit(AT_GREEN, ship, buf);
  learn_from_success(ch, gsn_bomb);
}

void do_chaff( CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;


        if (  (ship = ship_from_cockpit(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
            return;
        }

        if ( ship->ship_class > SHIP_SPACE_STATION )
    	        {
    	            ch->println("&RThis isn't a spacecraft!\n\r",ch);
    	            return;
    	        }


        if (  (ship = ship_from_coseat(ch->in_room->vnum))  == NULL )
        {
            ch->println("&RThe controls are at the copilots seat!\n\r",ch);
            return;
        }

                if ( autofly(ship) )
    	        {
    	            ch->println("&RYou'll have to turn the autopilot off first...\n\r",ch);
    	            return;
    	        }

                if (ship->shipstate == SHIP_HYPERSPACE)
                {
                  ch->println("&RYou can only do that in realspace!\n\r",ch);
                  return;
                }
    	        if (ship->shipstate == SHIP_DOCKED)
    	        {
    	            ch->println("&RYou can't do that until after you've launched!\n\r",ch);
    	            return;
    	        }
                if (ship->chaff <= 0 )
    	        {
    	            ch->println("&RYou don't have any chaff to release!\n\r",ch);
    	            return;
    	        }
                chance = IS_NPC(ch) ? ch->top_level
                 : (int)  (ch->pcdata->learned[gsn_weaponsystems]) ;
        if ( number_percent( ) > chance )
        {
            ch->println("&RYou can't figure out which switch it is.\n\r",ch);
            learn_from_failure( ch, gsn_weaponsystems );
    	   return;
        }

    ship->chaff--;

    ship->chaff_released++;

    ch->println( "You flip the chaff release switch.\n\r", ch);
    act( AT_PLAIN, "$n flips a switch on the control pannel", ch,
         NULL, argument , TO_ROOM );
    //echo_to_cockpit( AT_YELLOW , ship , "A burst of chaff is released from the ship.");

    learn_from_success( ch, gsn_weaponsystems );

}*/

bool autofly( SHIP_DATA *ship )
{

     if (!ship)
        return false;

     if ( ship->type == MOB_SHIP )
        return true;

     if ( ship->autopilot )
        return true;

     return false;

}




/*
void do_salvage(CHAR_DATA *ch, char *argument)
{
  SHIP_DATA *ship;
  int value;
  char buf[MAX_STRING_LENGTH];
    ship = ship_in_room( ch->in_room, argument );
    if ( !ship )
    {
	ch->println( "No such ship.\n\r", ch );
	return;
    }
    if (str_cmp(ship->owner, ch->name)){
    	ch->println("You do not own this ship!\n\r", ch);
    	return;
    }
   if (!IS_NPC(ch) && ch->top_level < 11)
    {
      ch->println( "Due to cheating, players under level 11 are not allowed to sell ships.\n\r", ch);
      return;
    }
    //sprintf(buf,"%s: removeship %s",ch->name,argument);
    //log_string(buf);
    if (!IS_SET(ch->in_room->room_flags2, ROOM_SHIPYARD) && !IS_SET(ch->in_room->room_flags2, ROOM_BLACKMARKET)){
    	ch->println("You must be in a shipyard.\n\r", ch);
    	return;
    }
   if(!IS_SET(ch->in_room->room_flags2, ROOM_BLACKMARKET))
   {
    value = (get_ship_value(ship) / 2);
    act(AT_WHITE, "You sell your ship to the shipyard for spare parts.", ch, NULL, NULL, TO_CHAR);
    act(AT_WHITE, "$n sells $s ship to the shipyard for spare parts.", ch, NULL, NULL, TO_ROOM);
    sprintf(buf, "You get %d credits for the spare parts.\n\r", value);
    ch->println(buf, ch);
    ch->gold+=value;
    extract_ship(ship);
    ship_to_room(ship, 46);
    destroy_ship(ship, ch, "salvaged");
    ch->println("Ship removed.\r\n",ch);
   }
   else
   {
    if(!ship->clanowner || ship->clanowner == NULL || ship->clanowner[0] == '\0')
    {
    	ch->println("&R&CA smuggler says:&R&W I don't want that! Gimme an organization-owned ship.\n\r", ch);
    	return;
    }
    value = (get_ship_value(ship) / 2);
    ch->println("You sell your ship to the Black Market.\n\r", ch);
    act(AT_WHITE, "$n sells his ship to the Black Market.\n\r", ch, NULL, NULL, TO_ROOM);
    sprintf(buf, "You get %d credits for selling your ship.\n\r", value);
    ch->println(buf, ch);
    add_market_ship(ship);
    ch->gold+=value;
	extract_ship(ship);
	ship_to_room(ship, 46);
    destroy_ship(ship, ch, "sold to black market");
   }
  return;
}

void do_clansalvage(CHAR_DATA *ch, char *argument)
{
  SHIP_DATA *ship;
  CLAN_DATA * clan;
  int value;
  char buf[MAX_STRING_LENGTH];

    ship = ship_in_room( ch->in_room, argument );
    if ( !ship )
    {
	ch->println( "No such ship.\n\r", ch );
	return;
    }
   if ( !ch->pcdata->clan )
   {
   	ch->println( "&RYou aren't a member of any organizations!\n\r" ,ch );
   	return;
   }

   clan = ch->pcdata->clan;

   if ( ( ch->pcdata->bestowments
    &&    is_name("clanbuyship", ch->pcdata->bestowments))
    ||   nifty_is_name( ch->name, clan->leader  ))
	;
   else
   {
   	ch->println( "&RYour organization hasn't seen fit to bestow you with ship handling.\n\r" ,ch );
   	return;
   }

    if (str_cmp(ch->pcdata->clan->name,ship->owner) )
    {
    	ch->println("Your clan does not own this ship!\n\r", ch);
    	return;
    }

    if (!IS_SET(ch->in_room->room_flags2, ROOM_SHIPYARD))
    {
    	ch->println("You must be in a shipyard.\n\r", ch);
    	return;
    }
    value = (get_ship_value(ship) / 2);
    act(AT_WHITE, "You sell your clans ship to the shipyard for spare parts.", ch, NULL, NULL, TO_CHAR);
    act(AT_WHITE, "$n sells $s ship to the shipyard for spare parts.", ch, NULL, NULL, TO_ROOM);
    sprintf(buf, "Your clan is reimbursed %d credits for the parts.\n\r", value);
    ch->println(buf, ch);
    clan->funds +=value;
    destroy_ship(ship, ch, "salvaged");
  return;
}

void do_endsimulator( CHAR_DATA *ch, char *argument )
{
  SHIP_DATA *ship;
  char buf[MAX_INPUT_LENGTH];

  if ( (ship = ship_from_cockpit(ch->in_room->vnum)) == NULL )
  {
    ch->println("You must be in the cockpit of a simulator.\n\r", ch);
    return;
  }
  if (!IS_SET(ship->flags, SHIP_SIMULATOR) )
  {
    ch->println("You must be in the cockpit of a simulator.\n\r", ch);
    return;
  }
            	 sprintf(buf, "%s suddenly disappears from your radar.\n\r", ship->name);
          	 echo_to_system( AT_YELLOW, ship , buf , NULL );
	  	     ship->shipstate = SHIP_READY;
	         extract_ship( ship );
        	 ship_to_room( ship , ship->sim_vnum );
			 if(!ship->in_room) ship_to_room(ship, 45);
			 ship->shipyard = ship->sim_vnum;
          	 ship->location = ship->shipyard;
          	 ship->lastdoc = ship->shipyard;
          	 ship->shipstate = SHIP_DOCKED;
          	 if (ship->starsystem)
        	   ship_from_starsystem( ship, ship->starsystem );
                 save_ship(ship);
          	 ch->println("You press a button and the simulation ends.\n\r", ch);

}

void do_freeship( CHAR_DATA *ch, char *argument )
{
    SHIP_DATA *ship;
    char buf[MAX_INPUT_LENGTH];
    ch->println ("&z+-----------------------------------------------------------------------------+\n\r", ch);
    ch->println ("&z| &cShip                      &z| &YRent       &z| &RLocation &z                          |\n\r", ch);
    ch->println ("&z+---------------------------|------------|------------------------------------+\n\r", ch);
    for ( ship = first_ship; ship; ship = ship->next )
    {
       if ( !str_cmp(ship->owner, "Public") )
       {
            if  ( ship->in_room && (ship->in_room->vnum == 45) )
            {
               sprintf (buf, "&z| %s%-25.25s &z| &Y%-10ld&z | &R%-34.33s &z|\n\r", ship->type == SHIP_REPUBLIC ? "&r": ship->type == SHIP_IMPERIAL ? "&g":  "&B", ship->name, get_ship_value(ship)/100, "Space");
               ch->println (buf, ch);
            }
            else if ( ship->in_room )
            {
               sprintf (buf, "&z| %s%-25.25s &z| &Y%-10ld&z | &R%-34.33s &z|\n\r", ship->type == SHIP_REPUBLIC ? "&r": ship->type == SHIP_IMPERIAL ? "&g":  "&B", ship->name, get_ship_value(ship)/100, ship->in_room->name);
               ch->println (buf, ch);
            }
            else
            {
                sprintf (buf, "&z| %s%-25.25s &z| &Y%-10ld&z | &RSpace                              &z|\n\r",  ship->type == SHIP_REPUBLIC ? "&r": ship->type == SHIP_IMPERIAL ? "&g": "&B", ship->name, get_ship_value(ship)/100);
                ch->println (buf, ch);
            }
        }
    }
    ch->println ("&z+-----------------------------------------------------------------------------+\n\r", ch);
  return;
}

void do_giveship(CHAR_DATA *ch, char *argument)
{
 SHIP_DATA *ship;
 CHAR_DATA *victim;

 char logbuf[MAX_STRING_LENGTH];
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 if(arg1[0] == '\0' || arg2[0] == '\0')
 {
  ch->println("Incorrect syntax, see 'HELP GIVESHIP'.\n\r", ch);
  return;
 }

 ship = ship_in_room(ch->in_room, arg1);
 if(!ship)
 {
  ch->println("That ship isn't here.\n\r", ch);
  return;
 }
 if(str_cmp(ship->owner,ch->name))
 {
  ch->println("That isn't your ship.\n\r", ch);
  return;
 }

 if(( victim = get_char_room( ch, arg2 )) == NULL)
 {
  ch->println( "They aren't here.\n\r", ch );
  return;
 }
 if(IS_NPC(victim))
 {
  ch->println("Giving your ship to an NPC, eh? I don't think so.\n\r", ch);
  return;
 }
 if (!IS_NPC(ch) && ch->top_level < 11)
  {
    ch->println( "Due to cheating, players under level 11 are not allowed to move ships.\n\r", ch);
    return;
  }

STRFREE(ship->owner);
 ship->owner = STRALLOC(victim->name);
 ch->printlnf(  "&R&GYou give %s to %s.\n\r", ship->name, victim->name);
 ch_printf(victim, "&R&G%s gives %s to you.\n\r", ch->name, ship->name);
 sprintf(logbuf, "%s gave the ship %s to %s.\n\r", ch->name, ship->name, victim->name);
 log_string(logbuf);
 return;
}

void do_clangiveship(CHAR_DATA *ch, char *argument)
{
 SHIP_DATA *ship;
 CLAN_DATA *clan;

 char buf[MAX_STRING_LENGTH];
 char logbuf[MAX_STRING_LENGTH];
 char arg1[MAX_INPUT_LENGTH];

 argument = one_argument(argument, arg1);

 if(arg1[0] == '\0' || argument[0] == '\0')
 {
  ch->println("Give what ship to what clan?\n\r", ch);
  return;
 }

 ship = ship_in_room(ch->in_room, arg1);
 if(!ship)
 {
  ch->println("That ship isn't here.\n\r", ch);
  return;
 }
 if(str_cmp(ship->owner,ch->name))
 {
  ch->println("That isn't your ship.\n\r", ch);
  return;
 }

 if( (clan = get_clan(argument)) == NULL)
 {
  ch->println("No such clan. HELP CLANGIVESHIP for more info.\n\r", ch);
  return;
 }

 if (!IS_NPC(ch) && ch->top_level < 11)
  {
    ch->println( "Due to cheating, players under level 11 are not allowed to move credits.\n\r", ch);
    return;
  }

  STRFREE(ship->owner);
  ship->owner = STRALLOC(clan->name);
  ch->printlnf(  "&R&GYou give %s to %s.\n\r", ship->name, clan->name);
  sprintf(buf, "&R&G%s is now owned by %s.\n\r", ship->name, clan->name);
  act(AT_PLAIN, buf, ch, NULL, NULL, TO_ROOM);
  sprintf(logbuf, "%s gave the ship %s to %s.\n\r", ch->name, ship->name, clan->name);
  log_string(logbuf);

  if ( ship->ship_class <= SHIP_SPACE_STATION )
   clan->spacecraft++;
  else
   clan->vehicles++;

  return;
}

void do_request(CHAR_DATA *ch, char *argument)
{
 SHIP_DATA *ship;
 SHIP_DATA *target;
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];
 bool open;

 open = false;
 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

  	if(arg1[0] == '\0')
 	{
  	 ch->println("Syntax:  request <ship> <open/close>.\n\r", ch);
  	 return;
	}
 	if((ship = ship_from_pilotseat(ch->in_room->vnum)) == NULL)
 	{
  	 ch->println("You must be at the pilot controls of a ship to do that.\n\r",ch);
  	 return;
 	}
 	if((target = get_ship_here(arg1, ship->starsystem)) == NULL)
 	{
  	 ch->println("That ship doesn't seem to be here.\n\r", ch);
  	 return;
 	}
      if(!str_cmp(arg2, "open"))
	{
 	 if(target->bayopen == true)
 	 {
 	  ch->println("The bay doors are already open.\n\r", ch);
 	  return;
 	 }

	if(!str_cmp(ch->name, target->owner)) open = true;
 	if(ch->pcdata->clan)
 	{
  	 if(!str_cmp(ch->pcdata->clan->name, target->owner))
   	  open = true;
 	}
       if(!str_cmp (target->owner, ship->owner)) open = true;

 	sprintf(buf, "&GYou request clearance to land on %s.\n\r", target->name);
 	ch->println(buf, ch);
 	sprintf(buf, "&G%s requesting landing clearance.\n\r", ship->name);
 	//echo_to_cockpit(AT_BLOOD, target, buf);

 if(open == true)
 {
  sprintf(buf, "\n\r&R&G%s hails: &WConfirmed %s, you're clear to land.\n\r", target->name, ship->name);
  //echo_to_cockpit(AT_WHITE, ship, buf);
  sprintf(buf, "&YThe bay doors on %s slide open.\n\r", target->name);
  echo_to_system(AT_YELLOW, target, buf, NULL);
  //echo_to_cockpit(AT_WHITE, target, "&G[&gShip Computer&G]&w Opening bay doors.");

 		if(ship->hanger1 != 0)
	     echo_to_room(AT_WHITE, get_room_index(ship->hanger1), "&R&YThe bay doors slowly slide open.");
	    if(ship->hanger2 != 0)
		 echo_to_room(AT_WHITE, get_room_index(ship->hanger2), "&R&YThe bay doors slowly slide open.");
		if(ship->hanger3 != 0)
	     echo_to_room(AT_WHITE, get_room_index(ship->hanger3), "&R&YThe bay doors slowly slide open.");
		if(ship->hanger4 != 0)
	     echo_to_room(AT_WHITE, get_room_index(ship->hanger4), "&R&YThe bay doors slowly slide open.");

  target->bayopen = true;
  return;
 }
 else
 {
  sprintf(buf, "\n\r&R&G%s hails: &WNegative %s, resume previous flight pattern.\n\r", target->name, ship->name);
  //echo_to_cockpit(AT_WHITE, ship, buf);
  sprintf(buf, "&R%s is denied landing clearance.\n\r", ship->name);
  //echo_to_cockpit(AT_WHITE, target, buf);
  return;
 }
	}
	else if(!str_cmp(arg2, "close"))
    {
		if(target->bayopen == false)
		{
			ch->println("The bay doors are already closed.\n\r", ch);
			return;
		}

		sprintf(buf, "&R&G%s hails: &WAcknowledged, %s.", target->name, ship->name);
		//echo_to_cockpit(AT_WHITE, ship, buf);
		//echo_to_cockpit(AT_WHITE, target, "&G[&gShip Computer&G]&W Closing bay doors.");
		sprintf(buf, "The bay doors on %s slowly slide closed.", target->name);
		echo_to_system(AT_YELLOW, target, buf, NULL);
 		if(ship->hanger1 != 0)
	     echo_to_room(AT_WHITE, get_room_index(ship->hanger1), "&R&YThe bay doors slowly slide closed.");
	    if(ship->hanger2 != 0)
		 echo_to_room(AT_WHITE, get_room_index(ship->hanger2), "&R&YThe bay doors slowly slide closed.");
		if(ship->hanger3 != 0)
	     echo_to_room(AT_WHITE, get_room_index(ship->hanger3), "&R&YThe bay doors slowly slide closed.");
		if(ship->hanger4 != 0)
	     echo_to_room(AT_WHITE, get_room_index(ship->hanger4), "&R&YThe bay doors slowly slide closed.");
		target->bayopen = false;

		return;
	}
	else
	{
		ch->println("Syntax: request <ship> <open/close>.\n\r", ch);
		return;
	}
}

void do_makesimulator( CHAR_DATA * ch, char *argument )
{
   SHIP_DATA *ship;

   if( IS_NPC( ch ) || !IS_IMMORTAL( ch ) )
      return;

   if( !argument || ( argument[0] == '\0' ) )
   {
      ch->println( "&wUsage: &Ymakesimulator &c<&Wshipname&c>\n\r", ch );
      return;
   }

   ship = get_ship( argument );
   if( !ship )
   {
      ch->println( "No such ship.\n\r", ch );
      return;
   }

   if( IS_SET( ship->flags, SHIP_SIMULATOR ) )
   {
      ch->println( "That ship is already set as a simulator!\n\r", ch );
      return;
   }
   if( str_cmp( ship->owner, ch->name ) && ( ch->top_level < 57 ) && str_cmp( ship->owner, "public" ) )
   {
      ch->println( "&YYou can only make a simulator out of a public ship or one of your own!\n\r", ch );
      return;
   }
   if( ship->owner )
      STRFREE( ship->owner );
   ship->owner = STRALLOC( "public" );

   SET_BIT( ship->flags, SHIP_SIMULATOR );
   ship->sim_vnum = ch->in_room->vnum;
   ch_printf( ch, "&wThe ship &W%s &wis now set as a simulator with a simvnum of %d.\n\r", ship->name, ship->sim_vnum );
   save_ship( ship );
   return;
}

void do_makemobship( CHAR_DATA *ch, char *argument ){

}

void do_prototypes( CHAR_DATA *ch, char *argument )
{

}

//Written by Ackbar, updated by Tranin, Reupdated by Tawnos
void do_sabotage(CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int chance;
    SHIP_DATA *ship;
    strcpy( arg, argument );

    switch( ch->substate )
    {
         default:
            if (  (ship = ship_from_engine(ch->in_room->vnum))  == NULL )
            {
               ch->println("&RYou must be in the engine room of a ship to do that!\n\r",ch);
               return;
            }

                if ( str_cmp( argument, "hull" ) && str_cmp( argument, "drive" )
                     && str_cmp( argument, "launcher" ) && str_cmp( argument, "laser" ))
            {
                ch->println("&RYou need to specify something to sabotage!\n\r",ch);
                ch->println("&rTry: drive, launcher, laser\n\r",ch);
                return;
            }

	    if(IS_SET(ship->flags, SHIP_SIMULATOR))
	    {
		ch->println("&GYou pull at some wires.. and end up with space invaders! *Whee!*\n\r", ch);
		return;
	    }

            chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_sabotage]);
            if ( number_percent( ) < chance )

    		{
                ch->println( "&GYou begin yanking at wires.\n\r", ch);
                act( AT_PLAIN, "$n starts ripping into the ships wires.", ch,
                  NULL, argument , TO_ROOM );
                add_timer ( ch , TIMER_DO_FUN , 5 , do_sabotage , 1 );
                ch->dest_buf = str_dup(arg);
                return;
	        }
            ch->println("&RYou fail to locate the proper cords to sever!\n\r",ch);
	        learn_from_failure( ch, gsn_sabotage );
    	   	return;

    	case 1:
    		if ( !ch->dest_buf )
               return;
    		strcpy(arg, ch->dest_buf);
    		DISPOSE( ch->dest_buf);
    		break;

    	case SUB_TIMER_DO_ABORT:
    		DISPOSE( ch->dest_buf );
    		ch->substate = SUB_NONE;
    		if ( (ship = ship_from_engine(ch->in_room->vnum))  == NULL )
    		      return;
            ch->println("&RYou are distracted and fail to finish your work.\n\r", ch);
    		return;
    }

    ch->substate = SUB_NONE;

    if ( (ship = ship_from_engine(ch->in_room->vnum)) == NULL )
    {
       return;
    }

    if ( !str_cmp(arg,"drive") )
    {
       ship->shipstate = SHIP_DISABLED;
       ch->println("&GShips drive successfully disabled.\n\r", ch);
    }

    if ( !str_cmp(arg,"launcher") )
    {
       ship->missilestate = MISSILE_DAMAGED;
       ship->torpedostate = MISSILE_DAMAGED;
       ship->rocketstate = MISSILE_DAMAGED;
       ch->println("&GMissile launcher successfully disabled.\n\r", ch);
    }

    if ( !str_cmp(arg,"laser") )
    {
       ship->primaryState = LASER_DAMAGED;
       ch->println("&GMain laser successfully disabled.\n\r", ch);
    }

    act( AT_PLAIN, "$n finishes his rage outburst on the wires.", ch,
         NULL, argument , TO_ROOM );

    learn_from_success( ch, gsn_sabotage );
}

void do_makeshipbomb(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    int level, chance, strength, weight;
    bool checktool, checkdrink, checkbatt, checkchem1, checkchem2, checkchem3, checkcirc1, checkcirc2;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int circNum=0, chemNum=0;
    strcpy(arg, argument);

    switch( ch->substate )
    {
    	default:
    	        if ( arg[0] == '\0' )
                {
                  ch->println( "&RUsage: Makeshipbomb <name>\n\r&w", ch);
                  return;
                }

	checktool=checkdrink=checkbatt=checkchem1 =
			checkchem2=checkchem3=checkcirc1=checkcirc2=false;

                if ( !IS_SET(  ch->in_room->room_flags, ROOM_FACTORY ) )
                {
                   ch->println( "&RYou need to be in a factory or workshop to do that.\n\r", ch);
                   return;
                }

                for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
                {

                  if (obj->item_type == ITEM_TOOLKIT)
		  {
                    checktool = true;
		    continue;
		  }
                  if (obj->item_type == ITEM_DRINK_CON && obj->value[1] == 0 )
		  {
          	    checkdrink = true;
		    continue;
		  }
                  if (obj->item_type == ITEM_BATTERY)
		  {
                    checkbatt = true;
		    continue;
		  }
                  if (obj->item_type == ITEM_CIRCUIT)
                  {
		    circNum+=obj->count;
		    continue;
                  }
                  if (obj->item_type == ITEM_CHEMICAL)
                   {
		    chemNum += obj->count;
		    continue;
		   }
                  }

                if ( !checktool )
                {
                   ch->println( "&RYou need toolkit.\n\r", ch);
                   return;
                }

                if ( !checkdrink )
                {
                   ch->println( "&RYou will need an empty drink container to mix and hold the chemicals.\n\r", ch);
                   return;
                }

                if ( !checkbatt )
                {
                   ch->println( "&RYou need a battery to spark the explosion.\n\r", ch);
                   return;
                }

                if ( circNum==0 )
                {
                   ch->println( "&RYou need a circuit board for the receiver.\n\r", ch);
                   return;
                }

                if ( circNum==1 )
                {
                   ch->println( "&RYou need another circuit board for the transmitter.\n\r", ch);
                   return;
                }

                if ( chemNum == 0 )
                {
                   ch->println( "&RYou need 1 to 3 chemicals to cause an explosion!\n\r", ch);
                   return;
                }

    	        chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makeshipbomb]);
                if ( number_percent( ) < chance )
    		{
    		   ch->println( "&GYou begin the long process of making a shipbomb.\n\r", ch);
    		   act( AT_PLAIN, "$n takes $s tools and a drink container and begins to work on something.", ch,
		        NULL, argument , TO_ROOM );
		   add_timer ( ch , TIMER_DO_FUN , 25 , do_makeshipbomb , 1 );
    		   ch->dest_buf   = str_dup(arg);
    		   return;
	        }
	        ch->println("&RYou can't figure out how to fit the parts together.\n\r",ch);
	        learn_from_failure( ch, gsn_makeshipbomb );
    	   	return;

    	case 1:
    		if ( !ch->dest_buf )
    		     return;
    		strcpy(arg, ch->dest_buf);
    		DISPOSE( ch->dest_buf);
    		break;

    	case SUB_TIMER_DO_ABORT:
    		DISPOSE( ch->dest_buf );
    		ch->substate = SUB_NONE;
    	        ch->println("&RYou are interupted and fail to finish your work.\n\r", ch);
    	        return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makeshipbomb]);
    vnum = 68;

    if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
    {
         ch->println( "&RThe item you are trying to create is missing from the database.\n\rPlease inform the administration of this error.\n\r", ch );
         return;
    }

    checktool = false;
    checkdrink = false;
    checkbatt = false;
    checkchem1 = false;
    checkchem2 = false;
    checkchem3 = false;
    checkcirc1 = false;
    checkcirc2 = false;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
       if (obj->item_type == ITEM_TOOLKIT)
       {
          checktool = true;
	  continue;
       }
       if (obj->item_type == ITEM_DRINK_CON && checkdrink == false && obj->value[1] == 0 )
       {
          checkdrink = true;
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
	  continue;
       }
       if (obj->item_type == ITEM_BATTERY && checkbatt == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkbatt = true;;
	  continue;
       }
       if (obj->item_type == ITEM_CHEMICAL)
       {
	  if(obj->count > 3)
	  {
	    obj->count -= 3;
	    chemNum += 3;
	  }
	  else if(obj->count == 1)
	  {
            separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
	    chemNum += 1;
	  }
	  else if(obj->count == 3)
	  {
            obj_from_char( obj );
            extract_obj( obj );
	    chemNum += 3;
	  }
	  else
	  {
            obj_from_char( obj );
            extract_obj( obj );
	    chemNum += 2;
	  }

          strength = URANGE( chemNum*10, obj->value[0] * 3, level * 15 );
          weight = obj->weight;
	  continue;
       }

       if (obj->item_type == ITEM_CIRCUIT)
       {
	  if(obj->count > 2)
	  {
	    obj->count -= 2;
	    circNum += 2;
	  }
	  else if(obj->count == 2)
	  {
	    obj_from_char(obj);
	    extract_obj(obj);
	    circNum += 2;
	  }
	  else
	  {
            obj_from_char( obj );
            extract_obj( obj );
  	    circNum += 1;
	  }
	  continue;
       }

    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makeshipbomb]) ;

    if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkdrink ) || ( !checkbatt ) || (chemNum < 1) || (circNum< 2) )
    {
	sprintf(buf,"%d %d", chemNum, circNum);
	log_string(buf);

       ch->println( "&RJust as you are about to finish your work,\n\ryour newly created bomb pops, and begins smoking...\n\ryou quickly realize you nearly died as it fizzled!\n\r", ch);
       learn_from_failure( ch, gsn_makeshipbomb );
       return;
    }

    obj = create_object( pObjIndex, level );

    obj->item_type = ITEM_SHIPBOMB;
    SET_BIT( obj->wear_flags, ITEM_TAKE );
    obj->level = level;
    obj->weight = weight;
    STRFREE( obj->name );
    strcpy( buf , arg );
    strcat( buf , " bomb");
    obj->name = STRALLOC( buf );
    strcpy( buf, arg );
    STRFREE( obj->short_descr );
    obj->short_descr = STRALLOC( buf );
    STRFREE( obj->description );
    strcat( buf, " was set here." );
    obj->description = STRALLOC( buf );
    obj->value[0] = strength/2;
    obj->value[1] = strength;
    obj->cost = obj->value[1]*25;

    obj = obj_to_char( obj, ch );

    ch->println( "&GYou finish your work and hold up your newly created shipbomb.&w\n\r", ch);
    act( AT_PLAIN, "$n finishes making $s new shipbomb.", ch,
         NULL, argument , TO_ROOM );

    {
         long xpgain;

         xpgain = UMIN( obj->cost*50 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "You gain %d engineering experience.", xpgain );
    }
        learn_from_success( ch, gsn_makeshipbomb );
}


void do_shiprepair(CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int chance, change;
    SHIP_DATA *ship;

    strcpy( arg, argument );

    if( (ch->pcdata->learned[gsn_shiprepair]) <= 0)
    {
	ch->println("Huh?\n\r", ch);
	return;
    }

    switch( ch->substate )
    {
    	default:
    	        if ( str_cmp(argument, "turret") )
    	        {
				   if( (ship = ship_from_engine(ch->in_room->vnum)) == NULL)
				   {
    	            ch->println("&RYou must be in the engine room of a ship to do that!\n\r",ch);
    	            return;
				   }
    	        }
				else
				{
				   if( (ship = ship_from_turret(ch->in_room->vnum)) == NULL)
				   {
					ch->println("&RYou must be in the turret you wish to repair.\n\r", ch);
					return;
				   }
				   else
				   {

        if(ch->in_room->vnum == ship->turret1) {if(ship->statet1 == LASER_READY){ch->println("&GThis turret is operational.\n\r", ch); return;}}
		else if(ch->in_room->vnum == ship->turret2) {if(ship->statet2 == LASER_READY){ch->println("&GThis turret is operational.\n\r", ch); return;}}
		else if(ch->in_room->vnum == ship->turret3) {if(ship->statet3 == LASER_READY){ch->println("&GThis turret is operational.\n\r", ch); return;}}
		else if(ch->in_room->vnum == ship->turret4) {if(ship->statet4 == LASER_READY){ch->println("&GThis turret is operational.\n\r", ch); return;}}
		else if(ch->in_room->vnum == ship->turret5) {if(ship->statet5 == LASER_READY){ch->println("&GThis turret is operational.\n\r", ch); return;}}
		else if(ch->in_room->vnum == ship->turret6) {if(ship->statet6 == LASER_READY){ch->println("&GThis turret is operational.\n\r", ch); return;}}
		else if(ch->in_room->vnum == ship->turret7) {if(ship->statet7 == LASER_READY){ch->println("&GThis turret is operational.\n\r", ch); return;}}
		else if(ch->in_room->vnum == ship->turret8) {if(ship->statet8 == LASER_READY){ch->println("&GThis turret is operational.\n\r", ch); return;}}
		else if(ch->in_room->vnum == ship->turret9) {if(ship->statet9 == LASER_READY){ch->println("&GThis turret is operational.\n\r", ch); return;}}
		else if(ch->in_room->vnum == ship->turret10) {if(ship->statet10 == LASER_READY){ch->println("&GThis turret is operational.\n\r", ch); return;}}
        else {ch->println("&RThis isn't a turret.\n\r", ch); return;}
					}
                }
                if ( !str_cmp( arg, "drive" ) )
                {
                    if (ship->shipstate == SHIP_HYPERSPACE)
                    {
                       ch->println("You cannot repair a hyperdrive unit in hyperspace.\n\r", ch);
                       return;
                    }

                     if ( ship->location == ship->lastdoc )
                         ship->shipstate = SHIP_DOCKED;
                     else
                        ship->shipstate = SHIP_READY;

                     ship->hyperstate = LASER_READY;
                     ch->println( "&GShips drive repaired.\n\r", ch );
                }
                if ( str_cmp( argument , "hull" ) && str_cmp( argument , "drive" ) &&
                     str_cmp( argument , "launcher" ) && str_cmp( argument , "laser" ) && str_cmp(argument, "turret"))
                {
                   ch->println("&RYou need to specify something to repair:\n\r",ch);
                   ch->println("&rTry: hull, drive, launcher, laser, turret.\n\r",ch);
                   return;
                }
                chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_shiprepair]);
                if ( number_percent( ) < chance )
    		{
    		   ch->println( "&GYou begin your repairs.\n\r", ch);
    		   act( AT_PLAIN, "$n begins repairing the ships $T.", ch,
		        NULL, argument , TO_ROOM );
    		   if ( !str_cmp(arg,"hull") )
    		     add_timer ( ch , TIMER_DO_FUN , 15 , do_shiprepair , 1 );
    		   else
    		     add_timer ( ch , TIMER_DO_FUN , 5 , do_shiprepair , 1 );
    		   ch->dest_buf = str_dup(arg);
    		   return;
	        }
	        ch->println("&RYou fail to locate the source of the problem.\n\r",ch);
    	   	return;

    	case 1:
    		if ( !ch->dest_buf )
    		   return;
    		strcpy(arg, ch->dest_buf);
    		DISPOSE( ch->dest_buf);
    		break;

    	case SUB_TIMER_DO_ABORT:
    		DISPOSE( ch->dest_buf );
    		ch->substate = SUB_NONE;
    	        ch->println("&RYou are distracted and fail to finish your repairs.\n\r", ch);
    		return;
    }

    ch->substate = SUB_NONE;

    if ( (ship = ship_from_engine(ch->in_room->vnum)) == NULL && (ship = ship_from_turret(ch->in_room->vnum)) == NULL)
    {
       return;
    }

    if ( !str_cmp(arg,"hull") )
    {
        change = URANGE( 0 ,
                         number_range( (int) ( ch->pcdata->learned[gsn_shiprepair] / 2 ) , (int) (ch->pcdata->learned[gsn_shiprepair]) ),
                         ( ship->maxhull - ship->hull ) );
        ship->hull += change;
        ch_printf( ch, "&GRepair complete.. Hull strength inreased by %d points.\n\r", change );
    }

    if ( !str_cmp(arg,"drive") )
    {
       if (ship->location == ship->lastdoc)
          ship->shipstate = SHIP_DOCKED;
       else
          ship->shipstate = SHIP_READY;
       ch->println("&GShips drive repaired.\n\r", ch);
    }

    if ( !str_cmp(arg,"launcher") )
    {
       ship->missilestate = MISSILE_READY;
       ch->println("&GMissile launcher repaired.\n\r", ch);
    }

    if ( !str_cmp(arg,"laser") )
    {
       ship->primaryState = LASER_READY;
       ch->println("&GMain laser repaired.\n\r", ch);
    }
    if( !str_cmp(arg, "turret") )
    {

        if(ch->in_room->vnum == ship->turret1) ship->statet1 = LASER_READY;
		if(ch->in_room->vnum == ship->turret2) ship->statet2 = LASER_READY;
		if(ch->in_room->vnum == ship->turret3) ship->statet3 = LASER_READY;
		if(ch->in_room->vnum == ship->turret4) ship->statet4 = LASER_READY;
		if(ch->in_room->vnum == ship->turret5) ship->statet5 = LASER_READY;
		if(ch->in_room->vnum == ship->turret6) ship->statet6 = LASER_READY;
		if(ch->in_room->vnum == ship->turret7) ship->statet7 = LASER_READY;
		if(ch->in_room->vnum == ship->turret8) ship->statet8 = LASER_READY;
		if(ch->in_room->vnum == ship->turret9) ship->statet9 = LASER_READY;
		if(ch->in_room->vnum == ship->turret10) ship->statet10 = LASER_READY;

	  ch->println("&GTurret operational.\n\r", ch);
    }
    act( AT_PLAIN, "$n finishes the repairs.", ch,
         NULL, argument , TO_ROOM );

    learn_from_success( ch, gsn_shiprepair );

}

void do_transship( CHAR_DATA *ch ,char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	int arg3;
    SHIP_DATA *ship;

    if ( IS_NPC( ch ) )
    {
	ch->println( "Huh?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    ship = get_ship( arg1 );
	if ( !ship )
    {
	ch->println( "No such ship.\n\r", ch );
	return;
    }

	arg3 = atoi( arg2 );

	 if ( arg1[0] == '\0' || arg2[0] == '\0' || arg1[0] == '\0' )
    {
	ch->println( "Usage: transship <ship> <vnum>\n\r", ch );
	return;
    }

     ship->shipstate = SHIP_READY;
     ship->location = arg3;

     if ( ship->ship_class != SHIP_SPACE_STATION && ship->type != MOB_SHIP )
     {
           extract_ship( ship );
           ship_to_room( ship , ship->location );

           ship->lastdoc = ship->location;
           ship->shipstate = SHIP_DOCKED;
     }

     if (ship->starsystem)
        ship_from_starsystem( ship, ship->starsystem );

     save_ship(ship);
	 ch->println( "Ship Transfered.\n\r", ch );
}

void do_transshipss( CHAR_DATA *ch ,char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    SHIP_DATA *ship;
    SPACE_DATA *starsystem;

    if ( IS_NPC( ch ) )
    {
	ch->println( "Huh?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    ship = get_ship( arg1 );
	if ( !ship )
    {
	ch->println( "No such ship.\n\r", ch );
	return;
    }

	 if ( arg1[0] == '\0' || arg2[0] == '\0' || arg1[0] == '\0' )
    {
	ch->println( "Usage: transshipss <ship> <starsystem>\n\r", ch );
	return;
    }

     ship->shipstate = SHIP_READY;

    starsystem = starsystem_from_name( arg2 );
    if ( !starsystem )
    {
	ch->println( "No such starsystem.\n\r", ch );
	return;

    }
    extract_ship(ship);
    ship_to_starsystem( ship, starsystem );
    ship->location = 0;
    ship->hx = 1;
    ship->hy = 1;
    ship->hz = 1;
    ship->vx = 1111;
    ship->vy = -1111;
    ship->vz = 1111;
    resetship(ship);
    save_ship(ship);
    ship->inship = NULL;
    ch->println( "Ship transferred.\n\r", ch );
}

void do_removeship(CHAR_DATA *ch, char *argument)
{
  SHIP_DATA *ship;
  char buf[MAX_STRING_LENGTH];

    if( argument[0] == '\0' )
    {
	ch->println( "Remove which ship?\n\r", ch);
	return;
    }

    ship = get_ship( argument );
    if ( !ship )
    {
	ch->println( "No such ship.\n\r", ch );
	return;
    }
    sprintf(buf,"%s: removeship %s",ch->name,argument);
    log_string(buf);
    destroy_ship(ship, ch, "removed");
    ch->println("Ship removed.\r\n",ch);
  return;
}



SHIP_DATA *get_ship2( char *name )
{
    SHIP_DATA *ship;
//    char buf[MAX_STRING_LENGTH];
    for ( ship = first_ship; ship; ship = ship->next ){
//       sprintf(buf, "'%s' '%s'", name, ship->name);
//       bug(buf, 0);
       if(ship->name)
         if ( !strcmp( name, ship->name ) )
           return ship;
    }
    return NULL;
}

void do_shiplock ( CHAR_DATA *ch, char *argument )
{
    int locknum;
    char buf[MAX_INPUT_LENGTH];
    SHIP_DATA *ship;

    if ( (ship = ship_from_cockpit(ch->in_room->vnum)) == NULL )
     {
     	ch->println("You must be in the cockpit of a ship to do that.\n\r", ch);
     	return;
     }

    if ( !check_pilot( ch , ship ) )
     {
        ch->println("&RHey, thats not your ship! Try renting a public one.\n\r",ch);
        return;
     }

    locknum = atoi( argument );
    if (locknum < 1000 || locknum > 9999)
     {
     	ch->println("Number out of range.\n\r", ch);
        return;
     }

    ship->password = locknum;
    sprintf(buf, "%s lock number set to %d.\n\r", ship->name, ship->password);
    ch->println(buf, ch);
    save_ship(ship);
}

char *ship_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
        if ( vector & SHIP_NOHIJACK     ) strcat( buf, " nohijack"      );
	if ( vector & SHIP_SHIELD_BOOST ) strcat( buf, " shield_boost"  );
	if ( vector & SHIP_TORP_BOOST   ) strcat( buf, " torp_boost"    );
	if ( vector & SHIP_CHAFF_BOOST  ) strcat( buf, " chaff_boost"   );
	if ( vector & SHIP_HULL_BOOST   ) strcat( buf, " hull_boost"    );
	if ( vector & SHIP_LASER_BOOST  ) strcat( buf, " laser_boost"   );
	if ( vector & SHIP_MISSILE_BOOST) strcat( buf, " missile_boost" );
	if ( vector & SHIP_ROCKET_BOOST ) strcat( buf, " rocket_boost"  );
	if ( vector & SHIP_SIMULATOR    ) strcat( buf, " simulator"     );
	if ( vector & SHIP_NODESTROY    ) strcat( buf, " nodestroy"     );
	if ( vector & SHIP_NOSLICER     ) strcat( buf, " noslicer"     );
	if ( vector & XSHIP_ION_LASERS   ) strcat( buf, " ion_lasers"     );
	if ( vector & XSHIP_ION_DRIVE    ) strcat( buf, " ion_drive"     );
	if ( vector & XSHIP_ION_ION      ) strcat( buf, " ion_ion"     );
	if ( vector & XSHIP_ION_TURRET1      ) strcat( buf, " ion_turret1"     );
	if ( vector & XSHIP_ION_TURRET2      ) strcat( buf, " ion_turret2"     );
	if ( vector & XSHIP_ION_TURRET3      ) strcat( buf, " ion_turret3"     );
	if ( vector & XSHIP_ION_TURRET4      ) strcat( buf, " ion_turret4"     );
	if ( vector & XSHIP_ION_TURRET5      ) strcat( buf, " ion_turret5"     );
	if ( vector & XSHIP_ION_TURRET6      ) strcat( buf, " ion_turret6"     );
	if ( vector & XSHIP_ION_TURRET7      ) strcat( buf, " ion_turret7"     );
	if ( vector & XSHIP_ION_TURRET8      ) strcat( buf, " ion_turret8"     );
	if ( vector & XSHIP_ION_TURRET9      ) strcat( buf, " ion_turret9"     );
	if ( vector & XSHIP_ION_TURRET10     ) strcat( buf, " ion_turret10"    );
	if ( vector & SHIP_RESPAWN          ) strcat( buf, " respawn"         );
	if ( vector & XSHIP_ION_MISSILES     ) strcat( buf, " ion_missiles"    );
	if ( vector & XSHIP_ION_HYPER        ) strcat( buf, " ion_hyper"       );
	if ( vector & SHIP_CLOAK	) strcat( buf, " cloak" );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

void do_cut(CHAR_DATA *ch, char *argument )
{
  SHIP_DATA *ship;
//  char buf[MAX_STRING_LENGTH];
  /*
   * This function will be used to repair damage caused by an ion cannon. Only this
   * damage caused by it will be repaired. It checks by a flag assigned to the ship
   * by the use of ion cannons.
  *//*


  if ((ship = ship_from_cockpit(ch->in_room->vnum)) == NULL)
  {
    ch->println("You must be in a ship to cut power.\n\r", ch);
    return;
  }

  if (ship->shipstate != SHIP_READY && ship->shipstate != SHIP_DISABLED)
  {
    ch->println("Please wait until you have finished your current manuever.\n\r", ch);
    return;
  }
 switch( ch->substate )
 {
 default:
 /*
  if(ship->cloaked == true)
  {
  	ch->println("&BYour cloak dissipates with the ship's power.\n\r", ch);
  	sprintf(buf,"&R&B%s blips onto your radar at %.0f, %.0f, %.0f.\n\r", ship->name, ship->vx, ship->vy, ship->vz);
  	echo_to_system(AT_BLUE, ship, buf, NULL);
  	ship->cloaked = false;
  }
  *//*
  ch->println("You flip the power switch and wait for the ship systems to reboot.\n\r", ch);
  add_timer ( ch , TIMER_DO_FUN , 20 , do_cut , 1 );
  return;

 case 1:
  break;

 case SUB_TIMER_DO_ABORT:
 ch->substate = SUB_NONE;
 ch->println("&RYou flip the power back on but find your systems are still damaged.\n\r", ch);
 return;
}

  ch->println("The ship's systems power up restoring ion damaged systems.\n\r", ch);

  if ( IS_SET(ship->flags, XSHIP_ION_DRIVE) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WDrive Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_DRIVE);
    ship->shipstate = SHIP_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_HYPER) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WHyper Drive Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_HYPER);
    ship->shipstate = LASER_READY;

  }

  if ( IS_SET(ship->flags, XSHIP_ION_ION) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WSecondary weapon systems functional.");
    REMOVE_BIT(ship->flags, XSHIP_ION_ION);
    ship->secondaryState = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_LASERS) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WPrimary weapon systems functional.");
    REMOVE_BIT(ship->flags, XSHIP_ION_LASERS);
    ship->primaryState = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_MISSILES) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WMissiles Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_MISSILES);
    ship->missilestate = MISSILE_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET1) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WTurret1 Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET1);
    ship->turret1 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET2) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WTurret2 Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET2);
    ship->turret2 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET3) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WTurret3 Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET3);
    ship->turret3 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET4) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WTurret4 Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET4);
    ship->turret4 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET5) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WTurret5 Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET5);
    ship->turret5 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET6) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WTurret6 Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET6);
    ship->turret6 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET7) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WTurret7 Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET7);
    ship->turret7 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET8) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WTurret8 Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET8);
    ship->turret8 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET9) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WTurret9 Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET9);
    ship->turret9 = LASER_READY;
  }

  if ( IS_SET(ship->flags, XSHIP_ION_TURRET10) )
  {
    //echo_to_cockpit( AT_GREEN , ship, "&G[&WALERT&G] &WTurret10 Functional");
    REMOVE_BIT(ship->flags, XSHIP_ION_TURRET10);
    ship->turret10 = LASER_READY;
  }
}

void do_transmit_pass( CHAR_DATA *ch, char *argument)
/* Coded by Trillen of SW:FotE for slave circuit modules
 * Allows player to remotely change the shiplock *//*
{
   int locknum;
   SHIP_DATA *ship;
   char buf[MAX_STRING_LENGTH];
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   bool ch_comlink;
   OBJ_DATA *obj;

   argument = one_argument(argument, arg1);
   argument = one_argument(argument, arg2);

   if ( !arg1 || arg1[0] == '\0')
   {

          ch->println( "Usage: transmit_pass <ship> <####>\n\r", ch );
          return;
   }

   if ( !arg2 || arg2[0] == '\0')
   {

          ch->println( "Usage: transmit_pass <ship> <####>\n\r", ch );
          return;
   }

	   for(ship=first_ship; ship; ship=ship->next)
	    if(!str_prefix(arg1, ship->name)) break;

		if  (!ship)
	     {
		ch->println( "Error: That ship does not exist.\n\r", ch );
		return;
	     }

        if (ship->slave < 1)
	     {
	       ch->println( "Error: That ship does not have a slave circuit.\n\r", ch);
	       return;
	     }

    if ( !check_pilot( ch , ship ) )
     {
        ch->println("&RHey, thats not your ship!\n\r",ch);
        return;
     }

	  if (!IS_IMMORTAL(ch) )
	  {
	    ch_comlink = false;

	    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	    {
	      if (obj->pIndexData->item_type == ITEM_COMLINK)
        ch_comlink = true;
	    }
	  }
	    if ( !ch_comlink )
		 {
		   ch->println( "You need a comlink to do that.\n\r", ch);
		    return;
		}

		if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
		{
		ch->println( "Your comlink will not function here.\n\r", ch );
		return;
		}

    locknum = atoi( arg2 );
    if (locknum < 1000 || locknum > 9999)
     {
     	ch->println("Number out of range. Shippass must be between 1000 and 9999.\n\r",ch);
        return;
     }

    ship->password = locknum;
    sprintf(buf, "%s lock number set to %d.\n\r", ship->name, ship->password);
    ch->println(buf, ch);
    save_ship(ship);
}

void do_transmit_call( CHAR_DATA *ch, char *argument)
/* Coded by Trillen and Eleven of SW:FotE for slave circuit modules
 * Allows user to call ship from anywhere in the galaxy. *//*
{
    SHIP_DATA *ship;
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int roomloc;
    int hprspd = 0;
    bool ch_comlink;
    OBJ_DATA *obj;

    switch( ch->substate )
    {
    	default:

		 if ( argument[0] == '\0' )
	    {
		ch->println( "Usage: transmit_call <ship>\n\r", ch );
		return;
	      }


	   for(ship=first_ship; ship; ship=ship->next)
	    if(!str_prefix(argument, ship->name)) break;

		if  (!ship)
	     {
		ch->println( "Error: That ship does not exist.\n\r", ch );
		return;
	     }

        if (ship->slave < 2)
	     {
	       ch->println( "Error: That ship does not have a Class 2 slave circuit.\n\r", ch );
	       return;
	     }

	       if (!check_pilot( ch , ship ))
	     {
	        ch->println( "That ship does not belong to you!\n\r", ch );
	       return;
	    }

	  if (!IS_IMMORTAL(ch) )
	  {
	    ch_comlink = false;

	    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	    {
	      if (obj->pIndexData->item_type == ITEM_COMLINK)
        ch_comlink = true;
	    }
	  }
	    if ( !ch_comlink )
		 {
		   ch->println( "You need a comlink to do that.\n\r", ch);
		    return;
		}

	 if (!IS_NPC(ch) && IS_SET(ch->pcdata->act2, ACT_GAGGED))
	 {
		    ch->println( "You can't activate the call signal when you're gagged!\n\r", ch);
		    return;
	 }

		if (!IS_NPC(ch)
		&& ( IS_SET(ch->act, PLR_SILENCE) ))
		{
		ch->println( "You can't activate the call signal when you're silenced!\n\r", ch );
		return;
		}

		if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
		{
		ch->println( "Your comlink will not function here.\n\r", ch );
		return;
		}

        if ( !IS_SET( ch->in_room->room_flags, ROOM_CAN_LAND ) )
		{
		ch->println( "You must be in a hanger or docking bay!\n\r", ch );
		return;
		}

      if (ship->hyperspeed == 5)
       hprspd = 25;

       if (ship->hyperspeed == 4)
       hprspd = 22;

       if (ship->hyperspeed == 3)
       hprspd = 19;

       if (ship->hyperspeed == 2)
       hprspd = 16;

       if (ship->hyperspeed == 1)
       hprspd = 13;

       sprintf(buf, "Remote Signal Recieved. En route to rendezvous point.\n\r");

       ch->println( "Remote Signal Recieved. Ship is en route to your location.\n\r", ch );
       echo_to_ship( AT_PLAIN, ship, buf );
       roomloc = ch->in_room->vnum;

       add_timer ( ch , TIMER_DO_FUN , hprspd , do_transmit_call, 1 );
       ch->dest_buf = str_dup(argument);
       return;


    	case 1:
    	 if ( !ch->dest_buf )
           return;
    	argument = str_dup(ch->dest_buf);
    	 DISPOSE( ch->dest_buf);
    		break;

    	case SUB_TIMER_DO_ABORT:
    	 if ( !ch->dest_buf )
    	   return;
    	argument = str_dup(ch->dest_buf);
    	 DISPOSE( ch->dest_buf);
    		break;
  }
    ch->substate = SUB_NONE;

    for(ship=first_ship; ship; ship=ship->next)
	if(!str_prefix(argument, ship->name)) break;

    if (!ship)
    {
      ch->println( "Error: Ship cannot be found.\n\r", ch );
      return;
    }

    if ( !IS_SET( ch->in_room->room_flags, ROOM_CAN_LAND ) )
    {
	ch->println( "Your ship must land in a hanger or docking bay!\n\r", ch );
        return;
    }
    roomloc = ch->in_room->vnum;

       sprintf(buf2, "Ship has arrived at rendezvous point.\n\r");

      {
       ch->println( "Ship has arrived at rendezvouz point.\n\r", ch );
       echo_to_ship( AT_PLAIN, ship, buf2 );
      }

       {
           ship->location = roomloc;
           extract_ship( ship );
           ship_to_room( ship , ship->location );

           ship->lastdoc = ship->location;
           ship->shipstate = SHIP_DOCKED;
       }
        if (ship->starsystem)
           ship_from_starsystem( ship, ship->starsystem );

     save_ship(ship);

}

void do_transmit_status( CHAR_DATA *ch, char *argument)
/* Coded by Trillen of SW:FotE for slave circuit modules.
 * Allows player to use comlink to get status on any of his ships with a slave circuit.  *//*
{
    SHIP_DATA *ship;
    int x;
    bool ch_comlink;
    OBJ_DATA *obj;

	 if ( argument[0] == '\0' )
         {
           ch->println( "Usage: transmit_status <ship>\n\r", ch );
	   return;
         }


         for(ship=first_ship; ship; ship=ship->next)
           if(!str_prefix(argument, ship->name)) break;

	     if  (!ship)
	      {
		ch->println( "Error: That ship does not exist.\n\r", ch );
		return;
	      }

         if (!ship->slave >= 3)
	     {
	       ch->println( "Error: That ship does not have a Class 3 slave circuit.\n\r", ch);
	       return;
	     }

         if (!check_pilot( ch , ship ))
	     {
	        ch->println( "That ship does not belong to you!\n\r", ch );
	        return;
	     }

	 if (!IS_IMMORTAL(ch) )
	 {
	   ch_comlink = false;

	   for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	   {
	     if (obj->pIndexData->item_type == ITEM_COMLINK)
               ch_comlink = true;
	   }
	 }
	 if ( !ch_comlink )
	 {
	   ch->println( "You need a comlink to do that.\n\r", ch);
	   return;
	 }

	 if (!IS_NPC(ch) && IS_SET(ch->pcdata->act2, ACT_BOUND))
	 {
	   ch->println( "You can't transmit the status request when you're bound!\n\r", ch);
	   return;
	 }

         if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
	 {
	   ch->println( "Your comlink will not function here.\n\r", ch );
	   return;
	 }

         ch->println( "Remote status request transmitted. Receiving response now.\n\r", ch );
         ch_printf( ch, "&w[&W %s &w]   &zShip Condition: %s\n\r\n\r",ship->name, ship->shipstate == SHIP_DISABLED ? "&RDisabled" : "&GRunning");
         ch_printf( ch, "&zCurrent Coordinates:&w %.0f %.0f %.0f &zCurrent Heading:&w %.0f  %.0f %.0f\n\r", ship->vx, ship->vy, ship->vz, ship->hx, ship->hy, ship->hz );
         ch_printf( ch, "&zSpeed:&w %d&W/%d   &zHyperdrive: &wClass %d %s %s\n\r", ship->currspeed , ship->realspeed, ship->hyperspeed, ship->ship_class >= SHIP_CRUISER ? "&zBay doors:" : "", (ship->ship_class >= SHIP_CRUISER && ship->bayopen == true) ? "&wOpen" : (ship->ship_class>= SHIP_CRUISER && ship->bayopen == false) ? "&wClosed" : "" );
         ch_printf( ch, "&zHull:&w %d&W/%d   &zShields:&w %d&W/%d    &zFuel:&w %d&W/%d\n\r\n\r", ship->hull, ship->maxhull, ship->shield, ship->maxshield, ship->energy, ship->maxenergy);
	 ch_printf( ch, "&zPrimary Weapon System:   &w%-7s&W::  &w%s\n\r", ship->primaryState == LASER_DAMAGED ? "&ROffline" : "&GOnline", primary_beam_name(ship));
	switch(ship->primaryType)
	{
	 case 0: x = 0; break;
	 case 1: x = ship->primaryCount; break;
	 case 2: x = 2*ship->primaryCount; break;
	 case 3: x = 3*ship->primaryCount; break;
	 case 4: x = 4*ship->primaryCount; break;
	 case 5: x = ship->primaryCount; break;
	 case 6: x = ship->primaryCount; break;
	 case 7: x = ship->primaryCount; break;
	 case 8: x = ship->primaryCount; break;
	 case 9: x = ship->primaryCount; break;
	 default: x = 1; break;
	}

	ch->printlnf(  "&z   Linked fire: &w%s\n\r", (x <= 1) ? "Unavailable" : ship->primaryLinked == true ? "&GON" : "&ROFF");

	switch(ship->secondaryType)
	{
	 case 0: x = 0; break;
	 case 1: x = ship->secondaryCount; break;
	 case 2: x = 2*ship->secondaryCount; break;
	 case 3: x = 3*ship->secondaryCount; break;
	 case 4: x = 4*ship->secondaryCount; break;
	 case 5: x = ship->secondaryCount; break;
	 case 6: x = ship->secondaryCount; break;
	 case 7: x = ship->secondaryCount; break;
	 case 8: x = ship->secondaryCount; break;
	 case 9: x = ship->secondaryCount; break;
	 default: x = 1; break;
	}
         if(ship->secondaryCount != 0)
	 {
	   ch_printf( ch, "&zSecondary Weapon System: &w%-7s&W::  &w%s\n\r", ship->secondaryState == LASER_DAMAGED ? "&ROffline" : "&GOnline", secondary_beam_name(ship));
	   ch->printlnf(  "&z   Linked fire: &w%s\n\r", (x <= 1) ? "Unavailable" : ship->secondaryLinked == true ? "&GON" : "&ROFF");
   	 }
         if (ship->turret1)
           ch_printf( ch, "\n\r&zTurret One:  &w %s  &zCurrent Target:&w %s\n\r", ship->statet1 == LASER_DAMAGED ? "Damaged" : "Operational" , ship->target1 ?  ship->target1->name : "none");
    if (ship->turret2)
        ch_printf( ch, "&zTurret Two:  &w %s  &zCurrent Target:&w %s\n\r", ship->statet2 == LASER_DAMAGED ? "Damaged" : "Operational" , ship->target2 ? ship->target2->name : "none");
    if (ship->turret3)
        ch_printf( ch, "&zTurret Three:&w %s  &zCurrent Target:&w %s\n\r", ship->statet3 == LASER_DAMAGED ? "Damaged" : "Operational" , ship->target3 ? ship->target3->name : "none");
    if (ship->turret4)
        ch_printf( ch, "&zTurret Four: &w %s  &zCurrent Target:&w %s\n\r", ship->statet4 == LASER_DAMAGED ? "Damaged" : "Operational" , ship->target4 ? ship->target4->name : "none");
    if (ship->turret5)
        ch_printf( ch, "&zTurret Five: &w %s  &zCurrent Target:&w %s\n\r", ship->statet5 == LASER_DAMAGED ? "Damaged" : "Operational" , ship->target5 ? ship->target5->name : "none");
    if (ship->turret6)
        ch_printf( ch, "&zTurret Six:  &w %s  &zCurrent Target:&w %s\n\r", ship->statet6 == LASER_DAMAGED ? "Damaged" : "Operational" , ship->target6 ? ship->target6->name : "none");
    if (ship->turret7)
        ch_printf( ch, "&zTurret Seven:&w %s  &zCurrent Target:&w %s\n\r", ship->statet7 == LASER_DAMAGED ? "Damaged" : "Operational" , ship->target7 ? ship->target7->name : "none");
    if (ship->turret8)
        ch_printf( ch, "&zTurret Eight:&w %s  &zCurrent Target:&w %s\n\r", ship->statet8 == LASER_DAMAGED ? "Damaged" : "Operational" , ship->target8 ? ship->target8->name : "none");
    if (ship->turret9)
        ch_printf( ch, "&zTurret Nine: &w %s  &zCurrent Target:&w %s\n\r", ship->statet9 == LASER_DAMAGED ? "Damaged" : "Operational" , ship->target9 ? ship->target9->name : "none");
    if (ship->turret10)
        ch_printf( ch, "&zTurret Ten:  &w %s  &zCurrent Target:&w %s\n\r", ship->statet10 == LASER_DAMAGED ? "Damaged" : "Operational" , ship->target10 ? ship->target10->name : "none");
	ch->printlnf(  "\n\r&w          | &zMissile launcher:&w %-15s   &zPayload: &w%d&W/%d\n\r", !ship->maxmissiles ? "&RNot installed" :
		ship->missilestate == LASER_DAMAGED ? "&ROffline" : "&GOnline",
		ship->missiles, ship->maxmissiles);
	ch->printlnf(  " %-10s&w | &zTorpedo launcher:&w %-15s   &zPayload: &w%d&W/%d\n\r",
		ship->warheadLinked == true ? "&GLinked" : "&RUnlinked",
		!ship->maxtorpedos ? "&RNot installed" :
		ship->torpedostate == LASER_DAMAGED ? "&ROffline" : "&GOnline",
		ship->torpedos, ship->maxtorpedos);
	ch->printlnf(  "          &w| &zRocket launcher: &w %-15s   &zPayload: &w%d&W/%d\n\r",
		!ship->maxrockets ? "&RNot installed" :
		ship->rocketstate == LASER_DAMAGED ? "&ROffline" : "&GOnline",
		ship->rockets, ship->maxrockets);
	if(ship->maxbombs > 0) ch->printlnf(  "&zBomb Payload: &w%d&W/%d\n\r", ship->bombs, ship->maxbombs);
}

void do_transmit_broadcast( CHAR_DATA *ch, char *argument)
/* Coded by Trillen and Tawnos of SW:FotE for slave circuit modules */
/* Allows player to send a message over his ship, in a one-way shiptalk *//*
{
   SHIP_DATA *ship;
   int newloc;
   ROOM_INDEX_DATA *original;
   CHAR_DATA *wch;
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   bool ch_comlink;
   OBJ_DATA *obj;

   argument = one_argument(argument, arg1);

   if ( !arg1 || arg1[0] == '\0')
   {

          ch->println( "Usage: transmit_broadcast <ship> <message>\n\r", ch );
          return;
   }

   if ( !arg2 || arg2[0] == '\0')
   {

          ch->println( "Usage: transmit_broadcast <ship> <message>\n\r", ch );
          return;
   }

	   for(ship=first_ship; ship; ship=ship->next)
	    if(!str_prefix(arg1, ship->name)) break;

		if  (!ship)
	     {
		ch->println( "Error: That ship does not exist.\n\r", ch );
		return;
	     }

        if (ship->slave < 4)
	     {
	       ch->println( "Error: That ship does not have a class 4 slave circuit.\n\r", ch);
	       return;
	     }

    if ( !check_pilot( ch , ship ) )
     {
        ch->println("&RHey, thats not your ship!\n\r",ch);
        return;
     }

	  if (!IS_IMMORTAL(ch) )
	  {
	    ch_comlink = false;

	    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	    {
	      if (obj->pIndexData->item_type == ITEM_COMLINK)
        ch_comlink = true;
	    }
	  }

          if (IS_IMMORTAL(ch) )
          {
            ch_comlink = true;
          }

	    if ( !ch_comlink )
		 {
		   ch->println( "You need a comlink to do that.\n\r", ch);
		    return;
		}

	 if (!IS_NPC(ch) && IS_SET(ch->pcdata->act2, ACT_GAGGED))
	 {
		    ch->println( "You can't send a message when you're gagged!\n\r", ch);
		    return;
	 }

		if (!IS_NPC(ch)
		&& ( IS_SET(ch->act, PLR_SILENCE) ))
		{
		ch->println( "You can't send a message when you're silenced!\n\r", ch );
		return;
		}

		if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
		{
		ch->println( "Your comlink will not function here.\n\r", ch );
		return;
		}

    newloc = ship->cockpit;
    original = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, get_room_index(newloc) );
    talk_channel( ch, argument, CHANNEL_SHIP, "shiptalk" );
    for ( wch = first_char; wch; wch = wch->next )
    {
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original );
	    break;
	}
    }

    return;
}

/*
 * Split-S - a pilot maneuver snippet for the SWFotE codebase.
 * Copyright (c) 2003 by Nuance. All rights reserved... I guess.
 * This code may be freely used and distributed, as long as this
 * header is included and unmodified. Feel free to modify the
 * code however, improvements are welcome.
 *//*
void do_split_s(CHAR_DATA *ch, char *argument)
{
    SHIP_DATA *ship;
    char buf[MAX_STRING_LENGTH];
    int chance;
    int hx, hy, hz;

 if( (ship = ship_from_cockpit(ch->in_room->vnum)) == NULL )
 {
  ch->println("&RYou must be in the cockpit of a ship to perform a maneuver.\n\r", ch);
  return;
 }

 if(ship->manuever < 85)
 {
  ch->println("This ship isn't maneuverable enough to perform a split-S.\n\r", ch);
  return;
 }

 if(ship->shipstate == SHIP_HYPERSPACE)
 {
  ch->println("&RYou can only do that in realspace.\n\r",ch);
  return;
 }

 if(!ship->starsystem)
 {
  ch->println("&RYou can only do that in realspace.\n\r",ch);
  return;
 } // Bunch of flashy bullshit that doesn't do anything, but it's just practice

 if(!ship->target0)
 {
	chance = IS_NPC(ch) ? ch->top_level*2 : ch->pcdata->learned[gsn_split_s]-5;
	if( number_percent() > chance )
	{
		ch->println("&wYou attempt to do a split-S...\n\r", ch);
		 hx = ship->hx;
		 hy = ship->hy;
		 hz = ship->hz;
		ship->hx = number_range(-2000, 3000);
		ship->hy = number_range(-2000, 3000);
		ship->hz = number_range(-2000, 3000);
		if(ship->hx != hx && ship->hy != hy && ship->hz != hz)
		{
		  ch->println("&G...and just barely pull it off.\n\r", ch);
		  act(AT_PLAIN, "$n performs a split-S.", ch, NULL, NULL, TO_ROOM);
		  sprintf(buf, "%s quickly rolls down and around.", ship->name);
		  echo_to_system(AT_YELLOW, ship, buf, NULL);
		  learn_from_success(ch, gsn_split_s);
		  WAIT_STATE(ch, 8);
		}
		else
		{
		  ch->println("&R...but end up in an odd position.\n\r", ch);
	      act(AT_PLAIN, "$n wildly spins the ship around.", ch, NULL, NULL, TO_ROOM);
	      sprintf(buf, "%s wildly spins around.", ship->name);
	      echo_to_system(AT_YELLOW, ship, buf, NULL);
	      learn_from_failure(ch, gsn_split_s);
		  WAIT_STATE( ch , 8 );
		}
		return; // End chance failure
	}
    else
	{
		ch->printlnf(  "%s",
		  ship->rolling == true ? "You stop rolling, and perform a split-S.\n\r" :
		  ship->juking == true ? "You stop juking, and perform a split-S.\n\r" :
		  "You perform a split-S.\n\r");
		ship->rolling = ship->juking = false;
		ship->hx = number_range(-2000, 3000);
		ship->hy = number_range(-2000, 3000);
		ship->hz = number_range(-2000, 3000);
		act(AT_PLAIN, "$n performs a split-S.", ch, NULL, NULL, TO_ROOM);
		sprintf(buf, "%s quickly rolls down and around.",
				ship->name);
		echo_to_system(AT_YELLOW, ship, buf, NULL);
		learn_from_success(ch, gsn_split_s);
		WAIT_STATE(ch, 8);
		return;
	}
 }
 // Target.
  chance = IS_NPC(ch) ? ch->top_level*2 : ch->pcdata->learned[gsn_split_s];
  if(number_percent() > chance)
  {
	ch->printlnf(  "&wYou attempt to do a split-S%s",
			ship->rolling == true ? ", breaking out of the roll...\n\r" :
			ship->juking == true ? ", discontinuing the juke...\n\r" : "...\n\r" );
	ship->rolling = ship->juking = false;
	ship->hx = number_range(-2000, 3000);
	ship->hy = number_range(-2000, 3000);
	ship->hz = number_range(-2000, 3000);

	ch->println("&R...but end up in an odd position.\n\r", ch);
	act(AT_PLAIN, "$n wildly spins the ship around.", ch, NULL, NULL, TO_ROOM);
	sprintf(buf, "%s wildly spins around.", ship->name);
	echo_to_system(AT_YELLOW, ship, buf, NULL);
	learn_from_failure(ch, gsn_split_s);
	WAIT_STATE( ch , 8 );
	return; // End chance failure
  } // Success
 if(!is_facing(ship, ship->target0)) {
   ch->printlnf(  "&GYou perform a split-S, and face %s.\n\r",
		ship->target0->name);
   sprintf(buf, "%s quickly rolls down and around, facing %s.",
		ship->name, ship->target0->name);
  ship->hx = ship->target0->vx - ship->vx;
  ship->hy = ship->target0->vy - ship->vy;
  ship->hz = ship->target0->vz - ship->vz;
 }
 else {
   ch->printlnf(  "&GYou perform a split-S, turning away from %s.\n\r",
		ship->target0->name);
   sprintf(buf, "%s quickly rolls down and around, turning away from %s.",
		ship->name, ship->target0->name);
  ship->hx = 0-ship->target0->vx - ship->vx;
  ship->hy = 0-ship->target0->vy - ship->vy;
  ship->hz = 0-ship->target0->vz - ship->vz;
 }
   act(AT_PLAIN, "$n performs a split-S.", ch, NULL, NULL, TO_ROOM);
   echo_to_system(AT_YELLOW, ship, buf, NULL);
   learn_from_success(ch, gsn_split_s);
   WAIT_STATE(ch, 8);
   return;
}
/*
#ifdef USECARGO
void do_unload_cargo( CHAR_DATA *ch, char *argument)
{
   SHIP_DATA *ship;
   SHIP_DATA *target;
   int cost;
   PLANET_DATA  *planet;

   if ( argument[0] == '\0' )
   {
      act( AT_PLAIN, "Which ship do you want to unload?.", ch, NULL, NULL,
TO_CHAR);
      return;
   }

   target = ship_in_room( ch->in_room , argument );

   if ( !target )
   {
      act( AT_PLAIN, "I see no $T here.", ch, NULL, argument, TO_CHAR );
      return;
   }
   if (!check_pilot(ch, target))
   {
      ch->println("Hey, that's not your ship!\r\n", ch);
      return;
   }

   if ( target->cargo == 0 )
   {
      ch->println("You don't have any cargo.\r\n",ch);
      return;
   }

   if ( !IS_SET( ch->in_room->room_flags2, ROOM_IMPORT )
        && !IS_SET( ch->in_room->room_flags, ROOM_SPACECRAFT ))
   {
      ch->println("You can't do that here!", ch);
      return;
   }
   planet = ch->in_room->area->planet;

   if (!planet)
   {
      ship = ship_from_hanger( ch->in_room->vnum );

      if (!ship)
      {
         ch->println("You can't do that here!", ch);
         return;
      }
      if ((ship->maxcargo - ship->cargo) < 1)
      {
         ch->println("There is no room for anymore cargo\r\n",ch);
         return;
      }
      if (ship->cargo == 0)
          ship->cargotype = CARGO_NONE;

      if ((ship->cargo > 0) && (ship->cargotype != target->cargo))
      {
         ch->println("They have a differnt type of cargo.\n\r",ch);
         return;
      }
      if (ship->cargotype == CARGO_NONE)
         ship->cargotype = target->cargotype;
      if ((ship->maxcargo - ship->cargo) >= target->cargo)
      {
         ship->cargo += target->cargo;
         target->cargo = 0;
         target->cargo = CARGO_NONE;
          ch->println("Cargo unloaded.\r\n",ch);
         return;
      }
      else
      {
         target->cargo -= ship->maxcargo - ship->cargo;
         ship->cargo = ship->maxcargo;
         ch->printlnf(  "%s Loaded, %d tons still in %s hold.\r\n",
ship->name, target->cargo, target->name);
         return;
      }
   }
   if (planet->import[target->cargotype] < 1)
   {
      ch->println("You can't deliver that here.\r\n",ch);
      return;
   }
   cost = target->cargo;
   cost *= planet->import[target->cargotype];

   ch->gold += cost;
   target->cargo = 0;
   ch->printlnf( "You recieve %d credits for a load of %s.\r\n", cost,
cargo_names[target->cargotype]);
   target->cargotype = CARGO_NONE;
   return;
}

void do_load_cargo( CHAR_DATA *ch, char *argument)
{
   SHIP_DATA *ship;
   SHIP_DATA *target;
   int cost,cargo, i;
   PLANET_DATA  *planet;
   char arg1[MAX_INPUT_LENGTH];

   argument = one_argument(argument, arg1);

   if ( arg1[0] == '\0' )
   {
      act( AT_PLAIN, "Which ship do you want to load?.", ch, NULL, NULL,
TO_CHAR);
      return;
   }

   target = ship_in_room( ch->in_room , arg1 );

   if ( !target )
   {
      act( AT_PLAIN, "I see no $T here.", ch, NULL, argument, TO_CHAR );
      return;
   }

   if (!check_pilot(ch, target))
   {
      ch->println("Hey, that's not your ship!\r\n", ch);
      return;
   }

   if ( !IS_SET( ch->in_room->room_flags2, ROOM_IMPORT )
        && !IS_SET( ch->in_room->room_flags, ROOM_SPACECRAFT ))
   {
      ch->println("You can't do that here!", ch);
      return;
   }
   planet = ch->in_room->area->planet;

   if (!planet)
   {
      ship = ship_from_hanger( ch->in_room->vnum );
      if (!ship)
      {
         ch->println("You can't do that here!", ch);
         return;
      }
      if (ship->cargo == 0)
      {
         ch->println("They don't have any cargo\n\r", ch);
         return;
      }
      if ((target->maxcargo - target->cargo) < 1)
      {
         ch->println("There is no room for anymore cargo\r\n",ch);
         return;
      }
      if ((target->cargotype =! CARGO_NONE) && (ship->cargotype !=
target->cargotype));
      {
         ch->println("Maybe you should deliver your cargo first.\n\r",ch);
         return;
      }
      if (target->cargotype == CARGO_NONE)
          target->cargotype = ship->cargotype;

      if ((target->maxcargo - target->cargo) >= ship->cargo)
      {
         target->cargo += ship->cargo;
         ship->cargo = 0;
         ch->println("Cargo loaded.\r\n",ch);
         return;
      }
      else
      {
         ship->cargo -= target->maxcargo - target->cargo;
         target->cargo = target->maxcargo;
         ch->println("Cargo Loaded.\r\n",ch);
         return;
      }

   }
   if (argument[0] == '\0')
   {
      ch->println("&RWhat do you wnat to load&C&w\r\n", ch);
      return;
   }
   if (target->maxcargo - target->cargo <= 0)
   {
      ch->println("There is no room for more Cargo.\r\n", ch);
      return;
   }
   for(i = 1; i < CARGO_MAX; i++)
   {
      if(!strcmp(argument, cargo_names[i]))
         cargo = i;
   }

   if ((target->cargo > 0) && (target->cargotype != cargo))
   {
      ch->println("Maybe you should deliver your cargo first\r\n",ch);
      return;
   }

   if (planet->export[cargo] < 1)
   {
      ch->println("We don't export those goods here\r\n", ch);
      return;
   }

   if (planet->resource[cargo] < 1)
   {
      ch->println("&RSorry we have none left to ship\r\n", ch);
      return;
   }

   if ((target->maxcargo - target->cargo) >= planet->resource[cargo])
      cost = planet->resource[cargo];
   else
      cost = (target->maxcargo - target->cargo);

   cost *= planet->export[cargo];

   if (ch->gold < cost)
   {
      ch->println("You can't afford it!\r\n", ch);
      return;
   }
   ch->gold -= cost;

   if ((target->maxcargo - target->cargo) >= planet->resource[cargo])
   {

     target->cargo += planet->resource[cargo];
     planet->resource[cargo] = 0;
     target->cargotype = cargo;
   }
   else
   {
     planet->resource[cargo] -= target->maxcargo - target->cargo;
     target->cargo = target->maxcargo;
     target->cargotype = cargo;
   }

   ch->printlnf( "You pay %d credits for a load of %s.\r\n", cost, cargo_names[cargo]);
   return;
}
*/




/********************************************************************************/
/********************************************************************************/
/**************************************************************************/
// semilocalized globals
VEHICLE_DATA	*vehicle_list;
/**************************************************************************/
// create vehicle GIO lookup table

GIO_START(VEHICLE_DATA)
GIO_STRH(name,		        "Name         ")
GIO_INTH(reset,		        "Reset ")
GIO_INTH(vnum,		        "Vnum ")
GIO_INTH(type,		        "Type ")
GIO_INTH(model,		        "Model ")
GIO_INTH(ship_class,		"Class ")
GIO_INTH(shipstate,		    "ShipState ")
GIO_INTH(laserstate,		"LaserState ")
GIO_INTH(lasers,		    "Lasers ")
GIO_INTH(missilestate,		"MissileState ")
GIO_INTH(maxmissiles,		"MaxMissiles ")
GIO_INTH(missiles,		    "Missiles ")
GIO_BOOLH(hatchopen,        "HatchOpen")
GIO_BOOLH(autorecharge,     "AutoRecharge")
GIO_INTH(maxenergy,         "MaxEnergy")
GIO_INTH(energy,            "Energy")
GIO_INTH(shield,            "Shield")
GIO_INTH(maxshield,         "MaxShield")
GIO_INTH(hull,              "Hull")
GIO_INTH(maxhull,           "MaxHull")
GIO_INTH(location,          "Location")
GIO_STRH(description,		"Description  " )
GIO_STRH(short_d,			"Short ")
GIO_STRH(long_d,			"Long  ")

GIO_FINISH

//GIO_FINISH_STRDUP_EMPTY
/**************************************************************************/
// loads in the vehicle database
void load_vehicle_db(void)
{
	VEHICLE_DATA *node;
	OBJ_DATA *obj;
	room_index_data *room=NULL;

	logf("===Loading vehicle database from %s...", VEHICLE_FILE);
	GIOLOAD_LIST(vehicle_list, VEHICLE_DATA, VEHICLE_FILE);

	for (node = vehicle_list; node; node= node->next)
	{
		room = get_room_index(node->reset);
		if(room)
		{
			obj = create_object(get_obj_index(node->vnum));
			obj->name = node->name;
			obj->short_descr = node->short_d;
			obj->long_descr = node->long_d;
			obj->description = node->description;
			node->location = room->vnum;
			obj_to_room(obj, room);
		}
	}
	log_string ("load_vehicle_db(): finished");
}
/**************************************************************************/
// saves the vehicle database
void save_vehicle_db( void)
{
	logf("===save_vehicle_db(): saving vehicle database to %s", VEHICLE_FILE);
	GIOSAVE_LIST(vehicle_list, VEHICLE_DATA, VEHICLE_FILE, true);
}
/**************************************************************************/
/**************************************************************************/
// do func so it can be used as a command
void do_vsave( char_data *ch, char * )
{
	save_vehicle_db();
	ch->println("Vehicles saved...");
	logf( "do_vsave(): manual save of vehicles..." );
}


/**************************************************************************/
VEHICLE_DATA *vehicle_lookup( char *name )
{
    VEHICLE_DATA *pVehicle;

    // first try an exact match
	for(pVehicle=vehicle_list;pVehicle;pVehicle=pVehicle->next){
		if(!str_cmp(name,pVehicle->name)){
			return pVehicle;
		}
	}

	// now try a prefix match
	for(pVehicle=vehicle_list;pVehicle;pVehicle=pVehicle->next){
		if(!str_prefix(name,pVehicle->name)){
			return pVehicle;
		}
	}

    if ( is_number( name ) )
	{
	int vin;
		vin = atoi( name );
		for(pVehicle=vehicle_list;pVehicle;pVehicle=pVehicle->next){
		if(vin == pVehicle->vnum){
			return pVehicle;
		}
	}
	}


	// not found
    return NULL;
}
/**************************************************************************/
// OLC section
bool vehicle_create( char_data *ch, char *newName )
{
	VEHICLE_DATA	*node;
	static VEHICLE_DATA zero_node;

    if (IS_NULLSTR(newName)){
    ch->println("Give it a name first..");
    return false;
    }
	node	   = new VEHICLE_DATA;
	*node	   = zero_node;
	node->next = vehicle_list;
	vehicle_list = node;

	vehicle_list->name			        = str_dup( newName );
  //  vehicle_list->name			        = str_dup( newName );
    vehicle_list->owner			        = str_dup( "" );
	vehicle_list->pilot			        = str_dup( "" );
	vehicle_list->copilot		        = str_dup( "" );
    vehicle_list->type   		        = 0;
    vehicle_list->model   		        = 0;
    vehicle_list->ship_class		    = 0;
    vehicle_list->shipstate 		    = 0;
    vehicle_list->laserstate		    = 0;
    vehicle_list->lasers   	    	    = 0;
    vehicle_list->missilestate		    = 0;
    vehicle_list->missiles   		    = 0;
    vehicle_list->maxmissiles   		= 0;
    vehicle_list->hatchopen   		    = false;
    vehicle_list->autorecharge 		    = false;

	vehicle_list->short_d		    = str_dup( "" );
    vehicle_list->long_d		    = str_dup( "" );
	vehicle_list->description		= str_dup( "" );
    vehicle_list->energy       		= 0;
    vehicle_list->maxenergy    		= 0;
    vehicle_list->hull       		= 0;
    vehicle_list->maxhull      		= 0;
    vehicle_list->shield       		= 0;
    vehicle_list->maxshield    		= 0;

    vehicle_list->location     		= 0;
    vehicle_list->vnum      		= 0;
    if ( is_number( newName ) ){
    int vin = atoi(newName);
    vehicle_list->vnum      		= vin;
    }
    vehicle_list->reset     		= 0;


	ch->desc->pEdit		= (void *)vehicle_list;
	ch->desc->editor	= ED_VEHICLE;
	ch->println("Vehicle Created.");
	return false;
}

/**************************************************************************

void do_vedit( char_data *ch, char *argument )
{
	VEHICLE_DATA	*pVehicle;
	char		arg[MIL];
	int         vin;

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


	argument = one_argument( argument, arg );

	if (IS_NULLSTR(arg)){
		ch->titlebar("VEDIT: SYNTAX");
		ch->println("syntax: vedit <vehicle>");
		ch->println("        vedit `=Ccreate`x to make a new vehicle.");
		ch->println("        vedit `=Clist`x lists existing vehicle.");
		return;
	}


	if ( !str_cmp( arg, "list" ))
	{
		VEHICLE_DATA	*pV;
		int			col = 0;

		ch->titlebar("Existing Vehicles");

		for( pV=vehicle_list;pV;pV=pV->next)
		{
            ch->printf(" (%d) %-40s", pV->vnum, pV->name );
			if ( ++col % 2 == 0 )
				ch->println("");
		}
		ch->println("`x");
		return;
	}

	if ( !str_cmp( arg, "create" ))
	{
		one_argument( argument, arg );
		vehicle_create( ch, arg );
		return;
	}

	pVehicle = vehicle_lookup( arg );

    if ( !pVehicle )
    {
        ch->printlnf("There is no vehicle named '`Y%s`x'.", arg);
        return;
    }

	ch->desc->pEdit = (void *)pVehicle;
	ch->desc->editor = ED_VEHICLE;
	ch->printlnf("Editing properties '%s'", pVehicle->name );
	return;
}*/
void do_vedit( char_data *ch, char *argument )
{
	VEHICLE_DATA *pVehicle;
	AREA_DATA *pArea;
	char arg[MSL];
	int vin;

	// do security checks
	if (!HAS_SECURITY(ch, 8))
	{
		ch->println( "You must have an olc security 8 or higher to use this command." );
		return;
	}

	argument = one_argument( argument, arg );

	if ( is_number( arg ) )
	{
		vin = atoi( arg );
			pVehicle = vehicle_lookup( arg );

		if ( !( pVehicle))
		{
			ch->println( "VEdit:  That vin does not exist." );
			return;
		}


		ch->desc->pEdit = (void *)pVehicle;
		ch->desc->editor = ED_VEHICLE;

		ch->wraplnf( "`=rYou are now editing vehicle: '`r%s`=r' VIN: `Y%d`x",
			pVehicle->short_d, pVehicle->vnum);
		ch->println( "`=rType `=Cdone`=r to finish editing." );

		return;
	}
	else
	{
		if ( !str_cmp( arg, "create" ) )
		{
		one_argument( argument, arg );
		vehicle_create( ch, arg );
		return;
		}
	else if ( !str_cmp( arg, "list" )){
		VEHICLE_DATA	*pV;
		int			col = 0;

		ch->titlebar("Existing Vehicles");

		for( pV=vehicle_list;pV;pV=pV->next)
		{
            ch->printf(" (%d) %-40s", pV->vnum, pV->name );
			if ( ++col % 2 == 0 )
				ch->println("");
		}
		ch->println("`x");
		return;
        }
	}

	ch->wrapln( "VEdit:  Type the vin of the vehicle you want to edit or 'vedit create <vin>/<name>" );
	return;
}
/**************************************************************************/
bool vedit_show(char_data *ch, char *)
{
	VEHICLE_DATA *pV;

    EDIT_VEHICLE(ch, pV);

	if(IS_NULLSTR( pV->name )) {ch->println("NULL VEHICLE?!?");return false;}

	ch->printlnf("`=rName:  `x%-20s   `=r", pV->name );
	ch->printlnf( "`=rDock: `x%-5d", pV->reset);

/*	ch->printlnf( "`=rOwner: `x%-5d", pV->owner);
    ch->printlnf( "`=rPilot: `x%-5d", pV->pilot);
    ch->printlnf( "`=rCoPilot: `x%-5d", pV->copilot);*/
    ch->printlnf( "`=rShip Type: `x%-5d", pV->type);
    ch->printlnf( "`=rShip Model: `x%-5d", pV->model);
    ch->printlnf( "`=rShip Class: `x%-5d", pV->ship_class);
//    ch->printlnf( "`=rShip Type: `x%-5d", pV->type);

    ch->printlnf( "`=rMax Missiles: `x%-5d", pV->maxmissiles);
    ch->printlnf( "`=rMax Energy: `x%-5d", pV->maxenergy);
    ch->printlnf( "`=rMax Shield: `x%-5d", pV->maxshield);
    ch->printlnf( "`=rMax Hull: `x%-5d", pV->maxhull);
    ch->printlnf( "`=rLocation: `x%-5d", pV->location);


	ch->wraplnf("`=rDescription: `x%s",	pV->description );
	ch->printlnf( "`=rShort desc: `=x%s`x", pV->short_d);
        if (has_color(pV->short_d))
        {
		ch->print( "`sShort desc: `x" );
		ch->printlnbw(pV->short_d);
        }
    ch->printlnf( "`=rLong descr: `=x%s`x", pV->long_d);
        if (has_color(pV->long_d))
        {
		ch->print( "`sLong descr: `x" );
		ch->printlnbw(pV->long_d);
        }


//    mxp_display_olc_flags(ch, element_flags,	pM->element,"element", "Element:");

	return false;
}

/**************************************************************************/
bool vedit_descr(char_data *ch, char *argument)
{
	VEHICLE_DATA *pV;
	EDIT_VEHICLE( ch, pV );

	if(!IS_NULLSTR(argument)){
		ch->println("Syntax:  description (string editor used)");
		ch->println("uses the string editor to edit/add the vehicle description.");
		return false;
	}
	string_append( ch, &pV->description);
	return true;
}
/**************************************************************************/
bool vedit_long(char_data *ch, char *argument)
{
	VEHICLE_DATA *pV;

	EDIT_VEHICLE(ch, pV);

    // trim the spaces to the right of the long
	argument=rtrim_string(argument);

    if ( IS_NULLSTR(argument) )
    {
		ch->println( "Syntax: long <string>" );
		ch->println( "  (this is the description of the vehicle seen in the room when you type look.)" );
		ch->println( "  e.g. A vehicle is here." );
		return false;
	}

	char *ptemp=pV->long_d;
	pV->long_d = str_dup( argument );
	pV->long_d[0] = UPPER( pV->long_d[0] );
	ch->printlnf( "Long description from '%s' to '%s'.",
		ptemp, pV->long_d);

    free_string( ptemp );

    return true;

}
/**************************************************************************/
bool vedit_short(char_data *ch, char *argument)
{
	VEHICLE_DATA *pV;

	EDIT_VEHICLE(ch, pV);

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
			pV->short_d, argument);
		replace_string(pV->short_d, argument);
	}else{
		ch->printlnf( "Changed vehicle short description from '%s' to '%s'.",
			pV->short_d, lowercase(argument));
		replace_string(pV->short_d, lowercase(argument));
	}
    return true;
}
/**************************************************************************/
bool vedit_name(char_data *ch, char *argument)
{
	VEHICLE_DATA		*pV;

	EDIT_VEHICLE( ch, pV );

    if ( argument[0] == '\0' )
    {
		ch->println("Syntax:   name [name]");
		return false;
    }
    VEHICLE_DATA *pVcheck;
    pVcheck = vehicle_lookup( argument );
    if ( pVcheck )
    {
        ch->printlnf("There is already a ship named '`Y%s`x'.", argument);
        return false;
    }

    free_string( pV->name );
    pV->name = str_dup( argument );
	pV->name[0] = UPPER( pV->name[0] );

    ch->println("Vehicle name set.");

    return true;
}
/**************************************************************************/
bool vedit_maxmissiles(char_data *ch, char *argument)
{
	VEHICLE_DATA *pV;
	EDIT_VEHICLE( ch, pV );
	int value;

	if ( argument[0] == '\0' || !is_number( argument ) ){
        ch->println("Syntax:  maxmissiles [number]");
        return false;}

	value = atoi( argument );

	if (value<0){
        ch->println("Value must be at least 0 you idiot.");
        return false;}

	if (value>200){
        ch->println("Value can not be over 200.");
        return false;}


    pV->maxmissiles = value;
    ch->printlnf("Missile Armament Maximum set to %d.", pV->maxmissiles );
    return true;
}
/**************************************************************************/
bool vedit_maxenergy(char_data *ch, char *argument)
{
	VEHICLE_DATA *pV;
	EDIT_VEHICLE( ch, pV );
	int value;

	if ( argument[0] == '\0' || !is_number( argument ) ){
        ch->println("Syntax:  maxenergy [number]");
        return false;}

	value = atoi( argument );

	if (value<2500){
        ch->println("Value must be at least 2500.");
        return false;}

	if (value>35000){
        ch->println("Value can not be over 35000.");
        return false;}


    pV->maxenergy = value;
    ch->printlnf("Energy Maximum set to %d.", pV->maxenergy );
    return true;
}
/**************************************************************************/
bool vedit_maxshield(char_data *ch, char *argument)
{
	VEHICLE_DATA *pV;
	EDIT_VEHICLE( ch, pV );
	int value;

	if ( argument[0] == '\0' || !is_number( argument ) ){
        ch->println("Syntax:  maxshield [number]");
        return false;}

	value = atoi( argument );

	if (value<0){
        ch->println("Value must be at least 0 you idiot.");
        return false;}

	if (value>1000){
        ch->println("Value can not be over 1000.");
        return false;}


    pV->maxshield = value;
    ch->printlnf("Shield Maximum set to %d.", pV->maxshield );
    return true;
}
/**************************************************************************/
bool vedit_maxhull(char_data *ch, char *argument)
{
	VEHICLE_DATA *pV;
	EDIT_VEHICLE( ch, pV );
	int value;

	if ( argument[0] == '\0' || !is_number( argument ) ){
        ch->println("Syntax:  maxarmor [number]");
        return false;}

	value = atoi( argument );

	if (value<1){
        ch->println("Value must be at least 1 you idiot.");
        return false;}

	if (value>30000){
        ch->println("Value can not be over 30000.");
        return false;}

    pV->maxhull = value;
    ch->printlnf("Vehicle Armor Maximum set to %d.", pV->maxhull );
    return true;
}
/**************************************************************************/



/**************************************************************************/
/*               OLD VEHICLE SYSTEM BELOW                                 */
/**************************************************************************/
// lists vehicles
void do_vehiclelist( char_data *ch, char *)
{
	VEHICLE_DATA *node;
	int count;

	ch->titlebar("-=VEHICLE RESETS=-");

	count=0;
	for (node = vehicle_list; node; node= node->next)
	{
	 	ch->printlnf("`s%d> `MName: `m%s  `MRoom: `m%d  `MVnum: `m%d", ++count, node->name, node->reset, node->vnum);
	}
	return;
}
/**************************************************************************************/
void queue_vehicle(char *name, int reset, int vnum)
{
	VEHICLE_DATA *node = new VEHICLE_DATA;
	node->name      = name;
	node->reset     = reset;
	node->vnum      = vnum;
/*	node->maxenergy = maxenergy;
	node->energy    = energy;
	node->shield    = shield;
	node->maxshield = maxshield;
	node->hull      = hull;
	node->maxhull   = maxhull;
	node->location  = location;
*/
	node->next = vehicle_list;
	vehicle_list = node;
	save_vehicle_db();
}
/**************************************************************************/
EXTRA_DESCR_DATA *ed_prefix(EXTRA_DESCR_DATA *ed, char *keyword, char *description);
/**************************************************************************/
/**************************************************************************
void do_vehicle_add(char_data *ch, char *argument)
{
    OBJ_DATA *obj = NULL;
    char arg1[MIL];

    argument = one_argument( argument, arg1 );

    obj = get_obj_list( ch, arg1, ch->in_room->contents );

    if (obj == NULL )
    {
	ch->println("No vehicle here to add to database.");
	return;
    }

    if ( obj->item_type!=ITEM_VEHICLE)
    {
	ch->println("That is not a vehicle.");
	return;
    }

    queue_vehicle(obj->short_descr, ch->in_room->vnum, obj->pIndexData->vnum);
    ch->printlnf("%s `W[`S%d`W] `wadded in room %d.",obj->short_descr, obj->pIndexData->vnum, ch->in_room->vnum);
    return;
}

void travelecho( char_data *ch, OBJ_DATA *vehicle )
{
   int chance = 0;
   for(char_data *vch = player_list; vch; vch = vch->next_player)
   {
	if(vch==ch)
	{
   		if(IS_IMMORTAL(vch) || IS_SET(ch->comm,COMM_BUILDING))
   		{
			vch->print("`#`STravelEcho: `^");
   		}
   		vch->print(get_extra_descr("_TRAVEL", vehicle->pIndexData->extra_descr));
   		vch->println("`x");
		continue;
	}

	if(ch->in_room->area==vch->in_room->area && vch->in_room->sector_type == SECT_VEHICLE)
	{
		chance = number_range(1,2);
		if(vch!=ch && chance==1)
		{
  			if(IS_IMMORTAL(vch) || IS_SET(vch->comm,COMM_BUILDING))
   			{
				vch->print("`#`STravelEcho: `^");
	   		}
	   		vch->print(get_extra_descr("_TRAVEL", vehicle->pIndexData->extra_descr));
   			vch->println("`x");
		}
	}
   }
   return;
}
/**************************************************************************/
void do_drive( char_data *ch, char *argument )
{
    char_data *vch;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    char arg1[MIL];
    OBJ_DATA *controls;
    OBJ_DATA *vehicle = NULL;
    int direction;
    int dir_name_2;

    for ( controls = ch->in_room->contents; controls != NULL; controls = controls->next_content )
    {
	if ( controls->item_type == ITEM_VEHICLE_CONTROLS )
	    break;
    }

    if ( controls == NULL )
    {
	ch->println("Nothing to drive here.");
	return;
    }

    if ( ( vehicle = get_obj_type( get_obj_index( controls->value[0] ) ) ) == NULL )
    {
	ch->println("Error, can't find vehicle for control panel." );
	return;
    }

    if ( vehicle->item_type != ITEM_VEHICLE )
    {
	do_huh(ch, "");
	return;
    }

    if ( vehicle->in_room == NULL )
    {
	ch->println("Error, vehicle room is null." );
	return;
    }

    in_room = vehicle->in_room;

    argument = one_argument( argument, arg1 );

    direction = get_direction( arg1 );

    if ( arg1[0] == '\0' )
	direction = -1;

    if ( direction == -1 )
    {
	ch->println("Drive where ?" );
	return;
    }

    if ( ( pexit   = in_room->exit[direction] ) == NULL
	||   ( to_room = pexit->u1.to_room   ) == NULL
	||   !can_see_room(ch,pexit->u1.to_room))
    {
	ch->println("There is no way to travel in that direction." );
	return;
    }

    if (IS_SET(pexit->exit_info, EX_ISDOOR) && to_room->sector_type!=SECT_CITY)
    {
	ch->println("You cannot drive through there." );
	return;
    }

    if (IS_SET(to_room->room_flags, ROOM_INDOORS) && !IS_SET(vehicle->value[2], VEHICLE_CAVE))
    {
	ch->println("You cannot drive indoors." );
	return;
    }

    if (IS_SET(pexit->exit_info, EX_LOCKED))
    {
	ch->println("That direction seems to be barred to you." );
	return;
    }

	if( to_room->sector_type==SECT_LAVA && !IS_SET(vehicle->value[2], VEHICLE_LAVA))
	{
		ch->println("You would burn there.");
		return;
	}

	if( to_room->sector_type==SECT_WATER_NOSWIM && !IS_SET(vehicle->value[2], VEHICLE_WATER))
	{
		ch->println("You are not in a boat.");
		return;
	}

	if( to_room->sector_type==SECT_AIR && !IS_SET(vehicle->value[2], VEHICLE_AIR))
	{
		ch->println("This vehicle cannot fly.");
		return;
	}

	if( to_room->sector_type==SECT_SPACE && !IS_SET(vehicle->value[2], VEHICLE_SPACE))
	{
		ch->println("This vehicle cannot fly through space.");
		return;
	}

	if( to_room->sector_type==SECT_CAVE && !IS_SET(vehicle->value[2], VEHICLE_CAVE))
	{
		ch->println("This vehicle cannot travel underground.");
		return;
	}

	if( to_room->sector_type==SECT_INSIDE)
	{
		ch->println("You cannot go there.");
		return;
	}

	if ( to_room->sector_type==SECT_UNDERWATER ||IS_SET( to_room->affected_by, ROOMAFF_UNDERWATER ))
	{
		ch->println("This vehicle cannot go underwater.");
		return;
	}

	if((to_room->sector_type==SECT_CITY || to_room->sector_type==SECT_FIELD ||
	    to_room->sector_type==SECT_FOREST || to_room->sector_type==SECT_HILLS ||
	    to_room->sector_type==SECT_MOUNTAIN || to_room->sector_type==SECT_SWAMP ||
	    to_room->sector_type==SECT_DESERT ||
	    to_room->sector_type==SECT_SNOW || to_room->sector_type==SECT_ICE ||
	    to_room->sector_type==SECT_TRAIL/* || to_room->sector_type==SECT_GATEWAY*/ ) &&
	    !IS_SET(vehicle->value[2], VEHICLE_LAND))
	{
		ch->println("This vehicle cannot travel on land.");
		return;
	}

    for ( vch = in_room->people; vch != NULL; vch = vch->next_in_room )
    {
	if ( can_see_obj( vch, vehicle ) )
	    vch->printlnf( "%s leaves %s.",  capitalize(vehicle->short_descr), dir_name[direction] );
    }

    obj_from_room( vehicle );
    obj_to_room( vehicle, to_room );

    dir_name_2 = rev_dir[direction];

    for ( vch = to_room->people; vch != NULL; vch = vch->next_in_room )
    {
	if ( can_see_obj( vch, vehicle ) )
	    vch->printlnf( "%s has arrived from %s.", capitalize(vehicle->short_descr), dir_name[dir_name_2] );
    }

    bool travel = false;

    if( to_room->sector_type==SECT_AIR && IS_SET(vehicle->value[2], VEHICLE_AIR) && travel==false)
    {
   	ch->printlnf( "You fly %s %s.", vehicle->short_descr, dir_name[direction]  );
	travel = true;
    }

    if( to_room->sector_type==SECT_SPACE && IS_SET(vehicle->value[2], VEHICLE_SPACE) && travel==false)
    {
   	ch->printlnf( "You fly %s %s through the darkness of space.", vehicle->short_descr, dir_name[direction]  );
	travel = true;
    }

    if (IS_SET(vehicle->value[2], VEHICLE_LAND) && to_room->sector_type==SECT_SNOW && travel==false)
    {
    	ch->printlnf( "You drive %s %s through the snow.", vehicle->short_descr, dir_name[direction]  );
	travel = true;
    }

    if (IS_SET(vehicle->value[2], VEHICLE_LAND) && to_room->sector_type==SECT_SNOW && travel==false)
    {
    	ch->printlnf( "You drive %s %s through the ice.", vehicle->short_descr, dir_name[direction]  );
	travel = true;
    }

    if (IS_SET(vehicle->value[2], VEHICLE_LAND) && to_room->sector_type!=SECT_WATER_NOSWIM && travel==false)
    {
    	ch->printlnf( "You drive %s %s.", vehicle->short_descr, dir_name[direction]  );
	travel = true;
    }

    if (IS_SET(vehicle->value[2], VEHICLE_WATER) && to_room->sector_type==SECT_WATER_NOSWIM && travel==false)
    {
	ch->printlnf( "You sail %s %s.", vehicle->short_descr, dir_name[direction]  );
	travel = true;
    }

    if (IS_SET(vehicle->value[2], VEHICLE_WATER) && to_room->sector_type==SECT_WATER_SWIM && travel==false)
    {
	ch->printlnf( "You sail %s %s through the shallow water.", vehicle->short_descr, dir_name[direction]  );
	travel = true;
    }

    if (travel==false)
    {
    	ch->printlnf( "You move %s %s.", vehicle->short_descr, dir_name[direction]  );
	travel = true;
    }

//    travelecho(ch, controls);
    in_room = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, to_room );
    do_look( ch, "" );
    char_from_room( ch );
    char_to_room( ch, in_room );

    return;
}
/**************************************************************************/
void do_board( char_data *ch, char *argument )
{
    char arg1[MIL];
    ROOM_INDEX_DATA *to_room;
    OBJ_DATA *building, *building_next = NULL;
    OBJ_DATA *vehicle = NULL;

    argument = one_argument( argument, arg1 );

    if ( IS_NULLSTR(arg1))
    {
	for ( building = ch->in_room->contents; building != NULL; building = building_next )
    	{
		building_next = building->next_content;

		if ( building->item_type!=ITEM_VEHICLE_EXIT)
			continue;
		if ( building->item_type==ITEM_VEHICLE_EXIT)
    		{
    			if ( ( vehicle = get_obj_type( get_obj_index( building->value[0] ) ) ) == NULL )
    			{
				ch->println("Error, can't find ship for vehicle exit." );
				return;
    			}
    			act("You leave $p.", ch, vehicle, NULL, TO_CHAR );
    			act("$n leaves $p.", ch, vehicle, NULL, TO_ROOM );

    			char_from_room( ch );
    			char_to_room( ch, get_room_index(vehicle->in_room->vnum));
    			do_look( ch, "" );

    			act("$n has arrived from $p.", ch, vehicle, NULL, TO_ROOM);
			return;
		}
	}
    }

    building = get_obj_list( ch, arg1, ch->in_room->contents );
    if ( building == NULL )
    {
	ch->println("Nothing like that to board here.");
	return;
    }

	if ( building->item_type==ITEM_VEHICLE_EXIT)
    	{
    		if ( ( vehicle = get_obj_type( get_obj_index( building->value[0] ) ) ) == NULL )
    		{
			ch->println("Error, can't find vehicle for this exit." );
			return;
    		}
    		act("You leave $p.", ch, vehicle, NULL, TO_CHAR );
    		act("$n leaves $p.", ch, vehicle, NULL, TO_ROOM );
    		char_from_room( ch );
		char_to_room( ch, get_room_index(vehicle->in_room->vnum));
		do_look( ch, "" );
		act("$n has arrived from $p.", ch, vehicle, NULL, TO_ROOM);
		return;
	}

    if ( building->item_type!=ITEM_VEHICLE)
    {
        ch->println("Nothing to board here." );
	return;
    }

    if ( ( to_room = get_room_index( building->value[0] ) ) == NULL )
    {
	ch->println("Error, bad room pointed to." );
	return;
    }

    act("You board $p.", ch, building, NULL, TO_CHAR );
    act("$n boards $p.", ch, building, NULL, TO_ROOM );

    char_from_room( ch );
    char_to_room( ch, to_room);
    do_look( ch, "" );

    act("$n has arrived from outside.", ch, NULL, NULL, TO_ROOM);

    return;
}
/**************************************************************************/
void do_vehicle_delete( char_data *ch, char *argument )
{
	VEHICLE_DATA *node;
	VEHICLE_DATA *prevnode=NULL;
	int target, count=1;

	if(vehicle_list==NULL){
		ch->println("There are no vehicles in the database.");
		return;
	}

	if(IS_NULLSTR(argument)){
		ch->println("syntax: vehicledelete <vehicle_number>");
		return;
	};

	if(!is_number(argument)){
		ch->print("Argument to vehicledelete must be a number.");
		return;
	}

	target=atoi(argument);

	for (node=vehicle_list;node;node=node->next, ++count){
		if(count==target)
			break;
		prevnode = node;
	}

	if(node==NULL){
		ch->printf(0, "Vehicle not found.");
		return;
	}

	ch->printf(0, "Vehicle Vnum #%d has been deleted.\r\n", node->vnum);

	if(!prevnode){ // delete the head
		vehicle_list = vehicle_list->next;
	}else{
		prevnode->next=node->next;
	}

	delete node;
	save_vehicle_db();
}
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
