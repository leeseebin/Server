
// ServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include "ListenSocket.h"
#include "ChildSocket.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg 대화 상자



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_strData);

	DDX_Control(pDX, IDC_TEXT, m_Text);
	DDX_Control(pDX, IDC_TEXT2, m_Text2);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CServerDlg::OnBnClickedButtonSend)
//	ON_EN_CHANGE(IDC_EDIT_DATA, &CServerDlg::OnEnChangeEditData)
//ON_EN_CHANGE(IDC_EDIT_DATA, &CServerDlg::OnEnChangeEditData)
//ON_LBN_SELCHANGE(IDC_LIST1, &CServerDlg::OnLbnSelchangeList1)
//ON_EN_CHANGE(IDC_EDIT2, &CServerDlg::OnEnChangeEdit2)
ON_WM_TIMER()
END_MESSAGE_MAP()


// CServerDlg 메시지 처리기

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//Listen 소켓 생성
	m_pListenSocket = new CListenSocket;

	//TCP 소켓을 생성하고 7000번 포트에서 연결대기
	if (m_pListenSocket->Create(7000, SOCK_STREAM))
	{
		if (m_pListenSocket->Listen())
		{

		}
		else
		{
			AfxMessageBox(_T("연결 실패"));
		}
	}
	else
	{
		AfxMessageBox(_T("실패"));
	}
	player1X = 5.0f;
	player2X = 500.0f;
	playerAngle = 0.0f;
	playerPower = 0;
	turn = 1;
	playerballX = -30;
	playerballY = -30;
	player1HP = player2HP = 10;
	PI = 3.1415926535897;
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);


		//CDialogEx::OnPaint();

	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	POSITION pos;
	pos = m_pListenSocket->m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;

	while (pos != NULL)
	{
		pChild = (CChildSocket*)m_pListenSocket->m_ptrChildSocketList.GetNext(pos);

		if (pChild != NULL)
		{
			pChild->ShutDown();
			pChild->Close();
			delete pChild;
		}
	}

	m_pListenSocket->ShutDown();
	m_pListenSocket->Close();
}


void CServerDlg::OnBnClickedButtonSend()//클라이언트들에게 메세지 보내기
{
	CString str;
	GetDlgItemText(IDC_EDIT1, str);

	m_List.AddString(str);
	m_List.SetCurSel(m_List.GetCount() - 1);
	m_strData = _T("");
	
	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->BroadCast(str);


	UpdateData(FALSE);
}


void CServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 1394://포 날락가는중
	{
		playerballX += ballSpeed;
		jumpSpeed -= G;
		playerballY -= jumpSpeed;

		//피격판정 추가해줘야함
		if (turn == 1)//플레이어 2가 맞는상황
		{
			if (playerballX + 30 >= player2X && playerballX <= player2X + 30)//충돌시
			{
				if (playerballY + 30 >= ground && playerballY <= ground + 30)
				{
					/*CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
					pServerSocket->ShootStop();
					turnChnage();
					player2HP--;
					playerballX = -30;
					playerballY = -30;
					shooting = false;*/

					KillTimer(1394);
					SetTimer(4029, 0, NULL);
				}
			}
		}
		else if (turn == 2)//플레이어 1이 맞는상황
		{
			if (playerballX + 30 >= player1X && playerballX <= player1X + 30)
			{
				if (playerballY + 30 >= ground && playerballY <= ground + 30)
				{
					/*CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
					pServerSocket->ShootStop();
					turnChnage();
					player1HP--;
					playerballX = -30;
					playerballY = -30;
					shooting = false;*/

					KillTimer(1394);
					SetTimer(4019, 0, NULL);
				}
			}
		}
		if (playerballY > ground + 30 )//포 바닥충돌했음
		{
			/*CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
			pServerSocket->ShootStop();
			turnChnage();
			playerballX = -30;
			playerballY = -30;
			shooting = false;*/
			KillTimer(1394);
			SetTimer(4014, 0, NULL);
		}
		else//날아가는중
			SetTimer(3001, 0, NULL);

		break;
	}
	case 3001://포 x좌표 보내기
	{
		double sendX = doubleToInt(playerballX);
		CString sendStr;
		sendStr.Format(_T("%f"), sendX);
		sendStr.Insert(0, _T("4011"));

		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->ReceiveBroadCast(sendStr);

		KillTimer(3001);
		SetTimer(3002, 0, NULL);
		break;
	}
	case 3002://포 y좌표 보내기
	{
		double sendY = doubleToInt(playerballY);
		CString sendStr;
		sendStr.Format(_T("%f"), sendY);
		sendStr.Insert(0, _T("4012"));
		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->ReceiveBroadCast(sendStr);
		KillTimer(3002);
		break;
	}
	case 4019://p1피격
	{
		player1HP--;//피깍고
		CString sendStr;
		sendStr.Format(_T("%d"), player1HP);
		sendStr.Insert(0, _T("4019"));
		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->ReceiveBroadCast(sendStr);//깍인피 클라에게 보냄

		turnChnage();//턴바꾸고
		playerballX = -30;
		playerballY = -30;
		shooting = false;//사격중지

		KillTimer(4019);
		break;
	}
	case 4029://p2피격
	{
		player2HP--;
		CString sendStr;
		sendStr.Format(_T("%d"), player2HP);
		sendStr.Insert(0, _T("4029"));
		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->ReceiveBroadCast(sendStr);

		turnChnage();
		playerballX = -30;
		playerballY = -30;
		shooting = false;

		KillTimer(4029);
		break;
	}
	case 4014://빗나가서 사격중지
	{
		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->ShootStop();
		turnChnage();
		playerballX = -30;
		playerballY = -30;
		shooting = false;
		KillTimer(4014);
		break;
	}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CServerDlg::test()
{
	CString angleStr;
	angleStr.Format(_T("%f"), playerAngle);
	m_Text.SetWindowTextW(angleStr);

	CString powerStr;
	powerStr.Format(_T("%f"), playerPower);
	m_Text2.SetWindowTextW(powerStr);
}

double CServerDlg::doubleToInt(double num)//더블형 보내기전에
{
	num = num * 10000;
	return num;
}

double CServerDlg::intToDouble(double num)//변환된 더블형 받을때
{
	num = num / 10000;
	return num;
}

void CServerDlg::player1Shoot()
{
	if (turn == 1)
	{
		double num = getRadian(playerAngle);
		ballSpeed = cos(num) * (playerPower + 5) / 2;
		jumpSpeed = sin(num) * (playerPower + 5) / 2;
		playerballX = player1X + 30;
		playerballY = ground - 1;

		shooting = true;
		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->ShootStart();
		SetTimer(1394, 100, NULL);
	}
}

void CServerDlg::player2Shoot()
{
	if (turn == 2)
	{
		double num = getRadian(180 - playerAngle);
		ballSpeed = cos(num) * (playerPower + 5) / 2;
		jumpSpeed = sin(num) * (playerPower + 5) / 2;
		playerballX = player2X - 15;
		playerballY = ground - 1;

		shooting = true;
		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->ShootStart();
		SetTimer(1394, 100, NULL);
	}
}

void CServerDlg::turnChnage()
{
	if (turn == 1)
		turn = 2;
	else if (turn == 2)
		turn = 1;
}

double CServerDlg::getRadian(int num)
{
	return num * (PI / 180);
}