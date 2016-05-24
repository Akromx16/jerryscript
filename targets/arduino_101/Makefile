# Copyright © 2016 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# use TAB-8
.DEFAULT_GOAL := all

ZEPHYR    ?= $(CURDIR)/../zephyr/zephry
TYPE      ?= release
JERRYHEAP ?= 16

ZEPHYRINC   = $(ZEPHYR_BASE)/include
ZEPHYRLIB   = $(ZEPHYR_BASE)/lib

SOURCE_DIR = ./targets/arduino_101/src
$(info SOURCE_DIR=$(SOURCE_DIR))

export JERRY_INCLUDE = $(CURDIR)/jerry-core/
$(info JERRY_INCLUDE=$(JERRY_INCLUDE))

EXTRA_LINKER_CMD_OPT = Hello world

MDEF_FILE = $(SOURCE_DIR)/../prj.mdef
CONF_FILE = $(SOURCE_DIR)/../prj.conf
KERNEL_TYPE = micro

KBUILD_VERBOSE = 1

# For testing without real hardware use qemu_x86 instead of arduino_101
BOARD ?= arduino_101

APP = main-zephyr.c

$(info Compiling Zephyr)
include ${ZEPHYR_BASE}/Makefile.inc 
