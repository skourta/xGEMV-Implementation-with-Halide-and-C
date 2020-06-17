#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Desclare the default values
#define AHEIGHT 5
#define AWIDTH 10
#define ALPHA 3
#define BETA 2
#define UPPER 50
#define LOWER -50

int main(int argc, char **argv)
{
    // set the random seed to avoid getting the same values
    srand(time(0));

    // initialize the variables
    int debug = 0;
    int aheight = AHEIGHT;
    int awidth = AWIDTH;
    int alpha = ALPHA;
    int beta = BETA;
    int random = 1;
    int saveFile = 0;

    // check if debug is the only argument set
    if (argc == 2)
    {
        //enable printing the results
        debug = atoi(argv[1]);
    }
    else
    {
        //set all arguments to their specific variables
        if (argc > 2)
        {
            //enable printing the results
            debug = atoi(argv[1]);
            aheight = atoi(argv[2]);
            awidth = atoi(argv[3]);
            alpha = atoi(argv[4]);
            beta = atoi(argv[5]);
            //enable generating random values
            random = atoi(argv[6]);
            if (argc == 8)
            {
                //enable saving files
                saveFile = 1;
            }
        }
    }
    //declare the variables containing the values we are gonna use
    int A[aheight][awidth];
    int x[awidth];
    int y[aheight];
    int res[aheight];
    int yTimesBeta[aheight];

    //filling the variables
    for (int i = 0; i < awidth; i++)
    {
        for (int j = 0; j < aheight; j++)
        {
            if (random)
            {
                // filling randomly with a value between -50 and 50
                A[j][i] = (rand() % (UPPER - LOWER + 1)) + LOWER;
            }
            else
            {
                //filling with index of row
                A[j][i] = j;
            }
        }
        if (random)
        {
            // filling randomly with a value between -50 and 50
            x[i] = (rand() % (UPPER - LOWER + 1)) + LOWER;
        }
        else
        {
            //filling with index of row
            x[i] = i;
        }
        if (i < aheight)
        {
            if (random)
            {
                // filling randomly with a value between -50 and 50
                y[i] = (rand() % (UPPER - LOWER + 1)) + LOWER;
            }
            else
            {
                //filling with index of row
                y[i] = i;
            }
        }
    }
    // Print the Values to the screen if in debug
    if (debug)
    {
        printf("\n\nMatrix A\n");
        for (int i = 0; i < aheight; i++)
        {
            for (int j = 0; j < awidth; j++)
            {
                printf("%d \t", A[i][j]);
            }
            printf("\n");
        }
        printf("\nVector x\n");
        for (int i = 0; i < awidth; i++)
        {
            printf("%d \t", x[i]);
        }
        printf("\n\nVector y\n");
        for (int i = 0; i < aheight; i++)
        {
            printf("%d \t", y[i]);
        }
    }

    // timing the execution of the gemv pipeline
    clock_t t;
    t = clock();
    //calculating the values
    for (int i = 0; i < aheight; i++)
    {
        int sum = 0;
        for (int j = 0; j < awidth; j++)
        {
            sum += A[i][j] * x[j];
        }
        res[i] = alpha * sum + beta * y[i];
    }
    // calculating the execution time of the gemv pipeline
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds
    // Print the Results to the screen if in debug
    if (debug)
    {
        printf("\n\nResult\n");
        for (int i = 0; i < aheight; i++)
        {
            printf("%d \t", res[i]);
        }
    }
    //save the results to the "./results/c_results.txt" file
    if (saveFile)
        if (saveFile)
        {
            FILE *fp;
            int i;
            /* open the file for writing*/
            fp = fopen("./results/c_results.txt", "w");

            /* write 10 lines of text into the file stream*/
            for (i = 0; i < aheight; i++)
            {
                fprintf(fp, "%d ", res[i]);
            }

            /* close the file*/
            fclose(fp);
        }

    printf("\nC GEMV took %f seconds to execute on width=%d and height=%d\n", time_taken, awidth, aheight);
}
