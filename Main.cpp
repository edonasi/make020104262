//ヘッダファイルの読み込み
#include "DxLib.h";//必ず必要
#include "Keyboard.h"

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

//列挙型
//ゲームのシーン
enum GAME_SCENE 
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};

//シーンを管理する変数
//現在のゲームのシーン
GAME_SCENE GameScene;
//前回のゲームのシーン
GAME_SCENE GameSceneOld;
//次のゲームのシーン
GAME_SCENE GameSceneNext;

//画面の切り替え
//フェードアウトしているか
bool isFadeOut = false;
//フェードインしているか
bool isFadeIn = false;

//※Alt+Shift+左ドラッグ=矩形選択
// //プロトタイプ宣言
//タイトル画面
void Title();
//タイトル画面 処理
void TitleProc();
//タイトル画面 描画
void TitleDraw();
//プレイ画面
void Play();
//プレイ画面 処理
void PlayProc();
//プレイ画面 描画
void PlayDraw();
//エンド画面
void End();
//エンド画面 処理
void EndProc();
//エンド画面 描画
void EndDraw();
//切り替え画面
void Change();
//切り替え画面 処理
void ChangeProc();
//切り替え画面 描画
void ChangeDraw();


//押している間加速
int AddSpeed(int speed);

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Log.txtを出力しない(GitHubで管理するため、現在必要ない)
	SetOutApplicationLogValidFlag(FALSE);	
	//ウィンドウモードに設定
	ChangeWindowMode(TRUE);
	//ゲームタイトルの設定
	SetMainWindowText(GAME_TITLE);
	//ゲームウィンドウ解像度設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	//ウィンドウのサイズを設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);
	//背景色の設定
	SetBackgroundColor(BACK_GROUND_COLOR);
	//ゲームのICONを設定
	SetWindowIconID(GAME_ICON_ID);
	//ウィンドウバーの状態
	SetWindowStyleMode(GAME_WINDOW_BAR);
	//ウィンドウの垂直同期を有効にする(ディスプレイの能力を制限して、フレームレート(fps)を一定に保つ)=FALSEはフレームレートを管理しなければならない
	SetWaitVSyncFlag(TRUE);
	//非アクティブでも実行する
	SetAlwaysRunFlag(TRUE);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	
	//円の中心点
	int x = GAME_WIDTH / 2;
	int y = GAME_HEIGHT / 2;
	//円の半径
	int radius = 50;
	//移動の速さ
	int speed = 1;

	//最初のシーンはタイトル画面から
	GameScene = GAME_SCENE_TITLE;

	while (1)
	{		
		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }
		//キー入力状態を更新する
		AllKeyUpdate();
		
		//メッセージを受け取り続ける、×などでウィンドウが閉じたとき
		if (ProcessMessage() != 0) 
		{
			//無限ループを抜ける
			break;
		}

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE)) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE) 
		{
			GameSceneOld = GameScene;
		}

		//※switch tab→enum候補 tab→switch_onの中にenumの変数が入る enter
		switch (GameScene)
		{
			//タイトル画面
		case GAME_SCENE_TITLE:
			Title();
			break;

			//プレイ画面
		case GAME_SCENE_PLAY:
			Play();
			break;

			//エンド画面
		case GAME_SCENE_END:
			End();
			break;

			//切り替え画面
		case GAME_SCENE_CHANGE:
			Change();
			break;
		default:
			break;
		}

		//速度をリセット
		if (KeyUpping(KEY_INPUT_UP) || KeyUpping(KEY_INPUT_DOWN)
			|| KeyUpping(KEY_INPUT_RIGHT) || KeyUpping(KEY_INPUT_LEFT))
		{
			if (speed > 0)
			{
				speed--;
			}
			else 
			{
				speed = 0;
			}
			
		}

		//上下の座標を計算
		if (KeyDown(KEY_INPUT_UP)) 
		{
			y -= speed;
			speed = AddSpeed(speed);
		}
		if (KeyDown(KEY_INPUT_DOWN)) 
		{
			y += speed;
			speed = AddSpeed(speed);
		}

		//左右の座標を計算
		if (KeyDown(KEY_INPUT_RIGHT)) 
		{
			x += speed;
			speed = AddSpeed(speed);
		}
		if (KeyDown(KEY_INPUT_LEFT)) 
		{
			x -= speed;
			speed = AddSpeed(speed);
		}

		DrawFormatString(0, 0, GetColor(0, 0, 0), "%d", speed);

		//円の描画
		DrawCircle(x, y, radius, GetColor(0, 0, 0), TRUE);

		//ダブルバッファリングした画像を描画
		ScreenFlip();
	}

	// ＤＸライブラリ使用の終了処理準備(return 0でソフトが終了する)
	DxLib_End();				

	// ソフトの終了
	return 0;				 
}

int AddSpeed(int speed) 
{
	//最大速度
	int max = 100;

	if (speed < max)
	{
		return speed + 1;
	}
	else 
	{
		return max;
	}
}

/// <summary>
/// タイトル画面
/// </summary>
void Title() 
{
	//処理
	TitleProc();
	//描画
	TitleDraw();
}

//処理
void TitleProc() 
{
	return;
}

//描画
void TitleDraw() 
{
	const char* string = "Title";
	DrawString(0, 50, string, GetColor(0, 0, 0));
	//※レイヤー概念は下に書いた処理がゲーム画面では上のレイヤーに表示される(photoshopやillustratorと逆)
	return;
}

//プレイ画面
void Play() 
{
	//処理
	PlayProc();
	//描画
	PlayDraw();
}

//処理
void PlayProc() 
{
	return;
}

//描画
void PlayDraw() 
{
	return;
}

//エンド画面
void End() 
{
	//処理
	EndProc();
	//描画
	EndDraw();
}

//処理
void EndProc()
{
	return;
}

//描画
void EndDraw()
{
	return;
}

//切り替え画面
void Change() 
{
	//処理
	ChangeProc();
	//描画
	ChangeDraw();
}

//処理
void ChangeProc()
{
	return;
}

//描画
void ChangeDraw()
{
	return;
}
