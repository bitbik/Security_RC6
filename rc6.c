/**************************************************************
 *							                                *
 *	This is CS558 (Security) Project1 implement RC6	        *
 *							                                *
 *							                                *
 *			@ NEIL CHEN				                        *
 *							                                *
 *							                                *
 *************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define W 32
#define R 20
#define LW 5
#define SIZE_S R*2+4
#define Pw 0xB7E15163
#define Qw 0x9E3779B9


int max(int num1, int num2){
	return ((num1>num2)?num1:num2);
}

unsigned int rotate_l(unsigned int x, unsigned int bit){
	return ((x << bit) | (x >> (32-bit)));
}

unsigned int rotate_r(unsigned int x, unsigned int bit){
	return ((x >> bit) | (x << (32 -bit)));
}


int main(int argc, char** argv){


/******************************************************

        part 1, variants and storage arrays, etc.
******************************************************/

	FILE *input;
	FILE *output;
	int usage;//to indicate the usage of program
	char inputfile[30];
	char outputfile[30];

	int i;
	unsigned int text;

	int r;
	char text_type[20];
	unsigned int t;
	unsigned int u;
	unsigned int temp;
	unsigned int A,B,C,D;
	unsigned int S[SIZE_S];

	int b=0;//key size
	int c=0;//L size
	int keybit=0;//max is 256
        int CC;
	int v;

	unsigned int L[9];//make it full size+1, actually max used is 8
	unsigned int key_A;
	unsigned int key_B;
	unsigned int key_i;
	unsigned int key_j;

	char keyword[30];


/*******************************************************
		new part for command arguments
*******************************************************/


	if (argc != 3){
		printf("\nWrong! There should be 3 arguments.\n");
		return 0;
	}


	if((input =fopen(argv[1],"r"))==NULL) {
		printf("\nFile Open Failed\n");
		return 0;

	}
	fscanf(input, "%s", keyword);

	if (strcmp(keyword, "Encryption") == 0){
		usage = 0;
	}
	else if (strcmp(keyword, "Decryption") == 0){
		usage  = 1;
	}



/***************************************************************

 ****  Part 2, make sure input the right file and command   ***

 **************************************************************/
/*
	printf("\nPlease choose the usage of the project\nenter 0 for encryption\nenter 1 for decryption\n\n");
	scanf("%d",&usage);

	while((usage!=0)&&(usage!=1)){
		printf("\n\nYou have input a wrong number, only 0 or 1 is allowed, please input it again\n]n");
		scanf("%d",&usage);
	}
	if(usage==0)
		printf("\nThe input is 0, to encrypt message\n");

	else
		printf("\nThe input is 1, to decrypt message\n");

	printf("\nPlease input the name of the input file:\n\n");

	scanf("%s",&inputfile);

	if((input =fopen(inputfile,"r"))==NULL) {
		printf("\nFile Open Failed\n");
		return 0;

	}
*/

/***************************************************************

 ****  Part 3, To implement the Encryption/Decryption        ***

 **************************************************************/

	fscanf(input, "%s",  text_type);

	if(usage==0){
		if (strcmp(text_type, "plaintext:") != 0){
			printf("\nError, there should be \"plaintext:\" here in input file\n");
			fclose(input);
			return 0;
		}
	}
	else{

		if (strcmp(text_type, "ciphertext:") != 0){
			printf("\nError, there should be \"ciphertext:\" here in input file\n");
			fclose(input);
			return 0;
		}
	}

	A=0x00000000;
	B=0x00000000;
	C=0x00000000;
	D=0x00000000;

	for(i=0;i<(W/8);i++){
		fscanf(input, "%x", &text);
		A=A|(text<<(i*8));
	}
	for(i=0;i<(W/8);i++){
		fscanf(input, "%x", &text);
		B=B|(text<<(i*8));
	}
	for(i=0;i<(W/8);i++){
		fscanf(input, "%x", &text);
		C=C|(text<<(i*8));
	}
	for (i=0;i<(W/8);i++){
		fscanf(input, "%x", &text);
		D=D|(text<<(i*8));
	}

	fscanf(input, "%s", text_type);

	if(usage==0){
		if (strcmp(text_type, "userkey:") != 0){
			printf("Cannot find out keyword \"userkey\", your format does not match.\n");
			fclose(input);
			return 0;
		}/*end of if*/
	}

	else{

		if (strcmp(text_type, "userkey:") != 0){
			printf("Cannot find out keyword \"userkey\", your format does not match.\n");
			fclose(input);
			return 0;
		}/*end of if*/
	}/*end of else*/


	for(i=0;i<9;i++){

		L[i]=0x00000000;

	}

	while ((fscanf(input, "%x", &text) == 1)&&(keybit<=256)){
		c=b/4;
		L[c]=(L[c]|(text<< (b%(W/8)* 8)));
		b++;
		keybit=keybit+8;
	}

	CC=c+1;
	if(keybit>256)
		printf("\n\nNotice, the input key exceeds 256bits\nbut this projects ignores the rest\n]n");
	else
		printf("\nKey size is %d\n\n",CC*32);

	fclose(input);

	/*************************
	 *      Key schedule     *
	**************************/

	S[0]=Pw;

	for(i=1;i<SIZE_S;i++){

                S[i]=S[i-1]+Qw;
	}

	key_A=key_B=key_i=key_j=0;


	v=3*max(CC,SIZE_S);
	for (i=1;i<=v;i++){

		key_A=S[key_i]=rotate_l((S[key_i]+key_A+key_B),3);
		key_B=L[key_j]=rotate_l((L[key_j]+key_A+key_B),(key_A+key_B));
		key_i=(key_i+1)%(SIZE_S);
		key_j=(key_j+1)%(CC);
       	 }

	/*************************
	 *      Encryption       *
	**************************/
	if(usage==0){
		B=B+S[0];
		D=D+S[1];

		for(r=1;r<=R;r++){/*do 20 rounds*/

			t=rotate_l((B*(2*B+1)), LW);
			u=rotate_l((D*(2*D+1)), LW);
			A=rotate_l((A^t),(u & 0x1f))+S[2*r];
			C=rotate_l((C^u),(t & 0x1f))+S[2*r+1];
			temp=A;
			A=B;
			B=C;
			C=D;
			D=temp;
		}

		A=A+S[2*R+2];
		C= C+S[2*R+3];

	}/*end of if: usage==0*/

	/*************************
	 *      Decryption       *
	**************************/
	else{
		C=C-S[2*R+3];
		A=A-S[2*R+2];

		for(r=R;r>=1;r--){
			temp=D;
			D=C;
			C=B;
			B=A;
			A=temp;

			u=rotate_l((D*(2*D+1)),LW);
			t=rotate_l((B*(2*B+1)), LW);
			C=(rotate_r((C-S[2*r+1]), (t & 0x1f))^u);
			A=(rotate_r((A-S[2*r]), (u & 0x1f))^t);
		}
		D=D-S[1];
		B=B-S[0];

	}/*end of else, this is decryption*/

/**********************************************************
			output
**********************************************************/

	if((output=fopen(argv[2],"w"))==NULL) {
		printf("\nFailed to open output file\n");
		exit(-1);
	}

	if(usage==0){
		fprintf(output, "ciphertext: ");
		printf("The ciphertext is:\n\n");

	}

	else{
		fprintf(output, "plaintext: ");
		printf( "The plaintext is:\n\n");

	}

	for (i=0;i<(W/8);i++){
		fprintf(output, "%.2x ", (A&(0xff<<(i*8))) >> (i*8));
		printf("%.2x ", (A&(0xff<<(i*8))) >> (i*8));
	}
	//fprintf(output, "\n");

	for (i=0;i<(W/8);i++){
		fprintf(output, "%.2x ", (B&(0xff<<(i*8))) >> (i*8));
		printf("%.2x ", (B&(0xff<<(i*8))) >> (i*8));

	}
	//fprintf(output, "\n");
	for (i=0;i<(W/8);i++){
		fprintf(output, "%.2x ", (C&(0xff<<(i*8))) >> (i*8));
		printf("%.2x ", (C&(0xff<<(i*8))) >> (i*8));

	}
	//fprintf(output, "\n");
	for (i=0;i<(W/8);i++){
		fprintf(output, "%.2x ", (D&(0xff<<(i*8))) >> (i*8));
		printf("%.2x ", (D&(0xff<<(i*8))) >> (i*8));

	}
	//fprintf(output, "\n");

	fclose(output);

	printf("\n\nSucceed! The out put is also stored in %s\n\n",argv[2]);

	return 0;
}
