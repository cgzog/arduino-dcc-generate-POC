# arduino-dcc-generate
A POC (Proof of Concept) for an Arduino-based project to generate DCC timing signals from network or serial-based DCC packets

This specific repository is a hard coded DCC packet generate intended to show that the DCC timings as specified in the corresponding NMRA specifications can be met (whic it appears they can but only with direct port access, using the more abstract Arduino library calls for dealing with the output ports is too slow).

The intent is to enable a total software-based command station which can generate DCC packets and use an Arduino-based device to generate a properly timed DCC signal to drive a DCC booster.  The hope is to enable the rapid development and enhancement of a 100% software DCC ocmmand station rather than being tied to the development and release cycles of the hardware-based DCC vendors.

Chris Herzog
chris.herzog@gmail.com
