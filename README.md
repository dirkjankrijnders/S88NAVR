S88NAVR
=======

An implementation of the S88-N protocol for Atmel Atmega[8/48/88/168/328] microcontrollers. So far, only an Arduino Pro Mini version has been tested.

Goal
----

The goal is the make small, cheap reliable feedback modules for the S88 system. Hence choice for the microcontroller, one IC instead of the three discreet ones. The choice for SMD technology for a small board with components on both sides, as most will be hand soldered. The reliability comes from the use of the S88-N Cabling and timing and a RC input network. If wanted the RC network can be short to minimize cost when needed, leaving 10 components (2 connectors, an uc, two caps and five resistors). The board is designed for production by cheap production houses like seeedstudio's PCB service. Goal is to stay under €5.00 for the fully build PCB.

LICENSE
-------

Both hardware and firmware are placed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. See <http://creativecommons.org/licenses/by-sa/3.0/> and License.txt for more details.

Copyright 2013
Dirkjan Krijnders
<dirkjan@krijnders.net>
