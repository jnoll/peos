process milestone5
   {
   sequence milestone5Steps
      {
      action milestone5Checkout manual
         {
         agent    {"developer"}
         tool     {"peos"} 
         requires {"checkout.pml"}
         provides {"checkout an instance of the project repository"}
         script   {"checkout an instance of the project repository"}  
         }

      branch milestone5Edit 
         {
         sequence projectDevelopmentPlan 
            {
            action createDevelopmentPlan manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/plan.body"}
               provides {"/home/$USER/peos/plan/$PROJECT/plan.body"}
               script   {"edit file according to deliverable instructions"}
               }
            action compileDevelopmentPlan manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/plan.html"}
               script   {"compile file using make"}
               }
            action verifyDevelopmentPlan manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/out/plan.html"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/plan.html"}
               script   {"verify file for spelling and grammatical content"}
               }
            }
         sequence projectDeploymentInstructions 
            {
            action createProjectDeploymentInstructions manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/doc/$PROJECT/deployment.body"}
               provides {"/home/$USER/peos/doc/$PROJECT/deployment.body"}
               script   {"edit file according to deliverable instructions"}
               }
            action compileProjectDeploymentInstructions manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/doc/$PROJECT/out/deployment.html"}
               script   {"compile file using make"}
               }
            action verifyProjectDeploymentInstructions manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/doc/$PROJECT/out/deployment.html"}
               provides {"/home/$USER/peos/doc/$PROJECT/out/deployment.html"}
               script   {"verify file for spelling and grammatical content"}
               }
            }
         sequence projectUserGuide 
            {
            action createProjectUserGuide manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/doc/$PROJECT/user-guide.body"}
               provides {"/home/$USER/peos/doc/$PROJECT/user-guide.body"}
               script   {"edit file according to deliverable instructions"}
               }
            action compileProjectUserGuide manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/doc/$PROJECT/out/user-guide.html"}
               script   {"compile file using make"}
               }
            action verifyProjectUserGuide manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/doc/$PROJECT/out/user-guide.html"}
               provides {"/home/$USER/peos/doc/$PROJECT/out/user-guide.html"}
               script   {"verify file for spelling and grammatical content"}
               }
            }
         sequence projectSourceCode 
            {
            action createProjectSourceCode manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/src/$PROJECT/*"}
               provides {"/home/$USER/peos/src/$PROJECT/*"}
               script   {"create file(s) for project implementation"}
               }
            action createProjectSourceCodeMakefile manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/src/$PROJECT/*"}
               provides {"/home/$USER/peos/src/$PROJECT/*"}
               script   {"create makefile for project implementation"}
               }
            action createProjectSourceCodeMakefile manual
               {
               agent    {"developer"}
               tool     {"cd /home/$USER/peos/src/$PROJECT;cvs add *"}
               requires {"/home/$USER/peos/src/$PROJECT/*"}
               provides {"/home/$USER/peos/src/$PROJECT/*"}
               script   {"add file(s) for project implementation"}
               }
            }
        }

      action milestone5UpdateANDResolve manual
         {
         agent    {"developer"}
         tool     {"peos"} 
         requires {"updateANDresolve.pml"}
         provides {"update and resolve conflictions on an instance of the project repository"}
         script   {"update and resolve conflictions on an instance of the project repository"}  
         }

      action milestone5Checkin manual
         {
         agent    {"developer"}
         tool     {"peos"} 
         requires {"checkin.pml"}
         provides {"checkin an instance of the project repository"}
         script   {"checkin an instance of the project repository"}  
         }
      }
   }
