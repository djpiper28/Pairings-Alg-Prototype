#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <jemalloc/jemalloc.h>
#include "player.h"

float fact(float n) {
	float r = 1;
	for (float i = 1; i <= n; i++) {
		r *= i;
	}

	return r;
}

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

		snprintf(buffer, 1024, "Player-%03d", i);
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
			fprintf(stderr, "Successfully paired %d\n", pairings.length);
			float c = fact(match_size) / (2 * fact(match_size - 2));
	  	
			for (int i = 0; i < pairings.length; i++) {
				printf("Match #%03d Players: ", i);
				paired_match_t *match = &pairings.players_paired[i];
				
				for (int j = 0; j < match->length; j++) {
					player_t plr = match->players[j];
					printf("%s(%d)", plr.player_name, plr.elo);
					
					if (j + 1 == pairings.match_size) {
						printf(" ");
					} else {
						printf(", ");
					} 
				}

				// Print average elo difference
				float avg_difference = 0;
				for (int i = 0; i < match->length; i++) {
					for (int j = i + 1; j < match->length; j++, c++) {
						avg_difference += match->players[i].elo - match->players[j].elo;						
					}
				}

				avg_difference /= c;
				printf("Averageg elo delta: %3.3f\n", avg_difference);
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

