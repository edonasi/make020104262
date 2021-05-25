//FPSソースファイル

//ヘッダファイルの読み込み
#include "DxLib.h"
#include "Fps.h"

//グローバル変数
FPS fps;	//fpsの管理
float divDeltaTime = 1000000.0f;	//deltaTimeを割る数(1000000.0f)

//関数

/// <summary>
/// Fps値を測定し、値を更新する
/// </summary>
/// <param name=""></param>
void FpsUpdate(void) 
{
	//一番最初の処理
	if (fps.IsInitFlag == false) 
	{
		//測定開始時刻をμ秒単位で取得
		fps.StartTime = GetNowHiPerformanceCount();	//Windowが起動してから経過した時間(マイクロ秒)

		//最初の処理終了
		fps.IsInitFlag = true;
	}

	//現在の時刻をμ秒単位で取得
	fps.NowTime = GetNowHiPerformanceCount();

	//前回取得した時間からの経過時間を秒に(小数)に変換してからセット
	fps.DeltaTime = (fps.NowTime - fps.OldTime) / divDeltaTime;

	//今回取得した時間を保存
	fps.OldTime = fps.NowTime;

	//1フレーム目～FPS設定値まではカウントアップ
	if (fps.Count < fps.SampleRote) 
	{
		//カウンタを増やす
		fps.Count++;
	}
	else	//FPS設定値フレームで平均FPSを計算
	{
		//現在の時刻から0フレームmwの時間をひきFPSの数値で割る
		fps.DrawValue =
			divDeltaTime / ((fps.NowTime - fps.StartTime) / static_cast<float>(fps.SampleRote));
	
		//測定開始時刻をマイクロ秒単位で取得
		fps.StartTime = GetNowHiPerformanceCount();	//Windowsが起動してから経過した時間

		//カウンタ初期化
		fps.Count = 1;
	}

	return;
}

/// <summary>
/// デバック用の平均FPS値を描画する
/// </summary>
/// <param name="draw_x">描画するx位置</param>
/// <param name="draw_y">描画するy位置</param>
/// <param name="isBlackColor">描画色を黒にするか</param>
void FpsDraw(int draw_x,int draw_y,bool isBlackColor) 
{
	//デバックモードなら描画する
	if (GAME_DEBUG) 
	{
		//描画色(初期は白)
		int drawColor = 255;
		//引数で黒にする命令が出たら描画色を黒にする
		if (isBlackColor)
		{
			drawColor = 0;
		}

		//文字列を描画
		DrawFormatString(draw_x, draw_y, GetColor(drawColor, drawColor, drawColor),
			"FPS : %.1f", fps.DrawValue);

	}
	
	return;
}

/// <summary>
/// FPSで処理を待つ
/// </summary>
void FpsWait() 
{
	//現在の時刻-最初の時刻で、現在かかっている時刻を取得する
	LONGLONG resultTime = fps.NowTime - fps.StartTime;

	//待つべきミリ秒数(1秒/FPS値＊現在のフレーム数)から、現在かかっている時刻を引く
	LONGLONG waitTime = divDeltaTime / fps.Value * fps.Count - resultTime;

	//ミリ秒に変換させる割る数
	int millTime = 1000.0f;
	//マイクロ秒からミリ秒に変換
	waitTime /= millTime;

	//処理が速かったら、処理を待つ
	if (waitTime > 0) 
	{
		WaitTimer(waitTime);	//引数ミリ秒待つ(※WindowProcで停止するので途中でイベントが起きたときその処理を行う)
	}

	//垂直同期をOFFにしているか
	if (GetWaitVSyncFlag() == false) 
	{
		//FPSが最大値ではないとき
		if (fps.Value < GAME_FPS_MAX) 
		{
			//1秒毎のFPS値よりも、待つ時間が小さいときは、もっとFPS値を上げてもよい
			//待つ時間10ミリ <= 1秒/60FPS=16.6666ミリ　もう少し早くできる
			if (waitTime > 0
				&& waitTime <= millTime / fps.Value)
			{
				fps.Value++;
			}
			else 
			{
				//FPS値が追いついておらず、遅いときは、FPS値を下げる
				if (fps.Value > GAME_FPS_MIN) 
				{
					fps.Value--;
				}
			}
		}
	}

	return;
}