function fireClick(node) {
    if (document.createEvent) {
        var evt = document.createEvent('MouseEvents');
        evt.initEvent('click', true, false);
        node.dispatchEvent(evt);    
    } else if (document.createEventObject) {
        node.fireEvent('onclick') ; 
    } else if (typeof node.onclick == 'function') {
        node.onclick(); 
    }
}

function clickAction() {
	var ol = document.getElementById('b_results');
	if (ol != null) {
		var a = ol.getElementsByTagName('A')[0];
		if (a != null) {
			fireClick(a);
		}
	}
}