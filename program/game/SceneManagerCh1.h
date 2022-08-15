#pragma once

// -- 自作クラス用意 --
#include "Player.h"
#include "Back.h"
#include "Enemy.h"
#include "DeadEffect.h"
#include "Score.h"
#include "Item.h"
#include "define.h"
#include "Tutorial.h"

class SceneManagerCh1
{
private:

	// -- 変数 ---
	// - 選択したチャプター番号
	int chapterNum;
	// - 時間 - 
	float totalTime;			// 経過時間
	// - 座標情報 -
	double playerX, playerY;	// プレイヤーの位置
	// - 敵の行動パターン係数格納
	int moveK[ENEMY_MOVE_K];
	// - 敵をタイムテーブルの上から順にスポーン. そのカウント
	int enemySpawnCount;
	bool spawnEndFlag;			// 敵スポーン完了フラグ
	// - All初期処理フラグ -
	bool allInit;				// All()関数実行時、初回のみの処理実行のためのフラグ
	// -- BGMハンドラ --
	int hb_stageSound;			// ステージBGM
	int hb_bossSound;			// ボス戦闘BGM
	// -- ポーズ画面 --
	bool pauseDrawInit;			// ポーズ画面描画初期処理フラグ
	bool pauseFlag;				// ボーズ画面実行フラグ
	float pauseWaitTime;		// キー入力チャタリング防止用のカウンタ
	int pauseBackGh;			// ポーズ画面背景
	int pauseTitleLogoGh;		// ポーズ画面ロゴグラフィックハンドラ
	int pauseSelectLogoGh[8];	// ポーズ画面選択肢ロゴグラフィックハンドラ
	int guideGh;				// 操作ガイドグラフィックハンドラ
	int pauseSelectLogo;		// ポーズ画面選択肢
	int pauseHelpLogo;			// ポーズヘルプロゴグラフィックハンドラ
	bool pauseHelpFlag;		// ポーズ中、操作説明画像を開いているかのフラグ
	int controlGuideGh;		// 操作方法画像のグラフィックハンドラ格納

	// -- ゲームオーバー画面 -- 
	bool gameOverFlag;			// ゲームオーバーフラグ
	float gameOverWindowTime;	// ゲームオーバーになってから、ゲームオーバー画面を出すまでの時間
	bool gameOverDrawInit;		// ゲームオーバー画面描画初期処理フラグ
	int gameOverBackGh;			// ゲームオーバー画面背景
	int gameOverTitle;			// ゲームオーバー画面タイトルロゴグラフィックハンドラ
	int gameOverLogoGh[6];		// ゲームオーバー画面ロゴグラフィックハンドラ
	int gameOverSelectLogo;		// ゲームオーバー画面選択肢
	// -- リザルト画面 --
	bool resultFlag;			// リザルト画面表示フラグ
	bool resultInit;			// リザルト画面初期処理フラグ
	int resultBackGh;			// リザルト画面背景
	int resultTitleGh;			// リザルト画面タイトルロゴグラフィックハンドラ
	int resultLogo1Gh;			// リザルト画面ロゴグラフィックハンドラ１
	int resultLogo2Gh;			// リザルト画面ロゴグラフィックハンドラ２
	int numGh[10];				// 数字の画像ハンドラ
	int rankGh[5];				// ランク(S-D)の画像ハンドラ
	float resultWindowTime;		// リザルト画面表示ディレイカウンタ
	int hs_winBGM;				// 勝利BGM

	// -- 効果音ハンドル等 --　
	bool soundInit;
	int hs_playerRedShot01;		// 自機標準弾発射音ハンドラ
	int hs_playerBlueShot01;		// 自機標準弾発射音ハンドラ
	int hs_playerYellowShot01;		// 自機標準弾発射音ハンドラ
	int hs_playerDead;			// プレイヤー破壊音
	int hs_playerTransform;	// プレイヤー変形音ハンドラ
	bool fs_playerDamaged;		// プレイヤーダメージフラグ
	int hs_playerDamaged;		// プレイヤーダメージ音ハンドラ
	bool fs_playerBombShot;		// プレイヤーボムショット音フラグ
	int hs_playerBombShot;		// プレイヤーボムショット音ハンドラ
	bool fs_playerBombExplode;	// プレイヤーボム爆発音フラグ
	int hs_playerBombExplode;	// プレイヤーボム爆発音ハンドラ
	bool fs_playerRefrect;		// プレイヤーノーダメージ音フラグ
	int hs_playerRefrect;		// プレイヤーノーダメージ音ハンドラ
	float fs_shiftedFlying;		// シフトで高速移動中かを判定
	int hs_playerFlying;		// プレイヤー飛行音ハンドラ
	int hs_playerShiftedFlying;	// プレイヤー高速飛行音ハンドラ(シフトキー入力)
	bool fs_enemyDead;			// 敵消滅フラグ
	int hs_enemyDead;			// 敵消滅音ハンドラ
	bool fs_enemyDamaged;		// 敵ダメージフラグ
	int hs_enemyDamaged;		// 敵ダメージ音ハンドラ
	bool fs_itemGot;			// アイテム取得フラグ
	int hs_itemGot;				// アイテム取得音ハンドラ
	bool fs_enemyShot01;		// 敵発砲プラグ
	int hs_enemySho01;			// 敵発砲音ハンドラ
	bool fs_enemyReflect;		// 敵機ノーダメージ音フラグ
	int hs_enemyReflect;		// 敵機ノーダメージ音ハンドラ
	// メニュー画面
	bool fs_choiceSE;			// 選択時効果音フラグ
	int hs_choiceSE;			// 選択肢効果音ハンドラ
	bool fs_decisionSE;			// 選択肢決定音フラグ
	int hs_decisionSE;			// 選択肢決定音ハンドラ
	// -- BGM, SE音量 --
	int bgmVolume;				// BGM音量(0-100%)
	int seVolume;				// BGM音量(0-100%)
	// -- 画面遷移先格納 --
	int tempScene;				// SceneDefine.h参照
	// -- ボスフラグ関係 --
	bool bossExistFlag;			// ボス出現フラグ
	bool bossExistInit;			// ボス出現初期処理フラグ
	int deadBossCounter;		// ボス撃破数カウンタ
	int bossNumMax;				// ボス総数を設定
	// -- チュートリアル --
	bool tutorialFlag;			// チュートリアル実施フラグ
	bool tutorialEndFlag;		// チュートリアル終了フラグ

	// - 敵のタイムテーブル用意
	EnemyData edata[ENEMY_NUM];

	// -- クラス ---
	// プレイヤークラス
	Player* player;
	// 背景描画クラス
	Back* back;
	// 敵クラス
	Enemy* enemy[ENEMY_NUM];
	// スコアボードクラス
	Score* score;
	// アイテムクラス
	Item* item[ITEM_NUM];
	// チュートリアルクラス
	Tutorial tutorial;

private:
	void SoundAll();
	void CollisionAll();
	bool CircleCollision(double cir1, double cir2, double cirX1, double cirX2, double cirY1, double cirY2);
	void GetPlayerPosition();
	void Time(float delta_time);
	void ScoreUpdate();
	int PauseWindow(bool pauseFlag);
	int GameOverWindow(bool gameOverFlag);
	int ResultWindow(bool resultFlag);
	bool BossExistFlag();
	bool BossDefeatFlag();

public:
	SceneManagerCh1(int chapterNum, char backName[NAME_SIZE], char effect1[NAME_SIZE], char effect2[NAME_SIZE], char enemyData[NAME_SIZE]);
	~SceneManagerCh1();
	int  All(float delta_time);
	void SetVolume(int bgmVolume, int seVolume);
};