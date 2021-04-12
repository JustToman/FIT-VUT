PROJ=proj2

all:$(PROJ).tex
	pdflatex $(PROJ)
	pdflatex $(PROJ)

pack: $(PROJ).tex Makefile
	zip xpopel24-fit.zip $(PROJ).tex Makefile

clean: 
	rm $(PROJ).pdf $(PROJ).dvi $(PROJ).ps $(PROJ).aux $(PROJ).log $(PROJ).out
