Hall Effect:
	A Hall effect sensor is a transducer that varies its output
	voltage in response to a magnetic field. Hall effect sensors are
	used for proximity switching, positioning, speed detection, and
	current sensing applications. In its simplest form, the sensor
	operates as an analog transducer, directly returning a voltage.

Discussion:
	- What would you use this for?
	- what if we connect to 3v vs 5v?
	- no pulldown?

	- can we get jitter in readings?
	
	- what happens if we move it back and forth very very fast?  
	  (e.g., mechanically?)


	swap pi: 
		What if you get no output?

hardware heuristics:
	- if h/w read/write has to be volatile.
	- hardware bugs = nothing happens.
		look for stupid stuff first.
			e.g., put wires where you don't have to count.
		hardware is not eternal like 0,1: use differential
		analysis to check if broken

			- swap sensor.
			- swap pi --- if you don't have protection, can work
			  initially, then fry.

		natural to partition project.  don't for device.
		easy to misunderstand.  have partner do too without talking
			hope to get non-correlated errors.
		
	- the pdf's can have errors!   
	- just b/c it 'works' does not mean it does.   this is very common
	with dangerous voltages --- just hasn't fried pin yet.
