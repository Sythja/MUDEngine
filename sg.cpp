/***************************************************************************
 * sg.cpp - Planetary Travel Gates                                         *
 ***************************************************************************/
#include "include.h"
#include "tables.h"
#include "lookup.h"
#include "olc.h"
#include "gio.h"

RESET_DATA *new_reset_data args ( ( void ) );

// GIO
GIO_START( SGATE_DATA )
GIO_STRH(  code, "Code         " )
GIO_INTH(  vnum, "Vnum         " )
GIO_FINISH

sgate_type *sgate_list;

/**************************************************************************/
// load sg db
void load_sgate_db( void )
{
   logf("===Loading gate database from %s...", SGATE_FILE );
   GIOLOAD_LIST( sgate_list, SGATE_DATA, SGATE_FILE );
   log_string( "load_sgate_db(): finished" );
}
/**************************************************************************/
// save sgate list
void save_sgate_db( void )
{
   logf( "save_sgate_db(): saving gate database to %s...", SGATE_FILE );
   GIOSAVE_LIST( sgate_list, SGATE_DATA, SGATE_FILE, true );
}
/**************************************************************************/
// manual command
void do_savesgate( char_data *ch, char * )
{
   save_sgate_db();
   ch->println("Gateways saved...");
   logf( "do_savesgate(): manual save of gateways..." );
}
/**************************************************************************/
SGATE_DATA *sgate_lookup( char *name )
{
       SGATE_DATA *pGateway;

   // first try an exact match
   for(pGateway=sgate_list;pGateway;pGateway=pGateway->next){
      if(!str_cmp(name,pGateway->code)){
         return pGateway;
      }
   }

   // now try a prefix match
   for(pGateway=sgate_list;pGateway;pGateway=pGateway->next){
      if(!str_prefix(name,pGateway->code)){
         return pGateway;
      }
   }
   // not found
       return NULL;
}
/**************************************************************************/
SGATE_DATA *sgate_number( int number )
{
       SGATE_DATA *pGateway;

   // first try an exact match
   for(pGateway=sgate_list;pGateway;pGateway=pGateway->next){
      if(number == pGateway->vnum){
         return pGateway;
      }
   }
   // not found
       return NULL;
}
/**************************************************************************/
// OLC section
bool sgate_create( char_data *ch, char *newName )
{
   SGATE_DATA   *node;
   static SGATE_DATA zero_node;

   node      = new SGATE_DATA;
   *node      = zero_node;
   node->next = sgate_list;
   sgate_list = node;

   sgate_list->code         = str_dup( "new" );
   sgate_list->vnum         = 30000;
   ch->desc->pEdit         = (void *)sgate_list;
   ch->desc->editor      = ED_SGATE;
   save_sgate_db();
   ch->println("Gateway Created.");
   return false;
}
/**************************************************************************/
void do_sgedit( char_data *ch, char *argument )
{
   SGATE_DATA   *pGateway;
   char      arg[MIL];

   if ( IS_NPC( ch ))
   {
      ch->println("Players only.");
      return;
   }

   // do security checks
   if ( !HAS_SECURITY( ch, 8 ))
   {
      ch->println("Access denied. You must have an olc security 8 or higher to use this command.");
      return;
   }

   if (IS_NULLSTR(argument)){
      ch->titlebar("SGEDIT: SYNTAX");
      ch->println("syntax: sgedit <gate>");
      ch->println("        sgedit `=Ccreate`x to make a new gate.");
      ch->println("        sgedit `=Clist`x lists existing gateways.");
      return;
   }

   argument = one_argument( argument, arg );

   if ( !str_cmp( arg, "list" ))
   {
      SGATE_DATA   *pD;
      int         col = 0;

      ch->titlebar("Existing Gateways");

      for( pD=sgate_list;pD;pD=pD->next)
      {
                  ch->printf(" %-5s -> %-7d", pD->code, pD->vnum );
         if ( ++col % 2 == 0 )
            ch->println("");
      }
      ch->println("`x");
      return;
   }

   if ( !str_cmp( arg, "create" ))
   {
      one_argument( argument, arg );
      sgate_create( ch, arg );
      return;
   }

   pGateway = sgate_lookup( arg );

       if ( !pGateway )
       {
           ch->printlnf("There is no gate coded '`Y%s`x'.", arg);
           return;
       }

   ch->desc->pEdit = (void *)pGateway;
   ch->desc->editor = ED_SGATE;
   ch->printlnf("Editing properties '%s'", pGateway->code );
   return;
}
/**************************************************************************/
bool sgedit_code(char_data *ch, char *argument)
{
    SGATE_DATA      *pGateway;

    EDIT_SGATE( ch, pGateway );

    if ( argument[0] == '\0' )
    {
   ch->println("Syntax:   code <code>");
   return false;
    }
    free_string( pGateway->code );
    pGateway->code = str_dup( argument );
    ch->println("Gateway code set.");
    save_sgate_db();
    return true;
}
/**************************************************************************/
bool sgedit_show(char_data *ch, char *)
{
   SGATE_DATA *pD;

       EDIT_SGATE(ch, pD);

   if(IS_NULLSTR( pD->code)) {ch->println("NULL GATE?!?");return false;}

   ch->printlnf("`=rCode:  `x%-20s   `=r", pD->code );
   ch->printlnf("`=rVnum:  `x%-5d", pD->vnum);
   return false;
}
/**************************************************************************/
bool sgedit_vnum( char_data *ch, char *argument )
{
   SGATE_DATA   *pD;

       if ( IS_NULLSTR(argument))
       {
      ch->println("Syntax: vnum [vnum]");
      return false;
       }

   if ( !is_number( argument ))
   {
      ch->println("The vnum value must be numeric.");
      sgedit_vnum( ch, "" );      // redisplay the help
      return false;
   }

   if ( get_room_index( atoi( argument )) == '\0' )
   {
      ch->println("That room vnum doesn't exist.");
      return false;
   }

       EDIT_SGATE( ch, pD );

   pD->vnum = atoi( argument );
   ch->printf("The vnum is now set to '%d'.", pD->vnum );
   save_sgate_db();
       return true;
}
/**************************************************************************/
void do_sgateway(char_data *ch, char *argument)
{
	char arg1[MIL];
	OBJ_DATA *portal = NULL;
	OBJ_DATA *obj = NULL;
	SGATE_DATA *pGateway = NULL;

       if (ch->position == POS_SLEEPING)
       {
        ch->println("You can't do that while sleeping.");
           return;
       }

       argument = one_argument( argument, arg1);

   	obj = get_obj_list(ch, "planetary_gateway", ch->in_room->contents);

   if (obj == NULL)
   {
      ch->println("There is no Planetary Gateway here.");
      return;
   }

       if (obj->pIndexData->vnum != OBJ_VNUM_SGATE)
   {
      ch->println("There is no Planetary Gateway here.");
      return;
   }

       if ( arg1[0] == '\0' && ch->fighting == NULL)
       {
      ch->println ("Where did you want to set the Planetary Gateway to go?");
      return;
       }
       if ( arg1[1] == '\0' && ch->fighting == NULL)
       {
      ch->println ("Where did you want to set the Planetary Gateway to go?");
      return;
       }
       if ( arg1[2] == '\0' && ch->fighting == NULL)
       {
      ch->println ("Where did you want to set the Planetary Gateway to go?");
      return;
       }
       if ( arg1[3] == '\0' && ch->fighting == NULL)
       {
      ch->println ("Where did you want to set the Planetary Gateway to go?");
      return;
       }
       if ( arg1[4] == '\0' && ch->fighting == NULL)
       {
      ch->println ("Where did you want to set the Planetary Gateway to go?");
      return;
       }

      if (obj->item_type == ITEM_PORTAL)
   {
      ch->println("You must wait for the Planetary Gateway to close.");
      return;
   }

   pGateway = sgate_lookup( lowercase(arg1) );

   act("$n presses the arcaic glyphs on the Planetary Gateway.",ch,NULL,NULL,TO_ROOM);
   act("You press the arcaic glyphs on the Planetary Gateway.",ch,NULL,NULL,TO_CHAR);

   if(pGateway == NULL)
   {
      act("$n fails to activate the Planetary Gateway.",ch,NULL,NULL,TO_ROOM);
      act("You fail to activate the Planetary Gateway.",ch,NULL,NULL,TO_CHAR);
          return;
   }

   if(pGateway->vnum == ch->in_room->vnum ){

      act("The area within the Planetary Gateway shimmers a moment, but then goes back to normal.",ch,NULL,NULL,TO_ROOM);
      act("The area within the Planetary Gateway shimmers a moment, but then goes back to normal.",ch,NULL,NULL,TO_CHAR);
      return;
   }

   portal = create_object(get_obj_index(OBJ_VNUM_SGATE));
   portal->item_type = ITEM_PORTAL;
   portal->long_descr = str_dup( "" );
   SET_BIT(portal->extra_flags,OBJEXTRA_GLOW);
   SET_BIT(portal->extra_flags,OBJEXTRA_HUM);
   REMOVE_BIT(portal->extra2_flags,OBJEXTRA2_NODECAY);
   REMOVE_BIT(portal->extra_flags,OBJEXTRA_NO_DEGRADE);
   portal->condition = 2;
   portal->short_descr = str_dup( "a `Cs`Wh`Ci`Wm`Cm`We`Cr`Wi`Cn`Wg `#`ssilvery portal`^" );
   portal->name		  = str_dup( "planetary gateway gate portal" );
   portal->timer = 2;
   portal->value[0] = -1;
   SET_BIT(portal->value[2],GATE_OPAQUE);
   portal->description	= str_dup("`#`wA `Cs`Wh`Ci`Wm`Cm`We`Cr`Wi`Cn`Wg `ssilvery portal`w ripples inside the `YStellar Gateway`x.");
   portal->value[3] = pGateway->vnum;
   obj_to_room(portal,ch->in_room);
   act("$p forms inside the Planetary Gateway.",ch,portal,NULL,TO_ROOM);
   act("$p forms inside the Planetary Gateway.",ch,portal,NULL,TO_CHAR);
   return;

}
/**************************************************************************/



