#include "Halide.h"
#include "time.h"

// We'll also include stdio for printf.
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace Halide;
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
    // declare variables for the Halide fill pipline
    Var x_fill, y_fill;
    // declare  the Halide fill Matrix pipline
    Func fillMatrix;
    if (random)
    {
        // Add definition to the Halide fillMatrix pipline
        fillMatrix(x_fill, y_fill) = Halide::cast<int>(((random_int()) % (UPPER - LOWER + 1)) + LOWER);
    }
    else
    {
        // Add definition to the Halide fillMatrix pipline
        fillMatrix(x_fill, y_fill) = Halide::cast<int>(y_fill);
    }
    // Add parallalisation using the x_fill to the Halide fillMatrix pipline
    fillMatrix.parallel(x_fill);

    // declare  the Halide fillVector pipline
    Func fillVector;

    if (random)
    {
        // Add definition to the Halide fillVector pipline
        fillVector(x_fill) = Halide::cast<int>((random_int() % (UPPER - LOWER + 1)) + LOWER);
    }
    else
    {
        // Add definition to the Halide fillVector pipline
        fillVector(x_fill) = Halide::cast<int>(x_fill);
    }
    // Add parallalisation using the x_fill to the Halide fillVector pipline
    fillVector.parallel(x_fill);

    //Declare and initialize the buffers for the values we are gonna use
    Buffer<int> matrixA = fillMatrix.realize(awidth, aheight);
    Buffer<int> vectorX = fillVector.realize(awidth);
    Buffer<int> vectorY = fillVector.realize(aheight);
    // Print the Values to the screen if in debug
    if (debug)
    {
        cout << "Matrix A" << endl;
        for (int i = 0; i < matrixA.height(); i++)
        {
            for (int j = 0; j < matrixA.width(); j++)
            {
                cout << matrixA(j, i) << "\t";
            }
            cout << endl;
        }
        cout << "\nVector x\n";

        for (int i = 0; i < vectorX.width(); i++)
        {
            cout << vectorX(i) << "\t";
        }

        cout << "\n\nVector y\n";
        for (int i = 0; i < vectorY.width(); i++)
        {
            cout << vectorY(i) << "\t";
        }
        cout << endl;
    }
    // check that all dimensions are valid
    if (((matrixA.width() != vectorX.width()) || (matrixA.height() != vectorY.width())))
    {
        printf("The dimensions are not correct");
        exit(1);
    }
    // declare the variable used in the gemv pipeline
    Var Y;
    // declare  the gemv pipeline
    Func gemv;
    // declare the RDom used in the gemv pipeline
    RDom rdom(0, matrixA.width());
    // Add definition to the Halide gemv pipline
    gemv(Y) = alpha * sum(matrixA(rdom, Y) * vectorX(rdom)) + beta * vectorY(Y);
    // Add parallalisation using the Y to the Halide gemv pipline
    gemv.parallel(Y);
    // timing the execution of the gemv pipeline
    clock_t t = clock();
    // executing the gemv pipeline and saving the results in the reslut buffer
    Buffer<int> result = gemv.realize(aheight);
    t = clock() - t;
    // calculating the execution time of the gemv pipeline
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds
    // Print the Values to the screen if in debug
    if (debug)
    {
        cout << "\n\nResult\n";
        for (int i = 0; i < aheight; i++)
        {
            cout << result(i) << "\t";
        }
    }
    printf("\nHalide GEMV took %f seconds to execute \n", time_taken);
    //declare the transpose pipeline
    Func transpose;
    //declare the variables used in the transpose pipeline
    Var x1, y1;
    //adding definition to the transpose pipeline
    transpose(x1, y1) = matrixA(y1, x1);
    // Add parallalisation using the Y to the Halide gemv pipline
    transpose.parallel(y1);
    // executing the transpose pipeline and saving the results in the trans buffer
    Buffer<int> trans = transpose.realize(aheight, awidth);
    // Print the Values to the screen if in debug
    if (debug)
    {
        cout << "\n\nA^t" << endl;
        for (int i = 0; i < trans.height(); i++)
        {
            for (int j = 0; j < trans.width(); j++)
            {
                cout << trans(j, i) << "\t";
            }
            cout << endl;
        }
    }
    //declare the gemvTrans pipeline
    Func gemvTrans;
    // Add definition to the Halide gemvTrans pipline
    gemvTrans(Y) = alpha * sum(trans(Y, rdom) * vectorX(rdom)) + beta * vectorY(Y);
    // Add parallalisation using the Y to the Halide gemvTrans pipline
    gemvTrans.parallel(Y);

    t = clock();
    // executing the gemv pipeline and saving the results in the reslutTrans buffer
    Buffer<int> resultTrans = gemvTrans.realize(aheight);
    t = clock() - t;
    // calculating the execution time of the gemv pipeline
    time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds
    // Print the Results to the screen if in debug
    if (debug)
    {
        cout << "\n\nResult\n";
        for (int i = 0; i < aheight; i++)
        {
            cout << resultTrans(i) << "\t";
        }
    }
    //save the results to the "./results/halide_result.txt" file
    if (saveFile)
    {
        ofstream myfile;
        myfile.open("./results/halide_result.txt");
        for (int i = 0; i < aheight; i++)
        {
            myfile << result(i) << " ";
        }
        myfile.close();
    }
    printf("\nHalide GEMV transpose took %f seconds to execute \n", time_taken);

    return 0;
}
