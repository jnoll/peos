process p {

action test1 {
     requires {(abstract) r1}
     provides {r2}
     script {"test_script"} 
 }

action test2 {
     requires {r3}
     provides {(new) r4}
     script {"test_script"} 
 }

}
