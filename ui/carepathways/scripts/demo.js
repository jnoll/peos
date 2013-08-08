/*Displays on the page all data about the patient*/
function displayPatientRecord(patient) {
	$("#patientrecord #patientname").text(patient.text());
	var dob = patient.attr("data-cp-dob");
	var age = patient.attr("data-cp-age");
	var gender = patient.attr("data-cp-gender");
	var patientId = patient.attr("data-cp-patientid");
	$("#patientrecord #patientinfo #dob").text("Date of birth: " + dob);
	$("#patientrecord #patientinfo #age").text("Age: " + age);
	$("#patientrecord #patientinfo #gender").text("Gender: " + gender);
	$("#patientrecord").attr("data-cp-patientid", patientId);
	resetPatientRecordDisplayArea();
	updatePatientRecord(patientId, "", "patientrecord");
	$("#patientrecord").show();	
	$("#inputsarea textarea").val('');
	$("#inputsarea input[type='text']").val('');
	$("#inputsarea").show();
}

/*Send a command to the server to update the patient record depending on action and data.
Displays the updated are in the parient record area.
Also updates all the carepathway processes for the specific patient.*/
function updatePatientRecord(patientId, data, action) {
	var postData = "patientid=" + patientId + "&action=" + action;
	
	if (data != "") {
		postData += "&" + data;
	}
	console.log("updatePatientRecord postData:" + postData);
	$.ajax({
		type: "POST",
		url: "demo.php",
		data: postData,
		processData: false,
		success: function(data) {  
				//alert(data);
				$("#patientdata").html(data);
				updateAllProcesses(patientId);
			},
		error: function(XMLHttpRequest, textStatus, errorThrown) { 
				alert("updatePatientRecord \nStatus: " + textStatus + "\nError: " + errorThrown); 
			},
		dataType: "text"
	});
}

/*Implements the action for the input menu area. 
Displays or hide the corresponding div.*/
function inputMenuAction(inputItem) {
	var action = inputItem.attr("data-cp-action");
	if ($("#"+action).css('display') == 'none') {
		$('#patientrecord').addClass("short");
		$('#inputsarea div').hide();
		$('#'+action).show();
	} else {
		resetPatientRecordDisplayArea();
	}		
}

/*Reset the patient record area to the original size */
function resetPatientRecordDisplayArea() {
	$('#patientrecord').removeClass("short");
	$('#inputsarea div').hide();
}

/*Issue to the server command for the various buttons in the demo.*/
function demoAction(button) {
	var action = button.attr("data-cp-action");
	var patientId =$("#patientrecord").attr("data-cp-patientid");
	var data = "";
	switch (action) {
		case "sbmt_symptoms":
			data = "symptoms=" + $("#" + action + " textarea").val();
			break;
		case "req_bloodtest":
			data = "testtype=" + $("#" + action + " input[name='bloodtest']").val();
			break;
		case "sbmt_diagnosis":
			data = "diagnosis=" + $("#" + action + " input[name='diagnosis']").val();
			break;
		case "get_testresult":
			data = "testtype=" + button.attr("data-cp-testtype");
			break;
	}
	
	console.log("demoAction data:" + data);
	
	updatePatientRecord(patientId, data, action);
	
	if (action == "sbmt_symptoms") {
		$("#" + action + " textarea").val('');
	} else {
		$("#" + action + " input[type='text']").val('');
	}
	
	//issue finish event for some actions so that they get to done
	if (action == "get_testresult") {
		var key = getPatientId() + '_' + 'Diabetes_assessment';
		console.log("demoAction get_testresult: key: " + key);
		var pid = $.cookie(key);
		console.log("demoAction get_testresult: pid: " + pid);
		if  (!(pid == undefined)) { //if the cookie is present
			var testtype = button.attr("data-cp-testtype");
			if (testtype == "cholesterol") {
				//finish cholesterol_test action
				console.log("demoAction get_testresult: submitEvent: " + testtype);
				submitEvent(pid, 'Cholesterol_test', 'finish', $('#Diabetes_assessment'));
			} else {
				//finish glucose_test action
				console.log("demoAction get_testresult: submitEvent: " + testtype);
				submitEvent(pid, 'Glucose_test', 'finish', $('#Diabetes_assessment'));
			}	
		}
	}
	
	resetPatientRecordDisplayArea();
}


// Wait for the DOM to be loaded. 
$(document).ready(function() { 
	console.log("********* ready ********");
	
	$("#patientslist li").click(function(){
		displayPatientRecord($(this));
		//loads the pathway support, if it is visible
		if (!($("#supportsystem").css('display') == 'none')) {
			getModelsXML();
		}
	}); 
	
	$("#inputsmenu li").click(function(){
	  inputMenuAction($(this));
	});
	
	$("#demomenu li").click(function(){
	  demoAction($(this));
	});
	
	$("#inputsarea input[type='submit']").click(function(){
	  //inputMenuAction($(this));
	  //alert('submit');
	  demoAction($(this));
	});
	
	//All cookies and processes when the page in closed or reloaded
	$(window).bind('beforeunload', function() {
		var postData = "&action=reset_resourcefile";
		
		$.ajax({
			async: false,
			type: "POST",
			url: "demo.php",
			data: postData,
			processData: false,
			error: function(XMLHttpRequest, textStatus, errorThrown) { 
					alert("reset_resourcefile beforeunload \nStatus: " + textStatus + "\nError: " + errorThrown); 
				},
			dataType: "text"
		});				
	});
}); 