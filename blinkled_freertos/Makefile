############################################################################
#
#    Mbed LPC1768 Makefile
#
#    By Sergio Prado, Embedded Labworks, Brazil
#    <sergio.prado@e-labworks.com>
#
############################################################################

# project name
PRJNAME = blinkled

# toolchain configuration
PATH := ${PATH}:/opt/toolchain/arm-bare_newlib_cortex_m3_nommu-eabi/bin/
TOOLCHAIN_PREFIX = arm-cortexm3-bare-

# cpu configuration
THUMB = -mthumb
MCU   = cortex-m3

# project directories
PRJDIR := $(shell pwd)
OUTDIR := $(PRJDIR)/out
SRCDIR := $(PRJDIR)/src
INCDIR := $(PRJDIR)/inc
LIBDIR := $(PRJDIR)/lib

# CMSIS directory
CMSISDIR    := $(PRJDIR)/../../cmsis/
CMSISSRCDIR := $(CMSISDIR)/src/
CMSISINCDIR := $(CMSISDIR)/inc/

# FreeRTOS directories
FRTDIR    := $(PRJDIR)/../../freertos
FRTSRCDIR := $(FRTDIR)/Source
FRTINCDIR := $(FRTSRCDIR)/include
FRTMEMDIR := $(FRTSRCDIR)/portable/MemMang/
FRTPORDIR := $(FRTSRCDIR)/portable/GCC/ARM_CM3


# C source files
C_SRC := $(CMSISSRCDIR)/core_cm3.c $(CMSISSRCDIR)/system_LPC17xx.c $(SRCDIR)/main.c \
         $(SRCDIR)/startup.c $(FRTSRCDIR)/list.c $(FRTSRCDIR)/queue.c $(FRTSRCDIR)/tasks.c \
         $(FRTPORDIR)/port.c $(FRTMEMDIR)/heap_2.c

# Assembler source files
A_SRC = 

# include directories
INCDIRS = $(INCDIR) $(CMSISINCDIR) $(FRTINCDIR) $(FRTPORDIR) $(SRCDIR)

# C flags
CFLAGS += -mcpu=$(MCU)
#CFLAGS += -Wall -Wextra -Werror -Wcast-align -Wpointer-arith -Wredundant-decls -Wcast-qual
CFLAGS += -Wall -Wextra -Wcast-align -Wpointer-arith -Wredundant-decls -Wcast-qual
CFLAGS += -Wa,-adhlns=$(addprefix $(OUTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))
CFLAGS += -MMD -MP -MF $(OUTDIR)/dep/$(@F).d
CFLAGS += -I. $(patsubst %,-I%,$(INCDIRS))

# Assembler flags
AFLAGS  = -mcpu=$(MCU) -x assembler
AFLAGS += -D__ASSEMBLY__
AFLAGS += -Wa,-adhlns=$(addprefix $(OUTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))
AFLAGS += -I. $(patsubst %,-I%,$(INCDIRS))

# Linker flags
LDFLAGS = -Wl,-Map=$(OUTDIR)/$(PRJNAME).map,--cref
LDFLAGS += -L. $(patsubst %,-L%,$(LIBDIR))
LDFLAGS += -lc -lgcc
LDFLAGS +=-T$(PRJDIR)/lpc1768.ld

# Define programs and commands.
CC      = $(TOOLCHAIN_PREFIX)gcc
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
NM      = $(TOOLCHAIN_PREFIX)nm

# List of all source files.
ALL_SRC = $(A_SRC) $(C_SRC)

# List of all source files without directory and file-extension.
ALL_SRC_BASE = $(notdir $(basename $(ALL_SRC)))

# Define all object files.
ALL_OBJ = $(addprefix $(OUTDIR)/, $(addsuffix .o, $(ALL_SRC_BASE)))

# build again when Makefile changes
BUILDONCHANGE = Makefile 

# Default target.
all: createdirs build

createdirs:
	@mkdir -p $(OUTDIR)
	@mkdir -p $(OUTDIR)/dep

# targets for the build-sequence
build: elf lss sym hex bin

# output files
elf: $(OUTDIR)/$(PRJNAME).elf
lss: $(OUTDIR)/$(PRJNAME).lss 
sym: $(OUTDIR)/$(PRJNAME).sym
hex: $(OUTDIR)/$(PRJNAME).hex
bin: $(OUTDIR)/$(PRJNAME).bin

# Create final output file in ihex format from ELF output file (.hex).
%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@
	
# Create final output file in raw binary format from ELF output file (.bin)
%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

# Create extended listing file/disassambly from ELF output file.
# using objdump (testing: option -C)
%.lss: %.elf
	$(OBJDUMP) -h -S -C -r $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	$(NM) -n $< > $@

# Link: create ELF output file from object files.
.SECONDARY : $(PRJNAME).elf
.PRECIOUS : $(ALL_OBJ)
%.elf:  $(ALL_OBJ) $(BUILDONCHANGE)
	$(CC) $(THUMB) $(CFLAGS) $(ALL_OBJ) --output $@ -nostartfiles $(LDFLAGS)

# compile asm source code
define ASSEMBLE_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1) $(BUILDONCHANGE)
	$(CC) -c $(THUMB) $$(AFLAGS) $$< -o $$@ 
endef
$(foreach src, $(A_SRC), $(eval $(call ASSEMBLE_TEMPLATE, $(src)))) 

# compile C source code
define COMPILE_C_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1) $(BUILDONCHANGE)
	$(CC) -c $(THUMB) $$(CFLAGS) $$< -o $$@ 
endef
$(foreach src, $(C_SRC), $(eval $(call COMPILE_C_TEMPLATE, $(src))))

# Target: clean project.
clean:
	rm -Rf $(OUTDIR)

# install binary (only for mbed board)
install:
	cp -av $(OUTDIR)/${PRJNAME}.bin /media/MBED/
	sync
	
# Include the dependency files.
-include $(wildcard *.d)

# Listing of phony targets.
.PHONY : all build elf hex bin lss sym clean createdirs install
