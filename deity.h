/**************************************************************************/
// deity.h -
/***************************************************************************
 * The Dawn of Time v1.69s.beta6 (c)1997-2010 Kalahn                       *
 * >> A number of people have contributed to the Dawn codebase, with the   *
 *    majority of code written by Kalahn - www.dawnoftime.org              *
 * >> To use this source code, you must fully comply with the dawn license *
 *    in licenses.txt... In particular, you may not remove this copyright  *
 *    notice.                                                              *
 **************************************************************************/
#ifndef DEDIT_H
#define DEDIT_H

DECLARE_OLC_FUN( dedit_name					);
DECLARE_OLC_FUN( dedit_show					);
DECLARE_OLC_FUN( dedit_shrinevnum			);
DECLARE_OLC_FUN( dedit_symbol				);
DECLARE_OLC_FUN( dedit_rival				);
DECLARE_OLC_FUN( dedit_alliance				);
//DECLARE_OLC_FUN( dedit_tendency				);
DECLARE_OLC_FUN( dedit_sex					);
DECLARE_OLC_FUN( dedit_race					);
DECLARE_OLC_FUN( dedit_maxfollowers			);
DECLARE_OLC_FUN( dedit_alignflags			);
//DECLARE_OLC_FUN( dedit_tendflags			);
DECLARE_OLC_FUN( dedit_raceallow			);
DECLARE_OLC_FUN( dedit_description			);

/**************************************************************************/
const struct olc_cmd_type dedit_table[] =
{
//	{	COMMAND			FUNCTION				}
	{	"alliance",		dedit_alliance			},
	{	"commands",		show_commands			},
	{	"description",	dedit_description		},
	{   "rival",		dedit_rival				},
	{	"maxfollowers",	dedit_maxfollowers		},
	{	"name",			dedit_name				},
	{	"race",			dedit_race				},
	{	"sex",			dedit_sex				},
	{	"show",			dedit_show				},
	{	"shrinevnum",	dedit_shrinevnum		},
	{	"symbol",		dedit_symbol			},
//	{	"tendency",		dedit_tendency			},
	{	"alignflags",	dedit_alignflags		},
//	{	"tendflags",	dedit_tendflags			},
	{	"raceallow",	dedit_raceallow			},
	{	"?",			show_help				},
	{	NULL,			0,						}
};
/**************************************************************************/
DECLARE_OLC_FUN( magnusedit_name );
DECLARE_OLC_FUN( magnusedit_number );
DECLARE_OLC_FUN( magnusedit_level );
DECLARE_OLC_FUN( magnusedit_refine );
DECLARE_OLC_FUN( magnusedit_refinecost );
DECLARE_OLC_FUN( magnusedit_element );
DECLARE_OLC_FUN( magnusedit_descr );
DECLARE_OLC_FUN( magnusedit_short );
DECLARE_OLC_FUN( magnusedit_long );
DECLARE_OLC_FUN( magnusedit_show );
//DECLARE_OLC_FUN( magnusedit_create );
/**************************************************************************/
const struct olc_cmd_type magnusedit_table[] =
{
//	{	COMMAND			    FUNCTION				}
	{	"commands",		    show_commands			},
	//{   "show",             magnusedit_create         },
	{   "show",             magnusedit_show         },
	{	"description",		magnusedit_descr		},
	{	"name",			    magnusedit_name			},
	{	"number",			magnusedit_number		},
	{	"level",			magnusedit_level		},
	{	"refine",			magnusedit_refine		},
	{	"refinecost",		magnusedit_refinecost	},
	{	"element",			magnusedit_element		},
	{	"short",			magnusedit_short		},
	{	"long",			    magnusedit_long			},
	{	"?",			    show_help				},
	{	NULL,			    0,						}
};
/**************************************************************************/

#endif // DEDIT_H

