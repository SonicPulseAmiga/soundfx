# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>

float clip (float byte) {
    if (byte>127) byte = 127;
    if (byte<-128) byte = -128;
    return byte;
}

main (int argc, char *argv[]) {
    FILE *fp;
    FILE *fpii;
    char ime[80];
    float input, high, mid, low, freq, f, fs, recq, q;
    char a;
    if (argc!=2) {
        printf ("Usage: rez <infile>\n");
        exit(0);
    }
    if ((fp=fopen(argv[1],"r"))==NULL) {
        printf ("Cannot open file %s.\n",argv[1]);
        exit (0);
    }
    strcpy(ime,argv[1]);
    strcat(ime,".rez");
    if ((fpii=fopen(ime,"w"))==NULL) {
        printf ("Cannot create file %s.\n",argv[1]);
    }
    printf ("Enter the desired cutoff (0-100): ");
    scanf ("%f", &f);
    freq = (f*1.25/100);
    printf ("%f", freq);
    printf ("Enter the desired resonance: ");
    scanf ("%f", &q);
    recq = 1/q;
    high = 0; low = 0; mid = 0;
    while (!feof(fp)) {
        a=fgetc(fp);
        input=a;
        high = (input - mid * recq - low);
        high = clip(high);
        mid += (high * freq);
        mid = clip (mid);
        low += (mid * freq);
        low = clip (low);
        fputc((char)low,fpii);
        }
}
