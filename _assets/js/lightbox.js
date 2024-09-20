// Started with https://jekyllcodex.org/without-plugin/lightbox/
// Made some simplifications

function showInLightbox(img) {
    var src = img.getAttribute('src');
    var title = img.getAttribute('title');
    if (!title) title = "";
    document.getElementById('lightbox').innerHTML = '<a id="close"></a><div class="img" style="background: url(\''
        +src+'\') center center / contain no-repeat;" title="'+title+'" ><img src="'+src+'" /></div><span>'+title+'</span>';
    document.getElementById('lightbox').style.display = 'block';
}

document.addEventListener("DOMContentLoaded", function() {

    //create lightbox div in the footer
    var newdiv = document.createElement("div");
    newdiv.setAttribute('id',"lightbox");
    document.body.appendChild(newdiv);

    // click on lightbox -> close
    document.getElementById('lightbox').addEventListener("click", function(event) {
        this.innerHTML = '';
        document.getElementById('lightbox').style.display = 'none';
    });

    // click on image -> show in lightbox
    var elements = document.querySelectorAll('img.zoom');
    elements.forEach(element => {
        element.addEventListener("click", function(event) {
            console.log("add listener on img.zoom");
            event.preventDefault();
            showInLightbox(this);
        });
    });

});
