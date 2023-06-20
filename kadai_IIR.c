// -------------------------------------------------------------------------------
// ディジタル信号処理 課題 (2)
// -------------------------------------------------------------------------------
// FileName : kadai_IIR_filter.c
// Version  : ver 1.0
// History  :
//   ver 1.0 (2007.06.22) Edited by Tatsuya Nakamura
//     ・作成
//   UTF-8に変更(2012.06.07)
// -------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitmap.h"
#define PI 3.14159265358 // π
#define MAX_MSK_SIZE 15	 // フィルタのマスクサイズの最大値:15×15(初期値)
#define MSK_IIR_SIZE 2	 // IIR フィルタのマスクサイズの値
//--------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------
int MSK_SIZE; // フィルタのマスクサイズ
double SIGMA; // ガウス関数の分散
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

// ソート記述部分

//--------------------------------------------------------------------------------

unsigned char IIR_Filter(unsigned char aPixel[MSK_IIR_SIZE * MSK_IIR_SIZE])
{
	//IIRフィルタ アルゴリズムを記述
	// 返り値は仮なので変更してOKです

	double a = 0.70, b = 0.70;
	double sum = 0.0;

	// IIRフィルタの計算
	for (int i = 0; i < MSK_IIR_SIZE * MSK_IIR_SIZE; i++)
	{
		sum += aPixel[i];
	}

	unsigned char result = (unsigned char)(a * sum + b); // フィルタの出力値

	return result;
}
//--------------------------------------------------------------------------------

unsigned char AverageFilter(unsigned char aPixel[MAX_MSK_SIZE * MAX_MSK_SIZE])
{

	// 平均フィルタ アルゴリズムを記述
	// 返り値は仮なので変更してOKです

	return 0;
}
//--------------------------------------------------------------------------------

unsigned char GaussianFilter(unsigned char aPixel[MAX_MSK_SIZE * MAX_MSK_SIZE])
{
	// ガウシアンフィルタ アルゴリズムを記述
	// 返り値は仮なので変更してOKです

	return 0;
}
//--------------------------------------------------------------------------------

// メイン関数
int main(int argc, char *argv[])
{

	char szInputFile[256];	// 入力ファイル名
	char szOutputFile[256]; // 出力ファイル名
	int i, j, k, num = 0;	// カウンタ

	const long lWidth = 256;  // 画像の幅
	const long lHeight = 256; // 画像の高さ

	unsigned char **aBitmapIn;	// ビットマップデータ(2次元配列へのポインタ)
	unsigned char **aBitmapOut; // ビットマップデータ(2次元配列へのポインタ)

	unsigned char aPixel[MAX_MSK_SIZE * MAX_MSK_SIZE];

	// ビットマップデータ(2次元配列)の作成
	aBitmapIn = CreateBitmap8(lWidth, lHeight);
	aBitmapOut = CreateBitmap8(lWidth, lHeight);

	// ファイル名を指定
	printf("入力ファイル名: ");
	scanf("%s", szInputFile);
	printf("出力ファイル名: ");
	scanf("%s", szOutputFile);

	// ビットマップファイルの読み込み
	ReadBitmap8(szInputFile, aBitmapIn, lWidth, lHeight);

	// -----------------------------------------------------
	// 上記関数と以下を変更して
	//
	// フィードフォワード部分の入力はFIRフィルターを通した画素として
	// IIRフィルタをかけてください
	// iがY方向, jがX方向です.
	// -----------------------------------------------------

	printf("Input mask size:");
	scanf("%d", &MSK_SIZE); // MSK_SIZEの読み込み

	SIGMA = (double)(MSK_SIZE - 1) / 4; // ガウス関数の分散の設定(MSK_SIZE=2k+1のときsigma=k/2)

	//フィードフォワードの部分の入力を作成
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			if ((i - (MSK_SIZE - 1) / 2 < 0) || (i + (MSK_SIZE - 1) / 2 > lHeight - 1) || (j - (MSK_SIZE - 1) / 2 < 0) || (j + (MSK_SIZE - 1) / 2 > lWidth - 1))
			{
				aBitmapOut[i][j] = aBitmapIn[i][j]; // 外周ならそのままコピー
			}
			else
			{
				for (k = 0; k < MSK_SIZE * MSK_SIZE; k++)
				{
					aPixel[k] = aBitmapIn[i + k / MSK_SIZE - (MSK_SIZE - 1) / 2][j + k % MSK_SIZE - (MSK_SIZE - 1) / 2]; // マスク生成
				}
				aBitmapOut[i][j] = MedianFilter(aPixel); // 例: メディアンフィルターをかける
			}
		}
	}

	// IIR フィルタの出力を作成
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			if (i - (MSK_IIR_SIZE - 1) < 0 || j - (MSK_IIR_SIZE - 1) < 0)
			{
				//外周ならそのまま
			}
			else
			{
				for (k = 0; k < MSK_IIR_SIZE * MSK_IIR_SIZE; k++)
				{
					aPixel[k] = aBitmapOut[i - (MSK_IIR_SIZE - 1) + k / MSK_IIR_SIZE][j - (MSK_IIR_SIZE - 1) + k % MSK_IIR_SIZE];
				}
				aBitmapOut[i][j] = IIR_Filter(aPixel); // IIRフィルタをかける
			}
		}
	}

	// ビットマップファイル書き込み
	WriteBitmap8(szOutputFile, aBitmapOut, lWidth, lHeight);

	// 2次元配列の削除
	DeleteBitmap8(aBitmapIn, lWidth, lHeight);
	DeleteBitmap8(aBitmapOut, lWidth, lHeight);

	return 0;
}

// -------------------------------------------------------------------------------
