function actionForm(pid, action, op) {
    var newTxt = [];
    var i = 0;
    newTxt[i++] = "<td>";
    newTxt[i++] = "<form>";
    newTxt[i++] = "<input type='hidden' name='pid' value='" + pid + "'/>";
    newTxt[i++] = "<input type='hidden' name='pid' value='" + action + "'/>";
    newTxt[i++] = "<input type='submit' name='op' value='" + op + "'/>";
    newTxt[i++] = "</form>";
    newTxt[i++] = "</td>";
    return newTxt.join("\n");
}

function processXML(responseXML) { 
    // 'responseXML' is the XML document returned by the server; we use 
    // jQuery to extract the content from the XML doc 

    target = $("#results");
    var i=0;
    var newTxt = [];

    newTxt[i++] = "<div id='results'>";
    newTxt[i++] = "<h2>Models</h2>";
    newTxt[i++] = "<table id='models'>";

    // See
    // http://www.tvidesign.co.uk/blog/improve-your-jquery-25-excellent-tips.aspx#tip9
    // for an explanation of the $(selector, context) form.
    var models = $('models', responseXML);
    models.children('model').each(function() {
	    newTxt[i++] = "<tr>"; 
	    newTxt[i++] = "<td>" ;
	    newTxt[i++] = "<form>";
	    newTxt[i++] =  $(this).text();
	    newTxt[i++] = "<input type='hidden' name='model' value='" + $(this).text() + "'/>";
	    newTxt[i++] = "<input type='submit' name='op' value='create'/>";
	    newTxt[i++] = "</form>";
	    newTxt[i++] = "</td>";
	    newTxt[i++] = "</tr>"; 
	});
    newTxt[i++] = "</table>";

    newTxt[i++] = "<h2>Process Instances</h2>";
    newTxt[i++] = "<table id='proc_table'>";
    var recs = $('process_table', responseXML);
    recs.children('process').each(function() {
	    console.log($(this).attr("pid"))
	    newTxt[i++] = "<tr>"; 
	    newTxt[i++] = "<td>" + $(this).attr("pid") + "</td>";
	    newTxt[i++] = "<td>" + $(this).attr("model") + "</td>";
	    newTxt[i++] = "<td>" + $(this).attr("status") + "</td>";
	    newTxt[i++] = "</tr>"; 

	    var actions = $('action', this).each(function() {
		    var name = $(this).attr("name");
		    var pid  = $(this).attr("pid");

		    console.log($(this).attr("name"))
		    newTxt[i++] = "<tr>"; 
		    newTxt[i++] = "<td>" + name + "</td>";
		    newTxt[i++] = "<td>" + $(this).attr("state") + "</td>";
		    newTxt[i++] = "<td>" + $("req_resource", this).attr("name") + "</td>";
		    newTxt[i++] = "<td>" + $("req_resource", this).attr("value") + "</td>";
		    newTxt[i++] = "<td>" + $("prov_resource", this).attr("name") + "</td>";
		    newTxt[i++] = "<td>" + $("prov_resource", this).attr("value") + "</td>";

		    newTxt[i++] = actionForm(pid, name, "start");
		    newTxt[i++] = actionForm(pid, name, "suspend");
		    newTxt[i++] = actionForm(pid, name, "finish");
		    newTxt[i++] = actionForm(pid, name, "abort");

		    newTxt[i++] = "</tr>"; 

		});
	});
    newTxt[i++] = "</table>";
    newTxt[i++] = "</div>"; 
    target.replaceWith(newTxt.join("\n"));
}

function submitEvent() {
    //alert("submitEvent");
    $.ajax({
	type: "GET",
	url: "peos.cgi",
	processData: false,
	success: processXML,
	//	dataType: "xml",
	})
	.fail(function(e, status, msg) { alert(":submitEvent:error: " + status + " : " + msg + " : " + e); })

    return false; 
}

// Wait for the DOM to be loaded. 
$(document).ready(function() { 
	// Bind an action to the submit button.
	$('#sg-advisor-consult').submit(function(e){ submitEvent(); return e.preventDefault(); });
	// Load the questions.
	submitEvent();
    }); 

