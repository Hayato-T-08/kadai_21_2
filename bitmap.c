// -------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
// -------------------------------------------------------------------------------
// ファイル情報ヘッダ
typedef struct tagBITMAPFILEHEADER
{
	unsigned short bfType;
	unsigned long  bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long  bfOffBits;
} BITMAPFILEHEADER;

// 画像情報ヘッダ
typedef struct tagBITMAPINFOHEADER
{
	unsigned long  biSize;
	long           biWidth;
    long           biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long  biCompression;
    unsigned long  biSizeImage;
    long           biXPixPerMeter;
    long           biYPixPerMeter;
    unsigned long  biClrUsed;
    unsigned long  biClrImporant;
} BITMAPINFOHEADER;

// カラーパレット
typedef struct tagRGBQUAD{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} RGBQUAD;
// -------------------------------------------------------------------------------
int ReadFileHeader( BITMAPFILEHEADER* pHeader, FILE* pFile )
{
	int nRet = 1;
	
	nRet &= fread( &pHeader->bfType,      2, 1, pFile );
	nRet &= fread( &pHeader->bfSize,      4, 1, pFile );
	nRet &= fread( &pHeader->bfReserved1, 2, 1, pFile );
	nRet &= fread( &pHeader->bfReserved2, 2, 1, pFile );
	nRet &= fread( &pHeader->bfOffBits,   4, 1, pFile );

	return nRet;
}
// -------------------------------------------------------------------------------
int ReadInfoHeader( BITMAPINFOHEADER* pHeader, FILE* pFile )
{
	int nRet = 1;

	nRet &= fread( &pHeader->biSize,         4, 1, pFile );
	nRet &= fread( &pHeader->biWidth,        4, 1, pFile );
	nRet &= fread( &pHeader->biHeight,       4, 1, pFile );
	nRet &= fread( &pHeader->biPlanes,       2, 1, pFile );
	nRet &= fread( &pHeader->biBitCount,     2, 1, pFile );
	nRet &= fread( &pHeader->biCompression,  4, 1, pFile );
	nRet &= fread( &pHeader->biSizeImage,    4, 1, pFile );
	nRet &= fread( &pHeader->biXPixPerMeter, 4, 1, pFile );
	nRet &= fread( &pHeader->biYPixPerMeter, 4, 1, pFile );
	nRet &= fread( &pHeader->biClrUsed,      4, 1, pFile );
	nRet &= fread( &pHeader->biClrImporant,  4, 1, pFile );

	return nRet;
}
// -------------------------------------------------------------------------------
int WriteFileHeader( BITMAPFILEHEADER* pHeader, FILE* pFile )
{
	int nRet = 1;
	
	nRet &= fwrite( &pHeader->bfType,      2, 1, pFile );
	nRet &= fwrite( &pHeader->bfSize,      4, 1, pFile );
	nRet &= fwrite( &pHeader->bfReserved1, 2, 1, pFile );
	nRet &= fwrite( &pHeader->bfReserved2, 2, 1, pFile );
	nRet &= fwrite( &pHeader->bfOffBits,   4, 1, pFile );

	return nRet;
}
// -------------------------------------------------------------------------------
int WriteInfoHeader( BITMAPINFOHEADER* pHeader, FILE* pFile )
{
	int nRet = 1;

	nRet &= fwrite( &pHeader->biSize,         4, 1, pFile );
	nRet &= fwrite( &pHeader->biWidth,        4, 1, pFile );
	nRet &= fwrite( &pHeader->biHeight,       4, 1, pFile );
	nRet &= fwrite( &pHeader->biPlanes,       2, 1, pFile );
	nRet &= fwrite( &pHeader->biBitCount,     2, 1, pFile );
	nRet &= fwrite( &pHeader->biCompression,  4, 1, pFile );
	nRet &= fwrite( &pHeader->biSizeImage,    4, 1, pFile );
	nRet &= fwrite( &pHeader->biXPixPerMeter, 4, 1, pFile );
	nRet &= fwrite( &pHeader->biYPixPerMeter, 4, 1, pFile );
	nRet &= fwrite( &pHeader->biClrUsed,      4, 1, pFile );
	nRet &= fwrite( &pHeader->biClrImporant,  4, 1, pFile );

	return nRet;
}
// -------------------------------------------------------------------------------
// 8bit ビットマップ
BYTE** CreateBitmap8( long lWidth, long lHeight )
{
	BYTE** aBitmap; // 2次元配列
	int    i;       // カウンタ

	aBitmap = (BYTE**)calloc( lHeight, sizeof(BYTE*) );

	for ( i=0; i<lHeight; i++ )
	{
		aBitmap[i] = (BYTE*)calloc( lWidth, sizeof(BYTE) );
	}

	return aBitmap;
}
// -------------------------------------------------------------------------------
void DeleteBitmap8( BYTE** aBitmap, long lWidth, long lHeight )
{
	int i;    // カウンタ

	for ( i=0; i<lHeight; i++ )
	{
		free( aBitmap[i] );
	}

	free( aBitmap );
}
// -------------------------------------------------------------------------------
int ReadBitmap8( const char* pszFileName, BYTE** aBuffer, long lWidth, long lHeight )
{
	BITMAPFILEHEADER vFH={}; // ファイル情報ヘッダ
	BITMAPINFOHEADER vIH={}; // 画像情報ヘッダ

	FILE* pFile; // ファイルポインタ
	int i, j;    // カウンタ

	// ファイルオープン
	if ( (pFile = fopen( pszFileName, "rb" )) == NULL )
	{
		printf( "Error: ファイルが開けません" );
		return 0;
	}

	// BITMAPFILEHEADER
	if ( !ReadFileHeader( &vFH, pFile ) )
	{
		printf( "Error: ファイルサイズが小さすぎます" );
		fclose( pFile );
		return 0;
	}

	// BITMAPINFOHEADER
	if ( !ReadInfoHeader( &vIH, pFile ) )
	{
		printf( "Error: Windowsビットマップではありません" );
		fclose( pFile );
		return 0;
	}

	
	if ( vIH.biSize != 40 )
	{
		printf( "Error: OS/2ビットマップには対応していません" );
		fclose( pFile );
		return 0;
	}

	if ( vIH.biBitCount != 8 )
	{
		printf( "Error: %dビット ビットマップには対応していません", vIH.biBitCount );
		fclose( pFile );
		return 0;
	}

	if ( vIH.biCompression != 0 )
	{
		printf( "Error: 圧縮形式には対応していません" );
		fclose( pFile );
		return 0;
	}

	// カラーパレットを読み飛ばす
	fseek( pFile, 256*4, SEEK_CUR );

	// 画像データ読み込み(左下から右上へ)
	for ( i=lHeight-1; i>=0; i-- )
	{
		for ( j=0; j<lWidth; j++ )
		{
			fread( &aBuffer[i][j], sizeof(BYTE), 1, pFile );
		}
	}

	// ファイルクローズ
	fclose( pFile );
	return 1;
}
// -------------------------------------------------------------------------------
int WriteBitmap8( const char* pszFileName, BYTE** aBuffer, long lWidth, long lHeight )
{
	long lSize;   // 補正したファイルサイズ
	long lOffset; // 画像データまでのオフセット
	
	BITMAPFILEHEADER vFH; // ファイル情報ヘッダ
	BITMAPINFOHEADER vIH; // 画像情報ヘッダ
	RGBQUAD vPalette;     // パレット情報

	FILE* pFile; // ファイルポインタ       
	int i, j; // カウンタ

	// サイズ補正
	if ( lWidth % 4 != 0 )
	{
		lWidth = ((lWidth/4)+1)*4;
	}

	lSize = lWidth * lHeight;

	// オフセット
	lOffset = 14 + 40 + 256*4;

	// ファイル情報ヘッダの設定
	vFH.bfType      = 'B' + ('M'<<8);  // "BM"固定
	vFH.bfSize      = lSize + lOffset; // ヘッダを含むデータ数
	vFH.bfReserved1 = 0;               // 予約領域 (0固定)
	vFH.bfReserved2 = 0;               // 予約領域 (0固定)
	vFH.bfOffBits   = lOffset;         // 画像データまでのオフセット

	// 画像情報ヘッダの設定
	vIH.biSize         = 40;       // 情報ヘッダのサイズ(Byte) (40: Windows BMP)
	vIH.biWidth        = lWidth;   // 画像の幅
	vIH.biHeight       = lHeight;  // 画像の高さ
	vIH.biPlanes       = 1;        // プレーン数 (1固定)
	vIH.biBitCount     = 8;        // 1ピクセルのビット数
	vIH.biCompression  = 0;        // 圧縮形式 (0: 無圧縮)
	vIH.biSizeImage    = lSize;    // 画像サイズ(Byte) (補正済みの値)
	vIH.biXPixPerMeter = 0;        // X方向解像度 (96dpi, 1inch = 0.0254m のとき 96/0.0254 = 3780)
	vIH.biYPixPerMeter = 0;        // Y方向解像度 (96dpi, 1inch = 0.0254m のとき 96/0.0254 = 3780)
	vIH.biClrUsed      = 1 << 8;   // カラーパレットの色数
	vIH.biClrImporant  = 0;        // カラーパレットの重要な色

// ファイル書き込み
	// ファイルオープン
	if ( (pFile = fopen( pszFileName, "wb" )) == NULL )
	{
		printf( "Error: ファイルが開けません" );
		return 0;
	}

	// ヘッダ書き込み
	WriteFileHeader( &vFH, pFile );
	WriteInfoHeader( &vIH, pFile );

	// カラーパレット(グレースケール)作成
	vPalette.rgbReserved = 0;
	for ( i=0; i<256; i++ )
	{
		vPalette.rgbRed = vPalette.rgbGreen = vPalette.rgbBlue = (unsigned char)i;
		fwrite( &vPalette, sizeof(RGBQUAD), 1, pFile ); 
	}

	// 画像データ書き込み(左下から右上へ)
	for ( i=lHeight-1; i>=0; i-- )
	{
		for ( j=0; j<lWidth; j++ )
		{
			fwrite( &aBuffer[i][j], sizeof(BYTE), 1, pFile );
		}
	}

	// ファイルクローズ
	fclose( pFile );
	return 1;
}
// -------------------------------------------------------------------------------
// 24bit ビットマップ
RGB** CreateBitmap24( long lWidth, long lHeight )
{
	RGB** aBitmap; // 2次元配列
	int   i;       // カウンタ

	aBitmap = (RGB**)calloc( lHeight, sizeof(RGB*) );

	for ( i=0; i<lHeight; i++ )
	{
		aBitmap[i] = (RGB*)calloc( lWidth, sizeof(RGB) );
	}

	return aBitmap;
}
// -------------------------------------------------------------------------------
void DeleteBitmap24( RGB** aBitmap, long lWidth, long lHeight )
{
	int i;    // カウンタ

	for ( i=0; i<lHeight; i++ )
	{
		free( aBitmap[i] );
	}

	free( aBitmap );
}
// -------------------------------------------------------------------------------
int ReadBitmap24( const char* pszFileName, RGB** aBuffer, long lWidth, long lHeight )
{
	BITMAPFILEHEADER vFH={}; // ファイル情報ヘッダ
	BITMAPINFOHEADER vIH={}; // 画像情報ヘッダ

	FILE* pFile; // ファイルポインタ
	int i, j;    // カウンタ

	// ファイルオープン
	if ( (pFile = fopen( pszFileName, "rb" )) == NULL )
	{
		printf( "Error: ファイルが開けません" );
		return 0;
	}

	// BITMAPFILEHEADER
	if ( !ReadFileHeader( &vFH, pFile ) )
	{
		printf( "Error: ファイルサイズが小さすぎます" );
		fclose( pFile );
		return 0;
	}

	// BITMAPINFOHEADER
	if ( !ReadInfoHeader( &vIH, pFile ) )
	{
		printf( "Error: Windowsビットマップではありません" );
		fclose( pFile );
		return 0;
	}

	if ( vIH.biSize != 40 )
	{
		printf( "Error: OS/2ビットマップには対応していません" );
		fclose( pFile );
		return 0;
	}

	if ( vIH.biBitCount != 24 )
	{
		printf( "Error: %dビット ビットマップには対応していません", vIH.biBitCount );
		fclose( pFile );
		return 0;
	}

	if ( vIH.biCompression != 0 )
	{
		printf( "Error: 圧縮形式には対応していません" );
		fclose( pFile );
		return 0;
	}
	
	// 画像データ読み込み(左下から右上へ)
	for ( i=lHeight-1; i>=0; i-- )
	{
		for ( j=0; j<lWidth; j++ )
		{
			fread( &aBuffer[i][j], sizeof(RGB), 1, pFile );
		}
	}

	// ファイルクローズ
	fclose( pFile );
	return 1;
}
// -------------------------------------------------------------------------------
int WriteBitmap24( const char* pszFileName, RGB** aBuffer, long lWidth, long lHeight )
{
	long lSize;   // 補正したファイルサイズ
	long lOffset; // 画像データまでのオフセット
	
	BITMAPFILEHEADER vFH; // ファイル情報ヘッダ
	BITMAPINFOHEADER vIH; // 画像情報ヘッダ

	FILE* pFile; // ファイルポインタ       
	int i, j; // カウンタ

	// サイズ補正
	if ( (lWidth*3)%4 != 0 )
	{
		lWidth = ((lWidth*3/4)+1)*4;
	}
	lSize = 3 * lWidth * lHeight; // 現時点ではサイズ固定

	// オフセット
	lOffset = 14 + 40; // 24bitはカラーパレットなし

	// ファイル情報ヘッダの設定
	vFH.bfType      = 'B' + ('M'<<8);  // "BM"固定
	vFH.bfSize      = lSize + lOffset; // ヘッダを含むデータ数
	vFH.bfReserved1 = 0;               // 予約領域 (0固定)
	vFH.bfReserved2 = 0;               // 予約領域 (0固定)
	vFH.bfOffBits   = lOffset;         // 画像データまでのオフセット

	// 画像情報ヘッダの設定
	vIH.biSize         = 40;       // 情報ヘッダのサイズ(Byte) (40: Windows BMP)
	vIH.biWidth        = lWidth;   // 画像の幅
	vIH.biHeight       = lHeight;  // 画像の高さ
	vIH.biPlanes       = 1;        // プレーン数 (1固定)
	vIH.biBitCount     = 24;       // 1ピクセルのビット数
	vIH.biCompression  = 0;        // 圧縮形式 (0: 無圧縮)
	vIH.biSizeImage    = lSize;    // 画像サイズ(Byte) (補正済みの値)
	vIH.biXPixPerMeter = 0;        // X方向解像度 (96dpi, 1inch = 0.0254m のとき 96/0.0254 = 3780)
	vIH.biYPixPerMeter = 0;        // Y方向解像度 (96dpi, 1inch = 0.0254m のとき 96/0.0254 = 3780)
	vIH.biClrUsed      = 0;        // カラーパレットの色数
	vIH.biClrImporant  = 0;        // カラーパレットの重要な色

// ファイル書き込み
	// ファイルオープン
	if ( (pFile = fopen( pszFileName, "wb" )) == NULL )
	{
		printf( "Error: ファイルが開けません" );
		return 0;
	}

	// ヘッダ書き込み
	WriteFileHeader( &vFH, pFile );
	WriteInfoHeader( &vIH, pFile );

	// 画像データ書き込み(左下から右上へ)
	for ( i=lHeight-1; i>=0; i-- )
	{
		for ( j=0; j<lWidth; j++ )
		{
			fwrite( &aBuffer[i][j], sizeof(RGB), 1, pFile );
		}
	}

	// ファイルクローズ
	fclose( pFile );
	return 1;
}
// ----------------------------------------------------------------------------
