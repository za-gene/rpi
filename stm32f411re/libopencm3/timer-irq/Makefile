PROJECT = app
BUILD_DIR = build

#SHARED_DIR = ../my-common-code
CFILES = main.c
#CFILES += api.c
#AFILES += api-asm.S

# TODO - you will need to edit these two lines!
#DEVICE=stm32f407vgt6
DEVICE=stm32f411re
OOCD_FILE = board/stm32f411re.cfg

# You shouldn't have to edit anything below here.
#VPATH += $(SHARED_DIR)
#INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
#OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
