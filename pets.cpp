/**************************************************************************/
// pets.cpp - Pet Code by Ixliam
/***************************************************************************
 * The Dawn of Time v1.69r (c)1997-2004 Michael Garratt                    *
 * >> A number of people have contributed to the Dawn codebase, with the   *
 *    majority of code written by Michael Garratt - www.dawnoftime.org     *
 * >> To use this source code, you must fully comply with the dawn license *
 *    in licenses.txt... In particular, you may not remove this copyright  *
 *    notice.                                                              *
 **************************************************************************/

#include "include.h"
#include "areas.h"
#include "olc.h"
#include "security.h"
/**************************************************************************/
void fread_newpet( char_data *ch, FILE *fp )
{
    	char *word;
    	char_data *pet;
    	bool fMatch;
    	time_t lastlogoff = current_time;
    	int percent;
	bool non_existant_pet=false;
	int pet_vnum=0;

    // first entry had BETTER be the vnum or we barf
	word = feof(fp) ? (char*)"END" : fread_word(fp);
    if (!str_cmp(word,"Vnum"))
    {
		pet_vnum= fread_number(fp);
		if (get_mob_index(pet_vnum) == NULL)
		{
			bugf("Fread_pet: bad vnum %d.",pet_vnum);
			non_existant_pet=true;
			pet = create_mobile(limbo_mob_index_data, 0);
		}else{
			pet = create_mobile(get_mob_index(pet_vnum), 0);
		}
    }
    else
    {
		bug("Fread_pet: no vnum in file.");
		pet = create_mobile(limbo_mob_index_data, 0);
    }

    for ( ; ; )
    {
		word    = feof(fp) ? (char*)"END" : fread_word(fp);
		fMatch = false;

		switch (UPPER(word[0]))
		{
		case '*':
			fMatch = true;
			fread_to_eol(fp);
			break;

		case 'A':
			KEY( "Act",         pet->act,               fread_flag(fp));
			KEY( "AfBy",        pet->affected_by,       fread_flag(fp));
			KEY( "AffBy",       pet->affected_by,       fread_wordflag(affect_flags, fp));
			KEY( "AffBy2",      pet->affected_by2,      fread_wordflag(affect2_flags,fp));
			KEY( "AffBy3",      pet->affected_by3,      fread_wordflag(affect3_flags,fp));
			KEY( "Alig",        pet->alliance,          3*fread_number(fp)/1000);

			if (!str_cmp(word,"Algn"))
			{
//				pet->tendency=fread_number(fp);
				pet->alliance=fread_number(fp);
				fMatch = true;
				break;
			}

			if (!str_cmp(word,"ACs"))
			{
				int i;

				for (i = 0; i < 4; i++)
					pet->armor[i] = fread_number(fp);
				fMatch = true;
				break;
			}

			if (!str_cmp(word,"Affc"))
			{
				AFFECT_DATA *paf;
				int sn;

				paf = new_affect();

				sn = skill_lookup(fread_word(fp));
				if (sn < 0)
					bug("Fread_char: unknown skill.");
				else
					paf->type = sn;

				paf->where      = fread_number(fp);
				paf->level      = fread_number(fp);
				paf->duration   = fread_number(fp);
				paf->modifier   = fread_number(fp);
				paf->location   = translate_old_apply_number(fread_number(fp));
				paf->bitvector  = fread_number(fp);
				paf->next       = pet->affected;
				pet->affected   = paf;
				fMatch          = true;
				break;
			}

			if (!str_cmp(word,"AMod"))
			{
				int stat;
				if(ch->version>8)
				{
					for (stat = 0; stat < MAX_STATS; stat++)
						pet->modifiers[stat] = fread_number(fp);
				}
				fMatch = true;
				break;
			}

			if (!str_cmp(word,"Attr"))
			{
				int stat;
				if(ch->version>8)
				{
					for (stat = 0; stat < MAX_STATS; stat++)
						pet->perm_stats[stat] = fread_number(fp);
				}
				fMatch = true;
				break;
			}
			break;

		case 'C':
			KEY( "Clan",       pet->clan,       clan_slookup(fread_string(fp)));
			KEY( "Color1",     pet->petcolor1,       fread_string(fp));
			KEY( "Color2",     pet->petcolor2,       fread_string(fp));
			KEY( "Color3",     pet->petcolor3,       fread_string(fp));
			KEY( "Color4",     pet->petcolor4,       fread_string(fp));
			KEY( "Comm",       pet->comm,              fread_flag(fp));
			KEY( "Com",        pet->comm,              fread_wordflag(comm_flags, fp));
			break;

		case 'D':
			KEY( "Dam",        pet->damroll,           fread_number(fp));
			KEY( "Desc",       pet->description,       fread_string(fp));
			break;

		case 'E':
			if (!str_cmp(word,"End"))
			{
				if(non_existant_pet){
					logf("Removing pet from %s, because the vnum of it couldn't be located.", PERS(ch, NULL));
					ch->println("`R********************************************************************************");
					ch->printlnf("`R*`W Your pet vnum %5d couldn't be located for loading, contact the admin. `R*", pet_vnum);
					ch->println("`R********************************************************************************`x");
				}else{
					pet->leader = ch;
					pet->master = ch;
					ch->pet = pet;
					/* adjust hp mana move up  -- here for speed's sake */
					percent = (int)(current_time - lastlogoff) * 25 / ( 2 * 60 * 60);

					if (percent > 0 && !IS_AFFECTED(ch,AFF_POISON)
						&&  !IS_AFFECTED(ch,AFF_PLAGUE))
					{
						percent = UMIN(percent,100);
						pet->hit    += (pet->max_hit - pet->hit) * percent / 100;
						pet->mana   += (pet->max_mana - pet->mana) * percent / 100;
						pet->move   += (pet->max_move - pet->move)* percent / 100;
					}
				}
				return;
			}
			KEY( "Exp",        pet->exp,               fread_number(fp));
			break;

		case 'G':
			KEY( "Gold",       pet->gold,              fread_number(fp));
			break;

		case 'H':
			KEY( "Hit",        pet->hitroll,           fread_number(fp));

			if (!str_cmp(word,"HMPV"))
			{
				pet->hit        = fread_number(fp);
				pet->max_hit    = fread_number(fp);
				pet->mana       = fread_number(fp);
				pet->max_mana   = fread_number(fp);
				pet->psi       = fread_number(fp);
				pet->max_psi   = fread_number(fp);
				pet->move       = fread_number(fp);
				pet->max_move   = fread_number(fp);
				fMatch = true;
				break;
			}
			break;

		case 'L':
			KEY( "Levl",       pet->level,             fread_number(fp));
			KEY( "LnD",        pet->long_descr,        fread_string(fp));
			KEY( "LogO",       lastlogoff,             fread_number(fp));
			break;

		case 'M':
			KEY( "MAct",       pet->act,              fread_wordflag(act_flags,fp));
			KEY( "MTknown",    pet->magic_trick,     fread_number(fp));
			KEY( "Mtricks",    pet->pet_magic_flags, fread_wordflag(pet_magic_flags,fp));
			break;

		case 'N':
			KEY( "Name",       pet->name,              fread_string(fp));
			KEY( "NTknown",    pet->norm_trick,   fread_number(fp));
			KEY( "Ntricks",    pet->pet_norm_flags, fread_wordflag(pet_norm_flags,fp));

			break;

		case 'P':
			KEY( "Pos",        pet->position,          fread_number(fp));
			KEY( "Petbirth",   pet->petbirth,             fread_number(fp));
			KEY( "PsiAfBy",        pet->psionic_affects,       fread_flag(fp));
			KEY( "PsiAffBy",       pet->psionic_affects,       fread_wordflag(psionic_affects, fp));
		case 'R':
			//	    KEY( "Race",        pet->race, race_lookup(fread_string(fp)));
			if ( !str_cmp( word, "Race" ) )
			{
				ch->race = race_lookup(fread_string( fp ));
				if (ch->race == -1)
				{
					ch->race = race_lookup("human");
					bugf("fread_newpet: pet '%s' has an invalid race!!!",
						ch->name);
				}
				fMatch = true;
			}


			if ( !str_cmp( word, "Room" ) )
			{
				pet->in_room = get_room_index(ROOM_VNUM_LIMBO);
				fMatch = true;
			}

			break;

		case 'S' :
			KEY( "Save",        pet->saving_throw,      fread_number(fp));
			KEY( "Sex",         pet->sex,               fread_number(fp));
			KEY( "ShD",         pet->short_descr,       fread_string(fp));
			KEY( "Silv",        pet->silver,            fread_number( fp ) );
			break;

			if ( !fMatch )
			{
				bug("Fread_pet: no match.");
				fread_to_eol(fp);
			}

		}
    }

    char_to_room( pet, ch->in_room );

    pet->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
    SET_BIT(pet->act, ACT_PET);
    SET_BIT(pet->affected_by, AFF_CHARM);
    SET_BIT(pet->dyn, DYN_IS_BEING_ORDERED);
    add_follower( pet, ch );
    pet->leader = ch;
    ch->pet = pet;

    return;
}
/**************************************************************************/
// write a pet
void fwrite_newpet( char_data *pet, FILE *fp)
{
	AFFECT_DATA *paf;

	fprintf(fp,"Vnum %d\n",pet->pIndexData->vnum);

	fprintf(fp,"Name %s~\n", pet->name);
    fprintf(fp,"LogO %ld\n", (long) current_time);
    if (pet->short_descr != pet->pIndexData->short_descr)
		fprintf(fp,"ShD  %s~\n", fix_string(pet->short_descr));
    if (pet->long_descr != pet->pIndexData->long_descr)
		fprintf(fp,"LnD  %s~\n", fix_string(pet->long_descr));
	if (pet->description != pet->pIndexData->description)
		fprintf(fp,"Desc %s~\n", fix_string(pet->description));
    if (pet->race != pet->pIndexData->race)
		fprintf(fp,"Race %s~\n", race_table[pet->race]->name);
	if (pet->clan){
		fprintf( fp, "Clan %s~\n",pet->clan->savename());
	}
	fprintf(fp,"Sex  %d\n", pet->sex);
	if (pet->level != pet->pIndexData->level)
		fprintf(fp,"Levl %d\n", pet->level);
	fprintf(fp, "HMPV  %d %d %d %d %d %d %d %d\n",
		pet->hit, pet->max_hit, pet->mana, pet->max_mana, pet->psi, pet->max_psi, pet->move, pet->max_move);
	if (pet->gold > 0)
		fprintf(fp,"Gold %ld\n",pet->gold);
	if (pet->silver > 0)
		fprintf(fp,"Silv %ld\n",pet->silver);

	fprintf( fp, "Room %d\n",
		(  pet->in_room == get_room_index( ROOM_VNUM_LIMBO )
		&& pet->was_in_room != NULL )
		? pet->was_in_room->vnum
		: pet->in_room == NULL ? ROOM_VNUM_LIMBO : pet->in_room->vnum );

	if (pet->exp > 0)
		fprintf(fp, "Exp  %d\n", pet->exp);
    if (pet->act != pet->pIndexData->act){
		fwrite_wordflag( act_flags, pet->act, "MAct ", fp);
	}
    if (pet->affected_by != pet->pIndexData->affected_by){
		fwrite_wordflag( affect_flags, pet->affected_by, "AffBy ", fp);
	}
    if (pet->affected_by2 != pet->pIndexData->affected_by2){
		fwrite_wordflag( affect2_flags, pet->affected_by2, "AffBy2 ", fp);
	}
	if (pet->affected_by3 != pet->pIndexData->affected_by3){
		fwrite_wordflag( affect3_flags, pet->affected_by3, "AffBy3 ", fp);
	}
	if (pet->psionic_affects != pet->pIndexData->psionic_affects){
		fwrite_wordflag( psionic_affects, pet->psionic_affects, "PsiAffBy ", fp);
	}
	if (pet->comm != 0){
		fwrite_wordflag( comm_flags, pet->comm, "Com ", fp);
	}

	fprintf(fp,"Pos  %d\n", pet->position = POS_FIGHTING ? POS_STANDING : pet->position);

  	if(pet->petcolor1 != NULL){
	    fprintf(fp,"Color1 %s\n", pet->petcolor1);
	}
  	if(pet->petcolor2 != NULL){
	    fprintf(fp,"Color2 %s\n", pet->petcolor2);
	}
  	if(pet->petcolor3 != NULL){
	    fprintf(fp,"Color3 %s\n", pet->petcolor3);
	}
  	if(pet->petcolor4 != NULL){
	    fprintf(fp,"Color4 %s\n", pet->petcolor4);
	}

	fprintf(fp,"Petbirth %ld\n", pet->petbirth);

	if(pet->norm_trick){
		fprintf(fp,"NTknown  %d\n", pet->norm_trick);
	}
	if(pet->magic_trick){
		fprintf(fp,"MTknown  %d\n", pet->magic_trick);
	}

    	if (pet->pet_norm_flags != 0){
		fwrite_wordflag( pet_norm_flags, pet->pet_norm_flags, "Ntricks ", fp);
	}
    	if (pet->pet_magic_flags != 0){
		fwrite_wordflag( pet_magic_flags, pet->pet_magic_flags, "Mtricks ", fp);
	}

    if (pet->saving_throw != 0)
		fprintf(fp, "Save %d\n", pet->saving_throw);
    if (pet->alliance != pet->pIndexData->alliance/* ||
		pet->tendency != pet->pIndexData->tendency*/)
		fprintf(fp, "Algn %d\n", pet->alliance);
    if (pet->hitroll != pet->pIndexData->hitroll)
		fprintf(fp, "Hit  %d\n", pet->hitroll);
	if (pet->damroll != pet->pIndexData->damage[DICE_BONUS])
		fprintf(fp, "Dam  %d\n", pet->damroll);
    fprintf(fp, "ACs  %d %d %d %d\n",
		pet->armor[0],pet->armor[1],pet->armor[2],pet->armor[3]);
	fprintf(fp, "Attr %d %d %d %d %d %d %d %d %d %d\n",
		pet->perm_stats[STAT_ST], pet->perm_stats[STAT_QU],
		pet->perm_stats[STAT_PR], pet->perm_stats[STAT_EM],
		pet->perm_stats[STAT_IN],pet->perm_stats[STAT_CO],
		pet->perm_stats[STAT_AG],
		pet->perm_stats[STAT_SD], pet->perm_stats[STAT_ME],
		pet->perm_stats[STAT_RE]);
	fprintf(fp, "AMod %d %d %d %d %d %d %d %d %d %d\n",
		pet->modifiers[STAT_ST], pet->modifiers[STAT_QU],
		pet->modifiers[STAT_PR], pet->modifiers[STAT_EM],
		pet->modifiers[STAT_IN],
		pet->modifiers[STAT_CO], pet->modifiers[STAT_AG],
		pet->modifiers[STAT_SD], pet->modifiers[STAT_ME],
		pet->modifiers[STAT_RE] );


	for ( paf = pet->affected; paf != NULL; paf = paf->next )
    {
		if (paf->type < 0 || paf->type >= MAX_SKILL)
			continue;

		fprintf(fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
			skill_table[paf->type].name,
			paf->where, paf->level, paf->duration, paf->modifier,paf->location,
			paf->bitvector);
    }

	fprintf(fp,"End\n");
	return;
}
/**************************************************************************/
bool save_pet(char_data *ch)
{
	FILE *fp;
	char write_filename[MIL];
	char pet_filename[MIL];
	char_data *pet;
	int x=1;
    if (ch->kennelupg<=0 || ch->kennelupg>=6){
    ch->kennelupg=0;}
	for (x=1; x; x++)
	{
		sprintf(write_filename, "%s%s%d.write", PET_DIR, ch->name, x);
		sprintf(pet_filename, "%s%s%d.txt", PET_DIR, ch->name, x);
		if(!file_exists(pet_filename))
		{
			break;
		}
	if(x==5 && ch->kennelupg==0)
		{
			return false;
		}
	if(x==8 && ch->kennelupg==1)
		{
			return false;
		}
	if(x==11 && ch->kennelupg==2)
		{
			return false;
		}
	if(x==14 && ch->kennelupg==3)
		{
			return false;
		}
	if(x==17 && ch->kennelupg==4)
		{
			return false;
		}
	if(x==20 && ch->kennelupg>=5)
		{
			return false;
		}
	}

	unlink(write_filename);

	fclose( fpReserve );
    	fp = fopen( write_filename, "w" );

	pet = ch->pet;

	fwrite_newpet(pet, fp);
	if ( pet->carrying != NULL )
	{
		fwrite_obj( ch->pet->carrying, fp, 0, "PO" ); // pet objects
	}
	fprintf(fp, "\n");

	int bytes_written=fprintf(fp, "EOF\n");
	fclose( fp );
	if(   bytes_written != str_len("EOF\n") ){
		ch->printlnf("Incomplete write of %s, write aborted - check diskspace! - error %d (%s)",
			write_filename, errno, strerror( errno));
	    fpReserve = fopen( NULL_FILE, "r" );
	}else{
		unlink(pet_filename);
		rename(write_filename, pet_filename);
	}

	if(x==1)
		ch->pet1 = pet->short_descr;
	if(x==2)
		ch->pet2 = pet->short_descr;
	if(x==3)
		ch->pet3 = pet->short_descr;
	if(x==4)
		ch->pet4 = pet->short_descr;
    if(x==5)
		ch->pet5 = pet->short_descr;
    if(x==6)
		ch->pet6 = pet->short_descr;
    if(x==7)
		ch->pet7 = pet->short_descr;
    if(x==8)
		ch->pet8 = pet->short_descr;
    if(x==9)
		ch->pet9 = pet->short_descr;
    if(x==10)
		ch->pet10 = pet->short_descr;
    if(x==11)
		ch->pet11 = pet->short_descr;
    if(x==12)
		ch->pet12 = pet->short_descr;
    if(x==13)
		ch->pet13 = pet->short_descr;
    if(x==14)
		ch->pet14 = pet->short_descr;
    if(x==15)
		ch->pet15 = pet->short_descr;
    if(x==16)
		ch->pet16 = pet->short_descr;
    if(x==17)
		ch->pet17 = pet->short_descr;
    if(x==18)
		ch->pet18 = pet->short_descr;
    if(x==19)
		ch->pet19 = pet->short_descr;
    if(x==20)
		ch->pet20 = pet->short_descr;

    	fpReserve = fopen( NULL_FILE, "r" );
	extract_char(pet,true);
	ch->pet = NULL;
	return true;
}
/**************************************************************************/
void list_pet(char_data *ch)
{
	char pet_file1[MIL];
	char pet_file2[MIL];
	char pet_file3[MIL];
	char pet_file4[MIL];
	char pet_file5[MIL];
	char pet_file6[MIL];
	char pet_file7[MIL];
	char pet_file8[MIL];
	char pet_file9[MIL];
	char pet_file10[MIL];
	char pet_file11[MIL];
	char pet_file12[MIL];
	char pet_file13[MIL];
	char pet_file14[MIL];
	char pet_file15[MIL];
	char pet_file16[MIL];
	char pet_file17[MIL];
	char pet_file18[MIL];
	char pet_file19[MIL];
	char pet_file20[MIL];

	bool found = false;

	sprintf(pet_file1, "%s%s1.txt", PET_DIR, ch->name);
	sprintf(pet_file2, "%s%s2.txt", PET_DIR, ch->name);
	sprintf(pet_file3, "%s%s3.txt", PET_DIR, ch->name);
	sprintf(pet_file4, "%s%s4.txt", PET_DIR, ch->name);
	sprintf(pet_file5, "%s%s5.txt", PET_DIR, ch->name);
	if (ch->kennelupg>=1){
	sprintf(pet_file6, "%s%s6.txt", PET_DIR, ch->name);
	sprintf(pet_file7, "%s%s7.txt", PET_DIR, ch->name);
	sprintf(pet_file8, "%s%s8.txt", PET_DIR, ch->name);
	}
	if (ch->kennelupg>=2){
	sprintf(pet_file9, "%s%s9.txt", PET_DIR, ch->name);
	sprintf(pet_file10, "%s%s10.txt", PET_DIR, ch->name);
	sprintf(pet_file11, "%s%s11.txt", PET_DIR, ch->name);
	}
	if (ch->kennelupg>=3){
	sprintf(pet_file12, "%s%s12.txt", PET_DIR, ch->name);
	sprintf(pet_file13, "%s%s13.txt", PET_DIR, ch->name);
	sprintf(pet_file14, "%s%s14.txt", PET_DIR, ch->name);
	}
	if (ch->kennelupg>=4){
	sprintf(pet_file15, "%s%s15.txt", PET_DIR, ch->name);
	sprintf(pet_file16, "%s%s16.txt", PET_DIR, ch->name);
	sprintf(pet_file17, "%s%s17.txt", PET_DIR, ch->name);
	}
	if (ch->kennelupg==5){
	sprintf(pet_file18, "%s%s18.txt", PET_DIR, ch->name);
	sprintf(pet_file19, "%s%s19.txt", PET_DIR, ch->name);
	sprintf(pet_file20, "%s%s20.txt", PET_DIR, ch->name);
	}

	ch->titlebar("-= PETS IN KENNEL =-");

	if(file_exists(pet_file1))
	{
		found = true;
	 	ch->printlnf("`s1> `CPet: `c%s", ch->pet1);
	}
	if(file_exists(pet_file2))
	{
		found = true;
	 	ch->printlnf("`s2> `CPet: `c%s", ch->pet2);
	}
	if(file_exists(pet_file3))
	{
		found = true;
	 	ch->printlnf("`s3> `CPet: `c%s", ch->pet3);
	}
	if(file_exists(pet_file4))
	{
		found = true;
	 	ch->printlnf("`s4> `CPet: `c%s", ch->pet4);
	}
	if(file_exists(pet_file5))
	{
		found = true;
	 	ch->printlnf("`s5> `CPet: `c%s", ch->pet5);
	}
	if(file_exists(pet_file6))
	{
		found = true;
	 	ch->printlnf("`s6> `CPet: `c%s", ch->pet6);
	}
	if(file_exists(pet_file7))
	{
		found = true;
	 	ch->printlnf("`s7> `CPet: `c%s", ch->pet7);
	}
	if(file_exists(pet_file8))
	{
		found = true;
	 	ch->printlnf("`s8> `CPet: `c%s", ch->pet8);
	}
	if(file_exists(pet_file9))
	{
		found = true;
	 	ch->printlnf("`s9> `CPet: `c%s", ch->pet9);
	}
	if(file_exists(pet_file10))
	{
		found = true;
	 	ch->printlnf("`s10> `CPet: `c%s", ch->pet10);
	}
	if(file_exists(pet_file11))
	{
		found = true;
	 	ch->printlnf("`s11> `CPet: `c%s", ch->pet11);
	}
	if(file_exists(pet_file12))
	{
		found = true;
	 	ch->printlnf("`s12> `CPet: `c%s", ch->pet12);
	}
	if(file_exists(pet_file13))
	{
		found = true;
	 	ch->printlnf("`s13> `CPet: `c%s", ch->pet13);
	}
	if(file_exists(pet_file14))
	{
		found = true;
	 	ch->printlnf("`s14> `CPet: `c%s", ch->pet14);
	}
	if(file_exists(pet_file15))
	{
		found = true;
	 	ch->printlnf("`s15> `CPet: `c%s", ch->pet15);
	}
	if(file_exists(pet_file16))
	{
		found = true;
	 	ch->printlnf("`s16> `CPet: `c%s", ch->pet16);
	}
	if(file_exists(pet_file17))
	{
		found = true;
	 	ch->printlnf("`s17> `CPet: `c%s", ch->pet17);
	}
	if(file_exists(pet_file18))
	{
		found = true;
	 	ch->printlnf("`s18> `CPet: `c%s", ch->pet18);
	}
	if(file_exists(pet_file19))
	{
		found = true;
	 	ch->printlnf("`s19> `CPet: `c%s", ch->pet19);
	}
	if(file_exists(pet_file20))
	{
		found = true;
	 	ch->printlnf("`s20> `CPet: `c%s", ch->pet20);
	}


	if (found == false)
		ch->println("None.");
	return;
}
/**************************************************************************/
bool load_pet(char_data *ch, char *num)
{
	FILE *fp;
	char write_filename[MIL];

	sprintf(write_filename, "%s%s%s.txt", PET_DIR, ch->name, num);

	if(!file_exists(write_filename))
	{
		ch->println("Cannot find file.");
		return false;
	}

	fclose( fpReserve );
    	if ( ( fp = fopen( write_filename, "r" ) ) == NULL ){
		bugf("load_pet(): fopen '%s' failed for read - error %d (%s)",
			write_filename, errno, strerror( errno));
	    fpReserve = fopen( NULL_FILE, "r" );
		ch->println("Error in load_pet.");
		return false;
    	}

	fread_newpet( ch, fp );
    /***this didn't work before
	for ( ; ; )
	{
		char *word;
		word = fread_word( fp );
		if ( !str_cmp( word, "PO") )
		{
			OBJ_DATA *o=fread_obj( fp, write_filename);
			if(o && ch->pet)
			{
				obj_to_char(o, ch->pet);
			}
		}else if ( !str_cmp( word, "EOF"    ) )
		{
			break;
		}
	}******/

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
				bug("Load_char_obj: # not found.");
				break;
			}

			word = fread_word( fp );
			if ( !str_cmp(word,"OBJECT") || !str_cmp( word, "O") )
			{
				OBJ_DATA *o=fread_obj( fp, write_filename);
				if(o){
					obj_to_char(o, ch);
				}
			}else if ( !str_cmp( word, "PET"    ) ){
				fread_newpet( ch, fp );
			}else if ( !str_cmp( word, "PO") ){ // pet objects
				OBJ_DATA *o=fread_obj( fp, write_filename);
				if(o && ch->pet){
					obj_to_char(o, ch->pet);
				}

			}else if ( !str_cmp( word, "END"    ) ){
				break;
			}else{
				bugf("Load_char_obj: bad section '%s'.", word);
				break;
			}
		}

	fclose(fp);
	fpReserve = fopen( NULL_FILE, "r" );
	unlink(write_filename);
	remove(write_filename);
	char_to_room( ch->pet, ch->in_room );
	return true;
}
/**************************************************************************/
void do_kennel(char_data *ch, char *argument)
{
    char_data *mob;
    char arg1[MIL];
    char arg2[MIL];

    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    if (ch->position == POS_SLEEPING)
    {
	ch->println("You can't do that while sleeping.");
        return;
    }
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( IS_NPC(mob) && IS_SET(mob->act2, ACT2_KENNEL) )
            break;
    }
    if ( mob == NULL )
    {
        ch->println("You can't do that here.");
        return;
    }
    if ( ch->fighting ) {
	ch->println("You seem to be too busy to buy anything now.");
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NULLSTR(arg1))
    {
	act("$N says 'I can LIST, STORE, or RELEASE your pets.'",ch,NULL,mob,TO_CHAR);
	act("$N says 'It will cost 20 gold to store your pet.'",ch,NULL,mob,TO_CHAR);
	act("$N says 'If you need more storage I can UPGRADE your kennel account.'"
    ,ch,NULL,mob,TO_CHAR);
	return;
    }

    if (!str_prefix(arg1,"list"))
    {
	act("$N says 'Here is what you have in our kennels.'",ch,NULL,mob,TO_CHAR);
	list_pet(ch);
	return;
    }

    if (!str_prefix(arg1,"upgrade"))
    {
    int upgcost  = 20;
    if (ch->kennelupg <=0 || ch->kennelupg >= 6){
    ch->kennelupg=0;
    }
    if (ch->kennelupg==0){
    upgcost = 10000;
    }
    if (ch->kennelupg==1){
    upgcost = 50000;
    }
    if (ch->kennelupg==2){
    upgcost = 100000;
    }
    if (ch->kennelupg==3){
    upgcost = 300000;
    }
    if (ch->kennelupg==4){
    upgcost = 500000;
    }
	if (upgcost > (ch->gold * 100 + ch->silver))
	{
		act("$N says 'You do not have enough gold to upgrade your kennel account.'",ch,NULL,mob,TO_CHAR);
		if (ch->kennelupg==0){
        act("$N says 'It costs 100 Gold to upgrade.'",ch,NULL,mob,TO_CHAR);
        }
        if (ch->kennelupg==1){
        act("$N says 'It costs 500 Gold to upgrade.'",ch,NULL,mob,TO_CHAR);
        }
        if (ch->kennelupg==2){
        act("$N says 'It costs 1000 Gold to upgrade.'",ch,NULL,mob,TO_CHAR);
        }
        if (ch->kennelupg==3){
        act("$N says 'It costs 3000 Gold to upgrade.'",ch,NULL,mob,TO_CHAR);
        }
        if (ch->kennelupg==4){
        act("$N says 'It costs 5000 Gold to upgrade.'",ch,NULL,mob,TO_CHAR);
        }
		return;
	}
	if (ch->kennelupg==5){
		act("$N says 'Your kennel account can not be upgraded further.'",ch,NULL,mob,TO_CHAR);
		return;
	}
	if (!ch->attemptingupgrade || ch->attemptingupgrade<=0 ||ch->attemptingupgrade>=2){
        if (ch->kennelupg==0){
        act("$N says 'It costs 100 Gold to upgrade.'",ch,NULL,mob,TO_CHAR);
        }
        if (ch->kennelupg==1){
        act("$N says 'It costs 500 Gold to upgrade.'",ch,NULL,mob,TO_CHAR);
        }
        if (ch->kennelupg==2){
        act("$N says 'It costs 1000 Gold to upgrade.'",ch,NULL,mob,TO_CHAR);
        }
        if (ch->kennelupg==3){
        act("$N says 'It costs 3000 Gold to upgrade.'",ch,NULL,mob,TO_CHAR);
        }
        if (ch->kennelupg==4){
        act("$N says 'It costs 5000 Gold to upgrade.'",ch,NULL,mob,TO_CHAR);
        }
        ch->attemptingupgrade=1;
        ch->println("Type KENNEL UPGRADE again to confirm.");
        return;
        }

		act("$N says 'Your kennel account has been upgraded.'",ch,NULL,mob,TO_CHAR);
		ch->kennelupg++;
		ch->attemptingupgrade=0;
		if (ch->kennelupg==1){
		act("$N says 'You may now store 8 pets.'",ch,NULL,mob,TO_CHAR);}
		if (ch->kennelupg==2){
		act("$N says 'You may now store 11 pets.'",ch,NULL,mob,TO_CHAR);}
		if (ch->kennelupg==3){
		act("$N says 'You may now store 14 pets.'",ch,NULL,mob,TO_CHAR);}
		if (ch->kennelupg==4){
		act("$N says 'You may now store 17 pets.'",ch,NULL,mob,TO_CHAR);}
		if (ch->kennelupg==5){
		act("$N says 'You may now store 20 pets.'",ch,NULL,mob,TO_CHAR);}
		deduct_cost(ch,upgcost);
		mob->gold += upgcost;
		limit_mobile_wealth(mob);
		return;
	}


    if (!str_prefix(arg1,"store"))
    {
	int cost  = 2000;
	if (cost > (ch->gold * 100 + ch->silver))
	{
		act("$N says 'You do not have enough gold to store your pet.'",ch,NULL,mob,TO_CHAR);
		return;
	}
    if(!ch->pet)
	{
		act("$N says 'You don't even have a pet!'",ch,NULL,mob,TO_CHAR);
		return;
	}

	if(ch->pet->in_room != ch->in_room)
	{
		act("$N says 'Your pet is not here.'",ch,NULL,mob,TO_CHAR);
		return;
	}

	if(!IS_SET(ch->pet->act, ACT_PET) || IS_SET(ch->pet->act, ACT_AGGRESSIVE))
	{
		act("$N says 'Your pet cannot be kept here!'",ch,NULL,mob,TO_CHAR);
		return;
	}

	if (save_pet(ch) == true)
	{
		act("$N says 'I will keep your pet safe here.'",ch,NULL,mob,TO_CHAR);
		deduct_cost(ch,cost);
		mob->gold += cost;
		limit_mobile_wealth(mob);
	}
	else{
		act("$N says 'You will need to remove one of your other pets first. No more room.'",ch,NULL,mob,TO_CHAR);
        if (ch->kennelupg<5){
        act("$N says 'You can purchase an upgrade for more room.'",ch,NULL,mob,TO_CHAR);}
        }
	return;
    }

    if (!str_prefix(arg1,"release"))
    {
	if (IS_NULLSTR(arg1))
    	{
		act("$N says 'You need to give me the number of the pet you want to release.'",ch,NULL,mob,TO_CHAR);
		return;
    	}

	if(ch->pet)
    	{
		act("$N says 'You already have a pet!'",ch,NULL,mob,TO_CHAR);
		return;
    	}

	if (load_pet(ch, arg2) == true)
	{
		act("$N says 'Enjoy your pet.'",ch,NULL,mob,TO_CHAR);
	    	act( "$n now follows you.", ch->pet, NULL, ch, TO_VICT );
	}
	else
		act("$N says 'You do not have a pet with that number.'",ch,NULL,mob,TO_CHAR);
	return;
    }

    act("$N says 'I can LIST, STORE, or REMOVE your pets.'",ch,NULL,mob,TO_CHAR);
    act("$N says 'If you need more storage I can UPGRADE your kennel account.'"
    ,ch,NULL,mob,TO_CHAR);
    return;
}
/**************************************************************************/
bool pet_train (char_data * ch, int failure)
{
	if ( failure > 0 )
	{
		act( "You cannot seem to make your pet understand you.", ch, NULL, NULL, TO_CHAR );
		if ( !is_affected( ch, gsn_cause_headache ))
		{
			AFFECT_DATA af;

			ch->println( "Your head seems to explode with a sudden wave of indescribable pain!" );
			af.where		= WHERE_MODIFIER;
			af.type			= gsn_cause_headache;
			af.level		= ch->level/2;
			af.duration		= 5;
			af.location		= APPLY_SD;
			af.modifier		= - ch->level/10;
			af.bitvector	= 0;
			affect_to_char( ch, &af );
		}
		return false;
	}
	return true;
}
/**************************************************************************/
#define PET_BALL        	602



/**************************************************************************/
void do_pet(char_data *ch, char *argument)
{
    if (ch->casttimer1 >= 1){
    ch->println( "You can not do that while preparing a spell." );
		return;
    }

    char_data *pet;
    char arg1[MIL];
    char arg2[MIL];
    OBJ_DATA *obj=NULL;
    int modifier = 0;
    int failure = 0;
    int percent  = number_percent();

    if (ch->position == POS_SLEEPING)
    {
	ch->println("You can't do that while sleeping.");
        return;
    }
    if (!ch->pet)
    {
        ch->println("You do not have a pet right now.");
        return;
    }
    if(!IS_SET(ch->pet->act, ACT_PET))
   {
        ch->println("You do not have a pet right now.");
        return;
    }

    if ( ch->fighting ) {
	ch->println("You seem to be too busy right now.");
	return;
    }

    pet = ch->pet;

    obj = get_eq_char(ch, WEAR_HOLD);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!str_prefix(arg1,"train"))
    {
    	if (ch->pet->in_room != ch->in_room)
    	{
        	ch->println("Your pet is not here.");
	        return;
    	}

	modifier = ( ch->level - pet->level );
	if ( modifier > 0 ) modifier *= 3;
	else modifier *=5;
	modifier += 50;
	modifier += (( ch->modifiers[STAT_EM] +  ch->modifiers[STAT_EM] +ch->modifiers[STAT_PR] )/3);
	modifier += ((( get_skill( ch, gsn_animal_training) - 50 ) / 5) + (get_skill(ch, gsn_animal_training) > 0) ? 5 : 0);
	failure = percent - modifier;

	if (!str_prefix(arg2,"fetch") && obj != NULL && obj->pIndexData->vnum == PET_BALL)
        {

		if(IS_SET(pet->pet_norm_flags, PET_NORMTRICK_1))
		{
			ch->printlnf("%s already knows how to fetch.", capitalize(ch->pet->short_descr));
			return;
		}

		if(pet_train(ch, failure) == false)
		{
			ch->printlnf("You encourage %s to go fetch %s as you throw it.", capitalize(ch->pet->short_descr), obj->short_descr);
			act( "$n throws $p on the ground.", ch, obj, pet, TO_ROOM );
			obj_from_char(obj);
			obj_to_room(obj, ch->in_room);
			ch->printlnf("%s doesn't understand what you are trying to teach.", capitalize(ch->pet->short_descr));
			act( "$n attempts to train $M with $p.", ch, obj, pet, TO_ROOM );
			return;
		}

		SET_BIT(pet->pet_norm_flags, PET_NORMTRICK_1);
		ch->printlnf("You encourage %s to go fetch %s as you throw it.", capitalize(ch->pet->short_descr), obj->short_descr);
		act( "$n throws $p on the ground.", ch, obj, pet, TO_ROOM );
		act( "$M runs and grabs $p, and happily brings it back to $n.", ch, obj, pet, TO_ROOM );
		ch->printlnf("%s runs and grabs %s, and happily brings it to you.", capitalize(ch->pet->short_descr), obj->short_descr);
		ch->println("You pet has learned the trick FETCH!!");
		return;
	} // Fetch



    }


    if (!str_prefix(arg1,"trick"))
    {
    	if (ch->pet->in_room != ch->in_room)
    	{
        	ch->println("Your pet is not here.");
	        return;
    	}
	if (!str_prefix(arg2,"fetch") && obj != NULL && obj->pIndexData->vnum == PET_BALL)
        {
		if(!IS_SET(pet->pet_norm_flags, PET_NORMTRICK_1))
		{
			ch->printlnf("%s doesn't know how to fetch.", capitalize(ch->pet->short_descr));
			return;
		}
		obj_from_char(obj);
		obj_to_char(obj, pet);
		ch->printlnf("You throw %s for %s to fetch.", obj->short_descr, capitalize(ch->pet->short_descr));
		act( "$n throws $p on the ground for $M to fetch.", ch, obj, pet, TO_ROOM );
		act( "$N runs and grabs $p, and happily brings it back to $n.", ch, obj, pet, TO_ROOM );
		ch->printlnf("%s runs and grabs %s, and happily brings it back to you.", capitalize(ch->pet->short_descr), obj->short_descr);
		return;
	} // Fetch

    }

    ch->printlnf("%s looks at you in confusion.", capitalize(ch->pet->short_descr));
    return;
}
/**************************************************************************/

