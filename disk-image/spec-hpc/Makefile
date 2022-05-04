all: 
	@echo "Error: No option selected!"
	@echo "  mount"
	@echo "  unmount"

mount:
	@echo "Mounting Image"
	cd spec-hpc-image/; mkdir spec-hpc-mnt; sudo mount -o loop,offset=1048576 spec-hpc spec-hpc-mnt

unmount:
	@echo "Un-Mounting Image"
	cd spec-hpc-image; sudo umount spec-hpc-mnt