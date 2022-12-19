//------------------------------------------------------------------------
// マップの読み込み処理 File.cpp
//------------------------------------------------------------------------

#include "Map.h"

//-----------------------------------------------------------------------------
// マップファイルの読み込み
//
// 引数 TCHAR* FName
//      CMap* &pMap
//-----------------------------------------------------------------------------
BOOL CMapProc::MapRead( TCHAR* FileName, CMap* &pMap)
{
	const int BUFSIZE = 2048;                        
	FILE    *fp;
	TCHAR   szWork[BUFSIZE], ww[BUFSIZE];
	TCHAR   *p, *s;

	fp = _tfopen(FileName, _T("r"));	// スクリプト読み込み
	if (fp) 
	{
		SAFE_DELETE(pMap);				// 以前のマップを削除する
		pMap = new CMap(m_pGMain);		// マップの生成

		_tcscpy(pMap->m_MapFileName, FileName);			// マップ名を保存しておく 

		while (_fgetts(szWork, BUFSIZE, fp) != NULL)	// ファイルの最後に達するまで一行づつ読み込む
		{
			s = _tcsstr(szWork, _T("\n"));				// 文字列の最後の\nを削除する
			if (s != NULL)
			{
				*s = _T('\0');
			}

			if (_tcsncmp(szWork, _T("@MapData"), 8) == 0)		// マップ全体に関するデータ
			{
				_stscanf_s(szWork, _T("%s %d, %d, %d, %d, %s"), ww, sizeof(ww) / sizeof(TCHAR), &pMap->m_nMapX, &pMap->m_nMapY,
					&pMap->m_nMapchipWidth, &pMap->m_nMapchipHeight, pMap->m_ImageName, sizeof(pMap->m_ImageName) / sizeof(TCHAR));
				// マップの初期化
				pMap->InitMap();
			}
			else if (_tcsncmp(szWork, _T("@Back"), 5) == 0)		// 後景マップデータ
			{

				_stscanf_s(szWork, _T("%s"), ww, sizeof(ww) / sizeof(TCHAR));  // タイトル行

				for (int y = 0; y < pMap->m_nMapY; y++)			// マップデータ
				{
					if (_fgetts(szWork, BUFSIZE, fp) == NULL) break;  // 一行読み込む。データがなくなったら終了
					s = _tcsstr(szWork, _T("\n"));				// 文字列の最後の\nを削除する
					if (s != NULL)
					{
						*s = _T('\0');
					}
					p = szWork;
					for (int x = 0; x < pMap->m_nMapX; x++)
					{
						_stscanf_s(p, _T("%d,"), &pMap->m_MapBakTbl[y*pMap->m_nMapX + x]);
						p = _tcschr(p, _T(','));
						p++;
					}
				}
			}
			else if (_tcsncmp(szWork, _T("@Fore"), 5) == 0) {  // 前景マップデータ

				_stscanf_s(szWork, _T("%s"), ww, sizeof(ww) / sizeof(TCHAR));  // タイトル行

				for (int y = 0; y < pMap->m_nMapY; y++)  // マップデータ
				{
					if (_fgetts(szWork, BUFSIZE, fp) == NULL) break;  // 一行読み込む。データがなくなったら終了
					s = _tcsstr(szWork, _T("\n"));      // 文字列の最後の\nを削除する
					if (s != NULL)
					{
						*s = _T('\0');
					}
					p = szWork;
					for (int x = 0; x < pMap->m_nMapX; x++)
					{
						_stscanf_s(p, _T("%d,"), &pMap->m_MapForTbl[y*pMap->m_nMapX + x]);
						p = _tcschr(p, _T(','));
						p++;
					}
				}
			}
			else if (_tcsncmp(szWork, _T("@MapLine"), 8) == 0) {  // マップ線データ

				_stscanf_s(szWork, _T("%s %d"), ww, sizeof(ww) / sizeof(TCHAR), &pMap->m_nMapLnLength);  // タイトル行

				pMap->m_MapLn = new CMapLine[pMap->m_nMapLnLength];

				for (int i = 0; i < pMap->m_nMapLnLength; i++)  // マップ線データ
				{
					if (_fgetts(szWork, BUFSIZE, fp) == NULL) break;  // 一行読み込む。データがなくなったら終了
					s = _tcsstr(szWork, _T("\n"));      // 文字列の最後の\nを削除する
					if (s != NULL)
					{
						*s = _T('\0');
					}
					p = szWork;
					_stscanf_s(p, _T("%f,"), &pMap->m_MapLn[i].m_vStart.x);
					p = _tcschr(p, _T(','));
					p++;
					_stscanf_s(p, _T("%f,"), &pMap->m_MapLn[i].m_vStart.y);
					p = _tcschr(p, _T(','));
					p++;
					_stscanf_s(p, _T("%f,"), &pMap->m_MapLn[i].m_vEnd.x);
					p = _tcschr(p, _T(','));
					p++;
					_stscanf_s(p, _T("%f,"), &pMap->m_MapLn[i].m_vEnd.y);
					p = _tcschr(p, _T(','));
					p++;
					_stscanf_s(p, _T("%f,"), &pMap->m_MapLn[i].m_vNormal.x);
					p = _tcschr(p, _T(','));
					p++;
					_stscanf_s(p, _T("%f,"), &pMap->m_MapLn[i].m_vNormal.y);
					p = _tcschr(p, _T(','));
					p++;
				}
			}

			else if (_tcsncmp(szWork, _T("@EvtMap"), 7) == 0) {  // イベントマップデータ

				_stscanf_s(szWork, _T("%s %d"), ww, sizeof(ww)/sizeof(TCHAR), &pMap->m_nEvtMapLength);  // タイトル行
				pMap->m_EvtMap = new CEvtMap[pMap->m_nEvtMapLength];

				for (int i = 0; i < pMap->m_nEvtMapLength; i++)  // イベントマップデータ
				{
					if (_fgetts(szWork, BUFSIZE, fp) == NULL) break;  // 一行読み込む。データがなくなったら終了
					s = _tcsstr(szWork, _T("\n"));      // 文字列の最後の\nを削除する
					if (s != NULL)
					{
						*s = _T('\0');
					}
					p = szWork;

					_stscanf_s(p, _T("%f,"), &pMap->m_EvtMap[i].m_vStart.x);
					p = _tcschr(p,_T(','));
					p++;
					_stscanf_s(p, _T("%f,"), &pMap->m_EvtMap[i].m_vStart.y);
					p = _tcschr(p,_T(','));
					p++;

					_stscanf_s(p, _T("%d,"), &pMap->m_EvtMap[i].m_nEvtID);
					p = _tcschr(p,_T(','));
					p++;
					_stscanf_s(p, _T("%x,"), &pMap->m_EvtMap[i].m_dwEvtNo);
					p = _tcschr(p,_T(','));
					p++;
					_stscanf_s(p, _T("%d,"), &pMap->m_EvtMap[i].m_nCnt1);
					p = _tcschr(p,_T(','));
					p++;
				}
			}
			else {
				;  // タイトルのない行は読み飛ばす
			}
		}
		fclose(fp);
	}
	return TRUE;
}
