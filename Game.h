#pragma once

//ゲーム全体のヘッダファイル
//ヘッダファイルの読み込み
#include "DxLib.h"//必ず必要

////マクロ定義]
//ゲームタイトル
#define GAME_TITLE "ゲームタイトル"	
//ゲームの横幅
#define GAME_WIDTH 1280
//ゲームの縦幅
#define GAME_HEIGHT 720				
//ゲームの解像度
#define GAME_COLOR 32				
//ゲームの背景色
#define BACK_GROUND_COLOR 200,200,200	
//ゲームのICONのID
#define GAME_ICON_ID 333	
//ウィンドウバーの種類(0 : 縦横の長さを変更不可)(1 : バーがない)
#define GAME_WINDOW_BAR 0
//デバックモード
#define GAME_DEBUG true

//列挙型
//ゲームのシーン
enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};