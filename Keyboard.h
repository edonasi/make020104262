#pragma once

//取得するキーの種類
#define KEY_KIND_MAX 256

//キーボード構造体
struct KEYBOARD
{
	//入力状態を管理
	char tmpKeyState[KEY_KIND_MAX];
	//すべてのキーの入力時間を管理
	int allKeyState[KEY_KIND_MAX];
	//以前のすべてのキーの入力時間を管理
	int oldKeyState[KEY_KIND_MAX];
};

//外部のグローバル変数(宣言しているのではない)
extern KEYBOARD keyboard;

//----- プロトタイプ宣言 ------
//すべてのキーの入力状態を取得する
void AllKeyUpdate(void);
//特定のキーを押しているか
bool KeyDown(int keyInput);
//特定のキーを押した瞬間
bool Key1Down(int keyInput);
//特定のキーを上げているか
bool KeyUp(int keyInput);
bool KeyUpping(int keyInput);
//特定のキーをクリックしたか
bool KeyClick(int keyInput);
//特定のキーを指定のミリ秒分押し続けているか
bool KeyDownKeep(int keyInput, int millSec);