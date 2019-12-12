$().ready(function() {
    var b21 = '7hiop9tvins97kal7flob282e8@group.calendar.google.com';
    var ours = 'cs.stanford.edu_nhpcljcvd03ub5s8rkf14o3vig@group.calendar.google.com'
    var base = 'https://www.googleapis.com/calendar/v3/calendars/' + ours + '/events?key=AIzaSyAswReDophbZgMu3NnkPp6KV27Or1u2rS4';
    var min = "&timeMin=" + new Date().toISOString();
    var dayms = 1000*60*60*24; // convert to ms
    var max = "&timeMax=" + new Date(Date.now() + 12*dayms).toISOString(); 
    var options = '&singleEvents=true&orderBy=startTime&sortorder=ascending' + min + max;
    console.log("url is " + base + options);

    $.getJSON(base + options, function(data){
      var html = '';
      var prevDate = undefined;
      $.each(data.items, function(i, event){

        if (event.summary) {  // event w/ private visible has no summary, skip it

            var start = new Date(event.start.dateTime);
            var end = new Date(event.end.dateTime);

            if (start.getDate() != prevDate) {
                weekday = start.toLocaleDateString("en-US", {weekday:"long"});
                date = start.toLocaleDateString("en-US", {month:"short", day:"numeric", year:"numeric",}) 
                inner = '<span style="">' + weekday + '</span><span style="float:right;">' + date + '</span>';
                html += '<li class="list-group-item" style="padding: 3px 10px;color:#060;background-color:#eeeeee;font-weight:bold">' + inner + "</li>"
            }

            function my_time_format(t) {
                return t.toLocaleTimeString("en-US", { hour:"numeric", minute:"2-digit" }).toLowerCase().replace(":00", "");
            };

            var when = my_time_format(start) + ' - ' + my_time_format(end);
            var descr =  "" + event.summary + "" + " in " + event.location;
            inner = '<span style="float:left; width:10em;">' + when + '</span><span style="">' + descr + '</span>';
            html += '<li class="list-group-item" style="padding: 1px 20px;border-top:none;">' + inner + "</li>";
            prevDate = start.getDate();
        }
      });
    $('#upcoming ul').html(html ? html : "<li>None found</li>"); 
    });
});