#include "kmeans.hpp"
#include <variant>

// For evaluating the K-Means algorithm between the initial and latent space.
class KMeansEval : public KMeans {
	private:
		// Returns the silhouette of the cluster with the given index.
		double silhouette(int i, std::vector<std::vector<double>> &initial_dataset, std::vector<std::vector<double>> &decoded_centroids);

	public:
		KMeansEval(const std::vector<std::vector<double>>& dataset);
		
		// Returns the total silhouette of the clusters and the silhouette for each cluster in the latent space.
		std::vector<std::variant<double, std::vector<double>>> silhouette(std::vector<std::vector<double>> &initial_dataset, std::vector<std::vector<double>> &decoded_centroids);
};