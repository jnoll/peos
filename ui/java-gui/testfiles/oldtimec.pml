process timesheet {

     action Fill_name {
        requires {time_sheet}
	provides {good_times}
        script {"Fill in your name and other details in the $time_sheet. Make sure you have $good_times"}
     }
      action Fill_hours {
        requires {time_sheet}
        script {"Fill in the hours in the $time_sheet"}
     }
      action Fill_total_hours {
        requires {time_sheet}
        script {"THE $time_sheet DEMANDS THAT YOU Fill in total hours. I hope you are having $good_times"}
     }
     action Sign_and_date {
        requires {time_sheet}
        script {"Sign and Date the timesheet"}
     }
     action Turn_it_in {
        requires {time_sheet}
        script {"Hand it to your supervisor"}
     }
}
