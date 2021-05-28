#include "Game.h"		//ゲーム全体のヘッダファイル
#include "Fps.h"		//FPS処理のヘッダファイル
#include "Keyboard.h"	//キーボードの処理のヘッダファイル

//キャラクタの構造体
struct CHARACTER
{
	int handle = -1;	//画像のハンドル(管理番号)
	char path[255];		//画像の場所(パス)
	int x;				//xの位置
	int y;				//yの位置
	int width;			//画像の幅
	int height;			//画像の高さ
	int speed = 1;		//移動速度

	RECT coll;			//矩形当たり判定領域
	bool isDraw = false;//画像が描画できるか
};

struct MOVIE
{
	int handle = -1;	//動画ハンドル
	char path[255];		//動画パス
	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ
	int volume = 255;	//ボリューム(最小)0～255(最大)
};

//シーンを管理する変数
//現在のゲームのシーン
GAME_SCENE GameScene;
//前回のゲームのシーン
GAME_SCENE GameSceneOld;
//次のゲームのシーン
GAME_SCENE GameSceneNext;

//プレイ背景の動画
MOVIE playMovie;

//プレイヤー
CHARACTER player;
//ゴール
CHARACTER goal;

//画面の切り替え
//フェードアウトしているか
bool isFadeOut = false;
//フェードインしているか
bool isFadeIn = false;

//切り替えミリ秒
int fadeTimeMill = 2000;
//ミリ秒をフレーム秒に変換
int fadeTimeMax = fadeTimeMill / 1000 * 60;

int fadeCnt = 0;

//フェードアウト
//初期値
int fadeOutCntInit = 0;
//フェードアウトのカウンタ
int fadeOutCnt = fadeOutCntInit;
//フェードアウトのカウンタMAX
int fadeOutCntMax = fadeTimeMax;

//フェードイン
//初期値
int fadeInCntInit = fadeTimeMax;
//フェードアウトのカウンタ
int fadeInCnt = fadeInCntInit;
//フェードアウトのカウンタ
int fadeInCntMax = fadeTimeMax;

//※Alt+Shift+左ドラッグ=矩形選択
// //プロトタイプ宣言
//画面を切り替える処理
void ChangeScene(GAME_SCENE scene);
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
//当たり判定の領域を更新
void CollUpdate(CHARACTER* chara);
void CollUpdate(CHARACTER* chara, int addLeft, int addTop, int addRight, int addBottom);
//当たり判定(Enter)
bool CollStay(CHARACTER chara1, CHARACTER chara2);

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
	
	//最初のシーンはタイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//プレイヤー画像の読み込み
	strcpyDx(playMovie.path, ".\\Movie\\PlayMovie.mp4");
	playMovie.handle = LoadGraph(playMovie.path);

	//動画が読み込みなかったときはエラーを返す
	if (playMovie.handle == -1) 
	{
		MessageBox(
			GetMainWindowHandle(),
			playMovie.path,
			"Main.cpp : playMovie.handle : 動画読み込みエラー",
			MB_OK
		);
	}

	//プレイヤー画像の読み込み
	strcpyDx(player.path, ".\\Images\\player.png");
	player.handle = LoadGraph(player.path);
	//ゴールの画像の読み込み
	strcpyDx(goal.path, ".\\Images\\goal.png");
	goal.handle = LoadGraph(goal.path);

	//画像が読み込めなかったときはエラー(-1)が入る
	if (player.handle == -1) 
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			player.path,			//メッセージ文
			"画像読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);

		DxLib_End();	//強制終了
		return -1;		//エラー終了
	}

	//画像の幅と高さを取得
	GetGraphSize(player.handle, &player.width, &player.height);
	GetGraphSize(goal.handle, &goal.width, &goal.height);

	//0524
	//当たり判定を更新する

	//プレイヤーの初期化(プレイヤーを画面中央に配置)
	player.x = GAME_WIDTH / 2 - player.width / 2;
	player.y = GAME_HEIGHT / 2 - player.height / 2;
	player.speed = 500;
	player.isDraw = true;
	CollUpdate(&player, 100, 50, 30, 10);	//プレイヤーの当たり判定のアドレス

	//ゴールの初期化
	goal.x = 400;
	goal.y = 400;
	goal.speed = 500;
	goal.isDraw = true;
	CollUpdate(&goal);	//ゴールの当たり判定のアドレス

	while (1)
	{		
		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }
		//キー入力状態を更新する
		AllKeyUpdate();

		//FPS値の更新※AllKeyUpdateなどの一連の処理が終了後に書く
		FpsUpdate();
		
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

		//シーンを切り替える
		if (GameSceneOld != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene!=GAME_SCENE_CHANGE) 
			{
				GameSceneNext = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		

		//FPS値を描画
		FpsDraw(0,100,true);

		//FPS値を待つ
		FpsWait();

		//ダブルバッファリングした画像を描画
		ScreenFlip();
	}

	//終わるときの処理
	DeleteGraph(playMovie.handle);	//プレイヤー画像をメモリ上から削除
	DeleteGraph(player.handle);	//プレイヤー画像をメモリ上から削除
	DeleteGraph(goal.handle);	//プレイヤー画像をメモリ上から削除


	// ＤＸライブラリ使用の終了処理準備(return 0でソフトが終了する)
	DxLib_End();				

	// ソフトの終了
	return 0;				 
}

/// <summary>
/// シーンを切り替える
/// </summary>
/// <param name="scene">切り替え先のシーン</param>
void ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;				//画面切り替え
	isFadeIn = false;				//フェードインにしない
	isFadeOut = true;				//フェードアウトする	
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
	//ゲーム画面に切り替わる
	if (KeyClick(KEY_INPUT_RETURN)) 
	{
		//次のシーンの初期化をここで行うと楽

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);
	}
	

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
	//ゲーム画面に切り替わる
	if (KeyClick(KEY_INPUT_RETURN))
	{
		//次のシーンの初期化をここで行うと楽

		//プレイ画面に切り替え
		/*ChangeScene(GAME_SCENE_END);*/
	}

	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_DOWN)) 
	{
		player.y +=player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_UP))
	{
		player.y -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_LEFT))
	{
		player.x -= player.speed* fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_RIGHT))
	{
		player.x += player.speed *fps.DeltaTime;
	}
	DrawFormatString(500, 500, GetColor(0, 0, 0), "delta : %f", fps.DeltaTime);

	//当たり判定を更新する
	CollUpdate(&player, 0, 0, 30, 10);		//プレイヤー
	CollUpdate(&goal);						//ゴール

	
	//当たり判定
	if (CollStay(player, goal))
	{
		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);

		return;		//処理強制終了(※エンド画面に移行するのでPlayをする必要がない)
	}
	

	

	return;
}

//描画
void PlayDraw() 
{
	//もし、動画が再生されていないとき
	if (GetMovieStateToGraph(playMovie.handle) == 0) 
	{
		//再生する

		//シークバーを戻す(開始時間0にする)
		SeekMovieToGraph(playMovie.handle, 0);		//シークバーを最初に戻す
		PlayMovieToGraph(playMovie.handle);			//動画を再生(まだ描画はしていない)
	}
	//動画を描画
	//動画のサイズで描画
	DrawGraph(0, 0, playMovie.handle, true);

	//画面のサイズに伸ばして描画
	//DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, true);


	const char* string = "Play";
	DrawString(0, 50, string, GetColor(0, 0, 0));

	//プレイヤーの描画
	if (player.isDraw) 
	{
		//画像の描画
		DrawGraph(player.x, player.y, player.handle, true);

		//デバックのときは当たり判定領域を描画
		if (GAME_DEBUG) 
		{
		//四角を描画
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(0, 0, 0), false);
		}
	}

	//ゴールの描画
	if (goal.isDraw) 
	{
		//画像の描画
		DrawGraph(goal.x, goal.y, goal.handle, true);

		//デバックのときは当たり判定領域を描画
		if (GAME_DEBUG)
		{
			DrawBox(goal.coll.left, goal.coll.top, goal.coll.right, goal.coll.bottom, GetColor(0, 0, 0), false);
		}
	}

	//DrawFormatString()

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
	//ゲーム画面に切り替わる
	if (KeyClick(KEY_INPUT_RETURN))
	{
		//次のシーンの初期化をここで行うと楽

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}

//描画
void EndDraw()
{
	const char* string = "End";
	DrawString(0, 50, string, GetColor(0, 0, 0));

	return;
}

//切り替え画面
void Change() 
{
	DrawString(0, 0, "読み込み",GetColor(0,0,0));

	//処理
	ChangeProc();
	//描画
	ChangeDraw();
}

//処理
void ChangeProc()
{
	//フェードイン
	if (isFadeIn) 
	{
		if (fadeInCnt > fadeInCntMax) 
		{
			//カウンタを減らす
			fadeInCnt--;
		}
		else //フェードイン処理が終了したとき
		{
			//カウンタを初期化
			fadeInCnt = fadeInCntInit;
			isFadeIn = false;
		}
	}

	//フェードアウト
	if (isFadeOut) 
	{
		if (fadeOutCnt < fadeOutCntMax) 
		{
		//カウンタを増やす
			fadeOutCnt++;
		}
		else 
		{
		//カウンタ初期化
			fadeOutCnt = fadeOutCntInit;;
			isFadeOut = false;
		}
	}

	//切り替え処理終了か
	if (isFadeIn == false && isFadeOut == false) 
	{
	//フェードインしていない、かつフェードアウトもしていない
		//次のシーンに切り替え
		GameScene = GameSceneNext;
		//以前のシーンの更新
		GameSceneOld = GameScene;
	}

	return;
}

//描画
void ChangeDraw()
{
	switch (GameSceneOld)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ　画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド　画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (isFadeIn) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (isFadeOut) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), true);
	
	//四角を描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え", GetColor(0, 0, 0));

	return;
}



/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="coll"></param>
void CollUpdate(CHARACTER* chara) 
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// 当たり判定の領域更新(四方のオーバーロード)
/// </summary>
/// <param name="chara"></param>
/// <param name="addLeft"></param>
/// <param name="addTop"></param>
/// <param name="addRight"></param>
/// <param name="addBottom"></param>
void CollUpdate(CHARACTER* chara, int addLeft, int addTop, int addRight, int addBottom)
{
	chara->coll.left = chara->x + addLeft;
	chara->coll.top = chara->y + addTop;
	chara->coll.right = chara->x + chara->width + addRight;
	chara->coll.bottom = chara->y + chara->height + addBottom;

	return;
}

//当たり判定(Enter)
bool CollStay(CHARACTER chara1, CHARACTER chara2)
{
	//描画されていないなら行わない
	if (chara1.isDraw && chara2.isDraw)
	{
		//当たり判定
		if (
			chara1.coll.left<chara2.coll.right
			&& chara1.coll.right>chara2.coll.left
			&& chara1.coll.top<chara2.coll.bottom
			&& chara1.coll.bottom>chara2.coll.top
			)
		{
			return true;
		}
	}
	

	return false;
}