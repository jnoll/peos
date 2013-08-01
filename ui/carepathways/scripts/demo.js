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
	var postData = "patientid=" + patientId + "&action=" + action;
	
	if (data != "") {
		postData += "&" + data;
	}
	//alert(postData);
	$.ajax({
		type: "POST",
		url: "demo.php",
		data: postData,
		processData: false,
		success: function(data) {  
				//alert(data);
				$("#patientdata").html(data);
			},
		error: function(XMLHttpRequest, textStatus, errorThrown) { 
				alert("Status: " + textStatus); alert("Error: " + errorThrown); 
			},
		dataType: "text"
	});
}

function inputMenuAction(inputItem) {
	var action = inputItem.attr("data-cp-action");
	if (action == "update_patient_record") {
		alert("update_patient_record");
	} else {
		if ($("#"+action).css('display') == 'none') {
			$('#patientrecord').addClass("short");
			$('#inputsarea div').hide();
			$('#'+action).show();
		} else {
			resetPatientRecordDisplayArea();
		}		
	}
}

function resetPatientRecordDisplayArea() {
	$('#patientrecord').removeClass("short");
	$('#inputsarea div').hide();
}

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
			data = "testtype=" + button.attr("data-cp-testtype");;
			break;
	}
	
	updatePatientRecord(patientId, data, action);
	
	if (action == "sbmt_symptoms") {
		$("#" + action + " textarea").val('');
	} else {
		$("#" + action + " input[type='text']").val('');
	}
	
	resetPatientRecordDisplayArea();
}

// Wait for the DOM to be loaded. 
$(document).ready(function() { 
	console.log("********* ready ********");
	
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
	  //inputMenuAction($(this));
	  //alert('submit');
	  demoAction($(this));
	});
}); 