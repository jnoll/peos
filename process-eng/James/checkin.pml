process checkin
   {
   action checkinAction executable
      {
      agent    {"developer"}
      tool     {"cd /home/$USER;cvs commit"}
      requires {"/home/$USER/peos"}
      provides {"/home/tools/peosrep"}
      script   {"checkin an instance of project repository"}
      }
   }
