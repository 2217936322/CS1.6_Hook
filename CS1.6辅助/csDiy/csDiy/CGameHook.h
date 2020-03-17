#pragma once
#include <windows.h>
#include <stdio.h>

//*********************************************��ɱHOOK*********************************************
#define SECKILL_PATCHLENGTH 5						//��ɱHOOK�򲹶��ĳ���
#define DEC_BLOODFUNC_OFFSET 0x66CC0				//��Ѫ�����ڲ����ַ��ƫ��
#define ALLBLOOD	0X42C80000						//100Ѫ��
//0AEC6CC0    83EC 3C         sub esp,0x3C
//0AEC6CC3    53              push ebx
//0AEC6CC4    56              push esi
//*********************************************��������HOOK*********************************************
#define INFGRENADE_PATCHLENGTH 9					//��ɱHOOK�򲹶��ĳ���
#define DEC_NUMOFFGRENADE_OFFSET	0X753A					//�����������ĵ�ַƫ��
//1F56753A    49                                  dec ecx; ��������
//1F56753B    8908                                mov dword ptr ds : [eax], ecx
//1F56753D    8B8E C4000000                       mov ecx, dword ptr ds : [esi + 0xC4]
//*********************************************��������HOOK*********************************************
#define DRAW_PATCHLENGTH	6
#define DRAW_OFFSET 0X9C0C9
//0AD2C0C9    50              push eax
//0AD2C0CA    6A 00           push 0x0
//0AD2C0CC    51              push ecx
//0AD2C0CD    6A 01           push 0x1
//*********************************************�ر��������ױ�ըHOOK******************************************
#define BOMB_PATCHLENGTH	7
#define BOMB_OFFSET 0X77C79
//1FAF7C79    D940 08         fld dword ptr ds : [eax + 0x8]
//1FAF7C7C    D95C24 04       fstp dword ptr ss : [esp + 0x4]
//1FAF7C80    D940 0C         fld dword ptr ds : [eax + 0xC]













void  MySecKill();
void MyInfGrenade();
void MyDraw();		
void MyBomb();

class CGameHook
{
public:
	//******************************MP.DLLģ���ַ******************************
	static HMODULE hMpdll;	
	static DWORD dw_Player;													//������ҵ�ַ
	static DWORD dw_Player7c;
	//******************************��ɱHOOK�������******************************
	static BYTE by_SecKill_OriginalCode[SECKILL_PATCHLENGTH];				//����ԭ���ĵ�ַ,��UNHOOK
	static BYTE by_SecKill_HookCode[SECKILL_PATCHLENGTH];					//����JMP XXXXXX
	static DWORD dw_SecKill_HookFunc;										//����ҪHOOK�ĵ�ַ
	static DWORD dw_SecKill_Attacker;										//���湥���ߵ�ַ
	static BOOL Is_SecKill_Hooded;											//�����Ƿ��Ѿ�HOOK��
	static DWORD dw_SecKill_RetAddr;										//���淵�ص�ַ
	BOOL SecKillHook(BOOL Is_Hook);
	VOID GetSecKillInfo();
	//******************************��������HOOK�������******************************
	static BYTE by_InfGrenade_OriginalCode[INFGRENADE_PATCHLENGTH];			//����ԭ���ĵ�ַ,��UNHOOK
	static BYTE by_InfGrenade_HookCode[INFGRENADE_PATCHLENGTH];				//����JMP XXXXXX
	static DWORD dw_InfGrenade_HookAddr;									//����ҪHOOK�ĵ�ַ
	static DWORD dw_InfGrenade_Attacker;									//���������׵��˵Ļ�ַ(+208)
	static BOOL Is_InfGrenade_Hooded;										//�����Ƿ��Ѿ�HOOK��
	static DWORD dw_InfGrenade_RetAddr;										//���淵�ص�ַ
	VOID GetInfGrenadeInfo();	
	BOOL InfGrenadeHook(BOOL Is_Hook);
	//******************************��������HOOK�������******************************
	static DWORD dw_Draw_PeopleAddr7c;										//����׼��ָ�������ַ��ȫ�ֱ���
	static BYTE by_Draw_HookCode[DRAW_PATCHLENGTH];							//����JMP XXXXXX
	static DWORD dw_Draw_HookAddr;											//����ҪHOOK�ĵ�ַ
	static DWORD dw_Draw_RetAddr;											//���淵�ص�ַ
	static BOOL Is_Draw_Hooded;												//�����Ƿ��Ѿ�HOOK��
	VOID GetDrawInfo();														//��ȡ�����Ϣ
	BOOL DrawHook();
	//******************************�ر�����HOOK���ױ�ը�ص�******************************
	static BYTE by_Bomb_OriginalCode[BOMB_PATCHLENGTH];						//����ԭ���ĵ�ַ,��UNHOOK
	static BYTE by_Bomb_HookCode[BOMB_PATCHLENGTH];							//����JMP XXXXXX
	static DWORD dw_Bomb_HookAddr;											//����ҪHOOK�ĵ�ַ
	static BOOL Is_Bomb_Hooded;												//�����Ƿ��Ѿ�HOOK��
	static DWORD dw_Bomb_RetAddr;											//���淵�ص�ַ
	static DWORD dw_Bomb_Pos_x;												//�����Զ������ױ�ը������x
	static DWORD dw_Bomb_Pos_y;												//�����Զ������ױ�ը������y
	static DWORD dw_Bomb_Pos_z;												//�����Զ������ױ�ը������z
	VOID GetBombInfo();														//��ȡ�����Ϣ
	BOOL BombHook(BOOL Is_Hook);

	//�õ�ģ���ַ
	VOID GetMpdllAddr(); 

	CGameHook();
};
