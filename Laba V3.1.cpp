
//{============================================================================
/*!
\file
\brief
This program reads file with data,calculate resistivity and write result in file and resistivity measurement error.
\author Max Shkut ; Github: https://github.com/MXMSHKT
*/
//}============================================================================

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include<stdlib.h>

/// Cross-sectional area of a wire (10^(-3) сm^2)
const int Square = 1;
/// Length of wire 1 (cm)
const int L1 = 20;
///Length of wire 2 (cm)
const int L2 = 30;
/// Length of wire 3 (cm)
const int L3 = 50;
/// Number of measurments  (1-12)  for the first Length
const int NMeas1 = 12;
/// Number of measurments (13-24) for the second Length
const int NMeas2 = 24;
/// Number of measurments  (25-36) for the third Length
const int NMeas3 = 36;
/// Maximum value of amperage (A)
const double Imax = 300;
/// Minimum value of amperage (A) and voltage (V)
const double Min = 0;
/// Maximum value of voltage (V)
const double Umax = 600;
const double Max = 601;
const double tolerance = 1e-10;


int Calculation ();
int Read (double U[], double I[], int num, char* name);
double ErrorOfRes(int L, int S, double* avgU, double* avgI, double* avgres);
int ErrorinLine( double U[], double I[]);
int Count( int z, int L, double U[],int NMeas, int S, double I[], FILE* output, double* avgU, double* avgI, double* avgres);
void CalculateForL1( double U[], double I[], FILE* input, FILE* output, double* avgU1, double* avgI1, double* avgres1);
void PrintCSV (double U[], double I[], FILE* outputCSV);
int Tolerance(double value);

//{============================================================================
/*! \function
	Main function.
*/
//}============================================================================

int main ()
    {
        printf("# LABA 1.1.1 - measurement of resistivity v3.0\n");
        printf("# (c) MXMSHKT 10/2018\n\n");

        int correct = Calculation();
        if (correct > 0)
            {
                printf ("ERROR in file in line %d\n",correct);
            }
        if (correct == 0)
            {
                printf (" Data is written to a file labka.txt \n");
            }
    }

//{============================================================================
/*!
    \function
	Function reads file with data,calculate resistivity and write result in file.
    *\param U value of voltage (V)
    *\param I value of amperage (A)
    *\param avgU average voltage (V)
    *\param avgI average amperage (A)
    *\param avgres average resistivity (*10^(-3) Om*cm^2\)
*/
//}============================================================================

int Calculation ()
    {
        double avgU1 = 0, avgU2 = 0, avgU3 = 0, avgI1 = 0, avgI2 = 0, avgI3 = 0, avgres1 = 0, avgres2 = 0, avgres3 = 0;

        double *U = (double *) calloc (NMeas3,sizeof(double));
        double *I = (double *) calloc (NMeas3,sizeof(double));

        if(!U || !I)
            {
                printf("Error processing data\n");
                return -1;
            }

        FILE* output = fopen ("labka.txt", "w");
        FILE* outputCSV = fopen ("grafik.csv", "w");

        int num = NMeas3;

        char name[1024]="";
        scanf ("%s", name);

        int check = Read (U, I, num, name);
        if (check == -1)
            return (-1);

        if (output == NULL)
            {
                printf (" File \\labka.txt\\ wasn't opened\n");
                return (-1);
            }

        if (outputCSV == NULL)
            {
                printf (" File \\grafik.csv\\ wasn't opened\n");
                return (-1);
            }

        int Line = ErrorinLine(U, I);
        if (Line > 0)
            return (Line);

        int z = 1;
        Count( z, L1, U, NMeas1, Square, I, output, &avgU1, &avgI1, &avgres1);
        double ErrorOfRes1 = ErrorOfRes(L1, Square, &avgU1, &avgI1, &avgres1);
        fprintf (output, "\n error of resistivity\n");
        fprintf(output, "%4.5f\n", ErrorOfRes1);

        int y = 13;
        Count( y, L2, U, NMeas2, Square, I, output, &avgU2, &avgI2, &avgres2);
        fprintf(output, "\n");
        double ErrorOfRes2 = ErrorOfRes(L2, Square, &avgU2, &avgI2, &avgres2);
        fprintf (output, " error of resistivity\n");
        fprintf(output, "%4.5f\n", ErrorOfRes2);

        int t = 25;
        Count( t, L3, U, NMeas3, Square, I, output, &avgU3, &avgI3, &avgres3);
        fprintf(output, "\n");
        double ErrorOfRes3 = ErrorOfRes(L3, Square, &avgU3, &avgI3, &avgres3);
        fprintf (output, " error of resistivity\n");
        fprintf(output, "%4.5f\n", ErrorOfRes3);

        PrintCSV (U, I, outputCSV);


        fclose (output);
        fclose (outputCSV);

        free(U);
        free(I);

        return (0);
    }

//{============================================================================
/*!
    \function
	Function calculates resistivity measurement error.
    \return returns resistivity error.
*/
//}============================================================================

double ErrorOfRes(int L , int S, double* avgU , double* avgI , double* avgres)
    {
        double errorofL = 0.01;
        double errorofS = 0.01;
        double errorofU = 0.05;
        double errorofI = 0.01;

        double ffL = (errorofL * L) * (errorofL * L);
        double ffS = (errorofS * S) * (errorofS * S);
        double ffU = (errorofU * *avgU) * (errorofU * *avgU);
        double ffI = (errorofI * *avgI) * (errorofI * *avgI);

        double ErrorOfRes = *avgres * sqrt(ffL + ffS + ffU + ffI);
        return (ErrorOfRes);
    }

//=============================================================================

int Read (double U[], double I[], int num, char* name)
    {
        FILE* input = fopen (name,  "r");

        if (input == NULL)
            {
                printf (" File \\laba1.txt\\ wasn't opened\n");
                return (-1);
            }

        for (int line = 0; line < num; line++)
            {
                assert ((0 <= line) && (line <= num));
                fscanf (input, "%lg %lg", &U[line], &I[line]);
            }
        fclose (input);
        return 0;
    }

//{============================================================================
/*!
    \function
	Function find error line in file.
*/
//}============================================================================

int ErrorinLine( double U[], double I[])
    {
        for (int line = 0; line < NMeas3; line++)
            {
                if ((Tolerance(U[line]) == 1) || (Tolerance(I[line]) == 1))
                    {
                        return (line + 1);
                    }
            }
        return 0 ;
    }

//{============================================================================
/*!
    \function
	Function reads file with data,calculate resistivity and write result in file
*/
//}============================================================================

int Count( int z, int L, double U[],int NMeas, int S, double I[], FILE* output, double* avgU, double* avgI, double* avgres)
    {
        int m = 0;
        double SumI = 0;
        double SumU = 0;
        double SumRes = 0;
        fprintf (output, "\nResistivity for %d cm\n", L);

        for (int i = z; i < NMeas; i++)
            {
                assert ((I[i] * L) != 0);
                double resistivity = (U[i] * S ) / (I[i] * L);
                m++;
                SumI += I[i];
                SumU += U[i];
                SumRes += resistivity;
                fprintf (output, "%d)  ", i);
                fprintf (output, "%4.4f", resistivity);
                fprintf (output, " * 10^(-3) Om*cm^2\n");
            }

        *avgU = SumU / m;
        *avgI = SumI / m;
        *avgres = SumRes / m;
        return 0;
    }

//=============================================================================

void PrintCSV (double U[], double I[], FILE* outputCSV)
	{
        for (int i = 0; i < NMeas3; i++)
            {
                assert ((0 <= i) && (i < NMeas3));
                fprintf (outputCSV,"%5.2lg %5.2lg\n", U[i], I[i]);
                if ((i == 11) || (i == 23))
                    fprintf (outputCSV,"\n");
            }
	}

//=============================================================================

int Tolerance(double value)
    {
        if (value < Min - tolerance || value > Max - tolerance)
            return 1;
        if(fabs(value) < tolerance)
            return 1;

        return 0;
    }
