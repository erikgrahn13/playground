#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
        result = argv;
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
    if (message[0] == '\0') // if no argument is provided we exit the program
    {
        printf("Have a nice day!\n");
        return 1;
    }
    midinote = atoi(message);
    
    // MIDI notes are only valid from 0 to 127
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
    frequency = c0 * pow(semitone_ratio, midinote); // frequency in hertz for the provided midinote
    printf("frequency of MIDI note %d = %f\n", midinote, frequency);
    return 0;
}