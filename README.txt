gentasksets
===========
Generates a population of task sets that can be consumed by the wcrt program.

Usage: ./gentasksets -n <task set size> -m <task set population> > $TASKSETS

Currently, the range for task periods is hardcoded to [40,60),
and the range for task computation times is hardcoded to [4,10).

The output of gentasksets can be piped directly into the input of wcrt.

wcrt
====

Determines worst case running time (WCRT) for the lowest priority task
in a given task set and prints a table of statistics for each run of
the gap enumeration algorithm.

Usage: ./wcrt < $TASKSETS 1> $RESULTS 2> $GAP_REPS

Task set inputs are fed via stdin. Each task set in the input occupies one line
and is in the following format:

n:{r1,c1,p1}{r2,c2,p2}...{rn,cn,pn}

n = task set size
ri = task i release time
ci = task i computation time
di = task i period (= deadline = priority)

The program writes a table to stdout detailing the results from each task set
for every task up to the lowest priority task in the set or until the gaps
for execution have been exhausted. Rows labeled gap_enum1 use the original
gap-enumeration algorithm presented by Belwal and Cheng. Rows labeled gap_enum2
use the modified algorithm described in the report.

The program also exports a representation of the final gap set for each run
to stderr.

Contact
=======

Please direct all comments, questions, and bug reports to Michael Yantosca
via e-mail at mike@archivarius.net.
