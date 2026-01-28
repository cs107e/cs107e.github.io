---
permalink: /
title: Home
layout: dashboard
---

<style>
.upcoming ul {
    background-color:#f3f3f3;
    border: 1px solid gray;
    border-radius: 5px;
    border-top-left-radius: 0;
    border-top-right-radius: 0;
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

    border-top-left-radius: 5px;
    border-top-right-radius: 5px;
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

.upcoming .event    { font-size: 80%; }
.upcoming .what     { width: 180px; letter-spacing: 1px; font-weight: 900; }
.upcoming .where    {               letter-spacing: 0px; font-weight: 500; }
.upcoming .start, .upcoming .stop
                    { line-height:1.3; font-size: 90%;text-align: right; }
.upcoming .start    { font-weight: 700; }
.upcoming .stop     { color: #aaa; }
.upcoming li.event+li.event { border-top: 1px dotted #aaa; }

/* disable display of external link icon for links within button grid */
.button-grid a::after {
    display: none !important;
}

.boxed-link {
    border: 1px silver solid;
    border-radius: 5px;
    box-shadow: 0px 2px 4px #EEEEEE;
    padding: 10px;
    cursor: pointer;
    text-align: center;
}

.boxed-link:hover {
    background-color: #337ab7; /* link color */
}

.button-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, 160px);
    grid-template-rows: repeat(auto-fit, 90px);
    grid-gap: 10px;
    flex: 1;
    width: 100%;
    padding: 0 10px;
}

</style>

# {{ site.data.course.id }} {{ site.data.course.quarter }}

CS107e is __Computer Systems from the Ground Up__. Read more [about the course](/about/)
and review answers to [common questions from prospective students and enrollment information](http://cs107e.stanford.edu).

---
<div class="d-flex">

    <div id="upcoming" class="upcoming list-group">
    <div class="list-header">When and where</div>
    <ul></ul>
    </div>

    <div class="button-grid right-half">
    {% for p in site.data.dashboard.tools %}
        <a href="{{p.link}}" class="btn boxed-link rounded" role="button" target="_blank">
        <img class="icon" src="/_assets/img/{{p.icon}}" height="32px" width="32px">
        <div style="font-weight:bold;">{{p.title}}</div>
        <div style="font-weight:lighter; font-size:75%; color:gray">{{p.description}}</div>
        </a>
    {% endfor %}
    </div>

</div>

#### This week in CS107e
{% include schedule.html current_only=true %}
See [schedule for entire quarter](/schedule/).

### Meet the team
{% include gallery.html %}

<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
<script src="/_assets/js/googlecal.js"></script>
