//Loads a list of all the pathway (pml files) available on the server
//If not already started, it starts a process for aech pathway associated with the curent user
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

function createAction(action, parent, pid) {
	var actionName = action.attr("name");
	//console.log("@createAction: " + actionName);
	var actionLi = $('<li></li>', {"data-cp-action": actionName, "class": "action"});
	actionLi.attr("data-cp-state", action.attr("state").toLowerCase());
	actionLi.children('a.toggle').attr("title", "Status: " + action.attr("state").toLowerCase());
	//actionLi.text(action.attr("name") + " (" + action.attr("state").toLowerCase() + ")");
	actionLi.append($('<a></a>', {"class": "toggle"}).text(convertText(actionName)));
	//alert(action.attr("name"));
	//actionLi.appendTo(parent);
	parent.append("\n", actionLi);
	var actionDetailsDiv = $('<div></div>', {"class": "actiondetails"});
	actionLi.append("  \n", actionDetailsDiv);
	var actionDetailsDl = $('<dl></dl>');
	actionDetailsDiv.append("    \n", actionDetailsDl);
	
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
	if (action.children("script").text().search("null") < 0) {
		var scriptText = action.children("script").text();
		scriptText = scriptText.substring(2, scriptText.length-2)
		actionDetailsDiv.append("\n", $('<p></p>').html(scriptText));
	}
	
	var actionButtonsDiv = $('<div></div>', {"class": "actionButtons"});
	actionDetailsDiv.append("    \n", actionButtonsDiv);
	var buttons = ["Start", "Finish", "Cancel"];
	var events = ["start", "finish", "abort"];
	
	for (i in buttons) {
		actionButtonsDiv.append($('<input />', {"type": "button", "value": buttons[i], "data-cp-event": events[i]}));	
	}

	return;
}

function updateActionsState(data, carepathwayElement, pid) {
	console.log("@updateActionsState");
	$('process_table', data).find("process[pid='" + pid + "']").find('action').each(function() {
		var state = $(this).attr("state");
		var actionName = $(this).attr("name");
		console.log("state: " + state + " - action: " + actionName);
		var actionLi = carepathwayElement.find('.action[data-cp-action=' + actionName + ']');
		actionLi.attr("data-cp-state", state.toLowerCase());
		actionLi.children('a.toggle').attr("title", "Status: " + state.toLowerCase());
	});
	//console.log(carepathwayElement.html());
}

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

//Display or hide the care pathway on the page i.e. all the details of a process.
//If the process if not already loaded in the page, it gets it from the server and loads it 
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
	$('#carepathways li.action[data-cp-state=ready]').parents('ul.toggle').show();
	$('#carepathways li.action[data-cp-state=available]').parents('ul.toggle').show();
	$('#carepathways li.action[data-cp-state=satisfied]').parents('ul.toggle').show();
	$('#carepathways li.action[data-cp-state=run]').parents('ul.toggle').show();
	
	//console.log("openCarePathway html: \n");
	//console.log($('html').html());
}

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
					alert("Status: " + textStatus); alert("Error: " + errorThrown); 
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
					alert("Status: " + textStatus); alert("Error: " + errorThrown); 
				},
			dataType: "xml"
		});
	}
	
    return false; 
}

//Load the procress on the page. 
//Process are saved in the cookies as patientId_carepathwayName = pid
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
	
	//bind all action in the pathway so that they display or hide the action details
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
		var data = "pid=" + pid + "&action=" + actionName + "&event=" + event;
		console.log("query string: " + data);
		$.ajax({
			async: false,
			type: "POST",
			url: "peos.cgi",
			data: data,
			processData: false,
			success: function(data) {  
				   updateActionsState(data, carepathwayElement, pid);
				   updateAllProcesses(data, getPatientId());
				},
			error: function(XMLHttpRequest, textStatus, errorThrown) { 
					alert("Status: " + textStatus); alert("Error: " + errorThrown); 
				},
			dataType: "xml"
		});
		console.log("@clicked action button");
	});
	
	//clicking on a model name will open the corresponding care pathway
	carepathwayElement.children('a').click(function(){
       openCarePathway($(this).parent());
    });
	
	carepathwayElement.removeClass('loading');
	console.log("loadProcess-end carepathway: " + carepathway + "\n");
	//alert(process.html());
	//console.log("loadProcess html: \n");
	//console.log($('html').html());
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

function toggleDecisionSupport(button) {
	if ($("#supportsystem").css('display') == 'none') { //decision support is not visible
		console.log("toggleDecisionSupport: not visible");
		//check if the models for the current use are loaded; if not, load them
		var patientId = getPatientId();
		console.log($("#carepathways li").length);
		console.log(patientId);
		console.log($("#carepathways").attr("data-cp-patientid"));
		if (($("#carepathways li").length < 1) || !($("#carepathways").attr("data-cp-patientid") == patientId)) {
			console.log("toggleDecisionSupport: reloading");
			getModelsXML();
		}
		//display decision support
		$("#patientrecord").addClass("narrow");
		$("#supportsystem").css('display', 'block');
		button.addClass('pressed');
		button.attr('title', "Hide Decision support");
	} else {
		console.log("toggleDecisionSupport: visible");
		//hide decision support
		$("#patientrecord").removeClass("narrow");
		$("#supportsystem").css('display', 'none');
		button.removeClass('pressed');
		button.attr('title', "Display Decision support");
	}
		
}

function updateAllProcesses(patientId) {
    $('#carepathways').children('li.carepathway').each(function() {
		//gets the pid for the current carepathway/patientId from the cookie 
		var carepathway = $(this).attr('name');
		var key = patientId + '_' + carepathway;
		var pid = $.cookie(key);
		console.log("updateAllProcesses: pid: " + pid);
		var carepathwayElement = $('#' + carepathway);
		//if pid is found update the status of all actions
		if  (pid != undefined ) {
			var data = "pid=" + pid + "&event=update";
			//console.log("query string: " + data);
			$.ajax({
				type: "POST",
				url: "peos.cgi",
				data: data,
				processData: false,
				success: function(data) {  
					   updateActionsState(data, carepathwayElement, pid);
					},
				error: function(XMLHttpRequest, textStatus, errorThrown) { 
						alert("Status: " + textStatus); alert("Error: " + errorThrown); 
					},
				dataType: "xml"
			});
		}
	});
	
}

function getPatientId() {
	return $("#patientrecord").attr("data-cp-patientid");
}

function convertText(text) {
	return text.replace(/_/g," ")
}

// Wait for the DOM to be loaded. 
$(document).ready(function() { 
	console.log("********* ready ********");
	
	$("#decisionsupport_btn").click(function(){
		toggleDecisionSupport($(this));
	});
	
	//loads the decision support, if it is visible
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

