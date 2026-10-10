/*************************************************************************** 
 * sg.h - Planetary Travel Gates                                           * 
 ***************************************************************************/ 

#ifndef SGEDIT_H 
#define SGEDIT_H 

DECLARE_OLC_FUN( sgedit_show      ); 
DECLARE_OLC_FUN( sgedit_vnum      ); 
DECLARE_OLC_FUN( sgedit_code      ); 

/**************************************************************************/ 
const struct olc_cmd_type sgedit_table[] = 
{ 
//   {   COMMAND         FUNCTION            } 

   {   "commands",	show_commands	}, 
   {   "show",		sgedit_show	}, 
   {   "vnum",		sgedit_vnum	}, 
   {   "code",		sgedit_code	},       
   {   "?",		show_help	}, 
   {   NULL,		0,		} 
}; 
/**************************************************************************/ 
#endif // SGEDIT_H 

