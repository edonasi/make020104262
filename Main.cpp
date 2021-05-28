#include "Game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "Fps.h"		//FPS�����̃w�b�_�t�@�C��
#include "Keyboard.h"	//�L�[�{�[�h�̏����̃w�b�_�t�@�C��

//�L�����N�^�̍\����
struct CHARACTER
{
	int handle = -1;	//�摜�̃n���h��(�Ǘ��ԍ�)
	char path[255];		//�摜�̏ꏊ(�p�X)
	int x;				//x�̈ʒu
	int y;				//y�̈ʒu
	int width;			//�摜�̕�
	int height;			//�摜�̍���
	int speed = 1;		//�ړ����x

	RECT coll;			//��`�����蔻��̈�
	bool isDraw = false;//�摜���`��ł��邩
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

	//�v���C���[�摜�̓ǂݍ���
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
	}

	//�v���C���[�摜�̓ǂݍ���
	strcpyDx(player.path, ".\\Images\\player.png");
	player.handle = LoadGraph(player.path);
	//�S�[���̉摜�̓ǂݍ���
	strcpyDx(goal.path, ".\\Images\\goal.png");
	goal.handle = LoadGraph(goal.path);

	//�摜���ǂݍ��߂Ȃ������Ƃ��̓G���[(-1)������
	if (player.handle == -1) 
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			player.path,			//���b�Z�[�W��
			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		DxLib_End();	//�����I��
		return -1;		//�G���[�I��
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(player.handle, &player.width, &player.height);
	GetGraphSize(goal.handle, &goal.width, &goal.height);

	//0524
	//�����蔻����X�V����

	//�v���C���[�̏�����(�v���C���[����ʒ����ɔz�u)
	player.x = GAME_WIDTH / 2 - player.width / 2;
	player.y = GAME_HEIGHT / 2 - player.height / 2;
	player.speed = 500;
	player.isDraw = true;
	CollUpdate(&player, 100, 50, 30, 10);	//�v���C���[�̓����蔻��̃A�h���X

	//�S�[���̏�����
	goal.x = 400;
	goal.y = 400;
	goal.speed = 500;
	goal.isDraw = true;
	CollUpdate(&goal);	//�S�[���̓����蔻��̃A�h���X

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
		FpsDraw(0,100,true);

		//FPS�l��҂�
		FpsWait();

		//�_�u���o�b�t�@�����O�����摜��`��
		ScreenFlip();
	}

	//�I���Ƃ��̏���
	DeleteGraph(playMovie.handle);	//�v���C���[�摜���������ォ��폜
	DeleteGraph(player.handle);	//�v���C���[�摜���������ォ��폜
	DeleteGraph(goal.handle);	//�v���C���[�摜���������ォ��폜


	// �c�w���C�u�����g�p�̏I����������(return 0�Ń\�t�g���I������)
	DxLib_End();				

	// �\�t�g�̏I��
	return 0;				 
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

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);
	}
	

	return;
}

//�`��
void TitleDraw() 
{
	const char* string = "Title";
	DrawString(0, 50, string, GetColor(0, 0, 0));
	//�����C���[�T�O�͉��ɏ������������Q�[����ʂł͏�̃��C���[�ɕ\�������(photoshop��illustrator�Ƌt)
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
	//�Q�[����ʂɐ؂�ւ��
	if (KeyClick(KEY_INPUT_RETURN))
	{
		//���̃V�[���̏������������ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		/*ChangeScene(GAME_SCENE_END);*/
	}

	//�v���C���[�̑���
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

	//�����蔻����X�V����
	CollUpdate(&player, 0, 0, 30, 10);		//�v���C���[
	CollUpdate(&goal);						//�S�[��

	
	//�����蔻��
	if (CollStay(player, goal))
	{
		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

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


	const char* string = "Play";
	DrawString(0, 50, string, GetColor(0, 0, 0));

	//�v���C���[�̕`��
	if (player.isDraw) 
	{
		//�摜�̕`��
		DrawGraph(player.x, player.y, player.handle, true);

		//�f�o�b�N�̂Ƃ��͓����蔻��̈��`��
		if (GAME_DEBUG) 
		{
		//�l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(0, 0, 0), false);
		}
	}

	//�S�[���̕`��
	if (goal.isDraw) 
	{
		//�摜�̕`��
		DrawGraph(goal.x, goal.y, goal.handle, true);

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
	}

	return;
}

//�`��
void EndDraw()
{
	const char* string = "End";
	DrawString(0, 50, string, GetColor(0, 0, 0));

	return;
}

//�؂�ւ����
void Change() 
{
	DrawString(0, 0, "�ǂݍ���",GetColor(0,0,0));

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

	DrawString(0, 0, "�؂�ւ�", GetColor(0, 0, 0));

	return;
}



/// <summary>
/// �����蔻��̗̈�X�V
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
/// �����蔻��̗̈�X�V(�l���̃I�[�o�[���[�h)
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

//�����蔻��(Enter)
bool CollStay(CHARACTER chara1, CHARACTER chara2)
{
	//�`�悳��Ă��Ȃ��Ȃ�s��Ȃ�
	if (chara1.isDraw && chara2.isDraw)
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