#!/bin/sh

echo -n "."
echo TEST_PROCGRAPH.SH

./proc_graph sample1.pml

if !(grep "sample1->a" sample1.dot > /dev/null)
then
	echo
	echo	Failed sample1 to a.
	echo
fi

if !(grep "a->b" sample1.dot > /dev/null)
then
	echo
	echo	Failed a to b.
	echo
fi

if !(grep "b->a" sample1.dot > /dev/null)
then
	echo
	echo	Failed b to a.
	echo
fi

if !(grep "b->c" sample1.dot > /dev/null)
then
	echo
	echo	Failed b to c.
	echo
fi

if !(grep "c->sample1@end" sample1.dot > /dev/null)
then
	echo
	echo	Failed c to sample1@end.
	echo
fi

./proc_graph my_test.pml
