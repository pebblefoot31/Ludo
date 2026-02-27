# Variables make it easy to change filenames later
JGRAPH = ./jgraph/jgraph/jgraph
SOURCE = Ludo.jgr
OUTPUT = Ludo.jpg

# The 'all' target is what runs when you just type 'make'
all: $(OUTPUT)

# This rule tells Make: "To create $(OUTPUT), I need $(SOURCE)"
$(OUTPUT): $(SOURCE)
		sh -c "$(JGRAPH) -P $(SOURCE) | ps2pdf - | convert -density 300 - -quality 100 $(OUTPUT)"

# Clean up the generated image
clean:
		rm -f $(OUTPUT)
