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
         script   {"update an instance of project repository"}
         }
      action resolveAction manual
         {
         agent    {"developer"}
         tool     {"shell or editor"}
         requires {"conflicted file(s)"}
         provides {"deconflicted file(s)"}
         script   {"deconflict files in error"} 
         }
      }
   }
