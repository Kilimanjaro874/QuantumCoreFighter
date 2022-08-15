#include "SceneManagerCh1.h"
#include "define.h"
#include "sceneDefine.h"


// - メニュー画面：選択肢定義

// ポーズ画面
#define P_CONTINUE_SELECT 0
#define P_OPTION_SELECT 1
#define P_CHAPTER_SELECT 2
#define P_TITLE_SELECT 3

// ゲームオーバー画面
#define CONTINUE_SELECT 0			// コンティニューセレクト
#define RETRY 1						// チャプターリロード
#define TITLE_SELECT 2				// タイトルセレクト

SceneManagerCh1::SceneManagerCh1(int chapterNum, char backName[NAME_SIZE], char effect1[NAME_SIZE], char effect2[NAME_SIZE], char enemyData[NAME_SIZE]) {
	// --- コンストラクタ --- 
	
	// -- 変数初期処理 --
	// - 選択したチャプター番号 -
	this->chapterNum = chapterNum;
	// - 時間 - 
	totalTime = 0;
	// - 座標情報 --
	playerX = 0;		// プレイヤー座標
	playerY = 0;
	// - スポーン -
	enemySpawnCount = 0;
	spawnEndFlag = false;
	// - All初期処理フラグ -
	allInit = false;
	// - ポーズ画面 -
	pauseDrawInit = false;
	pauseFlag = false;
	pauseWaitTime = 0;
	pauseBackGh = 0;
	pauseTitleLogoGh = 0;
	pauseSelectLogo = 0;
	pauseHelpLogo = 0;
	pauseHelpFlag = false;
	// - ゲームオーバー画面
	gameOverDrawInit = false;
	gameOverFlag = false;
	gameOverWindowTime = 0;
	gameOverTitle = 0;
	gameOverSelectLogo = 0;
	// メニュー画面
	fs_choiceSE = false;
	hs_choiceSE = 0;
	fs_decisionSE = false;
	hs_decisionSE = 0;
	// - リザルト画面 - 
	resultFlag = false;
	resultInit = false;
	resultBackGh = 0;
	resultTitleGh = 0;
	resultLogo1Gh = 0;
	resultLogo2Gh = 0;
	numGh[10] = { 0 };
	resultWindowTime = 0;
	// - 効果音ハンドル等 -
	soundInit = false;			// SoundAll()内で初期化		
	hb_stageSound = 0;
	hb_bossSound = 0;
	hs_playerRedShot01 = 0;
	hs_playerBlueShot01 = 0;
	hs_playerYellowShot01 = 0;
	hs_playerDead = 0;
	hs_playerTransform = 0;
	fs_playerDamaged = false;
	hs_playerDamaged = 0;
	fs_playerBombShot = false;
	hs_playerBombShot = 0;		
	fs_playerBombExplode = false;	
	hs_playerBombExplode = 0;	
	fs_playerRefrect = false;
	hs_playerRefrect = 0;
	fs_shiftedFlying = false;
	hs_playerFlying = 0;
	hs_playerShiftedFlying = 0;
	fs_enemyDead = false;
	hs_enemyDead = 0;
	fs_enemyDamaged = false;
	hs_enemyDamaged = 0;
	fs_itemGot = false;
	hs_itemGot = 0;
	fs_enemyShot01 = false;
	hs_enemySho01 = 0;
	fs_enemyReflect = false;
	hs_enemyReflect = 0;
	// - BGM,SE音量 -
	bgmVolume = BGM_INITIAL_VOLUME;
	seVolume = SE_INITIAL_VOLUME;
	// - 画面遷移先格納 -
	tempScene = SCENE_NON_CHANGE;
	// - ボスフラグ関係 -
	bossExistFlag = false;
	bossExistInit = false;
	deadBossCounter = 0;
	// チャプター番号に応じてボスの最大数変化
	switch (chapterNum)
	{
	case 1:
		bossNumMax = 3;
		break;

	default:
		bossNumMax = 1;
		break;
	}
	// チュートリアル
	tutorialFlag = false;
	tutorialEndFlag = false;
	// -- インスタンスの作成 -- 
	player = new Player();		// プレイヤー
	back = new Back(backName, effect1, effect2);			// 背景
	score = new Score();		// スコアボード
	for (int i = 0; i < ITEM_NUM; i++) {
		// アイテムクラス
		item[i] = new Item;
	}

	// -- 敵データの読み込み(enemyControlData.csv 参照) --
	// ※ インスタンスはAll()内で管理
	FILE* fp;
	char buff[100];
	int c1;
	int col = 1;
	int row = 0;
	int err;
	memset(buff, 0, sizeof(buff));
	// ファイルを読み取りで開く
	//err = fopen_s(&fp, "enemyControlDataCh1.csv", "r");
	err = fopen_s(&fp, enemyData, "r");
	if (err != 0) {
		DrawStringEx(200, 200, -1, "file not open!\n");
	}

	// ヘッダファイル読み飛ばし(3行読み飛ばし)
	while (fgetc(fp) != '\n');
	while (fgetc(fp) != '\n');
	while (fgetc(fp) != '\n');

	while (true) {
		while (true) {
			c1 = fgetc(fp);
			// 末尾ならループを抜ける
			if (c1 == EOF) {
				goto out;
			}
			// カンマ, 改行で無ければ文字としてつなげる
			if (c1 != ',' && c1 != '\n')
				strcat(buff, (const char*)&c1);
			else
				break;
		}
		// 1セル分の文字列をデータに格納していく
		switch (col)
		{
		case 1:	 edata[row].x				= atoi(buff); break;
		case 2:  edata[row].y				= atoi(buff); break;
		case 3:  edata[row].type			= atoi(buff); break;
		case 4:  edata[row].stype			= atoi(buff); break;
		case 5:  edata[row].s_pattern		= atoi(buff); break;
		case 6:  edata[row].item			= atoi(buff); break;
		case 7:  edata[row].m_pattern		= atoi(buff); break;
		case 8:  edata[row].in_time			= atof(buff); break;
		case 9:  edata[row].start_fire_time = atof(buff); break;
		case 10: edata[row].end_fire_time	= atof(buff); break;
		case 11: edata[row].out_time		= atof(buff); break;
		case 12: edata[row].outX			= atoi(buff); break;
		case 13: edata[row].outY			= atoi(buff); break;
		case 14: edata[row].moveK[0]		= atoi(buff); break;
		case 15: edata[row].moveK[1]		= atoi(buff); break;
		case 16: edata[row].moveK[2]		= atoi(buff); break;
		case 17: edata[row].moveK[3]		= atoi(buff); break;
		case 18: edata[row].moveK[4]		= atoi(buff); break;
		case 19: edata[row].moveK[5]		= atoi(buff); break;
		case 20: edata[row].moveK[6]		= atoi(buff); break;
		case 21: edata[row].moveK[7]		= atoi(buff); break;
		case 22: edata[row].moveK[8]		= atoi(buff); break;
		case 23: edata[row].moveK[9]		= atoi(buff); break;
		case 24: edata[row].moveK[10]		= atoi(buff); break;
		case 25: edata[row].moveK[11]		= atoi(buff); break;
		case 26: edata[row].moveK[12]		= atoi(buff); break;
		}
		// バッファ初期化
		memset(buff, 0, sizeof(buff));
		// 列数を足す
		++col;
		// もし読み込んだ文字が改行であれば、列数初期化＆行数増加
		if (c1 == '\n') {
			col = 1;
			++row;
		}
		
	}
out:
	fclose(fp);	// ファイルを閉じる
	// - 敵データ読み込み終了 -
	// ->クラスはAll関数内でインスタンス化する. 暫定敵にnullpointerを与える
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i] = nullptr;
	}


}

SceneManagerCh1::~SceneManagerCh1() {
	// --- デストラクタ --- 

	// -- BGM停止 --
	if (CheckSoundMem(hb_stageSound)) {
		StopSoundMem(hb_stageSound);
	}

	if (CheckSoundMem(hb_bossSound)) {
		StopSoundMem(hb_bossSound);
	}

	// -- 飛行音停止 --
	if (CheckSoundMem(hs_playerFlying)) {
		StopSoundMem(hs_playerFlying);
	}
	if (CheckSoundMem(hs_playerShiftedFlying)) {
		StopSoundMem(hs_playerShiftedFlying);
	}
	// -- インスタンス破壊 --
	delete player;
	delete back;
	delete score;
	for (int i = 0; i < enemySpawnCount; i++) {
		if (enemy[i] != NULL) {
			delete enemy[i];
		}
	}
	for (int i = 0; i < ITEM_NUM; i++) {
		delete item[i];
	}

}

int SceneManagerCh1::All(float delta_time) {
	// ---  シーン＆オブジェクトの管理 ---
	
	// -- デバッグ用機能 --
	if (tnl::Input::IsKeyDown(eKeys::KB_LCONTROL) && tnl::Input::IsKeyDown(eKeys::KB_T)) {
		delta_time = delta_time * 5;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_LCONTROL) && tnl::Input::IsKeyDown(eKeys::KB_P)) {
		player->SetRespawn();
		for (int i = 0; i < 9; i++) {
			for (int k = 0; k < 5; k++) {
				player->SetItem(k);
			}
		}
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_LCONTROL) && tnl::Input::IsKeyDown(eKeys::KB_K)) {
		for (int i = 0; i < enemySpawnCount; i++) {
			if (enemy[i] != NULL) {
				enemy[i]->SetDamage(NON_ELEMENT, 9999);
				enemy[i]->DamageCalc();
			}
		}
	}

	// -- チュートリアル(画面表示は後に実行) --
	if (chapterNum == 0 && !tutorialEndFlag) {
		tutorialFlag = true;
		// ポーズ実行
		delta_time = 0;
	}

	// -- 初期処理 --
	if (!allInit) {
		// - マウスカーソル位置非表示
		SetMouseDispFlag(false);
		allInit = true;
	}

	// -- ボス出現 --
	if (BossExistFlag()) {
		bossExistFlag = true;
	}
	
	// - ボス撃破数カウント -
	if (BossDefeatFlag()) {
		deadBossCounter++;
	}

	// -- ゲームクリア確認(リザルト画面表示は後に実行) --
	if (deadBossCounter >= bossNumMax && !resultFlag) {
		// リザルト画面表示
		resultFlag = true;
		// リザルト画面表示ディレイカウンタ
		resultWindowTime = 1.5;
	}
	if (resultFlag && resultWindowTime <= 0) {
		// - ポーズ実行
		delta_time = 0;
	}


	// -- ゲームオーバー処理(ゲームオーバー画面表示は後に実行 --
	if (player->GetLifeNum() <= 0 && !gameOverFlag) {
		// プレイヤーのライフ数がなくなった時
		gameOverFlag = true;
		// - ウィンドウ表示ディレイカウンタ -
		gameOverWindowTime = 2.0;
	}
	
	if (gameOverFlag && gameOverWindowTime <= 0) {
		// - ポーズ実行
		delta_time = 0;
	}

	// -- ボーズ実行(ボーズ画面表示は後に実行) -- 
	if (!pauseFlag) {
		// - ESC入力でボーズフラグを立てる(ゲームオーバー中＆リザルト中は実施出来ない) -
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE) && !gameOverFlag && !resultFlag) {
			pauseFlag = true;
			// - チャタリング防止用カウンタ -
			pauseWaitTime = 1.0;
			// - 操作説明画像表示フラグを折る
			pauseHelpFlag = false;
		}
	} else {
		// - ゲーム刻み時間を0にする - 
		delta_time = 0;
		pauseWaitTime -= 0.1;
		if (pauseWaitTime <= 0) {
			pauseWaitTime = 0;
		}
		// - ESC入力でゲーム続行 -
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE) && pauseWaitTime <= 0) {
			pauseFlag = false;
		}
	}

	// -- シーン管理 --
	// - 描画領域指定 - 
	SetDrawArea(MARGINE, MARGINE, WIDTH - MENU_WIDTH, HEIGHT - MARGINE);

	// -- オブジェクト管理 --
	// 背景描写
	back->All(delta_time);

	// プレイヤー座標を取得
	GetPlayerPosition();

	// 敵操作
	// - タイムテーブルの上から順に敵をスポーンする
	if (!spawnEndFlag) {
		if (edata[enemySpawnCount].in_time <= totalTime) {
			// 経過時間に応じて敵をスポーン
			enemy[enemySpawnCount] = new Enemy(
				totalTime,
				(double)edata[enemySpawnCount].x,
				(double)edata[enemySpawnCount].y,
				edata[enemySpawnCount].type,
				edata[enemySpawnCount].stype,
				edata[enemySpawnCount].s_pattern,
				edata[enemySpawnCount].item,
				edata[enemySpawnCount].m_pattern,
				edata[enemySpawnCount].in_time,
				edata[enemySpawnCount].start_fire_time,
				edata[enemySpawnCount].end_fire_time,
				edata[enemySpawnCount].out_time,
				(double)edata[enemySpawnCount].outX,
				(double)edata[enemySpawnCount].outY,
				edata[enemySpawnCount].moveK
			);
			enemySpawnCount++;

			if (enemySpawnCount >= ENEMY_NUM) {
				// タイムテーブルの敵を全てスポーン完了
				enemySpawnCount = ENEMY_NUM;
				spawnEndFlag = true;
			}
		}
	}

	// - 敵を動かす。役目を終えた敵は消去する - 
	for (int i = 0; i < enemySpawnCount; i++) {
		if (enemy[i] != NULL) {

			// 敵が死亡した瞬間（1フレーム)のフラグ確認＆処理実施
			if (enemy[i]->GetDeadMoment()) {
				// 敵死亡効果音フラグを有効化
				fs_enemyDead = true;
				// スコアに得点を加算する
				score->SetScore(CURRENT_SCORE, enemy[i]->GetScore(totalTime));
				// アイテム出現
				for (int k = 0; k < ITEM_NUM; k++) {
					// フラグ未セットのアイテム走査＆セット
					if (!item[k]->GetFlag()) {
						double enemyItemX, enemyItemY;
						enemy[i]->GetPosition(&enemyItemX, &enemyItemY);
						item[k]->SetFlag(enemyItemX, enemyItemY, enemy[i]->GetItem());
						break;
					}
				}

			}

			// 敵のAll関数実行。同時に、死亡フラグ＆発射弾の無い敵は消去する
			if (enemy[i]->All(delta_time, this->playerX, this->playerY)) {
				delete enemy[i];
				enemy[i] = NULL;
			}

		}
	}

	// - プレイヤー死亡の間、敵は撤退する -
	if (!player->GetLife()) {
		for (int i = 0; i < enemySpawnCount; i++) {
			if (enemy[i] != NULL) {
				enemy[i]->SetOutTime(totalTime);
			}
		}
	}

	// プレイヤー操作
	player->All(delta_time);

	// - 当たり判定 - //
	CollisionAll();

	// - 音声・効果音再生 - //
	SoundAll();

	// - 時間管理 - 
	Time(delta_time);

	// - アイテム -
	for (int i = 0; i < ITEM_NUM; i++) {
		if (item[i]->GetFlag()) {
			item[i]->All(delta_time);
		}
	}

	// -- スコアボード -- 
	// - 描画領域を指定 
	SetDrawArea(0, 0, WIDTH - MARGINE, HEIGHT - MARGINE);
	// - スコア内容更新 -
	ScoreUpdate();
	// - スコア描画 -
	score->All();


	// ゲームオーバー画面実行関数(画面表示系：追々クラス化したい) - 
	if (gameOverFlag && gameOverWindowTime <= 0) {
		GameOverWindow(gameOverFlag);
		// エンター入力時、コンティニュー以外を選択の場合、シーン切り替えを実施
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			tempScene = GameOverWindow(gameOverFlag);

			// ユーザーがコンティニューを選んだ場合
			if (tempScene == SCENE_NON_CHANGE) {
				// プレイヤー復活
				player->SetRespawn();
				// スコア減点
				score->SetScore(CURRENT_SCORE, -2000);
				// ゲームオーバーフラグを折る
				gameOverFlag = false;
			}
		}
		else {
			tempScene = SCENE_NON_CHANGE;
		}
		
	}

	// リザルト画面実行関数 - 
	if (resultFlag && resultWindowTime <=0) {
		ResultWindow(resultFlag);
		// エンター入力時、シーン切り替えを実施
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			tempScene = ResultWindow(resultFlag);
		}
		else {
			tempScene = SCENE_NON_CHANGE;
		}
	}

	// - チュートリアル画面実行 - 
	if (tutorialFlag && !tutorialEndFlag) {
		tutorialEndFlag = tutorial.All();
	}

	// - ポーズ画面実行関数 -
	if(pauseFlag){
		PauseWindow(pauseFlag);
		// エンター入力時、コンティニュー以外を選択の場合、シーン切り替えを実施
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			tempScene = PauseWindow(pauseFlag);

			// ユーザーがコンティニューを選んだ場合
			if (tempScene == SCENE_NON_CHANGE) {
				// ポーズ画面終了 
				pauseFlag = false;
			}
		}
		else {
			tempScene = SCENE_NON_CHANGE;
		}
	}
	
	return tempScene;

}

void SceneManagerCh1::SoundAll() {
	// --- サウンドの管理 ---
	// -- 初期化処理 --
	if (!soundInit) {
		hb_stageSound = LoadSoundMem("sound/BGM/Ch1_ARPCHAIN.mp3");
		hb_bossSound = LoadSoundMem("sound/BGM/CH1_BossBGM.mp3");
		hs_playerRedShot01 = LoadSoundMem("sound/RedEffect/RedBullet001_MachinGun.mp3");
		hs_playerBlueShot01 = LoadSoundMem("sound/BlueEffect/BlueBullet001_Ray.mp3");
		hs_playerYellowShot01 = LoadSoundMem("sound/YellowEffect/YellowBullet001_Gus.mp3");
		hs_playerDead = LoadSoundMem("sound/Player/PlayerDeadSE.mp3");
		hs_playerTransform = LoadSoundMem("sound/Player/PlayerTransform.wav");
		hs_playerDamaged = LoadSoundMem("sound/Player/PlayerDamaged.mp3");
		hs_playerBombShot = LoadSoundMem("sound/Player/PlayerBombShot.mp3");
		hs_playerBombExplode = LoadSoundMem("sound/Player/PlayerBombExplode.mp3");
		hs_playerRefrect = LoadSoundMem("sound/Player/PlayerRefrect.mp3");
		hs_playerFlying = LoadSoundMem("sound/Player/PlayerFlying.mp3");
		hs_playerShiftedFlying = LoadSoundMem("sound/Player/PlayerShiftedFlying.mp3");
		hs_enemyDead = LoadSoundMem("sound/EnemyEffect/EnemyDead.mp3");
		hs_enemyDamaged = LoadSoundMem("sound/EnemyEffect/EnemyDamaged.mp3");
		hs_itemGot = LoadSoundMem("sound/Player/PlayerItemGot.mp3");
		hs_enemySho01 = LoadSoundMem("sound/EnemyEffect/EnemyShot001.wav");
		// メニュー画面
		hs_choiceSE = LoadSoundMem("sound/Title/TitleCursorSE.mp3");
		hs_decisionSE = LoadSoundMem("sound/Title/TitleDecisionSE.mp3");
		hs_enemyReflect = LoadSoundMem("sound/EnemyEffect/EnemyReflect.mp3");

		PlaySoundMem(hb_stageSound, DX_PLAYTYPE_LOOP);

		// - 初期化処理終了 -
		soundInit = true;
	}

	// -- 音楽変更 --
	if (bossExistFlag) {
		// ボス出現時、BGM変更
		if (!bossExistInit) {
			StopSoundMem(hb_stageSound);
			PlaySoundMem(hb_bossSound, DX_PLAYTYPE_LOOP);

			bossExistInit = true;
		}
	}

	// -- 効果音再生 --
	
	// - 自機ショット効果音管理 -
	int tempShotType = player->GetShotFlag();
	switch (tempShotType)
	{
	case RED:
		PlaySoundMem(hs_playerRedShot01, DX_PLAYTYPE_BACK);
		break;

	case BLUE:
		PlaySoundMem(hs_playerBlueShot01, DX_PLAYTYPE_BACK);
		break;

	case YELLOW:
		PlaySoundMem(hs_playerYellowShot01, DX_PLAYTYPE_BACK);
		break;

	default:
		// 何もしない
		break;
	}
	// - 自機破壊効果音　or ダメージ効果音 -
	if (player->GetDeadFlag()) {
		// 自機破壊音再生
		PlaySoundMem(hs_playerDead, DX_PLAYTYPE_BACK);
	}
	else if (player->GetDamageSoundFlag()) {
		// 自機ダメージ音(無敵時間外＆生きてる間)
		if (player->GetLife()) {
			PlaySoundMem(hs_playerDamaged, DX_PLAYTYPE_BACK);
			fs_playerDamaged = false;
		}
	}
	
	// - 自機変形音
	if (player->GetTranformFlag()) {
		PlaySoundMem(hs_playerTransform, DX_PLAYTYPE_BACK);
	}

	// - 自機ボム発射音 -
	if (player->GetBombShotSoundFlag()) {
		fs_playerBombShot = true;
	}
	if (fs_playerBombShot) {
		PlaySoundMem(hs_playerBombShot, DX_PLAYTYPE_BACK);
		fs_playerBombShot = false;
	}

	// - 自機ボム爆破音 -
	if (player->GetBombExplodeSoundFlag()) {
		fs_playerBombExplode = true;
	}
	if (fs_playerBombExplode) {
		PlaySoundMem(hs_playerBombExplode, DX_PLAYTYPE_BACK);
		fs_playerBombExplode = false;
	}

	// - 自機ノーダメージ音 -
	if (player->GetRefrectFlag()) {
		fs_playerRefrect = true;
	}
	if (fs_playerRefrect) {
		PlaySoundMem(hs_playerRefrect, DX_PLAYTYPE_BACK);
		fs_playerRefrect = false;
	}

	// - 自機飛行中音 -
	if (!player->GetDeadFlag()) {
		// プレイヤーが生きてれば再生
		if (!CheckSoundMem(hs_playerFlying)) {
			PlaySoundMem(hs_playerFlying, DX_PLAYTYPE_LOOP);
		}
		if (player->GetShiftedFlying()) {
			// 高速飛行中は効果音変更
			if (CheckSoundMem(hs_playerFlying)) {
				StopSoundMem(hs_playerFlying);
			}
			if (!CheckSoundMem(hs_playerShiftedFlying)) {
				PlaySoundMem(hs_playerShiftedFlying, DX_PLAYTYPE_LOOP);
			}
		}
		else {
			StopSoundMem(hs_playerShiftedFlying);
		}
	}

	// - 敵ダメージ音 - 
	if (fs_enemyDamaged) {
		PlaySoundMem(hs_enemyDamaged, DX_PLAYTYPE_BACK);
		fs_enemyDamaged = false;
	}

	// - 敵ノーダメージ音
	if (fs_enemyReflect) {
		PlaySoundMem(hs_enemyReflect, DX_PLAYTYPE_BACK);
		fs_enemyReflect = false;
	}

	// - 敵発砲音
	if (!gameOverFlag && !pauseFlag && !resultFlag) {
		for (int i = 0; i < enemySpawnCount; i++) {
			if (enemy[i] != NULL) {
				if (enemy[i]->GetShotFlag()) {
					fs_enemyShot01 = true;
					break;
				}
			}
		}
	}
	if (fs_enemyShot01) {
		PlaySoundMem(hs_enemySho01, DX_PLAYTYPE_BACK);
		fs_enemyShot01 = false;
	}

	// - 敵死亡音 -
	if (fs_enemyDead) {
		PlaySoundMem(hs_enemyDead, DX_PLAYTYPE_BACK);
		fs_enemyDead = false;
	}

	// - アイテム取得音 - 
	if (fs_itemGot) {
		PlaySoundMem(hs_itemGot, DX_PLAYTYPE_BACK);
		fs_itemGot = false;
	}

	// - メニュー画面(選択音)
	if (fs_choiceSE) {
		PlaySoundMem(hs_choiceSE, DX_PLAYTYPE_BACK);
		fs_choiceSE = false;
	}

	// メニュー画面(決定音)
	if (fs_decisionSE) {
		PlaySoundMem(hs_decisionSE, DX_PLAYTYPE_BACK);
		fs_decisionSE = false;
	}


}

void SceneManagerCh1::CollisionAll() {
	// --- 当たり判定を実装 ---
	
	// -- 一時変数 --
	double t_enemyX, t_enemyY;
	double t_eShotX, t_eShotY;
	double t_playerX, t_playerY;
	double t_pShotX, t_pShotY;
	double t_itemX, t_itemY;
	bool t_damageFlag = false;
	double t_bombX, t_bombY;

	// -- プレイヤーの弾との当たり判定 --
	for (int i = 0; i < MACHINE_NUM; i++) {
		for (int k = 0; k < PSHOT_NUM; k++) {
			// 発射中の弾を判別して判定実施
			if (player->GetShotPosition(i, k, &t_pShotX, &t_pShotY)) {
				// 敵クラスのポイントがNULLでない　かつ
				// deadFlag がFalse
				for (int n = 0; n < enemySpawnCount; n++) {
					if (enemy[n] != NULL && !enemy[n]->GetDeadFlag()) {
						// 敵の位置取得
						enemy[n]->GetPosition(&t_enemyX, &t_enemyY);
						// - 当たり判定 - 
						// プレイヤーの弾の種類に応じて当たり判定変化
						int tempCollision;
						tempCollision = player->GetBulletCollision();

						if (CircleCollision(tempCollision, enemy[n]->GetCollision(), t_pShotX, t_enemyX, t_pShotY, t_enemyY)) {
							// 当たっていれば敵のDamageFlagを立てる
							// ダメージ計算を即座に実施
							enemy[n]->SetDamage(player->GetShotElement(i, k), player->GetShotDamage(i, k));
							enemy[n]->DamageCalc();
							// 当てたプレイヤーの弾のフラグを戻す(貫通弾はのぞく)
							if (!player->GetPenetrationFlag()) {
								player->SetShotFlag(i, k, false);
							}


							// 効果音フラグ
							if (enemy[n]->GetDamageFlag()) {
								fs_enemyDamaged = true;
							}
							if (enemy[n]->GetRefrectFlag()) {
								fs_enemyReflect = true;
							}

						}

					}
				}
			}
		}
	}

	// -- プレイヤーのボムとの当たり判定 --
	if (player->GetBombExplodeFlag()) {
		// 爆弾が爆発した時のみ実行
		player->GetBombPosition(&t_bombX, &t_bombY);
		// 出現中＆生存中の敵にのみ当たり判定計算
		for (int i = 0; i < enemySpawnCount; i++) {
			if (enemy[i] != NULL && !enemy[i]->GetDeadFlag()) {
				enemy[i]->GetPosition(&t_enemyX, &t_enemyY);	// 敵の位置を取得
				// 当たり判定計算
				if (CircleCollision(player->GetBombCollision(), enemy[i]->GetCollision(), t_bombX, t_enemyX, t_bombY, t_bombY)) {
					// 当たっていれば敵のDamageFlagを立てる。ボムは無属性攻撃
					// ダメージ計算を即座に実施
					enemy[i]->SetDamage(NON_ELEMENT, player->GetBombDamage());
					enemy[i]->DamageCalc();
					
				}

			}
		}

	}


	// -- 敵の弾と操作キャラとの当たり判定
	// プレイヤーが生きている時だけ
	if (player->GetLife()) {
		// 無敵時間でない時だけ
		if(!player->GetDamageInvalidFlag()){
			if (!player->GetDamageFlag()) {
				// 既に被弾判定で無ければ以下実施
				player->GetPosition(&t_playerX, &t_playerY);
				for (int i = 0; i < enemySpawnCount; i++) {
					// 現在出現中の敵の数だけループ
					if (enemy[i] != NULL) {
						for (int k = 0; k < ENEMY_SNUM; k++) {
							// 弾フラグ true : フィールド上に存在する弾のみで当たり判定実施
							if (enemy[i]->GetShotPosition(k, &t_eShotX, &t_eShotY)) {
								// 当たり判定実施(敵の弾のCollisionサイズを参照)
								int tempCollision = enemy[i]->GetShotCollision(k);

								// グレイズ判定

								// ダメージ判定
								if (CircleCollision(player->GetCollision(), tempCollision, t_playerX, t_eShotX, t_playerY, t_eShotY)) {
									// 一時的なダメージフラグ on
									t_damageFlag = true;
								}


								// ダメージ判定：true時の処理：
								if (t_damageFlag) {
									// 被弾フラグセット、弾の属性、ダメージ量をセット
									player->SetDamage(enemy[i]->GetShotElement(k), enemy[i]->GetShotDamage(k));
									// 弾の消去
									enemy[i]->SetShotFlag(k, false);
									// 一時的なフラグを戻す
									t_damageFlag = false;
								}
							}
						}
					}
				}
			}
		}

	}
	
	// -- アイテムとプレイヤーの当たり判定 -- 
	for (int i = 0; i < ITEM_NUM; i++) {
		// - フラグが有効なアイテムのみ参照 - 
		if (item[i]->GetFlag()) {
			item[i]->GetPosition(&t_itemX, &t_itemY);
			// プレイヤーとアイテムの当たり判定
			player->GetPosition(&t_playerX, &t_playerY);
			if (CircleCollision(player->GetCollision(), ITEM_COLLISION, t_playerX, t_itemX, t_playerY, t_itemY)) {
				// アイテムの効果をプレイヤーに与える
				player->SetItem(item[i]->GetType());
				// アイテム消滅フラグを立てる
				item[i]->Delete();
				// アイテム取得音セット
				fs_itemGot = true;

			}
		}
	}

}


bool SceneManagerCh1::CircleCollision(double cir1, double cir2, double cirX1, double cirX2, double cirY1, double cirY2) {
	// --- 円形当たり判定 ---
	// cir1, cir2 : 各々の円の半径
	// cirNn : 各々の円の位置X,Y
	double length = cir1 + cir2;
	double xLenght = cirX1 - cirX2;
	double yLength = cirY1 - cirY2;

	if (length * length >= xLenght * xLenght + yLength * yLength) {
		return true;
	}
	else {
		return false;
	}
}

void SceneManagerCh1::GetPlayerPosition() {
	// --- プレイヤーの位置を取得 ---
	player->GetPosition(&this->playerX, &this->playerY);
}

void SceneManagerCh1::Time(float delta_time) {
	// --- 時間の管理 ---  
	totalTime += delta_time;	// 経過時間
	if (gameOverFlag) {
		// - ゲームオーバーウィンドウ表示ディレイカウンタ -
		gameOverWindowTime -= delta_time;
		if (gameOverWindowTime <= 0) {
			gameOverWindowTime = 0;
		} 
	}
	else {
		gameOverWindowTime = 0;
	}

	if (resultFlag) {
		// - リザルト画面表示ディレイカウンタ -
		resultWindowTime -= delta_time;
		if (resultWindowTime <= 0) {
			resultWindowTime = 0;
		}
	}
	else {
		resultWindowTime = 0;
	}
}

void SceneManagerCh1::ScoreUpdate() {
	// --- スコアボードの情報更新：プレイヤー ---
	score->SetScore(LIFE, player->GetLifeNum());
	score->SetScore(BOMB, player->GetBombNum());
	score->SetScore(HP, player->GetHpNum());
	score->SetScore(POWER, player->GetPowerNum());
	score->SetScore(SPEED, player->GetSpeedNum());
	score->SetScore(DEFENSE, player->GetDefenseNum());
	score->SetScore(ELEMENT, player->GetElement());
}

int SceneManagerCh1::PauseWindow(bool pauseFlag) {
	// --- ボーズ画面表示＆画面遷移フラグ(sceneDefine.h参照)を返す ---
	if (pauseFlag) {
		// -- ポーズ画面描画 ---
		if (!pauseDrawInit) {
			// - 描画初期処理 -
			pauseBackGh = LoadGraph("graphics/Pause/PauseShade_1_1024_768.png");
			pauseTitleLogoGh = LoadGraph("graphics/Pause/PauseLogo.png");
			LoadDivGraph("graphics/Pause/PauseSelection_8_225_488.png", 8, 1, 8, 225, 488 / 8, pauseSelectLogoGh);
			guideGh = LoadGraph("graphics/Pause/OptionGuide.png");
			pauseHelpLogo = LoadGraph("graphics/Pause/Help.png");
			controlGuideGh = LoadGraph("graphics/Pause/Controls.png");

			pauseDrawInit = true;
		}
		// - 背景表示(画面を暗くする) -
		DrawGraph(0, 0, pauseBackGh, true);
		DrawRotaGraph(100, 90, 0.7, 0, pauseTitleLogoGh, true, false);
		// - 操作ガイドの表示 -
		DrawRotaGraph(130, 200, 0.7, 0, guideGh, true, false);
		// - 選択肢の表示 -
		for (int i = 0; i < 4; i++) {
			DrawRotaGraph(WIDTH / 2, 120 + i * 75, 0.7, 0, pauseSelectLogoGh[i], true, false);
		}
		// - ヘルプガイドの表示
		DrawRotaGraph(150, 275, 0.7, 0, pauseHelpLogo, true, false);
		// - H入力時、 操作方法説明画像表示
		if (pauseHelpFlag) {
			// 操作方法ガイドの表示
			DrawRotaGraph(WIDTH/2, HEIGHT/2, 1, 0, controlGuideGh, true, false);
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
				pauseHelpFlag = false;
			}
		}
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_H)) {
			pauseHelpFlag = true;
		} 
		


		// -- ロゴセレクト --
		if (!pauseHelpFlag) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
				pauseSelectLogo++;
				fs_choiceSE = true;
				if (pauseSelectLogo > P_TITLE_SELECT) {
					pauseSelectLogo = P_CONTINUE_SELECT;
				}
			}
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
				pauseSelectLogo--;
				fs_choiceSE = true;
				if (pauseSelectLogo < P_CONTINUE_SELECT) {
					pauseSelectLogo = P_TITLE_SELECT;
				}
			}
		}

		// - 選択中のロゴ表示
		DrawRotaGraph(WIDTH / 2, 120 + 75 * (pauseSelectLogo), 0.7, 0,  pauseSelectLogoGh[pauseSelectLogo + 4], true, false);



		// -- エンター入力で決定SEを流す --
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			fs_decisionSE = true;
		}

		// -- ロゴセレクトに応じて選択肢分岐(sceneDefine.hと対応付け) -- 
		if (!pauseHelpFlag) {
			switch (pauseSelectLogo)
			{
			case P_CONTINUE_SELECT:
				return SCENE_NON_CHANGE;

				break;

			case P_OPTION_SELECT:
				return SCENE_OPTION;

				break;

			case P_CHAPTER_SELECT:
				return SCENE_CH_SELECT;

			case P_TITLE_SELECT:
				return SCENE_TITLE;

			default:
				return SCENE_NON_CHANGE;

				break;
			}
		}
		else {
			return SCENE_NON_CHANGE;
		}

	}
	else {
		return SCENE_NON_CHANGE;
	}

}

int SceneManagerCh1::GameOverWindow(bool gameOverFlag) {
	// --- ゲームオーバー画面表示＆画面遷移フラグ(sceneDefine.h参照)を返す ---
	if (gameOverFlag) {
		// -- ゲームオーバー画面描画 --
		if (!gameOverDrawInit) {
			// - 描画初期処理 - 
			gameOverBackGh = LoadGraph("graphics/Pause/PauseShade_1_1024_768.png");		// ポーズ画面背景流用
			gameOverTitle = LoadGraph("graphics/GameOver/Title.png");					// ゲームオーバー画面タイトル
			LoadDivGraph("graphics/GameOver/SelectLogo.png", 6, 1, 6, 378, 342 / 6, gameOverLogoGh);

			gameOverDrawInit = true;
		}
		// - 背景表示 (画面を暗くする) -
		DrawGraph(0, 0, gameOverBackGh, true);
		DrawRotaGraph((WIDTH - MENU_WIDTH) / 2, 150, 1.0, 0, gameOverTitle, true, false);
		DrawGraph((WIDTH - MENU_WIDTH) / 2 - 140, 250, gameOverLogoGh[0], true);
		DrawGraph((WIDTH - MENU_WIDTH) / 2 - 140, 300, gameOverLogoGh[1], true);
		DrawGraph((WIDTH - MENU_WIDTH) / 2 - 140, 350, gameOverLogoGh[2], true);

		// - 選択中のロゴ表示
		DrawGraph((WIDTH - MENU_WIDTH) / 2 - 140, 250 + 50 * (gameOverSelectLogo), gameOverLogoGh[gameOverSelectLogo + 3], true);

		// -- ロゴセレクト --
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
			gameOverSelectLogo++;
			fs_choiceSE = true;
			if (gameOverSelectLogo > TITLE_SELECT) {
				gameOverSelectLogo = CONTINUE_SELECT;
			}
			
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
			gameOverSelectLogo--;
			fs_choiceSE = true;
			if (gameOverSelectLogo < CONTINUE_SELECT) {
				gameOverSelectLogo = TITLE_SELECT;
			}
		}

		// -- エンター入力で決定SEを流す --
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			fs_decisionSE = true;
		}

		// -- ロゴセレクトに応じて選択肢分岐(sceneDefine.hと対応付け) -- 
		switch (gameOverSelectLogo)
		{
		case CONTINUE_SELECT:
			return SCENE_NON_CHANGE;

			break;

		case RETRY:
			return SCENE_CH_1;
		
			break;

		case TITLE_SELECT:
			return SCENE_TITLE;

		default:
			return SCENE_NON_CHANGE;

			break;
		}

	
	}
	else {
		return SCENE_NON_CHANGE;
	}

}

int SceneManagerCh1::ResultWindow(bool resultFlag) {
	// --- リザルト画面を表示する ---
	if (resultFlag) {
		if (!resultInit) {
			// -- 描画初期処理 --
			resultBackGh = LoadGraph("graphics/Pause/PauseShade_1_1024_768.png");		// ポーズ画面背景流用
			resultTitleGh = LoadGraph("graphics/Result/ResultTitle.png");		
			resultLogo1Gh = LoadGraph("graphics/Result/ResultLogo01.png");
			resultLogo2Gh = LoadGraph("graphics/Result/ResultLogo02.png");
			// 数字の画像取得
			LoadDivGraph("graphics/Score/ScoreNumber_10_300_38.png", 10, 10, 1, 300 / 10, 38, numGh);
			// ランクの画像取得
			LoadDivGraph("graphics/Result/ResultRank_5_475_95.png", 5, 5, 1, 475 / 5, 95, rankGh);

			resultInit = true;
		}
		// - 背景表示(画面を暗くする) -
		DrawGraph(0, 0, resultBackGh, true);
		DrawRotaGraph((WIDTH - MENU_WIDTH) / 2, 150, 1.0, 0, resultTitleGh, true, false);
		DrawGraph((WIDTH - MENU_WIDTH) / 2 -240, 250, resultLogo1Gh, true);
		DrawGraph((WIDTH - MENU_WIDTH) / 2 - 300, 450, resultLogo2Gh, true);
		// スコアの表示
		int tempScore = score->GetScore(CURRENT_SCORE);
		char buf[20];
		int num;
		num = sprintf(buf, "%d", tempScore);
		for (int i = 0; i < num; i++) {
			DrawRotaGraph((WIDTH - MENU_WIDTH) / 2 + 100 + i * 25, 287, 1.5, 0, numGh[(buf[i] - '0')], true, false);
		}
		// ランクの表示
		int tempRank;
		// - チャプター番号に応じて変化 - //
		// Ch. 0
		if (chapterNum == 0) {
			if (tempScore >= 5000) {
				tempRank = 0;
			}
			else if (5000 > tempScore && tempScore >= 4500) {
				tempRank = 1;
			}
			else if (4500 > tempScore && tempScore >= 3500) {
				tempRank = 2;
			}
			else if (3500 > tempScore && tempScore >= 2500) {
				tempRank = 3;
			}
			else {
				tempRank = 4;
			}

		}
		// Ch. 1
		else if (chapterNum == 1) {

			if (tempScore >= 20000) {
				tempRank = 0;
			}
			else if (20000 > tempScore && tempScore >= 18000) {
				tempRank = 1;
			}
			else if (18000 > tempScore && tempScore >= 15000) {
				tempRank = 2;
			}
			else if (15000 > tempScore && tempScore >= 10000) {
				tempRank = 3;
			}
			else {
				tempRank = 4;
			}
		}
		else {
			if (tempScore >= 20000) {
				tempRank = 0;
			}
			else if (20000 > tempScore && tempScore >= 18000) {
				tempRank = 1;
			}
			else if (18000 > tempScore && tempScore >= 15000) {
				tempRank = 2;
			}
			else if (15000 > tempScore && tempScore >= 10000) {
				tempRank = 3;
			}
			else {
				tempRank = 4;
			}
		}

		DrawRotaGraph((WIDTH - MENU_WIDTH) / 2 + 100, 378, 1.0, 0, rankGh[tempRank], true, false);

		return SCENE_CH_SELECT;

	}
	else {
		return SCENE_NON_CHANGE;
	}
}

bool SceneManagerCh1::BossExistFlag() {
	// --- ボスが画面上に存在するかどうかを判定 ---
	for (int i = 0; i < enemySpawnCount; i++) {
		if (enemy[i] != NULL) {
			bool temp = enemy[i]->GetBossExist();
			if (temp) {
				return true;
			}
		}
	}
	return false;
}

bool SceneManagerCh1::BossDefeatFlag() {
	// --- ボスを撃破した時、trueを返す ---
	for (int i = 0; i < enemySpawnCount; i++) {
		if (enemy[i] != NULL) {
			bool temp = enemy[i]->GetBossDead();
			if (temp) {
				return true;
			}
		}
	}
	return false;
}

void SceneManagerCh1::SetVolume(int bgmVolume, int seVolume) {
	// --- BGM&SEの音量をセット ---
	this->bgmVolume = bgmVolume;
	this->seVolume = seVolume;
	// -- 各ハンドラに音量を適応させる --
	int tempBGMVol, tempSEVol;
	tempBGMVol = (int)255 * (float)bgmVolume / 100.0;
	tempSEVol = (int)255 * (float)seVolume / 100.0;
	// - BGM -
	ChangeVolumeSoundMem(tempBGMVol, hb_stageSound);
	ChangeVolumeSoundMem(tempBGMVol, hb_bossSound);
	// - SE -
	ChangeVolumeSoundMem(tempSEVol, hs_playerRedShot01);
	ChangeVolumeSoundMem(tempSEVol, hs_playerBlueShot01);
	ChangeVolumeSoundMem(tempSEVol, hs_playerBlueShot01);
	ChangeVolumeSoundMem(tempSEVol, hs_playerYellowShot01);
	ChangeVolumeSoundMem(tempSEVol, hs_playerDead);
	ChangeVolumeSoundMem(tempSEVol, hs_playerTransform);
	ChangeVolumeSoundMem(tempSEVol, hs_playerDamaged);
	ChangeVolumeSoundMem(tempSEVol, hs_playerBombShot);
	ChangeVolumeSoundMem(tempSEVol, hs_playerBombExplode);
	ChangeVolumeSoundMem(tempSEVol, hs_playerRefrect);
	ChangeVolumeSoundMem(tempSEVol, hs_playerFlying);
	ChangeVolumeSoundMem(tempSEVol, hs_playerShiftedFlying);
	ChangeVolumeSoundMem(tempSEVol, hs_enemyDead);
	ChangeVolumeSoundMem(tempSEVol, hs_enemyDamaged);
	ChangeVolumeSoundMem(tempSEVol, hs_itemGot);
	ChangeVolumeSoundMem(tempSEVol, hs_enemySho01);
	ChangeVolumeSoundMem(tempSEVol, hs_enemyReflect);
	// メニュー画面
	ChangeVolumeSoundMem(tempSEVol, hs_choiceSE);
	ChangeVolumeSoundMem(tempSEVol, hs_decisionSE);

}