PROJ=proj1

all:$(PROJ).tex
	latex $(PROJ).tex
	latex $(PROJ).tex
	dvips -t a4 $(PROJ).dvi
	ps2pdf $(PROJ).ps

pack: $(PROJ).tex Makefile
	zip xpopel24-fit.zip $(PROJ).tex Makefile

clean: 
	rm $(PROJ).pdf $(PROJ).dvi $(PROJ).ps $(PROJ).aux $(PROJ).log $(PROJ).out
