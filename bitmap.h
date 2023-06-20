// -------------------------------------------------------------------------------
// FileName : bitmap.h bitmap.c
// Version  : ver1.0 (2005/6/17)
// History  :
//   ver1.0 (2005/06/17) Edited by Terutoshi Iwasaki
//     ・作成
//   UTF-8に変更(2012.06.07)
// -------------------------------------------------------------------------------
#ifndef bitmapH
#define bitmapH
// -------------------------------------------------------------------------------
// 8bit ピクセルデータ
typedef unsigned char BYTE;

// 24bit ピクセルデータ
typedef struct tagRGB
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
} RGB;
// -------------------------------------------------------------------------------
// 8bit用
BYTE** CreateBitmap8( long lWidth, long lHeight );
void DeleteBitmap8( BYTE** aBitmap, long lWidth, long lHeight );
int ReadBitmap8( const char* pszFileName, BYTE** aBuffer, long lWidth, long lHeight );
int WriteBitmap8( const char* pszFileName, BYTE** aBuffer, long lWidth, long lHeight );

// 24bit用
RGB** CreateBitmap24( long lWidth, long lHeight );
void DeleteBitmap24( RGB** aBitmap, long lWidth, long lHeight );
int ReadBitmap24( const char* pszFileName, RGB** aBuffer, long lWidth, long lHeight );
int WriteBitmap24( const char* pszFileName, RGB** aBuffer, long lWidth, long lHeight );
// -------------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
