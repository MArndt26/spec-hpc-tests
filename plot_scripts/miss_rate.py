#!/usr/bin/python3
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np


def miss_rate(data):
    '''
    miss rate over cache sweep
    '''
    fig, ax = plt.subplots()

    caches = np.array([])
    misses = np.array([])

    for b in sorted(data):
        group = data[b]
        for g in group:
            params = g.split("-")
            stats = group[g]
            c = params[2][:-2]  # cache size

            caches = np.append(caches, c)
            misses = np.append(
                misses, stats["misses"] / stats["accesses"] * 100)

        if b != sorted(data)[-1]:
            # Add spacing
            caches = np.append(caches, "")
            misses = np.append(misses, 0)

    width = 0.3
    x = np.arange(len(caches))  # the label locations
    ax.bar(x, misses, width, label='misses')

    handles, labels = ax.get_legend_handles_labels()
    ax.legend(handles[::-1], labels[::-1],
              bbox_to_anchor=(1.04, 0.5), loc='upper left')
    ax.set_xticks(x, caches)
    ax.yaxis.set_major_formatter(ticker.PercentFormatter())

    ax.set_ylabel('Miss Rate (%)')
    ax.set_title('Cache Miss Rate')

    # label the benchmarks
    # TODO: THIS IS HARDCORED TO TWO BENCHMARKS, MAYBE MAKE THIS FLEXIBLE FOR ANY NUMBER OF BENCHMARKS
    ax2 = ax.twiny()

    ax2.spines["bottom"].set_position(("axes", -0.10))
    ax2.tick_params('both', length=0, width=0, which='minor')
    ax2.tick_params('both', direction='in', which='major')
    ax2.xaxis.set_ticks_position("bottom")
    ax2.xaxis.set_label_position("bottom")

    ax2.set_xticks([0.0, 0.5, 1.0])
    ax2.xaxis.set_major_formatter(ticker.NullFormatter())
    ax2.xaxis.set_minor_locator(ticker.FixedLocator([0.25, 0.75]))
    ax2.xaxis.set_minor_formatter(
        ticker.FixedFormatter(sorted(data)))

    fig.tight_layout()

    plt.savefig('parse_out/miss_rate.png')
