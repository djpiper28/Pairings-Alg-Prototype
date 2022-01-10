#include <stdlib.h>
#include <string.h>
#include <jemalloc/jemalloc.h>
#include "player.h"

int init_player(player_t *p, char *name, int elo) {
 	int status = 1;
  int len = strlen(name) + 1;
	char *name_cpy = malloc(sizeof * name_cpy * len);
	
	if (name_cpy == NULL) {
    status = 0;
	} else {
		strcpy(name_cpy, name);	
		
		p->elo = elo;
		p->player_name = name_cpy;
	}

	return status;
}

int copy_player(player_t *dest, player_t *src) {
	return init_player(dest, src->player_name, src->elo);
}

int free_player(player_t *p) {
  int status = 1;

  if (p == NULL) {
		status = 0;
	} else if (p->player_name == NULL) {
		status = 0;
	} else {
		free(p->player_name);
		memset(p, 0, sizeof * p);		
	}

	return status;
}

int pair_players(player_t *p, int len, int match_size, player_pairings_t *out) {
	int status = 1;
	int max = len % match_size;
	int paired = 0;
	int *player_is_paired = malloc(sizeof * player_is_paired * len);

	out->match_size = match_size;
	out->length = paired;

	out->players_not_paired = malloc(sizeof * out->players_not_paired * len - max);
	status |= out->players_not_paired != NULL;
	
	if (status) {
		out->player_pairings = malloc(sizeof * out->player_pairings * max);
		status |= out->player_pairings != NULL;

		if (status) {
			int i = 0;	
			for (; status && i < max; i++) {
				out->player_pairings[i] = malloc(sizeof ** out->player_pairings * match_size);
				status |= out->player_pairings[i] != NULL;
			}
		}
	}
	
	if (status) {
		while (paired < max) {
					
		}	
	} else {
		free_pairings(out);
	}

	return status;
}

int free_pairings(player_pairings_t *p) {
	int status = 1;
	if (p->player_pairings != NULL) {
		for (int i = 0; i < p->length; i++) {
			for (int j = 0; j < p->length; j++) {
				player_t player = p->player_pairings[i][j];
				free_player(&player);
			}

			if (p->player_pairings[i] != NULL) {
				free(p->player_pairings[i]);
			} else {
				status = 0;
			}
		}
		
		free(p->player_pairings);
	} else {
		status = 0;
	}

	if (p->players_not_paired != NULL) {
		for (int i = 0; i < p->not_paired; i++) {
			int s = free_player(&p->players_not_paired[i]);

			if (!s) {
				status = 0;
			}
		}
		free(p->players_not_paired);
	} else {
		status = 0;
	}
	
	p->match_size = 0;
	p->length = 0;
	p->not_paired = 0;

	return status;
}

