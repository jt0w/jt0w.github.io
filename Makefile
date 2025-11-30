TYP_FILES := $(wildcard blogs/*.typ)
HTML_FILES := $(patsubst blogs/%.typ,blogs_html/%.html,$(TYP_FILES))

.PHONY: all clean
all: $(HTML_FILES) index.html

$(HTML_FILES): | blogs_html

blogs_html/%.html: blogs/%.typ | blogs_html
	typst compile --features html -f html $< $@

blogs_html:
	mkdir -p blogs_html

index.html: index.gen $(HTML_FILES) blogs_html
	./index.gen > index.html

index.gen: index.gen.c
	gcc -o index.gen index.gen.c

index.gen.c: build
	./build > index.gen.c

build: build.c
	gcc -o build build.c

clean:
	rm -rf build index.gen.c index.gen index.html
	rm -rf blogs_html
