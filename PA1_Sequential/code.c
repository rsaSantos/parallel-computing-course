#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Tamanho
int N = 100;
int K = 10;

// dados
float *points;
int *new_points_cluster;
int *old_points_cluster;
int *new_clusters_size;
int *old_clusters_size;
float *clusters_centroid;


//TODO: Falta fazer o gerador de pontos e testar


// função para criar amostras
void generate_points()
{
    points = (float *)malloc(N * 2 * sizeof(float));
    new_points_cluster = (float *)malloc(N * sizeof(int));
    old_points_cluster = (float *)malloc(N * sizeof(int));
    new_clusters_size = (float *)malloc(K * sizeof(int));
    old_clusters_size = (float *)malloc(K * sizeof(int));
    clusters_centroid = (float *)malloc(K * 2 * sizeof(float));

    // generate points

    // choose clusters
    for (int i = 0; i < K; i++)
    {
        clusters_centroid[2 * i] = points[2 * i];
        clusters_centroid[2 * i + 1] = points[2 * i + 1];
    }
}

// free
void free_data()
{
    free(new_points_cluster);
    free(old_points_cluster);
    free(new_clusters_size);
    free(old_clusters_size);
    free(clusters_centroid);
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
        new_points_cluster[i] = 0;
    }
}

// função que calcula centroide
void calculate_centroids()
{
    for (int i = 0; i < K; i++)
    {
        clusters_centroid[2 * i] = 0;
        clusters_centroid[2 * i + 1] = 0;
    }

    for (int i = 0; i < N; i++)
    {
        int cluster = new_points_cluster[i];
        int size_cluster = new_clusters_size[cluster];

        clusters_centroid[2 * cluster] += points[2 * i] / size_cluster;
        clusters_centroid[2 * cluster + 1] += points[2 * i + 1] / size_cluster;
    }
}

// calcula distancia a cluster
float distance_points(float x1, float y1, float x2, float y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// função que calcula os clusters
void calculate_clusters()
{
    change_clusters_new_to_old();

    for (int i = 0; i < N; i++)
    {
        int x1 = points[2 * i];
        int y1 = points[2 * i + 1];

        int cluster = 0;
        float distance = -1;

        for (int j = 0; j < K; j++)
        {
            float distance_j = distance_points(x1, y1, clusters_centroid[2 * j], clusters_centroid[2 * j + 1]);

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

// k means
void k_means()
{
    generate_points();

    calculate_clusters();

    while (compare_clusters() == -1)
    {
        calculate_centroids();
        calculate_clusters();
    }

    free_data();
}

int main()
{
    k_means();
    return 0;
}
