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
    double rootA = 440.0;
    double temperament = 12.0;

    if(argc < 2)
    {
        printf("Converts MIDI note to frequnecy.\n");
        printf("usage: ./a.out MIDInote [rootAInHz] [temperament]\n");
        return 1;
    }
    
    // a
    if(argc > 2)
    {
        rootA = atoi(argv[2]);
        
        if(rootA != 440.0 && rootA != 435.0 && rootA != 432.0 && rootA != 415.0)
        {
            printf("Frequency of root A should be anyone of 415, 432, 435 and 440\n");
            return 1;
        }
    }

    // b
    if(argc > 3)
    {
        temperament = atoi(argv[3]);

        if(temperament != 12 && temperament != 15 && temperament != 19)
        {
            printf("Temperament should be anyone of 12, 15 and 19\n");
            return 1;
        }
    }

    midinote = atoi(argv[1]);
    semitone_ratio = pow(2.0, 1.0 / temperament);
    c5 = rootA / 2 * pow(semitone_ratio, 30);
    c0 = c5 * pow(0.5, 5.0);

    frequency = c0 * pow(semitone_ratio, (double)midinote);

    printf("MIDI Note %d has frequency %f\n", midinote, frequency);

    return 0;
}