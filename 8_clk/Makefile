CROSS_COMPILE		?= arm-none-linux-gnueabihf-
TARGET				?= clk

CC					:= $(CROSS_COMPILE)gcc
LD					:= $(CROSS_COMPILE)ld
OBJCOBY				:= $(CROSS_COMPILE)objcopy
OBJDUMP				:= $(CROSS_COMPILE)objdump

INCDIRS 			:=	imx6ull\
						bsp/clk\
						bsp/led\
						bsp/delay\
						bsp/beep\
						bsp/gpio\
						bsp/key

SRCDIRS            :=	project\
						bsp/clk\
						bsp/led\
						bsp/delay\
						bsp/beep\
						bsp/gpio\
						bsp/key

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
	$(LD) -Timx6ull.lds -o $(TARGET).elf $^
	$(OBJCOBY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(COBJS) $(SOBJS) load.imx