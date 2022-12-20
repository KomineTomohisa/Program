#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<conio.h>



//定数の宣言 --------------------------------------------------
#define SysVer "Ver.1.10 2021/01/26"
#define PW "plp0831"
#define PWNum 7

#define DEF_PAR_FNAME "接触関係導出システムパラメータ.txt"					//使用するパラメータファイルの名称
#define STR_MAX 500														//文字の最大の部品数を500とする
#define FGETS_MAX_LENGTH 20000											//文字の最大文字数を20000とする
#define MaxMM 50000														//取り扱う最大の組付けや動作の数を50000とする
#define Max_VFDL 200000		
#define MaxKN 50000 

//グローバル変数の宣言 --------------------------------------------------

char SEIHIN_Name[STR_MAX], outDir[STR_MAX];
char inPath_Input[STR_MAX];
int s_File;		//ファイルの最大番号

//char-------------------------------------------------------------------

char NowPath[STR_MAX];								//最終的にパラメータファイルのフルパスを入れる
char SEIHIN_Name[STR_MAX];							//ハンドリング評価シート内の製品名を入れる


//int,double--------------------------------------------------------------

int N;				//全部品の数
int A;				//組付けの数
//int VFDL_Count;		//VFDLの数


//プロトタイプ宣言 --------------------------------------------------

//必ず必要なもの

void GetNowPath(char* NP, char* S);		//現在のファイルを特定する関数
void ShowFinMsg(char*);					//文字を出力？する関数
void ReadPara(char* S);					//使用するパラメータファイルを読み込む関数
void Del_Com(char*);
char* Conv_Char(int);//int型の変数をchar型の文字列に変換する関数


//***自作の関数***//

//Input情報読み込み用
void Read_Sessyoku();						//接触関係を読み込む関数

//Output情報読み込み用
void Write_Sessyoku();    //接触関係を書き出す関数

struct Sessyoku
{
	int Sessyoku_Num;
	int Kotei_B;
	int Idou_B;
	char Kumituke_Houkou[STR_MAX];
	double ASPa_X;
	double ASPa_Y;
	double ASPa_Z;
	double OFFSETa_X;
	double OFFSETa_Y;
	double OFFSETa_Z;
	double APPa_X;
	double APPa_Y;
	double APPa_Z;
	int OK;
	double Time;
};
struct Sessyoku R_Se[150];


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// プログラム部分
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void main(int argc, char* argv[])
{
	printf("argv[0]:%s\n", argv[0]);		//.exeを出力
	printf("NowPath:%s\n", NowPath);		//NowPathを出力
	GetNowPath(NowPath, argv[0]);			//NowPathを特定する

	strcat(NowPath, "Input\\");				//パラメータファイルの置き場は.exeがあるフォルダ下のInputとする
	strcat(NowPath, DEF_PAR_FNAME);			//パラメータファイルを探す
	printf("\n");							//改行
	printf("NowPath:%s\n", NowPath);		//NowPathを出力
	printf("\n");							//改行


	ReadPara(NowPath);						//パラメータファイルを読み込む
}

void ReadPara(char* s)
{
	FILE* fp1;//ファイルポインタの宣言

	/************************************************************************************************/
	/*パラメータファイルを開く*/
	/************************************************************************************************/
	// パラメータファイルの仕様
	//  1行目：製品名
	//  2行目：VFDLを含めたフルパス
	//  3行目：出力するフォルダのパス

	fp1 = fopen(NowPath, "r");

	if (fp1 == NULL) ShowFinMsg("パラメータファイルがありません"); // ファイルを開けなければ終了
	if (fscanf(fp1, "%s", SEIHIN_Name) == EOF) ShowFinMsg("製品名が不正です");
	if (fscanf(fp1, "%s", inPath_Input) == EOF) ShowFinMsg("入力先フォルダのパスが不正です");
	if (fscanf(fp1, "%s", outDir) == EOF) ShowFinMsg("出力先フォルダのパスが不正です");
	fclose(fp1);

	printf("製品名：%s\n", SEIHIN_Name);
	printf("Input情報：%s\n", inPath_Input);
	printf("Output情報：%s\n\n", outDir);

	//VFDLの読み込みと出力
	printf("評価を作成中です\n");
	Read_Sessyoku();
	printf("\n評価の作成が完了しました。\n");

	//終了の処理
	printf("(Enterキーを押してください)\n");
	unsigned char key;
	while (1) {
		key = getch();
		if (key == 0x0d) break;
	}
}

void Read_Sessyoku()           //部品属性を読み込む関数
{
	FILE* fp1;
	char* temp, * s;     //テキストの内容を読み込む
	int trash[MaxKN];
	char trash_char[100][100];
	double trash_double[MaxKN];

	temp = (char*)malloc(500);

	fp1 = fopen(inPath_Input, "r");						//NowPathのファイルを開いて、読み込む
	if (fp1 == NULL)ShowFinMsg("接触線情報がありません");		//ファイルを開けなければ終了

	//以降は、開けたファイルが接触線に関する情報である前提で処理をする

	//1行目の読み込み
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	//2行目の読み込み
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	sscanf(temp, "%d", &A);

	for (int i = 1; i <= A; i++)
	{
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d%d%d%s%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%lf", &R_Se[i].Sessyoku_Num, &R_Se[i].Kotei_B, &R_Se[i].Idou_B,&R_Se[i].Kumituke_Houkou,&R_Se[i].ASPa_X, &R_Se[i].ASPa_Y,&R_Se[i].ASPa_Z, &R_Se[i].OFFSETa_X , &R_Se[i].OFFSETa_Y, &R_Se[i].OFFSETa_Z, &R_Se[i].APPa_X, &R_Se[i].APPa_Y, &R_Se[i].APPa_Z,& R_Se[i].OK, &R_Se[i].Time);
	}


	Write_Sessyoku();
	free(temp);
	fclose(fp1);
}


void Write_Sessyoku()
{
	FILE* fp;
	char m16a4[STR_MAX], a[Max_VFDL];

	//sprintf(a, "VFDL%d", VFDL_No);

	//出力先のフルパスをshimaに代入する

	strcpy(m16a4, outDir);
	strcat(m16a4, "\\");
	strcat(m16a4, SEIHIN_Name);
	//strcat(m16a4, "_");
	//strcat(m16a4, a);
	strcat(m16a4, "_接触関係.csv");

	/***出力するファイルを開く***/

	//  ここで、ファイルポインタを取得する 

	if ((fp = fopen(m16a4, "w")) == NULL)				// エラーの場合は通常、異常終了する 
	{
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "部番,部品名,CADファイル,部品番号,中心X,中心Y,中心Z,長さX,長さY,長さZ,Xmin,Xmax,Ymin,Ymax,Zmin,Zmax,色番号,重量,工具番号,工具名,質感,特徴(1:ねじ、2:O、Cリング、3:Eリング、4:ばね、5:ナット),OK\n");


	fprintf(fp, "%d\n", A);

	for (int i = 1; i <= A; i++)
	{
		fprintf(fp, "%d,%d,%d,%s,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%d,%4.2lf\n", R_Se[i].Sessyoku_Num, R_Se[i].Kotei_B, R_Se[i].Idou_B, R_Se[i].Kumituke_Houkou, R_Se[i].ASPa_X, R_Se[i].ASPa_Y, R_Se[i].ASPa_Z, R_Se[i].OFFSETa_X, R_Se[i].OFFSETa_Y, R_Se[i].OFFSETa_Z, R_Se[i].APPa_X, R_Se[i].APPa_Y, R_Se[i].APPa_Z, R_Se[i].OK, R_Se[i].Time);
	}
	fclose(fp);

}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Work
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


void GetNowPath(char* NP, char* S)
{
	char* p;
	int i;

	p = strrchr(S, '\\');
	// strrchr
	// 文字列の最後から文字を検索する
	// 【引数】
	// const char *s　：　検索対象文字列
	// int c　：　検索文字
	// 【戻り値】
	// 見つかったとき　：　一致文字のアドレス
	// 見つからなかったとき　：　NULL

	for (i = 0; i <= (p - S); i++) {
		NP[i] = S[i];
	}
	NP[i] = '\0'; // 最後にNULLを付加
}

void ShowFinMsg(char* msg) {
	int c;
	printf("%s\n", msg);
	printf("(Enterキーを押してください)\n");
	unsigned char key;
	while (1) {
		key = getch();
		if (key == 0x0d) break;
	}
}

//読み込んだ文字列の「,」（カンマ）を「 」（半角スペース）に置き換える関数
void Del_Com(char* s) {
	int	i;

	i = 0;
	while (s[i] != '\0') {
		if (s[i] == ',') s[i] = ' ';
		i++;
	}
}

//int型の変数をchar型の文字列に変換する関数
char* Conv_Char(int no) {
	char buf[10];
	sprintf(buf, "%d", no);
	return buf;
}









