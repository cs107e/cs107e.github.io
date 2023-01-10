---
layout: header
permalink: /header
---

# CS107e libpi header files
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

{% assign header_names = "assert,backtrace,console,fb,font,gl,gpio,gpio_extra,gpio_interrupts,interrupts,keyboard,mailbox,malloc,mouse,pi,printf,ps2,ps2_keys,ringbuffer,shell,shell_commands,strings,timer,uart" | split: ',' %}

<div class="row flex-row">
<div class="nav nav-pills flex-column sidenav">
{%- for f in header_names %}
    <a class="tab-link nav-link" id="{{f}}-tab" data-toggle="pill" href="#{{f}}" onclick="location.replace('#{{f}}');"> 
    {{f}}.h</a>
{%- endfor %}
</div>

<div class="tab-content content">
{%- for f in header_names %}
<div class="tab-pane" id="{{f}}" markdown="1">
```c
{% include_relative /cs107e/include/{{f}}.h %}
```
</div>
{% endfor %}
</div>
</div>
