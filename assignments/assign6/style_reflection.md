---
title: "Style Reflection Assign 6"
toc: true
attribution: Written by Ben Ruland
---

<style>
div .blockquote {
  background: #f9f9f9;
  font-size: 80%;
  border-left: 10px solid #ccc;
  margin: 1.5em 10px;
  padding: 0.5em 10px;
}
</style>

### Program and module design

Our previous discussions on style have been been fairly micro in scope. This time we will zoom out somewhat and look at the bigger choices of how we organize our programs and the effect these choices have on our final product.

One of the big tasks of assignment 5 was implementing a keyboard driver. We (the staff) put a lot of thought into how we structured the ps2 and keyboard modules.The intro in the assignment handout explains our thinking:

<div class="blockquote" markdown="1"> The design of the keyboard driver is worth a pause to understand and appreciate. All of the nitty-gritty low-level protocol details are encapsulated within the `ps2` module. The `keyboard `module layers on that to implement the logic to process scancodes into sequences, events, and typed characters. Within the keyboard module, the functionality is arranged hierarchically, each routine building on the next. The bottom level routine reads a raw scancode (by delegating to the `ps2` module), the next level gathers a sequence of scancodes into one logical key action, the higher level routines translate actions into key events and typed characters.

The layered interface cleanly supports the needs of different clients. A client that simply wants typed characters might use only the top level `keyboard_read_next`; a client that reacts to key up and down events would instead access the mid level `keyboard_read_event`.

The hierarchical design also eases the job of the implementer. Each level focuses on a discrete part of the operation and delegates tasks above and below to other functions. This makes each function simpler to implement and test. Your implementation plan of attack is to start at the bottom and work your way upward.
</div>

Imagine a world where the starter code provided no structure and just told you to implement `keyboard_read_next` and use it in `shell`.  That would be a pretty hard assignment! Design the module and its interface is a neat part of the engineering job but it is a skill you have to work at learning. It usually takes a few tries, if not a few dozen, to iterate and refine a design into a pleasing result. There is rarely one “right” answer, no silver bullet for all situations. This is where experience and practice come into play and the art of the craft shows.

What are some things to keep in mind when working through a design? A good start is to begin with sketching a visual explanations, pens on paper or whiteboard. Diagram the module parts, how the parts interact, and how the module interfaces with the rest of the program. Identify the inputs and outputs where information flows, note possible side effects and dependencies, consider any global state that is being affected. For example, if a function receives a pointer and copies the data it points to, but in the process destroys the original data, you will want to be sure to trace that side effect in the diagram.  A module with a complex control flow might benefit from a diagram to show the different states the program can be in and the transitions between them -- shout out to finite state machines from CS103! Find a captive audience, or use a rubber duck that can't run away, and talk them through your diagram. Describing your ideas out loud works wonders for finding the edge cases and loopholes in your logic.

Think in layers. Assignments 5 and 6 are a good example of this. The `shell` is acting as the topmost layer, sitting atop the the `keyboard` module which goes down through layers of functions to the bottommost `ps2` module which directly interacts with the hardware. Layers allow us to abstract away implementations behind interfaces. We like that. Imagine instead a program that had all of the logic for a PS/2 keyboard mashed straight into the shell module.  Our friend drops by and sees us using the antiquated PS/2 protocol and says “hey I wrote a driver for a fancy USB keyboard, let me share it with you". What a great friend! But now we have to go muck through shell and change all the ps2 references to usb and rework the logic for USB which uses a totally different protocol than PS/2. And after all that is fixed, if we want to also use that USB keyboard in other programs, we have to propagate those same changes everywhere. If we instead have designed in layers with clean boundaries between them, the changes are encapsulated in the low-level hardware module and easily re-used.  Recall how the `shell` uses function pointers to allow for different inputs and outputs, allowing you to substitute an alternate input device or redirect output to the screen, just by changing one function pointer.

Make components that do one thing. It can be tempting to amass a super-function that does the entire uber-task in one go, but we much prefer decomposing to a set of independent functions that each do one thing well. This facilitates code reuse and makes restructuring your program a breeze. Think of it like legos, a 1x16 long brick only fits into certain configurations, but you can re-arrange 16 1x1 bricks in lots of different ways (don’t read too much into the structural integrity of our metaphor, we're programmers, not civil engineers 😃 ).

Play around, try different ideas. You are unlikely to hit the perfect structure on your first try, but that's the beauty of the process. By going slow, spending time at the drawing board, and making the code you write modular, you give yourself the space to try different structures and implementations. Never be afraid to rewrite a piece of code or scrap a design, it might feel like a loss at first but throwing away that first attempt and
using what you learned from it to restart with a clean design is often the best
path forward; far better than grinding along with a flawed approach.

Learn about design patterns! Some really smart people have been doing this sort of thing for a while now and have identified and codified a vocabulary of common programming idioms. One great pattern employed by the `shell` module is the command pattern. This pattern from object-oriented programming is used to decouple actions and the objects they affect. There is a great take on it from the lens of video game design in the reading linked below. Although the C language doesn't formally support objects and classes, we can cobble together an object-like command dispatch in our shell by using a table of function pointers. We can easily add and remove commands just by editing the table entries, no need to edit the code or program logic. This might seem like the obvious way to structure it but the hallmark of a great design is when it is so intuitive you can’t imagine doing it any other way.

#### Model View Controller
Continuing the conversation of design patterns, there is a particularly important design pattern that we use when presenting data to a user: Model View Controller (MVC). In MVC, the data is stored in the model, is acted upon by the controller(s), and views are used to show the model state to the user. The controller interacts with the model to change the state, the views regenerate to show the latest state.  Here are a few concrete examples on MVC in action:

* The assignment writeup encouraged using of MVC for the console as it applies very nicely here. The console stores a grid of text contents; this is the model. The model is updated in response to controller actions (in this case, characters typed by the user in the shell). The view reads the model data and draw those characters on screen.  Notice that we don't read from the view to figure out what pixels to draw in an updated view, nor do we go straight from receiving a character to drawing it on the screen. Changes flow from the controller into the model, and the view is then regenerated based on the updated model. This avoids manys and edge cases caused by inconsistent state.

* A simple recipe app is another good example. The app might have a collection of recipes to browse, organized in a index by category. A recipe is shown as a notecard of essentials info like title, category, and a thumbnail image. When you click on a notecard, it expands to a full page of the detailed information of ingredients and instructions. Each representation is just a different view of the data maintained in the one model. Using MVC in this way, the controller can change the category for a recipe by telling the model to make the change and regenerate all the views. The update is reflected everywhere because all views connect to same one model.

Part of why we are so passionate about introducing you to MVC is because this design principle can be found in many of the apps you use daily!

{% comment %}
### Public vs Private

A very similar notion has come up previously with respect to the scope of variables. Here we will talk about exposing functions as public endpoints of an API or keeping them private to the module for use only within the module. As individual programmers working on a solo class project the distinction is not all that important, but when you start working on large code bases with many other layers and developers this becomes a very big consideration. When we expose a function from a module we write it becomes callable by other modules. This means that if we want to change the parameters of our function, its return type, or its behavior in any way we risk breaking the code of everyone that uses this function. For this reason it is very difficult to make changes to the public parts of your module, and so you should consider very carefully before you move something from private to public. Not only are you expected to not make breaking changes to public functions, you will also have to maintain them. Maintaining can be as simple as making sure you keep functionality as the codebase updates, or it can require fielding many requests to improve functionality, add features, and whatever else your coworkers can think of asking you. At this point you might be wondering why make anything public at all. The flip side to all of this is that in an ideal world the more of your module you expose the more use cases you can cover and the more useful your efforts are. What to expose and what to keep private is a decision that requires careful consideration of who you want to consume your code and who might end up consuming it.
{% endcomment %}


## Prompt questions
Edit the text file named `style_reflection.txt` in your `assign6` directory to include your responses to the following questions. The entire document should be about one page. Please do not repeat the question prompts in the text file, just your answers.
<a name="FSM"></a>


1. One challenge when designing a module is striking a good balance between building in flexibility for anticipated future use cases versus a simpler design that fits exactly and only what is needed today. Consider the assign 5 `keyboard` module. Tell us what code changes would be needed to support a different keymap like Dvorak. How much work would be required to add support for other modifiers, i.e alt+letter or ctrl+letter? Identify an extended feature the current design does not seem to anticipate and how you would change the design to better accommodate it.

2. Read one or both of the short readings below introducing design patterns.  Pick a pattern from the reading that you find interesting and explain it to us. Give us an example of how you would use this pattern and why it would be useful. Include a pseudocode example.

    [Patterns explained through the lens of game design (a bit easier to understand)](https://gameprogrammingpatterns.com/design-patterns-revisited.html)

    [Wiki page on original design patterns book (more content, a bit harder to grok)](https://en.wikipedia.org/wiki/Design_Patterns)

3. Function pointers are a pretty neat concept, they allow us to treat functions as data, flexibly manage control flow, and attach operations to data, such as is used for objects and methods in OOP. This is powerful stuff! Imagine adding a `bind` command to to your shell that allows binding shell functions to shortcuts, e.g., the command `bind help h` would make shortcut `h` execute the `help` command. Tell us how you could leverage function pointers to implement this feature.

4. 

    a. Diagramming as a finite state machine can be a good tool when designing the implementation of a logically complex operation. If you are unfamiliar with finite state machines or need a refresher, check out these resources:

    - [Wikipedia entry](https://en.wikipedia.org/wiki/Finite-state_machine) gives a brief overview and some useful examples
    - [CS103 lecture introducing Finite Automata](https://web.stanford.edu/class/archive/cs/cs103/cs103.1254/lectures/14/) is a theoretical treatment that goes into more depth
    
    Break out your pencils and diagram a finite state machine for your `ps2_read` function. Make sure to show separate transitions for each failed check (timing, start, parity, stop). 
    We aren't going to be pedantic about getting a perfect FSM here so don't stress over that, mostly correct is good enough. Take a photo of your FSM diagram and add and commit it to your repo.

    b. Tell us what design insights you are able to glean from your FSM diagram. Think about how you would write `ps2_read` if you were handed just this diagram and no write up. How would that implementation differ from your current implementation?


