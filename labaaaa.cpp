#include <stdio.h>
#include <assert.h>

const int S = 1;            // Cross-sectional area of a wire (10^(-3) ñm^2)
const int L1 = 20;          // Length of wire 1 (cm)
const int L2 = 30;          // Length of wire 2 (cm)
const int L3 = 50;          // Length of wire 3 (cm)
const int NMeas1 = 12;      // Number of measurments  (1-12)  for the first Length
const int NMeas2 = 24;      // Number of measurments (13-24) for the second Length
const int NMeas3 = 36;      // Number of measurments  (25-36) for the third Length
const double Imax = 300;    // Maximum value of amperage (A)
const double Imin = 0;      // Minimum value of amperage (A)
const double Umax = 600;    // Maximum value of voltage (V)
const double Umin = 0;      // Minimum value of voltage (V)


int calculation ();
void ZeroArray (float array[], int size);
void Read (float U[], float I[], int num, FILE* input);

int main ()
    {
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
//=============================================================================

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


   int num = NMeas3;

    Read (U, I, num, input);
    for (int line = 0; line < NMeas3; line++)
        {

        if ((U[line] <= 0) || (I[line] <= 0) || (U[line] > Umax) || (I[line] > Imax))
            {
            return (line + 1);
            }
        }

    fprintf (output, " Resistivity for %dcm\n", L1);
    for (int i = 1; i <= NMeas1; i++)
        {

        float resistivity = (U[i] * S) / (I[i] * L1);
        fprintf (output, "%d)  ", i);
        fprintf (output, "%4.2f", resistivity);
        fprintf (output, " * 10^(-3) Om*cm^2\n");

        }


    fprintf (output, "\n Resistivity for %dcm\n", L2);
    for (int i = 13; i <= NMeas2; i++)
        {

        float resistivity = (U[i] * S) / (I[i] * L2);
        fprintf (output, "%d)  ", i);
        fprintf (output, "%4.2f", resistivity);
        fprintf (output, " * 10^(-3) Om*cm^2\n");

        }


    fprintf (output, "\n Resistivity for %dcm\n", L3);
    for (int i = 25; i <= NMeas3; i++)
        {

        float resistivity = (U[i] * S) / (I[i] * L3);
        fprintf (output, "%d)  ", i);
        fprintf (output, "%4.2f", resistivity);
        fprintf (output, " * 10^(-3) Om*cm^2\n");

        }
    fclose (input);
    fclose (output);

    return (0);

    }
//=============================================================================

void ZeroArray (float array[], int size)
    {
    for (int i = 0; i < size; i++)
        {
        assert (0 <= i and i < size);

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



