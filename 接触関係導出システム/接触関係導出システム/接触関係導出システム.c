#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<conio.h>



//�萔�̐錾 --------------------------------------------------
#define SysVer "Ver.1.10 2021/01/26"
#define PW "plp0831"
#define PWNum 7

#define DEF_PAR_FNAME "�ڐG�֌W���o�V�X�e���p�����[�^.txt"					//�g�p����p�����[�^�t�@�C���̖���
#define STR_MAX 500														//�����̍ő�̕��i����500�Ƃ���
#define FGETS_MAX_LENGTH 20000											//�����̍ő啶������20000�Ƃ���
#define MaxMM 50000														//��舵���ő�̑g�t���⓮��̐���50000�Ƃ���
#define Max_VFDL 200000		
#define MaxKN 50000 

//�O���[�o���ϐ��̐錾 --------------------------------------------------

char SEIHIN_Name[STR_MAX], outDir[STR_MAX];
char inPath_Input[STR_MAX];
int s_File;		//�t�@�C���̍ő�ԍ�

//char-------------------------------------------------------------------

char NowPath[STR_MAX];								//�ŏI�I�Ƀp�����[�^�t�@�C���̃t���p�X������
char SEIHIN_Name[STR_MAX];							//�n���h�����O�]���V�[�g���̐��i��������


//int,double--------------------------------------------------------------

int N;				//�S���i�̐�
int A;				//�g�t���̐�
//int VFDL_Count;		//VFDL�̐�


//�v���g�^�C�v�錾 --------------------------------------------------

//�K���K�v�Ȃ���

void GetNowPath(char* NP, char* S);		//���݂̃t�@�C������肷��֐�
void ShowFinMsg(char*);					//�������o�́H����֐�
void ReadPara(char* S);					//�g�p����p�����[�^�t�@�C����ǂݍ��ފ֐�
void Del_Com(char*);
char* Conv_Char(int);//int�^�̕ϐ���char�^�̕�����ɕϊ�����֐�


//***����̊֐�***//

//Input���ǂݍ��ݗp
void Read_Sessyoku();						//�ڐG�֌W��ǂݍ��ފ֐�

//Output���ǂݍ��ݗp
void Write_Sessyoku();    //�ڐG�֌W�������o���֐�

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
// �v���O��������
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void main(int argc, char* argv[])
{
	printf("argv[0]:%s\n", argv[0]);		//.exe���o��
	printf("NowPath:%s\n", NowPath);		//NowPath���o��
	GetNowPath(NowPath, argv[0]);			//NowPath����肷��

	strcat(NowPath, "Input\\");				//�p�����[�^�t�@�C���̒u�����.exe������t�H���_����Input�Ƃ���
	strcat(NowPath, DEF_PAR_FNAME);			//�p�����[�^�t�@�C����T��
	printf("\n");							//���s
	printf("NowPath:%s\n", NowPath);		//NowPath���o��
	printf("\n");							//���s


	ReadPara(NowPath);						//�p�����[�^�t�@�C����ǂݍ���
}

void ReadPara(char* s)
{
	FILE* fp1;//�t�@�C���|�C���^�̐錾

	/************************************************************************************************/
	/*�p�����[�^�t�@�C�����J��*/
	/************************************************************************************************/
	// �p�����[�^�t�@�C���̎d�l
	//  1�s�ځF���i��
	//  2�s�ځFVFDL���܂߂��t���p�X
	//  3�s�ځF�o�͂���t�H���_�̃p�X

	fp1 = fopen(NowPath, "r");

	if (fp1 == NULL) ShowFinMsg("�p�����[�^�t�@�C��������܂���"); // �t�@�C�����J���Ȃ���ΏI��
	if (fscanf(fp1, "%s", SEIHIN_Name) == EOF) ShowFinMsg("���i�����s���ł�");
	if (fscanf(fp1, "%s", inPath_Input) == EOF) ShowFinMsg("���͐�t�H���_�̃p�X���s���ł�");
	if (fscanf(fp1, "%s", outDir) == EOF) ShowFinMsg("�o�͐�t�H���_�̃p�X���s���ł�");
	fclose(fp1);

	printf("���i���F%s\n", SEIHIN_Name);
	printf("Input���F%s\n", inPath_Input);
	printf("Output���F%s\n\n", outDir);

	//VFDL�̓ǂݍ��݂Əo��
	printf("�]�����쐬���ł�\n");
	Read_Sessyoku();
	printf("\n�]���̍쐬���������܂����B\n");

	//�I���̏���
	printf("(Enter�L�[�������Ă�������)\n");
	unsigned char key;
	while (1) {
		key = getch();
		if (key == 0x0d) break;
	}
}

void Read_Sessyoku()           //���i������ǂݍ��ފ֐�
{
	FILE* fp1;
	char* temp, * s;     //�e�L�X�g�̓��e��ǂݍ���
	int trash[MaxKN];
	char trash_char[100][100];
	double trash_double[MaxKN];

	temp = (char*)malloc(500);

	fp1 = fopen(inPath_Input, "r");						//NowPath�̃t�@�C�����J���āA�ǂݍ���
	if (fp1 == NULL)ShowFinMsg("�ڐG����񂪂���܂���");		//�t�@�C�����J���Ȃ���ΏI��

	//�ȍ~�́A�J�����t�@�C�����ڐG���Ɋւ�����ł���O��ŏ���������

	//1�s�ڂ̓ǂݍ���
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	//2�s�ڂ̓ǂݍ���
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	sscanf(temp, "%d", &A);

	for (int i = 1; i <= A; i++)
	{
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
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

	//�o�͐�̃t���p�X��shima�ɑ������

	strcpy(m16a4, outDir);
	strcat(m16a4, "\\");
	strcat(m16a4, SEIHIN_Name);
	//strcat(m16a4, "_");
	//strcat(m16a4, a);
	strcat(m16a4, "_�ڐG�֌W.csv");

	/***�o�͂���t�@�C�����J��***/

	//  �����ŁA�t�@�C���|�C���^���擾���� 

	if ((fp = fopen(m16a4, "w")) == NULL)				// �G���[�̏ꍇ�͒ʏ�A�ُ�I������ 
	{
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "����,���i��,CAD�t�@�C��,���i�ԍ�,���SX,���SY,���SZ,����X,����Y,����Z,Xmin,Xmax,Ymin,Ymax,Zmin,Zmax,�F�ԍ�,�d��,�H��ԍ�,�H�,����,����(1:�˂��A2:O�AC�����O�A3:E�����O�A4:�΂ˁA5:�i�b�g),OK\n");


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
	// ������̍Ōォ�當������������
	// �y�����z
	// const char *s�@�F�@�����Ώە�����
	// int c�@�F�@��������
	// �y�߂�l�z
	// ���������Ƃ��@�F�@��v�����̃A�h���X
	// ������Ȃ������Ƃ��@�F�@NULL

	for (i = 0; i <= (p - S); i++) {
		NP[i] = S[i];
	}
	NP[i] = '\0'; // �Ō��NULL��t��
}

void ShowFinMsg(char* msg) {
	int c;
	printf("%s\n", msg);
	printf("(Enter�L�[�������Ă�������)\n");
	unsigned char key;
	while (1) {
		key = getch();
		if (key == 0x0d) break;
	}
}

//�ǂݍ��񂾕�����́u,�v�i�J���}�j���u �v�i���p�X�y�[�X�j�ɒu��������֐�
void Del_Com(char* s) {
	int	i;

	i = 0;
	while (s[i] != '\0') {
		if (s[i] == ',') s[i] = ' ';
		i++;
	}
}

//int�^�̕ϐ���char�^�̕�����ɕϊ�����֐�
char* Conv_Char(int no) {
	char buf[10];
	sprintf(buf, "%d", no);
	return buf;
}









