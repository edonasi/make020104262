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

		//スピードを速くする
		/*if (Key1Down(KEY_INPUT_UP) || Key1Down(KEY_INPUT_DOWN)
			|| Key1Down(KEY_INPUT_RIGHT) || Key1Down(KEY_INPUT_LEFT))
		{
			speed++;
		}*/

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