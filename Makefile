.PHONY: help, prepare, build, install

.PHONY: help
.DEFAULT_GOAL := help
help:
	@grep -h -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: prepare
prepare: ## Install prerequisites
ifeq (,$(wildcard $(PWD)/build))
	@mkdir $(PWD)/build
endif
ifneq ($(OS),Windows_NT)
ifeq ($(shell uname),Darwin)
	@cp -r $(PWD)/lib/sdl2/arm64/SDL2.framework /Library/Frameworks
endif
endif

.PHONY: build
build: ## Build the project
	@cd $(PWD)/build && echo $(PWD) && cmake .. && make

.PHONY: install
install: ## Install built executables to the system directory
	@cd $(PWD)/build && make install