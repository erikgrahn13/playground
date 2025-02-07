#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    double semitone_ratio;
    double c0;
    double c5;
    double frequency;
    int midinote;

    midinote = atoi(argv[1]);
    semitone_ratio = pow(2.0, 1.0/12.0);
    c5 = 220.0 * pow(semitone_ratio, 30);
    c0 = c5 * pow(0.5, 5.0);

    frequency = c0 * pow(semitone_ratio, (double)midinote);

    printf("MIDI Note %d has frequency %f\n", midinote, frequency);

    return 0;
}