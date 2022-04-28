all: 
	@echo "Error: No option selected!"
	@echo "  mount"
	@echo "  umount"
	@echo "  transfer"

mount:
	@echo "Mounting Image"
	cd cloverleaf-image/; mkdir cloverleaf-mnt; sudo mount -o loop,offset=1048576 cloverleaf cloverleaf-mnt

unmount:
	@echo "Un-Mounting Image"
	cd cloverleaf-image; sudo umount cloverleaf-mnt

transfer:
	@echo "Transfering image to saguaro1"
	cd image/; scp image arndt20@saguaro1.ecn.purdue.edu:/home/shay/a/arndt20/ece666/spec-hpc-tests/disk-image/cloverleaf/cloverleaf-image/cloverleaf