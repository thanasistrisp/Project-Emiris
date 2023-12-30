struct config
{
    char* model;   // LSH, CUBE, MRNG, NSG, GNN, BRUTE
    int *vals;     // algorithm parameters
    double window;
    const char *dataset;
    const char *query;
    const char *encoded_dataset;
    const char *decoded_dataset;
};