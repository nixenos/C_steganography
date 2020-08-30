.PHONY: clean
__ : dokumentacja.pdf

dokumentacja.pdf : sprawozdanie.pdf refman.pdf
	pdftk sprawozdanie.pdf refman.pdf cat output dokumentacja.pdf
sprawozdanie.pdf : sprawozdanie.tex
	pdflatex sprawozdanie
	bibtex   sprawozdanie
	pdflatex sprawozdanie
	pdflatex sprawozdanie
clean : 	
	for f in `ls doxy Doxyfile *.aux *.log *.out *.gz *.bib *.blg *.bbl *.o *~ sprawozdanie.pdf` ; do  if [ -f $$f ] ; then rm $$f  ; fi;   done;
	if [ -d latex ] ; then rm -r latex ; fi ;
	if [ -d html ]  ; then rm -r html  ; fi ;

