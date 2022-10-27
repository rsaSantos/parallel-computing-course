#include <stdlib.h>
#include <stdio.h>

// Number of points
#define NPOINTS 10000000

// Number of clusters
#define NCLUSTERS 4

// Data structures
//
//
// Points: Represented as an array of 2 * NPOINTS floats.
//  Any two consecutive floats in indexes i,j such that (0 <= i < j < 2 * NPOINTS)
//      represent a point iff i % 2 == 0 and j % 2 == 1.
// Example:
//  points[0] and points[1] represent the first point.
//
float *points;

//
// This array represents the most updated cluster assignment for each point.
//  The value at index i represents the cluster assignment for the i-th point.
// Example:
//  new_points_cluster[0] represents the cluster assignment for the first point.
//      meaning, the cluster where the first point belongs to.
//
int *new_points_cluster;

//
// Similar to new_points_cluster, but represents the cluster assignment for the
//  previous iteration. These and the new_points_cluster array are used to
//  determine if the algorithm is converging.
//
int *old_points_cluster;

//
// This array represents the current size of each cluster.
//
int *new_clusters_size;

//
// This array represents the previous size of each cluster.
//
int *old_clusters_size;

//
// This array represents the current center of each cluster.
// Similar to points, any two consecutive floats in indexes i,j such that
//  (0 <= i < j < 2 * NCLUSTERS) represent a point iff i % 2 == 0 and j % 2 == 1.
// Example:
//  clusters_center[0] and clusters_center[1] represent the first cluster center
//      x and y, respectively.
//
float *clusters_center;

/**
 * @brief Allocates memory for the data structures.
 *        Generates random points.
 *        Initializes the clusters centers as the first NCLUSTERS points.
 */
void init_ds()
{
    // Allocate memory for all the data structures.
    points = (float *)malloc(NPOINTS * 2 * sizeof(float));
    new_points_cluster = (int *)malloc(NPOINTS * sizeof(int));
    old_points_cluster = (int *)malloc(NPOINTS * sizeof(int));
    new_clusters_size = (int *)malloc(NCLUSTERS * sizeof(int));
    old_clusters_size = (int *)malloc(NCLUSTERS * sizeof(int));
    clusters_center = (float *)malloc(NCLUSTERS * 2 * sizeof(float));

    // Random seed of 10 for reproducibility.
    srand(10);

    // Index variable.
    int i;

    // Generate random points and store them in the points array.
    for (i = 0; i < NPOINTS; i++)
    {
        points[2 * i] = (float)rand() / RAND_MAX;
        points[2 * i + 1] = (float)rand() / RAND_MAX;
    }

    // Initialize the clusters centers as the first NCLUSTERS points.
    for (i = 0; i < NCLUSTERS; i++)
    {
        clusters_center[2 * i] = points[2 * i];
        clusters_center[2 * i + 1] = points[2 * i + 1];
    }
}

/**
 * @brief Free previously allocated memmory.
 */
void free_data()
{
    free(new_points_cluster);
    free(old_points_cluster);
    free(new_clusters_size);
    free(old_clusters_size);
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
        old_clusters_size[i] = new_clusters_size[i];
        new_clusters_size[i] = 0;
    }

    // Update the points of the old cluster.
    // No need to set the new points to other value, since they will be overwritten.
    for (i = 0; i < NPOINTS; i++)
    {
        old_points_cluster[i] = new_points_cluster[i];
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

    // Set the new clusters centers x and y to 0.
    for (i = 0; i < NCLUSTERS; i++)
    {
        clusters_center[2 * i] = 0;
        clusters_center[2 * i + 1] = 0;
    }

    // Sum all the x and y coordinates of the points in each cluster.
    for (i = 0; i < NPOINTS; i++)
    {
        // Get the cluster assignment of the i-th point.
        int cluster = new_points_cluster[i];

        // Sum the x and y coordinates of the i-th point to the cluster center.
        clusters_center[2 * cluster] += points[2 * i];
        clusters_center[2 * cluster + 1] += points[2 * i + 1];
    }

    // Calculate the new clusters centers.
    for (i = 0; i < NCLUSTERS; i++)
    {
        // Divide the sum of the x and y coordinates by the number of points in the cluster.
        // This will give the new cluster center.
        clusters_center[2 * i] /= new_clusters_size[i];
        clusters_center[2 * i + 1] /= new_clusters_size[i];
    }
}

/**
 * @brief Calculates the squared distance between two points.
 *
 * @param x1 The x coordinate of the first point.
 * @param y1 The y coordinate of the first point.
 * @param x2 The x coordinate of the second point.
 * @param y2 The y coordinate of the second point.
 *
 * @return float The squared distance between the two points.
 */
float squared_distance(float x1, float y1, float x2, float y2)
{
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
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
        float x1 = points[2 * i];
        float y1 = points[2 * i + 1];

        // Variable that will store the cluster assignment of the i-th point.
        // To better check errors, the point is "assigned" to cluster -1 (invalid).
        int cluster = -1;

        // Indicates that the distance between a point and a cluster center
        //  was not yet calculated.
        float distance = -1;

        for (j = 0; j < NCLUSTERS; j++)
        {
            // Get the x and y coordinates of the j-th cluster center.
            float x2 = clusters_center[2 * j];
            float y2 = clusters_center[2 * j + 1];

            // Calculate the squared distance between the i-th point and the
            //  j-th cluster center.
            float distance_j = squared_distance(x1, y1, x2, y2);

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
        new_clusters_size[cluster]++;

        // Update the cluster assignment of the i-th point.
        new_points_cluster[i] = cluster;
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
    for (i = 0; (i < NCLUSTERS) && (new_clusters_size[i] == old_clusters_size[i]); i++)
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
        for (i = 0; (i < NPOINTS) && (new_points_cluster[i] == old_points_cluster[i]); i++)
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
        printf("Center: (%.3f, %.3f) : Size: %d\n", clusters_center[2 * i], clusters_center[2 * i + 1], new_clusters_size[i]);
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
