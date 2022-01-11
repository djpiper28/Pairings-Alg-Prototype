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

// Used for qsort()
static int compare_players(const void *a, const void *b) {
	player_t *pa = (player_t *) a;
	player_t *pb = (player_t *) b;

	return pa->elo - pb->elo;
}

int pair_players(player_t *p, int len, int match_size, player_pairings_t *out) {
	int status = 1;
	int max = len / match_size;
	int paired = 0;
	int *player_is_paired = malloc(sizeof * player_is_paired * len);
	if (player_is_paired == NULL) {
		return 0;
	} else {
		memset(player_is_paired, 0, sizeof * player_is_paired * len);
	}

	out->match_size = match_size;
	out->length = max;
	out->not_paired = len - (max * match_size);

	if (out->not_paired != 0) {
		out->players_not_paired = malloc(sizeof * out->players_not_paired * out->not_paired);
		status |= out->players_not_paired != NULL;
	} else {
		out->players_not_paired = NULL;
	}
	
	if (status) {		
		out->players_paired = malloc(sizeof * out->players_paired * max);
		status |= out->players_paired != NULL;
	}

	if (status) {
		for (int i = 0; status && i < max; i++) {
			status |= init_paired_match(&out->players_paired[i], match_size);			
		}
	}
	
	// Create an array of all possible pairings
	if (status) {
	
	}
	
	if (status) {
		// Sort the players by elo
		qsort(p, len, sizeof * p, compare_players);
		
		// TOOD: Use a less niave paired algorithm
		// Create pairings
		int top = 0;
		while (paired < max) {
			paired_match_t *match = &out->players_paired[paired];
			for (int i = 0; i < match_size; i++, top++) {
				player_t *plr = &p[top];
				status |= copy_player(&match->players[i], plr);
			}

			paired++;
		}

		// Create array of unpaired
		int i, offset;
		for (i = offset = 0; i < len && offset < out->not_paired; i++) {
			if (!player_is_paired[i]) {
				copy_player(&out->players_not_paired[offset], &p[i]);
				offset++;
			}
		}
	} else {
		free_pairings(out);
	}

	free(player_is_paired);

	return status;
}

int free_pairings(player_pairings_t *p) {
	int status = 1;
	if (p->players_paired != NULL) {
		for (int i = 0; i < p->length; i++) {
			free_paired_match(&p->players_paired[i]);
		}
		
		free(p->players_paired);
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

int init_paired_match(paired_match_t *m, int len) {
	m->length = len;
	m->players = malloc(sizeof * m->players * len);

	return m->players != NULL;
}

int free_paired_match(paired_match_t *m) {
	int status = 1;
	
	if (m->players == NULL) {
		status = 0;
	} else {
		for (int i = 0; i < m->length; i++) {
			free_player(&m->players[i]);
		}
		free(m->players);
	}

	return status;
}

