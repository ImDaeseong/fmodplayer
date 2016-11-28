#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CfmodplayerDlg : public CDialog
{
public:
	CfmodplayerDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_FMODPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	FMOD::System    *system;
    FMOD::Sound     *sound;
    FMOD::Channel   *channel;

	FMOD::DSP       *dspEcho;
	FMOD::DSP       *dspDisto;
	
	FMOD_TAG          tag;
    FMOD_SOUND_TYPE   soundtype;

	void InitFMOD();
	void CloseFMOD();
	void PlayMusic(LPCSTR chMusicName);
	void Play();
	void Pause();
	void Stop();
	void MusicLoad();
	void MusicNext();
	void MusicPreview();

	CString GetModulePath();

	void PlayLoop(BOOL bFirst = FALSE);
	int m_nMaxCount;
	int m_nCurrentPlay;

	void setVolume();
	void IncreaseVolume(BOOL bInCrease);
	float m_fVolume;

	
	CListBox m_lstBox;
	afx_msg void OnLbnDblclkList1();
};
