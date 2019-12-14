# Iter++

## Goals

A experiment with zero-cost iterators in c++.

The idea is to have rust like iterators in c++. The key goals:

- no run-time overhead compared to for loop,
- use rust's iterator API as an inspiration,
- try to get decent performance while using high level constructs.

## Current State

So far, the goals aren't met:

- there is run-time cost,
- some templates specifications are required on maps.

This is a good starting point.

Right now, with the one lousy benchmark, there seems to be a non zero
initial cost and strangely, the iterators have less overhead on AMD
systems.

Results on an Intel i5-7300HQ GCC:

| benchmark | iter (avg µs) | for (avg µs) | iter performance |
|-----------|--------------:|-------------:|-----------------:|
| prime     |       955 242 |      694 747 |              72% |

Right now it's likely that the cost is coming from wrapping the
`next()` result in an `std::Option`. Using raw pointers or references
would likely improve the performance.

## Non-Goals

Having a zero run-time cost means that the iteration has to be
converted to a for like structure. This is done with a lot of
templates and optimizations which makes compiling slower.
