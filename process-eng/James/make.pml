process make
   {
   action makeAction executable
      {
      agent    {"developer"}
      tool     {"cd out;make"}
      requires {"raw source file(s)"}
      provides {"compiled source file(s)"}
      script   {"execute make to compile raw source file(s)"}
      }
   }
