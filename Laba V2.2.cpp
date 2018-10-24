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

/// Cross-sectional area of a wire (10^(-3) сm^2)
const int S = 1;
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
/// Minimum value of amperage (A)
const double Imin = 0;
/// Maximum value of voltage (V)
const double Umax = 600;
/// Minimum value of voltage (V)
const double Umin = 0;




int Calculation ();
void ZeroArray (float array[], int size);
int Read (float U[], float I[], int num, char* name);
float ErrorOfRes(float L, float S, float* avgU, float* avgI, float* avgres);
int ErrorinLine( float U[], float I[]);
int Count( int z, int L, float U[],int NMeas, int S, float I[], FILE* output, float* avgU, float* avgI, float* avgres);
void CalculateForL1( float U[], float I[], FILE* input, FILE* output, float* avgU1, float* avgI1, float* avgres1);
void PrintCSV (float U[], float I[], FILE* outputCSV);

//{============================================================================
/*! \function
	Main function.
*/
//}============================================================================

int main ()
    {
        printf("# LABA 1.1.1 - measurement of resistivity v2.1\n");
        printf("# (c) MXMSHKT 10/2018\n\n");

        int correct = Calculation();
        if (correct > 0)
            {
                printf ("ERROR in file in line %d\n",correct);
            }
        if (correct == 0)
            {
                printf (" Data is written to a file \labka.txt\ \n");
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
        float avgU1, avgU2, avgU3, avgI1, avgI2, avgI3, avgres1, avgres2, avgres3;
        int size = NMeas3;
        float U[size], I[size];

        ZeroArray (U, size);
        ZeroArray (I, size);

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

        int Line = ErrorinLine(U, I);
        if (Line > 0)
            return (Line);

        int z = 1;
        Count( z, L1, U, NMeas1, S, I, output, &avgU1, &avgI1, &avgres1);
        float ErrorOfRes1 = ErrorOfRes(L1, S, &avgU1, &avgI1, &avgres1);
        fprintf (output, "\n error of resistivity\n");
        fprintf(output, "%4.5f\n", ErrorOfRes1);

        int y = 13;
        Count( y, L2, U, NMeas2, S, I, output, &avgU2, &avgI2, &avgres2);
        fprintf(output, "\n");
        float ErrorOfRes2 = ErrorOfRes(L2, S, &avgU2, &avgI2, &avgres2);
        fprintf (output, " error of resistivity\n");
        fprintf(output, "%4.5f\n", ErrorOfRes2);

        int t = 25;
        Count( t, L3, U, NMeas3, S, I, output, &avgU3, &avgI3, &avgres3);
        fprintf(output, "\n");
        float ErrorOfRes3 = ErrorOfRes(L3, S, &avgU3, &avgI3, &avgres3);
        fprintf (output, " error of resistivity\n");
        fprintf(output, "%4.5f\n", ErrorOfRes3);

        PrintCSV (U, I, outputCSV);


        fclose (output);
        fclose (outputCSV);

        return (0);
    }


//{============================================================================
/*!
    \function
	Function calculates resistivity measurement error.
    \return returns resistivity error.
*/
//}============================================================================

float ErrorOfRes(float L , float S, float* avgU , float* avgI , float* avgres)
    {
        float errorofL = 0.01;
        float errorofS = 0.01;
        float errorofU = 0.05;
        float errorofI = 0.01;

        float ffL = (errorofL * L) * (errorofL * L);
        float ffS = (errorofS * S) * (errorofS * S);
        float ffU = (errorofU * *avgU) * (errorofU * *avgU);
        float ffI = (errorofI * *avgI) * (errorofI * *avgI);

        float ErrorOfRes = *avgres * sqrt(ffL + ffS + ffU + ffI);
        return (ErrorOfRes);
    }

//=============================================================================

void ZeroArray (float array[], int size)
    {
        for (int i = 0; i < size; i++)
            {
                assert (0 <= i && i < size);
                array[i] = 0;
            }
    }
//=============================================================================

int Read (float U[], float I[], int num, char* name)
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
                fscanf (input, "%f %f", &U[line], &I[line]);
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

int ErrorinLine( float U[], float I[])
    {
        for (int line = 0; line < NMeas3; line++)
            {
                if ((U[line] <= 0) || (I[line] <= 0) || (U[line] > Umax) || (I[line] > Imax))
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

int Count( int z, int L, float U[],int NMeas, int S, float I[], FILE* output, float* avgU, float* avgI, float* avgres)
    {
        int m = 0;
        float SumI = 0;
        float SumU = 0;
        float SumRes = 0;
        fprintf (output, "\nResistivity for %dcm\n", L);

        for (int i = z; i < NMeas; i++)
            {
                assert  ((I[i] * L) != 0);
                float resistivity = (U[i] * S) / (I[i] * L);
                m++;
                SumI += I[i];
                SumU += U[i];
                SumRes += resistivity;
                fprintf (output, "%d)  ", i);
                fprintf (output, "%4.2f", resistivity);
                fprintf (output, " * 10^(-3) Om*cm^2\n");
            }
        *avgU = SumU / m;
        *avgI = SumI / m;
        *avgres = SumRes / m;
        return 0;
    }

void PrintCSV (float U[], float I[], FILE* outputCSV)
	{
        for (int i = 0; i < NMeas3; i++)
            {
                assert ((0 <= i) && (i < NMeas3));
                fprintf (outputCSV,"%5.2f %5.2f\n", U[i], I[i]);
                if ((i == 11) || (i == 23))
                    fprintf (outputCSV,"\n");
            }
	}
