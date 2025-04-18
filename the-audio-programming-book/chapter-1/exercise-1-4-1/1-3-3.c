#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv)
{
    double c5, c0, semitone_ratio, frequency;
    int midinote;
    char message[256];
    char *result;
    semitone_ratio = pow(2, 1.0 / 12);
    c5 = 220.0 * pow(semitone_ratio, 3);
    c0 = c5 * pow(0.5, 5);

    if(argc > 1)
    {
        result = argv[1];
        strcpy(message, argv[1]);
    }
    else
    {
        printf("Enter MIDI note (0 - 127): ");
        result = gets(message);
    }
    if (result == NULL)
    {
        printf("There was an error reading the input.\n");
        return 1;
    }
    if (message[0] == '\0')
    {
        printf("Have a nice day!\n");
        return 1;
    }

    if (strchr(message, '.') != NULL)
    {
        printf("Midinotes consists of integers and not floats!\n");
        return 1;
    }

    midinote = atoi(message);

    if (midinote < 0)
    {
        printf("Sorry - %s is a bad MIDI note number\n", message);
        return 1;
    }
    if (midinote > 127)
    {
        printf("Sorry - %s is beyond the MIDI range!\n", message);
        return 1;
    }
    frequency = c0 * pow(semitone_ratio, midinote);
    printf("frequency of MIDI note %d = %f\n", midinote, frequency);
    return 0;
}