#pragma once
#include "CGameHook.h"
#include <vector>
#include <algorithm>
#include <stdlib.h>

using namespace std;
typedef struct _stPlayerPos
{
	float x;
	float y; 
	float z;
}stPlayerPos, * pstPlayerPos;

class CGame
{
public:
	HMODULE hModule;										//���浱ǰexeģ���ַ
	DWORD * pPlayer;										//��ǰ������Ϣ��ַ
	DWORD * pPlayer7c;										//��ַ+7C
	DWORD * pWeapon;										//����������Ϣ��ַ
	DWORD dwInGame[4];										//�ж��Ƿ�����Ϸ�е��ĸ�ƫ����
	stPlayerPos NowPos;										//���浱ǰ��ַ
	stPlayerPos MarkPos1;									//�����ǵ�ַ1
	stPlayerPos MarkPos2;									//�����ǵ�ַ2
	float fNowBlood_suoxue = 0;								//����Ҫ������Ѫ��ֵ
	CGameHook * pCGameHook;									//һ��ȫ�ֱ���ָ��HOOK��Ķ���
	DWORD dwEnemyCode;										//���������Ӫ�ı�� 1�ֲ����� 2���־�Ӣ
	
	vector<DWORD> pAllPeople7c;								//������������Ļ�ַ +7C
	vector<DWORD> pEnemy7c;									//�������е��˵Ļ�ַ +7C
	vector<DWORD> pEnemy4;									//�������е��˵���ϸ��Ϣ��ַ +4
	vector<stPlayerPos> stRandomPos;						//�����������
	BOOL IsUseDiBaoTianXing = FALSE;						//�����Ƿ�����ʹ�õر�����
	BOOL IsFirstDiBaoTianXing = FALSE;						//�Ƿ��ǵ�һ�εر�����
public:
	CGame();
	~CGame();
	DWORD * GetPlayerAddress();								//��ȡ�����Ϣ��ַ
	DWORD * GetWeaponAddress();								//��ȡ������Ϣ��ַ
	BOOL IsInGame();										//�ж����������滹����Ϸ��
	float GetMyBlood();										//��ȡ��ǰѪ��
	float GetMyHujia();										//��ȡ����ֵ
	int GetNumOfBullet();									//��ȡ��ǰ�ӵ���
	void GetLocalPos(pstPlayerPos pNowPos);					//��ȡ��ǰ����
	DWORD GetMyMoney();										//��ȡ��ǰ��Ǯ��
	void SuoXue(float nowBlood, bool IsFirst);				//ʵ����Ѫ����,ֻ�е�һ�ε����Ѫ��ʱ��������Ѫ�������Ա�������
	void SuperMan();										//ʵ���޵�״̬
	void SuManHujia();										//��������
	void SuoDingMoney();									//�������
	void WuXinBulletNum();									//�����ӵ�
	void NoHouzuoli();										//�޺�����
	bool HookReduceBlood(bool IsHook);						//Hook��Ѫ������ʵ����ɱ
	bool HookInfGrenade(bool IsHook);						//��������
	bool Changingview(bool IsFPV);							//�л��ӽ�ʵ�ִ�ǽ			
	bool BuyAnywhere();										//��ع���
	DWORD GetMyTeam();										//��ȡ��ǰ��Ӫ 1�ֲ����� 2���־�Ӣ
	DWORD GetPointToWho();									//ָ����˭ ���� ���� ǽ
	BOOL CopyPos(pstPlayerPos p1, pstPlayerPos p2);			//��������		������ָ������ĵ�ַ��ֱ���޸�
	BOOL GetAllPeople7c();									//ѭ��������ȡ�����ַ��ֱ����������˳�
	DWORD FilteEnemy();										//�����ҵ������е��˵Ļ�ַ��ɸѡ�����˻��Ƕ���,	����ֵ�ǵ��˵ĸ���
	BOOL EmeptyPeopleAddr();								//��ջ�ȡ�������ַ
	VOID GenerateRandomPos();								//�����������
	VOID DiBaoTianXingCopyPos();							
	BOOL DiBaoTianXing666();								//�ر����ǹ���
	
	BOOL IsAllEnemyKilled();								//�Ƿ����е�������

};

