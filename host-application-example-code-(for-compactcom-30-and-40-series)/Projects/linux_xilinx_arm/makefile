CC := arm-linux-gnueabihf-gcc
CFLAGS := -Wall

ifeq ($(OS),Windows_NT)
   SHELL = cmd.exe
   RM = del /Q $(DEPDIR)\*.d $(OBJDIR)\*.o $(subst /,\\,$(OUTPUT))
else
   RM = rm -f $(DEPDIR)/*.d $(OBJDIR)/*.o *~ $(OUTPUT)
endif

DEPDIR := dep
OBJDIR := obj
OUTDIR := out
OUTPUT := $(OUTDIR)/abcc40esk

LOCAL_SRC_DIRS := .
LOCAL_SRC_DIRS += abcc_adapt
LOCAL_SRC_DIRS += abcc_drv/src
LOCAL_SRC_DIRS += abcc_drv/src/par
LOCAL_SRC_DIRS += abcc_drv/src/par30
LOCAL_SRC_DIRS += abcc_drv/src/serial
LOCAL_SRC_DIRS += abcc_drv/src/spi
LOCAL_SRC_DIRS += abcc_obj
LOCAL_SRC_DIRS += abcc_obj/nw_obj
LOCAL_SRC_DIRS += example_app

LOCAL_INC_DIRS := .
LOCAL_INC_DIRS += abcc_abp
LOCAL_INC_DIRS += abcc_adapt
LOCAL_INC_DIRS += abcc_drv/inc
LOCAL_INC_DIRS += abcc_drv/src
LOCAL_INC_DIRS += abcc_drv/src/par
LOCAL_INC_DIRS += abcc_drv/src/par30
LOCAL_INC_DIRS += abcc_drv/src/serial
LOCAL_INC_DIRS += abcc_drv/src/spi
LOCAL_INC_DIRS += abcc_obj
LOCAL_INC_DIRS += abcc_obj/nw_obj
LOCAL_INC_DIRS += example_app

SRC := $(foreach sdir,$(LOCAL_SRC_DIRS),$(wildcard $(sdir)/*.c))
INC := $(foreach idir,$(LOCAL_INC_DIRS),-I$(idir))
OBJ := $(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))
DEP := $(addprefix $(DEPDIR)/,$(notdir $(OBJ:.o=.d)))

VPATH = $(LOCAL_SRC_DIRS)

ifneq "$(findstring clean,$(MAKECMDGOALS))" "clean"
-include $(DEP)
endif

.PHONY: all clean dirtree
.DEFAULT_GOAL := all

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) $^ -o $@

$(DEPDIR)/%.d: %.c
	$(CC) -MM -MF $@ -MP -MT "$(OBJDIR)/$*.o $@" $(CFLAGS) $(INC) $<

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(OBJ) $(DEP): | dirtree

dirtree:
ifeq ($(OS),Windows_NT)
	if not exist $(subst /,\\,$(DEPDIR)) mkdir $(subst /,\\,$(DEPDIR))
	if not exist $(subst /,\\,$(OBJDIR)) mkdir $(subst /,\\,$(OBJDIR))
	if not exist $(subst /,\\,$(OUTDIR)) mkdir $(subst /,\\,$(OUTDIR))
else
	mkdir -p $(DEPDIR) $(OBJDIR) $(OUTDIR)
endif

clean:
	$(RM)
