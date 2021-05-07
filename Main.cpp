//ヘッダファイルの読み込み
#include "DxLib.h";//必ず必要

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

enum NowColorState
{
	black,
	red,
	green,
	blue,
	magenta,
	cyan,
	yellow,
	white
};
NowColorState ChangeColorState(NowColorState nowColor);


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

	//四角の位置
	int xPos = 0;
	int yPos = 0;

	//円の位置(中心位置)
	int xcPos = 0;
	int ycPos = 0;
	int xcHalf = GAME_WIDTH / 2;
	int ycHalf = GAME_HEIGHT / 2;

	//動かす量
	int speedAbs = 8;
	int xSpeed = speedAbs;
	int ySpeed = speedAbs;
	int xcSpeed = speedAbs;
	int ycSpeed = speedAbs;

	//四角の大きさ
	//幅
	int width = 32;
	//高さ
	int height = 32;

	//円の大きさ
	int radius = 100;

	//四角の色
	NowColorState nowColorState = red;
	int maxColor = 255;
	int minColor = 0;
	//色の構造体
	struct Color
	{
		int red;
		int green;
		int blue;
	};
	//色の構造体を設定
	Color nowColor;
	Color blackColor;
	Color redColor;
	Color greenColor;
	Color blueColor;
	Color magentaColor;
	Color cyanColor;
	Color yellowColor;
	Color whiteColor;
	//色を初期化
	//black
	blackColor.red = minColor;
	blackColor.green = minColor; 
	blackColor.blue = minColor;
	//red
	redColor.red = maxColor;
	redColor.green = minColor;
	redColor.blue = minColor;
	//green
	greenColor.red = minColor;
	greenColor.green = maxColor;
	greenColor.blue = minColor;
	//blue
	blueColor.red = minColor;
	blueColor.green = minColor;
	blueColor.blue = maxColor;
	


	while (1)
	{
		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }
		

		//キーが押されたとき
		if (CheckHitKeyAll() != 0)
		{
			//無限ループを抜ける
			break;
		}

		//メッセージを受け取り続ける、×などでウィンドウが閉じたとき
		if (ProcessMessage() != 0) 
		{
			//無限ループを抜ける
			break;
		}

		/*
		//色を設定する
		switch (nowColorState)
		{
		case black:
			nowColor = blackColor;
			break;

		case red:
			nowColor = redColor;
			break;

		case green:
			nowColor = greenColor;
			break;

		case blue:
			nowColor = blueColor;
			break;

		case magenta:
			nowColor = magentaColor;
			break;

		case cyan:
			nowColor = cyanColor;
			break;

		case yellow:
			nowColor = yellowColor;
			break;

		case white:
			nowColor = whiteColor;
			break;
		}
		*/

		//四角を描画
		//DrawBox(xPos, yPos, xPos+width, yPos+height, GetColor(nowColor.red, nowColor.green, nowColor.blue), TRUE);
		DrawCircle(xcPos, ycPos, radius, GetColor(0, 0, 0), FALSE,5);
		//座標移動計算
		xPos += xSpeed;
		yPos += ySpeed;
		xcPos += xcSpeed;
		ycPos += ycSpeed;

		//四角
		//x- なら反対方向に移動させる
		if (xPos > GAME_WIDTH - width)
		{
			xSpeed = -speedAbs;
			//色の状態を変更
			nowColorState = ChangeColorState(nowColorState);
		}
		else if (xPos < 0)
		{
			xSpeed = speedAbs;
			//色の状態を変更
			nowColorState = ChangeColorState(nowColorState);
		}

		if (yPos > GAME_HEIGHT - height)
		{
			ySpeed = -speedAbs;
			//色の状態を変更
			nowColorState = ChangeColorState(nowColorState);
		}
		else if (yPos < 0)
		{
			ySpeed = speedAbs;
			//色の状態を変更
			nowColorState = ChangeColorState(nowColorState);
		}

		//円
		if (xcPos + radius > GAME_WIDTH)
		{
			xcSpeed = -speedAbs;
		}
		else if (xcPos - radius < 0)
		{
			xcSpeed = speedAbs;
		}

		
		if (ycPos + radius > GAME_HEIGHT)
		{
			ycSpeed = -speedAbs;
		}
		else if (ycPos - radius < 0)
		{
			ycSpeed = speedAbs;
		}
		
		//ダブルバッファリングした画像を描画
		ScreenFlip();
	}

	// ＤＸライブラリ使用の終了処理準備(return 0でソフトが終了する)
	DxLib_End();				

	// ソフトの終了
	return 0;				 
}


//色の状態を変更
NowColorState ChangeColorState(NowColorState nowColor) 
{
	int nowColorNum = (int)nowColor;

	nowColorNum++;

	if (nowColorNum > white)
	{
		nowColorNum = (int)black;
	}

	nowColor = (NowColorState)nowColorNum;

	return nowColor;
}