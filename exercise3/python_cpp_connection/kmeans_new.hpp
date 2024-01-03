#include "kmeans.hpp"

class KMeansEval : public KMeans { // for evaluating KMeans between initial and latent space
	private:
		double silhouette(int i, std::vector<std::vector<double>> &initial_dataset, std::vector<std::vector<double>> &decoded_centroids);

	public:
		KMeansEval(const std::vector<std::vector<double>>& dataset);
		
		std::vector<std::variant<double, std::vector<double>>> silhouette(std::vector<std::vector<double>> &initial_dataset);
};