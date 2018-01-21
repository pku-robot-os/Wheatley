var c=0
var t
function httpGet(theUrl)
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", theUrl, false); // false for synchronous request
    xmlHttp.send( null );
    return xmlHttp.responseText;
}
function timedCount(){
	document.getElementById('txt').value=httpGet('query')
	t=setTimeout("timedCount()",1000)
}


