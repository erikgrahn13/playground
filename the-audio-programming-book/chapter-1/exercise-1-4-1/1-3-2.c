#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    double semitone_ratio;
    double c0;
    double c5;
    double frequency;
    int midinote;
    double pitchbend;
    char *result;
    char message[256];

    if(argc > 1)
    {
        result = argv[1];
        strcpy(message, argv[1]);
    }
    else
    {
        printf("Enter frequency: ");
        result = fgets(message, sizeof(message), stdin);
    }

    if(result == NULL)
    {
        printf("There was an error reading the input.\n");
        return 1;
    }
    if(message[0] == '\0') {
        printf("Have a nice day!\n");
        return 1;
    }

    frequency = atoi(message);
    if(frequency <= 0)
    {
        printf("Sorry - 0 or negative frequencies does not work\n");
        return 1;
    }
    
    semitone_ratio = pow(2.0, 1.0/12.0);
    c5 = 220.0 * pow(semitone_ratio, 3.0);
    c0 = c5 * pow(0.5, 5.0);

    double fracmidi = log(frequency / c0) / log(semitone_ratio);
    midinote = (int)(fracmidi + 0.5);

    double frequency_above = c0 * pow(semitone_ratio, (double)midinote);
    double frequency_below = c0 * pow(semitone_ratio, (int)(fracmidi - 0.5)); 
    double deviation = (frequency - frequency_above) / (frequency_above - frequency_below);

    printf("The nearest MIDI note to the frequency %f is %d and has a deviation of %d%%\n", frequency, midinote, (int)(deviation*100.0));

    return 0;
}