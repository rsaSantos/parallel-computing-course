#include <stdlib.h>
#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>

// Data structures
//
//
// This struct represents a collection of points.
// Arrays x and y represent the position of each point.
// Arrays new and old help us store something related to each point.
// Arrays new_x and new_y are used to calculate new values for the arrays x and y (not always needed).
// struct
// {
//     float *x;
//     float *y;
//     int *new;
//     int *old;
//     float *new_x;
//     float *new_y;
// } typedef points;

// Pointer for all the points with struct points, for the struct points:
// Arrays x and y will be used to store the points.
// Arrays new and old will store the cluster of the point on the present and previous iteration of the algorithm, respectively.
// Arrays new_x and new_y are not needed.
// points *_points_;
float *points_x;
float *points_y;
int *points_cluster;

// Pointer for all the cluster with struct points, for the struct points:
// Arrays x and y will be used to store the clusters center.
// Arrays new and old will store the the number of points in each cluster on the present and previous iteration of the algorithm, respectively.
// Arrays new_x and new_y are used to calculate the center of each cluster  in the next iteration.
// points *clusters_center;
float *clusters_center_x;
float *clusters_center_y;
int *clusters_size;

/**
 * @brief Allocates memory for the data structures.
 *        Generates random points.
 *        Initializes the clusters centers as the first NCLUSTERS points.
 */
void init_ds(int n_points, int n_clusters)
{
    // Allocate memory for all the data structures.
    points_x = (float *)malloc(n_points * sizeof(float));
    points_y = (float *)malloc(n_points * sizeof(float));
    points_cluster = (int *)malloc(n_points * sizeof(int));

    clusters_center_x = (float *)malloc(n_clusters * sizeof(float));
    clusters_center_y = (float *)malloc(n_clusters * sizeof(float));
    clusters_size = (int *)malloc(n_clusters * sizeof(int));

    // Random seed of 10 for reproducibility.
    srand(10);

    // Index variable.
    int i;

    // Generate random points and store them in the points array.
    for (i = 0; i < n_points; i++)
    {
        points_x[i] = (float)rand() / RAND_MAX;
        points_y[i] = (float)rand() / RAND_MAX;
        points_cluster[i] = -1;
    }

    // Initialize the clusters centers as the first NCLUSTERS points.
    for (i = 0; i < n_clusters; i++)
    {
        clusters_center_x[i] = points_x[i];
        clusters_center_y[i] = points_y[i];
    }
}

/**
 * @brief Free previously allocated memmory.
 */
void free_data()
{
    free(points_x);
    free(points_y);
    free(points_cluster);
    free(clusters_center_x);
    free(clusters_center_y);
    free(clusters_size);
}


__global__ void equalToZero(int n_clusters, float *nclusters_center_x, float *nclusters_center_y, int *nclusters_size) {
    // Index variable.
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n_clusters)
    {
        nclusters_center_x[i] = 0;
        nclusters_center_y[i] = 0;
        nclusters_size[i] = 0;
    }
}

__global__ void sumClusterPoints(int n_points, int n_clusters, float *npoints_x, float *npoints_y, int *npoints_cluster, float *out_clusters_x, float *out_clusters_y, int *out_clusters_size) {
    extern __shared__ float sdata[];
    float *s_clusters_x = (float *) sdata;
    float *s_clusters_y = n_clusters * blockDim.x * sizeof(float) + sdata;
    int *s_clusters_size = n_clusters * blockDim.x * sizeof(float) + (int *) s_clusters_y;


    // each thread loads data from global to shared mem
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

    for(int j = 0; j < n_clusters; j++){
        s_clusters_x[tid + j] = 0;
        s_clusters_y[tid + j] = 0;
        s_clusters_size[tid + j] = 0;
    }

    if(i < n_points){
        int cluster = npoints_cluster[i];
        s_clusters_x[tid + cluster] = npoints_x[i];
        s_clusters_y[tid + cluster] = npoints_y[i];
        s_clusters_size[tid + cluster] = 1;

        __syncthreads();

        // do reduction in shared mem
        for(unsigned int s=1; s < blockDim.x; s *= 2) {
            if (tid % (2*s) == 0) {
                for(int j = 0; j < n_clusters; j++){
                    s_clusters_x[tid + j] = s_clusters_x[tid + j + s];
                    s_clusters_y[tid + j] = s_clusters_y[tid + j + s];
                    s_clusters_size[tid + j] = s_clusters_size[tid + j + s];
                }
            }
            __syncthreads();
        }

        // write result for this block to global mem
        if (tid == 0){
            for(int j = 0; j < n_clusters; j++){
                out_clusters_x[blockIdx.x + j] = s_clusters_x[j];
                out_clusters_y[blockIdx.x + j] = s_clusters_y[j];
                out_clusters_size[blockIdx.x + j] = s_clusters_size[j];
            }

        }   
    }
}

__global__ void reduceArrays(int blockNum, int n_clusters, float *out_clusters_x, float *out_clusters_y, int *out_clusters_size) {
    extern __shared__ float data[];
    float * s_clusters_x = (float *) data;
    float * s_clusters_y = n_clusters * blockDim.x * sizeof(float) + data;
    int * s_clusters_size = n_clusters * blockDim.x * sizeof(float) + (int *) s_clusters_y;

    // each thread loads one element from global to shared mem
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

    if(i < blockNum) {
        for(int j = 0; j < n_clusters; j++){
            s_clusters_x[tid + j] = out_clusters_x[i + j];
            s_clusters_y[tid + j] = out_clusters_y[i + j];
            s_clusters_size[tid + j] = out_clusters_size[i + j];
        }
        __syncthreads();

        // do reduction in shared mem
        for(unsigned int s=1; s < blockDim.x; s *= 2) {
            if (tid % (2*s) == 0) {
                for(int j = 0; j < n_clusters; j++){
                    s_clusters_x[tid + j] = s_clusters_x[i + s + j];
                    s_clusters_y[tid + j] = s_clusters_y[i + s + j];
                    s_clusters_size[tid + j] = s_clusters_size[i + s + j];
                }
            }
            __syncthreads();
        }

        // write result for this block to global mem
        if (tid == 0){
            for(int j = 0; j < n_clusters; j++){
                out_clusters_x[blockIdx.x + j] = out_clusters_x[j];
                out_clusters_y[blockIdx.x + j] = out_clusters_y[j];
                out_clusters_size[blockIdx.x + j] = out_clusters_size[j];
            }
        }
    }
}

__global__ void updateArrays(int n_clusters, float *out_clusters_x, float *out_clusters_y, int *out_clusters_size, float *d_clusters_center_x, float *d_clusters_center_y, int *d_clusters_size) {
    for(int i = 0; i < n_clusters; i++) {
        d_clusters_center_x[i] = out_clusters_x[i];
        d_clusters_center_y[i] = out_clusters_y[i];
        d_clusters_size[i] = out_clusters_size[i];
    }
}

/**
 * @brief Calculates the new clusters centers.
 *
 */
__global__ void divideSumClusterPoints(int n_clusters, float *nclusters_center_x, float *nclusters_center_y, int *nclusters_size)
{
    // Index variable.
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    // Calculate the new clusters centers.
    if (i < n_clusters)
    {
        nclusters_center_x[i] /= nclusters_size[i];
        nclusters_center_y[i] /= nclusters_size[i];
    }
}

/**
 * @brief Calculates the new cluster assignment for each point.
 *
 */
__global__ void calculate_clusters(int n_points, int n_clusters, float *npoints_x, float *npoints_y, int *npoints_cluster, float *nclusters_center_x, float *nclusters_center_y)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n_points)
    {
        // Get the x and y coordinates of the i-th point.
        float x1 = npoints_x[i];
        float y1 = npoints_y[i];

        // Variable that will store the cluster assignment of the i-th point.
        // To better check errors, the point is "assigned" to cluster -1 (invalid).
        int cluster = -1;

        // Indicates that the distance between a point and a cluster center
        //  was not yet calculated.
        float distance = -1;

        for (int j = 0; j < n_clusters; j++)
        {
            // Get the x and y coordinates of the j-th cluster center.
            float x2 = nclusters_center_x[j];
            float y2 = nclusters_center_y[j];

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

        // Update the cluster assignment of the i-th point.
        if (npoints_cluster[i] != cluster)
        {
            // *changed = 1;
            npoints_cluster[i] = cluster;
        }
    }
}

/**
 * @brief Prints the results of the k-means algorithm.
 *        Prints each cluster center, its size and the number of iterations.
 */
void print_results(int n_points, int n_clusters, int iterations)
{
    // Index variable.
    int i;

    // Number of points and clusters.
    printf("N = %d, K = %d\n", n_points, n_clusters);

    // Center and number of points in each cluster.
    for (i = 0; i < n_clusters; i++)
    {
        printf("Center: (%.3f, %.3f) : Size: %d\n", clusters_center_x[i], clusters_center_y[i], clusters_size[i]);
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
int k_means(int n_points, int n_clusters)
{
    // Number of iterations.
    int iterations = 0;
    // int changed = 0;

    // Device grid dimensions
    int block_size = 512;
    int grid_size =( n_points + block_size) / block_size;
    int grid_size2 = (n_clusters + block_size) / block_size;

    printf("block num: %d\n", grid_size);

    // Device variables
    float *d_points_x, *d_points_y, *d_clusters_center_x, *d_clusters_center_y, *out_clusters_x, *out_clusters_y;
    int *d_points_cluster, *d_clusters_size, *out_clusters_size;

    // Allocate device memory
    cudaMalloc((void **)&d_points_x, sizeof(float) * n_points);
    cudaMalloc((void **)&d_points_y, sizeof(float) * n_points);
    cudaMalloc((void **)&d_points_cluster, sizeof(int) * n_points);
    cudaMalloc((void **)&d_clusters_center_x, sizeof(float) * n_clusters);
    cudaMalloc((void **)&d_clusters_center_y, sizeof(float) * n_clusters);
    cudaMalloc((void **)&d_clusters_size, sizeof(int) * n_clusters);
    cudaMalloc((void **)&out_clusters_x, sizeof(float) * n_clusters * grid_size);
    cudaMalloc((void **)&out_clusters_y, sizeof(float) * n_clusters * grid_size);
    cudaMalloc((void **)&out_clusters_size, sizeof(int) * n_clusters * grid_size);

    // Transfer data from host to device memory
    cudaMemcpy(d_points_x, points_x, sizeof(float) * n_points, cudaMemcpyHostToDevice);
    cudaMemcpy(d_points_y, points_y, sizeof(float) * n_points, cudaMemcpyHostToDevice);
    cudaMemcpy(d_points_cluster, points_cluster, sizeof(int) * n_points, cudaMemcpyHostToDevice);
    cudaMemcpy(d_clusters_center_x, clusters_center_x, sizeof(float) * n_clusters, cudaMemcpyHostToDevice);
    cudaMemcpy(d_clusters_center_y, clusters_center_y, sizeof(float) * n_clusters, cudaMemcpyHostToDevice);

    // Calculate the clusters for the first time.
    calculate_clusters<<<grid_size, block_size>>>(n_points, n_clusters, d_points_x, d_points_y, d_points_cluster, d_clusters_center_x, d_clusters_center_y);

    // Keep calculating the clusters for 20 iterations
    // for (iterations = 0; iterations < 20; iterations++)
    // {
    //     equalToZero<<<grid_size2, block_size>>>(n_clusters, d_clusters_center_x, d_clusters_center_y, d_clusters_size);
    //     // sumClusterPoints<<<grid_size, block_size, 3 * block_size * n_clusters>>>(n_points, n_clusters, d_points_x, d_points_y, d_points_cluster, out_clusters_x, out_clusters_y, out_clusters_size);
    //     // reduceArrays<<<1,1>>>(grid_size, n_clusters, out_clusters_x, out_clusters_y, out_clusters_size, d_clusters_center_x, d_clusters_center_y, d_clusters_size);
    //     divideSumClusterPoints<<<grid_size2, block_size>>>(n_clusters, d_clusters_center_x, d_clusters_center_y, d_clusters_size);

    //     // changed = 0;
    //     calculate_clusters<<<grid_size, block_size>>>(n_points, n_clusters, d_points_x, d_points_y, d_points_cluster, d_clusters_center_x, d_clusters_center_y);
    // }

    equalToZero<<<grid_size2, block_size>>>(n_clusters, d_clusters_center_x, d_clusters_center_y, d_clusters_size);
    sumClusterPoints<<<grid_size, block_size, 3 * block_size * n_clusters>>>(n_points, n_clusters, d_points_x, d_points_y, d_points_cluster, out_clusters_x, out_clusters_y, out_clusters_size);
    
    // int blockNum = grid_size;
    // // while(blockNum > block_size) {
    // //     int newGrid = (blockNum + block_size) / block_size;
    // //     reduceArrays<<<newGrid,block_size>>>(blockNum, n_clusters, out_clusters_x, out_clusters_y, out_clusters_size);
    // //     blockNum = newGrid;
    // // }
    // reduceArrays<<<1,block_size>>>(blockNum, n_clusters, out_clusters_x, out_clusters_y, out_clusters_size);
    // updateArrays<<<1,1>>>(n_clusters, out_clusters_x, out_clusters_y, out_clusters_size, d_clusters_center_x, d_clusters_center_y, d_clusters_size);
    // divideSumClusterPoints<<<grid_size2, block_size>>>(n_clusters, d_clusters_center_x, d_clusters_center_y, d_clusters_size);

    cudaMemcpy(clusters_center_x, d_clusters_center_x, sizeof(float) * n_clusters, cudaMemcpyDeviceToHost);
    cudaMemcpy(clusters_center_y, d_clusters_center_y, sizeof(float) * n_clusters, cudaMemcpyDeviceToHost);
    cudaMemcpy(clusters_size, d_clusters_size, sizeof(int) * n_clusters, cudaMemcpyDeviceToHost);


    // Deallocate device memory
    cudaFree(d_points_x);
    cudaFree(d_points_y);
    cudaFree(d_points_cluster);
    cudaFree(d_clusters_center_x);
    cudaFree(d_clusters_center_y);

    return iterations;
}

/**
 * @brief Main function.
 *        It initializes the data and calls the k-means algorithm.
 *        It also prints the results and frees the allocated memory.
 *
 * Needed arguments:
 * - Number of points.
 * - Number of clusters.
 * - Number of threads.
 *
 * @return int 0 if the program finishes successfully.
 */
int main(int argc, char *argv[])
{

    // Declare the event handles
    cudaEvent_t start, stop;

    // Create the events
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Record the start time
    cudaEventRecord(start, 0);

    // Variables to store the number of points, clusters and threads.
    int n_points, n_clusters;

    // Check if the number of arguments is correct.
    if (argc < 3)
    {
        printf("Usage: %s <number of points> <number of clusters> <number of threads>", argv[0]);
        exit(1);
    }
    else
    {
        n_points = atoi(argv[1]);
        n_clusters = atoi(argv[2]);

        // Print command line arguments.
        // printf("Points: %d | Clusters: %d | Threads: %d \n", n_points, n_clusters, n_threads);
    }

    // Initialize the data structures.
    init_ds(n_points, n_clusters);

    // Call the k-means algorithm.
    int iterations = k_means(n_points, n_clusters);

    // Show the results.
    print_results(n_points, n_clusters, iterations);

    // Free the allocated memory.
    free_data();

    // Record the stop time
    cudaEventRecord(stop, 0);

    // Wait for the events to complete
    cudaEventSynchronize(start);
    cudaEventSynchronize(stop);

    // Get the elapsed time
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);

    // Print the elapsed time in milliseconds
    printf("Elapsed time: %f ms\n", elapsedTime);

    // Clean up
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return 0;
}
