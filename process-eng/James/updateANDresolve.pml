process updateANDresolve
   {
   sequence updateANDresolveSequence
      {
      action updateAction executable
         {
         agent    {"developer"}
         tool     {"cd /home/$USER;cvs update"}
         requires {"/home/$USER/peos"}
         provides {"/home/$USER/peos"}
         script   {"Update an instance of project repository \
                    using the following LINUX shell commands... \
                    \
                    cd /home/$USER \
                    cvs update"}
         }
      action resolveAction manual
         {
         agent    {"developer"}
         tool     {"shell or editor"}
         requires {"conflicted file(s)"}
         provides {"deconflicted file(s)"}
         script   {"Deconflict files in error using a LINUX shell or editor..."} 
         }
      }
   }
