//{============================================================================
/*!
\file
\brief
This program is aimed to solve all cases of square polynomial equations
with a single variable(univariate),
including solutions with rational and irrational roots as well as real and imaginary numbers.
\author Max Shkut ; Github: https://github.com/MXMSHKT
*/
//}============================================================================

#include <stdio.h>
#include <assert.h>
#include <math.h>

int SolveSquare(double a, double b, double c, double *x1, double *x2);
int main ()
    {

        printf("# Square equation solver v0.1\n");
        printf("# (c) MXMSHKT 10/2018\n\n\n");
        printf ("# Enter the coefficients of the quadratic equation\n");

        double a = 0, b = 0, c = 0;
        double x1, x2;

        scanf("%lg%lg%lg", &a, &b, &c);

        assert(!(isnan(a) || isnan(b) || isnan(c)));

        int nRoots = SolveSquare (a, b, c, &x1, &x2);

        switch (nRoots)
        {

            case -1:
                printf("# Equation has no solutions in real numbers.");
                printf("\n");
                break;

            case 0:
                printf("# Equation has 2 solutions in real numbers:\n");
                printf("# X1= ");
                printf("%0.4lg", x1);
                printf(", X2= ");
                printf("%0.4lg", x2);
                printf("\n");
                break;

            case 1:
                printf("# Equation has infinite solutions in real numbers.");
                printf("\n");
                break;

            case 2:
                printf("# Equation has 1 solution in real numbers.\n");
                printf("# X= ");
                printf("%0.4lg", x1);
                printf("\n");
                break;

            default:
                printf("Calculation error.");
                printf("\n");
                break;
        }
    return 0;
    }

//{============================================================================
/*!
    *Solves square equation of common type (ax^2 + bx + c = 0) in real numbers.
    *\param a Coefficient before x^2
    *\param b Coefficient before x
    *\param c Free coefficient
    *\param x1 Reference to the first solution variable
    *\param x2 Reference to the second solution variable
    *\return -1 no solutions in real numbers
    *\return 0  Equation has 2 solutions
    *\return 1  Equation has infinite solutions in real numbers
    *\return 2  Equation has 1 solution in real numbers
*/
//}============================================================================

int SolveSquare(double a, double b, double c, double *x1, double *x2)
    {
        assert(x1!=NULL);
        assert(x2!=NULL);
        assert(!isnan(a));
        assert(!isnan(b));
        assert(!isnan(c));

        double D = b * b - 4 * a * c;

        if(a == 0)
        {
            if(b == 0 && c == 0)
                 return 1;
            if(b == 0)
                return -1;
        }

        if (c == 0)
        {
            if (a != 0)
            {
                *x1 = -b / a;
                return 2;
            }
            else { return 1; }
        }

        if ( D < 0)
        {
            return 1;
        }

        if (D == 0)
        {
            *x1 = -b / (2 * a);
            return 2;
        }

        if (D > 0)
        {
            *x1 = (-b + sqrt(D)) / (2 * a);
            *x2 = (-b - sqrt(D)) / (2 * a);
            return 0;
        }
    }
