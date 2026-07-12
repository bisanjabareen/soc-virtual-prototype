# SoC virtual prototype 
A project that uses SystemC to model a SoC at transcation level.

## Motivation
Honestly... I came accross a job req that I was curious about: virtual prototypes/platforms. I decided I want to explore this kind of field further, to see if I actually like this kind of work, and well, I fell in love! 
We don't have classes on SystemC in the CS department, and so I had to learn this on my own.
I looked through some production grade github repos, as well as some lectures from a German university, and watched them to ramp up on SystemC and TLM: 
https://youtube.com/playlist?list=PL_JZo16Mzs5VhcqopCA3YRjfZLaGywstl&si=aQqB2txwEnttH9HO

---
## What am I modeling exactly? 
I'm modeling a system on a chip:
- RISC-V RV32I core in cpp 
- A simple memory module 
- RISC-V systemC wrapper 
- UART peripheral
- Bus 
- Interrupt mechanisms 

```
┌─────────────────────────────────────────────────┐
│              System on Chip (SoC)               │
├─────────────────────────────────────────────────┤
│                                                 │
│  ┌──────────────────┐                          │
│  │  RISC-V RV32I    │                          │
│  │   Core (C++)     │                          │
│  └────────┬─────────┘                          │
│           │                                    │
│  ┌────────▼──────────┐                         │
│  │   TLM Bus         │                         │
│  └────┬──────────┬───┘                         │
│       │          │                             │
│  ┌────▼───┐  ┌──▼──────────┐                  │
│  │ Memory │  │    UART      │                  │
│  │ Module │  │  Peripheral  │                  │
│  └────────┘  └──────────────┘                  │
│                                                 │
│  [Interrupt Mechanisms]                        │
│                                                 │
└─────────────────────────────────────────────────┘
```


## What is working today? 
- RISC-V RV32I core in cpp & RISC-V systemC wrapper
- A simple memory module 
- Bus 
- UART peripheral TX

## What is next?
- Multicore modeling (nice to have)
- Interrupt mechanisms 
- Halting mechanisms
- Comprehensive tests
- firmware
---
## The project stack:

### For core development, I used:
- RISC-V cards and specs
- SystemC
- CPP

### For CI/CD and the development ecosystem I used:
- GitHub
- Github workflows
- WSL/Ubuntu
- VSCode

### For testing, I used:
- Unit testing of modules: CPP, WIP (implemented some basic tests so far)
- Integration testing: Python, WIP
---
## In the real world, how would something like this be used? 
In the real world, a virtual platform like this would be used to to do architectural exploration, HW-SW co-op (concurrent design) and early stage debugging and verification (shift-left)


## How to run:
You will need to set up your environment first: 
1- While this prototype is not OS-dependent, I highly recommend running it in a Linux machine or WSL at the very least
2- Install the RISC-V toolchain gcc-riscv64-unknown-elf
3- Install SystemC


