#
# (C) Copyright 2020
# Zimplistic Private Limited
# Sicris Rey Embay, sicris@zimplistic.com
#
.PHONY: menuconfig defconfig savedefconfig
MCONF := $(CURDIR)/configs/scripts/kconfig/mconf
CONF := $(CURDIR)/configs/scripts/kconfig/conf

$(info MCONF is $(MCONF))
$(info CONF is $(CONF))

ifeq ("$(origin DEF_CONF)", "command line")
DEFCONFIG_FILE := configs/$(DEF_CONF)
else
DEFCONFIG_FILE := configs/test_defconfig
endif

menuconfig:
	$(MCONF) Kconfig
	mkdir -p $(CURDIR)/include/config
	mkdir -p $(CURDIR)/include/generated
	$(CONF) --silentoldconfig Kconfig

defconfig:
	@echo 'Building $(DEFCONFIG_FILE)'
	$(CONF) --defconfig=$(CURDIR)/$(DEFCONFIG_FILE) Kconfig
	mkdir -p $(CURDIR)/include/config
	mkdir -p $(CURDIR)/include/generated
	$(CONF) --silentoldconfig Kconfig
    
savedefconfig:
	cp $(CURDIR)/.config $(CURDIR)/configs/defconfig

help:
	@echo 'Help'
	@echo 'make menuconfig - run menu configuration and generate autoconf.h'
	@echo 'make defconfig - generate autoconf.h using default file test_defconfig'
	@echo 'make defconfig DEF_CONF=file - generate autoconf.h using specified defconfig file'    
	@echo 'make savedefconfig - copies .config to configs/defconfig'    
