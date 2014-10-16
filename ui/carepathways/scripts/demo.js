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
        console.log("@updatePatientRecord: action=" + action + " data=" + data);
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
	    $("#"+action).dialog("open");
	}
	updateProcessState();
}

// Handle slide button press
function slideButtonAction(inputItem) {
        console.log("@inputMenuAction");
	var slide = inputItem.attr("data-cp-slide");
	$("#slide"+slide).dialog("open");
}

// Handle submit from textareas opened by inputMenuActions.
function inputDataAction(action) {
        console.log("@inputDataAction: " + action);
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
    	        case "reset":
		        deleteProcesses();
		        break;
	}
	
 	updatePatientRecord(patientId, data, action);
	resetPatientRecordDisplayArea();
	updateProcessState();
}


function createInputDialog(action) {
    console.log("action=" + action + " i=" + i);
    $("#"+action).dialog({
	autoOpen: false,
	height: "auto",
	width: 350,
	modal: true,
	buttons: {
		"OK": function() { 
		    console.log("OK: " + action);
		    $("#"+action).dialog( "close" );
		    inputDataAction(action);
		},
		    Cancel: function() {
		    $("#"+action).dialog( "close" );
		}
	    },
	close: function() {
		//$("#"+action).find("form").reset();
	    }
	}); 
}


function createSlidePopup(image, ok) {
    console.log("image=" + image + " i=" + i);
    $("#slide"+image).dialog({
	autoOpen: false,
	height: "auto",
	width: 650,
	modal: true,
	buttons: {
	    Ok: ok
	    }
	}); 
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
	
	$("#demomenu a").click(function(){
	  demoAction($(this));
	});

	var actions = ["sbmt_symptoms", "req_bloodtest", "sbmt_diagnosis"];
	for (i in actions) {
	    var action = actions[i];
	    createInputDialog(action);
	}
	$("#inputsarea input[type='submit']").on("submit", function(e){
	  e.preventDefault();
	  inputDataAction($(this.attr("demo-cp-action")));
	});
	
	var images = ["1", "2", "4", "6", "8", "10", "12", "14", "15", "17", "19", "21"];
	for (i in images) {
	    var image = images[i];
	    if (i == 8) {
		createSlidePopup(image, function() {
		    $( this ).dialog( "close" );
		    var patientId =$("#patientrecord").attr("data-cp-patientid");
		    var data = "testtype=cholesterol";
		    updatePatientRecord(patientId, data, "get_testresult");
		});
	    } else if (i == 9) {
		createSlidePopup(image, function() {
		    $( this ).dialog( "close" );
		    var patientId =$("#patientrecord").attr("data-cp-patientid");
		    var data = "testtype=glucose";
		    updatePatientRecord(patientId, data, "get_testresult");
		});
	    } else {
		createSlidePopup(image, function() {
		    $( this ).dialog( "close" );
		});
	    }
	}

	$("#slides a").click(function(){
		var slide = $(this).attr("data-cp-slide");
		$("#slide"+slide).dialog("open");
	});

	console.log("********* document ready ********");
}); 