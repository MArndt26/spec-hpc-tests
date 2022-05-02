#!/usr/bin/python3
import numpy as np
import os
from plot_scripts.speedup import speedup
from plot_scripts.on_chip_traffic import on_chip_traffic


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
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.L2_Replacement::total": "l2 stores",
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.Mem_Data::total": "l2 loads",
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.WB_Data::total": "l2 wb",
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

    if not os.path.isdir("parse_out"):
        print("Creating output directory")
        os.system("mkdir parse_out")

    ps = [1, 2, 4, 8]
    benchmarks = ["cloverleaf", "cloverleaf_copy"]
    data = {}

    for b in benchmarks:
        data[b] = {}
        for p in ps:
            name = "{}-p{}-32kB".format(b, p)
            stats = parse(
                "archive-remote/archive/{}-lock/stats.txt".format(name))
            data[b][name] = stats

    speedup(data)  # Plot parallel speedup

    on_chip_traffic(data)  # Plot on-chip traffic
