/* Author: Julie Zelenski
 * updated Winter 2024
 *
 * Reminder of how calendar widget works
 * Displays next N days of events from our Google calendar
 * For now I am adding lecture/lab/assign due dates to google calendar manually
 * Would be nice if it auto-populated from schedule.yml data...
 */
'use strict';

/*
BEGIN:VEVENT
DTSTAMP:20210111T055000Z
UID:event-calendar-event-328368
DTSTART:20210111T193000Z
DTEND:20210111T210000Z
CLASS:PUBLIC
DESCRIPTION:[Click here to join Zoom Meeting:958 4302 8571] (https://stanfo
 rd.zoom.us/j/95843028571?pwd=WVd4dHdQL0oySkx3WC9yTjNOdWx3Zz09)
LOCATION:Zoom Online Meeting
SEQUENCE:0
SUMMARY:Lecture [W21-CS-107E-01]
URL:https://canvas.stanford.edu/calendar?include_contexts=course_130524&mon
 th=01&year=2021#calendar_event_328368
X-ALT-DESC;FMTTYPE=text/html:<p><a href="https://stanford.zoom.us/j/9584302
 8571?pwd=WVd4dHdQL0oySkx3WC9yTjNOdWx3Zz09" target="_blank">Click here to j
 oin Zoom Meeting:958 4302 8571</a></p>
END:VEVENT
*/

// Attempt to read ics from canvas feed -- not used

/*
function readICS(url) {
    var request = new XMLHttpRequest();
    url = 'https://cors-anywhere.herokuapp.com/'+url;
    console.log(url);
    request.open("GET", url, true);
    request.send(null);
    var allEvents = [];

    request.onreadystatechange = function() {
    if (request.readyState === 4 && request.status === 200) {
        var type = request.getResponseHeader("Content-Type");
        if (type.indexOf("text") !== 1) {
          var lines = request.responseText.split("\n");
          var curEvent = undefined;
          for (i = 0; i < lines.length; i++) {
            var fields = lines[i].split(":");
            var key = fields[0];
            if (key == 'DTSTART') {
                curEvent = { start: fields[1]};
                allEvents.push(curEvent);
            } else if (key == 'DTEND') {
                curEvent['end'] = fields[1];
            } else if (key == 'SUMMARY') {
                curEvent['summary'] = fields[1];
            } else if (key == 'END:VEVENT') {
               curEvent = undefined;
            }
          }
          console.log(allEvents);
        }
      }
    };
    return allEvents;
}
*/

function eventType(event) {
    if (event.summary.includes('YEAH')) return "oh";
    if (event.summary.includes('Lecture')) return "lecture";
    if (event.summary.includes('Lab')) return "lab";
    if (event.summary.includes('Assign')) return "assign";
    if (event.summary.includes('OH')) return "oh";
    return "";
}

/* upcoming list ios-style */

function createHTML(events) {
      let html = '';
      let prevDate = undefined;
      $.each(events, function(i, event){
          const start = new Date(event.start.dateTime);
          const stop = new Date(event.end.dateTime);

          if (start.getDate() != prevDate) {
              const date = start.toLocaleDateString('en-US', {weekday: 'long', month:'short', day:'numeric'})
              html += '<li class="day">' + date + '</li>';
              prevDate = start.getDate();
          }

          function my_time_format(t) { return t.toLocaleTimeString('en-US', { hour:'numeric', minute:'2-digit' }).toLowerCase(); };
          const startTime = '<div class="start">' + my_time_format(start) + '</div>';
          const stopTime  = '<div class="stop">' + (start.getTime() != stop.getTime() ? my_time_format(stop) : '') +  '</div>';
          const where = event.location ? '<span class="where">' +  ' in ' + event.location + '</span>' : ''
          const what = '<div class="what align-self-center">' + event.summary + where + '</div>';
          const when = '<div class="ml-auto d-flex-col">' + startTime + stopTime + '</div>';
          html += '<li class="event d-flex ' + eventType(event) + '">' + what  + when + '</li>';
    });
    return html;
}

function showMessage(text) {
    $('#upcoming ul').html('<li><div class="what align-self-center">' + text  + '</div></li>');
}

$().ready(function() {
    let today = new Date(Date.now());
    today.setHours(0); // include all events from today (even if already past)
    // read events from today through next 8 days
    const ndays = 20;
    const dayms = 1000*60*60*24; // convert to ms
    const span = '&timeMin=' + today.toISOString() + '&timeMax=' + new Date(today.getTime() + ndays*dayms).toISOString();
    const options = '&singleEvents=true&orderBy=startTime&sortorder=ascending' + span;
    const url = 'https://www.googleapis.com/calendar/v3/calendars/ID/events?key=AIzaSyAswReDophbZgMu3NnkPp6KV27Or1u2rS4' + options;
    const id = 'h15bjedfkcdpfa17tdf5d0l2j8@group.calendar.google.com';
    const request = url.replace('ID', id)

    //const canvas = 'https://canvas.stanford.edu/feeds/calendars/user_dxK91vZkYAnDP6V6jvO2eqXPPo3K9AbHAIFuWKX6.ics';
    //readICS(canvas);

    showMessage('Loading... ');
    $.when($.getJSON(request)).done(function(data) {
        // event w/ private visibility will not have summary, skip it
        // filter out events with no dateTime (e.g. all-day events, which only have date)
        let events = data.items.filter(e => e.summary && e.start.dateTime && e.end.dateTime);
        // cheezy (string sort) but 'works' as format is YYYY-MM-DD-HH-MM-SS
        events.sort((a, b) => a.start.dateTime.localeCompare(b.start.dateTime));
        if (events.length == 0)
            showMessage('None');
        else
            $('#upcoming ul').html(createHTML(events));
    });
});
