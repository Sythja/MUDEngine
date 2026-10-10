/**************************************************************************/
// mate.h - mate system header
/***************************************************************************

 **************************************************************************/
#ifndef MATE_H
#define MATE_H

#include "include.h"

enum mate_state{
	MS_REQUESTED_BY,// person being asked to mate
	MS_WAITING_FOR_REPLY,	// person asking
	MS_ABOUT_TO_START,
	MS_MATING,
	MS_REMOVE  // easy way to handle all removal from the linked list
};

// mate flags, not in dawn.h so adding a new flag doesn't require a 
// complete recompile

struct mate_node
{
	char_data	* victim;	// the other person involved in the mating process
	time_t		till;		// when the mate will last till
	mate_state	state;
	mate_node	*next;
};



class mate_data
{
public:
	// state questions
	bool is_known(char_data *victim); // true if they are known - find_char() knows about them
	bool has_been_requested_by(char_data *victim); 
	bool is_waiting_for_reply(char_data *victim); 
	bool is_about_to_start(char_data *victim); 
	bool is_active(char_data *victim); 
	bool is_mating(char_data* ch, char_data *victim); 
	
	void logout(char_data *ch, char_data *victim);  // when someone logs out

	void accept_mate(char_data *ch, char *argument); 
	void decline_mate(char_data *ch, char *argument); 
	void display_pkinfo(char_data *ch); 
										
	mate_data();	// constructor
	~mate_data();	// destructor

	void update_mates(char_data *ch); // updates the mates on the current character
	void update_request(char_data *victim, mate_state to_state); // adds a new node if required
	mate_node	*find_char(char_data *victim); // if the char is in the list of nodes, it is returned
private:
	mate_node	*first; 
	mate_node	*node; // workhorse node - used for all the loops etc
};

#endif
/**************************************************************************/
