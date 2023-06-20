// -------------------------------------------------------------------------------
// ディジタル信号処理 課題 (2)
// -------------------------------------------------------------------------------
// FileName : kadai_FIR_filter.c
// Version  : ver 1.0
// History  :
//   ver 1.0 (2007.06.22) Edited by Tatsuya Nakamura
//     ・作成
//   UTF-8に変更(2012.6.07)
// -------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitmap.h"
#define PI 3.14159265358 // π
#define MAX_MSK_SIZE 15	 // フィルタのマスクサイズの最大値:15×15(初期値)

//--------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------
int MSK_SIZE; // フィルタのマスクサイズ
double SIGMA; // ガウス関数の分散
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// ソート記述部分

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
unsigned char GaussianFilter(unsigned char aPixel[MAX_MSK_SIZE * MAX_MSK_SIZE])
{
	// ガウシアンフィルタ アルゴリズムを記述
	// 返り値は仮なので変更してOKです
	double weightSum = 0.0;
	double filteredPixel = 0.0;

	// ガウシアンフィルタの重み付けと重みの合計を計算
	for (int i = 0; i < MSK_SIZE; i++)
	{
		for (int j = 0; j < MSK_SIZE; j++)
		{
			int x = j - (MSK_SIZE - 1) / 2;
			int y = i - (MSK_SIZE - 1) / 2;

			double weight = exp(-(x * x + y * y) / (2 * SIGMA * SIGMA));
			weightSum += weight;

			filteredPixel += weight * aPixel[i * MSK_SIZE + j];
		}
	}

	// 正規化した値を返す
	return (unsigned char)(filteredPixel / weightSum);
}
//--------------------------------------------------------------------------------
unsigned char MedianFilter(unsigned char aPixel[MAX_MSK_SIZE * MAX_MSK_SIZE])
{
	// メディアンフィルタ アルゴリズムを記述
	// 返り値は仮なので変更してOKです
	// ピクセル値のソート
	for (int i = 0; i < MSK_SIZE * MSK_SIZE - 1; i++)
	{
		for (int j = 0; j < MSK_SIZE * MSK_SIZE - i - 1; j++)
		{
			if (aPixel[j] > aPixel[j + 1])
			{
				// ピクセル値の入れ替え
				unsigned char temp = aPixel[j];
				aPixel[j] = aPixel[j + 1];
				aPixel[j + 1] = temp;
			}
		}
	}

	// 中央値を返す
	return aPixel[(MSK_SIZE * MSK_SIZE) / 2];
}
//--------------------------------------------------------------------------------
unsigned char AverageFilter(unsigned char aPixel[MAX_MSK_SIZE * MAX_MSK_SIZE])
{
	// 平均フィルタ アルゴリズムを記述
	// 返り値は仮なので変更してOKです
	int sum = 0;

	// ピクセル値の合計を計算
	for (int i = 0; i < MSK_SIZE * MSK_SIZE; i++)
	{
		sum += aPixel[i];
	}

	// 平均値を返す
	return sum / (MSK_SIZE * MSK_SIZE);
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

	unsigned char aPixel[MAX_MSK_SIZE * MAX_MSK_SIZE]; //処理を行う時のマスク画素の配列

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
	// 1:メディアンフィルタ
	// 2:平均フィルタ
	// 3:ガウシアンフィルタ
	//
	// の3種類のフィルタをかけてください
	// iがY方向, jがX方向です.
	// -----------------------------------------------------

	printf("Input mask size:");
	scanf("%d", &MSK_SIZE); // MSK_SIZEの読み込み

	SIGMA = (double)(MSK_SIZE - 1) / 4; // ガウス関数の分散の設定(MSK_SIZE=2k+1のときsigma=k/2)

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
				aBitmapOut[i][j] = AverageFilter(aPixel); // 例: メディアンフィルタをかける
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
