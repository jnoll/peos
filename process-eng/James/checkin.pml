process checkin
   {
   action checkinAction executable
      {
      agent    {"developer"}
      tool     {"cd /home/$USER;cvs commit"}
      requires {"/home/$USER/peos"}
      provides {"/home/tools/peosrep"}
      script   {"Checkin an instance of project repository \
                 using the following LINUX shell commands... \
                 \
                 cd /home/$USER \
                 cvs commit"}
      }
   }
