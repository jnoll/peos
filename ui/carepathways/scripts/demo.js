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


function updatePatientRecord(patientId, data, action) {
        console.log("@updatePatientRecord");
	var postData = "patientid=" + patientId + "&action=" + action;
	
	if (data != "") {
		postData += "&" + data;
	}

	console.log("updatePatientRecord: submitting "); // + postData);
	$.ajax({
		type: "POST",
		url: "demo.php",
		data: postData,
		processData: false,
		success: function(data) {  
		                console.log("updatePatientRecord: success");
				$("#patientdata").html(data);
			},
		error: function(XMLHttpRequest, textStatus, errorThrown) { 
				console.log("Status: " + textStatus); console.log("Error: " + errorThrown); 
			},
		dataType: "text"
	});
}

function resetPatientRecordDisplayArea() {
        console.log("@resetPatientDisplayArea");
	$('#patientrecord').removeClass("short");
	$('#inputsarea div').hide();
}

// Handle patient record actions.
function inputMenuAction(inputItem) {
        console.log("@inputMenuAction");
	var action = inputItem.attr("data-cp-action");
	if (action == "update_patient_record") {
	    console.log("update_patient_record: updated");
	} else {
		if ($("#"+action).css('display') == 'none') {
			$('#patientrecord').addClass("short");
			$('#inputsarea div').hide();
			$('#'+action).show();
		} else {
			resetPatientRecordDisplayArea();
		}		
	}
	updateProcessState();
}

// Handle submit from textareas opened by inputMenuActions.
function inputDataAction(button) {
        console.log("@inputDataAction");
	var action = button.attr("data-cp-action");
	var patientId =$("#patientrecord").attr("data-cp-patientid");
	var data = "";
	switch (action) {
	        case "sbmt_symptoms":
		    // Handle "Enter patient symptoms" input form.
		    data = "symptoms=" + $("#" + action + " textarea").val();
		    break;
	        case "req_bloodtest":
		    // Handle "Request blood test" input form.
		    data = "testtype=" + $("#" + action + " input[name='bloodtest']").val();
		    break;
		case "sbmt_diagnosis":
		    // Handle "Enter diagnosis" input form.
		    data = "diagnosis=" + $("#" + action + " input[name='diagnosis']").val();
		    break;
	}
	
 	updatePatientRecord(patientId, data, action);
	
	if (action == "sbmt_symptoms") {
		$("#" + action + " textarea").val('');
	} else {
		$("#" + action + " input[type='text']").val('');
	}
	
	resetPatientRecordDisplayArea();
	updateProcessState();
}

// Handle events from "Demo" menu buttons.
function demoAction(button) {
        console.log("@demoAction");
	var action = button.attr("data-cp-action");
	var patientId =$("#patientrecord").attr("data-cp-patientid");
	var data = "";
	switch (action) {
	        case "get_testresult": 
		    // Respond to "Demo" menu "Get ... test" buttons
			data = "testtype=" + button.attr("data-cp-testtype");;
			break;
	}
	
 	updatePatientRecord(patientId, data, action);
	resetPatientRecordDisplayArea();
	updateProcessState();
}

// Wait for the DOM to be loaded. 
$(document).ready(function() { 
	//console.log("********* ready ********");
	
	$("#patientslist li").click(function(){
	  displayPatientRecord($(this));
	}); 
	
	$("#inputsmenu li").click(function(){
	  inputMenuAction($(this));
	});
	
	$("#demomenu li").click(function(){
	  demoAction($(this));
	});
	
	$("#inputsarea input[type='submit']").click(function(){
	  inputDataAction($(this));
	  //	  demoAction($(this));
	});

	console.log("********* document ready ********");
}); 