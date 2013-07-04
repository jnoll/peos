function loadModels(modelsXML) { 

    target = $("#carepathways");

    var models = $('models', modelsXML);
    models.children('model').each(function() {
		var carepathway = $(this).text();
		var carepathwayLi = $('<li></li>', {"class": "carepathway"});
		carepathwayLi.attr("name", carepathway);
		carepathwayLi.attr("id", carepathway);
		carepathwayLi.append($('<a></a>', {"class": "carepathwaylink"}).text(carepathway));
		$('#carepathways').append(carepathwayLi, "\n");
	});

	$('#carepathways li.carepathway a.carepathwaylink').click(function(){
       openCarePathway($(this).parent());
    });
	//console.log($('html').html());
}

function createAction(action, parent, pid) {	
	var actionName = action.attr("name");
	var actionLi = $('<li></li>', {"class": "action"});
	actionLi.addClass(action.attr("state").toLowerCase());
	//actionLi.text(action.attr("name") + " (" + action.attr("state").toLowerCase() + ")");
	actionLi.text(actionName);
	//alert(action.attr("name"));
	//actionLi.appendTo(parent);
	parent.append("\n", actionLi);
	var actionDetailsDiv = $('<div></div>', {"data-cp-action": actionName, "class": "actiondetails"});
	actionLi.append("  \n", actionDetailsDiv);
	var actionDetailsDl = $('<dl></dl>');
	actionDetailsDiv.append("    \n", actionDetailsDl);
	actionDetailsDl.append("      \n", $('<dt></dt>').text("State: "));
	actionDetailsDl.append($('<dd></dd>').text(action.attr("state")));
	
	if (action.children("req_resource").length > 0) {
		actionDetailsDl.append("      \n", $('<dt></dt>').text("Required resources: "));
		var reqResourcesUl = $('<ul></ul>');
		actionDetailsDl.append($('<dd></dd>').append(reqResourcesUl));
		action.children("req_resource").each(function() {
				var text = $(this).attr("name");
				text += " = ";
				text += $(this).attr("value");
				if ($(this).attr("qualifier") != "") {
					text += " (" + $(this).attr("qualifier") + ")";
				}
				reqResourcesUl.append("      \n", $('<li></li>').text(text)); 
			});	
	}
	
	if (action.children("prov_resource").length > 0) {
		actionDetailsDl.append("      \n", $('<dt></dt>').text("Provided resources: "));
		var reqResourcesUl = $('<ul></ul>');
		actionDetailsDl.append($('<dd></dd>').append(reqResourcesUl));
		action.children("prov_resource").each(function() {
				var text = $(this).attr("name");
				text += " = ";
				text += $(this).attr("value");
				if ($(this).attr("qualifier") != "") {
					text += " (" + $(this).attr("qualifier") + ")";
				}
				reqResourcesUl.append("      \n", $('<li></li>').text(text)); 
			});	
	}
	if (action.children("script").text().search("null") < 0) {
		actionDetailsDl.append("      \n", $('<dt></dt>').text("Script: "));
		actionDetailsDl.append($('<dd></dd>').text(action.children("script").text()));
	}
	
	var actionButtonsDiv = $('<div></div>', {"class": "actionButtons"});
	actionDetailsDiv.append("    \n", actionButtonsDiv);
	var buttons = ["Start", "Suspend", "Finish", "Abort"];
	
	for (i in buttons) {
		actionButtonsDiv.append($('<input />', {"type": "button", "value": buttons[i]}));	
	}

	return;
}

function createElement(element, type, parent, pid) {	
	var elementLi = $('<li></li>', {"class": type});
	//elementLi.text(element.attr("name"));
	parent.append("\n", elementLi); 
	var elementUl = $('<ul></ul>').appendTo(elementLi);
	processElements(element, elementUl, pid);
	return;
}

function processElements(node, parent, pid) {
	node.children().each(function() {
		var element = $(this);
		var nodeType = element[0].nodeName;
		if (nodeType == "action") {
			createAction(element, parent, pid);	
		} else {
			createElement(element, nodeType, parent, pid)
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
	//console.log("openCarePathway html: \n");
	//console.log($('html').html());
}


function loadProcess(processXML, carepathway, pid) {
	console.log("loadProcess pid: " + pid);
	var processUl;
	if (carepathway.children('ul.process').length == 0) {
		processUl = $('<ul></ul>', {"class": "process"}).appendTo(carepathway);
	} else {
		processUl = carepathway.children('ul.process');
		processUl.empty();
	}
	var procTable = $('process_table', processXML);
	//alert("pid: " +  pid);
	if (pid < 0) {
		//alert("pid<0");
		procTable.children('process').each(function() {
			//alert($(this).attr('model'));
			//alert(process.attr("name") + ".pml");
			
				var processName = carepathway.attr("name");
				if ($(this).attr('model').search(processName + ".pml")>=0) {
					pid = $(this).attr("pid");
					processElements($(this), processUl, pid);	
					//alert(processName);
					//alert($(this).attr("pid"));
					$.cookie(processName, pid);
					//alert("cookie: " + $.cookie(processName));
				}
			
		});	
	} else {
		processElements(procTable.children("process[pid='" + pid + "']"), processUl, pid);	
	}
	
	// Bind a function to the action buttons.
	carepathway.attr("data-cp-pid", pid);
	carepathway.find('.actiondetails input').click(function(){
		//alert("button");
		var event = $(this).attr("value").toLowerCase();
		var actionName = $(this).parents('.actiondetails').attr("data-cp-action");
		var data = "pid=" + pid + "&action=" + actionName + "&event=" + event;
		console.log("query string: " + data);
		$.ajax({
			async: false,
			type: "POST",
			url: "peos.cgi",
			data: data,
			processData: false,
			success: function(data) {  
				   loadProcess(data, carepathway, pid);
				},
			error: function(XMLHttpRequest, textStatus, errorThrown) { 
					alert("Status: " + textStatus); alert("Error: " + errorThrown); 
				},
			dataType: "xml"
		});
		console.log("@clicked action button");
	});
	
	
	//alert(process.html());
	console.log("loadProcess html: \n");
	console.log($('html').html());
}

function deleteProcess(pid) {
	//console.log('deleting');
	//console.log("pid=" + pid+ "&event=delete");
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
				//console.log("cookie name: " + cookieName);
				//console.log("pid: " + $.cookie(cookieName));
				var pid = $.cookie(cookieName);
				$.removeCookie(cookieName);
				deleteProcess(pid);
			}
		}				
	});
	
}); 

