process p {
  action a0 {
      requires {r0}
      provides {r1}
  }
  action a1 {
      requires {r2 && r3}
      provides {r4 && r5}
  }
}
