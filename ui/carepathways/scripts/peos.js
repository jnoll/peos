
function loadModels(modelsXML) { 

    target = $("#carepathways");

    var models = $('models', modelsXML);
    models.children('model').each(function() {
		var carepathway = $(this).text();
		var carepathwayLi = $('<li></li>', {"class": "carepathway"});
		carepathwayLi.attr("name", carepathway);
		carepathwayLi.attr("id", carepathway);
		carepathwayLi.append($('<a></a>', {"class": "toggle"}).text(carepathway.replace(/_/g," ")));
		$('#carepathways').append(carepathwayLi, "\n");
	});

	$('#carepathways li.carepathway').children('a').click(function(){
       openCarePathway($(this).parent());
    });
	//console.log($('html').html());
}

function createAction(action, parent, pid) {
	var actionName = action.attr("name");
	console.log("@createAction: " + actionName);
	var actionLi = $('<li></li>', {"data-cp-action": actionName, "class": "action"});
	actionLi.attr("data-cp-state", action.attr("state").toLowerCase());
	//actionLi.text(action.attr("name") + " (" + action.attr("state").toLowerCase() + ")");
	actionLi.append($('<a></a>', {"class": "toggle"}).text(actionName.replace(/_/g," ")));
	//alert(action.attr("name"));
	//actionLi.appendTo(parent);
	parent.append("\n", actionLi);
	var actionDetailsDiv = $('<div></div>', {"class": "actiondetails"});
	actionLi.append("  \n", actionDetailsDiv);
	var actionDetailsDl = $('<dl></dl>');
	actionDetailsDiv.append("    \n", actionDetailsDl);
	actionDetailsDl.append("      \n", $('<dt></dt>').text("State: "));
	actionDetailsDl.append($('<dd></dd>', {"class": "state"}).text(action.attr("state")));
	
	if (action.children("req_resource").length > 0) {
		//console.log("req_resource");
		actionDetailsDl.append("      \n", $('<dt></dt>').text("Required resources: "));
		var reqResourcesUl = $('<ul></ul>');
		actionDetailsDl.append($('<dd></dd>').append(reqResourcesUl));
		action.children("req_resource").each(function() {
				console.log($(this).attr("name"));
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
		var scriptText = action.children("script").text();
		scriptText = scriptText.substring(2, scriptText.length-2)
		actionDetailsDl.append($('<dd></dd>').html(scriptText));
	}
	
	var actionButtonsDiv = $('<div></div>', {"class": "actionButtons"});
	actionDetailsDiv.append("    \n", actionButtonsDiv);
	var buttons = ["Start", "Suspend", "Finish", "Abort"];
	
	for (i in buttons) {
		actionButtonsDiv.append($('<input />', {"type": "button", "value": buttons[i]}));	
	}

	return;
}

function updateActionsState(data, carepathway, pid) {
	console.log("@updateActionsState");
	$('process_table', data).find("process[pid='" + pid + "']").find('action').each(function() {
		var state = $(this).attr("state");
		var actionName = $(this).attr("name");
		console.log("state: " + state + " - action: " + actionName);
		var actionLi = carepathway.find('.action[data-cp-action=' + actionName + ']');
		actionLi.attr("data-cp-state", state.toLowerCase());
		actionLi.find('dd.state').text(state);
	});
	//console.log(carepathway.html());
}

// Handle process table data, from getProcessState().
function handleProcessState(data) {
	console.log("@handleProcessState");
	$('process_table', data).find("process").each(function() {
		pid = $(this).attr("pid");
		model = $(this).attr("model").replace(/[.][/]/,'').replace(/[.]pml/, '');
		carepathway = $('#' + model);
		console.log("@udpateProcessState: pid=" + pid + " model=" + model + " carepathway.length=" + carepathway.length);
		if (carepathway.length) {
		    updateActionsState(data, carepathway, pid);
		}
	});
}

// Get current process state for all processes from server.
function updateProcessState() {
    $.ajax({
	async: false,
	type: "GET",
	url: "peos.cgi",
	data: "",
	processData: false,
	success: function(data) {  
		handleProcessState(data);
	    },
	error: function(XMLHttpRequest, textStatus, errorThrown) { 
		console.log("Status: " + textStatus); console.log("Error: " + errorThrown); 
	    },
	dataType: "xml"
	});
}

function createElement(element, type, parent, pid) {	
	var elementLi = $('<li></li>', {"class": type});
	//elementLi.text(element.attr("name"));
	elementLi.append($('<a></a>', {"class": "toggle"}).text(element[0].nodeName));
	parent.append("\n", elementLi); 
	var elementUl = $('<ul></ul>', {"class": "toggle"}).appendTo(elementLi);
	processElements(element, elementUl, pid);
	return;
}

function processElements(node, parent, pid) {
	node.children().each(function() {
		var element = $(this);
		var nodeType = element[0].nodeName;
		if (nodeType == "action") {
			createAction(element, parent, pid);	
		} else if (nodeType == "sequence") {
			if (element.children().length == 1) {
				processElements(element, parent, pid);
			} else if (element.children().length > 1) {
				createElement(element, nodeType, parent, pid)
			}
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
	.fail(function(e, status, msg) { console.log(":getModelsXML:error: " + status + " : " + msg + " : " + e); })
    return false; 
}

function openCarePathway(carepathway) {
	var carepathwayName = carepathway.attr("name");
	console.log("@openCarePathway: name=" + carepathwayName);
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
					console.log("Status: " + textStatus); console.log("Error: " + errorThrown); 
				},
			dataType: "xml"
		});
		//alert('created');	
	}	
	//console.log("openCarePathway html: \n");
	//console.log($('html').html());
}


function loadProcess(processXML, carepathway, pid) {
	//console.log(processXML);
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
	
	$('#carepathways ul.process li ul.toggle').hide();
	
	$('#carepathways li.action[data-cp-state=ready]').parents('ul.toggle').show();
	//$('#carepathways li.action[data-cp-state=available]').parents('ul.toggle').show();
		
	carepathway.find('li a.toggle').click(function(){
		$(this).parent().children('ul').toggle();
	});
	
	carepathway.find('li.action div.actiondetails').hide();
	
	carepathway.find('li.action a.toggle').click(function(){
		//alert("action");
		$(this).parent().children('div.actiondetails').toggle();
	});
	
	
	// Bind a function to the action buttons.
	carepathway.attr("data-cp-pid", pid);
	carepathway.find('.actiondetails input').click(function(){
		//alert("button");
		var event = $(this).attr("value").toLowerCase();
		var actionName = $(this).parents('.action').attr("data-cp-action");
		var data = "pid=" + pid + "&action=" + actionName + "&event=" + event;
		console.log("query string: " + data);
		$.ajax({
			async: false,
			type: "POST",
			url: "peos.cgi",
			data: data,
			processData: false,
			success: function(data) {  
			    handleProcessState(data); // data has state of all processes, so update them all.
				},
			error: function(XMLHttpRequest, textStatus, errorThrown) { 
					console.log("Status: " + textStatus); console.log("Error: " + errorThrown); 
				},
			dataType: "xml"
		});
		console.log("@clicked action button");
	});
	
	
	//alert(process.html());
	console.log("loadProcess html: \n");
	//console.log($('html').html());
}

// Delete all processes in current view.
function deleteProcesses() {
	console.log("@deleteProcesses: not implemented");
}

function deleteProcess(pid) {
	console.log("@deleteProcess: deleting pid=" + pid+ "&event=delete");

	$.ajax({
			async: false,
			type: "POST",
			url: "peos.cgi",
			data: "pid=" + pid+ "&event=delete",
			processData: false,
			error: function(XMLHttpRequest, textStatus, errorThrown) { 
					console.log("Status: " + textStatus); console.log("Error: " + errorThrown); 
				},
		});
		//alert('deleted');
}

function toggleDecisionSupport(button) {
	if ($("#supportsystem").css('display') == 'none') {
		//$("#main").removeClass("pure-u-19-24");
		//$("#main").addClass("pure-u-13-24");
		$("#supportsystem").css('display', 'block');
		button.addClass('pressed');
		button.attr('title', "Hide Decision support");
	} else {
		//$("#main").removeClass("pure-u-19-24");
		//$("#main").addClass("pure-u-13-24");
		$("#supportsystem").css('display', 'none');
		button.removeClass('pressed');
		button.attr('title', "Display Decision support");
	}
		
}

// Wait for the DOM to be loaded. 
$(document).ready(function() { 
	console.log("********* ready ********");
	
	getModelsXML();
	
	$("#decisionsupport_btn").click(function(){
	  toggleDecisionSupport($(this));
	});
	
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

