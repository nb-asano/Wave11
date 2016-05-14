################################################################################
#
# Makefile for Wave11
#
# Copyright (c) 2012-2016 Sakura-zen soft. All rights reserved.
#
################################################################################

MAKE = make --no-print-directory

SUBDIRS = Wave11

all:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	$(MAKE) -C $$subdir || exit 1; \
	done

clean:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	$(MAKE) clean -C $$subdir || exit 1; \
	done

