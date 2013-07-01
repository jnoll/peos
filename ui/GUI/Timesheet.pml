process timesheet {

     action Fill_name {
        requires {time_sheet}
        script {"Fill in your name and other details in the $time_sheet"}
     }
      action Fill_hours {
        requires {time_sheet}
        script {"Fill in the hours in the $time_sheet"}
     }
      action Fill_total_hours {
        requires {time_sheet}
        script {"Fill in total hours in the $time_sheet"}
     }
     action Sign_and_date {
        requires {time_sheet}
        script {"Sign and Date the $time_sheet"}
     }
     action Turn_it_in {
        requires {time_sheet}
        script {"Hand the $time_sheet to your supervisor"}
     }
}
