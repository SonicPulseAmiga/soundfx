#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define POINTS 5
#define STEP (1.0/(double)(POINTS-1))

double voll[POINTS][POINTS],volr[POINTS][POINTS];
double volv[POINTS][POINTS],volh[POINTS][POINTS];
double vols[POINTS][POINTS];

void calc_speaker_audio(void) {
	int i,j;
	double lr,rl,vh,hv,v;

	for(i=0,vh=1.0;i<POINTS;vh-=STEP,i++) {
		hv=1.0-vh;
		for(j=0,lr=1.0;j<POINTS;lr-=STEP,j++) {
			rl=1.0-lr;

			voll[j][i]=(vh<0.5)?((lr-0.5)+vh):((lr-0.5)+hv);
			if(voll[j][i]<0.0) voll[j][i]=0.0;

			volr[j][i]=(vh<0.5)?((0.5-lr)+vh):((0.5-lr)+hv);
			if(volr[j][i]<0.0) volr[j][i]=0.0;

			volv[j][i]=(lr<0.5)?((vh-0.5)+lr):((vh-0.5)+rl);
			if(volv[j][i]<0.0) volv[j][i]=0.0;

			volh[j][i]=(lr<0.5)?((0.5-vh)+lr):((0.5-vh)+rl);
			if(volh[j][i]<0.0) volh[j][i]=0.0;

			v=voll[j][i]+volr[j][i]+volv[j][i]+volh[j][i];				// normalise
			voll[j][i]/=v;volr[j][i]/=v;volv[j][i]/=v;volh[j][i]/=v;

			vols[j][i]=voll[j][i]+volr[j][i]+volv[j][i]+volh[j][i];
		}
	}
}

void calc_speaker_video(void) {
	int i,j;
	double lr,rl,vh,hv,v;

	for(i=0,vh=1.0;i<POINTS;vh-=STEP,i++) {
		hv=1.0-vh;
		for(j=0,lr=1.0;j<POINTS;lr-=STEP,j++) {
			rl=1.0-lr;

			voll[j][i]=(lr-0.5)+((vh-0.5)-rl);
			if(voll[j][i]<0.0) voll[j][i]=0.0;

			volr[j][i]=(rl-0.5)+((vh-0.5)-lr);
			if(volr[j][i]<0.0) volr[j][i]=0.0;

			volv[j][i]=(lr>0.5)?vh-((lr-0.5)*2):vh-((rl-0.5)*2);
			if(volv[j][i]<0.0) volv[j][i]=0.0;

			volh[j][i]=hv;
			if(volh[j][i]<0.0) volh[j][i]=0.0;

			v=voll[j][i]+volr[j][i]+volv[j][i]+volh[j][i];				// normalise
			voll[j][i]/=v;volr[j][i]/=v;volv[j][i]/=v;volh[j][i]/=v;

			vols[j][i]=voll[j][i]+volr[j][i]+volv[j][i]+volh[j][i];
		}
	}
}

void display_speaker(double vol[POINTS][POINTS]) {
	int i,j;

	for(i=0;i<POINTS;i++) {
		printf("\t");
		for(j=0;j<POINTS;j++) {
			printf("%4.2lf ",vol[j][i]);
		}
		printf("\n");
	}
}

void main(int argc,char **argv) {

	printf("panning with stepsize=%3.2lf\n",STEP);

	calc_speaker_audio();
	//calc_speaker_video();

	puts("left"); display_speaker(voll);
	puts("right");display_speaker(volr);
	puts("front");display_speaker(volv);
	puts("back"); display_speaker(volh);
	puts("sum");  display_speaker(vols);
	puts("ready");
}
