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

//����� Ŭ���̾�Ʈ�� �����ּҸ� m_pListenSocket�� ����
void CChildSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}

//Ŭ���̾�Ʈ ������ ������ �Ǹ� ȣ��Ǵ� �Լ�
void CChildSocket::OnClose(int nErrorCode)
{
	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);

	CSocket::OnClose(nErrorCode);
}

//Ŭ���̾�Ʈ�� �����κ��� �����͸� �����ԵǸ� �ڵ����� ȣ��Ǵ� �Լ�
void CChildSocket::OnReceive(int nErrorCode)
{
	//0001 ä��, 0002 Ŭ���̾�Ʈ�� �������� ������� �̵�����, 0003 ������ Ŭ���̾�Ʈ�鿡�� player1�� ��ǥ����


	CString temp = _T("");
	CString strIPAddress = _T("");
	char szBuffer[1024];
	::ZeroMemory(szBuffer, 1024);

	UINT uPortNumber = 0;

	CString receiveString;

	//����� Ŭ���̾�Ʈ�� IP�ּҿ� ��Ʈ ��ȣ�� �˾Ƴ���.
	GetPeerName(strIPAddress, uPortNumber);
	

	//������ ������ ����
	int len = 0;
	if ((len = Receive(szBuffer, 1024)) > 0)
	{
		//�����͸� �����Ͽ��ٸ� ���� �޽����� ����Ʈ�� ���
		CServerDlg* pMain = (CServerDlg*)AfxGetMainWnd();

		CString portStr;
		portStr.Format(L"%d", uPortNumber);//�÷��̾� �����ϴ� ��Ʈ��ȣ

		if (memcmp(szBuffer, L"0001", 8) == 0)//ä��
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
			//����� ��� Ŭ���̾�Ʈ�� �ش� �޽��� ����
			pServerSocket->ReceiveBroadCast(pszBuffer);

			
		}
		else if (memcmp(szBuffer, L"0002", 8) == 0)//�̵�
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
				pszBuffer.Insert(0, _T("1003"));//player1�� ��ǥ �缳��

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
				pszBuffer.Insert(0, _T("2003"));//player2�� ��ǥ �缳��

				CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
				pServerSocket->ReceiveBroadCast(pszBuffer);
			}
		}
		
		else if (memcmp(szBuffer, L"9999", 8) == 0)//��Ʈ�ޱ�. ù���ӽÿ� ����
		{
			CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
			pServerSocket->portStr[pServerSocket->portindex].Format(L"%d", uPortNumber);
			pServerSocket->portindex++;
			if (portStr == pServerSocket->portStr[0])//ù�����Ͻ�
			{
				CString pszBuffer;
				pszBuffer.Format(_T("9991"));//player1����
				pServerSocket->FirstSend(pszBuffer);
			}
			else if(portStr == pServerSocket->portStr[1])
			{
				CString pszBuffer;
				pszBuffer.Format(_T("9992"));//player1����
				pServerSocket->SecondSend(pszBuffer);
			}
		}

		else if (memcmp(szBuffer, L"4001", 8) == 0)//�߻���. ������ �ް�
		{
			char receiveStr[1024];
			memcpy(receiveStr, szBuffer + 8, 1016);
			pMain->playerAngle = _ttoi(LPCTSTR(receiveStr));
			pMain->playerAngle = pMain->intToDouble(pMain->playerAngle);
			//pMain->playerAngle = (atof(receiveStr));//ù�ڸ��� �����ؼ� �ȵ�

			temp.Format(_T("[%s] : %f"), strIPAddress, pMain->playerAngle);
			pMain->m_List.AddString(temp);
			pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);

		}
		else if (memcmp(szBuffer, L"4002", 8) == 0)//�߻���2. �Ŀ��� �޴´�
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
