/**************************************************************************/
// mate.cpp - mate system implementation
/***************************************************************************

 **************************************************************************/
#include "mate.h"

time_t mate_times[5]=
{
	60, //MS_REQUESTED_OF
	60, // MS_WAITING_FOR_REPLY
	15, //MS_ABOUT_TO_START,
	30*60,//MS_MATE,
	-1 //DS_PROTECTED - remainding time
};

/**************************************************************************/
char *ms_state_name(mate_state st)
{
	switch (st){
	case MS_REQUESTED_BY:
		return "requested by";

	case MS_WAITING_FOR_REPLY:
		return "waiting for them to reply";

	case MS_ABOUT_TO_START:
		return "mating about to activate";

	case MS_MATING:
		return "mating";

	default:
		return "BUG - MISSING A STATE IN char *ms_state_name(mate_state st) - PLEASE REPORT!!!";
	}
}
/**************************************************************************/
static char *get_display_pkinfo_time(time_t till)
{
	static char result[MIL];

	int diff=(int)(till-current_time);
	if(diff>15){
		sprintf(result, "%d:%02d", diff/60, diff%60);
	}else{
		sprintf(result, " <15s");
	}
	return result;
}

/**************************************************************************/
// called from in update_handler() on pulse_violence
void mate_update()
{
	char_data *mch;
    for ( mch = player_list; mch; mch = mch->next_player )
    {
		if(mch->mates){
			mch->mates->update_mates(mch);
		}
	}
}
/**************************************************************************/
// called from in extract_char when someone logs out
void mate_logout(char_data *ch)
{
	char_data *mch;
    for ( mch = player_list; mch; mch = mch->next_player )
    {
		if(mch->mates){
			mch->mates->logout(mch, ch);
		}
	}
}
/**************************************************************************/
// when someone logs out  - currently players are allowed to logout
//							during a mate, victim is the person logging out
void mate_data::logout(char_data *ch, char_data *victim)
{
	node=find_char(victim);
	if(node){
		ch->printlnf("`YMate request cancelled with %s because they logged out.`x",
			PERS(victim, ch));
		node->state=MS_REMOVE;
		node->till=current_time-1;
		ch->mates->update_mates(ch);
	}
}
/**************************************************************************/

/**************************************************************************/

/**************************************************************************/
void do_acceptmate(char_data *ch, char *argument)
{
	if (IS_OOC(ch)){
		ch->println("You can't perform mate related commands in OOC.");
		return;
	}

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	if (ch->fighting){
		ch->println("AREN'T YOU BUSY ENOUGH?!?!.");
		return;
	}

	if(ch->mates){
		ch->mates->accept_mate(ch, argument);
		return;
	}

	ch->println("You have not had any mate requests recently.");
	return;
};
/**************************************************************************/
void do_declinemate(char_data *ch, char *argument)
{
	if (IS_OOC(ch)){
		ch->println("You can't perform mate related commands in OOC.");
		return;
	}
	if (ch->fighting){
		ch->println("AREN'T YOU BUSY ENOUGH?!?!");
		return;
	}

	if(ch->mates){
		ch->mates->decline_mate(ch, argument);
		return;
	}

	ch->println("You have not had any mate requests recently.");
	return;
};

/**************************************************************************/
// used for one player to request another to mate
void do_mate(char_data *ch, char *argument)
{
	char arg[MIL];
	char_data *victim;

	if (IS_OOC(ch)){
		ch->println("You can't perform mate related commands in OOC.");
		return;
	}

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

	if (ch->fighting){
		ch->println("AREN'T YOU BUSY ENOUGH?!?!");
		return;
	}

	if(!IS_LETGAINED(ch)){
		ch->println("You can't use this command unless letgained.");
		return;
	}

    one_argument( argument, arg );

    if ( IS_NULLSTR(arg))
    {
		ch->println("Whom do you wish to mate with?");
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		ch->printlnf("You can't find any '%s' in the current room.", arg);
		return;
    }

	if(victim==ch){
		ch->println("You don't need a command to masturbate.");
		return;
	}

	if(IS_NPC(victim)){
		ch->println("You can't mate NPCs.");
		return;
	}

	if(!can_see(victim,ch)){
		ch->printlnf("%s can't currently see you, you must go visible to mate with them.",
			CPERS(victim, ch));
		return;
	}

	if(victim->sex == 0 || ch->sex == 0){
		ch->println("Not gonna happen.");
		return;
	}
	if(victim->sex == ch->sex){
		ch->println("Can't mate with same sex, sorry.");
		return;
	}

	// allocate memory if required
	if(!ch->mates){
		ch->mates=new mate_data();
	}
	if(!victim->mates){
		victim->mates=new mate_data();
	}

	// checks about previous mate information
	if(ch->mates->is_known(victim)){
		if(ch->mates->has_been_requested_by(victim)){
			ch->printf("%s has already requested to mate with you,\r\n"
				"Use `=Cacceptmate`x to accept their mate.\r\n"
				"Use `=Cdeclinemate`x to decline their invitation to mate.\r\n",
				CPERS(victim, ch));
			return;
		}else if(ch->mates->is_waiting_for_reply(victim)){
			ch->printf("You have already requested to mate with %s,\r\n"
				"You must wait longer to give them a chance to respond before requesting again.\r\n",
				PERS(victim, ch));
			return;
		}
	}

	// setup mate activities on both characters
	ch->mates->update_request(victim, MS_WAITING_FOR_REPLY);
	victim->mates->update_request(ch, MS_REQUESTED_BY);

	ch->mate_requested++;

	// do the room/people echos
    act( "***$n has requested to mate with $N!",  ch, NULL, victim, TO_NOTVICT );
    act( "`W***You request to mate with $N!`x",  ch, NULL, victim, TO_CHAR );
    act( "`W***$n has requested to mate with you!`x", ch, NULL, victim, TO_VICT );
	victim->printf("You may use either `=Cacceptmate`x or `=Cdeclinemate`x to respond...\r\n"
		"(Of course you can always ignore the request if you don't wish to respond)\r\n");
}
/**************************************************************************/

/**************************************************************************/
void mate_data::display_pkinfo(char_data *ch)
{
	ch->titlebar("MATE INFO");

	ch->println(" Time - Who(status)");
	for(node=first; node; node=node->next){
		if(node->state!=MS_REMOVE){
			ch->printlnf("%5s - %s(%s)",
				get_display_pkinfo_time(node->till),
				CPERS(node->victim,ch),
				ms_state_name(node->state));
		}
	}
}
/**************************************************************************/
void do_pregnant(char_data *ch, char *argument);
/**************************************************************************/
void mate_data::accept_mate(char_data *ch, char *argument)
{
	char arg[MIL];
	char_data *victim;
    one_argument( argument, arg );

	// check there is only 1 who is currently requesting
	if(IS_NULLSTR(arg)){
		mate_node *last=NULL;
		int count=0;
		for(node=first; node; node=node->next){
			if(node->state==MS_REQUESTED_BY){
				count++;
				last=node;}
		}
		if(count==0){
			ch->println("There is currently no one requesting to mate with you.");
			return;}
		if(count>1){
			ch->printf( "There is more than one person currently requesting to mate with you,\r\n"
				"please type `=Cacceptmate <name>`x to be more specific.\r\n");
			return;}

		{ // single person has requested - accept them
			node=last;
			victim = get_char_room( ch, node->victim->name);
			if(victim==node->victim){
				act( "***$n and $N's start to mate!",  ch, NULL, victim, TO_NOTVICT );
				act( "`W***You and $N start to mate!`x",  ch, NULL, victim, TO_CHAR );
				act( "`W***You and $n start to mate!`x", ch, NULL, victim, TO_VICT );
				ch->mate_decline++;

				node->state=MS_REMOVE;
				node->till=current_time-1;
				// update the victim node
				if(victim->mates){
					mate_node *temp=victim->mates->find_char(ch);
					if(temp){
						temp->state=MS_REMOVE;
						temp->till=current_time-1;}
				}
				WAIT_STATE( ch, 40 );
				WAIT_STATE( victim, 40 );
				/*if (IS_SET(ch->form, FORM_UNDEAD)){
					return;}
				if (IS_SET(victim->form, FORM_UNDEAD)){
					return;}*/
				/*preg chances go here*/
				char_data *male;
				char_data *female;
				female = ch;
				male = victim;
				if (IS_SET(ch->sex,  SEX_FEMALE)){
					female = ch;
					male = victim;}
				if (IS_SET(victim->sex, SEX_FEMALE)){
					female = victim;
					male = ch;}
				int chance;
				chance = 25;
				/*if (female->race == race_lookup("human")){
					chance+=0;}*/
				if (female->race == race_lookup("elf")){
					chance+=5;}
				if (female->race == race_lookup("dwarf")){
					chance-=5;}
				if (female->race == race_lookup("gnome")){
					chance+=8;}
				if (female->race == race_lookup("halfling")){
					chance+=5;}
				if (female->race == race_lookup("orc")){
					chance+=3;}
				if (female->race == race_lookup("goblin")){
					chance+=3;}
				if (female->race == race_lookup("gnoll")){
					chance+=3;}
				if (female->race == race_lookup("voadkyn")){
					chance-=8;}
				if (female->race == race_lookup("ogre")){
					chance-=8;}
				if (female->race == race_lookup("drow")){
					chance+=5;}
				if (female->race == race_lookup("sprite")){
					chance+=7;}
				if (female->race == race_lookup("duergar")){
					chance-=5;}
				if (female->race == race_lookup("shadai")){
					chance+=3;}
				if (female->race == race_lookup("kobold")){
					chance+=3;}
				if (female->race == race_lookup("neko")){
					chance-=5;}
				if (female->race == race_lookup("lizalfi")){
					chance+=7;}
				if (female->race == race_lookup("garuda")){
					chance-=3;}
				if (female->race == race_lookup("drider")){
					chance-=5;}
				if (female->race == race_lookup("lycan")){
					chance-=5;}
				if (female->race == race_lookup("vampyre")){
					chance-=10;}
				if (female->race == race_lookup("troll")){
					chance-=10;}
				if (female->race == race_lookup("minotaur")){
					chance-=8;}
				if (female->race == race_lookup("brijidine")){
					chance-=7;}
				if (female->race == race_lookup("bralani")){
					chance-=6;}
				if (female->race == race_lookup("leonal")){
					chance-=10;}
				if (female->race == race_lookup("deva")){
					chance-=10;}
				if (female->race == race_lookup("archon")){
					chance-=10;}
				if (female->race == race_lookup("leukodaemon")){
					chance-=15;}
				if (female->race == race_lookup("thanadaemon")){
					chance-=20;}
				if (female->race == race_lookup("crucidaemon")){
					chance-=15;}
				if (female->race == race_lookup("succubus")){
					chance-=2;}
				if (female->race == race_lookup("hamatula")){
					chance-=10;}
				if (female->race == race_lookup("shadowdaemon")){
					chance-=15;}
				if (female->race == race_lookup("chromadraco")){
					chance-=10;}
				if (female->race == race_lookup("metallumdraco")){
					chance-=10;}
				if (female->race == race_lookup("gemmadraco")){
					chance-=10;}
				if (female->race == race_lookup("arcanorumdraco")){
					chance-=10;}
				if (female->race == race_lookup("divinadraco")){
					chance-=10;}
				if (female->race == race_lookup("psychicaedraco")){
					chance-=10;}
				/*if (male->race == race_lookup("human")){
					chance+=10;}*/
				if (male->race == race_lookup("elf")){
					chance+=3;}
				if (male->race == race_lookup("dwarf")){
					chance-=3;}
				if (male->race == race_lookup("gnome")){
					chance+=5;}
				if (male->race == race_lookup("halfling")){
					chance+=3;}
				if (male->race == race_lookup("orc")){
					chance+=2;}
				if (male->race == race_lookup("goblin")){
					chance+=2;}
				if (male->race == race_lookup("gnoll")){
					chance+=2;}
				if (male->race == race_lookup("voadkyn")){
					chance-=6;}
				if (male->race == race_lookup("ogre")){
					chance-=6;}
				if (male->race == race_lookup("drow")){
					chance+=3;}
				if (male->race == race_lookup("sprite")){
					chance+=5;}
				if (male->race == race_lookup("duergar")){
					chance-=3;}
				if (male->race == race_lookup("shadai")){
					chance+=1;}
				if (male->race == race_lookup("kobold")){
					chance+=1;}
				if (male->race == race_lookup("neko")){
					chance-=3;}
				if (male->race == race_lookup("lizalfi")){
					chance+=5;}
				if (male->race == race_lookup("garuda")){
					chance-=1;}
				if (male->race == race_lookup("drider")){
					chance-=3;}
				if (male->race == race_lookup("lycan")){
					chance-=3;}
				if (male->race == race_lookup("vampyre")){
					chance-=10;}
				if (male->race == race_lookup("troll")){
					chance-=8;}
				if (male->race == race_lookup("minotaur")){
					chance-=6;}
				if (male->race == race_lookup("brijidine")){
					chance-=5;}
				if (male->race == race_lookup("bralani")){
					chance-=4;}
				if (male->race == race_lookup("leonal")){
					chance-=8;}
				if (male->race == race_lookup("deva")){
					chance-=8;}
				if (male->race == race_lookup("archon")){
					chance-=8;}
				if (male->race == race_lookup("leukodaemon")){
					chance-=13;}
				if (male->race == race_lookup("thanadaemon")){
					chance-=18;}
				if (male->race == race_lookup("crucidaemon")){
					chance-=15;}
				if (male->race == race_lookup("incubus")){
					chance-=2;}
				if (male->race == race_lookup("hamatula")){
					chance-=10;}
				if (male->race == race_lookup("shadowdaemon")){
					chance-=15;}
				if (male->race == race_lookup("chromadraco")){
					chance-=10;}
				if (male->race == race_lookup("metallumdraco")){
					chance-=10;}
				if (male->race == race_lookup("gemmadraco")){
					chance-=10;}
				if (male->race == race_lookup("arcanorumdraco")){
					chance-=10;}
				if (male->race == race_lookup("divinadraco")){
					chance-=10;}
				if (male->race == race_lookup("psychicaedraco")){
					chance-=10;}

				if (female->race == race_lookup("ent") &&
					male->race == race_lookup("ent")){
					chance=30;}

				if ((IS_SET( ch->in_room->affected_by, ROOMAFF_BLESSED_GROUNDS )
				    && ch->alliance >= 351 )||
				   (IS_SET( ch->in_room->affected_by, ROOMAFF_CURSED_GROUNDS )
					&& ch->alliance < -349 )){
					chance+=1;}
				if (IS_AFFECTED(female, AFF_POISON) ){
					chance-=1;}
				if (IS_AFFECTED(female, AFF_PLAGUE)){
					chance-=10;}
				if (IS_AFFECTED(male, AFF_PLAGUE)){
					chance-=10;}
				if (is_affected(female, gsn_bless )){
					chance+=5;}
				if (IS_AFFECTED(female,AFF_REGENERATION)){
					chance+=2;}
				if (IS_AFFECTED(female, AFF_SANCTUARY )){
					chance+=2;}

				if(IS_AFFECTED3(female, AFF3_FERTILITY)){
					chance = 100;}
				if (female->race == race_lookup("liche") ||
				male->race == race_lookup("liche")){
				chance=0;}

				if (female->race == race_lookup("ent") &&
				male->race != race_lookup("ent")){
				chance=0;}

				if (male->race == race_lookup("ent") &&
				female->race != race_lookup("ent")){
				chance=0;}
				if(IS_AFFECTED3(female, AFF3_STERILE)){
					chance = 0;}
				if(IS_AFFECTED3(male, AFF3_STERILE)){
					chance = 0;}
			/*	if (IS_IMMORTAL(female) || IS_IMMORTAL(male)){
					return;}*/
				if (number_percent() <= chance){
				female->matemale = race_table[male->race]->name;
				female->pcdata->matemale = race_table[male->race]->name;
				REMOVE_BIT(female->affected_by3, AFF3_FERTILITY);
				do_pregnant(female, arg);}
			}
		}
	}else{
		victim = get_char_room( ch, arg);

		if(!victim){
			ch->printlnf("You can't seem to find any '%s' here.", arg);
			return;}
		if(IS_NPC(victim)){ // idiot check
			ch->printlnf("%s is not a player, you can mate only other players.",
				CPERS(victim, ch));
			return;}

		node=find_char(victim);
		if(node){
			if(node->state==MS_REQUESTED_BY){

				act( "***$n has accepted $N's request to mate!",  ch, NULL, victim, TO_NOTVICT );
				act( "`W***You accept $N's mating request!`x",  ch, NULL, victim, TO_CHAR );
				act( "`W***$n accepts your mating request!`x", ch, NULL, victim, TO_VICT );
				ch->mate_decline++;

				node->state=MS_REMOVE;
				node->till=current_time-1;
				// update the victim node
				mate_node *temp=victim->mates->find_char(ch);
				assert(temp!=NULL);
				temp->state=MS_REMOVE;
				temp->till=current_time-1;
				WAIT_STATE( ch, 40 );
				WAIT_STATE( victim, 40 );
				/*if (IS_SET(ch->form, FORM_UNDEAD)){
					return;}
				if (IS_SET(victim->form, FORM_UNDEAD)){
					return;}*/
				/*preg chances go here*/
				char_data *male;
				char_data *female;
				female = ch;
				male = victim;
				if (IS_SET(ch->sex,  SEX_FEMALE)){
					female = ch;
					male = victim;}
				if (IS_SET(victim->sex, SEX_FEMALE)){
					female = victim;
					male = ch;}
				int chance;
				chance = 25;
				/*if (female->race == race_lookup("human")){
					chance+=0;}*/
				if (female->race == race_lookup("elf")){
					chance+=5;}
				if (female->race == race_lookup("dwarf")){
					chance-=5;}
				if (female->race == race_lookup("gnome")){
					chance+=8;}
				if (female->race == race_lookup("halfling")){
					chance+=5;}
				if (female->race == race_lookup("orc")){
					chance+=3;}
				if (female->race == race_lookup("goblin")){
					chance+=3;}
				if (female->race == race_lookup("gnoll")){
					chance+=3;}
				if (female->race == race_lookup("voadkyn")){
					chance-=8;}
				if (female->race == race_lookup("ogre")){
					chance-=8;}
				if (female->race == race_lookup("drow")){
					chance+=5;}
				if (female->race == race_lookup("sprite")){
					chance+=7;}
				if (female->race == race_lookup("duergar")){
					chance-=5;}
				if (female->race == race_lookup("shadai")){
					chance+=3;}
				if (female->race == race_lookup("kobold")){
					chance+=3;}
				if (female->race == race_lookup("neko")){
					chance-=5;}
				if (female->race == race_lookup("lizalfi")){
					chance+=7;}
				if (female->race == race_lookup("garuda")){
					chance-=3;}
				if (female->race == race_lookup("drider")){
					chance-=5;}
				if (female->race == race_lookup("lycan")){
					chance-=5;}
				if (female->race == race_lookup("vampyre")){
					chance-=10;}
				if (female->race == race_lookup("troll")){
					chance-=10;}
				if (female->race == race_lookup("minotaur")){
					chance-=8;}
				if (female->race == race_lookup("brijidine")){
					chance-=7;}
				if (female->race == race_lookup("bralani")){
					chance-=6;}
				if (female->race == race_lookup("leonal")){
					chance-=10;}
				if (female->race == race_lookup("deva")){
					chance-=10;}
				if (female->race == race_lookup("archon")){
					chance-=10;}
				if (female->race == race_lookup("leukodaemon")){
					chance-=15;}
				if (female->race == race_lookup("thanadaemon")){
					chance-=20;}
				if (female->race == race_lookup("crucidaemon")){
					chance-=15;}
				if (female->race == race_lookup("succubus")){
					chance-=2;}
				if (female->race == race_lookup("hamatula")){
					chance-=10;}
				if (female->race == race_lookup("shadowdaemon")){
					chance-=15;}
				if (female->race == race_lookup("chromadraco")){
					chance-=10;}
				if (female->race == race_lookup("metallumdraco")){
					chance-=10;}
				if (female->race == race_lookup("gemmadraco")){
					chance-=10;}
				if (female->race == race_lookup("arcanorumdraco")){
					chance-=10;}
				if (female->race == race_lookup("divinadraco")){
					chance-=10;}
				if (female->race == race_lookup("psychicaedraco")){
					chance-=10;}
				/*if (male->race == race_lookup("human")){
					chance+=10;}*/
				if (male->race == race_lookup("elf")){
					chance+=3;}
				if (male->race == race_lookup("dwarf")){
					chance-=3;}
				if (male->race == race_lookup("gnome")){
					chance+=5;}
				if (male->race == race_lookup("halfling")){
					chance+=3;}
				if (male->race == race_lookup("orc")){
					chance+=2;}
				if (male->race == race_lookup("goblin")){
					chance+=2;}
				if (male->race == race_lookup("gnoll")){
					chance+=2;}
				if (male->race == race_lookup("voadkyn")){
					chance-=6;}
				if (male->race == race_lookup("ogre")){
					chance-=6;}
				if (male->race == race_lookup("drow")){
					chance+=3;}
				if (male->race == race_lookup("sprite")){
					chance+=5;}
				if (male->race == race_lookup("duergar")){
					chance-=3;}
				if (male->race == race_lookup("shadai")){
					chance+=1;}
				if (male->race == race_lookup("kobold")){
					chance+=1;}
				if (male->race == race_lookup("neko")){
					chance-=3;}
				if (male->race == race_lookup("lizalfi")){
					chance+=5;}
				if (male->race == race_lookup("garuda")){
					chance-=1;}
				if (male->race == race_lookup("drider")){
					chance-=3;}
				if (male->race == race_lookup("lycan")){
					chance-=3;}
				if (male->race == race_lookup("vampyre")){
					chance-=10;}
				if (male->race == race_lookup("troll")){
					chance-=8;}
				if (male->race == race_lookup("minotaur")){
					chance-=6;}
				if (male->race == race_lookup("brijidine")){
					chance-=5;}
				if (male->race == race_lookup("bralani")){
					chance-=4;}
				if (male->race == race_lookup("leonal")){
					chance-=8;}
				if (male->race == race_lookup("deva")){
					chance-=8;}
				if (male->race == race_lookup("archon")){
					chance-=8;}
				if (male->race == race_lookup("leukodaemon")){
					chance-=13;}
				if (male->race == race_lookup("thanadaemon")){
					chance-=18;}
				if (male->race == race_lookup("crucidaemon")){
					chance-=15;}
				if (male->race == race_lookup("incubus")){
					chance-=2;}
				if (male->race == race_lookup("hamatula")){
					chance-=10;}
				if (male->race == race_lookup("shadowdaemon")){
					chance-=15;}
				if (male->race == race_lookup("chromadraco")){
					chance-=10;}
				if (male->race == race_lookup("metallumdraco")){
					chance-=10;}
				if (male->race == race_lookup("gemmadraco")){
					chance-=10;}
				if (male->race == race_lookup("arcanorumdraco")){
					chance-=10;}
				if (male->race == race_lookup("divinadraco")){
					chance-=10;}
				if (male->race == race_lookup("psychicaedraco")){
					chance-=10;}

				if (female->race == race_lookup("ent") &&
					male->race == race_lookup("ent")){
					chance=30;}

				if (chance <= 0){
					chance = 5;}

				if ((IS_SET( ch->in_room->affected_by, ROOMAFF_BLESSED_GROUNDS )
				    && ch->alliance >= 351 )||
				   (IS_SET( ch->in_room->affected_by, ROOMAFF_CURSED_GROUNDS )
					&& ch->alliance < -349 )){
					chance+=1;}
				if (IS_AFFECTED(female, AFF_POISON) ){
					chance-=1;}
				if (IS_AFFECTED(female, AFF_PLAGUE)){
					chance-=10;}
				if (IS_AFFECTED(male, AFF_PLAGUE)){
					chance-=10;}
				if (is_affected(female, gsn_bless )){
					chance+=5;}
				if (IS_AFFECTED(female,AFF_REGENERATION)){
					chance+=2;}
				if (IS_AFFECTED(female, AFF_SANCTUARY )){
					chance+=2;}

				if (chance <= 0){
					chance = 5;}

				if(IS_AFFECTED3(female, AFF3_FERTILITY)){
					chance = 100;}
				if (female->race == race_lookup("liche") ||
				male->race == race_lookup("liche")){
				chance=0;}

				if (female->race == race_lookup("ent") &&
				male->race != race_lookup("ent")){
				chance=0;}

				if (male->race == race_lookup("ent") &&
				female->race != race_lookup("ent")){
				chance=0;}
				if(IS_AFFECTED3(female, AFF3_STERILE)){
					chance = 0;}
				if(IS_AFFECTED3(male, AFF3_STERILE)){
					chance = 0;}
			/*	if (IS_IMMORTAL(female) || IS_IMMORTAL(male)){
					return;}*/
				if (number_percent() <= chance){
				REMOVE_BIT(female->affected_by3, AFF3_FERTILITY);
				do_pregnant(female, arg);}
			}else{
				ch->printlnf("There is no currently mating request from %s involving you.",
					PERS(victim, ch));
				return;}
		}

	}
}
/**************************************************************************/
void mate_data::decline_mate(char_data *ch, char *argument)
{
	char arg[MIL];
	char_data *victim;
    one_argument( argument, arg );
	// check there is only 1 who is currently requesting
	if(IS_NULLSTR(arg)){
		mate_node *last=NULL;
		int count=0;
		for(node=first; node; node=node->next){
			if(node->state==MS_REQUESTED_BY){
				count++;
				last=node;
			}
		}
		if(count==0){
			ch->println("There is currently no one requesting to mate with you.");
			return;}
		if(count>1){
			ch->printf( "There is more than one person currently requesting to mate with you,\r\n"
				"please type `=Cdeclinemate <name>`x to be more specific.\r\n");
			return;}
		{ // single person has requested - decline them
			node=last;
			victim = get_char_room( ch, node->victim->name);

			if(!victim){
				ch->printlnf("%s doesn't appear to be in the room to decline their mating request.",
					CPERS(node->victim, ch));
				return;}
			if(victim==node->victim){
				act( "***$n has declined $N's mating request!",  ch, NULL, victim, TO_NOTVICT );
				act( "`W***You decline $N's mating request!`x",  ch, NULL, victim, TO_CHAR );
				act( "`W***$n declines your mating request!`x", ch, NULL, victim, TO_VICT );
				ch->mate_decline++;

				node->state=MS_REMOVE;
				node->till=current_time-1;
				// update the victim node
				if(victim->mates){
					mate_node *temp=victim->mates->find_char(ch);
					if(temp){
						temp->state=MS_REMOVE;
						temp->till=current_time-1;
					}
				}
			}
		}
	}else{
		victim = get_char_room( ch, arg);

		if(!victim){
			ch->printlnf("You can't seem to find any '%s' here.", arg);
			return;}
		// idiot check here..
		if(IS_NPC(victim)){
			ch->printlnf("%s is not a player, you can mate only other players.",
				CPERS(victim, ch));
			return;}

		node=find_char(victim);
		if(node){
			if(node->state==MS_REQUESTED_BY){

				act( "***$n has declined $N's mating request!",  ch, NULL, victim, TO_NOTVICT );
				act( "`W***You decline $N's mating request!`x",  ch, NULL, victim, TO_CHAR );
				act( "`W***$n declines your mating request!`x", ch, NULL, victim, TO_VICT );
				ch->mate_decline++;

				node->state=MS_REMOVE;
				node->till=current_time-1;
				// update the victim node
				mate_node *temp=victim->mates->find_char(ch);
				assert(temp!=NULL);
				temp->state=MS_REMOVE;
				temp->till=current_time-1;
			}else{
				ch->printlnf("There is no currently mating request from %s involving you.",
					PERS(victim, ch));
				return;

			}
		}
	}
}
/**************************************************************************/
// destructor - deallocate all memory used
mate_data::~mate_data()
{
	node=first;
	while(node){
		first=first->next;
		delete node;
		node=first;
	}
}
/**************************************************************************/
// constructor
mate_data::mate_data()
{
	first=NULL;
}

/**************************************************************************/
// ignore them time has expired
mate_node* mate_data::find_char(char_data *victim)
{
	for(node=first; node; node=node->next){
		if(node->victim==victim){
			return node;
		}
	}
	return NULL;
};
/**************************************************************************/
// updates the mates on the current character
void mate_data::update_mates(char_data *ch)
{
	assert(ch->mates==this);
	assert(!IS_NPC(ch));

	mate_node *prev;
	mate_node *node_next;
	prev=NULL;
	bool remove=false; // if true, the node is removed

	for(node=first; node; node=node_next){
		node_next=node->next;
		if(node->till<current_time){ // a timer has expired
			switch(node->state){
			case MS_REQUESTED_BY:
				ch->printlnf("`W***You have taken to long to respond to %s, mating request considered ignored.`x",
					PERS(node->victim, ch));
				logf("MATEIGNORE: %s ignored the mating request of %s", ch->name, node->victim->name);
				ch->mate_ignore++;
				remove=true;
				break;

			case MS_WAITING_FOR_REPLY:
				ch->printlnf("`W***%s hasn't responded to your mating request, it appears they have ignored you.`x",
					CPERS(node->victim, ch));
				remove=true;
				break;

			case MS_ABOUT_TO_START:
				node->state=MS_MATING; // go active, but don't tell them
				node->till=current_time + mate_times[MS_MATING];
				break;

			case MS_REMOVE: // removes a node from the list for any particular reason
				remove=true;
				break;

			default:
				bug("Unknown state in mate_data::update_mates(char_data *ch)!!!");
				do_abort();
			}
		}

		// remove the node if necessary
		if(remove){
			if(prev){
				prev->next=node->next;
			}else{
				first=node->next;
			}
			delete node;
		}else{
			prev=node;
		}
	}
	// delete their mates pointer if it holds nothing
	if(!first){
		delete ch->mates;
		ch->mates=NULL;
	}
};
/**************************************************************************/
void mate_data::update_request(char_data *victim, mate_state to_state)
{
	node=find_char(victim);

	if(!node){
		node=new mate_node;
		node->victim=victim;
		// prepend to list of mates because it is new
		node->next=first;
		first=node;
	}
	node->state=to_state;
	node->till=current_time + mate_times[to_state];
}
/**************************************************************************/
// return true if they are previously known
bool mate_data::is_known(char_data *victim)
{
	node=find_char(victim);
	if(node){
		return true;
	}
	return false;
}
/**************************************************************************/
bool mate_data::has_been_requested_by(char_data *victim)
{
	node=find_char(victim);
	if(node){
		return (node->state==MS_REQUESTED_BY);
	}
	return false;
};
/**************************************************************************/
bool mate_data::is_waiting_for_reply(char_data *victim)
{
	node=find_char(victim);
	if(node){
		return (node->state==MS_WAITING_FOR_REPLY);
	}
	return false;
};
/**************************************************************************/
bool mate_data::is_about_to_start(char_data *victim)
{
	node=find_char(victim);
	if(node){
		return (node->state==MS_ABOUT_TO_START);
	}
	return false;
};
/**************************************************************************/

/**************************************************************************/
bool mate_data::is_mating(char_data* ch, char_data *victim){
	node=find_char(victim);

	if(node){
		return (node->state==MS_MATING);
	}
	return false;
};
/**************************************************************************/

