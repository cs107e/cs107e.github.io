---
permalink: /search/
title: Search
hidden_from_search: true
---

<!-- no action, will submit to this page -->
<form method="get" role="search">
  <div class="input-group">
  <input type="text" class="form-control bg-light" id="search-text" name="query" placeholder="🔍 Search {{site.data.course.id}}..." size=40 title="Query string" autofocus="autofocus" onfocus="this.select()">
  <div class="input-group-append">
  <input type="submit" class="btn btn-secondary" value="Search" >
  </div>
</div>
</form>
<hr>

<ul id="search-results" class="list-unstyled"></ul>

<script src="/_assets/js/lunr.js"></script>
<script>
  (function() {

    // show portion of contents that match search term, boldface matches
    function formatContent(content, searchTerm) {
        var termIdx = content.toLowerCase().indexOf(searchTerm.toLowerCase());
        if (termIdx >= 0) {
            var startIdx = Math.max(0, termIdx - 140);
            var endIdx = Math.min(content.length, termIdx + searchTerm.length + 140);
            var trimmedContent = (startIdx === 0) ? "" : "&hellip;";
            trimmedContent += content.substring(startIdx, endIdx);
            trimmedContent += (endIdx >= content.length) ? "" : "&hellip;"
            var highlightedContent = trimmedContent.replace(new RegExp(searchTerm, "ig"), function matcher(match) {
                return "<strong>" + match + "</strong>";
            });
            return highlightedContent;
        }
        else {
            var emptyTrimmedString = content.substr(0, 280);
            emptyTrimmedString += (content.length < 280) ? "" : "&hellip;";
            return emptyTrimmedString;
        }
    }

    function getQueryVariable(variable) {
      var query = window.location.search.substring(1);
      var vars = query.split('&');

      for (var i = 0; i < vars.length; i++) {
        var pair = vars[i].split('=');

        if (pair[0] === variable) {
          return decodeURIComponent(pair[1].replace(/\+/g, '%20'));
        }
      }
    }
    var searchTerm = getQueryVariable('query');   // get query from url

    if (searchTerm) {  // query from url, put into text field
      var field = document.getElementById('search-text');
      field.setAttribute("value", searchTerm);
      field.setSelectionRange(0, searchTerm.length);

        // Read search_data.json file (file was generated during jekyll build)
        $.getJSON('/search_data.json', function(loaded_data) {
          var ndocs = 0;
          var idx = lunr(function () {
            var bldr = this;
            bldr.field('id');
            bldr.field('url'); // init with fields to be searched
            bldr.field('pretty');
            bldr.field('content');
            var bldr = this;
            $.each(loaded_data, function(index, value) {
              ndocs += 1;
              bldr.add({
                'id': index,
                'url': value.url,
                'pretty': value.pretty,
                'content': value.content
              });
            });
          });

          var results = idx.search(searchTerm);

          // display results
          var searchResults = document.getElementById('search-results');
          if (results.length) {
            var appendString = '';
            for (var i = 0; i < results.length; i++) {
              var item = loaded_data[results[i].ref];
              appendString += '<li><a href="' + item.url + '"><h3>' + item.pretty + '</h3></a>';
              appendString += '<p>' + formatContent(item.content,searchTerm) + '...</p></li>';
            }
            searchResults.innerHTML = appendString;
          } else {
            searchResults.innerHTML = '<li><center><div class="alert alert-warning d-inline-block">No results found for <B>' + searchTerm + '</B> (' + ndocs + ' pages searched) </div><center></li>';
          }
        });
    }
  }
)();
</script>
