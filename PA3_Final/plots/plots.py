from matplotlib import pyplot as plt

# 4 clusters
nthreads = list(range(1,41))
texec = [5.1151, 3.7323, 1.9277, 1.6054, 1.4145, 1.1759, 1.1844, 1.0323, 0.9641, 0.8314, 0.8668, 0.8034, 0.8065, 0.7498, 0.7300, 0.7358, 0.7111, 0.7015, 0.6995, 0.6907, 0.6591, 0.6576, 0.6598, 0.6404, 0.6311, 0.6266, 0.6080, 0.6168, 0.5983, 0.5748, 0.5733, 0.5667, 0.5424, 0.5457, 0.5291, 0.5147, 0.5134, 0.5101, 0.5184, 0.5160]
terror = [0.0110, 0.0068, 0.0005, 0.0271, 0.0374, 0.0307, 0.0183, 0.0184, 0.0123, 0.0179, 0.0115, 0.0120, 0.0118, 0.0114, 0.0110, 0.0272, 0.0149, 0.0172, 0.0191, 0.0120, 0.0052, 0.0128, 0.0149, 0.0093, 0.0132, 0.0111, 0.0116, 0.0162, 0.0154, 0.0119, 0.0086, 0.0103, 0.0056, 0.0106, 0.0033, 0.0016, 0.0027, 0.0028, 0.0120, 0.0068]
cc = [14827112860, 14948630044, 15138265343, 15246466041, 15310509457, 15295836622, 15550513055, 15481661450, 15596063506, 15675683748, 15930966864, 16166028382, 16371549703, 16552540603, 16790758684, 17420311909, 17760462182, 18030219276, 18995868108, 20396071127, 20848960501, 21080507188, 21444311037, 21990735018, 22378258250, 22958014241, 23131553413, 23613772623, 23773065538, 24111912869, 24719488461, 24840833880, 25371591244, 25898917847, 25358960418, 25097788320, 25609064566, 25958773922, 26097907572, 26710009097]
cm = [5785870, 6700646, 7005436, 7144340, 7347542, 6505678, 6627103, 7077015, 7239748, 7678174, 7893986, 8083604, 8225892, 8366555, 8485572, 8665584, 8771375, 8894750, 9139810, 9214734, 9275849, 9517451, 9641933, 9781800, 9947104, 10160996, 10327423, 10643770, 10859290, 11154034, 11388646, 11746385, 12108486, 12539911, 12493486, 12823440, 13330024, 13597565, 13524626, 14008928]

# 32 clusters
nthreads2 = list(range(1,41))
texec2 = [19.3505, 9.8352, 6.6637, 5.0960, 4.1478, 3.5148, 3.0745, 2.7206, 2.4688, 2.2483, 2.1012, 1.9559, 1.8355, 1.7282, 1.6416, 1.6604, 1.5231, 1.4566, 1.4835, 1.4416, 1.6860, 1.6920, 1.6728, 1.6449, 1.6091, 1.5718, 1.5307, 1.4898, 1.4521, 1.4141, 1.3799, 1.3430, 1.3140, 1.2844, 1.2545, 1.2310, 1.2090, 1.1857, 1.1681, 1.1520]
terror2 = [0.0035, 0.0039, 0.0013, 0.0188, 0.0100, 0.0074, 0.0185, 0.0094, 0.0100, 0.0065, 0.0086, 0.0099, 0.0119, 0.0082, 0.0106, 0.0366, 0.0134, 0.0207, 0.0594, 0.0418, 0.0114, 0.0153, 0.0084, 0.0055, 0.0039, 0.0022, 0.0026, 0.0020, 0.0018, 0.0030, 0.0022, 0.0011, 0.0026, 0.0041, 0.0016, 0.0018, 0.0026, 0.0018, 0.0038, 0.0032]
cc2 = [56133442689, 56108582002, 56150434821, 56186193222, 56308750140, 56488105919, 56508108711, 56698975701, 57014899938, 56799865501, 57149223220, 57011304985, 57507011435, 57594438064, 57917123815, 58309079776, 58776238582, 59705810027, 61583612046, 62268296762, 65135633148, 67060243579, 70250040903, 72621423512, 74751337971, 77043889762, 78616627246, 80209461621, 82198802073, 84070562137, 86149552619, 87643108235, 89368603536, 90920743675, 92138087336, 93197974206, 94749010173, 95961171466, 97052008271, 98365009321]
cm2 = [11161020, 11913273, 12039025, 12092401, 12240657, 12357503, 12590612, 12802813, 13118818, 13264312, 13674034, 13691811, 14044874, 14112472, 14286048, 14481486, 14515456, 14601901, 14868582, 14718006, 15046883, 14996012, 15216304, 15352843, 15622336, 15777274, 15971704, 16081425, 16390699, 16630436, 17038407, 17203480, 17785104, 18171137, 18393681, 18371040, 18594279, 18896345, 19187226, 19427739]

nclusters = [4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64]
texecc = [0.6849, 0.8978, 1.1374, 1.2842, 1.4272, 1.6022, 1.8407, 1.9661, 2.0874, 2.2867, 2.5659, 2.6087, 2.7899, 2.9568, 3.2583, 3.2886]
terrorc = [0.0015, 0.0016, 0.0021, 0.0030, 0.0019, 0.0035, 0.0016, 0.0021, 0.0046, 0.0015, 0.0012, 0.0035, 0.0013, 0.0018, 0.0023, 0.0022]

texecc2 = [1.0209, 1.4411, 1.9171, 2.1459, 2.4971, 2.8359, 3.3127, 3.5101, 3.8504, 4.2197, 4.7697, 4.8949, 5.2303, 5.5645, 6.1707, 6.2396]
terrorc2 = [0.0065, 0.0022, 0.0032, 0.0037, 0.0059, 0.0041, 0.0062, 0.0049, 0.0034, 0.0063, 0.0075, 0.0075, 0.0079, 0.0090, 0.0082, 0.0092]


def make_plot(x, y, e, legend, xlabel, ylabel, xticks, yticks, fname, legendPos, title): 
    fig, ax = plt.subplots(figsize=(10, 5))
    colors = ['blue', 'green', 'orange', 'purple']

    if len(e) > 0:
        for i in range(len(x)):
            ax.errorbar(x[i], y[i], e[i], marker='.', ecolor='red', lw=0.75, c=colors[i])
    
    else:
        for i in range(len(x)):
            if legend[i] == 'ideal gain':
                ax.plot(x[i], y[i], marker='', lw=0.75, c='grey', ls='--')
            else:    
                ax.plot(x[i], y[i], marker='.', lw=0.75, c=colors[i])
    
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.legend(legend, loc=legendPos)

    ax.set_xticks(xticks)
    if len(yticks) > 0 :
        ax.set_yticks(yticks)

    ax.set_ylim(ymin=0)
    ax.set_xlim(xmin=0)

    #removing top and right borders
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)

    #adds major gridlines
    ax.grid(color='grey', linestyle='-', linewidth=0.25, alpha=0.5)

    fig.tight_layout()

    if(title != ""):
        ax.set_title(title)

    plt.savefig(f'{fname}.png')
    plt.show()


speedup = []
speedup2 = []
for i in texec:
    speedup.append(texec[0] / i)
for i in texec2:
    speedup2.append(texec2[0] / i)

make_plot([nthreads, nthreads2], [texec, texec2], [terror, terror2], ['4 clusters', '32 clusters'], 'Number of Threads', 'Execution Time (s)', [0] + nthreads, [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20], 'threadstime', 'upper right', "")
# make_plot([nclusters, nclusters], [texecc, texecc2], [terrorc, terrorc2], ['20 threads', '10 threads'],'Number of Clusters', 'Execution Time (s)', [0] + nclusters, [0, 1, 2, 3, 4, 5, 6], 'clusterstime', 'upper left', "")

make_plot([nthreads, nthreads2, nthreads], [speedup, speedup2, nthreads], [], ['4 clusters', '32 clusters', 'ideal gain'], 'Number of Threads', 'Speed Up', [0] + nthreads, [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40], 'speedup', 'upper left', "")

# make_plot([nthreads, nthreads], [cc, cc2], [], ['4 clusters', '32 clusters'], 'Number of Threads', 'Clock Cycles', [0] + nthreads, [], 'cc', 'upper left', "")

# make_plot([nthreads, nthreads], [cm, cm2], [], ['4 clusters', '32 clusters'], 'Number of Threads', 'Cache Misses', [0] + nthreads, [], 'cm', 'upper left', "")

# cl = [4,8,16,32]

# calculate_clusters  = [83.64, 89.24, 92.89, 95.62]
# calculate_centroids = [8.54 , 5.56 , 3.61 , 2.32]
# random              = [5.76 , 3.92 , 2.63 , 0.92]
# init_ds             = [0.83 , 0.43 , 0.30 , 0.19]
# make_plot([cl, cl, cl], [calculate_clusters, calculate_centroids, random], [], ['calculate_clusters', 'calculate_centroids', 'random'], 'Number of Clusters', 'Computional Load (%)', [0] + cl, [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100], 'grafico1', 'center right', "1 Thread")

# calculate_clusters  = [82.74, 88.65, 92.41, 95.42]
# calculate_centroids = [9.05 , 5.83 , 3.83 , 2.32]
# random              = [5.90 , 3.87 , 2.61 , 1.49]
# init_ds             = [0.71 , 0.49 , 0.33 , 0.18]
# make_plot([cl, cl, cl], [calculate_clusters, calculate_centroids, random], [], ['calculate_clusters', 'calculate_centroids', 'random'], 'Number of Clusters', 'Computional Load (%)', [0] + cl, [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100], '2threads', 'center right', '2 Threads')

# calculate_clusters  = [78.18, 85.08, 89.99, 94.46]
# calculate_centroids = [12.84, 8.60 , 5.69 , 2.98]
# random              = [5.32 , 3.47 , 2.45 , 1.54]
# init_ds             = [0.63 , 0.41 , 0.28 , 0.19]
# make_plot([cl, cl, cl], [calculate_clusters, calculate_centroids, random], [], ['calculate_clusters', 'calculate_centroids', 'random'], 'Number of Clusters', 'Computional Load (%)', [0] + cl, [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100], '4threads', 'center right', '4 Threads')
