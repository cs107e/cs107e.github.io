/*  Author: Julie Zelenski  Winter quarter 2020

    Reminder of how calendar widget works:
    Displays next 2 weeks of events from our Google calendars
    Lecture/lab schedule is from cs107e specific calendar
    OH are recorded as room reservation on B21 calendar, filter events to those with our tag
    Most events recorded on B21 calendar, just be sure to include #CS107E in event name
    If calendar event not located in B21, add to the cs107e-specific calendar
    For now I am adding lecture/lab/assign due dates to google calendar manually
    Would be nice if it auto-populated from quarter.yml data
*/
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
function read_ics(url) {
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

function eventType(event) {
    if (event.summary.includes('YEAH')) return "oh";
    if (event.summary.includes('Lecture')) return "lecture";
    if (event.summary.includes('Lab')) return "lab";
    if (event.summary.includes('Assign')) return "assign";
    if (event.summary.includes('OH')) return "oh";
    return "";
}

$().ready(function() {
    var dayms = 1000*60*60*24; // convert to ms
    var today = new Date(Date.now());
    today.setHours(0); // include all events from today (even if already past)
    // read events from today through 7 days from now
    var min = '&timeMin=' + today.toISOString();
    var max = '&timeMax=' + new Date(today.getTime() + 7*dayms).toISOString();
    var options = '&singleEvents=true&orderBy=startTime&sortorder=ascending' + min + max;

    var base = 'https://www.googleapis.com/calendar/v3/calendars/WHICH/events?key=AIzaSyAswReDophbZgMu3NnkPp6KV27Or1u2rS4';
    var b21 = '7hiop9tvins97kal7flob282e8@group.calendar.google.com';
    var ours = 'stanford.edu_6c8r06pvjoff3ims422n60p3mk@group.calendar.google.com';
    var url = base + options;

    var canvas = 'https://canvas.stanford.edu/feeds/calendars/user_dxK91vZkYAnDP6V6jvO2eqXPPo3K9AbHAIFuWKX6.ics';
    read_ics(canvas);

    var request1 = $.getJSON(url.replace('WHICH', b21));
    var request2 = $.getJSON(url.replace('WHICH', ours));

    $.when(request1, request2).done(function(data1, data2) {
        // event w/ private visibility will not have summary, skip it
        // only harvest events with our tag from B21 (other class reservations ignored)
        var fromB21 = data1[0].items.filter(e => e.summary && e.summary.includes('#CS107E'));
        var fromOurs = data2[0].items.filter(e => e.summary);
        var allEvents = fromB21.concat(fromOurs);
        // cheezy (string sort) but 'works' as format is YYYY-MM-DD-HH-MM-SS
        allEvents.sort((a, b) => a.start.dateTime.localeCompare(b.start.dateTime));

        var html = '<li class="list-header">Upcoming Events</li>';
        var prevDate = undefined;

        $.each(allEvents, function(i, event){
            var start = new Date(event.start.dateTime);
            var end = new Date(event.end.dateTime);

            if (start.getDate() != prevDate) {
                date = start.toLocaleDateString('en-US', {weekday: 'long', month:'short', day:'numeric'})
                html += '<li class="day">' + date + '</li>';
                prevDate = start.getDate();
            }

            function formatted(t) {
               // return t.toLocaleTimeString('en-US', { hour:'numeric', minute:'2-digit' }).toLowerCase().replace(':00', '');
                return t.toLocaleTimeString('en-US', { hour:'numeric', minute:'2-digit' }).toLowerCase();
            };
            var startTime = formatted(start);
            var stopTime = start.getTime() != end.getTime() ? formatted(end) : '';
            //var duration = startTime + (stopTime != '' ? ' - ' + stopTime : '');
            var title = event.summary.replace('#CS107E', '');
            //var where = (event.location ? ' in ' + event.location : '');
            desc = '<div class="what align-self-center">' + title + '</div>';
            time = '<div class="ml-auto d-flex-col">' +
                       '<div class="start">' + startTime + '</div>' +
                       '<div class="stop">' + stopTime + '</div></div>';
            html += '<li class="event d-flex ' + eventType(event) + '">' + desc + time + '</li>';
      });
    $('#upcoming ul').html(html ? html : '<li>None found</li>');
    });
});
