#!/usr/bin/python3
import os
from plot_scripts.speedup import speedup
from plot_scripts.on_chip_traffic import on_chip_traffic
from plot_scripts.off_chip_traffic import off_chip_traffic
from plot_scripts.miss_rate import miss_rate
from plot_scripts.working_set import cl_working_set, tl_working_set


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
        "system.cache_hierarchy.ruby_system.l1_controllers0.L1Dcache.m_demand_misses": "p0 misses",
        "system.cache_hierarchy.ruby_system.l1_controllers1.L1Dcache.m_demand_misses": "p1 misses",
        "system.cache_hierarchy.ruby_system.l1_controllers2.L1Dcache.m_demand_misses": "p2 misses",
        "system.cache_hierarchy.ruby_system.l1_controllers3.L1Dcache.m_demand_misses": "p3 misses",
        "system.cache_hierarchy.ruby_system.l1_controllers4.L1Dcache.m_demand_misses": "p4 misses",
        "system.cache_hierarchy.ruby_system.l1_controllers5.L1Dcache.m_demand_misses": "p5 misses",
        "system.cache_hierarchy.ruby_system.l1_controllers6.L1Dcache.m_demand_misses": "p6 misses",
        "system.cache_hierarchy.ruby_system.l1_controllers7.L1Dcache.m_demand_misses": "p7 misses",
        "system.cache_hierarchy.ruby_system.l1_controllers0.L1Dcache.m_demand_accesses": "p0 accesses",
        "system.cache_hierarchy.ruby_system.l1_controllers1.L1Dcache.m_demand_accesses": "p1 accesses",
        "system.cache_hierarchy.ruby_system.l1_controllers2.L1Dcache.m_demand_accesses": "p2 accesses",
        "system.cache_hierarchy.ruby_system.l1_controllers3.L1Dcache.m_demand_accesses": "p3 accesses",
        "system.cache_hierarchy.ruby_system.l1_controllers4.L1Dcache.m_demand_accesses": "p4 accesses",
        "system.cache_hierarchy.ruby_system.l1_controllers5.L1Dcache.m_demand_accesses": "p5 accesses",
        "system.cache_hierarchy.ruby_system.l1_controllers6.L1Dcache.m_demand_accesses": "p6 accesses",
        "system.cache_hierarchy.ruby_system.l1_controllers7.L1Dcache.m_demand_accesses": "p7 accesses",
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.Inv::total": "shared writes",
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.M.Store::total": "private writes",
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.E.Load::total": "e reads",
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.M.Load::total": "m reads",
        "system.cache_hierarchy.ruby_system.L1Cache_Controller.S.Load::total": "shared reads",
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.L2_Replacement::total": "l2 stores",
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.Mem_Data::total": "l2 loads",
        "system.cache_hierarchy.ruby_system.L2Cache_Controller.WB_Data::total": "l2 wbs",
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

    results["misses"] = 0
    results["accesses"] = 0
    for i in range(0, 8):
        results["misses"] += results["p{} misses".format(i)]
        results["accesses"] += results["p{} accesses".format(i)]

    results['private reads'] = results['e reads'] + results["m reads"]
    return results


if __name__ == "__main__":

    if not os.path.isdir("parse_out"):
        print("Creating output directory")
        os.system("mkdir parse_out")

    benchmarks = ["cloverleaf", "tealeaf"]

    base_path = "archive-v0.5/archive/"
    print("Plotting from {}".format(base_path))

    p_data = {}  # parallel speedup data
    ps = [1, 2, 4, 8]

    for b in benchmarks:
        p_data[b] = {}
        for p in ps:
            name = "{}-p{}-32kB".format(b, p)
            stats = parse(
                "{}{}-lock/stats.txt".format(base_path, name))
            # "archive-old/{}-lock/stats.txt".format(name))
            p_data[b][name] = stats

    speedup(p_data)  # Plot parallel speedup

    off_chip_traffic(p_data)  # Plot off-chip traffic

    on_chip_traffic(p_data, "on-chip-traffic")  # Plot on-chip traffic

    p_data["tealeaf"]["tealeaf-p1-32kB"]['private reads'] = p_data["tealeaf"]["tealeaf-p2-32kB"]['private reads']
    on_chip_traffic(p_data, "on-chip-traffic-amended")  # Plot on-chip traffic

    c_data = {}  # cache sweep data
    cs = [8, 32, 128, 512, 2048, 4096]

    for b in benchmarks:
        c_data[b] = {}
        for c in cs:
            name = "{}-p4-{}kB".format(b, c)
            stats = parse(
                "{}{}-lock/stats.txt".format(base_path, name))
            c_data[b][name] = stats
            # print("\n" + name)
            # dprint(c_data[b][name])

    miss_rate(c_data)  # Plot cache miss rate over cache sweep

    cl_working_set(c_data["cloverleaf"])

    tl_working_set(c_data["tealeaf"])
