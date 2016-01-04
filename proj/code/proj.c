#include <minix/drivers.h>
#include <stdlib.h>

#include "constants.h"
#include "game.h"
#include "xpm.h"

static int proc_args(int argc, char *argv[]);

int main(int argc, char **argv) {

	/* Initialize service */

	sef_startup();

	char *video_mem;
	video_mem = vg_init(VBE_105_MODE);

	while (1) {
		if (menu(video_mem) == -1) {
			break;
		}

		int n_seconds_player1 = game(video_mem, 1, -1);
		if (n_seconds_player1 == -1) {
			break;
		}

		int n_seconds_player2 = game(video_mem, 2, n_seconds_player1);
		if (n_seconds_player2 == -1) {
			break;
		}

		if (n_seconds_player1 > n_seconds_player2) {
			player_wins(video_mem, 2);
		}
		else {
			player_wins(video_mem, 1);
		}
	}

	vg_exit();


	return 0;

}
