#include "Game.h"		//ゲーム全体のヘッダファイル
#include "Fps.h"		//FPS処理のヘッダファイル
#include "Keyboard.h"	//キーボードの処理のヘッダファイル
#define _CRT_SECURE_NO_WARNINGS

//当たり判定のない画像の構造体
struct IMAGE
{
	int handle = -1;	//画像のハンドル(管理番号)
	char path[255];		//画像の場所(パス)
	int x;				//xの位置
	int y;				//yの位置
	int width;			//画像の幅
	int height;			//画像の高さ
	int speed = 1;		//移動速度

	bool isDraw = false;//画像が描画できるか
};

//キャラクタの構造体
struct CHARACTER
{
	IMAGE img;			//画像構造体

	int speed = 1;		//移動速度

	RECT coll;			//矩形当たり判定領域
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

//音楽の構造体
struct AUDIO
{
	int handle = -1;	//音楽のハンドル
	char path[255];		//音楽のパス

	int volume = -1;	//ボリューム
	int playType = -1;
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
//ロゴ
IMAGE titleLogo;
IMAGE pushEnterLogo;
IMAGE gameClearLogo;

//音楽
AUDIO titleBgm;
AUDIO playBgm;
AUDIO endBgm;
AUDIO workSe;

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

//titleLogoの上下
int titleLogoCnt = 0;
const int titleLogoCntMax = 120;
bool isTitleEnterBrink = false;

//pushEnterの点滅
int pushEnterCnt = 0;
const int pushEnterCntMax = 60;
bool isPushEnterBrink = false;

//ゲーム全体の初期化
bool GameLoad();
//画像の読み込み
bool CharacterLoad(CHARACTER* chara, const char* path);
bool ImageLoad(IMAGE* chara, const char* path);
//音楽読み込み
bool AudioLoad(AUDIO* audio, const char* path, int playType, int volume);
//
void GameInit();
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

	//ゲーム読み込み
	if (!GameLoad()) 
	{
	//データの読み込みに失敗した時
		DxLib_End();	//DXライブラリの終了
		return -1;
	}

	//ゲームの初期化
	GameInit();
	
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
		FpsDraw(0,GAME_HEIGHT-16,true);

		//FPS値を待つ
		FpsWait();

		//ダブルバッファリングした画像を描画
		ScreenFlip();
	}

	//終わるときの処理
	//画像をメモリ上から削除
	DeleteGraph(playMovie.handle);	
	DeleteGraph(player.img.handle);	
	DeleteGraph(goal.img.handle);	
	DeleteGraph(titleLogo.handle);	
	DeleteGraph(pushEnterLogo.handle);	
	DeleteGraph(gameClearLogo.handle);	

	//音楽の消去
	DeleteSoundMem(titleBgm.handle);
	DeleteSoundMem(playBgm.handle);
	DeleteSoundMem(endBgm.handle);
	DeleteSoundMem(workSe.handle);


	// ＤＸライブラリ使用の終了処理準備(return 0でソフトが終了する)
	DxLib_End();				

	// ソフトの終了
	return 0;				 
}

const int VOLUME_MAX = 255;
const int BGM_LOOP = DX_PLAYTYPE_LOOP;

/// <summary>
/// ゲームデータの読み込み
/// </summary>
/// <returns>読み込めたらtrue, 読み込めなかったらfalse</returns>
bool GameLoad() 
{
	//プレイ動画の読み込み
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

		return false;
	}

	//プレイヤー画像読み込み
	if (CharacterLoad(&player, ".\\Images\\player.png") == false) { return false; }
	//ゴール画像の読み込み
	if (CharacterLoad(&goal, ".\\Images\\goal.png") == false) { return false; }
	//ロゴの読み込み
	if (ImageLoad(&titleLogo, ".\\Images\\titleLogo.png") == false) { return false; }
	if (ImageLoad(&pushEnterLogo, ".\\Images\\pushEnterLogo.png") == false) { return false; }
	if (ImageLoad(&gameClearLogo, ".\\Images\\gameClearLogo.png") == false) { return false; }

	//音量を小さめに調整
	int soundVolume = 100;

	//音楽の読み込み
	if (AudioLoad(
		&titleBgm,
		".\\Audio\\魔王魂  ファンタジー14.mp3",
		BGM_LOOP,
		VOLUME_MAX)
		== false) {
		return false;
	}
	//タイトルBGMのボリューム変更
	ChangeVolumeSoundMem(soundVolume, titleBgm.handle);

	if (AudioLoad(
		&playBgm,
		".\\Audio\\魔王魂  ファンタジー15.mp3",
		BGM_LOOP,
		VOLUME_MAX)
		== false) {
		return false;
	}
	//プレイBGMのボリューム変更
	ChangeVolumeSoundMem(soundVolume, playBgm.handle);

	if (AudioLoad(
		&endBgm,
		".\\Audio\\魔王魂  アコースティック52.mp3",
		BGM_LOOP,
		VOLUME_MAX)
		== false) {
		return false;
	}
	//エンドBGMのボリューム変更
	ChangeVolumeSoundMem(soundVolume, endBgm.handle);

	if (AudioLoad(
		&workSe,
		".\\Audio\\効果音ラボ_革靴で歩く.mp3",
		DX_PLAYTYPE_BACK,
		VOLUME_MAX)
		== false) {
		return false;
	}

	return true;
}

//キャラクターの読み込み
bool CharacterLoad(CHARACTER* chara, const char* path)
{
	//画像の読み込み
	strcpyDx(chara->img.path, path);
	chara->img.handle = LoadGraph(chara->img.path);

	if (chara->img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			chara->img.path,
			"画像読み込みエラー",
			MB_OK
		);

		return false;
	}

	//画像の幅と高さを取得
	GetGraphSize(chara->img.handle, &chara->img.width, &chara->img.height);

	return true;
}

//画像の読み込み
bool ImageLoad(IMAGE* chara, const char* path)
{
	//画像の読み込み
	strcpyDx(chara->path, path);
	chara->handle = LoadGraph(chara->path);

	if (chara->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			chara->path,
			"画像読み込みエラー",
			MB_OK
		);

		return false;
	}

	//画像の幅と高さを取得
	GetGraphSize(chara->handle, &chara->width, &chara->height);

	return true;
}

//音楽の読み込み
bool AudioLoad(AUDIO* audio,const char* path, int playType, int volume)
{
	//音楽の読み込み
	strcpyDx(audio->path, path);
	audio->handle = LoadSoundMem(audio->path);

	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			audio->path,
			"音楽読み込みエラー",
			MB_OK
		);

		return false;
	}

	audio->playType = playType;
	audio->volume = volume;

	return true;
}

/// <summary>
/// ゲームデータの初期化
/// </summary>
void GameInit() 
{
	

	//0524
	//当たり判定を更新する

	//プレイヤーの初期化(プレイヤーを画面中央に配置)
	player.img.x = GAME_WIDTH / 2 - player.img.width / 2;
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;
	player.speed = 500;
	player.img.isDraw = true;
	CollUpdate(&player, 100, 50, 30, 10);	//プレイヤーの当たり判定のアドレス

	//ゴールの初期化
	goal.img.x = 400;
	goal.img.y = 400;
	goal.speed = 500;
	goal.img.isDraw = true;
	CollUpdate(&goal);	//ゴールの当たり判定のアドレス

	//タイトルロゴの位置
	titleLogo.x = GAME_WIDTH / 2 - titleLogo.width / 2;	//中央揃え
	titleLogo.y = 100;

	//タイトルロゴの上下
	titleLogoCnt = 0;
	isTitleEnterBrink = false;

	//PushEnterロゴの位置を決める
	pushEnterLogo.x = GAME_WIDTH / 2 - pushEnterLogo.width / 2;	//中央揃え
	pushEnterLogo.y = GAME_HEIGHT - pushEnterLogo.height - titleLogo.y;

	//PushEnterの点滅
	pushEnterCnt = 0;
	isPushEnterBrink = false;

	//クリアロゴの位置を決める
	gameClearLogo.x = GAME_WIDTH / 2 - gameClearLogo.width / 2;		//中央揃え
	gameClearLogo.y = GAME_HEIGHT / 2 - gameClearLogo.height / 2;	//中央揃え
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
		GameInit();	//ゲームの初期化

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);

		//音楽を停止
		if (CheckSoundMem(titleBgm.handle) == 1)
		{
			StopSoundMem(titleBgm.handle);	
		}

		return;
	}
	
	//音楽を再生
	if (CheckSoundMem(titleBgm.handle) == 0)
	{
		PlaySoundMem(titleBgm.handle, titleBgm.playType);
	}

	return;
}

//描画
void TitleDraw() 
{
	if (GAME_DEBUG)
	{
		const char* string = "Title";
		DrawString(0, 50, string, GetColor(0, 0, 0));
	}
	//※レイヤー概念は下に書いた処理がゲーム画面では上のレイヤーに表示される(photoshopやillustratorと逆)

	int titleAddYMax = 40;
	//タイトルロゴを上下に動かす
	if (isTitleEnterBrink) 
	{
		titleLogoCnt -= 4;

		if (titleLogoCnt < 0) 
		{
			isTitleEnterBrink = false; 
			titleLogoCnt = 0;
		}
	}
	else 
	{
		titleLogoCnt++;

		if (titleLogoCnt > titleLogoCntMax) 
		{ 
			isTitleEnterBrink = true; 
			titleLogoCnt = titleLogoCntMax;
		}
	}

	//ロゴの表示
	DrawGraph(titleLogo.x, titleLogo.y + ((float)titleLogoCnt/titleLogoCntMax)*titleAddYMax, titleLogo.handle, true);


	//pushEnter点滅
	if (isPushEnterBrink)
	{
		pushEnterCnt--;

		if (pushEnterCnt < 0) { isPushEnterBrink = false; }
	}
	else
	{
		pushEnterCnt++;

		if (pushEnterCnt > pushEnterCntMax) { isPushEnterBrink = true; }
	}
	//pushEnterの表示
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)pushEnterCnt / pushEnterCntMax) * 255);
	DrawGraph(pushEnterLogo.x, pushEnterLogo.y, pushEnterLogo.handle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


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
	//音楽を再生
	if (CheckSoundMem(playBgm.handle) == 0)
	{
		PlaySoundMem(playBgm.handle, playBgm.playType);
	}

	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_DOWN)) 
	{
		player.img.y +=player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_UP))
	{
		player.img.y -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_LEFT))
	{
		player.img.x -= player.speed* fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_RIGHT))
	{
		player.img.x += player.speed *fps.DeltaTime;
	}

	//移動音
	if (KeyDown(KEY_INPUT_DOWN) || KeyDown(KEY_INPUT_UP)
		|| KeyDown(KEY_INPUT_RIGHT) || KeyDown(KEY_INPUT_LEFT))
	{
		if (CheckSoundMem(workSe.handle) == 0)
		{
			PlaySoundMem(workSe.handle, workSe.playType);
		}
	}

	if (GAME_DEBUG) 
	{
		DrawFormatString(500, 500, GetColor(0, 0, 0), "delta : %f", fps.DeltaTime);
	}
	

	//当たり判定を更新する
	CollUpdate(&player, 0, 0, 30, 10);		//プレイヤー
	CollUpdate(&goal);						//ゴール

	
	//当たり判定
	if (CollStay(player, goal))
	{
		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);

		//音楽を停止
		if (CheckSoundMem(playBgm.handle) == 1)
		{
			StopSoundMem(playBgm.handle);
		}

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

	if (GAME_DEBUG)
	{
		const char* string = "Play";
		DrawString(0, 50, string, GetColor(0, 0, 0));
	}

	//プレイヤーの描画
	if (player.img.isDraw)
	{
		//画像の描画
		DrawGraph(player.img.x, player.img.y, player.img.handle, true);

		//デバックのときは当たり判定領域を描画
		if (GAME_DEBUG) 
		{
		//四角を描画
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(0, 0, 0), false);
		}
	}

	//ゴールの描画
	if (goal.img.isDraw)
	{
		//画像の描画
		DrawGraph(goal.img.x, goal.img.y, goal.img.handle, true);

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

		//音楽を停止
		if (CheckSoundMem(endBgm.handle) == 1)
		{
			StopSoundMem(endBgm.handle);
		}

		return;
	}

	//音楽を再生
	if (CheckSoundMem(endBgm.handle) == 0)
	{
		PlaySoundMem(endBgm.handle, endBgm.playType);
	}

	return;
}

//描画
void EndDraw()
{
	if (GAME_DEBUG)
	{
		const char* string = "End";
		DrawString(0, 50, string, GetColor(0, 0, 0));
	}

	//ゲームクリアロゴ表示
	DrawGraph(gameClearLogo.x, gameClearLogo.y, gameClearLogo.handle, true);

	return;
}

//切り替え画面
void Change() 
{
	if (GAME_DEBUG)
	{
		DrawString(0, 100, "読み込み", GetColor(0, 0, 0));
	}

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

	if (GAME_DEBUG)
	{
		DrawString(0, 0, "切り替え", GetColor(0, 0, 0));
	}

	return;
}



/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="coll"></param>
void CollUpdate(CHARACTER* chara) 
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

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
	chara->coll.left = chara->img.x + addLeft;
	chara->coll.top = chara->img.y + addTop;
	chara->coll.right = chara->img.x + chara->img.width + addRight;
	chara->coll.bottom = chara->img.y + chara->img.height + addBottom;

	return;
}

//当たり判定(Enter)
bool CollStay(CHARACTER chara1, CHARACTER chara2)
{
	//描画されていないなら行わない
	if (chara1.img.isDraw && chara2.img.isDraw)
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