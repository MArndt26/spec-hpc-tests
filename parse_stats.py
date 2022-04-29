#!/usr/bin/python3
import matplotlib.pyplot as plt
import os
import numpy as np


def dprint(dict):
    for key in dict:
        try:
            print("{},{}".format(dict[key][0], dict[key][1]))
        except:
            print("{},{}".format(dict[key][0], "No Data"))


def parse(filename):
    search = {
        "simSeconds": ["simSeconds"],
        "system.cache_hierarchy.ruby_system.l1_controllers0.L1Dcache.m_demand_misses": ["misses"],
        "system.cache_hierarchy.ruby_system.l1_controllers0.L1Dcache.m_demand_accesses": ["accesses"],
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.MT.L1_GETS::total": ["MT L1_GETS"],
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.MT.L1_PUTX::total": ["L1_PUTX"],
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.ISS.L1_GETS::total": ["ISS L1_GETS"],
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.ISS.Mem_Data::total": ["ISS Mem_Data"],
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.NP.Load::total": ["NP Load"],
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.IS.Data_Exclusive::total": ["IS Data_Exclusive"],
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.IS.DataS_fromL1::total": ["IS DataS_fromL1"],
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.NP.Store::total": ["NP Store"],
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.S.Load::total": ["S Load"],
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.E.Load::total": ["D Load"],
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.E.Store::total": ["E Store"],
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.M.Load::total": ["M Load"],
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.M.Store::total": ["M Store"],
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.L1_GETS::total": ["L1_GETS"],
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.L1_GETX::total": ["L1_GETX"],
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.Mem_Data::total": ["L2 Mem_Data"],
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.WB_Data_clean::total": ["L2 WB_Data_clean"]
    }
    with open(filename, "r") as stats:
        lines = stats.readlines()
        for line in lines:
            line = line.strip()
            items = line.split()
            if any(s in line for s in search.keys()):
                # print(line)
                # print(items)
                search[items[0]].append(items[1])
    return search


if __name__ == "__main__":

    benchmarks = dict.fromkeys(["cloverleaf"])

    # ps = dict.fromkeys([1, 2, 4, 8])
    ps = dict.fromkeys([1, 2])

    if not os.path.isdir("parse_out"):
        print("Creating output directory")
        os.system("mkdir parse_out")

    # dprint(stats)

    # parallel speed up graph
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_ylabel('speedup')
    ax.set_xlabel('processors')
    ax.set_title('parallel speedup')

    for b in benchmarks:
        processors = []
        speedups = []

        for p in ps:
            stats = parse("archive/{}-p{}-lock/stats.txt".format(b, p))
            processors.append(float(p))
            speedups.append(float(stats["simSeconds"][1]))

        ax.plot(processors, speedups, marker='o')

    plt.savefig('parse_out/speedup.png')
