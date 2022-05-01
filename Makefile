out_dir = archive/$(benchmark)-p$(cores)-$(cache)
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
	./gem5/build/X86/gem5.opt --outdir=$(out_dir) x86-spec-hpc.py --benchmark $(benchmark) --cores $(cores) --cache-size $(cache) | awk '{ print strftime("[%Y-%m-%d %H:%M:%S]<$(benchmark)-p$(cores)-$(cache)>"), $$0; fflush()  }' >> transcript.log
	mv $(out_dir) $(out_dir)-lock 
	chmod a-w $(out_dir)-lock

pull_archive:
ifndef version
	$(error version is not set)
endif
	wget https://github.com/MArndt26/spec-hpc-tests/releases/download/$(version)/archive.zip
	unzip archive.zip -d archive-remote