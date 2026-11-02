# RISC-V Operating System Kernel

## Overview

An educational operating system kernel. It is written in C++ and RISC-V assembly and implements fundamental OS concepts, including memory management, thread scheduling, synchronization, system calls, and interrupt handling.

## Project Structure

- `h/` – Header files containing class declarations and interfaces.
- `src/` – C++ and assembly source files.
- `lib/` – Supporting libraries.
- `build/` – Generated build files.
- `kernel.ld` – Linker script defining the memory layout.
- `Makefile` – Build and execution configuration.

## Implemented Components

### Memory Allocator
A block-based memory allocator that supports dynamic memory allocation and deallocation. It manages heap space, splits free blocks when needed, and merges adjacent free blocks to reduce fragmentation.

### Thread Management
Implemented through the `TCB` and `Scheduler` classes. Supports thread creation, termination, scheduling, and context switching, with separate queues for ready and sleeping threads.

### Synchronization
The `KernelSemaphore` class provides semaphore operations, including opening, closing, waiting, and signaling. Waiting threads are managed through dedicated queues.

### Sleeping Threads
Supports suspending threads for a specified period. A timer interrupt mechanism tracks elapsed time and wakes threads when their sleep period expires.

### Buffer and Console
A circular character buffer provides synchronized access to stored characters using semaphores. The console interface supports character input and output.

### Trap and Interrupt Handling
The `RiscV` class handles system calls, timer interrupts, and external interrupts. Assembly routines save and restore processor registers and transfer control to the appropriate kernel handlers.

## System Calls

| ID | Operation |
|---|---|
| `0x01–0x02` | Memory allocation and deallocation |
| `0x11–0x13` | Thread operations |
| `0x21–0x26` | Semaphore operations |
| `0x31` | Thread sleep |
| `0x41–0x42` | Console input and output |

The project provides both C and C++ APIs. The C++ interface includes the `Thread`, `Semaphore`, `PeriodicThread`, and `Console` classes.

## Building and Running

Build the project:

```bash
make
```

Run using QEMU:

```bash
make qemu
```

Run with GDB support:

```bash
make qemu-gdb
```

Clean generated files:

```bash
make clean
```

## Implementation Status

All required components and functionality have been implemented and tested. The kernel provides working memory management, thread management, synchronization, console operations, system calls, and interrupt handling.
