all: 
	@echo "Error: No option selected!"

mount:
	@echo "Mounting Image"
	cd image/; mkdir image-mnt; sudo mount -o loop,offset=1048576 image image-mnt

transfer:
	@echo "Transfering image to saguaro1"
	cd image/; scp image arndt20@saguaro1.ecn.purdue.edu:/home/shay/a/arndt20/ece666/spec-hpc-tests/disk-image/from-src/from-src-image