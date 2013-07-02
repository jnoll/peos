process Dementia_Assessment {
	/*
	4
	*/
	action clinical_presentation {
		agent{GP}
		requires {patient_record}
		provides {patient_record.Suspected_Dementia}
		script{ "Suspect dementia when any of the following are reported [2]: cognitive symptoms, eg:	memory problems, eg: forgetfulness repetitive questioning difficulty recalling names and other words not knowing common facts communication problems (spoken or written) disorientation (time, place, person) challenging behaviours, psychiatric symptoms, and personality changes, eg: withdrawal or apathy depression anxiety blunting of emotions and disinterest aggression social withdrawal disinhibition, inappropriate friendliness or flirtatiousness suspiciousness, fearfulness, or paranoid beliefs insomnia restlessness or wandering psychotic symptoms, including delusions/abnormal beliefs or hallucinations neurological symptoms, eg: gait disturbances apraxia (loss of ability to perform learned purposeful movements) difficulties with activities of daily life, eg: getting lost taking prescribed medications erratically forgetting recipes when cooking neglecting household chores trouble with shopping difficulty handling money loss of driving skills neglecting hygiene or self-care - look for deterioration in personal appearance making mistakes at work  Reference:	[2] Clinical Knowledge Summaries (CKS). Dementia. Version 1.0. Newcastle upon Tyne: CKS; 2010."
		}
	}
	
	/*
	5
	*/
	action History_And_Examination {
		agent{GP}
		agent{patient && carer || family_member}
		requires {patient_record}
		provides {patient_record.Suspected_Dementia }
		script{ "Ideally ask the patient questions and then talk, together or separately, to a family member or carer who is able to provide additional information. History: • ask about: • problems with: • memory, • orientation, eg confusional state • speech and language • performing key roles and activities • alcohol consumption and history • incontinence • assess onset and progression of symptoms, eg: • when were they first noticed? • was onset sudden or gradual (over several months/years)? • are symptoms worse at night? • is there associated drowsiness, impairment of consciousness? • was onset linked to a head injury, blackout, or stroke? • consider: • non-cognitive manifestations, eg: • behavioural or personality changes • co-morbid psychiatric symptoms, eg depression, psychosis • disinhibition • feelings of restlessness, agitation • aggressive behaviour • patterns of non-cognitive manifestations - try to identify triggers and exacerbating factors • co-morbidities, particularly those which may impact on cognition function • medications, particularly those with significant anticholinergic side-effects, eg: • amitriptyline • antihistamines • antipsychotic medications • check for history of: • goitre, slow pulse, dry skin, hypothyroidism • sexually-transmitted infection (STI) or HIV • cardiovascular disease  • poor dietary intake, malnutrition, anaemia Examination should include: • clinical cognitive assessment to examine: • attention/concentration • orientation to: • time - ask about time of day, day of week, season, year • place - ask them where they are, where is the nearest shop to their house • memory - assess by asking them to repeat 3 common words immediately, then again 3-5 minutes later • language skills - ask them to name parts of the body and explain the function of common items (eg what would you do with a hammer?)  if speech/language difficulties make patient difficult to assess, refer to speech and language therapist for assessment • comprehensive assessment of: • risk of harm to self, eg: • self-harm • suicide • neglect • risk of falls - see 'Falls' care map • risk of harm to other, eg: • abuse • violence • formal cognitive testing using a standardised instrument, eg: • Mini Mental State Examination (MMSE): • maximum score on MMSE is 30 • mild Alzheimer's disease (AD) - 21-26 • moderate AD - 10-20 • severe AD - less than 10 • people with: • early AD fail mainly in orientation and memory tasks • frontotemporal dementia (FTD) exhibit early speech impairment • dementia with Lewy bodies (DLB)may be affected visuospatially • 6-item Cognitive Impairment Test (6-CIT) • General Practitioner Assessment of Cognition (GPCOG) • 7-Minute Screen • abbreviated mental test score • when interpreting test scores, take into account: • physical, sensory, or learning disabilities - may invalidate results • language background - test should be in patient's first language • educational level - standard cut-off scores should be increased in highly educated people • prior level of functioning and attainment • communication difficulties that make test difficult • hearing and vision - ensure patient is using reading glasses if needed • psychiatric illness  • physical/neurological problems • if vascular dementia is suspected, look for other signs of vascular disease, eg: • check peripheral pulse  • listen for carotid bruit • consider performing a carotid doppler scan  • also consider: • other standardised testing/rating scales, including those for mood, memory and cognitive functioning, eg Addenbrookes Cognitive Examination • directly observing behaviour, eg watch patient imitating putting on a shirt, waving goodbye, brushing teeth etc to test for apraxia This information was drawn from the following references: [1] American Psychiatric Association (APA). Practice guideline for the treatment of patients with Alzheimer's disease and other dementias of late life. Arlington, VA: APA; 2007. [2] Clinical Knowledge Summaries (CKS). Dementia. Version 1.0. Newcastle upon Tyne: CKS; 2010. [3] Scottish Intercollegiate Guidelines Network (SIGN). Management of patients with dementia. SIGN publication no 86. Edinburgh: SIGN; 2006. [5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in health and social care. Clinical guideline 42. London: NICE; 2006. [8] National Institute for Health and Clinical Excellence (NICE). Donepezil, galantamine, rivastigmine (review) and memantine for the treatment of Alzheimer's disease (amended). Technology appraisal 111. London: NICE; 2007. [9] World Health Organization (WHO). Abbreviated mental test score: screen for dementia. Geneva: WHO; 2004. [12] World Health Organization (WHO). mhGAP intervention guide for mental, neurological and substance use disorders in nonspecialized health settings. Geneva: WHO; 2010. [20] National Institute for Health and Clinical Excellence (NICE). Donepezil, galantamine, rivastigmine and memantine for the treatment of Alzheimer's disease. Review of NICE technology appraisal guidance 111. Technology appraisal 217. London: NICE; 2011. "
		}
		
	}
	/*
	6
	*/
	action Initial_investigations{
		agent{GP}	
		requires {patient_record}
		provides {patient_record.Suspected_Dementia}	
		script { "Quick info: Perform a basic dementia screen at the time of presentation, including [1,2,5]:  • full blood count (FBC) • urea, electrolytes and creatinine • glucose • liver function tests • calcium • serum vitamin B12 and folate levels • thyroid function tests Also perform [2,5]: • midstream urine test if considering acute confusional state • chest X-ray or electrocardiogram (ECG) as guided by clinical presentation Screen for co-morbidities, such as: • depression [1,3,5] • psychosis [3,5] • delirium [3,5] • Parkinson's disease [5] • stroke [3,5] • syphilis, Borrelia, or HIV – not routinely indicated, only perform if there is clinical suspicion or high risk [5,11] References: [1] American Psychiatric Association (APA). Practice guideline for the treatment of patients with Alzheimer's disease and other dementias of late life. Arlington, VA: APA; 2007. [2] Clinical Knowledge Summaries (CKS). Dementia. Version 1.0. Newcastle upon Tyne: CKS; 2010. [3] Scottish Intercollegiate Guidelines Network (SIGN). Management of patients with dementia. SIGN publication no 86. Edinburgh: SIGN; 2006. [5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in health and social care. Clinical guideline 42. London: NICE; 2006. [11] Hort J, O'Brien JT, Gainotti G et al. EFNS guidelines for the diagnosis and management of Alzheimers disease. European Journal of Neurology 2010; 17: 1236-48."
		}
	}
		/*
	7
	*/
	action Red_Flag {
		agent{GP}
		requires {patient_record}
		provides {patient_record.delirium || patient_record.unusual_features}
		script{"Refer to a specialist if [12]: • delirium is suspected due to presence of the following features: • symptom onset is abrupt and duration is short (days/weeks) • disturbance is more at night and is associated with impairment of consciousness • there is disorientation to time/place • unusual features are present, eg: • onset before the age of 60 years • clinical hypothyroidism • cardiovascular disease • history of: sexually transmitted infection (STI) or HIV • head injury or stroke Reference: [12] World Health Organisation (WHO). mhGAP intervention guide for mental, neurological and substance use disorders in nonspecialized health settings. Geneva: WHO; 2010."
		}
	}
	
	/*
	8
	*/
	action Refer_to_specialist_care {
		agent{GP}
		requires {patient_record.delirium || patient_record.unusual_features}
		provides {patient_record.Specialist_Care}
		script{ ""}
	}
	/*
	9
	*/
	action Confirm_diagnosis_and_discuss_with_patientORcarer {
		agent{GP}
		agent{carer || family_member}
		requires {patient_record.Suspected_Dementia}
		provides{patient_record.Confirmed_Dementia}
		provides{Dementia_Confirmation_Letter}
		script{"Quick info: Dementia is diagnosed when there are cognitive or behavioural symptoms that [17]: • interfere with the ability to function at work or at usual activities; and • represent a decline from previous levels of functioning and performing; and • are not explained by delirium or major psychiatric disorder Cognitive impairment is detected and diagnosed through a combination of [17]: • full medical and psychiatric history provided by the patient and carer; and • an objective cognitive assessment, either a “bedside” mental status examination or neuropsychological testing Cognitive or behavioral impairment involves a minimum of two of the following domains [17]: • impaired ability to acquire and remember new information - symptoms include: • repetitive questions or conversations • misplacing personal belongings • forgetting events or appointments • getting lost on a familiar route • impaired reasoning and handling of complex tasks, or poor judgment - symptoms include: • poor understanding of safety risks • inability to manage finances • poor decision-making ability • inability to plan complex or sequential activities • impaired visuospatial abilities - symptoms include: • inability to recognize faces or common objects or to find objects in direct view despite good acuity • inability to operate simple implements, or orient clothing to the body • impaired language functions (speaking, reading, writing) - symptoms include: • difficulty thinking of common words while speaking, hesitations • speech, spelling, and writing errors • changes in personality, behavior, or comportment - symptoms include: • uncharacteristic mood fluctuations such as: • agitation • impaired motivation • initiative • apathy • loss of drive • social withdrawal • decreased interest in previous activities • loss of empathy • compulsive or obsessive behaviors • socially unacceptable behaviors Disclosure of diagnosis [3]: • diagnosis should be given by a healthcare professional skilled in communication or counselling • ask patient if they wish to know the diagnosis and with whom it should be shared [5] • most people with dementia can understand their diagnosis, receive information and be involved in decision making - disclosing diagnosis may actually decrease depression and anxiety in these people [21] • however, in some situations, disclosure of a diagnosis may be inappropriate: • some people may not wish to know their diagnosis - the wishes of the patient should be upheld at all times • cultural, ethnic and religious differences may influence disclosure • where diagnosis is not disclosed there should be a clear record of the reasons NB: Be aware that Alzheimer's disease is often difficult to diagnose in the early stages due to an insidious onset [5]. References: [3] Scottish Intercollegiate Guidelines Network (SIGN). Management of patients with dementia. SIGN publication no 86. Edinburgh: SIGN; 2006. [5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in health and social care. Clinical guideline 42. London: NICE; 2006. [17] McKhamm GM, Knopman DS, Chertkow H et al. The diagnosis of dementia due to Alzheimer’s disease: Recommendations from the National Institute on Aging-Alzheimer’s Association workgroups on diagnostic guidelines for Alzheimer’s disease. Alzheimers & Dementia 2011; 7: 263-9.
[21] Hort J O'Brien JT, Gainotti G et al. EFNS guidelines for the diagnosis and management of Alzheimers disease. European Journal of Neurology 2010; 17: 1248."
		}
	}
	/*
	10
	*/
	action Principles_of_coordinated_care {
		agent{care_manager}
		agent{carer && patient}
		requires{patient_record}
		requires{patient_consent}
		provides{Info_on_services_and_interventions}
		provides{care_plan}
		script{"Quick info:Patients and carers should be provided with information about the services and interventions available to them at all stages of thepatient’s journey of care, including:• social support [20]• community dementia teams [20]• day centres/services and residential care (mainstream and specialist) [5,20]• home nursing and personal care [20]• assistive technology and telecare [5]• community services such as meals-on-wheels [20]• befriending services [20]• specialist mental health services [5]Care managers/co-ordinators should ensure that:• a combined care plan is agreed by health and social services, that takes into account [5]:• life history, social and family circumstances, and preferences• physical and mental health needs and current level of functioning• changing needs of the person with dementia and carers• the plan is endorsed by the person and/or carers [5]• named health and social care staff are charged with operating the care plan [5]• formal reviews of the plan take place at a frequency agreed with the person and carers, and recorded in the notes [5]• arrangements are in place to facilitate referral to specialist services if particular problems arise, eg [10]:• speech and language therapist (SLT) for communication problems• physiotherapist for mobility problemsPatient consent [5]:• the patient should have the opportunity to make informed choices about care and treatment• always seek valid consent from patients - explain options and check that the person understands, has not been coerced andcontinues to consent over time• abide by the 5 key principles of the Mental Capacity Act 2005:• assume all adults to have the capacity to make decisions for themselves unless proven otherwise• provide individuals with all possible support before concluding that they cannot make decisions for themselves• individuals must retain the right to make what might be seen as eccentric or unwise decisions• when the individual lacks mental capacity, always act in their best interests• any action taken for or on behalf of an individual without capacity must be the least restrictive alternative in terms of theirrights and basic freedoms• only in exceptional circumstances should confidential information be disclosed to others, including all otherprofessionals, without the patient's consent• when the patient is not able to give informed consent, any sharing of information should be within the Code of Practice of theMental Capacity Act 2005, and be in the patient's best interests• while the patient is still at an early stage of their illness, discuss with the patient and their family/carer:• advance statements• advance decisions to refuse treatment• Lasting Power of Attorney (LPA)• a Preferred Place of Care Plan, concerning issues such as end of life careDiversity and equality:• always treat people with dementia and their carers with respect [5]• ensure people with dementia are not excluded from services because of:• their diagnosis• age (whether regarded as too old or too young)• any learning disability• if English is a second language, offer [5]:• written information in the preferred language and/or an accessible format• independent interpreters• psychological interventions in the preferred language• if the person has language or communication difficulties arising from the dementia, ensure that information is accessible andthat other interventions take account of these issues - consider seeking the advice of an SLT on the most effective way toachieve communication [10]• ensure that people suspected of having dementia because of cognitive and functional deterioration, but who do not havesufficient memory impairment for diagnosis, are not denied access to support services [5]Impact of dementia on relationships [5]:• assess the impact of dementia on personal (including sexual) relationships at the time of diagnosis and when indicatedsubsequently• provide information about local support services if requiredReferences:[5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in healthand social care. Clinical guideline 42. London: NICE; 2006.[10] Map of Medicine (MoM) Clinical Editorial team and Fellows. London: MoM; 2011.[20] National Institute for Health and Clinical Excellence (NICE). Donepezil, galantamine, rivastigmine and memantine for thetreatment of Alzheimer's disease. Review of NICE technology appraisal guidance 111. Technology appraisal 217. London: NICE;2011."
		}
	}
	/*
	11
	*/
	action Supportive_information_for_carers{
		agent{GP}
		agent{carer}
		requires {care_plan}

		script{"Quick info:Supporting the carer in their role:• discuss individually tailored interventions, eg [3]:• individual or group psycho-education• peer-support groups tailored to the needs of the carer, eg the stage of dementia of the person being cared for• voluntary/self-help groups, eg the Alzheimer’s Society [1,6]• telephone and internet information and support• training courses on dementia, services and benefits, and dementia-care problem solving• ensure support, eg transport or short-break services, is provided to enable carer participation in interventions [3]• offer advice about wandering and other potentially hazardous behaviours [10]• consider involving other family members, as well as the primary carer [3]• additional support may be needed during decisions about institutionalisation [1]Personal support for the carer:• be sensitive to the effects that caring for a person with dementia may have on carers [6]• depressive illness is common - treatment should be considered where appropriate [6]• offer psychological therapy, eg cognitive behavioural therapy (CBT), with a specialist practitioner to carers who experiencepsychological distress [3]• offer help with communication difficulties by referral to SLT [10]• ensure that carers have access to a range of respite or short-break services [5]:• these should:• meet the needs of both the carer (in terms of location, flexibility, and timeliness) and the person with dementia• include therapeutic activities tailored to the person with dementia• be provided in an environment that meets their needs - in the person’s own home wherever possible• possible services include:• day care• day- and night-sitting• short-term or overnight residential care• longer-term placement• transport to these services should be offered• offer referral for a full assessment of carer's needs from social services - may lead to respite services and supportive day careor other services [6]References:[1] American Psychiatric Association (APA). Practice guideline for the treatment of patients with Alzheimer's disease and otherdementias of late life. Arlington, VA: APA; 2007.[3] Scottish Intercollegiate Guidelines Network (SIGN). Management of patients with dementia. SIGN publication no 86. Edinburgh:SIGN; 2006.[5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in healthand social care. Clinical guideline 42. London: NICE; 2006.[6] Royal College of Psychiatrists (RCPSYCH). Forgetful but not forgotten: assessment and aspects of treatment of people withdementia by a specialist old age psychiatry service. London: RCPSYCH; 2005.[10] Map of Medicine (MoM) Clinical Editorial team and Fellows. London: MoM; 2011."
		}
	}
	/*
	12
	*/
	action Consider_referral_to_memory_assessment_services {
		agent{care_manager}
		requires {patient_record}
		provides {patient_record.referal == "memory assessment services"}
		script{"Quick info:A diagnosis of subtype of dementia should be made by healthcare professionals with expertise in differential diagnosis usinginternational standardised criteria [5].Memory assessment services [5]:• should be the single point of referral for all people with a possible diagnosis of dementia, including people who show signs ofmild cognitive impairment (MCI) - more than 50% of people with MCI later develop dementia• may be provided by a memory assessment clinic or by community mental health teamsConsider referral to a specialist psychiatric service if [6]:• complexity or uncertainty exists about the diagnosis after initial assessment and follow-up• a second opinion is requested by the patient/family• the patient suffers from significant depression and/or psychosis, especially if there is:• no response to treatment• acute distress• there are treatment/medication problems• management is difficult due to:• challenging or risky behaviour• multiple problems• concerns about possible abuse• specialist opinion is needed on issues such as:• financial capacity• driving or similar medico-legal areas, eg ineligibility for donating blood [18]References:[5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in healthand social care. Clinical guideline 42. London: NICE; 2006.[6] Royal College of Psychiatrists (RCPSYCH). Forgetful but not forgotten: assessment and aspects of treatment of people withdementia by a specialist old age psychiatry service. London: RCPSYCH; 2005.[18] UK Blood Transfusion and Tissue Transplantation Services (UK BTTTS). Dementia. London: UK BTTTS; 2011."
		}
	}
	/*
	13
	*/
	action Refer_to_memory_assessment_services {
		agent{memory_assessment_service}
		requires {patient_record.referal == "memory assessment services"}
		provides {patient_record.Confirmed_Dementia_Subtype}
		script{""}
	}
	/*
	14
	*/
	action Diagnostic_Criteria {
		/*requires {}
		requires {}
		provides { }*/
		script{"Quick info:Diagnostic criteria for dementia:• Alzheimer’s disease:• preferred criteria - National Institute of Neurological and Communicative Diseases and Stroke (NINCDS)/Alzheimer'sDisease and Related Disorders Association (ADRDA) [3,5]• alternatives include [5]:• International Classification of Diseases (ICD)-10• Diagnostic and Statistical Manual of Mental Disorders (DSM)-IV• vascular dementia:• preferred criteria - Neuroepidemiology Branch of the National Institute of Neurological Disorders and Stroke (NINDS)-Association Internationale pour la Recherche et l'Enseignement en Neurosciences (AIREN) [3,5]• alternatives include:• ICD-10 [5]• DSM-IV [5]• the Hachinski Ischaemic Scale [3]• dementia with Lewy bodies (DLB) [5]:• International Consensus criteria for DLB• frontotemporal dementia [5]:• Lund-Manchester criteria• NINDS criteria for frontotemporal dementiaReferences:[3] Scottish Intercollegiate Guidelines Network (SIGN). Management of patients with dementia. SIGN publication no 86. Edinburgh:SIGN; 2006.[5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in healthand social care. Clinical guideline 42. London: NICE; 2006."
		}
	}
	/*
	15
	*/
	action Consider_differential_diagnoses_and_comorbidities{
		/*requires {}
		requires {}
		provides { }*/
		script{"Quick info:Consider differential diagnoses and co-morbidities (at the time of diagnosis and then at regular intervals), eg:• depression - cognitive impairment may be the result of severe depression, rather than dementia• psychosis• delirium• amnesic disorder• aphasia• head trauma• substance misuse• focal dementias, eg posterior cortical atrophyThis information was drawn from the following references:[1] American Psychiatric Association (APA). Practice guideline for the treatment of patients with Alzheimer's disease and otherdementias of late life. Arlington, VA: APA; 2007.[3] Scottish Intercollegiate Guidelines Network (SIGN). Management of patients with dementia. SIGN publication no 86. Edinburgh:SIGN; 2006.[5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in healthand social care. Clinical guideline 42. London: NICE; 2006.[12] World Health Organisation (WHO). mhGAP intervention guide for mental, neurological and substance use disorders in nonspecializedhealth settings. Geneva: WHO; 2010."
		}
	}
	/*
	16
	*/
	action Secondary_investigations {
		requires {patient_record}
		script{"Perform non-contrast MRI [11]:• preferred modality for the assessment of dementia• sensitive to:• subtle vascular changes (eg strategic infarcts)• changes that may indicate specific conditions such as:• multiple sclerosis (MS)• prion disease• corticobasal degeneration (CBD)CT scan[5]:• can be used for:• diagnostic evaluation• excluding other cerebral pathologies• establishing the subtype diagnosis• request view of temporal lobe - to determine presence of hippocampal or medial temporal atrophy [10]Multislice CT and coronal MRI may be used to assess hippocampal atrophy to support a clinical diagnosis of AD [11].Assess neurological findings [5]:• normal neurology or non-localised findings may be associated with AD• focal neurology is associated with vascular dementia• Parkinsonism is associated with DLBReferences:[5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in healthand social care. Clinical guideline 42. London: NICE; 2006.[10] Map of Medicine (MoM) Clinical Editorial team and Fellows. London: MoM; 2011.[11] Hort J, O'Brien JT, Gainotti G et al. EFNS guidelines for the diagnosis and management of Alzheimers disease. EuropeanJournal of Neurology 2010; 17: 1236-48."
		}
	}
	/*
	17
	*/
	action Further_investigations{
		requires {patient_record}
		script{"Quick info:Further investigations may include:• functional neuroimaging eg fluorodeoxy-glucose (FDG-) positron emission tomography (PET) or single photon emission CTscan (SPECT) [11]:• may increase diagnostic confidence• can help to differentiate between subtypes of dementia, eg dopaminergic SPECT is useful for differentiating betweenAlzheimer's disease (AD) and dementia with Lewy bodies (DLB)• cerebrospinal fluid (CSF) examination - if Creutzfeld-Jakob disease (CJD) or another form of rapidly progressive dementia issuspected [5]• electroencephalography:• not recommended as a routine investigation [5]• recommended in differential diagnosis of atypical clinical presentations of AD [11]• also consider if the following are suspected:• delirium [5]• frontotemporal dementia [5]• CJD [5]• toxic metabolic disorder [11]• transient epileptic amnesia or other previously unrecognised seizure disorder [11]• brain biopsy:• should only be considered in selected people whose dementia is thought to be a potentially reversible condition thatcannot be diagnosed in any other way [5]• role may increase as disease-modifying therapies become available [11]Reference:[5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in healthand social care. Clinical guideline 42. London: NICE; 2006.[11] Hort J, O'Brien JT, Gainotti G et al. EFNS guidelines for the diagnosis and management of Alzheimers disease. EuropeanJournal of Neurology 2010; 17: 1236-48."
		}
	}
	/*
	18
	*/	
	action Manage_according_to_predominant_cause {
		requires {patient_record}
		provides{patient_record.Dementia_Predominant_Cause}
		script{"Quick info: Many cases of dementia may have mixed pathology, particularly Alzheimer's and vascular dementia. Such cases should be managed according to the condition that is thought to be the predominant cause [5]. Reference:
[5] National Institute for Health and Clinical Excellence (NICE). Dementia: Supporting people with dementia and their carers in health and social care. Clinical guideline 42. London: NICE; 2006."
		}
	}	
	/*
	19
	*/
	action Alzheimer_disease {
		agent{GP}	
		requires {patient_record.Dementia_Predominant_Cause == "Alzheimer's disease"}
		script{"Quick info:Diagnosis of Alzheimer's disease (AD) is primarily based on clinical features, after excluding other systemic and brain disorders thatcould account for the cognitive impairment.Clinical features:• onset is typically insidious - condition progresses slowly over 7-10 years• in the early stages of Alzheimer's disease, people commonly:• have memory lapses - forget the names of people, places, recent events, and things• miss appointments• experience problems with executive function - for example, when performing tasks involving multiple steps, eg preparing ameal• as the disease progresses, other symptoms become apparent, eg difficulties with:• language and communication• planning and decision makingOther commonly reported symptoms include:• apraxia (problems with learned movement)• apathy• agnosia• depression• anxiety or agitation• disinhibition• psychosis, eg delusions, hallucinations• wandering• aggression• incontinence• altered eating habitsA significant minority of people present with atypical symptoms, such as visual problems due to posterior cortical atrophy.This information was drawn from the following references:[1] American Psychiatric Association (APA). Practice guideline for the treatment of patients with Alzheimer's disease and otherdementias of late life. Arlington, VA: APA; 2007.[2] Clinical Knowledge Summaries (CKS). Dementia. Version 1.0. Newcastle upon Tyne: CKS; 2010.[3] Scottish Intercollegiate Guidelines Network (SIGN). Management of patients with dementia. SIGN publication no 86. Edinburgh:SIGN; 2006."
		}
	}
	/*
	20
	*/
	action Vascular_dementia{
		agent{GP}	
		requires {patient_record.Dementia_Predominant_Cause == "Vascular dementia"}

		script{"Quick info:Vascular dementia results from the effects of cerebrovascular disease on cognitive function.The full range of clinical symptoms is not well understood.Clinical features:• tends to present as a series of stepwise increases in the severity of symptoms• often accompanied by focal neurological signs• subcortical ischaemic vascular dementia often presents insidiously with:• slowness and attention problems• personality changes• gait problems• hemiparesis• hemianopia• language and/or swallowing difficulties• urinary incontinence• decreased mobility• balance problems• brain imaging may show:• cortical infarcts• multiple cortical lacunae (holes)• extensive white matter changes• comorbid depression, apathy, or other challenging behaviours may be present• often co-exists with Alzheimer's disease (AD):• both aspects of disease must be managed appropriately• recognition and management of vascular risk factors in those with mixed disease is essentialThis information was drawn from the following references:[1] American Psychiatric Association (APA). Practice guideline for the treatment of patients with Alzheimer's disease and otherdementias of late life. Arlington, VA: APA; 2007.[2] Clinical Knowledge Summaries (CKS). Dementia. Version 1.0. Newcastle upon Tyne: CKS; 2010.[3] Scottish Intercollegiate Guidelines Network (SIGN). Management of patients with dementia. SIGN publication no 86. Edinburgh:SIGN; 2006.[10] Map of Medicine (MoM) Clinical Editorial team and Fellows. London: MoM; 2011."
		}
	}
	/*
	21
	*/
	action Dementia_with_Lewy_bodies_DLB {
		agent{GP}	
		requires {patient_record.Dementia_Predominant_Cause == "Dementia with Lewy bodies (DLB)"}

		script{"Quick info:Dementia with Lewy bodies (DLB) has been recognised clinically only in the last 10-15 years.Clinical features may include:• impairment of executive function• Parkinsonism• visuospatial dysfunction• visual hallucinations• cognitive fluctuations lasting days to weeks• marked sensitivity to extrapyramidal adverse effects of antipsychotic medication• neuroleptic sensitivity• rapid eye movement (REM) sleep behaviour disorder• fainting and falls• problems with swallowing or continence• depression• non-visual hallucinationsThis information was drawn from the following references:[1] American Psychiatric Association (APA). Practice guideline for the treatment of patients with Alzheimer's disease and otherdementias of late life. Arlington, VA: APA; 2007.[2] Clinical Knowledge Summaries (CKS). Dementia. Version 1.0. Newcastle upon Tyne: CKS; 2010.[3] Scottish Intercollegiate Guidelines Network (SIGN). Management of patients with dementia. SIGN publication no 86. Edinburgh:SIGN; 2006."
		}
	}
	/*
	22
	*/
	action Frontotemporal_dementia_FTD{
		agent{GP}	
		requires {patient_record.Dementia_Predominant_Cause == "Alzheimer's disease"}

		script{"Quick info: Frontotemporal dementia (FTD) [2]: • develops insidiously • second most common cause of early-onset dementia (after AD) • presents with: • behavioural disturbances, eg disinhibition or apathy; or: • language disturbance • other cognitive functions are relatively spared in most cases • in some cases, people may develop associated motor neurone disease Reference: [2] Clinical Knowledge Summaries (CKS). Dementia. Version 1.0. Newcastle upon Tyne: CKS; 2010."
		}
	}
	
	
}
	