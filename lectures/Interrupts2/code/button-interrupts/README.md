Program that displays button pushes (GPIO21) on the display.
GPIO21 should be wired to ground through a button (it is
configured to be pullup).

Uses interrupts so button presses are enqueued -- slow screen refresh
shows them spool out.
