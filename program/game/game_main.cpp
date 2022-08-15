#include <time.h>
#include <algorithm>
#include "game_main.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "sceneDefine.h"

bool init = false;
int scene = SCENE_OPENING;


bool gameMain(float delta_time) {
	// * �Q�[���I������������false��Ԃ� *

	if (!init) {
		init = true;
	}
	
	// -- �V�[���؂�ւ�
	switch (scene)
	{
	case SCENE_OPENING:
		sceneOpening(delta_time);
		return true;
		break;

	case SCENE_TITLE:
		sceneTitle(delta_time);
		return true;
		break;
		
	case SCENE_CH_SELECT:
		sceneChSelect();
		return true;
		break;

	case SCENE_CH_1:
		scenePlayCh1(delta_time);
		return true;
		break;

	case END_GAME:
		return false;
		break;
	}

	return true;

}

void gameEnd() {
	// --- �Q�[���I�����Ɏ��s --- 
	
}