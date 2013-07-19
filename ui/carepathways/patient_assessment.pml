process diabetes_assessment {
	
	action get_patient_symptoms {
		requires {patient_record}
		provides {patient_symptoms}
	}
	
	action assess_patient_symptoms {
		requires {patient_symptoms}
		provides {patient_symptoms.diagnosis}
	}
}