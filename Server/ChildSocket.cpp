#include "pch.h"
#include "ChildSocket.h"
#include "ListenSocket.h"
#include "ServerDlg.h"

CChildSocket::CChildSocket(void)
{
}

CChildSocket::~CChildSocket(void)
{
}

//연결된 클라이언트의 소켓주소를 m_pListenSocket에 저장
void CChildSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}

//클라이언트 소켓이 닫히게 되면 호출되는 함수
void CChildSocket::OnClose(int nErrorCode)
{
	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);

	CSocket::OnClose(nErrorCode);
}

//클라이언트가 서버로부터 데이터를 보내게되면 자동으로 호출되는 함수
void CChildSocket::OnReceive(int nErrorCode)
{
	//0001 채팅, 0002 클라이언트가 서버에게 어느정도 이동할지, 0003 서버가 클라이언트들에게 player1의 좌표갱신


	CString temp = _T("");
	CString strIPAddress = _T("");
	char szBuffer[1024];
	::ZeroMemory(szBuffer, 1024);

	UINT uPortNumber = 0;

	CString receiveString;

	//연결된 클라이언트의 IP주소와 포트 번호를 알아낸다.
	GetPeerName(strIPAddress, uPortNumber);
	

	//실제로 데이터 수신
	int len = 0;
	if ((len = Receive(szBuffer, 1024)) > 0)
	{
		//데이터를 수신하였다면 받은 메시지를 리스트에 출력
		CServerDlg* pMain = (CServerDlg*)AfxGetMainWnd();

		CString portStr;
		portStr.Format(L"%d", uPortNumber);//플레이어 구분하는 포트번호

		if (memcmp(szBuffer, L"0001", 8) == 0)//채팅
		{
			temp.Format(_T("[%s] : %s"), strIPAddress, szBuffer);

			pMain->m_List.AddString(temp);
			pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);

			CString pszBuffer;
			CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
			pszBuffer.Format(_T("%s"), szBuffer);

			if (portStr == pServerSocket->portStr[0])
				pszBuffer.Insert(4, _T("Player1 : "));
			else
				pszBuffer.Insert(4, _T("Player2 : "));
			//연결된 모든 클라이언트에 해당 메시지 에코
			pServerSocket->ReceiveBroadCast(pszBuffer);

			
		}
		else if (memcmp(szBuffer, L"0002", 8) == 0)//이동
		{
			CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
			/*temp.Format(_T("[%s] : %f"), strIPAddress, pMain->player1X);
			pMain->m_List.AddString(temp);
			pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);*/
			if (portStr == pServerSocket->portStr[0] && pMain->turn == 1)
			{
				char receiveStr[1024];
				memcpy(receiveStr, szBuffer + 8, 1016);
				double moveReceive = _ttoi(LPCTSTR(receiveStr));
				pMain->player1X = pMain->player1X + moveReceive;


				CString pszBuffer;
				pszBuffer.Format(_T("%f"), pMain->player1X);
				pszBuffer.Insert(0, _T("1003"));//player1의 좌표 재설정

				CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
				pServerSocket->ReceiveBroadCast(pszBuffer);
			}
			else if (portStr == pServerSocket->portStr[1] && pMain->turn == 2)
			{
				char receiveStr[1024];
				memcpy(receiveStr, szBuffer + 8, 1016);
				double moveReceive = _ttoi(LPCTSTR(receiveStr));
				pMain->player2X = pMain->player2X + moveReceive;

				CString pszBuffer;
				pszBuffer.Format(_T("%f"), pMain->player2X);
				pszBuffer.Insert(0, _T("2003"));//player2의 좌표 재설정

				CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
				pServerSocket->ReceiveBroadCast(pszBuffer);
			}
		}
		
		else if (memcmp(szBuffer, L"9999", 8) == 0)//포트받기. 첫접속시에 해줌
		{
			CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
			pServerSocket->portStr[pServerSocket->portindex].Format(L"%d", uPortNumber);
			pServerSocket->portindex++;
			if (portStr == pServerSocket->portStr[0])//첫접속일시
			{
				CString pszBuffer;
				pszBuffer.Format(_T("9991"));//player1설정
				pServerSocket->FirstSend(pszBuffer);
			}
			else if(portStr == pServerSocket->portStr[1])
			{
				CString pszBuffer;
				pszBuffer.Format(_T("9992"));//player1설정
				pServerSocket->SecondSend(pszBuffer);
			}
		}

		else if (memcmp(szBuffer, L"4001", 8) == 0)//발사명령. 각도를 받고
		{
			char receiveStr[1024];
			memcpy(receiveStr, szBuffer + 8, 1016);
			pMain->playerAngle = _ttoi(LPCTSTR(receiveStr));
			pMain->playerAngle = pMain->intToDouble(pMain->playerAngle);
			//pMain->playerAngle = (atof(receiveStr));//첫자리만 복사해서 안됨

			temp.Format(_T("[%s] : %f"), strIPAddress, pMain->playerAngle);
			pMain->m_List.AddString(temp);
			pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);

		}
		else if (memcmp(szBuffer, L"4002", 8) == 0)//발사명령2. 파워를 받는다
		{
			char receiveStr[1024];
			memcpy(receiveStr, szBuffer + 8, 1016);
			pMain->playerPower = _ttoi(LPCTSTR(receiveStr));
			pMain->playerPower = pMain->intToDouble(pMain->playerPower);
			CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
			if (portStr == pServerSocket->portStr[0])
			{
				pMain->player1Shoot();
			}
			else if (portStr == pServerSocket->portStr[1])
			{
				pMain->player2Shoot();
			}
		}
	}
	CSocket::OnReceive(nErrorCode);
}
