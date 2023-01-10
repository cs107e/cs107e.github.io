---
layout: header
permalink: /src
---

# CS107e libpi source files
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

{% assign filenames = "armtimer.c,bits_asm.s,font.c,gpio_extra.c,gpio_interrupts.c,interrupts.c,interrupts_asm.s,mailbox.c,pi.c,ps2_keys.c,ringbuffer.c,uart.c" | split: ',' %}

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
<div class="tab-pane" id="{{base}}" markdown="1">
```c
{% include_relative /cs107e/src/{{f}} %}
```
</div>
{% endfor %}
</div>
</div>
