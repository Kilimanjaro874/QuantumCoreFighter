#include "Back.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "define.h"
#include "string.h"

Back::Back(char back[NAME_SIZE], char effect1[NAME_SIZE], char effect2[NAME_SIZE]) {
	// --- �R���X�g���N�^ --- 
	
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
	// --- �w�i�`��֐� --- 
	// -- ������ -- 
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
	
	// -- �w�i�`�� -- 
	for (int i = 0; i < backNum; i++) {
		DrawGraph(x[i], y[i], backGh[i], true);				// �w�i1����
		DrawGraph(x[i], y[i] - height[i], backGh[i], true);	// �w�i2����
		if (y[i] >= height[i] + MARGINE) {
			y[i] = MARGINE;
		}
	}


	//DrawGraph(x, y, gh, false);				// �w�i1����
	//DrawGraph(x, y - height, gh, false);	// �w�i2����
	//if (y >= height + MARGINE) {
	//	y = MARGINE;
	//}

}

void Back::Move(float delta_time) {
	// --- �w�i�𓮍삳����֐� ---
	for (int i = 0; i < backNum; i++) {
		switch (i)
		{
		case 0:
			// �C�摜
			y[i] += SCROLL_SPEED * delta_time;
			break;

		case 1:
			// �g�摜
			y[i] += SCROLL_SPEED * delta_time;

			if (xScrool[i] == false) {
				// ���ֈړ�
				x[i] -= SCROLL_SPEED / 20 * delta_time;
				// �ړ�����
				if (x[i] < -100) {
					xScrool[i] = true;
				}
			}
			else if(xScrool[i] == true) {
				// �E�ֈړ�
				x[i] += SCROLL_SPEED / 20 * delta_time;
				// �ړ�����
				if (x[i] > 0) {
					xScrool[i] = false;
				}
			}
			
			break;

		case 2:
			// �܉摜
			y[i] += SCROLL_SPEED * 1.5 * delta_time;
			break;

		default:
			break;
		}
	}

}

void Back::All(float delta_time) {
	// --- �w�i�̑�����O������Ăяo�� ---
	Draw();
	Move(delta_time);
}