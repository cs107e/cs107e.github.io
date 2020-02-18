Most bare-bones application to show the use of timers. Not clean,
but very simple, concise, and contained to this directory. Uses the 
armtimer interrupt to tick at 1Hz and toggles the ACT LED as well
as displaying on console.

Authors: Dawson Engler <engler@cs.stanford.edu>
         Philip Levis <pal@cs.stanford.edu>
Date: 5/18/17

Why are sp and lr special?

sub lr, lr, #4 // re-execute instruction

