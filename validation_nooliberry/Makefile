CONTIKI_PROJECT=validation_nooliberry
TARGET=nooliberry
all: $(CONTIKI_PROJECT)
APPS = slip-cmd

CONTIKI=../..


WITH_UIP6=1
UIP_CONF_IPV6=1
UIP_CONF_RPL=0
OUTPUT_FILE = validation_nooliberry.elf

#linker optimizations
SMALL=1

CFLAGS += -DUIP_CONF_IPV6=1



include $(CONTIKI)/Makefile.include
