#!/usr/bin/python3
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import os


def dprint(dict):
    for key in dict:
        try:
            print("{},{}".format(key, dict[key]))
        except:
            print("{},{}".format(key, "No Data"))


def parse(filename):
    search = {
        "simSeconds": "simSeconds",
        "simInsts": "simInstr",
        "system.cache_hierarchy.ruby_system.l1_controllers0.L1Dcache.m_demand_misses": "misses",
        "system.cache_hierarchy.ruby_system.l1_controllers0.L1Dcache.m_demand_accesses": "accesses",
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.Inv::total": "shared writes",
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.M.Store::total": "private writes",
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.E.Load::total": "private reads",
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.S.Load::total": "shared reads",
        # "system.cache_hierarchy.ruby_system.L2Cache_Controller.MT.L1_GETS::total": "MT L1_GETS",
        # "system.cache_hierarchy.ruby_system.L2Cache_Controller.MT.L1_PUTX::total": "L1_PUTX",
        # "system.cache_hierarchy.ruby_system.L2Cache_Controller.ISS.L1_GETS::total": "ISS L1_GETS",
        # "system.cache_hierarchy.ruby_system.L2Cache_Controller.ISS.Mem_Data::total": "ISS Mem_Data",
        # "system.cache_hierarchy.ruby_system.L1Cache_Controller.NP.Load::total": "NP Load",
        # "system.cache_hierarchy.ruby_system.L1Cache_Controller.IS.Data_Exclusive::total": "IS Data_Exclusive",
        # "system.cache_hierarchy.ruby_system.L1Cache_Controller.IS.DataS_fromL1::total": "IS DataS_fromL1",
        # "system.cache_hierarchy.ruby_system.L1Cache_Controller.NP.Store::total": "NP Store",
        # "system.cache_hierarchy.ruby_system.L1Cache_Controller.S.Load::total": "S Load",
        # "system.cache_hierarchy.ruby_system.L1Cache_Controller.E.Load::total": "D Load",
        # "system.cache_hierarchy.ruby_system.L1Cache_Controller.E.Store::total": "E Store",
        # "system.cache_hierarchy.ruby_system.L1Cache_Controller.M.Load::total": "M Load",
        # "system.cache_hierarchy.ruby_system.L1Cache_Controller.M.Store::total": "M Store",
        # "system.cache_hierarchy.ruby_system.L2Cache_Controller.L1_GETS::total": "L1_GETS",
        # "system.cache_hierarchy.ruby_system.L2Cache_Controller.L1_GETX::total": "L1_GETX",
        # "system.cache_hierarchy.ruby_system.L2Cache_Controller.Mem_Data::total": "L2 Mem_Data",
        # "system.cache_hierarchy.ruby_system.L2Cache_Controller.WB_Data_clean::total": "L2 WB_Data_clean"
    }
    with open(filename, "r") as stats:
        lines = stats.readlines()
        results = {}
        for line in lines:
            line = line.strip()
            items = line.split()
            if any(s in line for s in search.keys()):
                # print(line)
                # print(items)
                results[search[items[0]]] = float(items[1])
    return results


if __name__ == "__main__":

    benchmarks = dict.fromkeys(["cloverleaf", "cloverleaf-copy"])

    ps = dict.fromkeys([1, 2, 4, 8])

    if not os.path.isdir("parse_out"):
        print("Creating output directory")
        os.system("mkdir parse_out")

    # parallel speed up graph
    fig = plt.figure()
    ax = fig.add_subplot(111)

    for b in benchmarks:
        processors = []
        speedups = []
        ref_time = None

        for p in sorted(ps):
            stats = parse(
                "archive-remote/archive/{}-p{}-32kB-lock/stats.txt".format(b, p))
            # dprint(stats)
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

    # On Chip Traffic
    fig, ax = plt.subplots()

    processors = np.array([])
    s_writes = np.array([])
    p_writes = np.array([])
    s_reads = np.array([])
    p_reads = np.array([])

    for b in sorted(benchmarks):
        for p in ["1", "2", "4", "8"]:
            stats = parse(
                "archive-remote/archive/{}-p{}-32kB-lock/stats.txt".format(b, p))
            instr = 1
            instr = stats["simInstr"] / 4  # TODO: UPDATE THIS FOR BYTES/INSTR
            processors = np.append(processors, p)
            s_writes = np.append(s_writes, stats["shared writes"]/instr)
            s_reads = np.append(s_reads, stats["shared reads"]/instr)
            p_writes = np.append(p_writes, stats["private writes"]/instr)
            p_reads = np.append(p_reads, stats["private reads"]/instr)

        if b != sorted(benchmarks.keys())[-1]:
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

    ax.set_ylabel('Traffic (accesses)')
    ax.set_title('On-Chip Traffic')

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
        ticker.FixedFormatter(sorted(benchmarks.keys())))

    fig.tight_layout()

    plt.savefig('parse_out/on-chip-traffic.png')
