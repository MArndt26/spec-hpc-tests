#!/usr/bin/python3
import matplotlib.pyplot as plt


def speedup(data):
    '''
    parallel speedup plot
    '''
    fig, ax = plt.subplots()

    for b in sorted(data):
        processors = []
        speedups = []
        ref_time = None
        group = data[b]
        for g in group:
            params = g.split("-")
            stats = group[g]
            p = int(params[1][-1])
            if p == 1:
                ref_time = stats["simSeconds"]
            processors.append(p)
            speedups.append(ref_time / stats["simSeconds"])

        ax.plot(processors, speedups, marker='o', label=b)

    ax.set_ylabel('speedup')
    ax.set_xlabel('processors')
    ax.set_title('parallel speedup')
    ax.legend()
    plt.savefig('parse_out/speedup.png')
