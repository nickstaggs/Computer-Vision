// Nick Staggs
// 8/31/2015
// Robot Vision Fall 2015
// HW1 Pt 1
// Modify Sobel.c to include threshold and run it 3 times on face05
// 1. With magnitude of the gradient 2. Black/white image with liberal threshold
// 3. Black/white image with restrictive threshold

#include <stdio.h>                          /* Sobel.c */
#include <math.h>
#include <stdlib.h>


int pic[256][256];
int outpicx[256][256];
int outpicy[256][256];
int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; // sobel x mask
int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}}; // sobel y mask
double ival[256][256], maxival;

main(argc,argv)
int argc;
char **argv;
{
    int i,j,p,q,mr,sum1,sum2;
    double threshold1, threshold2;
    FILE *fo1, *fo2, *fo3, *fp1, *fopen();
    char *foobar;

    argc--; argv++;
    foobar = *argv;
    fp1=fopen(foobar,"rb"); // argument 1 read file

	argc--; argv++;
	foobar = *argv;
	fo1=fopen(foobar,"wb"); // argument 2 write file

    argc--; argv++;
	foobar = *argv;
	fo2=fopen(foobar,"wb"); // argument 3 write file 2

	argc--; argv++;
	foobar = *argv;
	fo3=fopen(foobar,"wb"); // argument 4 write file 3

    argc--; argv++;
	foobar = *argv;
	threshold1 = atof(foobar); // argument 5 threshold1

	argc--; argv++;
	foobar = *argv;
	threshold2 = atof(foobar); // argument 6 threshold1

    char c;

    for(i = 0; i < 15; i++) {
        c = getc(fp1);
    }
    // something about unsigned char and int not being eqivalent data types, conversion
    for (i=0;i<256;i++) {
        for (j=0;j<(256);j++) {

            pic[i][j]  =  getc (fp1);
            pic[i][j]  &= 0377;
        }
    }

    mr = 1;

    // Scanning convolution in x and y directions, finds out gradient in x and y direction
    for (i=mr;i<256-mr;i++) {
        for (j=mr;j<256-mr;j++) {

            sum1 = 0;
            sum2 = 0;

            for (p=-mr;p<=mr;p++) {

                for (q=-mr;q<=mr;q++) {

                   sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                   sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
            }

            outpicx[i][j] = sum1;
            outpicy[i][j] = sum2;
        }
    }

    maxival = 0;

    // finds magnitude of the gradient
    for (i=mr;i<256-mr;i++) {
        for (j=mr;j<256-mr;j++) {

            ival[i][j] = sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                     (outpicy[i][j]*outpicy[i][j])));
            if (ival[i][j] > maxival)
            maxival = ival[i][j];

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


    // Ensures values are between 0-255 and prints
    for (i=0;i<256;i++) {
        for (j=0;j<256;j++) {

            ival[i][j] = (ival[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)((int)(ival[i][j])));
        }
    }

    for (i=0;i<256;i++) {
        for (j=0;j<256;j++) {

        //printf("truth %d threshold %lf\n", (ival[i][j] > threshold1), threshold1);
        //printf("%f\n", ival[i][j]);

        if (ival[i][j] > threshold1)
            fprintf(fo2,"%c", ((char)(255)));

        else
            fprintf(fo2,"%c", ((char)(0)));

        }
    }


    for (i=0;i<256;i++) {
        for (j=0;j<256;j++) {

        //printf("truth 2 %d threshold %lf\n", (ival[i][j] > threshold2), threshold2);
        //printf("%f\n", ival[i][j]);

        if (ival[i][j] > threshold2)
            fprintf(fo3,"%c",(char)(255));

        else
            fprintf(fo3,"%c",(char)(0));
        }
    }


}
