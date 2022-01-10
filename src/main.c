#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <jemalloc/jemalloc.h>
#include "player.h"

int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s [PLAYERS] [MATCH SIZE]\n", argv[0]);
		return 1;
	}
	
	int players = atoi(argv[1]);
	int match_size = atoi(argv[2]);

	if (match_size < 2) {
		fprintf(stderr, "Error: the match size must be greater than or equal to 2\n");
		return 1;
	}

	if (players < 1) {
		fprintf(stderr, "Error: there must be one or more players\n");
		return 1;
	}

	fprintf(stderr, "Pairing %d players into %d pods of size %d\n", players, players / match_size, match_size);
	long int start_time = time(NULL);

	fprintf(stderr, "Creating players...\n");
	srand(time(NULL));	
	player_t *p = malloc(sizeof * p * players);
	int status = 1;
	for (int i = 0; status && i < players; i++) {
		char buffer[1024];
		int elo = abs(rand()) % 3000;

		snprintf(buffer, 1024, "Player-%d", i);
		status = init_player(&p[i], buffer, elo);
		if (!status) {
			players = i + 1;
		}
	}

	if (status) {
		fprintf(stderr, "Pairing players..\n");
		
  	player_pairings_t pairings;  	
	  int res = pair_players(p, players, match_size, &pairings);

	  if (res) {
			fprintf(stderr, "Successfully paired %ld\n", pairings.length);
	  	
			for (int i = 0; i < pairings.length; i++) {
				printf("Match #%03d Players: ", i); 
				for (int j = 0; j < pairings.match_size; j++) {
					player_t plr = pairings.player_pairings[i][j];
					char *p_name = plr.player_name;
					printf("%s", p_name);
					
					if (j + 1 == pairings.match_size) {
						printf("\n");
					} else {
						printf(", ");
					}
				}
			}

			for (int i = 0; i < pairings.not_paired; i++) {
				player_t plr = pairings.players_not_paired[i];
				printf("%s is not paired\n", plr.player_name);
			}
		} else {
			fprintf(stderr, "An error occurred pairing the players\n");
		}

		res = free_pairings(&pairings);
	} else {
		fprintf(stderr, "An error occurred during memory allocation\n");
	}
	
	fprintf(stderr, "Freeing players...\n");
	for (int i = 0; i < players; i++) {
		free_player(&p[i]);
	}

	long int end_time = time(NULL);
	fprintf(stderr, "Completed in %ld seconds\n", end_time - start_time);	

	return 0;
}

