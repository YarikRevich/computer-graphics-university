.PHONY: help, prepare, build, install

.ONESHELL:
SHELL := /bin/bash
.DEFAULT_GOAL := help

.PHONY: help
.DEFAULT_GOAL := help
help:
	@grep -h -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: prepare
prepare: ## Install prerequisites
ifeq (,$(wildcard $(shell pwd)/build))
	@mkdir $(shell pwd)/build
endif

ifneq ($(OS),Windows_NT)
ifeq ($(shell uname),Darwin)
	@sudo cp -r $(shell pwd)/lib/sdl2/macos/SDL2.framework /Library/Frameworks
	@sudo cp -r $(shell pwd)/lib/sdl2_image/macos/SDL2_image.framework /Library/Frameworks
endif
endif

.PHONY: build
build: prepare ## Build the project
	@cd $(shell pwd)/build && cmake .. && make

.PHONY: install
install: ## Install built executables to the system directory
	@cd $(shell pwd)/build && make install