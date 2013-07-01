process checkout
   {
   action checkoutAction executable
      {
      agent    {"developer"}
      tool     {"cd /home/$USER;rm -r peos;cvs -d /home/tools/peosrep checkout peos"}
      requires {"/home/tools/peosrep"}
      provides {"/home/$USER/peos"}
      script   {"Checkout an instance of project repository \
                 using the following LINUX shell commands... \
                 \
                 cd /home/$USER \
                 rm -r peos \
                 cvs -d /home/tools/peosrep checkout peos"}
      }
   }
