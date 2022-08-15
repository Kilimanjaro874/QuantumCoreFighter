#include "sceneDefine.h"
#include "../dxlib_ext/dxlib_ext.h"

#include "SceneManagerOpening.h"
#include "SceneManagerTitle.h"
#include "SelectCh.h"
#include "SceneManagerCh1.h"
#include "GameOption.h"

bool sceneInit = false;			// シーン初期化フラグ
int tempScene = 0;				// 暫定のシーン内での条件分岐用変数
// 
int choiceChapter = 0;			// ユーザのチャプターセレクト結果を格納

// -- 自作クラスを扱う(注意：必ずendGame()で削除する事) -- 

// - Opening -
SceneManagerOpening* sceneManagerOpening = nullptr;
// - Title -
SceneManagerTitle* sceneManagerTitle = nullptr;
// - ChapterSelect -
SelectCh* selectCh;
// - Ch. 1 - 
SceneManagerCh1* sceneManagerCh1 = nullptr;

// - オプションクラス(追々唯一化したい) -
GameOption gameOption;

// -- ゲーム終了時、インスタンスを破壊する関数 -- 
void endGame() {

	// - Opening -
	if(sceneManagerOpening != NULL)
		delete sceneManagerOpening;
	// - Title - 
	if (sceneManagerTitle != NULL)
		delete sceneManagerTitle;
	// - Chapter Select -
	if (selectCh != NULL) {
		delete selectCh;
	}
	// - Ch.1 - 
	if (sceneManagerCh1 != NULL) {
		delete sceneManagerCh1;
	}

}


void sceneOpening(float delta_time) {
	// -- オープニング画面 -- 
	// - シーン管理クラスのインスタンス作成 -
	if (!sceneInit) {
		sceneManagerOpening = new SceneManagerOpening();
		sceneInit = true;
	}

	// - オープニングクラス実行 -
	// 終了フラグに応じてシーン切替
	if (!sceneManagerOpening->All(delta_time)) {
		// タイトルに移動
		scene = SCENE_TITLE;
		endSceneOp();
	}

	// - エンター入力でオープニングスキップ - 
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		scene = SCENE_TITLE;
		endSceneOp();
	}
	
}


void sceneTitle(float delta_time) {

	// -- タイトル画面 --
	if (!sceneInit) {
		sceneManagerTitle = new SceneManagerTitle();
		sceneInit = true;
		tempScene = SCENE_TITLE;
	}
	
	// -- シーンクラス実行 --
	sceneManagerTitle->All(delta_time);
	// - BGM, SE音量変更 -
	if (gameOption.GetVolumeFlag()) {
		int tempBgm, tempSe;
		gameOption.GetVolume(&tempBgm, &tempSe);
		sceneManagerTitle->SetVolume(tempBgm, tempSe);
	}

	// -- tempSceneに応じて選択肢分岐 --
	switch (tempScene)
	{
	case SCENE_TITLE:
		// タイトルシーン 
		// - 選択肢に応じてシーン切替(Enter or Space 入力) -
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) || tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
			tempScene = sceneManagerTitle->GetUserChoice();
		}

		break;

	case SCENE_CH_SELECT:
		// チャプターセレクトに移動
		scene = SCENE_CH_SELECT;
		endSceneTitle();
		break;

	case SCENE_OPTION:
		// シーン遷移は行わず、ゲームオプションを開く
		if (!gameOption.All()) {

		}
		else {
			tempScene = SCENE_TITLE;
		}

		break;

	case END_GAME:
		// ゲーム終了
		scene = END_GAME;
		break;

	default:
		// エラー
		break;
	}
	
}


void sceneChSelect() {
	// - チャプターセレクト画面から各チャプターへ - 
	if (!sceneInit) {
		selectCh = new SelectCh();
		//  - BGM, SE音量を適用 -
		selectCh->All();
		int tempSE, tempBGM;
		gameOption.GetVolume(&tempBGM, &tempSE);
		selectCh->SetSEVolume(tempBGM, tempSE);

		sceneInit = true;
	}

	// - チャプターセレクトを実行
	selectCh->All();
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		// 選択したチャプターを格納
		choiceChapter = selectCh->All();
		scene = SCENE_CH_1;
		endSceneChSelect();
	}

}


// -- チャプター1プレイ関数 --
// - Start -
void scenePlayCh1(float delta_time) {

	// - choiceChapterの結果に応じて読み込む背景・スポーンする敵機リスト変化
	if (!sceneInit) {
		char backName[NAME_SIZE];
		char effect1[NAME_SIZE];
		char effect2[NAME_SIZE];

		switch (choiceChapter)
		{
		case 0:

			strcpy(backName, "graphics/Back/City2.png");
			strcpy(effect1, "graphics/Back/wave.png");
			strcpy(effect2, "graphics/Back/Cloud2.png");

			break;

		case 1:
			strcpy(backName, "graphics/Back/Sea.png");
			strcpy(effect1, "graphics/Back/wave.png");
			strcpy(effect2, "graphics/Back/Cloud2.png");

			break;

		case 2:
			strcpy(backName, "graphics/Back/Sea.png");
			strcpy(effect1, "graphics/Back/wave.png");
			strcpy(effect2, "graphics/Back/Cloud2.png");

			break;

		case 4:
			strcpy(backName, "graphics/Back/Sea.png");
			strcpy(effect1, "graphics/Back/wave.png");
			strcpy(effect2, "graphics/Back/Cloud2.png");

			break;

		case 5:
			strcpy(backName, "graphics/Back/Sea.png");
			strcpy(effect1, "graphics/Back/wave.png");
			strcpy(effect2, "graphics/Back/Cloud2.png");

			break;

		default:
			break;
		}

		sceneManagerCh1->All(delta_time);				// 一度描画＆音声処理初期化を完了する
		// - BGM,SE音量を適用
		int tempBGM, tempSE;
		gameOption.GetVolume(&tempBGM, &tempSE);
		sceneManagerCh1->SetVolume(tempBGM, tempSE);

		sceneInit = true;
	}


	//// - クラスのインスタンスを作成 - 
	//if (!sceneInit) {
	//	sceneManagerCh1 = new SceneManagerCh1();
	//	sceneManagerCh1->All(delta_time);				// 一度描画＆音声処理初期化を完了する
	//	// - BGM,SE音量を適用
	//	int tempBGM, tempSE;
	//	gameOption.GetVolume(&tempBGM, &tempSE);
	//	sceneManagerCh1->SetVolume(tempBGM, tempSE);

	//	sceneInit = true;
	//}

	// - Ch. 1を再生 - 
	if (tempScene != SCENE_OPTION) {
		tempScene = sceneManagerCh1->All(delta_time);
	}

	// -- tempSceneに応じて選択肢分岐 --
	switch (tempScene)
	{
	case SCENE_NON_CHANGE:
		// 何もしない

		break;

	case SCENE_CH_SELECT:
		// チャプターセレクトに移動
		scene = SCENE_CH_SELECT;
		endScenePlayCh1();
		break;

	case SCENE_TITLE:
		// タイトル画面に移動
		scene = SCENE_TITLE;
		endScenePlayCh1();
		break;

	case SCENE_OPTION:
		// シーン遷移は行わず、ゲームオプションを開く
		if (!gameOption.All()) {
			tempScene = SCENE_OPTION;
			// 音量設定を反映する
			int tempBGM, tempSE;
			gameOption.GetVolume(&tempBGM, &tempSE);
			sceneManagerCh1->SetVolume(tempBGM, tempSE);

		}
		else {
			tempScene = SCENE_NON_CHANGE;
		}

		break;

	case SCENE_CH_1:
		// 状態を初期化して再トライ
		endScenePlayCh1();

		break;

	case END_GAME:
		// ゲーム終了
		scene = END_GAME;
		break;

	default:
		// エラー
		break;
	}


	
}
// -- End -- 
// シーンクラスを破壊
void endSceneOp() {
	sceneInit = false;
	delete sceneManagerOpening;
	sceneManagerOpening = nullptr;
}

void endSceneTitle() {
	sceneInit = false;
	delete sceneManagerTitle;
	sceneManagerTitle = nullptr;
}

void endSceneChSelect() {
	sceneInit = false;
	delete selectCh;
	selectCh = nullptr;
}

void endScenePlayCh1() {
	sceneInit = false;
	delete sceneManagerCh1;
	sceneManagerCh1 = nullptr;
}


