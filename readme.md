## Setup
1) Obtain copy of hpc2021-1.0.3.iso.xz (or newer version) from SPEC
   1) Note: this follows [this](https://www.spec.org/hpg/hpc2021/Docs/install-guide-linux.html#mount) tutorial for installation
2) Copy compressed file into disk-image/hpc/
3) Decompress `xz -d hpc2021-1.0.3.iso.xz`
4) mount to the extracted .iso `mount -t iso9660 -o ro,loop hpc2021-1.0.n.iso /mnt`
5) change to the mount directory `cd /mnt`
6) `./install.sh`
   1) you will be prompted for the install directory: enter /path/to/
7) Change to the top-level spec directory `cd /spec-hpc-tests/disk-image/hpc`
8) source the env vars `. ./shrc` -> note dot-space-dot-slash-shrc
9) unmount the .iso `sudo umount /mnt` -> note: `u`mount is not a typo for `un`mount

## Running SPEC tests
1) cd $SPEC/config
2) cp default.cfg test.cfg
3) runhpc --config=test.cfg --action=build --tune=base -ranks 40 505.lbm_t
4) runhpc --config=test.cfg -ranks 40 --size=test --noreportable --tune=base --iterations=1 505.lbm_t
   1) This is causing errors right now //FIXME: NEED TO FIGURE OUT HOW TO FIX THIS

## Notes

- changed /proc/sys/kernel/perf_event_paranoid from 4 to 1

## TODO
- add ROI to the tests
- figure out how to run benchmark tests
  - run_test.sh will run the tealeaf 2d benchmark
- create disk with benchmark tests
  - so far I have added tealeaf 2d
- figure out how to parse data from stats.txt (m5.stats.dump())

## Building disk image
1) modify spec-hpc.json to contain files to move to image
```json
   {
      "type": "file",
      "source": "spec-hpc/TeaLeaf/2d",
      "destination": "/home/gem5/"
    }
```
2) modify spec-hpc-install.sh to contain shell commands to build binaries for your benchmark
3) check if valid `./packer validate spec-hpc/spec-hpc.json`
4) build image `./packer build spec-hpc/spec-hpc.json`
5) check size of disk image (in kB) `du -s --apparent-size spec-hpc/spec-hpc-image/spec-hpc`

## Run Tests
```bash
# run system configuration
./gem5/build/X86/gem5.opt x86-spec-hpc-benchmarks.py --benchmark <name>
# view run output
cat gem5/m5out/board.pc.com_1.device | less +G
```

## Parsing stat.txt
- speedup
  - simSeconds
  - simTicks
  - numCycles - expect to see all cores doing work
- miss rate
  - m_demand_misses / m_demand_accesses ?? maybe, currently there are 0's for both
- data sharing
  - //TODO: SHOULD THIS BE L1 OR L2?
  - see transition states [here](https://www.gem5.org/documentation/general_docs/ruby/MESI_Two_Level/)
  - L2Cache_Controller.MT.L1_GETS::total --> provide block to another core with S permissions
  - L2Cache_Controller.MT.L1_PUTX::total --> provide block then invalidate
  - L2Cache_Controller.ISS.L1_GETS::total --> another core wants same block in S state
  - L2Cache_Controller.ISS.Mem_Data::total
  - 
  - L1Cache_Controller.NP.Load --> private read
    - L1Cache_Controller.IS.Data_Exclusive `use this instead -- private read`
    - L1Cache_Controller.IS.DataS_fromL1 `use this instead -- shared read`
  - L1Cache_Controller.NP.Store --> private write
  - L1Cache_Controller.S.Load --> shared read
  - L1Cache_Controller.E.Load -> private read
  - L1Cache_Controller.E.Store --> private write
  - L1Cache_Controller.M.Load --> private read
  - L1Cache_Controller.M.Store --> private write
- off chip traffic
  - //TODO: I DON'T THINK THIS IS REALLY 'OFF CHIP'
  - simInsts
  - L2Cache_Controller.L1_GETS::total --> loads
  - L2Cache_Controller.L1_GETX::total --> stores
  - L2Cache_Controller.Mem_Data::total
  - L2Cache_Controller.WB_Data_clean::total --> wb

## Tmux
useful for creating a shell that will persist when a ssh session is closed

- create new tmux window session
  - `tmux`
- detach window:
  - <kbd>ctrl</kbd> + <kbd>b</kbd> then <kbd>d</kbd>
- kill window:
  - <kbd>ctrl</kbd> + <kbd>b</kbd> then <kbd>&</kbd> then confirm with <kbd>y</kbd>
- name window:
  - <kbd>ctrl</kbd> + <kbd>b</kbd> then <kbd>$</kbd> then type `custom_name`
- list sessions
  - `tmux ls`
- attach to tmux terminal
  - `tmux attach`
  - `tmux attach -t <session name>`

## Mounting to drive help
- [check disk size](https://unix.stackexchange.com/questions/398033/how-to-see-determine-on-disk-file-size-on-linux)
- [find partition](https://ubuntuforums.org/archive/index.php/t-1576011.html#:~:text=To%20mount%20a,img%20mount/point)
  - `sudo mount -o loop,offset=1048576 spec-hpc.img /mnt`
- `losetup -a` to list all loop devices
  - find all instances of loop and remove with `sudo losetup -d /dev/loop25`
- `lsblk` to list all devices mounted

## Resources
- https://www.spec.org/hpg/hpc2021/Docs/quick-start.html


## I think I may have found the bug.

The last line in `/home/gem5project/spec-hpc-tests/m5out/system.pc.com_1.device` says:
`./script.sh: line 1: /home/gem5/spec-hpc/bin/tealeaf.base.x: No such file or directory`


## uncomment in line 178 x86-spec-hpc-benchmarks.py, the simulation runs.