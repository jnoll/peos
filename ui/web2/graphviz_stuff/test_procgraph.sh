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

if !(grep "c->sample1_end" sample1.dot > /dev/null)
then
	echo
	echo	Failed c to sample1_end.
	echo
fi

./proc_graph my_test2.pml
if !(grep "my_test2->a_1" my_test2.dot > /dev/null)
then
	echo
	echo	Failed my_test2 to a_1.
	echo
fi
if !(grep "a_1->b_1" my_test2.dot > /dev/null)
then
	echo
	echo	Failed a_1 to b_1.
	echo
fi
if !(grep "b_1->my_test2_end" my_test2.dot > /dev/null)
then
	echo
	echo	Failed b_1 to my_test2_end.
	echo
fi

./proc_graph my_test.pml
if !(grep "p->t" my_test.dot > /dev/null)
then
	echo
	echo	Failed p to t.
	echo
fi
if !(grep "t->s" my_test.dot > /dev/null)
then
	echo
	echo	Failed t to s.
	echo
fi
if !(grep "t->e" my_test.dot > /dev/null)
then
	echo
	echo	Failed t to e.
	echo
fi
if !(grep "s->a" my_test.dot > /dev/null)
then
	echo
	echo	Failed s to a.
	echo
fi
if !(grep "s->b" my_test.dot > /dev/null)
then
	echo
	echo	Failed s to b.
	echo
fi
if !(grep "s->b" my_test.dot > /dev/null)
then
	echo
	echo	Failed s to b.
	echo
fi
if !(grep "s->bran" my_test.dot > /dev/null)
then
	echo
	echo	Failed s to bran.
	echo
fi
if !(grep "s->bran" my_test.dot > /dev/null)
then
	echo
	echo	Failed s to bran.
	echo
fi
if !(grep "a->s_end" my_test.dot > /dev/null)
then
	echo
	echo	Failed a to s_end.
	echo
fi
if !(grep "b->s_end" my_test.dot > /dev/null)
then
	echo
	echo	Failed b to s_end.
	echo
fi
if !(grep "bran->c" my_test.dot > /dev/null)
then
	echo
	echo	Failed bran to c.
	echo
fi
if !(grep "bran->d" my_test.dot > /dev/null)
then
	echo
	echo	Failed bran to d.
	echo
fi
if !(grep "c->bran_end" my_test.dot > /dev/null)
then
	echo
	echo	Failed c to bran_end.
	echo
fi
if !(grep "d->bran_end" my_test.dot > /dev/null)
then
	echo
	echo	Failed d to bran_end.
	echo
fi
if !(grep "bran_end->s_end" my_test.dot > /dev/null)
then
	echo
	echo	Failed bran_end to s_end.
	echo
fi
if !(grep "s_end->t_end" my_test.dot > /dev/null)
then
	echo
	echo	Failed s_end to t_end.
	echo
fi
if !(grep "e->t_end" my_test.dot > /dev/null)
then
	echo
	echo	Failed e to t_end.
	echo
fi
if !(grep "t_end->p_end" my_test.dot > /dev/null)
then
	echo
	echo	Failed t_end to p_end.
	echo
fi
