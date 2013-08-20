all: bone_pru0_out-00A0.dtbo

install:
	cp -v *.dtbo /lib/firmware
	
clean: 
	rm *.dtbo

%.dtbo : %.dts
	dtc -O dtb -o $@ -b 0 -@ $<