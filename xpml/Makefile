test: pml_test
pml_test: test.xml
	xsltproc xpml.xsl $< > test.out
	diff -w test.pml test.out
	../check/pmlcheck test.out
