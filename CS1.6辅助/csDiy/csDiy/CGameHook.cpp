#include "stdafx.h"
#include "CGameHook.h"


HMODULE CGameHook::hMpdll = 0;															//�����ģ���ַ
DWORD CGameHook::dw_Player = 0;															//������ҵ�ַ
DWORD CGameHook::dw_Player7c = 0;														//������ҵ�ַ
//******************************��ɱHOOK�������******************************
BYTE CGameHook::by_SecKill_HookCode[SECKILL_PATCHLENGTH] = { 0 };						//����JMP XXXXXX
BYTE CGameHook::by_SecKill_OriginalCode[SECKILL_PATCHLENGTH] = { 0 };					//����JMP XXXXXX
DWORD CGameHook::dw_SecKill_HookFunc = 0;												//����ҪHOOK�ĺ����׵�ַ
DWORD CGameHook::dw_SecKill_Attacker = 0;												//���湥���ߵ�ַ
BOOL CGameHook::Is_SecKill_Hooded = 0;													//�����Ƿ��Ѿ�HOOK��
DWORD CGameHook::dw_SecKill_RetAddr = 0;												//���淵�ص�ַ
//******************************��������HOOK�������******************************
BYTE CGameHook::by_InfGrenade_OriginalCode[INFGRENADE_PATCHLENGTH] = {0};				//����ԭ���ĵ�ַ,��UNHOOK
BYTE CGameHook::by_InfGrenade_HookCode[INFGRENADE_PATCHLENGTH] = {0};					//����JMP XXXXXX
DWORD CGameHook::dw_InfGrenade_HookAddr = 0;											//����ҪHOOK�ĵ�ַ
DWORD CGameHook::dw_InfGrenade_Attacker = 0;											//���������׵��˵Ļ�ַ(+208)
BOOL CGameHook::Is_InfGrenade_Hooded = 0;												//�����Ƿ��Ѿ�HOOK��
DWORD CGameHook::dw_InfGrenade_RetAddr = 0;												//���淵�ص�ַ
//******************************���������������******************************
DWORD CGameHook::dw_Draw_PeopleAddr7c = 0;												//����׼��ָ�������ַ��ȫ�ֱ���
BYTE CGameHook::by_Draw_HookCode[DRAW_PATCHLENGTH] = {0};								//����JMP XXXXXX
DWORD CGameHook::dw_Draw_HookAddr = 0;													//����ҪHOOK�ĵ�ַ
DWORD CGameHook::dw_Draw_RetAddr = 0;													//���淵�ص�ַ
BOOL CGameHook::Is_Draw_Hooded = 0;														//�����Ƿ�HOOK��
//*******************************�ر������������*******************************
BYTE CGameHook::by_Bomb_OriginalCode[BOMB_PATCHLENGTH];									//����ԭ���ĵ�ַ,��UNHOOK
BYTE CGameHook::by_Bomb_HookCode[BOMB_PATCHLENGTH];										//����JMP XXXXXX
DWORD CGameHook::dw_Bomb_HookAddr;														//����ҪHOOK�ĵ�ַ
BOOL CGameHook::Is_Bomb_Hooded;															//�����Ƿ��Ѿ�HOOK��
DWORD CGameHook::dw_Bomb_RetAddr;														//���淵�ص�ַ
DWORD CGameHook::dw_Bomb_Pos_x;															//�����Զ������ױ�ը������x
DWORD CGameHook::dw_Bomb_Pos_y;															//�����Զ������ױ�ը������y
DWORD CGameHook::dw_Bomb_Pos_z;															//�����Զ������ױ�ը������z

void __declspec(naked) MySecKill()
{
	/*

	|EIP
	|Arg1 = 0C29DBFC		//�����߻�ַ
	|Arg2 = 0C29DBFC		//�����߻�ַ
	|Arg3 = 41500000		//����Ѫ��
	|Arg4 = 00001002		//δ֪


	*/
	__asm {
		pushad;
		pushfd;

		mov eax, dword ptr ss : [esp + 0X28];				//���湥���ߵ�ַ
		mov dword ptr ds : [CGameHook::dw_SecKill_Attacker], eax;
		mov ebx, dword ptr ds : [CGameHook::dw_Player];

		cmp eax, ebx;										//�ж��ǲ����Լ������ı���
		jnz label;											//������ǣ�����			
		mov ebx, ALLBLOOD;
		mov dword ptr ss : [esp + 0X28 + 0X8], ebx;			//������������Լ��Ļ����ͽ�Ѫ������Ϊ100
	label:
		popfd;
		popad;

		sub esp, 0x3C;
		push ebx;
		push esi;

		jmp CGameHook::dw_SecKill_RetAddr;
	}
}


void __declspec(naked) MyInfGrenade()
{
	__asm {
		pushad;
		pushfd;

		mov dword ptr ds : [CGameHook::dw_InfGrenade_Attacker], ebx;
		
		mov eax, dword ptr ds : [CGameHook::dw_Player];	//����+208��������ַ
		add eax, 0X208;
		mov eax, dword ptr ds:[eax];	

		cmp eax, ebx;

		jz label;

		popfd;
		popad;

		dec ecx;
		mov dword ptr ds : [eax], ecx;
		mov ecx, dword ptr ds : [esi + 0xC4];
		jmp CGameHook::dw_InfGrenade_RetAddr;
label:
		popfd;
		popad;
		
		nop;
		mov dword ptr ds : [eax], ecx;
		mov ecx, dword ptr ds : [esi + 0xC4];

		jmp CGameHook::dw_InfGrenade_RetAddr;
	}
}

void __declspec(naked) MyDraw()
{
	__asm {
		pushad;			
		pushfd;

		mov eax, dword ptr ss : [esp + 0X24 - 0XD8];
		cmp eax, dword ptr ds : [CGameHook::dw_Player7c];
		jz label;

		mov dword ptr ds : [CGameHook::dw_Draw_PeopleAddr7c], eax;	//����ָ������Ļ�ַ

	label:
		popfd;
		popad;
		
		push eax;
		push 0x0;
		push ecx;
		push 0x1;
	
		jmp CGameHook::dw_Draw_RetAddr;
	}
}

void __declspec(naked) MyBomb()
{
	__asm {
		pushad;
		pushfd;

		lea eax, dword ptr ds : [eax + 0X8];
		mov ebx, dword ptr ds : [CGameHook::dw_Bomb_Pos_x];
		mov dword ptr ds:[eax], ebx
		mov ebx, dword ptr ds : [CGameHook::dw_Bomb_Pos_y];
		mov dword ptr ds : [eax + 0X4], ebx
		mov ebx, dword ptr ds : [CGameHook::dw_Bomb_Pos_z];
		mov dword ptr ds : [eax + 0X8], ebx

		popfd;
		popad;

		fld dword ptr ds : [eax + 0x8];
		fstp dword ptr ss : [esp + 0x4];
		fld dword ptr ds : [eax + 0xC];
		
		jmp CGameHook::dw_Bomb_RetAddr;
	}
}



VOID CGameHook::GetInfGrenadeInfo()
{
	dw_InfGrenade_HookAddr = (DWORD)hMpdll + DEC_NUMOFFGRENADE_OFFSET;
	memset(&by_InfGrenade_HookCode[0], 0X90, INFGRENADE_PATCHLENGTH);
	by_InfGrenade_HookCode[0] = 0XE9;
	*(DWORD *)&by_InfGrenade_HookCode[1] = (DWORD)MyInfGrenade - dw_InfGrenade_HookAddr - 5;
	dw_InfGrenade_RetAddr = dw_InfGrenade_HookAddr + INFGRENADE_PATCHLENGTH;
	if (!Is_InfGrenade_Hooded)
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)dw_InfGrenade_HookAddr, by_InfGrenade_OriginalCode, INFGRENADE_PATCHLENGTH, NULL);
}

BOOL CGameHook::InfGrenadeHook(BOOL Is_Hook)
{
	BOOL Ret = FALSE;
	DWORD dwOldProtect = 0;
	if (Is_Hook)		//�����HOOK
	{
		if (!Is_InfGrenade_Hooded)	//֮ǰû�б�HOOK
		{
			VirtualProtect((LPVOID)dw_InfGrenade_HookAddr, INFGRENADE_PATCHLENGTH, PAGE_EXECUTE_READWRITE, &dwOldProtect);
			WriteProcessMemory(GetCurrentProcess(), (LPVOID)dw_InfGrenade_HookAddr, by_InfGrenade_HookCode, INFGRENADE_PATCHLENGTH, NULL);
			VirtualProtect((LPVOID)dw_InfGrenade_HookAddr, INFGRENADE_PATCHLENGTH, dwOldProtect, NULL);
			Ret = TRUE;
			Is_Hook = TRUE;
		}
	}
	else
	{
		if (!Is_InfGrenade_Hooded)	//֮ǰû�б�HOOK
		{
			VirtualProtect((LPVOID)dw_InfGrenade_HookAddr, INFGRENADE_PATCHLENGTH, PAGE_EXECUTE_READWRITE, &dwOldProtect);
			WriteProcessMemory(GetCurrentProcess(), (LPVOID)dw_InfGrenade_HookAddr, by_InfGrenade_OriginalCode, INFGRENADE_PATCHLENGTH, NULL);
			VirtualProtect((LPVOID)dw_InfGrenade_HookAddr, INFGRENADE_PATCHLENGTH, dwOldProtect, NULL);
			Ret = TRUE;
			Is_Hook = FALSE;
		}
	}

	return Ret;
}

VOID CGameHook::GetDrawInfo()
{
	dw_Draw_HookAddr = (DWORD)hMpdll + DRAW_OFFSET;
	memset(&by_Draw_HookCode[0], 0X90, DRAW_PATCHLENGTH);
	by_Draw_HookCode[0] = 0XE9;
	*(DWORD *)&by_Draw_HookCode[1] = (DWORD)MyDraw - dw_Draw_HookAddr - 5;
	dw_Draw_RetAddr = dw_Draw_HookAddr + DRAW_PATCHLENGTH;
}

BOOL CGameHook::DrawHook()
{
	BOOL Ret = FALSE;
	DWORD dwOldProtect = 0;

	if (!Is_Draw_Hooded)
	{
		VirtualProtect((LPVOID)dw_Draw_HookAddr, DRAW_PATCHLENGTH, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)dw_Draw_HookAddr, by_Draw_HookCode, DRAW_PATCHLENGTH, NULL);
		VirtualProtect((LPVOID)dw_Draw_HookAddr, DRAW_PATCHLENGTH, dwOldProtect, NULL);
		Is_Draw_Hooded = TRUE;
		Ret = TRUE;
	}
	return Ret;
}

VOID CGameHook::GetBombInfo()
{
	dw_Bomb_HookAddr = (DWORD)hMpdll + BOMB_OFFSET;
	memset(&by_Bomb_HookCode[0], 0X90, BOMB_PATCHLENGTH);
	by_Bomb_HookCode[0] = 0XE9;
	*(DWORD *)&by_Bomb_HookCode[1] = (DWORD)MyBomb - dw_Bomb_HookAddr - 5;
	dw_Bomb_RetAddr = dw_Bomb_HookAddr + BOMB_PATCHLENGTH;
	if (!Is_Bomb_Hooded)
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)dw_Bomb_HookAddr, by_Bomb_OriginalCode, BOMB_PATCHLENGTH, NULL);
}

BOOL CGameHook::BombHook(BOOL Is_Hook)
{
	BOOL Ret = FALSE;
	DWORD dwOldProtect = 0;
	if (Is_Hook)		//�����HOOK
	{
		if (!Is_Bomb_Hooded)	//֮ǰû�б�HOOK
		{
			VirtualProtect((LPVOID)dw_Bomb_HookAddr, BOMB_PATCHLENGTH, PAGE_EXECUTE_READWRITE, &dwOldProtect);
			WriteProcessMemory(GetCurrentProcess(), (LPVOID)dw_Bomb_HookAddr, by_Bomb_HookCode, BOMB_PATCHLENGTH, NULL);
			VirtualProtect((LPVOID)dw_Bomb_HookAddr, BOMB_PATCHLENGTH, dwOldProtect, NULL);
			Ret = TRUE;
			Is_Hook = TRUE;
		}
	}
	else
	{
		if (!Is_Bomb_Hooded)	//֮ǰû�б�HOOK
		{
			VirtualProtect((LPVOID)dw_Bomb_HookAddr, BOMB_PATCHLENGTH, PAGE_EXECUTE_READWRITE, &dwOldProtect);
			WriteProcessMemory(GetCurrentProcess(), (LPVOID)dw_Bomb_HookAddr, by_Bomb_OriginalCode, BOMB_PATCHLENGTH, NULL);
			VirtualProtect((LPVOID)dw_Bomb_HookAddr, BOMB_PATCHLENGTH, dwOldProtect, NULL);
			Ret = TRUE;
			Is_Hook = FALSE;
		}
	}

	return Ret;
}

VOID CGameHook::GetMpdllAddr()
{
	hMpdll = GetModuleHandle("mp.dll");
}

CGameHook::CGameHook()
{
	
}

BOOL CGameHook::SecKillHook(BOOL Is_Hook)
{
	BOOL Ret = FALSE;
	DWORD dwOldProtect = 0;
	if (Is_Hook)		//�����HOOK
	{
		if (!Is_SecKill_Hooded)	//֮ǰû�б�HOOK
		{
			VirtualProtect((LPVOID)dw_SecKill_HookFunc, SECKILL_PATCHLENGTH, PAGE_EXECUTE_READWRITE, &dwOldProtect);
			WriteProcessMemory(GetCurrentProcess(), (LPVOID)dw_SecKill_HookFunc, by_SecKill_HookCode, SECKILL_PATCHLENGTH, NULL);
			VirtualProtect((LPVOID)dw_SecKill_HookFunc, SECKILL_PATCHLENGTH, dwOldProtect, NULL);
			Ret = TRUE;
			Is_Hook = TRUE;
		}
	}
	else
	{
		if (!Is_SecKill_Hooded)	//֮ǰû�б�HOOK
		{
			VirtualProtect((LPVOID)dw_SecKill_HookFunc, SECKILL_PATCHLENGTH, PAGE_EXECUTE_READWRITE, &dwOldProtect);
			WriteProcessMemory(GetCurrentProcess(), (LPVOID)dw_SecKill_HookFunc, by_SecKill_OriginalCode, SECKILL_PATCHLENGTH, NULL);
			VirtualProtect((LPVOID)dw_SecKill_HookFunc, SECKILL_PATCHLENGTH, dwOldProtect, NULL);
			Ret = TRUE;
			Is_Hook = FALSE;
		}
	}

	return Ret;
}

VOID CGameHook::GetSecKillInfo()
{
	dw_SecKill_HookFunc = (DWORD)hMpdll + DEC_BLOODFUNC_OFFSET;
	memset(&by_SecKill_HookCode[0], 0X90, SECKILL_PATCHLENGTH);
	by_SecKill_HookCode[0] = 0XE9;
	*(DWORD *)&by_SecKill_HookCode[1] = (DWORD)MySecKill - dw_SecKill_HookFunc - SECKILL_PATCHLENGTH;
	dw_SecKill_RetAddr = dw_SecKill_HookFunc + SECKILL_PATCHLENGTH;
	if (!Is_SecKill_Hooded)
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)dw_SecKill_HookFunc, by_SecKill_OriginalCode, SECKILL_PATCHLENGTH, NULL);
}
