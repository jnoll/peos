test: pml_test html_test
pml_test: test.xml
	xsltproc xpml.xsl $< > test.out
	diff -w test.pml test.out
	../check/pmlcheck test.out

html_test: test.xml
	xsltproc html.xsl $< > test.out
	diff -w test.html test.out

