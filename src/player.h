#pragma once

#include <stddef.h>

typedef struct player_t {	
	char *player_name;	
	int elo;
	/* other stuff */
} player_t;

typedef struct player_pairings_t {
	size_t length, match_size, not_paired;
  player_t **player_pairings; // A 2D array of players
  player_t *players_not_paired;
} player_pairings_t;

int init_player(player_t *p, char *name, int elo);
int copy_player(player_t *dest, player_t *src);
int free_player(player_t *p);

int pair_players(player_t *p, int len, int match_size, player_pairings_t *out);
int free_pairings(player_pairings_t *p);

