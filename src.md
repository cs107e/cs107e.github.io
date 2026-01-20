---
layout: header
permalink: /src
---

# CS107e library source files
{: .border-bottom }

<script>
    $(function() {
       if (window.location.hash != '') {
          $('a[href="' + window.location.hash + '"]').click();
       }
    });
</script>

<style>
pre {
    font-size: 80%;
    max-width: 600px;
    overflow-x: scroll;
    margin: 0;
}
.sidenav {
    width: 140px;
    background-color: #D3DAE3;
    margin: 0px;
    font-size: 90%;
    margin-right: 10px;
}
.content {
    flex-grow: 1;
}
.tab-link {
    padding: 0px 10px;
}
</style>

{% assign filenames = "ccu.c,cstart.c,de.c,font.c,gpio_interrupt.c,hdmi.c,hstimer.c,interrupts.c,interrupts_asm.s,mango.c,memmap.ld,ps2_keys.c,rand.c,ringbuffer.c,start.s,uart.c" | split: ',' %}

<div class="row flex-row">
<div class="nav nav-pills flex-column sidenav">
{%- for f in filenames %}
    {% assign base = f| split:'.' | first %}
    <a class="tab-link nav-link" id="{{f}}-tab" data-toggle="pill" href="#{{base}}" onclick="location.replace('#{{base}}');"> 
    {{f}}</a>
{%- endfor %}
</div>

<div class="tab-content content">
{%- for f in filenames %}
{% assign base = f | split:'.' | first %}
{% assign ext = f | split:'.' | last %}
<div class="tab-pane" id="{{base}}" markdown="1">
{%- if ext == "s" %}
```ini
{%- else %}
```c
{%- endif %}
{%- if f == "memmap.ld" %}
{% include_relative /cs107e/lib/memmap.ld %}
{%- else %}
{% include_relative /cs107e/src/{{f}} %}
{%- endif %}
```
</div>
{% endfor %}
</div>
</div>
