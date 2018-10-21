
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

/// Cross-sectional area of a wire (10^(-3) ñm^2)
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


int calculation ();
void ZeroArray (float array[], int size);
void Read (float U[], float I[], int num, FILE* input);
float ERRORofRes(float L, float S, float avgU, float avgI, float avgRes);


//{============================================================================
/*! \function
	Main function.
*/
//}============================================================================

int main ()
    {
        printf("# Resistivity calculation v2.0\n");
        printf("# (c) MXMSHKT 10/2018\n\n\n");

        printf (" LABA 1.1.1 - measurement of resistivity \n");
        int correct = calculation ();
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
    *\param avgRes average resistivity (*10^(-3) Om*cm^2\)
*/
//}============================================================================

int calculation ()
    {
        int size = NMeas3;
        float U[size], I[size];

        ZeroArray (U, size);
        ZeroArray (I, size);

        FILE*  input = fopen ("laba1.txt",  "r");
        FILE*  output = fopen ("labka.txt", "w");

        if (input == NULL)
            {
                printf (" File \\laba1.txt\\ wasn't opened\n");
                return (-1);
            }
        assert(output != NULL);

        int num = NMeas3;

        Read (U, I, num, input);
        for (int line = 0; line < NMeas3; line++)
            {
                if ((U[line] <= 0) || (I[line] <= 0) || (U[line] > Umax) || (I[line] > Imax))
                    {
                        return (line + 1);
                    }
            }

        int m = 0;
        float SumI1 = 0;
        float SumU1 = 0;
        float SumRes1 = 0;
        fprintf (output, " Resistivity for %dcm\n", L1);

//-----------------------------------------------------------------------------
        for (int i = 1; i <= NMeas1; i++)
            {
                assert  ((I[i] * L1) != 0);
                float resistivity = (U[i] * S) / (I[i] * L1);
                m++;
                SumI1 += I[i];
                SumU1 += U[i];
                SumRes1 += resistivity;
                fprintf (output, "%d)  ", i);
                fprintf (output, "%4.2f", resistivity);
                fprintf (output, " * 10^(-3) Om*cm^2\n");
            }
        float avgU1 = SumU1 / m;
        float avgI1 = SumI1 / m;
        float avgRes1 = SumRes1 / m;

        fprintf(output, "\n");
        float ERRORofRes1 = ERRORofRes(L1, S, avgU1, avgI1, avgRes1);
        fprintf (output, " error of resistivity\n");
        fprintf(output, "%4.5f\n", ERRORofRes1);

        m = 0;
        float SumI2 = 0;
        float SumU2 = 0;
        float SumRes2 = 0;
        fprintf (output, "\n Resistivity for %dcm\n", L2);

//-----------------------------------------------------------------------------
        for (int i = 13; i <= NMeas2; i++)
            {
                assert  ((I[i] * L2) != 0);
                float resistivity = (U[i] * S) / (I[i] * L2);
                m++;
                SumI2 += I[i];
                SumU2 += U[i];
                SumRes2 += resistivity;
                fprintf (output, "%d)  ", i);
                fprintf (output, "%4.2f", resistivity);
                fprintf (output, " * 10^(-3) Om*cm^2\n");
            }
        float avgU2 = SumU2 / m;
        float avgI2 = SumI2 / m;
        float avgRes2 = SumRes2 / m;

        fprintf(output, "\n");
        float ERRORofRes2 = ERRORofRes(L2, S, avgU2, avgI2, avgRes2);
        fprintf (output, " error of resistivity\n");
        fprintf(output, "%4.5f\n", ERRORofRes2);

        m = 0;
        float SumI3 = 0;
        float SumU3 = 0;
        float SumRes3 = 0;
        fprintf (output, "\n Resistivity for %dcm\n", L3);

//-----------------------------------------------------------------------------
        for (int i = 25; i <= NMeas3; i++)
            {
                assert  ((I[i] * L3) != 0);
                float resistivity = (U[i] * S) / (I[i] * L3);
                m++;
                SumI3 += I[i];
                SumU3 += U[i];
                SumRes3 += resistivity;
                fprintf (output, "%d) ", i);
                fprintf (output, "%4.2f", resistivity);
                fprintf (output, " * 10^(-3) Om*cm^2\n");
            }
        float avgU3 = SumU3 / m;
        float avgI3 = SumI3 / m;
        float avgRes3 = SumRes3 / m;

        fprintf(output, "\n");
        float ERRORofRes3 = ERRORofRes(L3, S, avgU3, avgI3, avgRes3);
        fprintf (output, " error of resistivity\n");
        fprintf(output, "%4.5f\n", ERRORofRes3);

        fclose (input);
        fclose (output);

        return (0);
    }


//{============================================================================
/*!
    \function
	Function calculates resistivity measurement error.
    \return returns resistivity error.
*/
//}============================================================================

float ERRORofRes(float L , float S, float avgU , float avgI , float avgRes)
    {
        float errorofL = 0.01;
        float errorofS = 0.01;
        float errorofU = 0.05;
        float errorofI = 0.01;

        float ffL = (errorofL * L) * (errorofL * L);
        float ffS = (errorofS * S) * (errorofS * S);
        float ffU = (errorofU * avgU) * (errorofU * avgU);
        float ffI = (errorofI * avgI) * (errorofI * avgI);

        ERRORofRes = avgRes * sqrt(ffL + ffS + ffU + ffI);
        return (ERRORofRes);
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

void Read (float U[], float I[], int num, FILE* input)
    {
        for (int line = 0; line < num; line++)
            {
                assert ((0 <= line) && (line <= num));
                fscanf (input, "%f %f", &U[line], &I[line]);
            }
    }
