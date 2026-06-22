// .,-~:;=!*#$@
//
// \033[A   --> the escape character to go up a line
//
// the depth characters in ascending order
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <unistd.h>

const char* chars = ".,-~:;=!*#$@";

long long power(long base, int exp)
{
    long long ans = 1;
    for (int i=0; i<exp; i++) {
        ans *= base;
    }
    return ans;
}

// ---------------------------------

void _print_screen(int rx, int ry)
{
    ry/=2; // had to half the y coordinates as the line gaps are a lot so it looks strecthed 
    for (int i=0; i<ry; i++) {
        for (int j=0; j<rx; j++) {
            printf("*");
        }
        printf("\n");
    }

    return;
}

int is_on_donut(int x, int y, int z, int r, int R)
{
    long long lhs = power(x*x + y*y + z*z + R*R - r*r, 2);
    long long rhs = 4*R*R*(x*x + y*y);
    // printf("%d, %d, %d : %d and %d \n", x,y,z,lhs,rhs);
    return llabs(rhs-lhs) <= 4000;
}

void _print_2D_donut(int rx, int ry, int rz)
{
    int R = 12; // the outer bigger radius
    int r = 6; // the inner radius
    char donut[rx+1][ry+1];

    int rem = rz / 12;

    double theta = 0.01;
    while (1) {
        for (int i=0; i<rx+1; i++) {
            for (int j=0; j<ry+1; j++) {
                donut[i][j] = ' ';
            }
        }
        if (theta > 6.28) theta = 0.00;
        for (int k=-rz/2; k<rz/2; k++) {
            for (int i=-ry/2; i<ry/2; i+=2) {
                for (int j=-rx/2; j<rx/2; j++) {
                    char c = '*';
                    c = chars[(k+rz/2)/rem];
                    // x remains x
                    // y -> y cos theta + z sin theta
                    // z -> -y sin theta + z cos theta
                    int x = j;
                    int y = cos(theta)*i + sin(theta)*k;
                    int z = -sin(theta)*i + cos(theta)*k;
                    if (is_on_donut(x, y, z, r, R)) donut[i+ry/2][j+rx/2] = c;
                }
            }
        }
        for (int i=0; i<rx+1; i+=2) printf("\033[A");
        for (int i=0; i<rx+1; i+=2) {
            for (int j=0; j<ry+1; j++) {
                printf("%c", donut[i][j]);
            }
            printf("\n");
        }
        theta += 0.01;
    }
}

int main(int argc, char* argv[])
{
    int range_x = 50, range_y = 50, range_z = 50;
    printf("DONUT\n\n");
    _print_screen(range_x, range_y);
    _print_2D_donut(range_x, range_y, range_z);
    return 0;
}
