
FunctionName(
  Module  s_mod,
  PrCount s_pc,
  Local   s_CL,
  Global  s_G,
  Local   s_ENTRY_CL,
  Global  s_ENTRY_CG
 )
  s_mod   <   s_pc,
  s_pc    <   s_CL,
  s_CL    ~+  s_ENTRY_CL,
  s_CL    <   s_G,
  s_G    ~+  s_ENTRY_CG
(
  // forward propagation on call transitions
  | (   s_pc=0
      & s_mod = <module_name>
      & CopyLocals(s_mod, s_ENTRY_CL, s_CL)    // Copy parameters to this function
      & CopyGlobals(s_mod, s_G, s_ENTRY_CG)
    )

  |  (
        s_mod = <module_name>
      & (exists                  // There exists an internal state that
           PrCount t_pc,
           Local   t_CL,
           Global  t_G.
           (
                FunctionName( s_mod, t_pc, t_CL, t_G, s_ENTRY_CL, s_ENTRY_CG)
              & (
                  (
                    programInt1( <module_name>, t_pc, s_pc, t_CL, s_CL, t_G, s_G)      // Assignment related
                    & CopyVariables_ProgramInt( <module_name>, t_pc, t_CL, s_CL, t_G, s_G)   //  Copy others global variable
                  )
                  | programInt3(<module_name>, t_pc, s_pc, t_CL, s_CL, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
        s_mod = <module_name>
       & (exists PrCount t_pc.
             (     FunctionName( s_mod, t_pc, s_CL, s_G, s_ENTRY_CL, s_ENTRY_CG)
                 & programInt2( <module_name>, t_pc, s_pc, s_CL, s_G)    // Control flow statement
             )
         )
    )

);