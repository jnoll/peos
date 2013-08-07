<?php 
$action = $_POST["action"];
$patientId = $_POST["patientid"];

switch ($action) {
	case "patientrecord":
		updateResourceFiles($patientId);
		loadHl7File($patientId);
		break;
	case "sbmt_symptoms":
		addSymptoms($patientId, $_POST["symptoms"]);
		break;
	case "req_bloodtest":
		requestBloodTest($patientId, $_POST["testtype"]);
		break;
	case "sbmt_diagnosis":
		addDiagnosis($patientId, $_POST["diagnosis"]);
		break;
	case "get_testresult":
		returnBloodTest($patientId, $_POST["testtype"]);
		break;
	case "reset":
		resetPatientRecord($patientId);
		break;
	case "reset_resourcefile":
		updateResourceFiles("");
		break;		
} 

returnPatientRecord($patientId);

?>

<?php 
/*Return the contents of the patient record file.
The patient record is a .rec file that contains html data with information about the patient
to be displayed on the page.*/
function returnPatientRecord($patientId) {
	$patientRecord = $patientId.".rec";
	//echo $patientRecord;
	if (file_exists($patientRecord)) {
		$patientFile = fopen($patientRecord,"r");
		while(!feof($patientFile)) {
			echo fgets($patientFile);
		}
		fclose($patientFile);
	} else {
		echo "";
	}
}

/*Deletes all the file related to a patient */
function resetPatientRecord($patientId) {
	if (file_exists($patientId.".rec")) {
		unlink($patientId.".rec");
	}
	if (file_exists($patientId.".spt")) {
		unlink($patientId.".spt");
	}
	if (file_exists($patientId."._hl7")) {
		copy($patientId."._hl7", $patientId.".hl7");
	}
}

function addSymptoms($patientId, $symptoms) {
	file_put_contents($patientId.".spt", $symptoms, FILE_APPEND);
	$data = "<dt>Symptoms:</dt>\n";
	$data = $data."<dd>".nl2br($symptoms)."</dd>\n";
	//$data = $data."<dd>".$symptoms."</dd>\n";
	file_put_contents($patientId.".rec", $data, FILE_APPEND);
}

function requestBloodTest($patientId, $testtype) {
	$data = "<dt>Requested blood test:</dt>\n";
	$data = $data."<dd>".$testtype."</dd>\n";
	file_put_contents($patientId.".rec", $data, FILE_APPEND);
}

function returnBloodTest($patientId, $testtype) {
	//error_log("returnBloodTest patientid: ".$patientId);
	if ($patientId == "001677980") {
		$data = "";	
		$hl7data = "";
		switch ($testtype) {
			case "cholesterol":
				$data = "<dt>Cholesterol blood test:</dt>\n";
				$data .= "<dd>CHOLESTEROL 138.000 MG/DL (100.000 - 200.000 MG/DL) Normal<br>\n";
				$data .= "LDL  (CALCULATED) 49.000 MG/DL (0.000 - 100.000 MG/DL) Normal<br>\n";
				$data .= "HDL 24.000 MG/DL (45.000 - 150.000 MG/DL) Low<br>\n";
				$data .= "TRIGLYCERIDES 324.000 MG/DL (0.000 - 150.000 MG/DL) High</dd>\n";
				$hl7data = "OBR|1|||80061^LIPID PROFILE^CPT-4||20070911||||||||||\n";
				$hl7data .= "OBX|1|NM|13457-7^LDL (CALCULATED)^LOINC|49.000|MG/DL| 0.000 - 100.000|N|||F|\n";
				$hl7data .= "OBX|2|NM|2093-3^CHOLESTEROL^LOINC|138.000|MG/DL|100.000 - 200.000|N|||F|\n";
				$hl7data .= "OBX|3|NM|2086-7^HDL^LOINC|24.000|MG/DL|45.000 - 150.000|L|||F|\n";
				$hl7data .= "OBX|4|NM|2571-8^TRIGLYCERIDES^LOINC|324.000|MG/DL| 0.000 - 150.000|H|||F|\n";
				break;
			case "glucose":
				$data = "<dt>Glucose blood test:</dt>\n";
				$data .= "<dd>Glucose Lvl 140 MG/DL (65 - 99 MG/DL) High</dd>\n";
				$hl7data = "OBR|2|341856649^HNAM_ORDERID|000002006326002362|648088^Basic Metabolic Panel|||20061122151600|||||||||1620^Hooker^Robert^L||||||20061122154733|||F|||||||||||20061122140000|\n";
				$hl7data .= "OBX|5|NM|GLU^Glucose Lvl|140|mg/dL|65-99^65^99|H|||F|||20061122154733|\n";
				break;
		}
		
		file_put_contents($patientId.".rec", $data, FILE_APPEND);
		file_put_contents($patientId.".hl7", $hl7data, FILE_APPEND);
	}

}

function addDiagnosis($patientId, $diagnosis) {
	$diagnosis = trim($diagnosis);
	$data = "<dt>Diagnosis:</dt>\n";
	$data = $data."<dd>".$diagnosis."</dd>\n";
	file_put_contents($patientId.".rec", $data, FILE_APPEND);
	$diagnosis = strtolower($diagnosis);
	if ($diagnosis == "suspect diabetes") {
		$data = "DG1|1||5409^SUSPECT DIABETES^I9CDX|||W\n";
	} elseif ($diagnosis == "diabetes") {
		$data = "DG1|2||5409^DIABETES^I9CDX|||W\n";
	}
	file_put_contents($patientId.".hl7", $data, FILE_APPEND);
}

//create a resource file where every reference to patientId is replaced by the actual patient id
function updateResourceFiles($patientId) {
	//echo $patientId;
	$resourceFileNames = glob("*._res");
	foreach ($resourceFileNames as $resourceFileName) {
		$resourceFile = fopen($resourceFileName,"r");
		$newResourceFileName = str_replace("._res", ".res", $resourceFileName);
		unlink($newResourceFileName);
		$newResourceFile= fopen($newResourceFileName,"a");
		while(!feof($resourceFile)) {
			$line = fgets($resourceFile);
			//echo $line;
			$line = str_replace("patientId", $patientId, $line);
			//echo $line;
			fwrite($newResourceFile, $line);
		}
		fclose($resourceFile);
		fclose($newResourceFile);
	}
}

function loadHl7File($patientId) {
	if (!file_exists($patientId.".hl7")) {
		if (file_exists($patientId."._hl7")) {
			copy($patientId."._hl7", $patientId.".hl7");
		}
	}
}
?>