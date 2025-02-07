#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
    double semitone_ratio;
    double c0;
    double c5;
    double frequency;
    int midinote;

    semitone_ratio = pow(2.0, 1.0/12.0);
    c5 = 220.0 * pow(semitone_ratio, 3);
    c0 = c5 * pow(0.5, 5.0);

    if(argc != 2)
    {
        printf("Converts MIDI note to frequnecy.\n");
        printf("usage: ./a.out MIDInote\n");
        return 1;
    }

    midinote = atoi(argv[1]);
    frequency = c0 * pow(semitone_ratio, (double)midinote);

    double stringLength = 660;

    printf("C4 string length is %fcm\n", stringLength);
    printf("C4# string length is %fcm\n", stringLength / pow(semitone_ratio, 1));
    printf("D4 string length is %fcm\n", stringLength / pow(semitone_ratio, 2));
    printf("D4# string length is %fcm\n", stringLength / pow(semitone_ratio, 3));
    printf("E4 string length is %fcm\n", stringLength / pow(semitone_ratio, 4));
    printf("F4 string length is %fcm\n", stringLength / pow(semitone_ratio, 5));
    printf("F4# string length is %fcm\n", stringLength / pow(semitone_ratio, 6));
    printf("G4 string length is %fcm\n", stringLength / pow(semitone_ratio, 7));
    printf("G4# string length is %fcm\n", stringLength / pow(semitone_ratio, 8));
    printf("A4 string length is %fcm\n", stringLength / pow(semitone_ratio, 9));
    printf("A4# string length is %fcm\n", stringLength / pow(semitone_ratio, 10));
    printf("B4 string length is %fcm\n", stringLength / pow(semitone_ratio, 11));
    printf("C5 string length is %fcm\n", stringLength / pow(semitone_ratio, 12));

    printf("MIDI Note %d has frequency %f\n", midinote, frequency);

    return 0;
}