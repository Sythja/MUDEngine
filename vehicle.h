// vehicle.h - Vehicle movement & Combat
/***************************************************************************
 * The Dawn of Time v1.69r (c)1997-2004 Michael Garratt                    *
 * >> A number of people have contributed to the Dawn codebase, with the   *
 *    majority of code written by Michael Garratt - www.dawnoftime.org     *
 * >> To use this source code, you must fully comply with the dawn license *
 *    in licenses.txt... In particular, you may not remove this copyright  *
 *    notice.                                                              *
 **************************************************************************/

#ifndef VEHICLE_H
#define VEHICLE_H


/**************************************************************************/
DECLARE_OLC_FUN( vedit_descr );
DECLARE_OLC_FUN( vedit_long );
DECLARE_OLC_FUN( vedit_name );
DECLARE_OLC_FUN( vedit_short );
DECLARE_OLC_FUN( vedit_show );
DECLARE_OLC_FUN( vedit_maxenergy );
DECLARE_OLC_FUN( vedit_maxhull );
DECLARE_OLC_FUN( vedit_maxmissiles );
DECLARE_OLC_FUN( vedit_maxshield );
/**************************************************************************/

const struct olc_cmd_type vedit_table[] =
{
//	{	COMMAND			    FUNCTION				}
	{	"commands",		    show_commands			},
	{   "show",             vedit_show         },
	{	"description",		vedit_descr		},
	{	"name",			    vedit_name			},
	{	"maxmissiles",		vedit_maxmissiles		},
	{	"maxenergy",		vedit_maxenergy		},
	{	"maxshield",		vedit_maxshield		},
	{	"maxarmor", 		vedit_maxhull	},
//	{	"element",			magnusedit_element		},
	{	"short",			vedit_short		},
	{	"long",			    vedit_long			},
	{	"?",			    show_help				},
	{	NULL,			    0,						}
};
/**************************************************************************/

//prototypes
void load_vehicle_db( void );
void save_vehicle_db( void );

// semilocalized globals
extern VEHICLE_DATA *vehicle_list;

#endif // VEHICLE_H


