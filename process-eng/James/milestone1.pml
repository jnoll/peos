process milestone1
  {
  sequence deliverable_steps
     {
     action copy_repository
        {
        requires {"project repository"}
        provides {"copy an instance of the project repository"}
        tool     {"cvs"}
        agent    {"developer"}
        script   {"copy an instance of the project repository"}  
        }

     branch edit
        {
        sequence meta_data 
           {
           action create_meta_data
              {
              requires {"PEOSROOT/plan/PROJECT/project.body"}
              requires {"PEOSROOT/plan/PROJECT/log.body"}
              tool     {"cvs"}
              agent    {"developer"}
              script   {"edit files according to deliverable instructions"}
              }
           action compile_meta_data
              {
              requires {"PEOSROOT/plan/PROJECT/out/Makefile"}
              provides {"PEOSROOT/plan/PROJECT/out/project.html"}
              provides {"PEOSROOT/plan/PROJECT/out/log.html"}
              tool     {"cvs and make"}
              agent    {"developer"}
              script   {"compile files using make"}
              }
           action verify_meta_data
              {
              requires {"PEOSROOT/plan/PROJECT/out/project.html"}
              requires {"PEOSROOT/plan/PROJECT/out/log.html"}
              tool     {"cvs and lynx"}
              agent    {"developer"}
              script   {"verify files for spelling and grammatical content"}
              }
           }
        sequence vision_statement 
           {
           action create_vision_statement
              {
              requires {"PEOSROOT/plan/PROJECT/justification.body"}
              tool     {"cvs"}
              agent    {"developer"}
              script   {"edit file according to deliverable instructions"}
              }
           action compile_vision_statement
              {
              requires {"PEOSROOT/plan/PROJECT/out/Makefile"}
              provides {"PEOSROOT/plan/PROJECT/out/justification.html"}
              tool     {"cvs and make"}
              agent    {"developer"}
              script   {"compile file using make"}
              }
           action verify_vision_statement
              {
              requires {"PEOSROOT/plan/PROJECT/out/justification.html"}
              tool     {"cvs and lynx"}
              agent    {"developer"}
              script   {"verify file for spelling and grammatical content"}
              }
           }
        sequence stakeholders 
            {
            action create_stakeholders
               {
               requires {"PEOSROOT/requirements/PROJECT/analysis.body"}
               tool     {"cvs"}
               agent    {"developer"}
               script   {"edit file according to deliverable instructions"}
               }
            action compile_stakeholders
               {
               requires {"PEOSROOT/requirements/PROJECT/out/Makefile"}
               provides {"PEOSROOT/requirements/PROJECT/out/analysis.html"}
               tool     {"cvs and make"}
               agent    {"developer"}
               script   {"compile file using make"}
               }
            action verify_stakeholders
               {
               requires {"PEOSROOT/requirements/PROJECT/out/analysis.html"}
               tool     {"cvs and lynx"}
               agent    {"developer"}
               script   {"verify file for spelling and grammatical content"}
               }
            }
        sequence risks 
            {
            action create_risks
               {
               requires {"PEOSROOT/plan/PROJECT/risks.body"}
               tool     {"cvs"}
               agent    {"developer"}
               script   {"edit file according to deliverable instructions"}
               }
            action compile_risks
               {
               requires {"PEOSROOT/plan/PROJECT/out/Makefile"}
               provides {"PEOSROOT/plan/PROJECT/out/risks.html"}
               tool     {"cvs and make"}
               agent    {"developer"}
               script   {"compile file using make"}
               }
            action verify_risks
               {
               requires {"PEOSROOT/plan/PROJECT/out/risks.html"}
               tool     {"lynx"}
               agent    {"developer"}
               script   {"verify file for spelling and grammatical content"}
               }
            }
        }

     sequence update 
        {
        action update_repository 
           {
           requires {"copy an instance of the project repository"}
           provides {"copy an instance of the project repository with conflicts denoted"}
           tool     {"cvs"}
           agent    {"developer"}
           script   {"update project repository"}  
           }
        action resolve_conflicts
           {
           requires {"conflicted file(s) and/or directory(ies)"}
           provides {"resolved conflicted file(s) and/or directory(ies)"}
           tool     {"cvs"}
           agent    {"developer"}
           script   {"resolve conflictions using editing processes"}  
           }
        }

     action commit_repository 
        {
        requires {"copy an instance of the project repository"}
        provides {"commit of copy into the project repository"}
        tool     {"cvs"}
        agent    {"developer"}
        script   {"commit project repository"}  
        }
     }
  }
