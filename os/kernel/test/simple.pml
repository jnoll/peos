process simple {
    action act_1 {
	requires {r1}
	provides {r2}
    }
    action act_2 {
	requires {r2}
	provides {r2}
    }
}
