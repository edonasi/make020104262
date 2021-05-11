#pragma once

//�擾����L�[�̎��
#define KEY_KIND_MAX 256

//�L�[�{�[�h�\����
struct KEYBOARD
{
	//���͏�Ԃ��Ǘ�
	char tmpKeyState[KEY_KIND_MAX];
	//���ׂẴL�[�̓��͎��Ԃ��Ǘ�
	int allKeyState[KEY_KIND_MAX];
	//�ȑO�̂��ׂẴL�[�̓��͎��Ԃ��Ǘ�
	int oldKeyState[KEY_KIND_MAX];
};

//�O���̃O���[�o���ϐ�(�錾���Ă���̂ł͂Ȃ�)
extern KEYBOARD keyboard;

//----- �v���g�^�C�v�錾 ------
//���ׂẴL�[�̓��͏�Ԃ��擾����
void AllKeyUpdate(void);
//����̃L�[�������Ă��邩
bool KeyDown(int keyInput);
//����̃L�[���������u��
bool Key1Down(int keyInput);
//����̃L�[���グ�Ă��邩
bool KeyUp(int keyInput);
bool KeyUpping(int keyInput);
//����̃L�[���N���b�N������
bool KeyClick(int keyInput);
//����̃L�[���w��̃~���b�����������Ă��邩
bool KeyDownKeep(int keyInput, int millSec);