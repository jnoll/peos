process PostMortem {
	sequence {
		action readIntroduction manual {
			agent { "student" }
			script { "Introduction: This is a take-home examination. You may work individually, or you may collaborate with your group members. You may consult class notes, the text book, journal articles, source code, etc. however, you must cite sources other than your own notes or the materials on our web site." }
		}
		action readInstructions manual {
			agent { "student" }
			script { "This semester we attempted to apply McConnel's staged release model to the development of the course projects. We also discussed some other process models, as well as issues and techniques for analyzing risk, specifying requirements, architectural design, implementing code, and testing. Many projects have made significant progress, others had difficulties. In any case, there is always room for improvement.  Your task is to analyze your project's shortcomings, and suggest how they could have been addressed.  I will use your analysis to determine both whether you learned anything from this class, and to improve subsequent classes. Please take this assignment seriously.  Also, your analysis will be confidential, so if you feel your project suffered from personnel problems, you can discuss this without fear of reprisal from your teammates." }
		}
		action readGrading manual {
			agent { "student" }
			script { "Your answers will be evaluated based on my assessment of their correctness and completeness. I will judge correctness by your supporting arguments and citations, or by what we discussed in class if you don't including supporting arguments or citations. I will judge completeness according to how well you address each of the issues above. However, be concise; don't pad your answer with meaningless verbiage. Additional points will be deducted for the following errors: 1. Spelling: One point deducted for the first misspelled word, two for the second, four for the third, eight for the fourth, etc. Extraneous binary characters will count as misspelled words. 2. Misnamed files: 10 point deduction for not following the naming conventions. 20 point deduction for squashing someone else's submission. 3. Format: 5 point deduction for incorrect file formats (like missing or incorrect headers, too long lines, etc.) Unreadable files will be assigned a score of 0. 4. Late submissions: no credit for late submissions. 5. Plagiarism: score of 0 for the exam. If you use outside sources, cite them. Exception: you may have someone at the UCD language center proofread your document for spelling errors. Please note this at the end of your file if you take advantage of this service (I'm interested to see if anyone does this, and if it works). "}
		}
		action readHints manual {
			agent { "student" }
			script { "Monitor the help mailing list for clarification and send requests for same to the list." }
		}
		iteration {
			iteration {
				action editFinal manual {
					agent { "student" }
					tool { "vi" }
					script { "Edit your final. 1.Identify the three most significant shortcomings of, or problems encountered by, your project. These may include failure to implement required functionality, process problems, personnel conflicts, inadequate infrastructure, or any other facet of your project that could be improved.  2.For each shortcoming, identify a solution or solutions from the readings or class discussion. Cite the source of your solution(s)." }
					provides { "Edited Final Deliverable File" }
				}
				action formatFinal manual {
					agent { "student" }
					tool { "vi" }
					script { "The file to be created for the final deliverable most confrom to the following: 1. One file only. 2. 300 lines maximum. 3. 72 characters maximum per line including the newline character. 4. Every line has a newline character at the end. Beware, Windows programs like Word and Notepad do not put newlines at the end of lines. 5. All answers must be of correctly spelled, college level English ASCII text. 6. First line of the file specifies your student ID, email address, and name in that order. I.e.: '123456789 astudent@ouray.cudenver.edu Andrew Smith'. A script is used to strip off the first line. If the format is not followed exactly, the script may fail to find your name, id, or email address, meaning I may not be able to associate your score with your identity, resulting in a score of zero on the final, and a disappointing grade for the course. 7. Name your file using your first name and first letter of your last name, or first letter of your first name and your last name. Give your file a .txt extension. So, if your name is Andrew Student, submit either andrews.txt or astudent.txt (but not both)." }
					provides { "Final Deliverable File" }
				}
			}
			action submitFinal manual {
				agent { "student" }
				tool { "ftp" }
				script { "ftp oslab-server.cudenver.edu; username = submit, password = CSC5573; cd 5728/final; put <filename>; ls; Make sure you do not submit to the same name as an already existing file.  1. If you see a file with the same name, close this action and return to formatFinal action in order to come up with an alternate file name.  2. If, for some reason, you need to re-submit your file, use the same basename with a digit appended. For example, if your first submission was astudent.txt, the second submission should be astudent2.txt. 3. If your connection gets dropped in the middle of transmitting the contents, resubmit another file following the naming conventions described above." }
				requires { "Final Deliverable File" }
				provides { "Submitted Final" }
			}
		}
	}
}
			
