process Dementia_Management {
  action identify_causal_or_exacerbating_factors { 
  }
  action provide_patient_carer_with_information {
    agent {GP && patient && carer}
  }	
  action create_treatment_or_care_plan {
    agent {memory_assessment_service} 
    agent {GP && clinical_psychologiest && nurses && occupational_therapists && phsiotherapists && speech_and_language_therapists && social_workers && voluntary_organisation}
  }
  action formal_review_of_care_plan {
    agent {person && carer}
  }
  action assess_carer_needs { 
    agent { carer}
  }
  branch  {
    branch cognitive_symptom_mgt {
      action non_medication_interventions {
      }
      action medication_interventions {
	agent {specialist}
	agent {carer}
      }
    } /* end of management_of_cognitive_symptoms  */
    branch non_cognitive_symptom_mgt {
      action non_medication_interventions { 
	agent {carer && patient}
      }
      action medication_interventions {
      }
    } /* end of management_of_non_cognitive_symptoms */
  } /* end cognitive/non-cognitive symptoms branch */
  action obtain_carers_view {
    agent {carer}
  }
  action document_symptoms {
    agent {patient}
  }
  /* optional, if required */
  action assess_behavioural_disturbances {
    agent {patient}
  }
  branch {
    iteration {
      action record_changes {
	agent {patient}
      }
    }
    iteration  {
      action monitor_patients_on_medication{
	agent {patient }
      }
      action consider_alt_assessments {
      }
    }
    iteration {
      action manage_comorbidity {
	/*requires { }*/
      }
      action psychosocial_interventions {
	agent {carer}
      }
    }
  } /* branch */
} /* process */
