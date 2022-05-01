all:
ifndef benchmark
	$(error benchmark is not set)
endif
ifndef cores
	$(error cores is not set)
endif
ifndef cache
	$(error cache is not set)
endif
	@echo "Starting test with $(benchmark): cores=$(cores), cache=$(cache)"
	./gem5/build/X86/gem5.opt --outdir=archive/$(benchmark)-p$(cores)-$(cache) x86-spec-hpc.py --benchmark $(benchmark) --cores $(cores) --cache-size $(cache) | awk '{ print strftime("[%Y-%m-%d %H:%M:%S]<$(benchmark)-p$(cores)-$(cache)>"), $$0 }' &>> transcript.log