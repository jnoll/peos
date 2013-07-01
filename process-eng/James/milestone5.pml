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
         script   {"Start the checkout.pml process model to continue..."}
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
               script   {"Edit file according to deliverable instructions... \
                          http://www.cudenver.edu/~jnoll/5728/deliverables.html \
                          \
                          The intent of the project plan is to capture information \
                          concerning team members roles, file change policies for \
                          the team members, development tasks in relation to \
                          requirements, responsible developer, and estimated time, \
                          and finally dependencies on components or services with \
                          other PEOS pieces or development team members."} 
               }
            action compileDevelopmentPlan manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/plan.html"}
               script   {"Start the make.pml process model to continue..."}
               }
            action verifyDevelopmentPlan manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/plan/$PROJECT/out/plan.html"}
               provides {"/home/$USER/peos/plan/$PROJECT/out/plan.html"}
               script   {"Verify file for spelling and grammatical content..."}
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
               script   {"Edit file according to deliverable instructions... \
                          http://www.cudenver.edu/~jnoll/5728/deliverables.html \
                          \
                          The intent of the project deployment instructions is to \
                          document the build and install procedures for installing \
                          that project's section of the PEOS system.  This shall \
                          include valid environments for operation, installation \
                          instructions, and startup procedures."}
               }
            action compileProjectDeploymentInstructions manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/doc/$PROJECT/out/deployment.html"}
               script   {"Start the make.pml process model to continue..."}
               }
            action verifyProjectDeploymentInstructions manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/doc/$PROJECT/out/deployment.html"}
               provides {"/home/$USER/peos/doc/$PROJECT/out/deployment.html"}
               script   {"Verify file for spelling and grammatical content..."}
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
               script   {"Edit file according to deliverable instructions... \
                          http://www.cudenver.edu/~jnoll/5728/deliverables.html \
                          \
                          The intent of the project user guide is to document how \
                          to use the PEOS system project, once installed."}
               }
            action compileProjectUserGuide manual
               {
               agent    {"developer"}
               tool     {"peos"}
               requires {"make.pml"}
               provides {"/home/$USER/peos/doc/$PROJECT/out/user-guide.html"}
               script   {"Start the make.pml process model to continue..."}
               }
            action verifyProjectUserGuide manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/doc/$PROJECT/out/user-guide.html"}
               provides {"/home/$USER/peos/doc/$PROJECT/out/user-guide.html"}
               script   {"Verify file for spelling and grammatical content..."}
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
               script   {"Create file(s) and/or directorie(s) for project implementation..."}
               }
            action createProjectSourceCodeMakefile manual
               {
               agent    {"developer"}
               tool     {"editor"}
               requires {"/home/$USER/peos/src/$PROJECT/*"}
               provides {"/home/$USER/peos/src/$PROJECT/*"}
               script   {"Create makefile for project implementation..."}
               }
            action createProjectSourceCodeMakefile manual
               {
               agent    {"developer"}
               tool     {"cd /home/$USER/peos/src/$PROJECT;cvs add *"}
               requires {"/home/$USER/peos/src/$PROJECT/*"}
               provides {"/home/$USER/peos/src/$PROJECT/*"}
               script   {"Add file(s) and/or directorie(s) for project implementation..."}
               }
            }
        }

      action milestone5UpdateANDResolve manual
         {
         agent    {"developer"}
         tool     {"peos"} 
         requires {"updateANDresolve.pml"}
         provides {"update and resolve conflictions on an instance of the project repository"}
         script   {"Start the updateANDresolve.pml process model to continue..."}
         }

      action milestone5Checkin manual
         {
         agent    {"developer"}
         tool     {"peos"} 
         requires {"checkin.pml"}
         provides {"checkin an instance of the project repository"}
         script   {"Start the checkin.pml process model to continue..."}
         }
      }
   }
