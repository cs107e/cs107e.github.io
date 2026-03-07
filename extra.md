---
layout: header
permalink: /extra
---

# CS107e libextra files
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

{% assign filenames = "dma.h,dma.c,i2c.h,i2c.c,twi_driver.h,twi_driver.c,i2s.h,i2s.c,spi.h,spi.c,spi_extra.h,spi_driver.h,spi_driver.c,pwm.h,pwm.c" | split: ',' %}

<div class="row flex-row">
<div class="nav nav-pills flex-column sidenav">
{%- for f in filenames %}
    {% assign base = f | replace:'.','_' %}
    <a class="tab-link nav-link" id="{{f}}-tab" data-toggle="pill" href="#{{base}}" onclick="location.replace('#{{base}}');">
    {{f}}</a>
{%- endfor %}
</div>

<div class="tab-content content">
{%- for f in filenames %}
{% assign base = f | replace:'.','_' %}
{% assign ext = f | split:'.' | last %}
<div class="tab-pane" id="{{base}}" markdown="1">
{%- if ext == "s" %}
```ini
{%- else %}
```c
{%- endif %}
{%- if ext == "h" %}
{% include_relative /cs107e/include/{{f}} %}
{%- else %}
{% include_relative /cs107e/src/{{f}} %}
{%- endif %}
```
</div>
{% endfor %}
</div>
</div>
