.PHONY: help, prepare, build, install

# USED ONLY FOR DEVELOPMENT PURPOSE
path := $(or $(path), $(PWD))

.PHONY: help
.DEFAULT_GOAL := help
help:
	@grep -h -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: prepare
prepare: ## Install prerequisites
ifeq (,$(wildcard $(path)/build))
	@mkdir $(path)/build
endif
ifneq ($(OS),Windows_NT)
ifeq ($(shell uname),Darwin)
	@cp -r $(path)/lib/sdl2/macos/SDL2.framework /Library/Frameworks
endif
endif

.PHONY: build
build: ## Build the project
	@cd $(path)/build && cmake .. && make

.PHONY: install
install: ## Install built executables to the system directory
	@cd $(path)/build && make install