process milestone1
   {
   sequence milestone1Steps
      {
      action milestone1Checkout manual
         {
         agent    {"developer"}
         tool     {"peos"} 
         requires {"checkout.pml"}
         provides {"checkout an instance of the project repository"}
         script   {"Start the checkout.pml process model to continue..."} 
         }

      branch milestone1Edit 
         {
         sequence projectLog 
            {
            action createProjectLog manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/log.body"}
               provides {"/home/$USER/peos/plan/$PROJECT/log.body"}
               script   {"Edit file according to deliverable instructions... \
                          http://www.cudenver.edu/~jnoll/5728/deliverables.html \
                          \
                          The intent of the project log is to capture information \
                          that does not fit into one of the other documents.  Use \
                          this file to record important decisions taken, useful bits \
                          of information and references to potentially useful external \
                          documents, as well as code defect data and actual effort \
                          expended on the project."}
               }
            action compileProjectLog manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/log.html"}
               script   {"Start the make.pml process model to continue..."} 
               }
            action verifyProjectLog manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/out/log.html"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/log.html"}
               script   {"Verify file for spelling and grammatical content..."}
               }
            }
         sequence projectDefinition 
            {
            action createProjectDefinition manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/project.body"}
               provides {"/home/$USER/peos/plan/$PROJECT/project.body"}
               script   {"Edit file according to deliverable instructions... \
                          http://www.cudenver.edu/~jnoll/5728/deliverables.html \
                          \
                          The intent of the project definition is to define the \
                          project name and team members."}
               }
            action compileProjectDefinition manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/project.html"}
               script   {"Start the make.pml process model to continue..."} 
               }
            action verifyProjectDefinition manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/out/project.html"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/project.html"}
               script   {"Verify file for spelling and grammatical content..."}
               }
            }
         sequence projectVisionStatement 
            {
            action createProjectVisionStatement manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/justification.body"}
               provides {"/home/$USER/peos/plan/$PROJECT/justification.body"}
               script   {"Edit file according to deliverable instructions... \
                          http://www.cudenver.edu/~jnoll/5728/deliverables.html \
                          \
                          The intent of the project vision statement is to define \
                          the objectives of the project and the estimated amount \
                          of effort for the team members to achieve those objectives."}
               }
            action compileProjectVisionStatement manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/justification.html"}
               script   {"Start the make.pml process model to continue..."} 
               }
            action verifyProjectVisionStatement manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/out/justification.html"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/justification.html"}
               script   {"Verify file for spelling and grammatical content..."}
               }
            }
         sequence projectStateholders 
            {
            action createProjectStakeholders manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/requirements/$PROJECT/analysis.body"}
               provides {"/home/$USER/peos/requirements/$PROJECT/analysis.body"}
               script   {"Edit file according to deliverable instructions... \
                          http://www.cudenver.edu/~jnoll/5728/deliverables.html \
                          \
                          The intent of the project stakeholder identification is \
                          to define the project stakeholders and their win conditions \
                          in relations to project requirements."}
               }
            action compileProjectStakeholders manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/requirements/$PROJECT/out/analysis.html"}
               script   {"Start the make.pml process model to continue..."} 
               }
            action verifyProjectStakeholders manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/requirements/$PROJECT/out/analysis.html"}
               provides {"/home/$USER/peos/requirements/$PROJECT/out/analysis.html"}
               script   {"Verify file for spelling and grammatical content..."}
               }
            }
         sequence projectRisks 
            {
            action createProjectRisks manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/risks.body"}
               provides {"/home/$USER/peos/plan/$PROJECT/risks.body"}
               script   {"Edit file according to deliverable instructions... \
                          http://www.cudenver.edu/~jnoll/5728/deliverables.html \
                          \
                          The intent of the project risks identification is to \
                          define and rank the potential risks in reference to the \
                          project objectives."}
               }
            action compileProjectRisks manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/risks.html"}
               script   {"Start the make.pml process model to continue..."} 
               }
            action verifyProjectRisks manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/out/risks.html"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/risks.html"}
               script   {"Verify file for spelling and grammatical content..."}
               }
            }
        }

      action milestone1UpdateANDResolve manual
         {
         agent    {"developer"}
         tool     {"peos"} 
         requires {"updateANDresolve.pml"}
         provides {"update and resolve conflictions on an instance of the project repository"}
         script   {"Start the updateANDresolve.pml process model to continue..."} 
         }

      action milestone1Checkin manual
         {
         agent    {"developer"}
         tool     {"peos"} 
         requires {"checkin.pml"}
         provides {"checkin an instance of the project repository"}
         script   {"Start the checkin.pml process model to continue..."} 
         }
      }
   }
