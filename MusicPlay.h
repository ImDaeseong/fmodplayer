#pragma once
#include "afxwin.h"

class MusicPlay : public CDialog
{
public:
	MusicPlay(CWnd* pParent = NULL);  
	
	enum { IDD = IDD_DIALOG_PLAY };

    protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

protected:
    virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	
private:
	FMOD::System    *system;
    FMOD::Sound     *sound;
    FMOD::Channel   *channel;

	FMOD::DSP       *dspEcho;
	FMOD::DSP       *dspDisto;

	float m_fVolume;

	void InitFMOD();
	void CloseFMOD();
	void PlayMusic(LPCSTR chMusicName);
	void Play();
	void Pause();
	void Stop();
	void setVolume();
	void IncreaseVolume(BOOL bInCrease);


private:
	void DrawSkin(CDC* pDC);
	BOOL LoadSkin();
	void MovePlayerLocation();
	void InitButtons();
	CGdiPlusBitmapResource* m_pBackSkin;
	void DisplayTime(Graphics& gps);
	
	CString m_strTime;
	int m_nCurrentPlay;
	int m_nMaxCount;

	CStringArray m_FileList;
	CString m_strSelectPath;
	CString m_strCurrentPath;
	void SetFileInfo(CString strPath);

	static UINT FindMp3ThreadFunc(LPVOID pParam);
	void ThreadCaryMp3Files();

	CString GetModulePath();
	void LoadFindPlayList();

public:
	CSkinButton m_btnPlay;
	CSkinButton m_btnLeft;
	CSkinButton m_btnRight;
	CSkinButton m_btnPlus;
	CSkinButton m_btnMin;
	CSkinButton m_btnFolder;
	CSkinButton m_btnStop;
	CSkinButton m_btnClose;
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonPlayleft();
	afx_msg void OnBnClickedButtonPlaynext();
	afx_msg void OnBnClickedButtonSuaraplus();
	afx_msg void OnBnClickedButtonSuaramin();
	afx_msg void OnBnClickedButtonPhilihfolder();
	afx_msg LRESULT OnFindFilesClose(WPARAM wParam, LPARAM lParam);
};
