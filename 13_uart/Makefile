CROSS_COMPILE		?= arm-none-linux-gnueabihf-
TARGET				?= uart

CC					:= $(CROSS_COMPILE)gcc
LD					:= $(CROSS_COMPILE)ld
OBJCOBY				:= $(CROSS_COMPILE)objcopy
OBJDUMP				:= $(CROSS_COMPILE)objdump

LIBPATH             := -lgcc -L /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/lib/gcc/arm-none-linux-gnueabihf/10.3.1

INCDIRS 			:=	imx6ull\
						stdio/inlude\
						bsp/clk\
						bsp/led\
						bsp/delay\
						bsp/beep\
						bsp/gpio\
						bsp/key\
						bsp/exit\
						bsp/int \
						bsp/epittimer\
						bsp/keyfilter\
						bsp/uart

SRCDIRS            :=	project\
						stdio/lib\
						bsp/clk\
						bsp/led\
						bsp/delay\
						bsp/beep\
						bsp/gpio\
						bsp/key\
						bsp/exit\
						bsp/int\
						bsp/epittimer\
						bsp/keyfilter\
						bsp/uart

INCLUDE    			:= $(patsubst %, -I %, $(INCDIRS))

SFILES              := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES              := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR           := $(notdir $(SFILES))
CFILENDIR           := $(notdir $(CFILES))


SOBJS               := $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS               := $(patsubst %, obj/%, $(CFILENDIR:.c=.o))
OBJS				:= $(SOBJS) $(COBJS)

VPATH				:= $(SRCDIRS)

.PHONY: clean

$(TARGET).bin : $(OBJS)
	$(LD) -Timx6ull.lds -o $(TARGET).elf $^ $(LIBPATH)
	$(OBJCOBY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) -Wall  -Wa,-mimplicit-it=thumb -nostdlib -fno-builtin -c -O2 $(INCLUDE) -o $@ $<

clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(COBJS) $(SOBJS) load.imx