#include <stdlib.h>
#include <stdio.h>

// Tamanho
int N = 10000000;
int K = 4;

// dados
float *points;
int *new_points_cluster;
int *old_points_cluster;
int *new_clusters_size;
int *old_clusters_size;
float *clusters_center;

// TODO: Falta fazer o gerador de pontos e testar

// função para criar amostras
void generate_points()
{
    points = (float *)malloc(N * 2 * sizeof(float));
    new_points_cluster = (int *)malloc(N * sizeof(int));
    old_points_cluster = (int *)malloc(N * sizeof(int));
    new_clusters_size = (int *)malloc(K * sizeof(int));
    old_clusters_size = (int *)malloc(K * sizeof(int));
    clusters_center = (float *)malloc(K * 2 * sizeof(float));

    // generate points
    srand(10);

    for (int i = 0; i < N; i++)
    {
        points[2 * i] = (float)rand() / RAND_MAX;
        points[2 * i + 1] = (float)rand() / RAND_MAX;
    }

    // choose clusters
    for (int i = 0; i < K; i++)
    {
        clusters_center[2 * i] = points[2 * i];
        clusters_center[2 * i + 1] = points[2 * i + 1];
    }
}

// free
void free_data()
{
    free(new_points_cluster);
    free(old_points_cluster);
    free(new_clusters_size);
    free(old_clusters_size);
    free(clusters_center);
}

// função para passar os clusters novos para os velhos
void change_clusters_new_to_old()
{
    for (int i = 0; i < K; i++)
    {
        old_clusters_size[i] = new_clusters_size[i];
        new_clusters_size[i] = 0;
    }

    for (int i = 0; i < N; i++)
    {
        old_points_cluster[i] = new_points_cluster[i];
    }
}

// função que calcula centroide
void calculate_centroids()
{
    for (int i = 0; i < K; i++)
    {
        clusters_center[2 * i] = 0;
        clusters_center[2 * i + 1] = 0;
    }

    for (int i = 0; i < N; i++)
    {
        int cluster = new_points_cluster[i];

        clusters_center[2 * cluster] += points[2 * i];
        clusters_center[2 * cluster + 1] += points[2 * i + 1];
    }

    for (int i = 0; i < K; i++)
    {
        clusters_center[2 * i] /= new_clusters_size[i];
        clusters_center[2 * i + 1] /= new_clusters_size[i];
    }
}

// calcula distancia a cluster
float distance_points(float x1, float y1, float x2, float y2)
{
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

// função que calcula os clusters
void calculate_clusters()
{
    change_clusters_new_to_old();

    for (int i = 0; i < N; i++)
    {
        float x1 = points[2 * i];
        float y1 = points[2 * i + 1];

        int cluster = 0;
        float distance = -1;

        for (int j = 0; j < K; j++)
        {
            float distance_j = distance_points(x1, y1, clusters_center[2 * j], clusters_center[2 * j + 1]);

            if (distance_j < distance || distance < 0)
            {
                cluster = j;
                distance = distance_j;
            }
        }

        new_clusters_size[cluster]++;
        new_points_cluster[i] = cluster;
    }
}

// compara clusters antigos com novos para ver se alterou
int compare_clusters()
{
    for (int i = 0; i < K; i++)
    {
        if (new_clusters_size[i] != old_clusters_size[i])
        {
            return -1;
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (new_points_cluster[i] != old_points_cluster[i])
        {
            return -1;
        }
    }

    return 0;
}

// prints
void prints(int it)
{
    printf("N = %d, K = %d\n", N, K);

    for (int i = 0; i < K; i++)
    {
        printf("Center: (%.3f, %.3f) : Size: %d\n", clusters_center[2 * i], clusters_center[2 * i + 1], new_clusters_size[i]);
    }

    printf("Iterations: %d\n", it);
}

// k means
void k_means()
{
    int iterations = 0;
    generate_points();

    calculate_clusters();

    while (compare_clusters() == -1)
    {
        calculate_centroids();
        calculate_clusters();
        iterations++;
    }

    prints(iterations);

    free_data();
}

int main()
{
    k_means();
    return 0;
}
