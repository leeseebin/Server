#include "pch.h"
#include "ListenSocket.h"
#include "ChildSocket.h"
#include "ServerDlg.h"

CListenSocket::CListenSocket(void)
{
}

CListenSocket::~CListenSocket(void)
{
}


//Ŭ���̾�Ʈ�� ���� ��û�� ȣ��Ǵ� �Լ�
void CListenSocket::OnAccept(int nErrorCode)
{
	CChildSocket* pChild = new CChildSocket(); //Ŭ���̾�Ʈ�� ������ ������ ���ϰ�ü ����
	CString str;

	

	BOOL check = Accept(*pChild); //Ŭ���̾�Ʈ�� ������ ����ϰ� �����ͼ��ϰ� ����.

	if (check == FALSE)
	{
		delete pChild;
		AfxMessageBox(_T("���� ��� ����"));
		return;
	}

	//CListenSocket ��ü�� �ּҸ� CChildSocket ��ü�� �˷��ֱ����� �Լ�ȣ��
	pChild->SetListenSocket(this); //CChildSocket Ŭ������ ����ڰ� ������ �Լ�
	m_ptrChildSocketList.AddTail(pChild);

	//Ŭ���̾�Ʈ�� �������� ����Ʈ�� ���
	CServerDlg* pMain = (CServerDlg*)AfxGetMainWnd();

	pMain->m_List.AddString(_T("���� ���� ���"));
	pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);


	CString strIPAddress = _T("");
	UINT uPortNumber = 0;
	GetPeerName(strIPAddress, uPortNumber);

	//portStr[portindex].Format(L"%d", uPortNumber);

	CSocket::OnAccept(nErrorCode);
}

void CListenSocket::CloseClientSocket(CSocket* pChild)
{
	POSITION pos;
	pos = m_ptrChildSocketList.Find(pChild);

	if (pos != NULL)
	{
		pChild->ShutDown();//Ŭ���̾�Ʈ�� ����� �����ͼ��� �ݱ�
		pChild->Close();
	}

	m_ptrChildSocketList.RemoveAt(pos);//����Ʈ������
	delete pChild; //�޸𸮿��� ����
}

void CListenSocket::BroadCast(CString pszBuffer)//ä�ú�����
{
	POSITION pos;
	pos = m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;

	pszBuffer.Insert(0, _T("0001"));

	while (pos != NULL)
	{
		pChild = (CChildSocket*)m_ptrChildSocketList.GetNext(pos);

		if (pChild != NULL)
		{
			pChild->Send(pszBuffer, pszBuffer.GetLength() * 2);
		}
	}
}

void CListenSocket::ReceiveBroadCast(CString pszBuffer)//Ŭ���̾�Ʈ�鿡�� ������ ������
{
	POSITION pos;
	pos = m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;

	while (pos != NULL)
	{
		pChild = (CChildSocket*)m_ptrChildSocketList.GetNext(pos);

		if (pChild != NULL)
		{
			pChild->Send(pszBuffer, pszBuffer.GetLength() * 2);
		}
	}
}
void CListenSocket::FirstSend(CString pszBuffer)//Ŭ���̾�Ʈ�鿡�� ������ ������
{
	POSITION pos;
	pos = m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;

	pChild = (CChildSocket*)m_ptrChildSocketList.GetNext(pos);
	pChild->Send(pszBuffer, pszBuffer.GetLength() * 2);
}
void CListenSocket::SecondSend(CString pszBuffer)//Ŭ���̾�Ʈ�鿡�� ������ ������
{
	POSITION pos;
	pos = m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;

	pChild = (CChildSocket*)m_ptrChildSocketList.GetNext(pos);
	pChild = (CChildSocket*)m_ptrChildSocketList.GetNext(pos);
	pChild->Send(pszBuffer, pszBuffer.GetLength() * 2);
}


void CListenSocket::ShootStart()
{
	POSITION pos;
	pos = m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;
	while (pos != NULL)
	{
		pChild = (CChildSocket*)m_ptrChildSocketList.GetNext(pos);
		if (pChild != NULL)
		{
			CString pszBuffer = (_T("4013"));
			pChild->Send(pszBuffer, pszBuffer.GetLength() * 2);
		}
	}
}
void CListenSocket::ShootStop()
{
	POSITION pos;
	pos = m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;
	while (pos != NULL)
	{
		pChild = (CChildSocket*)m_ptrChildSocketList.GetNext(pos);
		if (pChild != NULL)
		{
			CString pszBuffer = (_T("4014"));
			pChild->Send(pszBuffer, pszBuffer.GetLength() * 2);
		}
	}
}