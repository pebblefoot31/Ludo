# variables
CXX      = g++
CXXFLAGS = -std=c++11
JGRAPH   = ./jgraph/jgraph/jgraph

# relevant files
CPP_FILE    = Ludo.cpp
EXE_FILE    = Ludo
BOARD_BASE  = LudoBoard.jgr
TEMP_JGR    = Ludo.jgr
TEMP_POINTS = points.txt

#input files
INPUTS = input/rolls1.txt input/rolls2.txt input/rolls3.txt input/rolls4.txt input/rolls5.txt

# steps to generate an image for each input file
all: $(EXE_FILE)
	@for file in $(INPUTS); do \
		echo "Processing $$file..."; \
        ./$(EXE_FILE) $$file > $(TEMP_POINTS); \
		cat $(BOARD_BASE) > $(TEMP_JGR); \
		cat $(TEMP_POINTS) >> $(TEMP_JGR); \
		OUT_NAME=$$(basename $$file .txt).jpg; \
		sh -c "$(JGRAPH) -P $(TEMP_JGR) | ps2pdf - | convert -density 300 - -quality 100 $$OUT_NAME"; \
		echo "Created $$OUT_NAME"; \
	done
	@rm -f $(TEMP_POINTS) $(TEMP_JGR)

#compile the ludo computation code
$(EXE_FILE): $(CPP_FILE)
	$(CXX) $(CXXFLAGS) $(CPP_FILE) -o $(EXE_FILE)

clean:
	rm -f $(EXE_FILE) *.jpg $(TEMP_POINTS) $(TEMP_JGR)
