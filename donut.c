// .,-~:;=!*#$@
//
// \033[A   --> the escape character to go up a line
//
// the depth characters in ascending order
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <unistd.h>

const char* chars = ".,-~:;=!*#$@\0";

int clamp_i(int a, int mn, int mx)
{
    if (a < mn) return mn;
    else if (a > mx) return mx;
    return a;
}

double power(double base, int exp)
{
    double ans = 1;
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

int is_on_donut(double x, double y, double z, int r, int R)
{
    double lhs = power(x*x + y*y + z*z + R*R - r*r, 2);
    double rhs = 4*R*R*(x*x + y*y);
    // printf("%d, %d, %d : %d and %d \n", x,y,z,lhs,rhs);
    return fabs(rhs-lhs) <= 70000;
}



void _print_2D_donut(int rx, int ry, int rz)
{
    // int R = 12; // the outer bigger radius
    // int r = 6; // the inner radius
    int R = 24, r = 15;
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
                    // x remains x
                    // y -> y cos theta + z sin theta
                    // z -> -y sin theta + z cos theta
                    //
                    // x is j, y is i, and z is k

                    // rotate about x axis first
                    double x = j;
                    double y = cos(theta)*i + sin(theta)*k;
                    double z = -sin(theta)*i + cos(theta)*k;

                    double ox = x;
                    double theta_y = 2 * theta;

                    // then rotate about y axis next
                    x = cos(theta_y)*ox + sin(theta_y)*z;
                    y = y;
                    z = -sin(theta_y)*ox + cos(theta_y)*z;

                    double oy = y; ox = x;
                    double theta_z = 4 * theta;
                    // then rotate about z axis too
                    x = cos(theta_z)*ox + sin(theta_z)*oy;
                    y = -sin(theta_z)*ox + cos(theta_z)*oy;
                    z = z;

                    c = chars[clamp_i((k+rz/2)/rem,0,11)];

                    if (is_on_donut(x, y, z, r, R)) {
                        // if (donut[i+ry/2][j+rx/2] == ' ') {
                            donut[i+ry/2][j+rx/2] = c;
                        // }
                    }
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
    // int range_x = 50, range_y = 50, range_z = 50;
    int range_x = 80, range_y = 80, range_z = 80;
    printf("DONUT\n\n");
    _print_screen(range_x, range_y);
    _print_2D_donut(range_x, range_y, range_z);
    return 0;
}
