# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>

long clip (long byte) {
    byte = (byte>>8);
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
    long inputl, highl, lowl, midl, recql, freql;
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
    freql = (f*320/100);
    printf ("%d", freql);
    printf ("Enter the desired resonance: ");
    scanf ("%f", &q);
    recql = 256/q;
    printf ("%d", recql);
    highl = 0; lowl = 0; midl = 0;
    while (!feof(fp)) {
        a=fgetc(fp);
/*        input=a;
        high = (input - mid * recq - low);
        high = clip(high);
        mid += (high * freq);
        mid = clip (mid);
        low += (mid * freq);
        low = clip (low);*/
        inputl = a;
        highl = ((inputl<<8) - midl * recql - (lowl<<8));
        highl = clip(highl);
        midl = (midl<<8) + (highl * freql);
        midl = clip(midl);
        lowl = (lowl<<8) + (midl * freql);
        lowl = clip(lowl);
        fputc((char)lowl,fpii);
        }
}
