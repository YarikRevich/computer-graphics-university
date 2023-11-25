.PHONY: help, prepare, build, install

# USED ONLY FOR DEVELOPMENT PURPOSE
path := $(or $(path), 'dev')

.ONESHELL:
.DEFAULT_GOAL := help

.PHONY: help
.DEFAULT_GOAL := help
help:
	@grep -h -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: prepare
prepare: ## Install prerequisites
ifeq ($(path),'dev')
	export ROOT=$(PWD)
else
	export ROOT=$(path)
endif

ifeq (,$(wildcard $(root)/build))
	@mkdir $(root)/build
endif
ifneq ($(OS),Windows_NT)
ifeq ($(shell uname),Darwin)
	@cp -r $(root)/lib/sdl2/macos/SDL2.framework /Library/Frameworks
	@cp -r $(root)/lib/sdl2_image/macos/SDL2_image.framework /Library/Frameworks
endif
endif

.PHONY: build
build: ## Build the project
	@cd $(path)/build && cmake .. && make

.PHONY: install
install: ## Install built executables to the system directory
	@cd $(path)/build && make install