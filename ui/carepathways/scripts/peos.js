function actionForm(pid, action, event) {
    var newTxt = [];
    var i = 0;
    newTxt[i++] = "<div>";
    newTxt[i++] = "<form action='#' class='peos-op' method=POST>";
    newTxt[i++] = "<input type='hidden' name='pid' value='" + pid + "'/>";
    newTxt[i++] = "<input type='hidden' name='action' value='" + action + "'/>";
    newTxt[i++] = "<input type='hidden' name='event' value='" + event + "'/>"; // So serialize() will include a hidden 'event' input.
    newTxt[i++] = "<input type='submit' class='peos-submit' name='submit' value='" + event + "'/>";
    newTxt[i++] = "</form>";
    newTxt[i++] = "</div>";
	//alert (newTxt.join("\n"));
    return newTxt.join("\n");
}

function loadModels(modelsXML) { 

    target = $("#carepathways");

    var models = $('models', modelsXML);
    models.children('model').each(function() {
		var carepathway = $(this).text();
		var carepathwayLi = $('<li></li>', {"class": "carepathway"});
		carepathwayLi.attr("name", carepathway);
		carepathwayLi.attr("id", carepathway);
		carepathwayLi.text(carepathway);
		$('#carepathways').append(carepathwayLi, "\n");
	});

	$('#carepathways li.carepathway').click(function(){
       openCarePathway($(this));
    });
	console.log($('html').html());
}

function createAction(action, parent) {	
	var actionLi = $('<li></li>', {"class": "action"});
	actionLi.addClass(action.attr("state").toLowerCase());
	//actionLi.text(action.attr("name") + " (" + action.attr("state").toLowerCase() + ")");
	actionLi.text(action.attr("name"));
	//alert(action.attr("name"));
	//actionLi.appendTo(parent);
	parent.append("\n", actionLi);
	
	// Bind an action to the submit button.
   /* $('form').submit(function(e){
	    e.preventDefault(); 
	    $.ajax({
		type: "POST",
		url: "peos.cgi",
		data: $(this).serialize(),
		processData: false,
		success: processXML,
		//	dataType: "xml",
		})
		.fail(function(e, status, msg) { alert(":submitEvent:error: " + status + " : " + msg + " : " + e); })
		return false; 
	});*/
	return;
}

function createElement(element, type, parent) {	
	var elementLi = $('<li></li>', {"class": type});
	//elementLi.text(element.attr("name"));
	parent.append("\n", elementLi); 
	var elementUl = $('<ul></ul>').appendTo(elementLi);
	processElements(element, elementUl);
	return;
}

function processElements(node, parent) {
	node.children().each(function() {
		var element = $(this);
		var nodeType = element[0].nodeName;
		if (nodeType == "action") {
			createAction(element, parent);	
		} else {
			createElement(element, nodeType, parent)
		}
	});
	return;
}


function getModelsXML() {
    //alert("loadProcessTable");
    $.ajax({
	type: "GET",
	url: "peos.cgi",
	processData: false,
	success: loadModels,
	dataType: "xml",
	})
	.fail(function(e, status, msg) { alert(":getModelsXML:error: " + status + " : " + msg + " : " + e); })
    return false; 
}

function openCarePathway(carepathway) {
	var carepathwayName = carepathway.attr("name");
	
	if ($('#' + carepathwayName + ' ul.process').length) {
		//alert(carepathway.html());
		$('#' + carepathwayName + ' ul.process').toggle()
	} else {
		var pid = $.cookie(carepathwayName);
		if  (pid == undefined ) {
			pid=-1;
		}
		$.ajax({
			type: "POST",
			url: "peos.cgi",
			data: "model=" +carepathwayName + "&event=create",
			processData: false,
			success: function(data) {  
				   loadProcess(data, carepathway, pid);
				},
			error: function(XMLHttpRequest, textStatus, errorThrown) { 
					alert("Status: " + textStatus); alert("Error: " + errorThrown); 
				},
			dataType: "xml"
		});
		//alert('created');
	}	
}


function loadProcess(processXML, process, pid) {
	var processUl = $('<ul></ul>', {"class": "process"});
	processUl.appendTo(process);
	var procTable = $('process_table', processXML);
	//alert("pid: " +  pid);
	if (pid < 0) {
		//alert("pid<0");
		procTable.children('process').each(function() {
			//alert($(this).attr('model'));
			//alert(process.attr("name") + ".pml");
			
				var processName = process.attr("name");
				if ($(this).attr('model').search(processName + ".pml")>=0) {
					processElements($(this), processUl);	
					//alert(processName);
					//alert($(this).attr("pid"));
					$.cookie(processName, $(this).attr("pid"));
					//alert("cookie: " + $.cookie(processName));
				}
			
		});	
	} else {
		processElements(procTable.children("process[pid='" + pid + "']"), processUl);	
	}
	//alert(process.html());
}

function deleteProcess(pid) {
	console.log('deleting');
	console.log("pid=" + pid+ "&event=delete");
	$.ajax({
			async: false,
			type: "POST",
			url: "peos.cgi",
			data: "pid=" + pid+ "&event=delete",
			processData: false,
			error: function(XMLHttpRequest, textStatus, errorThrown) { 
					alert("Status: " + textStatus); alert("Error: " + errorThrown); 
				},
		});
		//alert('deleted');
}

// Wait for the DOM to be loaded. 
$(document).ready(function() { 
	console.log("********* ready ********");
	
	getModelsXML();
	
	//$.cookie(test, 1);	
	
	//$.removeCookie('test');
	
	
	
	$(window).bind('beforeunload', function() {
		for (cookieName in $.cookie()) {
			if (cookieName != "") {
				console.log("cookie name: " + cookieName);
				console.log("pid: " + $.cookie(cookieName));
				var pid = $.cookie(cookieName);
				$.removeCookie(cookieName);
				deleteProcess(pid);
			}
		}				
	});
	
}); 

