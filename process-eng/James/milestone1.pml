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
         script   {"checkout an instance of the project repository"}  
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
               script   {"edit file according to deliverable instructions"}
               }
            action compileProjectLog manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/log.html"}
               script   {"compile file using make"}
               }
            action verifyProjectLog manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/out/log.html"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/log.html"}
               script   {"verify file for spelling and grammatical content"}
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
               script   {"edit file according to deliverable instructions"}
               }
            action compileProjectDefinition manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/project.html"}
               script   {"compile file using make"}
               }
            action verifyProjectDefinition manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/out/project.html"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/project.html"}
               script   {"verify file for spelling and grammatical content"}
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
               script   {"edit file according to deliverable instructions"}
               }
            action compileProjectVisionStatement manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/justification.html"}
               script   {"compile file using make"}
               }
            action verifyProjectVisionStatement manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/out/justification.html"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/justification.html"}
               script   {"verify file for spelling and grammatical content"}
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
               script   {"edit file according to deliverable instructions"}
               }
            action compileProjectStakeholders manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/requirements/$PROJECT/out/analysis.html"}
               script   {"compile file using make"}
               }
            action verifyProjectStakeholders manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/requirements/$PROJECT/out/analysis.html"}
               provides {"/home/$USER/peos/requirements/$PROJECT/out/analysis.html"}
               script   {"verify file for spelling and grammatical content"}
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
               script   {"edit file according to deliverable instructions"}
               }
            action compileProjectRisks manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/risks.html"}
               script   {"compile file using make"}
               }
            action verifyProjectRisks manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/out/risks.html"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/risks.html"}
               script   {"verify file for spelling and grammatical content"}
               }
            }
        }

      action milestone1UpdateANDResolve manual
         {
         agent    {"developer"}
         tool     {"peos"} 
         requires {"updateANDresolve.pml"}
         provides {"update and resolve conflictions on an instance of the project repository"}
         script   {"update and resolve conflictions on an instance of the project repository"}  
         }

      action milestone1Checkin manual
         {
         agent    {"developer"}
         tool     {"peos"} 
         requires {"checkin.pml"}
         provides {"checkin an instance of the project repository"}
         script   {"checkin an instance of the project repository"}  
         }
      }
   }
