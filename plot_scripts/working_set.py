#!/usr/bin/python3
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np


def working_set(data, benchmark):
    '''
    plot of cache working set for cloverleaf
    '''
    fig, ax = plt.subplots()

    caches = np.array([])
    misses = np.array([])

    for d in data:
        params = d.split("-")
        stats = data[d]
        c = params[2][:-2]  # cache size

        caches = np.append(caches, c)
        misses = np.append(
            misses, stats["misses"] / stats["accesses"] * 100)

    width = 0.3
    x = np.arange(len(caches))  # the label locations
    ax.plot(x, misses, width, label='misses')

    ax.set_xticks(x, caches)
    ax.yaxis.set_major_formatter(ticker.PercentFormatter())

    ax.set_ylabel('Miss Rate (%)')
    ax.set_xlabel('Cache Size (kB)')

    ax.set_title(benchmark)

    fig.tight_layout()
    return ax, fig


def cl_working_set(data):
    print("Plotting Cloverleaf Working Set")
    ax, fig = working_set(data, "Cloverleaf")

    ax.annotate('WS1',
                xy=(1, 10), xycoords='data',
                xytext=(1.1, 15), fontsize=22, textcoords='data',
                arrowprops=dict(
                    connectionstyle="arc3,rad=0",
                    shrinkA=0, shrinkB=10,
                    arrowstyle='-|>', ls='-', linewidth=1
                ),
                va='bottom', ha='left', zorder=10
                )

    ax.annotate('WS2',
                xy=(3, 6.8), xycoords='data',
                xytext=(3, 15), fontsize=22, textcoords='data',
                arrowprops=dict(
                    connectionstyle="arc3,rad=0",
                    shrinkA=0, shrinkB=10,
                    arrowstyle='-|>', ls='-', linewidth=1
                ),
                va='bottom', ha='left', zorder=10
                )

    plt.savefig('parse_out/cl_working_set.png')


def tl_working_set(data):
    print("Plotting Tealeaf Working Set")
    ax, fig = working_set(data, "Tealeaf")

    ax.annotate('WS1',
                xy=(1, 10), xycoords='data',
                xytext=(1.1, 20), fontsize=22, textcoords='data',
                arrowprops=dict(
                    connectionstyle="arc3,rad=0",
                    shrinkA=0, shrinkB=10,
                    arrowstyle='-|>', ls='-', linewidth=1
                ),
                va='bottom', ha='left', zorder=10
                )

    ax.annotate('WS2',
                xy=(5, 9.5), xycoords='data',
                xytext=(4, 20), fontsize=22, textcoords='data',
                arrowprops=dict(
                    connectionstyle="arc3,rad=0",
                    shrinkA=0, shrinkB=10,
                    arrowstyle='-|>', ls='-', linewidth=1
                ),
                va='bottom', ha='left', zorder=10
                )

    plt.savefig('parse_out/tl_working_set.png')
