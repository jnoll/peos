#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <getopt.h>
#include <stdlib.h>
#include "vm.h"
#include "process.h"
#include "test_util.h"


/* Test control flags. */

int fork_status = CK_NOFORK;
int verbosity = CK_NORMAL;

char *p1[] = {
    "start",
    "call select &&id==$a",
    "pop ",
    "call set ready a",
    "jzero 35",
    "pop",
    "call wait done a",
    "jzero 35",
    "pop",
    "call select &&id==$a",
    "pop ",
    "call select &&id==$a",
    "pop ",
    "call set ready b",
    "jzero 35",
    "pop",
    "call wait done b",
    "jzero 35",
    "pop",
    "call select &&id==$a",
    "pop ",
    "call select &&id==$a",
    "pop ",
    "call set ready c",
    "jzero 35",
    "pop",
    "call wait done c",
    "jzero 35",
    "pop",
    "call select &&id==$a",
    "pop ",
    "end",
    "call error",
    NULL
};

int p1_size = sizeof(p1);


char *p_nostart[] = {
    "0 type action mode manual requires { a } provides { a }",
    "1 type action mode manual requires { a } provides { a }",
    "2 type action mode manual requires { a } provides { a }", 
    "call select &&id==$a",
    "pop ",
    "call set ready a",
    "jzero 35",
    "pop",
    "call wait done a",
    "jzero 35",
    "pop",
    "call select &&id==$a",
    "pop ",
    "call select &&id==$a",
    "pop ",
    "call set ready b",
    "jzero 35",
    "pop",
    "call wait done b",
    "jzero 35",
    "pop",
    "call select &&id==$a",
    "pop ",
    "call select &&id==$a",
    "pop ",
    "call set ready c",
    "jzero 35",
    "pop",
    "call wait done c",
    "jzero 35",
    "pop",
    "call select &&id==$a",
    "pop ",
    "end",
    "call error",
    NULL
};

int p_nostart_size = sizeof(p_nostart);

char *p1_context = 
"registers: 19 256 233
stack: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 
variables: 13
0 0
1 1
2 2
3 3
4 4
5 5
6 6
7 7
8 8
9 9
10 10
11 11
12 12
actions: 3
0 0
1 0
2 0
";


char *big_txt = \
"0 po_receiveproposal type action mode manual 
1 reviewcertifications type action mode manual 
2 reviewsignatures type action mode manual 
3 reviewbaadate type action mode manual 
4 reviewproposal type action mode manual 
5 maketechnicalselection type action mode manual 
6 performcostanalysis type action mode manual 
7 create_pr type action mode manual 
8 electronic_pr_to_o2 type action mode manual 
9 hardcopyproposalto_o2 type action mode manual 
10 o2_record_pr type action mode manual 
11 o2_prin_pr type action mode manual 
12 sort_prs_numerically type action mode manual 
13 verify_pr_complete type action mode manual 
14 compare_pr_dollarstoproposal type action mode manual 
15 carry_pr_packageto_bd type action mode manual 
16 assign_pr_packagetogs type action mode manual 
17 validatehardcopywith_pr type action mode manual 
18 verifybudgetamount type action mode manual 
19 verifyfundingamount type action mode manual 
20 verifyallowablecosts type action mode manual 
21 checkindirectrates type action mode manual 
22 signgrantdocument type action mode manual 
23 givefiletoreceptionist type action mode manual 
24 logreceiptdateininris type action mode manual 
25 forwardfiletofileroom type action mode manual 
26 copycoverpage type action mode manual 
27 copyfad type action mode manual 
28 copysigpage type action mode manual 
29 placeonpendingreviewshelf type action mode manual 
30 givecopiestoreceptionist type action mode manual 
31 logdatesenttofmininris type action mode manual 
32 signfadsheet type action mode manual 
33 pickuporiginalgrantdocument type action mode manual 
34 logdateretininris type action mode manual 
35 givefiletofileroom type action mode manual 
36 matchfilewithsignedfad type action mode manual 
37 copygrantdocument type action mode manual 
38 copydistsheet type action mode manual 
39 copycmemo type action mode manual 
40 placecopiesinenvelope type action mode manual 
41 recorddatemailedininris type action mode manual 
42 recorddatedistbutedininris type action mode manual 
43 fileoriginalingrantfile type action mode manual 
44 giveenvelopetoreceptionist type action mode manual 
45 distributecopytodfas type action mode manual 
46 distributecopytocao type action mode manual 
47 distributecopytowinner type action mode manual 
48 distributecopytoxxxx type action mode manual 
49 start
50 call select 
51 pop 
52 call set ready po_receiveproposal
53 jzero 541
54 pop
55 call wait done po_receiveproposal
56 jzero 541
57 pop
58 call select 
59 pop 
60 call select 
61 pop 
62 call set ready reviewcertifications
63 jzero 541
64 pop
65 call wait done reviewcertifications
66 jzero 541
67 pop
68 call select 
69 pop 
70 call select 
71 pop 
72 call set ready reviewsignatures
73 jzero 541
74 pop
75 call wait done reviewsignatures
76 jzero 541
77 pop
78 call select 
79 pop 
80 call select 
81 pop 
82 call set ready reviewbaadate
83 jzero 541
84 pop
85 call wait done reviewbaadate
86 jzero 541
87 pop
88 call select 
89 pop 
90 call select 
91 pop 
92 call set ready reviewproposal
93 jzero 541
94 pop
95 call wait done reviewproposal
96 jzero 541
97 pop
98 call select 
99 pop 
100 call select 
101 pop 
102 call set ready maketechnicalselection
103 jzero 541
104 pop
105 call wait done maketechnicalselection
106 jzero 541
107 pop
108 call select 
109 pop 
110 call select 
111 pop 
112 call set ready performcostanalysis
113 jzero 541
114 pop
115 call wait done performcostanalysis
116 jzero 541
117 pop
118 call select 
119 pop 
120 call select 
121 pop 
122 call set ready create_pr
123 jzero 541
124 pop
125 call wait done create_pr
126 jzero 541
127 pop
128 call select 
129 pop 
130 call select 
131 pop 
132 call set ready electronic_pr_to_o2
133 jzero 541
134 pop
135 call wait done electronic_pr_to_o2
136 jzero 541
137 pop
138 call select 
139 pop 
140 call select 
141 pop 
142 call set ready hardcopyproposalto_o2
143 jzero 541
144 pop
145 call wait done hardcopyproposalto_o2
146 jzero 541
147 pop
148 call select 
149 pop 
150 call select 
151 pop 
152 call set ready o2_record_pr
153 jzero 541
154 pop
155 call wait done o2_record_pr
156 jzero 541
157 pop
158 call select 
159 pop 
160 call select 
161 pop 
162 call set ready o2_prin_pr
163 jzero 541
164 pop
165 call wait done o2_prin_pr
166 jzero 541
167 pop
168 call select 
169 pop 
170 call select 
171 pop 
172 call set ready sort_prs_numerically
173 jzero 541
174 pop
175 call wait done sort_prs_numerically
176 jzero 541
177 pop
178 call select 
179 pop 
180 call select 
181 pop 
182 call set ready verify_pr_complete
183 jzero 541
184 pop
185 call wait done verify_pr_complete
186 jzero 541
187 pop
188 call select 
189 pop 
190 call select 
191 pop 
192 call set ready compare_pr_dollarstoproposal
193 jzero 541
194 pop
195 call wait done compare_pr_dollarstoproposal
196 jzero 541
197 pop
198 call select 
199 pop 
200 call select 
201 pop 
202 call set ready carry_pr_packageto_bd
203 jzero 541
204 pop
205 call wait done carry_pr_packageto_bd
206 jzero 541
207 pop
208 call select 
209 pop 
210 call select 
211 pop 
212 call set ready assign_pr_packagetogs
213 jzero 541
214 pop
215 call wait done assign_pr_packagetogs
216 jzero 541
217 pop
218 call select 
219 pop 
220 call select 
221 pop 
222 call set ready validatehardcopywith_pr
223 jzero 541
224 pop
225 call wait done validatehardcopywith_pr
226 jzero 541
227 pop
228 call select 
229 pop 
230 call select 
231 pop 
232 call set ready verifybudgetamount
233 jzero 541
234 pop
235 call wait done verifybudgetamount
236 jzero 541
237 pop
238 call select 
239 pop 
240 call select 
241 pop 
242 call set ready verifyfundingamount
243 jzero 541
244 pop
245 call wait done verifyfundingamount
246 jzero 541
247 pop
248 call select 
249 pop 
250 call select 
251 pop 
252 call set ready verifyallowablecosts
253 jzero 541
254 pop
255 call wait done verifyallowablecosts
256 jzero 541
257 pop
258 call select 
259 pop 
260 call select 
261 pop 
262 call set ready checkindirectrates
263 jzero 541
264 pop
265 call wait done checkindirectrates
266 jzero 541
267 pop
268 call select 
269 pop 
270 call select 
271 pop 
272 call set ready signgrantdocument
273 jzero 541
274 pop
275 call wait done signgrantdocument
276 jzero 541
277 pop
278 call select 
279 pop 
280 call select 
281 pop 
282 call set ready givefiletoreceptionist
283 jzero 541
284 pop
285 call wait done givefiletoreceptionist
286 jzero 541
287 pop
288 call select 
289 pop 
290 call select 
291 pop 
292 call set ready logreceiptdateininris
293 jzero 541
294 pop
295 call wait done logreceiptdateininris
296 jzero 541
297 pop
298 call select 
299 pop 
300 call select 
301 pop 
302 call set ready forwardfiletofileroom
303 jzero 541
304 pop
305 call wait done forwardfiletofileroom
306 jzero 541
307 pop
308 call select 
309 pop 
310 call select 
311 pop 
312 call set ready copycoverpage
313 jzero 541
314 pop
315 call wait done copycoverpage
316 jzero 541
317 pop
318 call select 
319 pop 
320 call select 
321 pop 
322 call set ready copyfad
323 jzero 541
324 pop
325 call wait done copyfad
326 jzero 541
327 pop
328 call select 
329 pop 
330 call select 
331 pop 
332 call set ready copysigpage
333 jzero 541
334 pop
335 call wait done copysigpage
336 jzero 541
337 pop
338 call select 
339 pop 
340 call select 
341 pop 
342 call set ready placeonpendingreviewshelf
343 jzero 541
344 pop
345 call wait done placeonpendingreviewshelf
346 jzero 541
347 pop
348 call select 
349 pop 
350 call select 
351 pop 
352 call set ready givecopiestoreceptionist
353 jzero 541
354 pop
355 call wait done givecopiestoreceptionist
356 jzero 541
357 pop
358 call select 
359 pop 
360 call select 
361 pop 
362 call set ready logdatesenttofmininris
363 jzero 541
364 pop
365 call wait done logdatesenttofmininris
366 jzero 541
367 pop
368 call select 
369 pop 
370 call select 
371 pop 
372 call set ready signfadsheet
373 jzero 541
374 pop
375 call wait done signfadsheet
376 jzero 541
377 pop
378 call select 
379 pop 
380 call select 
381 pop 
382 call set ready pickuporiginalgrantdocument
383 jzero 541
384 pop
385 call wait done pickuporiginalgrantdocument
386 jzero 541
387 pop
388 call select 
389 pop 
390 call select 
391 pop 
392 call set ready logdateretininris
393 jzero 541
394 pop
395 call wait done logdateretininris
396 jzero 541
397 pop
398 call select 
399 pop 
400 call select 
401 pop 
402 call set ready givefiletofileroom
403 jzero 541
404 pop
405 call wait done givefiletofileroom
406 jzero 541
407 pop
408 call select 
409 pop 
410 call select 
411 pop 
412 call set ready matchfilewithsignedfad
413 jzero 541
414 pop
415 call wait done matchfilewithsignedfad
416 jzero 541
417 pop
418 call select 
419 pop 
420 call select 
421 pop 
422 call set ready copygrantdocument
423 jzero 541
424 pop
425 call wait done copygrantdocument
426 jzero 541
427 pop
428 call select 
429 pop 
430 call select 
431 pop 
432 call set ready copydistsheet
433 jzero 541
434 pop
435 call wait done copydistsheet
436 jzero 541
437 pop
438 call select 
439 pop 
440 call select 
441 pop 
442 call set ready copycmemo
443 jzero 541
444 pop
445 call wait done copycmemo
446 jzero 541
447 pop
448 call select 
449 pop 
450 call select 
451 pop 
452 call set ready placecopiesinenvelope
453 jzero 541
454 pop
455 call wait done placecopiesinenvelope
456 jzero 541
457 pop
458 call select 
459 pop 
460 call select 
461 pop 
462 call set ready recorddatemailedininris
463 jzero 541
464 pop
465 call wait done recorddatemailedininris
466 jzero 541
467 pop
468 call select 
469 pop 
470 call select 
471 pop 
472 call set ready recorddatedistbutedininris
473 jzero 541
474 pop
475 call wait done recorddatedistbutedininris
476 jzero 541
477 pop
478 call select 
479 pop 
480 call select 
481 pop 
482 call set ready fileoriginalingrantfile
483 jzero 541
484 pop
485 call wait done fileoriginalingrantfile
486 jzero 541
487 pop
488 call select 
489 pop 
490 call select 
491 pop 
492 call set ready giveenvelopetoreceptionist
493 jzero 541
494 pop
495 call wait done giveenvelopetoreceptionist
496 jzero 541
497 pop
498 call select 
499 pop 
500 call select 
501 pop 
502 call set ready distributecopytodfas
503 jzero 541
504 pop
505 call wait done distributecopytodfas
506 jzero 541
507 pop
508 call select 
509 pop 
510 call select 
511 pop 
512 call set ready distributecopytocao
513 jzero 541
514 pop
515 call wait done distributecopytocao
516 jzero 541
517 pop
518 call select 
519 pop 
520 call select 
521 pop 
522 call set ready distributecopytowinner
523 jzero 541
524 pop
525 call wait done distributecopytowinner
526 jzero 541
527 pop
528 call select 
529 pop 
530 call select 
531 pop 
532 call set ready distributecopytoxxxx
533 jzero 541
534 pop
535 call wait done distributecopytoxxxx
536 jzero 541
537 pop
538 call select 
539 pop 
540 end
541 call error
";


/* Utility functions. */
peos_action_t *make_actions(int size, vm_act_state state)
{
    int i;
    peos_action_t *actions = (peos_action_t *)calloc(size, sizeof(peos_action_t));
    for (i = 0; i < size; i++) {
	char buf[256];

	sprintf(buf, "act_%d", i);
	actions[i].name = strdup(buf);
	actions[i].state = state;
    }
    return actions;
}

void free_actions(peos_action_t *actions, int size)
{
    int i;
    for (i = 0; i < size; i++) {
	free(actions[i].name);
    }
    free(actions);
}

void
setup_context(vm_context_t *context, int pc, int sp, int accum, int num_var, char* inst[])
{
    int i; 

    /* Setup context. */
    context->PC = pc;
    context->SP = sp;
    for (i = 0; i < context->SP; i++) {
	context->stack[i] = i;
    }
    context->A = accum;

    context->variables = (vm_vbinding_t *) calloc(num_var, sizeof(vm_vbinding_t));
    for (i = 0; i < num_var; i++) {
	char buf[32];
	sprintf(buf, "%d", i);
	context->variables[i].name = strdup(buf);
	context->variables[i].value = i;
    }
    context->variables[i].name = NULL;
    context->num_variables = i;

#ifdef NOTUSED
    actions = (peos_action_t *) calloc(256, sizeof(peos_action_t));
    for (num_actions = 0, i = 0; inst[i]; i++) {
	char name[256];
	if (strncmp(inst[i], "start", strlen("start")) == 0) {
	    break;
	}
	sscanf(inst[i], "%s ", name);
	actions[num_actions].name = strdup(name);
	actions[num_actions].state = ACT_NONE;
	num_actions++;
    }
#endif
}

void
parse_args(int argc, char *argv[])
{
    int c;

    while(( c = getopt(argc, argv, "vfq")) != -1)
    {
	switch (c){
	case 'v':
	    verbosity = CK_VERBOSE;
	    break;
	case 'q':
	    verbosity = CK_SILENT;
	    break;
	case 'f': 
	    fork_status = CK_FORK;
	    break;
	default:
	    fprintf(stderr, "usage: %s [-vf] (verbose, fork)\n", argv[0]);
	    exit(1);
	}
     }
}
