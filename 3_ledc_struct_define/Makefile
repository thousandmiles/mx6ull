objs := start.o main.o

ledc.bin : $(objs)
	arm-none-linux-gnueabihf-ld -Ttext 0X87800000 -o ledc.elf $^
	arm-none-linux-gnueabihf-objcopy -O binary -S ledc.elf $@
	arm-none-linux-gnueabihf-objdump -D -m arm ledc.elf > ledc.dis

%.o : %.s 
	arm-none-linux-gnueabihf-gcc -Wall -nostdlib -c -o $@ $<

%.o : %.S
	arm-none-linux-gnueabihf-gcc -Wall -nostdlib -c -o $@ $<

%.o : %.c 
	arm-none-linux-gnueabihf-gcc -Wall -nostdlib -c -o $@ $<

clean:
	rm -rf *.o ledc.elf ledc.dis ledc.bin load.imx