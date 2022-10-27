#include <stdlib.h>
#include <stdio.h>

// Number of points
#define NPOINTS 10000000

// Number of clusters
#define NCLUSTERS 4

// Data structures
//
//

// TODO: align?
struct
{
    float *x;
    float *y;
    int *new;
    int *old;
    float *new_x;
    float *new_y;
} typedef points;

points *_points_;

points *clusters_center;

/**
 * @brief Allocates memory for the data structures.
 *        Generates random points.
 *        Initializes the clusters centers as the first NCLUSTERS points.
 */
void init_ds()
{
    // Allocate memory for all the data structures.
    _points_ = malloc(sizeof(points));
    _points_->x = (float *)malloc(NPOINTS * sizeof(float));
    _points_->y = (float *)malloc(NPOINTS * sizeof(float));
    _points_->new = (int *)malloc(NPOINTS * sizeof(int));
    _points_->old = (int *)malloc(NPOINTS * sizeof(int));

    clusters_center = malloc(sizeof(points));
    clusters_center->x = (float *)malloc(NCLUSTERS * sizeof(float));
    clusters_center->y = (float *)malloc(NCLUSTERS * sizeof(float));
    clusters_center->new = (int *)malloc(NCLUSTERS * sizeof(int));
    clusters_center->old = (int *)malloc(NCLUSTERS * sizeof(int));
    clusters_center->new_x = (float *)malloc(NCLUSTERS * sizeof(float));
    clusters_center->new_y = (float *)malloc(NCLUSTERS * sizeof(float));

    // Random seed of 10 for reproducibility.
    srand(10);

    // Index variable.
    int i;

    // Generate random points and store them in the points array.
    for (i = 0; i < NPOINTS; i++)
    {
        _points_->x[i] = (float)rand() / RAND_MAX;
        _points_->y[i] = (float)rand() / RAND_MAX;
    }

    // Initialize the clusters centers as the first NCLUSTERS points.
    for (i = 0; i < NCLUSTERS; i++)
    {
        clusters_center->x[i] = _points_->x[i];
        clusters_center->y[i] = _points_->y[i];
        clusters_center->new_x[i] = 0;
        clusters_center->new_y[i] = 0;
    }
}

/**
 * @brief Free previously allocated memmory.
 */
void free_data()
{
    free(_points_->x);
    free(_points_->y);
    free(_points_->new);
    free(_points_->old);
    free(_points_);

    free(clusters_center->x);
    free(clusters_center->y);
    free(clusters_center->new);
    free(clusters_center->old);
    free(clusters_center->new_x);
    free(clusters_center->new_y);
    free(clusters_center);
}

/**
 * @brief Updates the old cluster sizes with the new cluster sizes.
 *        Updates the old cluster points with the new cluster points.
 */
void change_clusters_new_to_old()
{
    // Index variable.
    int i;

    // Update the size of each cluster.
    // Set the new size of each cluster to 0.
    for (i = 0; i < NCLUSTERS; i++)
    {
        clusters_center->old[i] = clusters_center->new[i];
        clusters_center->new[i] = 0;
    }
}

/**
 * @brief Calculates the new clusters centers.
 *
 */
void calculate_centroids()
{
    // Index variable.
    int i;

    // Calculate the new clusters centers.
    for (i = 0; i < NCLUSTERS; i++)
    {
        clusters_center->x[i] = clusters_center->new_x[i] / clusters_center->new[i];
        clusters_center->y[i] = clusters_center->new_y[i] / clusters_center->new[i];

        clusters_center->new_x[i] = 0;
        clusters_center->new_y[i] = 0;
    }
}

/**
 * @brief Calculates the new cluster assignment for each point.
 *
 */
void calculate_clusters()
{
    // Index variable.
    int i, j;

    // Update the previous cluster assignment.
    // The values of the "new cluster assignment" will be overwritten.
    change_clusters_new_to_old();

    // Iterate through all the points.
    for (i = 0; i < NPOINTS; i++)
    {
        // Get the x and y coordinates of the i-th point.
        float x1 = _points_->x[i];
        float y1 = _points_->y[i];

        _points_->old[i] = _points_->new[i];

        // Variable that will store the cluster assignment of the i-th point.
        // To better check errors, the point is "assigned" to cluster -1 (invalid).
        int cluster = -1;

        // Indicates that the distance between a point and a cluster center
        //  was not yet calculated.
        float distance = -1;

        for (j = 0; j < NCLUSTERS; j++)
        {
            // Get the x and y coordinates of the j-th cluster center.
            float x2 = clusters_center->x[j];
            float y2 = clusters_center->y[j];

            // Calculate the squared distance between the i-th point and the
            //  j-th cluster center.
            float distance_j = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);

            // Check if the i-th point is closer to the j-th cluster center
            //  than the previous closest cluster center.
            // If the distance is -1, then this is the first cluster center
            //  checked and the i-th point is automatically assigned to it.
            if (distance_j < distance || distance < 0)
            {
                cluster = j;
                distance = distance_j;
            }
        }

        // Update the cluster size that was assigned to the i-th point.
        clusters_center->new[cluster]++;

        // Update the cluster assignment of the i-th point.
        _points_->new[i] = cluster;

        //
        clusters_center->new_x[cluster] += _points_->x[i];
        clusters_center->new_y[cluster] += _points_->y[i];
    }
}

/**
 * @brief Checks if the clusters have changed.
 *        It first checks if the number of points in each cluster has changed.
 *        If the number of points in each cluster has not changed, then it checks
 *        if the points in each cluster have changed.
 *
 * @return int 1 if the clusters have changed, 0 otherwise.
 */
int compare_clusters()
{
    // Index variable.
    int i;

    // Return variable
    int changed = 0;

    // Check if the clusters size have changed.
    // If they have changed, then the clusters have changed.
    // And there is no need to check the points in each cluster.
    for (i = 0; (i < NCLUSTERS) && (clusters_center->new[i] == clusters_center->old[i]); i++)
    {
    };

    // Check if clusters size have changed.
    // If not all iterations of the for loop have been executed,
    //  then the clusters size have changed.
    if (i < NCLUSTERS)
    {
        changed = 1;
    }
    else
    {
        // Check if the points in each cluster have changed.
        for (i = 0; (i < NPOINTS) && (_points_->new[i] == _points_->old[i]); i++)
        {
        };

        // If not all iterations of the for loop have been executed,
        //  then the points in each cluster have changed.
        if (i < NPOINTS)
        {
            changed = 1;
        }
    }

    return changed;
}

/**
 * @brief Prints the results of the k-means algorithm.
 *        Prints each cluster center, its size and the number of iterations.
 */
void print_results(int iterations)
{
    // Index variable.
    int i;

    // Number of points and clusters.
    printf("NPOINTS = %d, NCLUSTERS = %d\n", NPOINTS, NCLUSTERS);

    // Center and number of points in each cluster.
    for (i = 0; i < NCLUSTERS; i++)
    {
        printf("Center: (%.3f, %.3f) : Size: %d\n", clusters_center->x[i], clusters_center->y[i], clusters_center->new[i]);
    }

    // Number of iterations.
    printf("Iterations: %d\n", iterations);
}

/**
 * @brief K-means algorithm.
 *       The algorithm will stop when the clusters stop changing.
 *
 * @return int The number of iterations.
 */
int k_means()
{
    // Number of iterations.
    int iterations;

    // Calculate the clusters for the first time.
    calculate_clusters();

    // Keep calculating the clusters until they stop changing.
    for (iterations = 0; compare_clusters() != 0; iterations++)
    {
        calculate_centroids();
        calculate_clusters();
    }

    return iterations;
}

/**
 * @brief Main function.
 *        It initializes the data and calls the k-means algorithm.
 *        It also prints the results and frees the allocated memory.
 *
 * @return int 0 if the program finishes successfully.
 */
int main()
{
    // Initialize the data structures.
    init_ds();

    // Call the k-means algorithm.
    int iterations = k_means();

    // Show the results.
    print_results(iterations);

    // Free the allocated memory.
    free_data();

    return 0;
}
