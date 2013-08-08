/*Loads a list of all the pathway (pml files) available on the server
If not already started, it starts a process for aech pathway associated with the curent user*/
function loadModels(modelsXML) { 
	console.log("loadModels");
	var patientId = getPatientId();
	console.log(patientId);
    var target = $("#carepathways");
	$("#carepathways").attr("data-cp-patientid", patientId);
	target.empty();
	//the xml returns a model for each pml file on the server
    var models = $('models', modelsXML);
    models.children('model').each(function() {
		var carepathway = $(this).text();
		var carepathwayLi = $('<li></li>', {"class": "carepathway"});
		carepathwayLi.attr("name", carepathway);
		carepathwayLi.attr("id", carepathway);
		carepathwayLi.append($('<a></a>', {"class": "toggle"}).text(convertText(carepathway)));
		carepathwayLi.addClass('loading');
		//the corresponding pathway process for the current patient is loaded on the page
		getProcessXML(carepathway, patientId);
		$('#carepathways').append(carepathwayLi, "\n");
	});
	
	//console.log($('html').html());
}

/*Gets xml data from the server with a list of all the pathway available
then executes loadModels*/
function getModelsXML() {
    console.log("getModelsXML");
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

/*Generates the html code for to display an action on the page*/
function createAction(action, parent, pid) {
	var actionName = action.attr("name");
	//console.log("@createAction: " + actionName);
	var actionLi = $('<li></li>', {"data-cp-action": actionName, "class": "action"});
	//the status of the action is saved in data-cp-state attribute
	actionLi.attr("data-cp-state", action.attr("state").toLowerCase());
	//the status of the action is saved also in the title attribute that is displayed as a toltip
	actionLi.children('a.toggle').attr("title", "Status: " + action.attr("state").toLowerCase());
	actionLi.append($('<a></a>', {"class": "toggle"}).text(convertText(actionName)));
	parent.append("\n", actionLi);
	
	//all the details about the action (required resource, provided resources, script) are 
	//put in a div with class actiondetails. This div is normally hidden and is displayed
	//when clicking on the action name (this behaviour is implemented in loadProcess)
	var actionDetailsDiv = $('<div></div>', {"class": "actiondetails"});
	actionLi.append("  \n", actionDetailsDiv);
	var actionDetailsDl = $('<dl></dl>');
	actionDetailsDiv.append("    \n", actionDetailsDl);
	
	//gets and display all the required resources
	if (action.children("req_resource").length > 0) {
		//console.log("req_resource");
		actionDetailsDl.append("      \n", $('<dt></dt>').text("Requires: "));
		var reqResourcesUl = $('<ul></ul>');
		actionDetailsDl.append($('<dd></dd>').append(reqResourcesUl));
		action.children("req_resource").each(function() {
				//console.log($(this).attr("name"));
				var text = convertText($(this).attr("name"));
				if ($(this).attr("qualifier") != "") {
					text += " (" + $(this).attr("qualifier") + ")";
				}
				reqResourcesUl.append("      \n", $('<li></li>').text(text)); 
			});	
	}
	
	//gets and display all the provided resources
	if (action.children("prov_resource").length > 0) {
		actionDetailsDl.append("      \n", $('<dt></dt>').text("Provides: "));
		var reqResourcesUl = $('<ul></ul>');
		actionDetailsDl.append($('<dd></dd>').append(reqResourcesUl));
		action.children("prov_resource").each(function() {
				var text = convertText($(this).attr("name"));
				if ($(this).attr("qualifier") != "") {
					text += " (" + $(this).attr("qualifier") + ")";
				}
				reqResourcesUl.append("      \n", $('<li></li>').text(text)); 
			});	
	}
	
	//display the script if present
	if (action.children("script").text().search("null") < 0) {
		var scriptText = action.children("script").text();
		scriptText = scriptText.substring(2, scriptText.length-2)
		actionDetailsDiv.append("\n", $('<p></p>').html(scriptText));
	}
	
	//displays button for submitting the action events.
	//The binding of the buttons is implemented in loadProcess
	var actionButtonsDiv = $('<div></div>', {"class": "actionButtons"});
	actionDetailsDiv.append("    \n", actionButtonsDiv);
	var buttons = ["Start", "Finish", "Cancel"];
	var events = ["start", "finish", "abort"];
	
	for (i in buttons) {
		actionButtonsDiv.append($('<input />', {"type": "button", "value": buttons[i], "data-cp-event": events[i]}));	
	}

	return;
}

/*Updates the state of all actions in a process*/
function updateActionsState(data, carepathwayElement, pid) {
	console.log("updateActionsState");
	$('process_table', data).find("process[pid='" + pid + "']").find('action').each(function() {
		var state = $(this).attr("state");
		var actionName = $(this).attr("name");
		console.log("updateActionsState state: " + state + " - action: " + actionName);
		var actionLi = carepathwayElement.find('.action[data-cp-action=' + actionName + ']');
		actionLi.attr("data-cp-state", state.toLowerCase());
		actionLi.children('a.toggle').attr("title", "Status: " + state.toLowerCase());
	});
	updateCarepathwaysStatus(carepathwayElement);
	displayRelevantActions();
	//console.log(carepathwayElement.html());
}

/*Generates the html code for displaying branch, selection, iteraction and sequence*/
function createElement(element, type, parent, pid) {	
	var elementLi = $('<li></li>', {"class": type});
	elementLi.addClass('expand');
	//elementLi.text(element.attr("name"));
	elementLi.append($('<a></a>', {"class": "toggle"}).attr('title', element[0].nodeName));
	elementLi.append($('<a></a>', {"class": "expand"}).attr('title', 'open'));
	parent.append("\n", elementLi); 
	var elementUl = $('<ul></ul>', {"class": "toggle"}).appendTo(elementLi);
	processElements(element, elementUl, pid);
	return;
}

/*Evaluetes the node type (coming from the xml data) and redirects to the 
apropriate function to create the html code.
Note that a sequence node would be displayed on the page only if has more
then one element inside, otherwise the inner element is diplayed directly;
this is done to semplify the user interface*/
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

/*Display or hide the care pathway on the page i.e. all the details of a process.
If the process if not already loaded in the page, it gets it from the server and loads it */
function openCarePathway(carepathwayElement) {
	console.log("openCarePathway");
	var carepathwayName = carepathwayElement.attr("name");
	console.log(carepathwayName);
	/*while (carepathwayElement.hasClass('loading')) {
		console.log("class: " + carepathwayElement.attr('class'));
		setTimeout(function () { }, 2000);	
	}*/
	if ($('#' + carepathwayName + ' ul.process').length) {
		//alert(carepathwayElement.html());
		var process = $('#' + carepathwayName + ' ul.process');
		if (process.css('display') == 'none') {
			carepathwayElement.addClass("opened");
			process.show();
		} else {
			carepathwayElement.removeClass("opened");
			process.hide();
		}
		
	} else {
		console.log("openCarePathway else");
		getProcessXML(carepathwayName, getPatientId()); 	
	}	
	
	//all the ready, run, avalaible and satisfied actions will be visible
	displayRelevantActions();
	
	//console.log("openCarePathway html: \n");
	//console.log($('html').html());
}

/*Makes visible all the ready, run, avalaible and satisfied actions */
function displayRelevantActions() {
	$('#carepathways li.action[data-cp-state=ready]').parents('ul.toggle').show();
	$('#carepathways li.action[data-cp-state=ready]').parents('li.expand').children('a.expand').addClass('opened');
	$('#carepathways li.action[data-cp-state=available]').parents('ul.toggle').show();
	$('#carepathways li.action[data-cp-state=available]').parents('li.expand').children('a.expand').addClass('opened');
	$('#carepathways li.action[data-cp-state=satisfied]').parents('ul.toggle').show();
	$('#carepathways li.action[data-cp-state=satisfied]').parents('li.expand').children('a.expand').addClass('opened');
	$('#carepathways li.action[data-cp-state=run]').parents('ul.toggle').show();
	$('#carepathways li.action[data-cp-state=run]').parents('li.expand').children('a.expand').addClass('opened');
}

/*Gets the xml data for a process. 
The process id realted to a specific carepathway for a patient is saved in a cokie.
If the cookie is not present, a new process is created */
function getProcessXML(carepathway, patientId) {
	console.log("getProcessXML: start");
    //check if a process id is saved in the cookies
	var key = patientId + '_' + carepathway;
	var pid = $.cookie(key);
	console.log("getProcessXML: pid: " + pid);
	if  (pid == undefined ) { //if the cookie is not present
		console.log("getProcessXML: create the process");
		//create the process
		$.ajax({
			type: "POST",
			url: "peos.cgi",
			data: "model=" + carepathway + "&event=create",
			processData: false,
			success: function(data) {  
				   loadProcess(data, carepathway, patientId);
				},
			error: function(XMLHttpRequest, textStatus, errorThrown) { 
					alert("getProcessXML \nStatus: " + textStatus + "\nError: " + errorThrown); 
				},
			dataType: "xml"
		});
	} else {
		console.log("getProcessXML: update the process");
		//get an update of the process
		$.ajax({
			type: "POST",
			url: "peos.cgi",
			data: "model=" + carepathway + "&event=update" + "&pid=" + pid,
			processData: false,
			success: function(data) {  
				   loadProcess(data, carepathway, patientId);
				},
			error: function(XMLHttpRequest, textStatus, errorThrown) { 
					alert("getProcessXML \nStatus: " + textStatus + "\nError: " + errorThrown); 
				},
			dataType: "xml"
		});
	}
	
    return false; 
}

/*Load the procress on the page. 
Process are saved in the cookies as patientId_carepathwayName = pid */
function loadProcess(processXML, carepathway, patientId) {
	//console.log(processXML);
	//console.log("loadProcess pid: " + pid);
	console.log("loadProcess-start carepathway: " + carepathway + "\n");
	
	var carepathwayElement = $('#' + carepathway);
	var processUl;
	//if the ul element is not present it creates otherwise clears it
	if (carepathwayElement.children('ul.process').length == 0) {
		processUl = $('<ul></ul>', {"class": "process"}).appendTo(carepathwayElement);
	} else {
		processUl = carepathwayElement.children('ul.process');
		processUl.empty();
	}
	//gets the pid and the process_table from the returned xml
	var pid = $('pid', processXML).text();
	console.log("loadProcess pid: " + pid + "\n");
	var procTable = $('process_table', processXML);
	//loads the process_table for the current pid and parses it
	if (pid >= 0) {
		processElements(procTable.children("process[pid='" + pid + "']"), processUl, pid);	
		//console.log("loadProcess processUl: \n");
		//console.log(processUl.html());
		//console.log("loadProcess carepathwayElement: \n");
		//console.log(carepathwayElement.html());
	}
	
	//if the pid is not already saved in a cookie, it saves it
	var key = patientId + '_' + carepathway;
	var cookiePid = $.cookie(key);
	if  (cookiePid == undefined ) {
		$.cookie(key, pid);
	}
	
	//$('#carepathways ul.process li ul.toggle').hide();
	//carepathwayElement.find('li.action div.actiondetails').hide();
	
	//bind all the element in the pathway so that they display or hide their content	
	carepathwayElement.find('li.expand a').click(function(){
		//$(this).parent().children('ul').toggle();
		var expandLi = $(this).parent();
		var expandUl = expandLi.children('ul');
		if (expandUl.css('display') == 'none') {
			expandLi.children('a.expand').addClass("opened");
			expandLi.children('a.expand').attr('title', 'close');
			expandUl.show();
		} else {
			expandLi.children('a.expand').removeClass("opened");
			expandLi.children('a.expand').attr('title', 'open');
			expandUl.hide();
		}
	});
	
	//bind all actions in the pathway so that they display or hide the action details
	carepathwayElement.find('li.action a.toggle').click(function(){
		//alert("action");
		$(this).parent().children('div.actiondetails').toggle();
	});
	
	carepathwayElement.attr("data-cp-pid", pid);
	
	// Bind a function to the action buttons.
	carepathwayElement.find('.actiondetails input').click(function(){
		//alert("button");
		var event = $(this).attr("data-cp-event").toLowerCase();
		var actionName = $(this).parents('.action').attr("data-cp-action");
		submitEvent(pid, actionName, event, carepathwayElement);
		updateAllProcesses(getPatientId());
		console.log("@clicked action button");
	});
	
	//clicking on a model name will open the corresponding care pathway
	carepathwayElement.children('a').click(function(){
       openCarePathway($(this).parent());
    });
	
	carepathwayElement.removeClass('loading');
	updateCarepathwaysStatus(carepathwayElement);
	console.log("loadProcess-end class: " + carepathwayElement.attr("class") + "\n");
	console.log("loadProcess-end carepathway: " + carepathway + "\n");
	//alert(process.html());
	//console.log("loadProcess html: \n");
	//console.log($('html').html());
}

/*Submites an event to the server, gets the xml and calls updateActionStates*/
function submitEvent(pid, action, event, carepathwayElement) {
	var data = "pid=" + pid + "&event=" + event + "&action=" + action;
		console.log("submitEvent querystring: " + data);
		$.ajax({
			async: false,
			type: "POST",
			url: "peos.cgi",
			data: data,
			processData: false,
			success: function(data) {  
				   updateActionsState(data, carepathwayElement, pid);
				   //finishActions(data, pid, carepathwayElement);
				},
			error: function(XMLHttpRequest, textStatus, errorThrown) { 
					alert("submitEvent \nStatus: " + textStatus + "\nError: " + errorThrown);  
				},
			dataType: "xml"
		});
}

/*Deletes a process*/
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
					alert("deleteProcess \nStatus: " + textStatus + "\nError: " + errorThrown);  
				},
		});
		//alert('deleted');
}

/*Display or hide the pathway support */
function togglePathwaySupport(button) {
	if ($("#supportsystem").css('display') == 'none') { //pathway support is not visible
		console.log("togglePathwaySupport: not visible");
		//check if the models for the current use are loaded; if not, load them
		var patientId = getPatientId();
		console.log($("#carepathways li").length);
		console.log(patientId);
		console.log($("#carepathways").attr("data-cp-patientid"));
		//if the carepathways are not already present, they are reloaded from the server
		if (($("#carepathways li").length < 1) || !($("#carepathways").attr("data-cp-patientid") == patientId)) {
			console.log("togglePathwaySupport: reloading");
			getModelsXML();
		}
		//display pathway support
		$("#patientrecord").addClass("narrow");
		$("#supportsystem").css('display', 'block');
		button.addClass('pressed');
		button.attr('title', "Hide Pathway support");
	} else {
		console.log("togglePathwaySupport: visible");
		//hide pathway support
		$("#patientrecord").removeClass("narrow");
		$("#supportsystem").css('display', 'none');
		button.removeClass('pressed');
		button.attr('title', "Display Pathway support");
	}
		
}

/*Sets the carepathway in a ready state (adds a ready class to it) if any of the action is ready*/
function updateCarepathwaysStatus(carepathwayElement) {
		//console.log("updateCarepathwaysStatus");
		var ready = false;
		carepathwayElement.find('li.action').each(function() {
			//console.log("updateCarepathwaysStatus state: " + $(this).attr("data-cp-state"));
			if ($(this).attr("data-cp-state") == "ready") {
				ready = true;
				//console.log("updateCarepathwaysStatus ready");
			}
		});
		
		if (ready) {
			carepathwayElement.addClass('ready');
		} else {
			carepathwayElement.removeClass('ready');
		}
}

/*Updates the status of the actions for all the carepathway processes associated to a specific patient*/
function updateAllProcesses(patientId) {
    $('#carepathways').children('li.carepathway').each(function() {
		//gets the pid for the current carepathway/patientId from the cookie 
		var carepathway = $(this).attr('name');
		var key = patientId + '_' + carepathway;
		var pid = $.cookie(key);
		console.log("updateAllProcesses: key: " + key);
		console.log("updateAllProcesses: pid: " + pid);
		var carepathwayElement = $('#' + carepathway);
		//if pid is found update the status of all actions
		if  (pid != undefined ) {
			submitEvent(pid, "", "update", carepathwayElement);
			updateCarepathwaysStatus(carepathwayElement);
		}
	});
	
}

/*Issues a finish event on all actions that are in satisfied state (recursively)
and the update the actions state on the page */
function finishActions(data, pid, carepathwayElement) {
	var action = $('process_table', data).find("process[pid='" + pid + "']").find('action[state="SATISFIED"]');
	console.log("finishActions action: " + action);
	if (action != 'undefined') {
		var actionName = action.attr("name");
		submitEvent(pid, actionName, 'finish', carepathwayElement);
	}
	updateActionsState(data, carepathwayElement, pid);	
}

/*Gets the id of the current patient*/
function getPatientId() {
	return $("#patientrecord").attr("data-cp-patientid");
}

/*Removes underscore in a string*/
function convertText(text) {
	return text.replace(/_/g," ")
}

// Wait for the DOM to be loaded. 
$(document).ready(function() { 
	console.log("********* ready ********");
	
	$("#pathwaysupport_btn").click(function(){
		togglePathwaySupport($(this));
	});
	
	//loads the pathway support, if it is visible
	if (!($("#supportsystem").css('display') == 'none')) {
		getModelsXML();
	}
	
	//All cookies and processes when the page in closed or reloaded
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

