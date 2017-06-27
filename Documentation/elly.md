# elly

State of the `elly branch` after the project has been finished:

## No support for multiple colonizations

It can happen that the island is colonized twice by different 
species from the same clade. The program detects this and
throws an exception. These events are assumed to be rare. 

See [Issue 203](https://github.com/richelbilderbeek/pbdmms/issues/203)

## Time units of branching times

In this simulation, we use 'time since stem age in million years' as a time unit.
There are hints that DAISIE uses 'time before present in million years' as a time unit.

[Issue 212](https://github.com/richelbilderbeek/pbdmms/issues/212) describes the problem

## Potential bug in DAISIE

There may be a bug in DAISIE. 

We used a bleeding-edge and untested version of DAISIE.

[Issue 190](https://github.com/richelbilderbeek/pbdmms/issues/190) signalled the problem

[Issue 221](https://github.com/richelbilderbeek/pbdmms/issues/211) failed to replicate the problem,
as there were higher priorities than write a bug report.

It is unknown what the consequences of this bug are. It is expected these are small, as it happened
after parameter estimation. It can also be hoped for that the next CRAN version will have it fixed.


## Issues

Most Issues linked to are marked with:

```
Documented in [Documentation/elly.md](Documentation/elly.md). Reopen this Issue when this needs to be  solved.
```