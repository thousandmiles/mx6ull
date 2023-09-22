objs := start.o main.o

CROSS_COMPILE := arm-none-linux-gnueabihf-

ledc.bin : $(objs)
	$(CROSS_COMPILE)ld -Ttext 0X87800000 -o ledc.elf $^
	$(CROSS_COMPILE)objcopy -O binary -S ledc.elf $@
	$(CROSS_COMPILE)objdump -D -m arm ledc.elf > ledc.dis

%.o : %.s 
	$(CROSS_COMPILE)gcc -Wall -nostdlib -c -o $@ $<

%.o : %.S
	$(CROSS_COMPILE)gcc -Wall -nostdlib -c -o $@ $<

%.o : %.c 
	$(CROSS_COMPILE)gcc -Wall -nostdlib -c -o $@ $<

clean:
	rm -rf *.o ledc.elf ledc.dis ledc.bin load.imx