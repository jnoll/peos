TESTS=$(wildcard *.test)

test: 
	@for t in $(TESTS); do \
		perl $$t | grep "not ok" || echo "$$t done"; \
	done
