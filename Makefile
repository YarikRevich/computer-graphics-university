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

.PHONY: build
build: prepare ## Build the project
	@cd $(PWD)/build && cmake .. && make

.PHONY: install
install: ## Install built executables to the system directory
	@cd $(PWD)/build && make install