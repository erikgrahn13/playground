/* Exercise 1.7.1
Listing 1.7.1 can be improved. The local variable point is initialized to the first point in the
supplied list. The loop then makes a redundant comparison with that same point. Modify the
code to skip this first test. Include tests to validate the input points argument accordingly.
(Hint: the function should not rely on the caller (main) validating supplied arguments.) */

typedef struct breakpoint {
    double time;
    double value;
} BREAKPOINT;

/* The prototype */
BREAKPOINT maxpoint(const BREAKPOINT* points, long npoints);

/* input: points = array of BREAKPOINTS,
npoints gives the lenth of the array */
/* output: copy of the BREAKPOINT containing largest value */

/* the function definition */
BREAKPOINT maxpoint(const BREAKPOINT* points, long npoints)
{
    int i;
    BREAKPOINT point;

    point.time = 0; /* initialize from first point */
    point.value = 0;
    float previousTime = 0;

    for(i = 0; i < npoints; i++) {
        if(points[i].time > previousTime)
        {
            if(point.value < points[i].value) {
                point.value = points[i].value;
                point.time = points[i].time;
            }
            previousTime = points[i].time;
        }
    }
    return point;
}


#include <stdio.h>

int main()
{
    printf("Hello World\n");
    FILE* fp = fopen("envelope.brk", "r");

    BREAKPOINT points[16];

    if(fp) {
        int ret;
        int count = 0;
        while(ret != EOF) {
            float value;
            float time;
            ret = fscanf(fp, "%f%f", &time, &value);
            if(ret != EOF) {
                points[count].time = time;
                points[count].value = value;
                count++;
                printf("%f    %f\n", time, value);
            }
        }
        BREAKPOINT max = maxpoint(points, count);

        printf("Max point is: %f    %f\n", max.time, max.value);
    }


    fclose(fp);

    return 0;
}