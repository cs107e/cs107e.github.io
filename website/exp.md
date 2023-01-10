---
permalink: /exp
title: Home
layout: dashboard
---

<style>
.upcoming ul {
    background-color:#f3f3f3;
    border: 1px solid gray;
    border-radius: 5px;
    padding: 10px; 
}

.upcoming li {
    list-style-type: none;
}

.upcoming .list-header {
    text-align: center;
    background-color: #666;
    color: white;
    font-weight: 700;
}

.upcoming .day {
    margin-top: 15px;
    border-bottom: 1px solid currentColor;
    font-size: 70%;
    font-weight: 700;
    color: #888;
    text-transform: uppercase;
    letter-spacing: 1px;
}

.upcoming .event:before {
    background-color: currentColor;
    border-radius: 3px;
    content: "|";
    margin: 3px 10px 3px 5px; /* TRBL */
}
.upcoming .lecture:before   { color:#008F00; }
.upcoming .lab:before       { color:#005493; }
.upcoming .assign:before    { color:#941100; }
.upcoming .oh:before        { color:#FF9300; }

.upcoming .event { font-size: 80%; }
.upcoming .what     { letter-spacing: 1px; width: 150px;}
.upcoming .start, .upcoming .stop   
                    { line-height:1.3; font-size: 90%;text-align: right; }
.upcoming .start    { font-weight: 700; }
.upcoming .stop     { color: #aaa; }
.upcoming li.event+li.event { border-top: 1px dotted #aaa; }

a[href^="http://"]:after, a[href^="https://"]:after {
    display: none;
}
</style>

## {{ site.data.course.id }} {{ site.data.course.quarter }} 

CS107e is __Computer Systems from the Ground Up__. Read more [about the course](/about/)
or answers to [common questions from prospective students](http://cs107e.stanford.edu).


---
<div class="d-flex flex-row">

<div id="upcoming" class="upcoming list-group" markdown="1">
- _Loading from calendar..._
</div>

<div class="" markdown="1">

<div class="btn-group rounded">
{%- for c in site.data.dashboard.tools %}
<a href="{{c.link}}" type="button" class="btn btn-link bg-light border"><img class="mx-auto d-block" src="/_assets/img/{{c.icon}}" width="32px"> {{c.title}}</a>
{%- endfor %}
</div>
## Course schedule
- Intro 
    - Lectures:  [Computer Architecture](/lectures/Intro/)
    - Lab [0 Orientation](/labs/lab0/)
    - Assignment [0 Git Oriented](/assignments/assign0/)
- ARM architecture and assembly language
    - Lectures [ARM architecture](/lectures/ARM/) and [ARM assembly](/lectures/asm/)
    - Lab [Blink](/labs/lab1/)
    - Assignment [1. Larson](/assignments/assign1/)
- From Assembly to C
    - Lectures [C Control](/lectures/C_Control/) and [C Pointers](/lectures/C_Pointers)
    - Lab [C and breadboarding](/labs/lab2/)
    - Assignment [2. Clock](/assignments/assign2/)
- C Strings and Communication
    - Lecture: [C Functions](/lectures/C_Functions/) and [Serial](/lectures/Serial)
    - Lab: [Debugging and testing](/labs/lab3/)
    - Assignment [3. String formatting](/assignments/assign3/)
- Memory
    - Lecture: [Linking](/lectures/Linking/) and [Memory](/lectures/Memory)
    - Lab: [Memory](/labs/lab4/)
    - Assignment [4. Backtrace and malloc](/assignments/assign4/)
- Keyboard
    - Lecture: [Keyboard](/lectures/Keyboard)
    - Lab: [5. Keyboard](/labs/lab4/)
    - Assignment [5. Keyboard and shell](/assignments/assign5/)
- Graphics
    - Lecture: [Graphics](/lectures/Linking/) and [Memory](/lectures/Memory)
    - Lab: [Memory](/labs/lab4/)
    - Assignment [6. Graphics and console](/assignments/assign6/)
- Interrupts
    - Lecture: [Linking](/lectures/Linking/) and [Memory](/lectures/Memory)
    - Lab: [Memory](/labs/lab4/)
    - Assignment [7. Interrupts](/assignments/assign7/)

</div>
</div>
---
See [schedule for entire quarter](/schedule/).

### Meet the CS107e team
{% include gallery.html %}

<iframe src="https://docs.google.com/forms/d/e/1FAIpQLSdBpyHM3qfbxmkqtXfjLDHyoIucF2K-4xtjehzIqUE3NUkBZg/viewform?embedded=true" width="640" height="519" frameborder="0" marginheight="0" marginwidth="0">Loading...</iframe>

<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
<script src="/_assets/js/newgcal.js"></script>
