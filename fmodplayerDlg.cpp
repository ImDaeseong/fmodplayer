#include "stdafx.h"
#include "fmodplayer.h"
#include "fmodplayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CfmodplayerDlg::CfmodplayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CfmodplayerDlg::IDD, pParent)
{
	system = 0;
    sound = 0;
    channel = 0;

	dspEcho = NULL;
	dspDisto = NULL; 

	m_nMaxCount = 0;
	m_nCurrentPlay = 0;
	m_fVolume = 0.5;
}

void CfmodplayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstBox);
}

BEGIN_MESSAGE_MAP(CfmodplayerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_LBN_DBLCLK(IDC_LIST1, &CfmodplayerDlg::OnLbnDblclkList1)
END_MESSAGE_MAP()

//sample source
int FMOD_playsound()
{
    FMOD::System     *system;
    FMOD::Sound      *sound1, *sound2, *sound3;
    FMOD::Channel    *channel = 0;
    FMOD_RESULT       result;
    unsigned int      version;
    void             *extradriverdata = 0;
   
	//Create a System object and initialize
    result = FMOD::System_Create(&system);
   
	result = system->getVersion(&version);
    if (version < FMOD_VERSION)
    {
    }
	
    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    
    result = system->createSound("c:\\Amnesia.mp3", FMOD_DEFAULT, 0, &sound1);

    result = sound1->setMode(FMOD_LOOP_OFF); 

    result = system->createSound("c:\\Growing.mp3", FMOD_DEFAULT, 0, &sound2);
  
	result = system->createSound("c:\\love.mp3", FMOD_DEFAULT, 0, &sound3);

	result = system->playSound(sound1, 0, false, &channel);
	result = system->playSound(sound2, 0, false, &channel);
	result = system->playSound(sound3, 0, false, &channel);

	CString strMsg1, strMsg2;
	BOOL bPlay = FALSE;
	do
    {  
	   result = system->update();

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
	   }
	   system->getChannelsPlaying(&channelsplaying, NULL);

	   strMsg1.Format("Time %02d:%02d:%02d/%02d:%02d:%02d : %s", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100);
	   strMsg2.Format("Channels Playing %d", channelsplaying);
	 
	   Sleep(50);
    } while (!bPlay);
     
	//Shut down
    result = sound1->release();  
    result = sound2->release();   
	result = sound3->release();
    result = system->close(); 
    result = system->release();   

    return 0;
}

//sample source
int FMOD_playstream()
{
    FMOD::System     *system;
    FMOD::Sound      *sound, *sound_to_play;
    FMOD::Channel    *channel = 0;
    FMOD_RESULT       result;
    unsigned int      version;
    void             *extradriverdata = 0;
    int               numsubsounds;
    
   	//Create a System object and initialize.
    result = FMOD::System_Create(&system);
    
    result = system->getVersion(&version);
    if (version < FMOD_VERSION)
    {
    }

    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    
    result = system->createStream("c:\\Amnesia.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
   
    result = sound->getNumSubSounds(&numsubsounds);
   
    if (numsubsounds)
    {
        sound->getSubSound(0, &sound_to_play);
    }
    else
    {
        sound_to_play = sound;
    }

	//Play the sound.
    result = system->playSound(sound_to_play, 0, false, &channel);
	  
    CString strMsg1;
	BOOL bPlay = FALSE;
	do
    {
        //bool paused;
		//result = channel->getPaused(&paused);
		//result = channel->setPaused(!paused);
       
        result = system->update();
       
        {
            unsigned int ms = 0;
            unsigned int lenms = 0;
            bool         playing = false;
            bool         paused = false;

            if (channel)
            {
                result = channel->isPlaying(&playing);               

                result = channel->getPaused(&paused);

                result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
               
                result = sound_to_play->getLength(&lenms, FMOD_TIMEUNIT_MS);
            }
        
            strMsg1.Format("Time %02d:%02d:%02d/%02d:%02d:%02d : %s", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100);
		}

        Sleep(50);
    } while (!bPlay);


	// Shut down
    result = sound->release(); 
    result = system->close();
    result = system->release();

    return 0;
}

//sample source
int FMOD_netstream()
{
    FMOD::System    *system = 0;
    FMOD::Sound     *sound = 0;
    FMOD::Channel   *channel = 0;
    FMOD_RESULT      result = FMOD_OK;
    FMOD_OPENSTATE   openstate = FMOD_OPENSTATE_READY;
    unsigned int     version = 0;
    void            *extradriverdata = 0;
    const int        tagcount = 4;
    int              tagindex = 0;
    char             tagstring[tagcount][128] = { 0 };
    
    //Create a System object and initialize.  
	result = FMOD::System_Create(&system);
  
    result = system->getVersion(&version);
    if (version < FMOD_VERSION)
    {
    }

    result = system->init(1, FMOD_INIT_NORMAL, extradriverdata);
  
	/* Increase the file buffer size a little bit to account for Internet lag. */
    result = system->setStreamBufferSize(64*1024, FMOD_TIMEUNIT_RAWBYTES);

    result = system->createSound("http://www.listen.pls", FMOD_CREATESTREAM | FMOD_NONBLOCKING, 0, &sound);    
	   
	BOOL bPlay = FALSE;
    do
    {
        unsigned int    pos = 0;
        unsigned int    percent = 0;
        bool            playing = false;
        bool            paused = false;
        bool            starving = false;
        const char     *state = "Stopped";
        
        if (channel)
        {
            //bool paused = false;
            //result = channel->getPaused(&paused);               
            //result = channel->setPaused(!paused);
        }
        
        result = system->update();
               
        result = sound->getOpenState(&openstate, &percent, &starving, 0);
        
        if (channel)
        {
            FMOD_TAG tag;
        
            
            //    Read any tags that have arrived, this could happen if a radio station switches
            //    to a new song.
            while (sound->getTag(0, -1, &tag) == FMOD_OK)
            {
                if (tag.datatype == FMOD_TAGDATATYPE_STRING)
                {
                    sprintf(tagstring[tagindex], "%s = '%s' (%d bytes)", tag.name, (char *)tag.data, tag.datalen);
                    tagindex = (tagindex + 1) % tagcount;
                }
                else if (tag.type == FMOD_TAGTYPE_FMOD)
                {
                    // When a song changes, the sample rate may also change, so compensate here. 
                    if (!strcmp(tag.name, "Sample Rate Change"))
                    {
                        float frequency = *((float *)tag.data);
                        
                        result = channel->setFrequency(frequency);
                    }
                }
            }
            
            result = channel->getPaused(&paused);
            
            result = channel->isPlaying(&playing);
            
            result = channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
            
            // Silence the stream until we have sufficient data for smooth playback. 
            result = channel->setMute(starving);
        }
        else
        {
            // This may fail if the stream isn't ready yet, so don't check the error code. 
            system->playSound(sound, 0, false, &channel);
        }
        
        if (openstate == FMOD_OPENSTATE_BUFFERING)
        {
            state = "Buffering...";
        }
        else if (openstate == FMOD_OPENSTATE_CONNECTING)
        {
            state = "Connecting...";
        }
        else if (paused)
        {
            state = "Paused";
        }
        else if (playing)
        {
            state = "Playing";
        }
       
    } while (!bPlay);


	////Stop the channel, then wait for it to finish opening before we release it.
 //   if (channel)
 //   {
 //       result = channel->stop();
 //   }

 //   do
 //   {   
 //       result = system->update();

 //       result = sound->getOpenState(&openstate, 0, 0, 0);

 //   } while (openstate != FMOD_OPENSTATE_READY);

 //   //Shut down
 //   result = sound->release();
 //   result = system->close();
 //   result = system->release();
 //   

    return 0;
}

int RandomBetween(int nMin, int nMax)
{
    if(nMin == nMax) return nMin;
    int nValue = rand()/RAND_MAX;
    return nMin + nValue * (nMax - nMin);
}

BOOL CfmodplayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//test
	//FMOD_playsound();
	//FMOD_playstream();
	//FMOD_netstream();

	InitFMOD();
	
	MusicLoad();

	return TRUE;  
}

void CfmodplayerDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CfmodplayerDlg::OnDestroy() 
{
	CloseFMOD();

	CDialog::OnDestroy();
}

void CfmodplayerDlg::OnLbnDblclkList1()
{
	if( m_lstBox.GetCount() == 0 ) return;

	int nIndex = m_lstBox.GetCurSel();
	if( nIndex == -1 ) return;
	
	CString strFileName;
	m_lstBox.GetText(nIndex, strFileName );
	
	m_nCurrentPlay = nIndex;

	PlayMusic((LPCTSTR)strFileName);
}

CString CfmodplayerDlg::GetModulePath()
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

void CfmodplayerDlg::MusicLoad()
{
	while( m_lstBox.GetCount() > 0 )
		m_lstBox.DeleteString( 0 );
	
	CString strWildcard;
	//strWildcard.Format("%s\\mp3\\*.wav", GetModulePath());
	strWildcard.Format("%s\\mp3\\*.mp3", GetModulePath());
	
	CFileFind finder;
    BOOL bWorking = finder.FindFile(strWildcard);
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
		 
		if (finder.IsDots() || finder.IsDirectory())
			continue;

		m_lstBox.AddString(finder.GetFilePath());		
    }
	finder.Close();

	m_nMaxCount = 0;
	m_nMaxCount = m_lstBox.GetCount();
	
	PlayLoop(TRUE);
}

void CfmodplayerDlg::PlayLoop(BOOL bFirst)
{
	if(bFirst)
	{
		if(m_nMaxCount == 0)
			return;

		CString strFileName;
		m_lstBox.GetText(m_nCurrentPlay, strFileName );
		m_lstBox.SetCurSel(m_nCurrentPlay);
		PlayMusic((LPCTSTR)strFileName);
	}
	else
	{
		if(m_nMaxCount == 0)
			return;

		m_nCurrentPlay ++;

		if(m_nCurrentPlay > m_nMaxCount-1)
			m_nCurrentPlay = 0;
		
		CString strFileName;
		m_lstBox.GetText(m_nCurrentPlay, strFileName );
		m_lstBox.SetCurSel(m_nCurrentPlay);

		PlayMusic((LPCTSTR)strFileName);
	}
}

void CfmodplayerDlg::MusicNext()
{
	int nIndex = m_lstBox.GetCurSel();
	if( nIndex == -1 || nIndex + 1 >= m_lstBox.GetCount() )	
		return;	
	
	nIndex++;
	
	CString strFileName;
	m_lstBox.GetText(nIndex, strFileName );
	m_lstBox.SetCurSel(nIndex);
	
	PlayMusic((LPCTSTR)strFileName);
}

void CfmodplayerDlg::MusicPreview()
{
	int nIndex = m_lstBox.GetCurSel();
	if( nIndex == -1 || nIndex - 1 < 0 )
		return;	

	nIndex--;

	CString strFileName;
	m_lstBox.GetText(nIndex, strFileName );
	m_lstBox.SetCurSel(nIndex);

	PlayMusic((LPCTSTR)strFileName);
}

void CfmodplayerDlg::OnTimer(UINT nIDEvent) 
{		
	FMOD_RESULT result;
	result = system->update();

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
	}
	system->getChannelsPlaying(&channelsplaying, NULL);

	
	CString strTime;
	strTime.Format("Play Time %02d:%02d:%02d/%02d:%02d:%02d", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100);
	SetDlgItemText(IDC_STATIC_TEXT, strTime);

	if(!playing)
	{
		PlayLoop();
	}

	
	CDialog::OnTimer(nIDEvent);
}

void CfmodplayerDlg::InitFMOD()
{
	FMOD_RESULT result;
    unsigned int version;
    void  *extradriverdata = 0;

	result = FMOD::System_Create(&system);
	result = system->getVersion(&version);
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);   
}

void CfmodplayerDlg::CloseFMOD()
{
	//FMOD_Channel_Stop(NULL);

	FMOD_RESULT result;
	result = sound->release();
	result = system->close(); 
    result = system->release();
}

void CfmodplayerDlg::PlayMusic(LPCSTR chMusicName)
{	
	FMOD_RESULT result;

	//FMOD_Channel_Stop(NULL);

	if( channel != NULL )
		channel->stop();
	
	if( sound != NULL )
		sound->release();
	
	//Loop
	//result = system->createStream(chMusicName, FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
	
	//Default
	result = system->createStream(chMusicName, FMOD_DEFAULT, 0, &sound);

	//result = system->createSound(chMusicName, FMOD_DEFAULT | FMOD_2D, NULL, &sound);

	system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspEcho);
	system->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dspDisto);

	dspEcho->setBypass(true);
	dspDisto->setBypass(true);
	channel->addDSP(0, dspEcho);
	channel->addDSP(0, dspDisto);

	//Set Volume
	setVolume();

	//Play
	result = system->playSound(sound, 0, false, &channel);
	

	//Timer
	KillTimer(1);
	SetTimer(1, 1000, NULL );
}

void CfmodplayerDlg::Play()
{
	if( channel != NULL )
		channel->setPaused( false );
}

void CfmodplayerDlg::Pause()
{
	if( channel != NULL )
	{
		FMOD_RESULT result;
		bool paused;
		result = channel->getPaused(&paused);
		result = channel->setPaused(!paused);
	}
}

void CfmodplayerDlg::Stop()
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

void CfmodplayerDlg::setVolume()
{
	FMOD_RESULT result;

	if (m_fVolume > 1)
		m_fVolume = 1;
	else if (m_fVolume < 0)
		m_fVolume = 0;

	result = channel->setVolume(m_fVolume);
}

void CfmodplayerDlg::IncreaseVolume(BOOL bInCrease)
{
	if (bInCrease)
		m_fVolume += 0.1;
	else
		m_fVolume -= 0.1;

	setVolume();
}