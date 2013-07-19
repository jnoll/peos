process Dementia_Management {

	action identify_causal_or_exacerbating_factors {
		requires { Guidelines_For_Treatment_Of_Patients }
	}
	
	action provide_patient_carer_with_information {
		role{GP && patient && carer}
		
		requires {patient_record.Confirmed_Dementia  }
		requires {patient_record.requests_privacy == "false" }
		
		provides { information_to_carer }
	}	
	
	action create_treatment_or_care_plan{
		role{memory_assessment_service}
		role{GP && clinical_psychologiest && nurses && occupational_therapists && phsiotherapists && speech_and_language_therapists && social_workers && voluntary_organisation}
		
		requires { patient_history }
		
		provides { care_plan }
	}
	
	action formal_review_of_care_plan {
		agent { person && carer }
		
		requires { care_plan }
		
		provides { care_plan.reviewed == "true" }
	}
	
	action assess_carer_needs {
		agent { carer }
		
		provides { care_plan.respite_care }
	}
	
	branch {
	
		branch cognitive_symptoms_mgt {
			action non_medication_intervensions {
				provides { support_for_carer }
				provides { info_about_sevices_nad_intervensions }
				provides { (optional) cognitive_simulation
			}
			
			action medication_intervensions {
				agent { specialist }
				agent { carer }
				
				requires { (intangible) carer_view_on_patient_condition }
				
				provides { prescription }
			}
		}
		
		branch non_cognitive_symptoms_mgt {
			action non_medication_intervensions {
				agent (carer && patient }
				
				requires { (nontangible) non_cognitive_symptoms || (nontangible) challenging_behaviour }
				
				provides { early assessment }
			}
			
			action medication_intervensions {
				requires { (intangible) risk_of_harm_or_distress }
				
				provides { medication }
			}
		}
	}
	
	action obtain_carer_view {
		agent { carer }
		
		provides { (intangible) view_on_condition }
	}
	
	action document_symptoms {
		agent { patient }
		
		provides { patient_record.symptoms }
	}
	
	action assess_behavioural_disturbances {
		agent { patient }
	
		requires { (intangible) risk_of_harm_or_distress }
		
		provides { care_plan.appropriate_setting }
	}
	
	branch {
		iteration {
			action record_changes {
				agent { patient }
				
				provides { patient_record.symptoms }
				provides { (optional) medication }
			}
		}
		
		iteration {
			action monitor_patients_on_medication {
				agent { patient }
				
				provides { (optional) care_plan.medication }
			}
			
			action consider_alt_assessments {
				requires { patient_record.disability || patient_record.sensory_imparement || patient_record.linguistic_problems || patient_record.speech_problems }
				
				provides { care_plan.alternative_assessment_method }
			}
		}
		
		iteration {
			action manage_comorbility {
				provides { comorbility.depression }
				provides { comorbility.psychosis }
				provides { comorbility.delirium }
				provides { comorbility.parkinsons_disease }
				provides { comorbility.stroke }
			}
			
			action psychosocial_intervensions {
				agent { carer }
				
				requires { comorbility.depression || comorbility.anxiety }
			}
		}
	}
}