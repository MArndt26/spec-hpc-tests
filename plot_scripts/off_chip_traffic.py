#!/usr/bin/python3
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np


def off_chip_traffic(data):
    '''
    off chip traffic plot
    TODO: this is basically the same as on-chip traffic, should generalize this code for reuse
    '''
    fig, ax = plt.subplots()

    processors = np.array([])
    wbs = np.array([])
    stores = np.array([])
    loads = np.array([])

    for b in sorted(data):
        group = data[b]
        for g in group:
            params = g.split("-")
            stats = group[g]
            p = params[1][-1]

            instr = stats["simInstr"] / 64
            processors = np.append(processors, p)
            wbs = np.append(wbs, stats["l2 wbs"]/instr)
            stores = np.append(stores, stats["l2 stores"]/instr)
            loads = np.append(loads, stats["l2 loads"]/instr)

        if b != sorted(data)[-1]:
            # Add spacing
            processors = np.append(processors, "")
            wbs = np.append(wbs, 0)
            stores = np.append(stores, 0)
            loads = np.append(loads, 0)

    width = 0.3
    x = np.arange(len(processors))  # the label locations
    ax.bar(x, loads, width, label='loads')
    ax.bar(x, stores, width, label='stores', bottom=loads)
    ax.bar(x, wbs, width,
           label='writebacks', bottom=loads+stores)

    handles, labels = ax.get_legend_handles_labels()
    ax.legend(handles[::-1], labels[::-1],
              bbox_to_anchor=(1.04, 0.5), loc='upper left')
    ax.set_xticks(x, processors)

    ax.set_ylabel('Traffic (accesses)')
    ax.set_title('Off-Chip Traffic')

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

    plt.savefig('parse_out/off-chip-traffic.png')
