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
bool KeyDown(int key_input);
//����̃L�[���グ�Ă��邩
bool KeyUp(int key_input);
//����̃L�[���N���b�N������
bool KeyClick(int key_input);
//����̃L�[���w��̃~���b�����������Ă��邩
bool KeyDownKeep(int key_input, int millSec);