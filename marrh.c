#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#define  PICSIZE 256
#define  MAXMASK 100

     int    pic[PICSIZE][PICSIZE];
     double outpic1[PICSIZE][PICSIZE];
     double outpic2[PICSIZE][PICSIZE];
     int    edgeflag[PICSIZE][PICSIZE];
     double x_mask[MAXMASK][MAXMASK];
     double y_mask[MAXMASK][MAXMASK];
     double conv[PICSIZE][PICSIZE];
     double gradient[PICSIZE][PICSIZE];
     int final1[PICSIZE][PICSIZE];
     int peaks[PICSIZE][PICSIZE];


bool Max_Test(double conv[PICSIZE][PICSIZE], double dir, int i, int j) {

    double val1, val2;

    if ((dir > -.4142) && (.4142 >= dir)) {
        printf("control 1\n");
        val1 = conv[i+1][j];
        val2 = conv[i-1][j];
    }

    else if (dir > .4142 && 2.4142 >= dir) {
        printf("control 2\n");
        val1 = conv[i-1][j-1];
        val2 = conv[i+1][j+1];
    }

    else if (dir > -2.4142 && -.4142 >= dir) {
        printf("control 3\n");
        val1 = conv[i-1][j+1];
        val2 = conv[i+1][j-1];
    }

    else {
        printf("control 4\n");
        val1 = conv[i][j+1];
        val2 = conv[i][j-1];
    }

    printf("check\n");
    if (conv[i][j] > val1 && conv[i][j] > val2)
        return true;

    return false;
}

void Double_threshold( int i, int j, double lo) {

    if (final1[i][j] == 1)
        return;

    else {
        if (conv[i][j] > lo && peaks[i][j] == 1) {

            final1[i][j] = 1;
            peaks[i][j] = 0;

            Double_threshold(i+1, j, lo);
            Double_threshold(i+1, j+1, lo);
            Double_threshold(i, j+1, lo);
            Double_threshold(i+1, j-1, lo);
            Double_threshold(i-1, j, lo);
            Double_threshold(i-1, j+1, lo);
            Double_threshold(i-1, j-1, lo);
            Double_threshold(i, j-1, lo);
        }

        else
            final1[i][j] = 0;
    }

    return;
}

int Hi_calc(double percent, double conv[PICSIZE][PICSIZE], double maxival) {
    double cutoff = percent*65536;
    double pixel;
    double histogram[PICSIZE];

    int i, j;

    for (i = 0; i < PICSIZE; i++) {
        histogram[i] = 0;
    }

    for (i = 0; i < PICSIZE; i++) {
        for(j = 0; j < PICSIZE; j++) {
            pixel = (conv[i][j] / maxival) * 255;
            histogram[(int) pixel]++;
        }
    }

    int sum = 0;
    i = 0;
    while (sum < cutoff) {

        sum += histogram[i];
        i++;
    }

    return i;
}

main(argc,argv)
int argc;
char **argv;
{
    int     i,j,p,q,s,t,mr,centx,centy, hi, lo;
    double  y_maskval, x_maskval, y_sum, x_sum,sig,maxival,minival,maxval,ZEROTOL, percent;
    FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
    char    *foobar;

    argc--; argv++;
    foobar = *argv;
    fp1=fopen(foobar,"rb");

    argc--; argv++;
    foobar = *argv;
    fo1=fopen(foobar,"wb");


    argc--; argv++;
    foobar = *argv;
    fo2=fopen(foobar,"wb");

    argc--; argv++;
    foobar = *argv;
    fo3=fopen(foobar,"wb");

    argc--; argv++;
    foobar = *argv;
    sig = atof(foobar);

    argc--; argv++;
    foobar = *argv;
    percent = atof(foobar);

    mr = (int)(sig * 3);
    centx = (MAXMASK / 2);
    centy = (MAXMASK / 2);

    char c;

    for(i = 0; i < 15; i++) {
        c = getc(fp1);
    }

    for (i=0;i<256;i++) {
        for (j=0;j<256;j++) {

            pic[i][j]  =  getc (fp1);
        }
    }

    for (p=-mr;p<=mr;p++) {
        for (q=-mr;q<=mr;q++) {

            x_maskval = p*
                      (exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));

            x_mask[p+centy][q+centx] = x_maskval;

            y_maskval = q*
                      (exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));

            y_mask[p+centy][q+centx] = y_maskval;
        }
    }

    maxival = 0;

    for (i=mr;i<=255-mr;i++) {
        for (j=mr;j<=255-mr;j++) {

            x_sum = 0;
            y_sum = 0;
            for (p=-mr;p<=mr;p++) {
                for (q=-mr;q<=mr;q++) {

                    x_sum += pic[i+p][j+q] * x_mask[p+centy][q+centx];
                    y_sum += pic[i+p][j+q] * y_mask[p+centy][q+centx];
                }
             }

             //outpic1[i][j] = sum;
             conv[i][j] = sqrt(x_sum*x_sum + y_sum*y_sum);

             if (x_sum == 0.0)
                x_sum = 0.000001;

             gradient[i][j] = y_sum/x_sum;

             if (conv[i][j] > maxival)
                maxival = conv[i][j];
        }
    }

    fprintf(fo1,"P5\n");
    fprintf(fo1,"256 256\n");
    fprintf(fo1,"255\n");

    fprintf(fo2,"P5\n");
    fprintf(fo2,"256 256\n");
    fprintf(fo2,"255\n");

    fprintf(fo3,"P5\n");
    fprintf(fo3,"256 256\n");
    fprintf(fo3,"255\n");


    for (i=0;i<256;i++) {
        for (j=0;j<256;j++) {

            //conv[i][j] = (conv[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)((int)(conv[i][j])));

            if (Max_Test(conv, gradient[i][j], i, j)) {
                fprintf(fo2, "%c", ((char)255));
                peaks[i][j] = 1;
            }

            else {
                fprintf(fo2, "%c", ((char)0));
                peaks[i][j] = 0;
            }


        }
    }

    hi = Hi_calc(percent, conv, maxival);
    printf("hi is %d\n", hi);
    lo = .35*hi;

    for (i = 0; i < PICSIZE; i++) {
        for (j = 0; j < PICSIZE; j++) {

            if (peaks[i][j] == 1 && conv[i][j] > hi) {

                    final1[i][j] = 1;
                    //peaks[i][j] = 0;

                    Double_threshold(i+1, j, lo);
                    Double_threshold(i+1, j+1, lo);
                    Double_threshold(i, j+1, lo);
                    Double_threshold(i+1, j-1, lo);
                    Double_threshold(i-1, j, lo);
                    Double_threshold(i-1, j+1, lo);
                    Double_threshold(i-1, j-1, lo);
                    Double_threshold(i, j-1, lo);


                }

                //else if (conv[i][j] < lo)
                    //final1[i][j] = 0;
        }
    }

    for (i = 0; i < PICSIZE; i++) {
        for(j = 0; j < PICSIZE; j++) {

            if (final1[i][j] == 1)
                fprintf(fo3, "%c", ((char)255));

            else
                fprintf(fo3, "%c", ((char)0));
        }
    }
}
