
  ProgramInt1(
 Module  cm,
 PrCount cp,
 PrCount dp,
 Local   L,
 Local   dL,
 Global  G,
 Global  dG
 )

  // // forward propagation on call transitions
  // | (   s_pc=0
  //     & CopyLocals(s_mod,s_ENTRY_CL,s_CL)    // Copy parameters to this function
  //     & (exists
  //         Module t_mod,
  //         PrCount t_pc,
  //         Local t_CL,
  //         Global t_CG,
  //         Local t_ENTRY_CL,
  //         Global t_ENTRY_CG.
  //         (
  //           (
  //               Reachable(t_mod,t_pc,t_CL,t_CG,t_ENTRY_CL,t_ENTRY_CG)
  //             & CopyGlobals(s_mod,t_CG,s_CG)
  //           )
  //           & CopyGlobals(s_mod, t_CG, s_ENTRY_CG)
  //           & programCall(t_mod,s_mod,t_pc,t_CL,s_CL,t_CG)
  //         )
  //       )
  //  )

  | (true
  & cp = <calling_pc>
  & dp = <next_pc>
  & (exists
      // PrCount t_pc,
      // Global t_CG,
      // Module u_mod,
      PrCount u_pc,
      Local u_ENTRY_CL.
        (
          // exists Local t_CL.
            // (
              true
              // ( // Caller, func call
              //     // Reachable(s_mod,t_pc,t_CL,t_CG,s_ENTRY_CL,s_ENTRY_CG)  // t is reachable
              //     // & SkipCall(s_mod,t_pc,s_pc)
              // )
                // programCall(s_mod,u_mod,t_pc,t_CL,u_ENTRY_CL,t_CG)
              & programCall(cm, <callee_module>, cp, L, u_ENTRY_CL, G)
              // & SetReturnTS(s_mod,u_mod,t_pc,u_pc,t_CL,s_CL,t_CG,s_CG)
              & SetReturnTS(cm, <callee_module>, cp, u_pc, L, dL, G, dG)  // Copy all variables except those in output
           // )
        )
      & ( exists
            Local u_CL,
            Global u_CG.
            (
                (   // Callee
                  // Reachable(u_mod,u_pc,u_CL,u_CG,u_ENTRY_CL,t_CG)        // u is reachable
                  <FunctionName>(<callee_module>, u_pc, u_CL, u_CG, u_ENTRY_CL, G)
                  & Exit(<callee_module>,u_pc)
                )                                       // u is an exit
              // & SetReturnUS(s_mod,u_mod,t_pc,u_pc,u_CL,s_CL,u_CG,s_CG)   // Set all variable following the output
              & SetReturnUS(cm,<callee_module>,cp,u_pc,u_CL,dL,u_CG,dG)   // Set all variable following the output
            )
        )
    )
  )