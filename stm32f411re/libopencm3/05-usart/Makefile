PROJECT = app
BUILD_DIR = build

VPATH = ..

CFILES = main.c mal.c

DEVICE=stm32f411ce
OOCD_FILE = board/$(DEVICE).cfg

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
