Release: 20120811

BSP430 is a set of C language adaptation layers simplifying cross-MCU
library and application development on Texas Instruments MSP430
microcontrollers.

The target audience is experienced embedded systems developers who need full
control over the microcontroller but are willing to trade a small amount of
performance for improved component re-usability and rapid prototyping.

BSP430 features:

* A configuration infrastructure allowing you to include only those
  resources and capabilities required by your application;

* A set of abstraction layers enabling efficient identification and
  manipulation of peripheral capabilities at both register and functional
  levels;

* A callbacks callback infrastructure allowing unrelated capabilities to
  share resources including port and timer interrupt handlers;

* A platform model that enables rapid development on common hardware
  solutions including six TI experimenter boards as well as custom boards;

* Integrated support for watchdog and low power mode execution

* A build infrastructure designed to simplify creating new applications,
  currently focused on the `MSPGCC`_ toolchain in a POSIX command-line
  environment supported by GNU Make

Copyright 2012, Peter A. Bigot, and licensed under `BSD-3-Clause`_.

.. _giturl:
.. _homepage: 
.. _BSD-3-Clause: http://www.opensource.org/licenses/BSD-3-Clause
.. _MSPGCC: http://sourceforge.net/projects/mspgcc/