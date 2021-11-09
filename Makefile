clean:
	find . -name build -type d -print0|xargs -0 rm -r --

.phony: clean
