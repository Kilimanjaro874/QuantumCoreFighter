#include "Back.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "define.h"
#include "string.h"

Back::Back(char back[NAME_SIZE], char effect1[NAME_SIZE], char effect2[NAME_SIZE]) {
	// --- ƒRƒ“ƒXƒgƒ‰ƒNƒ^ --- 
	
	for (int i = 0; i < backNum; i++) {
		x[i] = MARGINE;
		y[i] = MARGINE;
		backGh[i] = 0;
		width[i] = 0;
		height[i] = 0;
		xScrool[i] = false;
	}
	drawInit = false;
	strcpy(this->backName, back);
	strcpy(this->effect1, effect1);
	strcpy(this->effect2, effect2);

}

void Back::Draw() {
	// --- ”wŒi•`‰æŠÖ” --- 
	// -- ‰Šú‰» -- 
	if (!drawInit) {
	/*	backGh[0] = LoadGraph("graphics/Back/Sea.png");
		backGh[1] = LoadGraph("graphics/Back/wave.png");
		backGh[2] = LoadGraph("graphics/Back/Cloud2.png");*/
		backGh[0] = LoadGraph(backName);
		backGh[1] = LoadGraph(effect1);
		backGh[2] = LoadGraph(effect2);
		for (int i = 0; i < backNum; i++) {
			GetGraphSize(backGh[i], &width[i], &height[i]);
		}
		drawInit = true;
	}
	
	// -- ”wŒi•`‰æ -- 
	for (int i = 0; i < backNum; i++) {
		DrawGraph(x[i], y[i], backGh[i], true);				// ”wŒi1–‡–Ú
		DrawGraph(x[i], y[i] - height[i], backGh[i], true);	// ”wŒi2–‡–Ú
		if (y[i] >= height[i] + MARGINE) {
			y[i] = MARGINE;
		}
	}


	//DrawGraph(x, y, gh, false);				// ”wŒi1–‡–Ú
	//DrawGraph(x, y - height, gh, false);	// ”wŒi2–‡–Ú
	//if (y >= height + MARGINE) {
	//	y = MARGINE;
	//}

}

void Back::Move(float delta_time) {
	// --- ”wŒi‚ğ“®ì‚³‚¹‚éŠÖ” ---
	for (int i = 0; i < backNum; i++) {
		switch (i)
		{
		case 0:
			// ŠC‰æ‘œ
			y[i] += SCROLL_SPEED * delta_time;
			break;

		case 1:
			// ”g‰æ‘œ
			y[i] += SCROLL_SPEED * delta_time;

			if (xScrool[i] == false) {
				// ¶‚ÖˆÚ“®
				x[i] -= SCROLL_SPEED / 20 * delta_time;
				// ˆÚ“®§ŒÀ
				if (x[i] < -100) {
					xScrool[i] = true;
				}
			}
			else if(xScrool[i] == true) {
				// ‰E‚ÖˆÚ“®
				x[i] += SCROLL_SPEED / 20 * delta_time;
				// ˆÚ“®§ŒÀ
				if (x[i] > 0) {
					xScrool[i] = false;
				}
			}
			
			break;

		case 2:
			// “Ü‰æ‘œ
			y[i] += SCROLL_SPEED * 1.5 * delta_time;
			break;

		default:
			break;
		}
	}

}

void Back::All(float delta_time) {
	// --- ”wŒi‚Ì‘€ì‚ğŠO•”‚©‚çŒÄ‚Ño‚µ ---
	Draw();
	Move(delta_time);
}