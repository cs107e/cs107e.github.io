Application that demonstrates a race condition with interrupts.
It registers a handler for the hs timer and does a pair
of increment/decrements. If the handler executes in between
the load and store of the main code, then the intermediate value 
in a register will be lost. Print when this occurs to show
how quick it can be.
