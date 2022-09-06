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


//클라이언트가 접속 요청시 호출되는 함수
void CListenSocket::OnAccept(int nErrorCode)
{
	CChildSocket* pChild = new CChildSocket(); //클라이언트와 연결할 데이터 소켓객체 생성
	CString str;

	

	BOOL check = Accept(*pChild); //클라이언트의 접속을 허용하고 데이터소켓과 연결.

	if (check == FALSE)
	{
		delete pChild;
		AfxMessageBox(_T("접속 허용 실패"));
		return;
	}

	//CListenSocket 객체의 주소를 CChildSocket 객체에 알려주기위한 함수호출
	pChild->SetListenSocket(this); //CChildSocket 클래스에 사용자가 정의한 함수
	m_ptrChildSocketList.AddTail(pChild);

	//클라이언트가 접속함을 리스트에 출력
	CServerDlg* pMain = (CServerDlg*)AfxGetMainWnd();

	pMain->m_List.AddString(_T("서버 접속 허용"));
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
		pChild->ShutDown();//클라이언트와 연결된 데이터소켓 닫기
		pChild->Close();
	}

	m_ptrChildSocketList.RemoveAt(pos);//리스트제거후
	delete pChild; //메모리에서 해제
}

void CListenSocket::BroadCast(CString pszBuffer)//채팅보내기
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

void CListenSocket::ReceiveBroadCast(CString pszBuffer)//클라이언트들에게 데이터 보내기
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
void CListenSocket::FirstSend(CString pszBuffer)//클라이언트들에게 데이터 보내기
{
	POSITION pos;
	pos = m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;

	pChild = (CChildSocket*)m_ptrChildSocketList.GetNext(pos);
	pChild->Send(pszBuffer, pszBuffer.GetLength() * 2);
}
void CListenSocket::SecondSend(CString pszBuffer)//클라이언트들에게 데이터 보내기
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