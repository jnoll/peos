process diabetes_assessment {
	
	action assess_patient_symptoms {
		requires {patient_symptoms}
		/*provides {patient_symptoms.diagnosis == "diabetes"}*/
		script {"Diabetes symptoms:<br>
					Increased urination.<br>
					Increased appetite or loss of appetite.<br>
					Excessive thirst.<br>
					Visible weight loss.<br>
					Blurred vision.<br>
					Recurrent skin infections.<br>
					Fatigue.<br>
					Vaginal infections or infections of the foreskin in uncircumcised men.<br>
					Slowly healing sores.<br>"}

	}
	
	branch {
		action glucose_test {
			/*requires {patient_symptoms.suspect_diabetes == "true"}*/
			/*requires {patient_symptoms.diagnosis == "diabetes"}*/
			provides {blood_test.glucose_test}
		}
		
		action cholesterol_test {
			requires {patient_symptoms.suspect_diabetes == "true"}
			/* requires {patient_symptoms.diagnosis == "suspect diabetes"} */
			provides {blood_test.cholesterol_test}
		}	
	}
	
	action assess_diabetes {
		requires {blood_test.glucose_test == "true"}
		/*requires {(optional) blood_test.cholesterol_test}*/
		provides {diagnosis}
	}
}