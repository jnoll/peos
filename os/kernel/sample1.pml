process sample1 {
       iteration i {
         action a {
           requires {"x"}
           provides {"y"}
          }
         action b {
          requires {c.modified == "true"}
          provides {d.modified == "false"}
          }
           }
         action c {}
            
          }

