struct encoded_config
{
    char* model;   // LSH, CUBE, MRNG, NSG, GNN, BRUTE
    int *enc_vals; // parameters for encoded space
    const char *dataset;
    const char *query;
    const char *encoded_dataset;
    const char *decoded_dataset;
};