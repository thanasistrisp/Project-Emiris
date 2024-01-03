#include "kmeans.hpp"

class KMeansNew : public KMeans { // for evaluating KMeans between initial and latent space
	private:
		std::vector<std::vector<double>> *decoded_points;
		std::vector<std::vector<double>> *decoded_centroids;
		int dataset_size;

		double silhouette(int i);

	public:
		KMeansNew(const std::vector<std::vector<double>>& dataset);
		
		// need to be called before silhouette
		void compute_decoded(std::vector<std::vector<double>> &decoded_dataset, std::vector<std::vector<double>> &initial_dataset);
		// reference to dataset can be freed while this function is called
		std::vector<std::variant<double, std::vector<double>>> silhouette();
};