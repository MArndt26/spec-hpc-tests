all: 
	@echo "Error: No option selected!"
	@echo "  mount"
	@echo "  unmount"
	@echo "  transfer"

mount:
	@echo "Mounting Image"
	cd spec-hpc-image/; mkdir spec-hpc-mnt; sudo mount -o loop,offset=1048576 spec-hpc spec-hpc-mnt

unmount:
	@echo "Un-Mounting Image"
	cd spec-hpc-image; sudo umount spec-hpc-mnt

transfer:
	@echo "Transfering image to saguaro1"
	cd spec-hpc-image/; scp spec-hpc arndt20@saguaro1.ecn.purdue.edu:/home/shay/a/arndt20/ece666/spec-hpc-tests/disk-image/spec-hpc/spec-hpc-image/spec-hpc