#include "Game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "Fps.h"		//FPS�����̃w�b�_�t�@�C��
#include "Keyboard.h"	//�L�[�{�[�h�̏����̃w�b�_�t�@�C��
#define _CRT_SECURE_NO_WARNINGS

//�����蔻��̂Ȃ��摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h��(�Ǘ��ԍ�)
	char path[255];		//�摜�̏ꏊ(�p�X)
	int x;				//x�̈ʒu
	int y;				//y�̈ʒu
	int width;			//�摜�̕�
	int height;			//�摜�̍���
	int speed = 1;		//�ړ����x

	bool isDraw = false;//�摜���`��ł��邩
};

//�L�����N�^�̍\����
struct CHARACTER
{
	IMAGE img;			//�摜�\����

	int speed = 1;		//�ړ����x

	RECT coll;			//��`�����蔻��̈�
};

struct MOVIE
{
	int handle = -1;	//����n���h��
	char path[255];		//����p�X
	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����
	int volume = 255;	//�{�����[��(�ŏ�)0�`255(�ő�)
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;	//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int volume = -1;	//�{�����[��
	int playType = -1;
};

//�V�[�����Ǘ�����ϐ�
//���݂̃Q�[���̃V�[��
GAME_SCENE GameScene;
//�O��̃Q�[���̃V�[��
GAME_SCENE GameSceneOld;
//���̃Q�[���̃V�[��
GAME_SCENE GameSceneNext;

//�v���C�w�i�̓���
MOVIE playMovie;

//�v���C���[
CHARACTER player;
//�S�[��
CHARACTER goal;
//���S
IMAGE titleLogo;
IMAGE pushEnterLogo;
IMAGE gameClearLogo;

//���y
AUDIO titleBgm;
AUDIO playBgm;
AUDIO endBgm;
AUDIO workSe;

//��ʂ̐؂�ւ�
//�t�F�[�h�A�E�g���Ă��邩
bool isFadeOut = false;
//�t�F�[�h�C�����Ă��邩
bool isFadeIn = false;

//�؂�ւ��~���b
int fadeTimeMill = 2000;
//�~���b���t���[���b�ɕϊ�
int fadeTimeMax = fadeTimeMill / 1000 * 60;

int fadeCnt = 0;

//�t�F�[�h�A�E�g
//�����l
int fadeOutCntInit = 0;
//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCnt = fadeOutCntInit;
//�t�F�[�h�A�E�g�̃J�E���^MAX
int fadeOutCntMax = fadeTimeMax;

//�t�F�[�h�C��
//�����l
int fadeInCntInit = fadeTimeMax;
//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCnt = fadeInCntInit;
//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;

//titleLogo�̏㉺
int titleLogoCnt = 0;
const int titleLogoCntMax = 120;
bool isTitleEnterBrink = false;

//pushEnter�̓_��
int pushEnterCnt = 0;
const int pushEnterCntMax = 60;
bool isPushEnterBrink = false;

//�Q�[���S�̂̏�����
bool GameLoad();
//�摜�̓ǂݍ���
bool CharacterLoad(CHARACTER* chara, const char* path);
bool ImageLoad(IMAGE* chara, const char* path);
//���y�ǂݍ���
bool AudioLoad(AUDIO* audio, const char* path, int playType, int volume);
//
void GameInit();
//��Alt+Shift+���h���b�O=��`�I��
// //�v���g�^�C�v�錾
//��ʂ�؂�ւ��鏈��
void ChangeScene(GAME_SCENE scene);
//�^�C�g�����
void Title();
//�^�C�g����� ����
void TitleProc();
//�^�C�g����� �`��
void TitleDraw();
//�v���C���
void Play();
//�v���C��� ����
void PlayProc();
//�v���C��� �`��
void PlayDraw();
 
//�G���h���
void End();
//�G���h��� ����
void EndProc();
//�G���h��� �`��
void EndDraw();
//�؂�ւ����
void Change();
//�؂�ւ���� ����
void ChangeProc();
//�؂�ւ���� �`��
void ChangeDraw();
//�����蔻��̗̈���X�V
void CollUpdate(CHARACTER* chara);
void CollUpdate(CHARACTER* chara, int addLeft, int addTop, int addRight, int addBottom);
//�����蔻��(Enter)
bool CollStay(CHARACTER chara1, CHARACTER chara2);

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Log.txt���o�͂��Ȃ�(GitHub�ŊǗ����邽�߁A���ݕK�v�Ȃ�)
	SetOutApplicationLogValidFlag(FALSE);	
	//�E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);
	//�Q�[���^�C�g���̐ݒ�
	SetMainWindowText(GAME_TITLE);
	//�Q�[���E�B���h�E�𑜓x�ݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	//�E�B���h�E�̃T�C�Y��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);
	//�w�i�F�̐ݒ�
	SetBackgroundColor(BACK_GROUND_COLOR);
	//�Q�[����ICON��ݒ�
	SetWindowIconID(GAME_ICON_ID);
	//�E�B���h�E�o�[�̏��
	SetWindowStyleMode(GAME_WINDOW_BAR);
	//�E�B���h�E�̐���������L���ɂ���(�f�B�X�v���C�̔\�͂𐧌����āA�t���[�����[�g(fps)�����ɕۂ�)=FALSE�̓t���[�����[�g���Ǘ����Ȃ���΂Ȃ�Ȃ�
	SetWaitVSyncFlag(TRUE);
	//��A�N�e�B�u�ł����s����
	SetAlwaysRunFlag(TRUE);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);
	
	//�ŏ��̃V�[���̓^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���ǂݍ���
	if (!GameLoad()) 
	{
	//�f�[�^�̓ǂݍ��݂Ɏ��s������
		DxLib_End();	//DX���C�u�����̏I��
		return -1;
	}

	//�Q�[���̏�����
	GameInit();
	
	while (1)
	{		
		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }
		//�L�[���͏�Ԃ��X�V����
		AllKeyUpdate();

		//FPS�l�̍X�V��AllKeyUpdate�Ȃǂ̈�A�̏������I����ɏ���
		FpsUpdate();
		
		//���b�Z�[�W���󂯎�葱����A�~�ȂǂŃE�B���h�E�������Ƃ�
		if (ProcessMessage() != 0) 
		{
			//�������[�v�𔲂���
			break;
		}


		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE)) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE) 
		{
			GameSceneOld = GameScene;
		}

		//��switch tab��enum��� tab��switch_on�̒���enum�̕ϐ������� enter
		switch (GameScene)
		{
			//�^�C�g�����
		case GAME_SCENE_TITLE:
			Title();
			break;

			//�v���C���
		case GAME_SCENE_PLAY:
			Play();
			break;

			//�G���h���
		case GAME_SCENE_END:
			End();
			break;

			//�؂�ւ����
		case GAME_SCENE_CHANGE:
			Change();
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (GameSceneOld != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene!=GAME_SCENE_CHANGE) 
			{
				GameSceneNext = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		

		//FPS�l��`��
		FpsDraw(0,GAME_HEIGHT-16,true);

		//FPS�l��҂�
		FpsWait();

		//�_�u���o�b�t�@�����O�����摜��`��
		ScreenFlip();
	}

	//�I���Ƃ��̏���
	//�摜���������ォ��폜
	DeleteGraph(playMovie.handle);	
	DeleteGraph(player.img.handle);	
	DeleteGraph(goal.img.handle);	
	DeleteGraph(titleLogo.handle);	
	DeleteGraph(pushEnterLogo.handle);	
	DeleteGraph(gameClearLogo.handle);	

	//���y�̏���
	DeleteSoundMem(titleBgm.handle);
	DeleteSoundMem(playBgm.handle);
	DeleteSoundMem(endBgm.handle);
	DeleteSoundMem(workSe.handle);


	// �c�w���C�u�����g�p�̏I����������(return 0�Ń\�t�g���I������)
	DxLib_End();				

	// �\�t�g�̏I��
	return 0;				 
}

const int VOLUME_MAX = 255;
const int BGM_LOOP = DX_PLAYTYPE_LOOP;

/// <summary>
/// �Q�[���f�[�^�̓ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���true, �ǂݍ��߂Ȃ�������false</returns>
bool GameLoad() 
{
	//�v���C����̓ǂݍ���
	strcpyDx(playMovie.path, ".\\Movie\\PlayMovie.mp4");
	playMovie.handle = LoadGraph(playMovie.path);

	//���悪�ǂݍ��݂Ȃ������Ƃ��̓G���[��Ԃ�
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			playMovie.path,
			"Main.cpp : playMovie.handle : ����ǂݍ��݃G���[",
			MB_OK
		);

		return false;
	}

	//�v���C���[�摜�ǂݍ���
	if (CharacterLoad(&player, ".\\Images\\player.png") == false) { return false; }
	//�S�[���摜�̓ǂݍ���
	if (CharacterLoad(&goal, ".\\Images\\goal.png") == false) { return false; }
	//���S�̓ǂݍ���
	if (ImageLoad(&titleLogo, ".\\Images\\titleLogo.png") == false) { return false; }
	if (ImageLoad(&pushEnterLogo, ".\\Images\\pushEnterLogo.png") == false) { return false; }
	if (ImageLoad(&gameClearLogo, ".\\Images\\gameClearLogo.png") == false) { return false; }

	//���ʂ������߂ɒ���
	int soundVolume = 100;

	//���y�̓ǂݍ���
	if (AudioLoad(
		&titleBgm,
		".\\Audio\\������  �t�@���^�W�[14.mp3",
		BGM_LOOP,
		VOLUME_MAX)
		== false) {
		return false;
	}
	//�^�C�g��BGM�̃{�����[���ύX
	ChangeVolumeSoundMem(soundVolume, titleBgm.handle);

	if (AudioLoad(
		&playBgm,
		".\\Audio\\������  �t�@���^�W�[15.mp3",
		BGM_LOOP,
		VOLUME_MAX)
		== false) {
		return false;
	}
	//�v���CBGM�̃{�����[���ύX
	ChangeVolumeSoundMem(soundVolume, playBgm.handle);

	if (AudioLoad(
		&endBgm,
		".\\Audio\\������  �A�R�[�X�e�B�b�N52.mp3",
		BGM_LOOP,
		VOLUME_MAX)
		== false) {
		return false;
	}
	//�G���hBGM�̃{�����[���ύX
	ChangeVolumeSoundMem(soundVolume, endBgm.handle);

	if (AudioLoad(
		&workSe,
		".\\Audio\\���ʉ����{_�v�C�ŕ���.mp3",
		DX_PLAYTYPE_BACK,
		VOLUME_MAX)
		== false) {
		return false;
	}

	return true;
}

//�L�����N�^�[�̓ǂݍ���
bool CharacterLoad(CHARACTER* chara, const char* path)
{
	//�摜�̓ǂݍ���
	strcpyDx(chara->img.path, path);
	chara->img.handle = LoadGraph(chara->img.path);

	if (chara->img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			chara->img.path,
			"�摜�ǂݍ��݃G���[",
			MB_OK
		);

		return false;
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(chara->img.handle, &chara->img.width, &chara->img.height);

	return true;
}

//�摜�̓ǂݍ���
bool ImageLoad(IMAGE* chara, const char* path)
{
	//�摜�̓ǂݍ���
	strcpyDx(chara->path, path);
	chara->handle = LoadGraph(chara->path);

	if (chara->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			chara->path,
			"�摜�ǂݍ��݃G���[",
			MB_OK
		);

		return false;
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(chara->handle, &chara->width, &chara->height);

	return true;
}

//���y�̓ǂݍ���
bool AudioLoad(AUDIO* audio,const char* path, int playType, int volume)
{
	//���y�̓ǂݍ���
	strcpyDx(audio->path, path);
	audio->handle = LoadSoundMem(audio->path);

	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			audio->path,
			"���y�ǂݍ��݃G���[",
			MB_OK
		);

		return false;
	}

	audio->playType = playType;
	audio->volume = volume;

	return true;
}

/// <summary>
/// �Q�[���f�[�^�̏�����
/// </summary>
void GameInit() 
{
	

	//0524
	//�����蔻����X�V����

	//�v���C���[�̏�����(�v���C���[����ʒ����ɔz�u)
	player.img.x = GAME_WIDTH / 2 - player.img.width / 2;
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;
	player.speed = 500;
	player.img.isDraw = true;
	CollUpdate(&player, 100, 50, 30, 10);	//�v���C���[�̓����蔻��̃A�h���X

	//�S�[���̏�����
	goal.img.x = 400;
	goal.img.y = 400;
	goal.speed = 500;
	goal.img.isDraw = true;
	CollUpdate(&goal);	//�S�[���̓����蔻��̃A�h���X

	//�^�C�g�����S�̈ʒu
	titleLogo.x = GAME_WIDTH / 2 - titleLogo.width / 2;	//��������
	titleLogo.y = 100;

	//�^�C�g�����S�̏㉺
	titleLogoCnt = 0;
	isTitleEnterBrink = false;

	//PushEnter���S�̈ʒu�����߂�
	pushEnterLogo.x = GAME_WIDTH / 2 - pushEnterLogo.width / 2;	//��������
	pushEnterLogo.y = GAME_HEIGHT - pushEnterLogo.height - titleLogo.y;

	//PushEnter�̓_��
	pushEnterCnt = 0;
	isPushEnterBrink = false;

	//�N���A���S�̈ʒu�����߂�
	gameClearLogo.x = GAME_WIDTH / 2 - gameClearLogo.width / 2;		//��������
	gameClearLogo.y = GAME_HEIGHT / 2 - gameClearLogo.height / 2;	//��������
}

/// <summary>
/// �V�[����؂�ւ���
/// </summary>
/// <param name="scene">�؂�ւ���̃V�[��</param>
void ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;				//��ʐ؂�ւ�
	isFadeIn = false;				//�t�F�[�h�C���ɂ��Ȃ�
	isFadeOut = true;				//�t�F�[�h�A�E�g����	
}

/// <summary>
/// �^�C�g�����
/// </summary>
void Title() 
{
	//����
	TitleProc();
	//�`��
	TitleDraw();
}

//����
void TitleProc() 
{
	

	//�Q�[����ʂɐ؂�ւ��
	if (KeyClick(KEY_INPUT_RETURN)) 
	{
		//���̃V�[���̏������������ōs���Ɗy
		GameInit();	//�Q�[���̏�����

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		//���y���~
		if (CheckSoundMem(titleBgm.handle) == 1)
		{
			StopSoundMem(titleBgm.handle);	
		}

		return;
	}
	
	//���y���Đ�
	if (CheckSoundMem(titleBgm.handle) == 0)
	{
		PlaySoundMem(titleBgm.handle, titleBgm.playType);
	}

	return;
}

//�`��
void TitleDraw() 
{
	if (GAME_DEBUG)
	{
		const char* string = "Title";
		DrawString(0, 50, string, GetColor(0, 0, 0));
	}
	//�����C���[�T�O�͉��ɏ������������Q�[����ʂł͏�̃��C���[�ɕ\�������(photoshop��illustrator�Ƌt)

	int titleAddYMax = 40;
	//�^�C�g�����S���㉺�ɓ�����
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

	//���S�̕\��
	DrawGraph(titleLogo.x, titleLogo.y + ((float)titleLogoCnt/titleLogoCntMax)*titleAddYMax, titleLogo.handle, true);


	//pushEnter�_��
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
	//pushEnter�̕\��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)pushEnterCnt / pushEnterCntMax) * 255);
	DrawGraph(pushEnterLogo.x, pushEnterLogo.y, pushEnterLogo.handle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	return;
}

//�v���C���
void Play() 
{
	//����
	PlayProc();
	//�`��
	PlayDraw();

	
}

//����
void PlayProc() 
{
	//���y���Đ�
	if (CheckSoundMem(playBgm.handle) == 0)
	{
		PlaySoundMem(playBgm.handle, playBgm.playType);
	}

	//�v���C���[�̑���
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

	//�ړ���
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
	

	//�����蔻����X�V����
	CollUpdate(&player, 0, 0, 30, 10);		//�v���C���[
	CollUpdate(&goal);						//�S�[��

	
	//�����蔻��
	if (CollStay(player, goal))
	{
		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		//���y���~
		if (CheckSoundMem(playBgm.handle) == 1)
		{
			StopSoundMem(playBgm.handle);
		}

		return;		//���������I��(���G���h��ʂɈڍs����̂�Play������K�v���Ȃ�)
	}
	

	

	return;
}

//�`��
void PlayDraw() 
{
	//�����A���悪�Đ�����Ă��Ȃ��Ƃ�
	if (GetMovieStateToGraph(playMovie.handle) == 0) 
	{
		//�Đ�����

		//�V�[�N�o�[��߂�(�J�n����0�ɂ���)
		SeekMovieToGraph(playMovie.handle, 0);		//�V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playMovie.handle);			//������Đ�(�܂��`��͂��Ă��Ȃ�)
	}
	//�����`��
	//����̃T�C�Y�ŕ`��
	DrawGraph(0, 0, playMovie.handle, true);

	//��ʂ̃T�C�Y�ɐL�΂��ĕ`��
	//DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, true);

	if (GAME_DEBUG)
	{
		const char* string = "Play";
		DrawString(0, 50, string, GetColor(0, 0, 0));
	}

	//�v���C���[�̕`��
	if (player.img.isDraw)
	{
		//�摜�̕`��
		DrawGraph(player.img.x, player.img.y, player.img.handle, true);

		//�f�o�b�N�̂Ƃ��͓����蔻��̈��`��
		if (GAME_DEBUG) 
		{
		//�l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(0, 0, 0), false);
		}
	}

	//�S�[���̕`��
	if (goal.img.isDraw)
	{
		//�摜�̕`��
		DrawGraph(goal.img.x, goal.img.y, goal.img.handle, true);

		//�f�o�b�N�̂Ƃ��͓����蔻��̈��`��
		if (GAME_DEBUG)
		{
			DrawBox(goal.coll.left, goal.coll.top, goal.coll.right, goal.coll.bottom, GetColor(0, 0, 0), false);
		}
	}

	//DrawFormatString()

	return;
}

//�G���h���
void End() 
{
	//����
	EndProc();
	//�`��
	EndDraw();
}

//����
void EndProc()
{
	//�Q�[����ʂɐ؂�ւ��
	if (KeyClick(KEY_INPUT_RETURN))
	{
		//���̃V�[���̏������������ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		//���y���~
		if (CheckSoundMem(endBgm.handle) == 1)
		{
			StopSoundMem(endBgm.handle);
		}

		return;
	}

	//���y���Đ�
	if (CheckSoundMem(endBgm.handle) == 0)
	{
		PlaySoundMem(endBgm.handle, endBgm.playType);
	}

	return;
}

//�`��
void EndDraw()
{
	if (GAME_DEBUG)
	{
		const char* string = "End";
		DrawString(0, 50, string, GetColor(0, 0, 0));
	}

	//�Q�[���N���A���S�\��
	DrawGraph(gameClearLogo.x, gameClearLogo.y, gameClearLogo.handle, true);

	return;
}

//�؂�ւ����
void Change() 
{
	if (GAME_DEBUG)
	{
		DrawString(0, 100, "�ǂݍ���", GetColor(0, 0, 0));
	}

	//����
	ChangeProc();
	//�`��
	ChangeDraw();
}

//����
void ChangeProc()
{
	//�t�F�[�h�C��
	if (isFadeIn) 
	{
		if (fadeInCnt > fadeInCntMax) 
		{
			//�J�E���^�����炷
			fadeInCnt--;
		}
		else //�t�F�[�h�C���������I�������Ƃ�
		{
			//�J�E���^��������
			fadeInCnt = fadeInCntInit;
			isFadeIn = false;
		}
	}

	//�t�F�[�h�A�E�g
	if (isFadeOut) 
	{
		if (fadeOutCnt < fadeOutCntMax) 
		{
		//�J�E���^�𑝂₷
			fadeOutCnt++;
		}
		else 
		{
		//�J�E���^������
			fadeOutCnt = fadeOutCntInit;;
			isFadeOut = false;
		}
	}

	//�؂�ւ������I����
	if (isFadeIn == false && isFadeOut == false) 
	{
	//�t�F�[�h�C�����Ă��Ȃ��A���t�F�[�h�A�E�g�����Ă��Ȃ�
		//���̃V�[���ɐ؂�ւ�
		GameScene = GameSceneNext;
		//�ȑO�̃V�[���̍X�V
		GameSceneOld = GameScene;
	}

	return;
}

//�`��
void ChangeDraw()
{
	switch (GameSceneOld)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C�@��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h�@��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (isFadeIn) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (isFadeOut) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), true);
	
	//�l�p��`��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (GAME_DEBUG)
	{
		DrawString(0, 0, "�؂�ւ�", GetColor(0, 0, 0));
	}

	return;
}



/// <summary>
/// �����蔻��̗̈�X�V
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
/// �����蔻��̗̈�X�V(�l���̃I�[�o�[���[�h)
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

//�����蔻��(Enter)
bool CollStay(CHARACTER chara1, CHARACTER chara2)
{
	//�`�悳��Ă��Ȃ��Ȃ�s��Ȃ�
	if (chara1.img.isDraw && chara2.img.isDraw)
	{
		//�����蔻��
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