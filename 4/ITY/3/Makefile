PROJ=proj3

all:$(PROJ).tex
	latex $(PROJ).tex
	latex $(PROJ).tex
	dvips $(PROJ).dvi
	ps2pdf -sPAPERSIZE=a4 $(PROJ).ps

pack: $(PROJ).tex Makefile
	zip xpopel24-fit.zip $(PROJ).tex etiopan.eps oniisan.eps oniisan2.eps Makefile

clean: 
	rm $(PROJ).pdf $(PROJ).dvi $(PROJ).ps $(PROJ).aux $(PROJ).log $(PROJ).out
