#pragma once
#include <afxsock.h>
class CListenSocket :
	public CSocket
{
public:
	CListenSocket(void);
	~CListenSocket(void);

public:
	CPtrList m_ptrChildSocketList; //����� Ŭ���̾�Ʈ ���ϰ�ü �����ϴ� ����Ʈ ��ü
	CString portStr[100];
	int portindex = 0;
	virtual void OnAccept(int nErrorCode);
	void CloseClientSocket(CSocket* pChild);
	void BroadCast(CString pszBuffer);
	void ReceiveBroadCast(CString pszBuffer);
	void FirstSend(CString pszBuffer);
	void SecondSend(CString pszBuffer);
	//void CharPostionUpdate(CString pszBuffer);
	//void ShootBallXUpdate(CString pszbuffer);
	//void ShootBallYUpdate(CString pszbuffer);
	void ShootStart();
	void ShootStop();
};

