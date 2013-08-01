(function() {
    var widgets = [];
    var editor; 
    function updateHints(result) {

	editor.operation(function(){
		for (var i = 0; i < widgets.length; ++i) {
		    editor.removeLineWidget(widgets[i]);
		}
		widgets.length = 0;

		var errors = result.match(/^.*([\n\r]+|$)/gm);

		
		for (var i = 0; i < errors.length; ++i) {
		    var err = errors[i];
		    var fields = err.match(/[^:]*/g);
		    var line = fields[2];
		    if (line) {
			var msg = document.createElement("div");
			var icon = msg.appendChild(document.createElement("span"));
			icon.innerHTML = "!!";
			icon.className = "CodeMirror-lint-mark-error"; //lint-error-icon
			msg.appendChild(document.createTextNode(err));
			msg.className = "lint-error";
			widgets.push(editor.addLineWidget(line - 1, msg,
							  {coverGutter: false, noHScroll: true}));
		    }
		}
	    });
	var info = editor.getScrollInfo();
	var after = editor.charCoords({line: editor.getCursor().line + 1, ch: 0}, "local").top;
	if (info.top + info.clientHeight < after) {
	    editor.scrollTo(null, after - info.clientHeight + 3);
	}
    }


    function processResult(response) {
	//alert("pmlcheck.js:processResult: " + response);
	updateHints(response);
    };

    function submitCheck() {
	//alert("submitCheck");
	$.ajax({
	    type: "POST",
	    url: "check.cgi",
	    data: $('#pmlcheck').serialize(),
	    processData: false,
	    success: processResult,
	    dataType: "text",
	    })
	    .fail(function(e, status, msg) { alert("pmlcheck.js:submitCheck:error: " + status + " : " + msg + " : " + e); })
	    .success(function(e, status, msg) {
		    //alert("pmlcheck.js:submitCheck:success: " + status + " : " + msg + " : " + e)
	    });
	return false; 
    }

    $(document).ready(function() {
	    var editedText = $('#editedText');
	    editor = CodeMirror.fromTextArea(document.getElementById("editedText"),
					     {mode: 'tcl',
					      lineNumbers: true,
					      theme: "night"
					     }
					     );
	    $('form').submit(function(e){ e.preventDefault(); submitCheck(); return false; });
	});
}());