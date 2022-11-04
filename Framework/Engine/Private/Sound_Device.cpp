#include "Sound_Device.h"

IMPLEMENT_SINGLETON(CSound_Device)

CSound_Device::CSound_Device()
	: m_pSystem(nullptr)
	, m_eCurChannel(CH6)
	, m_iCurPlayerChannel(CH_PLAYER_0)
{}


HRESULT CSound_Device::Initialize()
{
	// ���带 ����ϴ� ��ǥ��ü�� �����ϴ� �Լ�
	FMOD_System_Create(&m_pSystem);

	//FMOD_System_SetSoftwareFormat(m_pSystem, 48000, FMOD_SOUND_FORMAT_PCM32, 0, 0, FMOD_DSP_RESAMPLER_NOINTERP);
	//FMOD_System_SetDSPBufferSize(m_pSystem, 512, 4);

	// 1. �ý��� ������, 2. ����� ����ä�� �� , �ʱ�ȭ ���) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();

	return S_OK;
}

void CSound_Device::Update()
{
	/*m_iCount++;
	if (5 < m_iCount)
	{
	m_iCount = 0;
	FMOD_System_Update(m_pSystem);
	}*/
}


void CSound_Device::LoadSoundFile()
{
	// _finddata_t : <io.h>���� �����ϸ� ���� ������ �����ϴ� ����ü
	_finddata_t fd;
	// _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�
	INT_PTR handle = _findfirst("../../Resources/Sound/GamePlay/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../../Resources/Sound/GamePlay/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../ Sound/Success.wav"
		strcat_s(szFullPath, fd.name);

		//wstring strFilePath = CPathMgr::Get_Instance()->Get_ContentPath();
		//strFilePath += fd.name;

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = (int)strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// �ƽ�Ű �ڵ� ���ڿ��� �����ڵ� ���ڿ��� ��ȯ�����ִ� �Լ�
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
		iResult = _findnext(handle, &fd);
	}


	_findclose(handle);




	{
		// _finddata_t : <io.h>���� �����ϸ� ���� ������ �����ϴ� ����ü
		_finddata_t fd;
		// _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�
		INT_PTR handle = _findfirst("../../Resources/Sound/UI/*.*", &fd);

		if (handle == -1)
			return;

		int iResult = 0;

		char szCurPath[128] = "../../Resources/Sound/UI/";
		char szFullPath[128] = "";

		while (iResult != -1)
		{
			strcpy_s(szFullPath, szCurPath);

			// "../ Sound/Success.wav"
			strcat_s(szFullPath, fd.name);

			//wstring strFilePath = CPathMgr::Get_Instance()->Get_ContentPath();
			//strFilePath += fd.name;

			FMOD_SOUND* pSound = nullptr;

			FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

			if (eRes == FMOD_OK)
			{
				int iLength = (int)strlen(fd.name) + 1;

				TCHAR* pSoundKey = new TCHAR[iLength];
				ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

				// �ƽ�Ű �ڵ� ���ڿ��� �����ڵ� ���ڿ��� ��ȯ�����ִ� �Լ�
				MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

				m_mapSound.emplace(pSoundKey, pSound);
			}
			//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
			iResult = _findnext(handle, &fd);
		}

		_findclose(handle);

	}

	FMOD_System_Update(m_pSystem);

}

void CSound_Device::PlayBGM(const _tchar * strSoundKey, _float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
	{
		return !lstrcmp(strSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSound_Device::Play_Sound(const _tchar * strSoundKey, _float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
	{
		return !lstrcmp(strSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	_uint iChannel = m_eCurChannel;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[m_eCurChannel], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[m_eCurChannel++]);
		SetChannelVolume((CHANNELID)iChannel, fVolume);
	}



	if (m_eCurChannel >= CH_END)
		m_eCurChannel = CH6;

	FMOD_System_Update(m_pSystem);
}

void CSound_Device::Play_Sound_Player(const _tchar * strSoundKey, _float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
	{
		return !lstrcmp(strSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	_uint iChannel = m_iCurPlayerChannel;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[m_eCurChannel], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[m_iCurPlayerChannel++]);
		SetChannelVolume((CHANNELID)iChannel, fVolume);
	}

	if (m_iCurPlayerChannel >= CH6)
		m_iCurPlayerChannel = CH_PLAYER_0;

	FMOD_System_Update(m_pSystem);
}


void CSound_Device::StopSound(CHANNELID eType)
{
	FMOD_Channel_Stop(m_pChannelArr[eType]);

}

void CSound_Device::StopAll()
{
	for (UINT i = 0; i < CH_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Device::SetVolume(_float fVolume)
{
	for (int i = BGM; i < CH_END; ++i)
	{
		FMOD_Channel_SetVolume(m_pChannelArr[i], fVolume);
	}

	FMOD_System_Update(m_pSystem);
}

void CSound_Device::SetChannelVolume(CHANNELID eID, _float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSound_Device::WithoutBGM()
{
	for (UINT i = 1; i < CH_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Device::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}


