
// ServerDlg.h: 헤더 파일
//

#pragma once
#include "ListenSocket.h"
#include "resource.h"

// CServerDlg 대화 상자
class CServerDlg : public CDialogEx
{
	// 생성입니다.
public:
	CServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	//클라이언트의 접속을 위해 대기하는 서버 소켓
	CListenSocket* m_pListenSocket;

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Text;
	CListBox m_List;
	CString m_strData;
	CStatic m_Text2;
	double player1X, player2X;
	double playerballX, playerballY, playerAngle, playerPower, ballSpeed, jumpSpeed=0;
	int turn, player1HP, player2HP;
	float PI;
	bool shooting;
	const float G = 1.0f;
	const float ground = 150.0f;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void test();
	void player1Shoot();
	void player2Shoot();
	void turnChnage();
	double doubleToInt(double num);
	double intToDouble(double num);
	double getRadian(int num);
};
