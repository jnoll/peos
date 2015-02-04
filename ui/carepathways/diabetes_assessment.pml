process Diabetes_assessment {
	
	action Assess_patient_symptoms {
		requires {patient_symptoms}
		provides {assessment.suspect_diabetes}
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
		action Glucose_test {
			requires {assessment.suspect_diabetes}
			provides {blood_test.glucose_test}
			script {"Fasting blood sugar (FBS) measures blood glucose after you have not eaten for at least 8 hours
						(normal: 70 and100 milligrams per deciliter (mg/dL))."}
			
		}
		
		action Cholesterol_test {
			requires {assessment.suspect_diabetes}
			provides {blood_test.cholesterol_test}
			script {"Diabetes tends to lower HDL cholesterol levels and raise triglyceride and LDL cholesterol levels, which increases the risk for heart disease and stroke.<br>
						Normal values<br>
						Total: below 200 mg/dL<br>
						LDL: below 70 mg/dL<br>
						HDL: 40 mg/dL and above (men)<br>
							50 mg/dL and above (women) <br>
						Triglycerides: below 150 mg/dL<br>"}
		}
	}
	
	action Assess_diabetes {
		requires {blood_test.glucose_test && (optional) blood_test.cholesterol_test}
		provides {diagnosis.diabetes}
		script {"Fasting blood sugar (FBS) glucose level of 100-125mg/dL means impaired fasting glucose, a type of prediabetes. This increases the risk for type 2 diabetes.<br>
				Fasting blood sugar (FBS) glucose level of 126 mg/dL and higher most often means diabetes. <br><br>
				Cholesterol test levels do not influence the diabetes diagnosis but are a risk factor that should be considered and assesed.<br>"}
	}
}