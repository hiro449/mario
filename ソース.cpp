#include<conio.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>

#define FIELD_WIDTH (60)
#define FIELD_HEIGHT (16)

#define SCREEN_WIDTH (16)
#define SCREEN_HEIGHT (16)

#define UPDATE_FPS (60)
#define UPDATE_INTERVAL (1000 / UPDATE_FPS)


#define DRAW_FPS (10)
#define DRAW_INTERVAL (1000 / DRAW_FPS)


char course[FIELD_HEIGHT][FIELD_WIDTH] = {
"                                                        ",
"                                                        ",
"                     22222                              ",
"                      2222        222222                ",
"                                  222222                ",
"                                                        ",
"                            2                           ",
"                                                        ",
"                                                        ",
"                                                        ",
"                  2   222222                            ",
"  2                                                     ",
" 222                                                    ",
"22222      222222     222      22        22             ",
"11111111111111111    11111111111111111111111111111111111",
"11111111111111111    11111111111111111111111111111111111",
};

char screen[SCREEN_HEIGHT][SCREEN_WIDTH];


char aa[60][2 + 1];

typedef struct {
	float x, y;
}VEC2;

typedef struct {
	VEC2 position;
	VEC2 velocity;
	bool jumping;
}PLAYER;

PLAYER player;


bool Intersect(VEC2 _v) {
	int x = (int)_v.x;
	int y = (int)_v.y;
	switch (course[y][x]) {
	case '1':
	case '2':
		return true;

	}
	return false;
}

void Draw() {
	int scrollX = (int)roundf(player.position.x) - SCREEN_WIDTH / 2;
	if (scrollX < 0)
		scrollX = 0;

	for (int y = 0; y < SCREEN_HEIGHT; y++)
		for (int x = 0; x < SCREEN_WIDTH; x++)
			screen[y][x] = course[y][x + scrollX];

	{
		int x = (int)roundf(player.position.x) - scrollX;
		int y = (int)roundf(player.position.y);

		if((x > 0) && (y > 0) && (y < SCREEN_HEIGHT) && (x < SCREEN_WIDTH))
		screen[y][x] = '3';
	}

	system("cls");


	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++)
			printf("%s", aa[screen[y][x]]);
		printf("\n");
	}

}

bool Keypress[60];


void init() {

	player.position = { SCREEN_WIDTH / 2, 13 };
	player.velocity = {};
	player.jumping = false;

	memset(Keypress, 0, sizeof Keypress);

	Draw();

}

int main() {
	sprintf_s(aa[' '], "@");
	sprintf_s(aa['1'], " ");
	sprintf_s(aa['2'], "");
	sprintf_s(aa['3'], "™");

	init();

	


	clock_t lastUpdateClock = clock();
	clock_t lastDrawClock = clock();

	while (1) {
		clock_t nowClock = clock();

		if (nowClock >= lastUpdateClock + UPDATE_INTERVAL) {
			lastUpdateClock = nowClock;


			float acceleration = 0.005f;

			if(Keypress['2'])
				player.velocity.x += acceleration;

			if (Keypress['1'])
				player.velocity.x -= acceleration;
		
			player.position.x += player.velocity.x;

			player.position.y += player.velocity.y;

			player.velocity.y += 0.005f;
			player.velocity.x *= 0.95f;


			VEC2 down = {
				player.position.x + 0.5f,
				player.position.y + 1.0f
			};

			if (Intersect(down)) {
				player.position.y = floorf(player.position.y);
				player.velocity.y = 0;
				player.jumping = false;
			}
			else {
				player.jumping = true;
			}

			VEC2 right = {
				player.position.x + 1.0f,
				player.position.y + 0.5f
			};

			if (Intersect(right)) {
				player.position.x = floorf(player.position.x);
				player.velocity.x = 0;
			}


			VEC2 left = {
				player.position.x ,
				player.position.y + 0.5f
			};

			if (Intersect(left)) {
				player.position.x = floorf(player.position.x) + 1.0f;
				player.velocity.x = 0;
			}

			VEC2 up = {
				player.position.x + 0.5f,
				player.position.y 
			};

			if (Intersect(up)) {
				player.position.y = floorf(player.position.y) + 1.0f;
				player.velocity.y = 0;
			}
		}

		if (nowClock >= lastDrawClock + DRAW_INTERVAL) {
			lastDrawClock = nowClock;

			Draw();

			
		}
	


		if (_kbhit()) {
			switch (_getch()) {
			case '1':
				if (Keypress['2'])
					Keypress['2'] = false;
				else
					Keypress['1'] = true;

				break;

			case '2':
				if (Keypress['1'])
					Keypress['1'] = false;
				else
					Keypress['2'] = true;
				break;

			default:
				if(!player.jumping)
				player.velocity.y = -0.25f;
				break;


			}
	  }
		if (player.position.y > 16) {
			init();
		}

	}

	

}