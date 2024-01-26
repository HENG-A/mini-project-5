#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <memory.h>

#define FS 48000.0f
#define FL 1500.0f
#define FH 3500.0f
#define PI 3.141592653589793f
#define START_SAMPLE 962880
#define END_SAMPLE 964880

typedef struct _wav {
	int fs;
	char header[44];
	size_t length;
	short *LChannel;
	short *RChannel;
} wav;

int wav_read_fn(char *fn, wav *p_wav)
{
	//char header[44];
	short temp = 0;
	size_t i = 0;

	FILE *fp = fopen(fn, "rb");
	if(fp==NULL) {
		fprintf(stderr, "cannot read %s\n", fn);
		return 0;
	}
	fread(p_wav->header, sizeof(char), 44, fp);
	while( !feof(fp) ) {
		fread(&temp, sizeof(short), 1, fp);
		i++;
	}
	p_wav->length = i / 2;
	p_wav->LChannel = (short *) calloc(p_wav->length, sizeof(short));
	if( p_wav->LChannel==NULL ) {
		fprintf(stderr, "cannot allocate memory for LChannel in wav_read_fn\n");
		fclose(fp);
		return 0;
	}
	p_wav->RChannel = (short *) calloc(p_wav->length, sizeof(short));
	if( p_wav->RChannel==NULL ) {
		fprintf(stderr, "cannot allocate memory for RChannel in wav_read_fn\n");
		fclose(fp);
		return 0;
	}
	fseek(fp, 44, SEEK_SET);
	for(i=0;i<p_wav->length;i++) {
		fread(p_wav->LChannel+i, sizeof(short), 1, fp);
		fread(p_wav->RChannel+i, sizeof(short), 1, fp);
	}
	fclose(fp);
	return 1;
}

int wav_save_fn(char *fn, wav *p_wav)
{
	FILE *fp = fopen(fn, "wb");
	size_t i;
	if(fp==NULL) {
		fprintf(stderr, "cannot save %s\n", fn);
		return 0;
	}
	fwrite(p_wav->header, sizeof(char), 44, fp);
	for(i=0;i<p_wav->length;i++) {
		fwrite(p_wav->LChannel+i, sizeof(short), 1, fp);
		fwrite(p_wav->RChannel+i, sizeof(short), 1, fp);
	}
	fclose(fp);
	return 1;
}

int wav_init(size_t length, wav *p_wav)
{
	p_wav->length = length;
	p_wav->LChannel = (short *) calloc(p_wav->length, sizeof(short));
	if( p_wav->LChannel==NULL ) {
		fprintf(stderr, "cannot allocate memory for LChannel in wav_read_fn\n");
		return 0;
	}
	p_wav->RChannel = (short *) calloc(p_wav->length, sizeof(short));
	if( p_wav->RChannel==NULL ) {
		fprintf(stderr, "cannot allocate memory for RChannel in wav_read_fn\n");
		return 0;
	}
	return 1;
}

void wav_free(wav *p_wav)
{
	free(p_wav->LChannel);
	free(p_wav->RChannel);
}

/* hamming: for n=0,1,2,...N, length of N+1 */
float hamming(int N, int n)
{
	return 0.54 - 0.46 * cosf(2*PI*((float)(n))/((float)N));
}

/* low-pass filter coef: n=0,1,2...,2M */
float low_pass(int m, int n)
{
	float wh = 2*PI*FH/FS;
  float wl = 2*PI*FL/FS;
	if(n==m) {// L'Hopital's Rule
		return 2.0*(wh/PI - wl/PI);
	}
	else {
		return 2.0*(sinf(wh*((float)(n-m)))-sinf(wl*((float)(n-m))))/PI/((float)(n-m)) * hamming(2*m+1, n);
	}
}

float band_stop(int m, int n)
{
	float wh = 2*PI*FH/FS;
  float wl = 2*PI*FL/FS;
	if(n==m) {// L'Hopital's Rule
		return 1.0 - 1.0*(wh/PI - wl/PI);
	}
	else {
		//printf("filter %f\n",sinf(PI*((float)(n-m)))/PI/((float)(n-m)) - 2.0*(sinf(wh*((float)(n-m)))-sinf(wl*((float)(n-m))))/PI/((float)(n-m)) * hamming(2*m+1, n));
		return sinf(PI*((float)(n-m)))/PI/((float)(n-m)) - 1.0*(sinf(wh*((float)(n-m)))-sinf(wl*((float)(n-m))))/PI/((float)(n-m)) * hamming(2*m+1, n);
	}
}

float band_pass(int m, int n)
{
	float wh = 2*PI*FH/FS;
  float wl = 2*PI*FL/FS;
	if(n==m) {// L'Hopital's Rule
		return 2.0*(wh/PI - wl/PI);
	}
	else {
		//printf("filter %f\n",2.0*(sinf(wh*((float)(n-m)))-sinf(wl*((float)(n-m))))/PI/((float)(n-m)) * hamming(2*m+1, n));
		return 2.0*(sinf(wh*((float)(n-m)))-sinf(wl*((float)(n-m))))/PI/((float)(n-m)) * hamming(2*m+1, n);
	}
}



int main(int argc, char **argv)
{
	int M;
	M = atoi(argv[1]);
	wav wavin;
	wav wavout;
    float h_L[2*M+1];
    float h_R[2*M+1];

    // 初始化
    for (int i = 0; i < 2*M+1; ++i) {
        h_L[i] = 0.0;
        h_R[i] = 0.0;
    }

	int n = 0;
	float y = 0;
	int k;


	// read wav
	if( wav_read_fn(argv[6], &wavin) == 0 ) {
		fprintf(stderr, "cannot read wav file %s\n", argv[6]);
		exit(1);
	}

	//輸出參數檔案
	FILE *file_hL = fopen(argv[2], "w");
  if (file_hL == NULL) {
      printf("無法打開檔案 %s\n", argv[2]);
      return 1;
  }

	FILE *file_hR = fopen(argv[3], "w");
	if (file_hR == NULL) {
      printf("無法打開檔案 %s\n", argv[3]);
      return 1;
  }

	FILE *file_YL = fopen(argv[4], "w");
  if (file_YL == NULL) {
      printf("無法打開檔案 %s\n", argv[4]);
      return 1;
  }

	FILE *file_YR = fopen(argv[5], "w");
	if (file_YR == NULL) {
      printf("無法打開檔案 %s\n", argv[5]);
      return 1;
  }



	// construct low-pass filter
	for(n=0;n<(2*M+1);n++) {
		h_L[n] = band_pass(M, n);
		fprintf(file_hL, "%24.16e\n", h_L[n]);
    h_R[n] = band_stop(M, n);
		fprintf(file_hR, "%24.16e\n", h_R[n]);
	}

    /*
	for(n=0;n<(2*M+1);n++) {
		fprintf(stdout, "%.15f\n", h[n]);
	}
    */

	// filtering (convolution)
	if( wav_init(wavin.length, &wavout)==0 ) {
		exit(1);
	}

	for(n=0;n<wavin.length;n++) {
		y = 0;
		for(k=0;k<(2*M+1);k++) {
			if( (n-k)>=0 )
				y = y + h_L[k] * ((float)(wavin.LChannel[n-k]));
		}
		wavout.LChannel[n] = (short)(roundf(y));

		y = 0;
		for(k=0;k<(2*M+1);k++) {
			if( (n-k)>=0 )
				y = y + h_R[k] * ((float)(wavin.RChannel[n-k]));
		}
		wavout.RChannel[n] = (short)(roundf(y));
		if(n>=START_SAMPLE&&n<=END_SAMPLE){
			fprintf(file_YL, "%24.16hd\n", wavout.LChannel[n]);
			fprintf(file_YR, "%24.16hd\n", wavout.RChannel[n]);
		}
	}
	memcpy(wavout.header, wavin.header, 44);


	// save wav
	if( wav_save_fn(argv[7], &wavout)==0) {
		fprintf(stderr, "cannot save %s\n", argv[7]);
		exit(1);

	}
	//printf("total length: %d",wavin.length);
	wav_free(&wavin);
	wav_free(&wavout);
	fclose(file_hL);
	fclose(file_hR);
	fclose(file_YL);
	fclose(file_YR);
  return 0;
}
