process p {
  action act_1 {
     requires {"file_1"}
     provides {"file_2"}
   }

  action act_2 {
     requires {"file_2"}
     provides {"file_3"}
   }

  action act_3 {
     requires {"file_3"}
     provides {"file_4"}
   }

  action act_4 {
     requires {"file_4"}
     provides {"file_5"}
   }
}

