/*  Author: Julie Zelenski  Winter quarter 2020

    Reminder of how calendar widget works:
    Displays next 2 weeks of events from our Google calendars
    Lecture/lab sechedule is from cs107e specific calendar
    OH are recorded as room reservation on B21 calendar, filter events to those with our tag
    Most events recorded on B21 calendar, just be sure to include #CS107E in event name
    If calendar event not located in B21, add to the cs107e-specific calendar
    For now I am adding lecture/lab/assign due dates to google calendar manually
    Would be nice if it auto-populated from quarter.yml data
*/

$().ready(function() {
    var base = 'https://www.googleapis.com/calendar/v3/calendars/WHICH/events?key=AIzaSyAswReDophbZgMu3NnkPp6KV27Or1u2rS4';
    var min = '&timeMin=' + new Date().toISOString();
    var dayms = 1000*60*60*24; // convert to ms
    var max = '&timeMax=' + new Date(Date.now() + 12*dayms).toISOString();
    var options = '&singleEvents=true&orderBy=startTime&sortorder=ascending' + min + max;
    var url = base + options;

    var b21 = '7hiop9tvins97kal7flob282e8@group.calendar.google.com';
    var ours = 'stanford.edu_6c8r06pvjoff3ims422n60p3mk@group.calendar.google.com';
    // var ours = 'cs.stanford.edu_nhpcljcvd03ub5s8rkf14o3vig@group.calendar.google.com';

    var request1 = $.getJSON(url.replace('WHICH', b21));
    var request2 = $.getJSON(url.replace('WHICH', ours));

    $.when(request1, request2).done(function(data1, data2) {
        // event w/ private visibility will not have summary, skip it
        // only harvest events with our tag from B21 (other class reservations ignored)
        var from_b21 = data1[0].items.filter(e => e.summary && e.summary.includes('#CS107E'));
        var from_ours = data2[0].items.filter(e => e.summary);
        var all_events = from_b21.concat(from_ours);
        // cheezy (string sort) but 'works' as format is YYYY-MM-DD-HH-MM-SS
        all_events.sort((a, b) => a.start.dateTime.localeCompare(b.start.dateTime));

        var html = '';
        var prevDate = undefined;
        $.each(all_events, function(i, event){
            var start = new Date(event.start.dateTime);
            var end = new Date(event.end.dateTime);

            if (start.getDate() != prevDate) {
                weekday = start.toLocaleDateString('en-US', {weekday:'long'});
                date = start.toLocaleDateString('en-US', {month:'short', day:'numeric', year:'numeric',})
                inner = '<span>' + weekday + '</span><span style="float:right;">' + date + '</span>';
                html += '<li class="list-group-item" style="padding: 3px 10px;color:#060;background-color:#eeeeee;font-weight:bold">' + inner + "</li>"
            }

            function my_time_format(t) {
                return t.toLocaleTimeString('en-US', { hour:'numeric', minute:'2-digit' }).toLowerCase().replace(':00', '');
            };
            var when = my_time_format(start) + (start.getTime() != end.getTime() ? (' - ' + my_time_format(end)) : '');
            var title = event.summary.replace('#CS107E', '');
            var where = (event.location ? ' in ' + event.location : '');
            var descr =  '<strong>' + title + '</strong>' + where;
            inner = '<span style="float:left; width:10em;">' + when + '</span><span>' + descr + '</span>';
            html += '<li class="list-group-item" style="padding: 1px 20px;border-top:none;">' + inner + "</li>";
            prevDate = start.getDate();
      });
    $('#upcoming ul').html(html ? html : '<li>None found</li>');
    });
});
