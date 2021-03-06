#include "stdafx.h"
#include "fmodplayer.h"
#include "MusicPlay.h"

MusicPlay::MusicPlay(CWnd* pParent /*=NULL*/)
	: CDialog(MusicPlay::IDD, pParent)
{
	system = 0;
    sound = 0;
    channel = 0;

	dspEcho = NULL;
	dspDisto = NULL; 

	m_pBackSkin	= NULL;	

	m_strSelectPath = "";

	m_strTime = "";

	m_fVolume = 0.0;
}

void MusicPlay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_BUTTON_PLAYLEFT, m_btnLeft);
	DDX_Control(pDX, IDC_BUTTON_PLAYNEXT, m_btnRight);
	DDX_Control(pDX, IDC_BUTTON_SUARAPLUS, m_btnPlus);
	DDX_Control(pDX, IDC_BUTTON_SUARAMIN, m_btnMin);
	DDX_Control(pDX, IDC_BUTTON_PHILIHFOLDER, m_btnFolder);
	DDX_Control(pDX, IDC_BUTTON_PLAYSTOP, m_btnStop);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
}

BEGIN_MESSAGE_MAP(MusicPlay, CDialog)
	ON_WM_PAINT()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &MusicPlay::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_PLAYLEFT, &MusicPlay::OnBnClickedButtonPlayleft)
	ON_BN_CLICKED(IDC_BUTTON_PLAYNEXT, &MusicPlay::OnBnClickedButtonPlaynext)
	ON_BN_CLICKED(IDC_BUTTON_SUARAPLUS, &MusicPlay::OnBnClickedButtonSuaraplus)
	ON_BN_CLICKED(IDC_BUTTON_SUARAMIN, &MusicPlay::OnBnClickedButtonSuaramin)
	ON_BN_CLICKED(IDC_BUTTON_PHILIHFOLDER, &MusicPlay::OnBnClickedButtonPhilihfolder)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &MusicPlay::OnBnClickedButtonClose)
	ON_MESSAGE(WM_FINDFILES_CLOSE, OnFindFilesClose)	
END_MESSAGE_MAP()

void MusicPlay::InitFMOD()
{
	FMOD_RESULT result;
    unsigned int version;
    void  *extradriverdata = 0;

	result = FMOD::System_Create(&system);
	result = system->getVersion(&version);
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);   

	setVolume();
}

void MusicPlay::CloseFMOD()
{
	FMOD_RESULT result;
	result = sound->release();
	result = system->close(); 
    result = system->release();
}

void MusicPlay::PlayMusic(LPCSTR chMusicName)
{	
	FMOD_RESULT result;

	if( channel != NULL )
		channel->stop();
	
	if( sound != NULL )
		sound->release();

	//Default
	result = system->createStream(chMusicName, FMOD_DEFAULT, 0, &sound);

	system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspEcho);
	system->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dspDisto);

	dspEcho->setBypass(true);
	dspDisto->setBypass(true);
	channel->addDSP(0, dspEcho);
	channel->addDSP(0, dspDisto);
	
	//Play
	result = system->playSound(sound, 0, false, &channel);
	
	//Timer
	KillTimer(1);
	SetTimer(1, 1000, NULL );
}

void MusicPlay::Play()
{
	if( channel != NULL )
		channel->setPaused( false );
}

void MusicPlay::Pause()
{
	if( channel != NULL )
	{
		FMOD_RESULT result;
		bool paused;
		result = channel->getPaused(&paused);
		result = channel->setPaused(!paused);
	}
}

void MusicPlay::Stop()
{
	bool isPlaying = false;
	FMOD_RESULT result;
	
	if (channel)
	{
		result = channel->isPlaying(&isPlaying);
	}
		
	if (isPlaying) 
	{
		result = channel->stop();
	}
	else 
	{
		result = system->playSound(sound, 0, false, &channel);
	}
}

void MusicPlay::setVolume()
{
	FMOD_RESULT result;

	if (m_fVolume > 1)
		m_fVolume = 1;
	else if (m_fVolume < 0)
		m_fVolume = 0;

	result = channel->setVolume(m_fVolume);
}

void MusicPlay::IncreaseVolume(BOOL bInCrease)
{
	if (bInCrease)
		m_fVolume += 0.1;
	else
		m_fVolume -= 0.1;

	setVolume();
}

void MusicPlay::OnDestroy() 
{		
	CloseFMOD();

	if(m_pBackSkin != NULL)
		delete m_pBackSkin;
	m_pBackSkin = NULL;	

	CDialog::OnDestroy();
}

void MusicPlay::OnTimer(UINT nIDEvent) 
{	
	FMOD_RESULT result;
	//result = system->update();

	unsigned int ms = 0;
	unsigned int lenms = 0;
	bool         playing = 0;
	bool         paused = 0;
	int          channelsplaying = 0;
	if (channel)
	{
		FMOD::Sound *currentsound = 0;
		result = channel->isPlaying(&playing);
		result = channel->getPaused(&paused);                
		result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);                
		
		channel->getCurrentSound(&currentsound);
		if (currentsound)
		{
			result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);                    
		}

		CString strTime;
		strTime.Format("Play Time %02d:%02d:%02d/%02d:%02d:%02d", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100);
		m_strTime = strTime;
		Invalidate();

		if(!playing)
		{
			OnBnClickedButtonPlaynext();
		}
	}
	//system->getChannelsPlaying(&channelsplaying, NULL);
	
	CDialog::OnTimer(nIDEvent);
}


BOOL MusicPlay::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT MusicPlay::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	CDC* pDC = GetDC();
	if(pDC != NULL)
	{
		DrawSkin(pDC);
		ReleaseDC(pDC);
	}
	MovePlayerLocation();
	return 0;
}

void MusicPlay::OnPaint()
{
	CPaintDC dc(this); 
	DrawSkin(&dc);
}

BOOL MusicPlay::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	
	InitButtons();

	if(!LoadSkin())
	{
		CDialog::OnCancel();
		return FALSE;
	}

	CDC* pDC = GetDC();
	DrawSkin(pDC);
	ReleaseDC(pDC);
			
	InitFMOD();

	LoadFindPlayList();

	return TRUE; 
}

BOOL MusicPlay::LoadSkin()
{
	m_pBackSkin = new CGdiPlusBitmapResource;
	if(!m_pBackSkin->Load(IDB_PNG_BG, "PNG", AfxGetApp()->m_hInstance))
	{
		delete m_pBackSkin;
		m_pBackSkin = NULL;
		return FALSE;
	}	
	MovePlayerLocation();

	return TRUE;
}

void MusicPlay::DrawSkin(CDC* pDC)
{
	CDC memDC;
	CBitmap bmp;
	CBitmap* bmp_old;

	CRect rect;
	GetWindowRect(&rect);

	int cx = rect.Width();
	int cy = rect.Height();

	memDC.CreateCompatibleDC(pDC);

	BITMAPINFO bmInfo;
	memset(&bmInfo, 0x00, sizeof(BITMAPINFO));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = cx;
	bmInfo.bmiHeader.biHeight = cy;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 32;
	bmInfo.bmiHeader.biCompression = BI_RGB;
	bmInfo.bmiHeader.biSizeImage = 0;
	bmInfo.bmiHeader.biClrUsed = 0;
	bmInfo.bmiHeader.biClrImportant = 0;

	LPBYTE pbmpBits = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), (LPBITMAPINFO)&bmInfo, DIB_RGB_COLORS, (void **)&pbmpBits, NULL, 0);
	if(hBitmap == NULL)
		bmp.CreateCompatibleBitmap(pDC, cx, cy);
	else
		bmp.Attach(hBitmap);

	bmp_old = memDC.SelectObject(&bmp);
	Graphics gps(memDC.GetSafeHdc());

	gps.DrawImage(
		m_pBackSkin->m_pBitmap, 
		Rect(0, 0, m_pBackSkin->m_pBitmap->GetWidth(), m_pBackSkin->m_pBitmap->GetHeight()), 
		0,
		0, 
		m_pBackSkin->m_pBitmap->GetWidth(), 
		m_pBackSkin->m_pBitmap->GetHeight(),
		UnitPixel);

	if(m_btnClose.IsAvailableDraw())
		m_btnClose.OnDrawLayerdWindow(gps);

	if(m_btnPlay.IsAvailableDraw())
		m_btnPlay.OnDrawLayerdWindow(gps);
	
	if(m_btnLeft.IsAvailableDraw())
		m_btnLeft.OnDrawLayerdWindow(gps);

	if(m_btnRight.IsAvailableDraw())
		m_btnRight.OnDrawLayerdWindow(gps);

	if(m_btnPlus.IsAvailableDraw())
		m_btnPlus.OnDrawLayerdWindow(gps);
	
	if(m_btnMin.IsAvailableDraw())
		m_btnMin.OnDrawLayerdWindow(gps);
	
	if(m_btnFolder.IsAvailableDraw())
		m_btnFolder.OnDrawLayerdWindow(gps);

	DisplayTime(gps);

	CPoint ptWindowScreenPosition(rect.TopLeft());
	CSize szWindow(cx, cy);

	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	CPoint ptSrc(0,0);

	HDC hdc = gps.GetHDC();

	BOOL bRet= ::UpdateLayeredWindow(GetSafeHwnd(), 
		pDC->GetSafeHdc(),
		&ptWindowScreenPosition,
		&szWindow,
		hdc,
		&ptSrc,
		0, 
		&blendPixelFunction,
		ULW_ALPHA);

	gps.ReleaseHDC(hdc);

	memDC.SelectObject(bmp_old);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

void MusicPlay::DisplayTime(Graphics& gps)
{	
USES_CONVERSION;

	Gdiplus::Font font1(L"굴림", 11, FontStyleRegular, UnitPixel);
	SolidBrush brush1(Color(234, 137, 6));

	StringFormat stringFormat(StringFormatFlagsDisplayFormatControl);
	stringFormat.SetAlignment(StringAlignmentCenter);	
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	
	CRect rect;
    GetClientRect(&rect);
	
	RectF rectF1(REAL(rect.left + 1), REAL(rect.top + 415), REAL(rect.Width()), REAL(30));
	gps.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	gps.DrawString(A2W(m_strTime), -1, &font1, rectF1, &stringFormat, &brush1);
}

void MusicPlay::MovePlayerLocation()
{	
	int nOffset = 10;
	int cx = m_pBackSkin->m_pBitmap->GetWidth();
	int cy = m_pBackSkin->m_pBitmap->GetHeight();

	RECT rcWorkArea;
	SystemParametersInfo (SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	
	MoveWindow(rcWorkArea.right - cx - nOffset, rcWorkArea.bottom - cy - nOffset,  cx, cy);
	::SetWindowPos(this->m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);	
}

void MusicPlay::InitButtons()
{
	m_btnPlay.SetImage(MAKEINTRESOURCE(IDB_PNG_PLAY), "PNG", AfxGetApp()->m_hInstance, 81, 112, 1);
	m_btnLeft.SetImage(MAKEINTRESOURCE(IDB_PNG_LEFT), "PNG", AfxGetApp()->m_hInstance, 25, 112, 1);
	m_btnRight.SetImage(MAKEINTRESOURCE(IDB_PNG_RIGHT), "PNG", AfxGetApp()->m_hInstance, 136, 112, 1);
	m_btnPlus.SetImage(MAKEINTRESOURCE(IDB_PNG_PLUS), "PNG", AfxGetApp()->m_hInstance, 81, 57, 1);
	m_btnMin.SetImage(MAKEINTRESOURCE(IDB_PNG_MIN), "PNG", AfxGetApp()->m_hInstance,  81, 167, 1);
	m_btnFolder.SetImage(MAKEINTRESOURCE(IDB_PNG_ADDFILE), "PNG", AfxGetApp()->m_hInstance,  65, 222, 1);
	m_btnClose.SetImage(MAKEINTRESOURCE(IDB_PNG_CLOSE), "PNG", AfxGetApp()->m_hInstance, 158, 11, 4);
}

void MusicPlay::OnBnClickedButtonClose()
{	
	CDialog::OnOK();
}

void MusicPlay::OnBnClickedButtonPlay()
{
	if( channel != NULL )
	{
		FMOD_RESULT result;
		bool paused;
		result = channel->getPaused(&paused);
		result = channel->setPaused(!paused);
	}
}

void MusicPlay::OnBnClickedButtonPlayleft()
{
	if(m_nMaxCount == 0)
		return;

    m_nCurrentPlay --;

	if(m_nCurrentPlay < 0)
		m_nCurrentPlay = m_nMaxCount - 1;


	m_strCurrentPath = m_FileList.GetAt(m_nCurrentPlay);	
		
	Stop();	
	PlayMusic((LPCTSTR)m_strCurrentPath);
}

void MusicPlay::OnBnClickedButtonPlaynext()
{
	if(m_nMaxCount == 0)
		return;

    m_nCurrentPlay ++;

	if(m_nCurrentPlay > m_nMaxCount-1)
		m_nCurrentPlay = 0;

	m_strCurrentPath = m_FileList.GetAt(m_nCurrentPlay);
	
	Stop();
	PlayMusic((LPCTSTR)m_strCurrentPath);
}

void MusicPlay::OnBnClickedButtonSuaraplus()
{
	IncreaseVolume(TRUE);
}

void MusicPlay::OnBnClickedButtonSuaramin()
{
	IncreaseVolume(FALSE);
}

void MusicPlay::OnBnClickedButtonPhilihfolder()
{
	LPMALLOC	pMalloc;

    if (::SHGetMalloc(&pMalloc) == NOERROR)
    {
        BROWSEINFO bi;
        char pszBuffer[MAX_PATH];
        LPITEMIDLIST pidl;
		
        bi.hwndOwner = GetSafeHwnd();
        bi.pidlRoot = NULL;
        bi.pszDisplayName = pszBuffer;
        bi.lpszTitle = _T("Select Mp3");
        bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
        bi.lpfn = NULL;
        bi.lParam = 0;

        if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)
        {
            if (::SHGetPathFromIDList(pidl, pszBuffer))
            { 
				m_strSelectPath = pszBuffer;
            }

            pMalloc->Free(pidl);
        }
        pMalloc->Release();
    }

	if(m_strSelectPath== "")
		return;

	Stop();		

	m_btnFolder.EnableWindow(FALSE);

	AfxBeginThread(FindMp3ThreadFunc, this);
}

void MusicPlay::SetFileInfo(CString strPath)
{
	m_FileList.RemoveAll();
	
	CFileFind finder;
	CString strWildcard = strPath;
	strWildcard += _T("\\*.*");

	CString strFullPath;
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			strFullPath = finder.GetFilePath();
			SetFileInfo(strFullPath);
		}
		else
		{
			CString strFullPath = finder.GetFilePath();
			CString strFileName = finder.GetFileName();
			CString strExtention = strFileName.Mid(strFileName.ReverseFind('.')+1); 

			if(lstrcmpi(strExtention, "mp3") != 0 )
				continue;

			m_FileList.Add(strFullPath);		 
		}
	}
	finder.Close();
}

UINT MusicPlay::FindMp3ThreadFunc(LPVOID pParam)
{
	MusicPlay * pManager = (MusicPlay *)pParam;
	pManager->ThreadCaryMp3Files();
	pManager->PostMessage(WM_FINDFILES_CLOSE, 0, 0);
	return 0;  
}

void MusicPlay::ThreadCaryMp3Files()
{
	SetFileInfo(m_strSelectPath);
}

LRESULT MusicPlay::OnFindFilesClose(WPARAM wParam, LPARAM lParam)
{
	m_strCurrentPath = m_FileList.GetAt(0);
	m_nMaxCount = m_FileList.GetSize();
	
	m_nCurrentPlay = 0;
	
	PlayMusic((LPCTSTR)m_strCurrentPath);
	
	m_btnFolder.EnableWindow(TRUE);

	return 0;
}

CString MusicPlay::GetModulePath()
{
	char cTemp[MAX_PATH];
	char *spzRealDirEnd;
	CString strModulePath;

	GetModuleFileName(NULL, cTemp, MAX_PATH);  
	spzRealDirEnd = strrchr(cTemp, '\\');
	*spzRealDirEnd = '\0';
	strModulePath = (CString)cTemp;
	return strModulePath;
}

void MusicPlay::LoadFindPlayList()
{
	m_FileList.RemoveAll();

	CString strWildcard;
	strWildcard.Format("%s\\mp3\\*.mp3", GetModulePath());
	
	CFileFind finder;
    BOOL bWorking = finder.FindFile(strWildcard);
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
		 
		if (finder.IsDots() || finder.IsDirectory())
			continue;

		m_FileList.Add(finder.GetFilePath());		
    }
	finder.Close();

	m_nMaxCount = m_FileList.GetSize();

	if(m_nMaxCount == 0) return;

	m_strCurrentPath = m_FileList.GetAt(0);
		
	m_nCurrentPlay = 0;
	
	PlayMusic((LPCTSTR)m_strCurrentPath);
}
