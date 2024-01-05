struct config
{
    char* model;   // BRUTE, LSH, CUBE, MRNG, NSG, GNN options.
    int *vals;     // Algorithm integer only parameters.
    double window; // window parameter for LSH, CUBE.
    const char *dataset;
    const char *query;
    const char *encoded_dataset;
    const char *decoded_dataset;
};