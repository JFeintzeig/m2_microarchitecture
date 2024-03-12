# M2 Micro-architecture

Messing around with programs to make my CPU spill it's secrets. I have a M2 Pro and was inspired by [this](https://dougallj.github.io/applecpu/firestorm.html) and [this](https://www.7-cpu.com/cpu/Apple_M1.html) and [this](https://dougallj.wordpress.com/2021/04/08/apple-m1-load-and-store-queue-measurements/) to try to recreate some of that work.

I copied performance counter code from [here](https://gist.github.com/ibireme/173517c208c7dc333ba962c1f0d67d12) to do this.

Measurements so far:

`fsqrt` w/D-registers:
- stack `fsqrt` instructions in a loop, run it many times. overhead per loop is a load, a store, a subtract, a conditional branch.
- best run for 10x in a loop averaged 138.4 cycles/loop.
- best run for 20x in a loop averaged 271.9 cycles/loop.
- \# of cycles/sqrt = 13.4 ==> **13 cycles/sqrt**
