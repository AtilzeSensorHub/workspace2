#
# Copyright (c) 2017, Intel Corporation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. Neither the name of the Intel Corporation nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL CORPORATION OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

### Variables
BOARD_DIR = $(APP_DIR)/sensor_hub_lib
OBJ_DIR = $(APP_DIR)/$(BUILD)/$(SOC)/$(TARGET)/$(OBJ)
OBJ_DIRS += $(OBJ_DIR)
DRIVER_SOURCES = $(foreach drv, $(PERIPH),$(wildcard $(BOARD_DIR)/$(drv)/*.c))
OBJECTS += $(addprefix $(OBJ_DIR)/,$(notdir $(DRIVER_SOURCES:.c=.o)))
GENERATED_DIRS += $(BOARD_DIR)/$(BUILD)

CFLAGS += -I$(BOARD_DIR)

### Build C files	
$(OBJ_DIR)/%.o: $(BOARD_DIR)/sensors/%.c
	$(call mkdir, $(OBJ_DIR))
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(OBJ_DIR)/%.o: $(BOARD_DIR)/wireless/%.c
	$(call mkdir, $(OBJ_DIR))
	$(CC) $(CFLAGS) -c -o $@ $<	
	
$(OBJ_DIR)/%.o: $(BOARD_DIR)/system/%.c
	$(call mkdir, $(OBJ_DIR))
	$(CC) $(CFLAGS) -c -o $@ $<	
	
$(OBJ_DIR)/%.o: $(BOARD_DIR)/shields/%.c
	$(call mkdir, $(OBJ_DIR))
	$(CC) $(CFLAGS) -c -o $@ $<				