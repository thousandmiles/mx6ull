led.bin:led.s
	arm-none-linux-gnueabihf-gcc -g -c led.s -o led.o
	arm-none-linux-gnueabihf-ld -Ttext 0X87800000 led.o -o led.elf
	arm-none-linux-gnueabihf-objcopy -O binary -S -g led.elf led.bin

clean:
	rm -rf *.o led.elf led.dis load.imx