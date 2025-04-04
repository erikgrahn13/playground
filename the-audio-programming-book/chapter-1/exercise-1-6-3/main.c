for (i = 0; i <= notes; i++)
{
    float interval_ratio;
    float interval = intervals[i];
    if (write_interval)
        interval_ratio = pow(ratio, i);
        printf("%d:\t%f\t%f\n", i, interval_ratio, interval);
    else
        printf("%d:\t%f\n", i, interval);
    if (fp)
    {
        if (write_interval)
            err = fprintf(fp, "%d:\t%f\t%f\n",
                          i, interval_ratio, interval);
        else
            err = fprintf(fp, "%d:\t%f\n", i, interval);
        if (err < 0)
            break;
    }
}
