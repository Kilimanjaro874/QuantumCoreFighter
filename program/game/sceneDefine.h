#pragma once

extern int scene;				// シーン切り替えフラグ(from game_main.c)
static bool screenFixedFlag = true;	// 固定ウィンドウモード

#define SCENE_NON_CHANGE 0		// シーン切り替えなし
#define END_GAME		-1		// ゲーム終了
#define SCENE_OPENING	100		// オープニング
#define SCENE_TITLE		200		// タイトル画面
#define SCENE_OPTION	201		// オプション画面
#define SCENE_CH_SELECT	300		// チャプターセレクト
#define SCENE_CH_1		400		// チャプター1

// -- プロトタイプ宣言 --
void sceneTitle(float delta_time);								// タイトル関数
void endSceneTitle();							// タイトル終了関数
void sceneOpening(float delta_time);			// オープニング関数
void endSceneOp();								// オープニング終了関数
void sceneChSelect();					// チャプターセレクト関数
void endSceneChSelect();				// チャプターセレクトエンド関数
void scenePlayCh1(float delta_time);	// Ch1 ゲームプレイ関数
void endScenePlayCh1();					// Ch1　ゲームプレイ終了関数

// -- ゲーム終了時 実行する関数 -- 
void endGame();					