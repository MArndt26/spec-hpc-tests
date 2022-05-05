#!/usr/bin/python3
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np


def on_chip_traffic(data, out_file):
    '''
    on chip traffic plot
    '''
    print("Plotting On Chip Traffic")
    fig, ax = plt.subplots()

    processors = np.array([])
    s_writes = np.array([])
    p_writes = np.array([])
    s_reads = np.array([])
    p_reads = np.array([])

    for b in sorted(data):
        group = data[b]
        for g in group:
            params = g.split("-")
            stats = group[g]
            p = params[1][-1]

            instr = stats["simInstr"] / 64
            processors = np.append(processors, p)
            s_writes = np.append(s_writes, stats["shared writes"]/instr)
            s_reads = np.append(s_reads, stats["shared reads"]/instr)
            p_writes = np.append(p_writes, stats["private writes"]/instr)
            p_reads = np.append(p_reads, stats["private reads"]/instr)

        if b != sorted(data)[-1]:
            # Add spacing
            processors = np.append(processors, "")
            s_writes = np.append(s_writes, 0)
            s_reads = np.append(s_reads, 0)
            p_writes = np.append(p_writes, 0)
            p_reads = np.append(p_reads, 0)

    width = 0.3
    x = np.arange(len(processors))  # the label locations
    ax.bar(x, p_reads, width, label='private reads')
    ax.bar(x, p_writes, width, label='private writes', bottom=p_reads)
    ax.bar(x, s_reads, width,
           label='shared reads', bottom=p_reads+p_writes)
    ax.bar(x, s_writes, width, label='shared writes',
           bottom=p_reads+p_writes+s_reads)

    handles, labels = ax.get_legend_handles_labels()
    ax.legend(handles[::-1], labels[::-1],
              bbox_to_anchor=(1.04, 0.5), loc='upper left')
    ax.set_xticks(x, processors)

    ax.set_ylabel('Traffic (bytes/instr)')
    ax.set_title('On-Chip Traffic')
    # ax.set_ylim([0, 15])

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

    plt.savefig('parse_out/{}.png'.format(out_file))
